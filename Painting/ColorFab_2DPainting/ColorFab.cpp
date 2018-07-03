#include "ColorFab.h"

#include <opencv2/opencv.hpp>

#include <QDesktopWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QFileDialog>
#include <QtGui/QCloseEvent>
#include <QtCore/QMetaObject>

#include "Config.h"

ColorFab::ColorFab(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ColorFabClass)
	, realSenseCamera(NULL)
	, pointGreyCamera(NULL)
	, calibration(NULL)
	, renderHelper(NULL)
	, rendering(NULL)
	, stream(NULL)
	, cameraViewer(NULL)
	, screen(NULL)
	, serialController(NULL)
	, transactionCount(NULL)
	, commandStatus(NULL)
{
	ui->setupUi(this);

	// 0. Check CUDA available or not
	if (cv::cuda::getCudaEnabledDeviceCount() <= 0)
		QMessageBox::information(0, tr("System Tricker"), tr("No GPU/CUDA available."), QMessageBox::Ok, QMessageBox::NoButton);
	else
		std::cout << "[CUDA AVAILABLE] " << cv::cuda::getCudaEnabledDeviceCount() << std::endl;

	// 1. Setup instance(s)
	realSenseCamera = new RealSense();
	pointGreyCamera = new PointGreyCamera();
	this->isCameraConnected = false;
	calibration = new Calibration();
	this->isCamProCalibrated = calibration->LoadCalibrateFile(def::CALIB_CAMTOPRO);
	renderHelper = new RenderHelper();
	rendering = new Rendering(renderHelper, 0);
	stream = new Stream(renderHelper);
	this->isConnecting = false;

	cameraViewer = new CameraViewer(this);
	this->isCameraViewVisible = false;
	screen = new Screen(this);
	this->isProjecting = false;
	this->projectionFile = QString::null;

	serialController = new SerialController();
	transactionCount = 0;
	commandStatus = 0;

	paintingUI = new PaintingUI(this);

	// 2. Setup GUI and connections
	this->SetupGUI();

	// 3. Setup ProCam Calibration connections
	connect(ui->buttonCalibCamToPro_input, SIGNAL(released()), this, SLOT(SetManualCalibrationProCam()));
	connect(this, SIGNAL(IsRequestCamProCalibrate(bool)), cameraViewer, SLOT(UpdateIsRequestCamProCalibrate(bool)));
	connect(this, SIGNAL(IsRequestCamProCalibrate(bool)), screen, SLOT(UpdateIsRequestCamProCalibrate(bool)));
	connect(cameraViewer, SIGNAL(MousePressPosition(int, int, int)), this, SLOT(OnScreenCalibCamPos(int, int, int)));
	connect(screen, SIGNAL(MousePressPosition(int, int, int)), this, SLOT(OnScreenCalibProPos(int, int, int)));
	connect(ui->buttonCalibCamToPro_exec, SIGNAL(released()), this, SLOT(CalCamProCalibrationOnScreen()));

	// 4. Setup CameraView connections
	connect(this, SIGNAL(IsCameraConnected(bool)), cameraViewer, SLOT(UpdateIsCameraConnected(bool)));
	connect(cameraViewer, SIGNAL(IsWidgetVisible(bool)), this, SLOT(UpdateIsCameraViewVisible(bool)));
	connect(realSenseCamera, SIGNAL(SetSize(int, int)), this, SLOT(UpdateViewSizeOnScreen(int, int)));
	connect(pointGreyCamera, SIGNAL(SetSize(int, int)), this, SLOT(UpdateViewSizeOnScreen(int, int)));

	// 5. Setup data connections
	qRegisterMetaType<cv::Mat>("cv::Mat");
	connect(realSenseCamera, SIGNAL(SetFrame(cv::Mat)), stream, SLOT(SetCameraFrame(cv::Mat)), Qt::QueuedConnection);
	connect(pointGreyCamera, SIGNAL(SetFrame(cv::Mat)), stream, SLOT(SetCameraFrame(cv::Mat)), Qt::QueuedConnection);
	connect(stream, SIGNAL(UpdateCaptureFrameToViewer(QImage)), cameraViewer, SLOT(UpdateFrame(QImage)), Qt::DirectConnection);
	connect(stream, SIGNAL(UpdateCaptureFrameToViewer(QImage)), this, SLOT(GetCameraFrame(QImage)), Qt::DirectConnection);
	connect(realSenseCamera, SIGNAL(SetSize(int, int)), cameraViewer, SLOT(UpdateCapturedSize(int, int)));
	connect(pointGreyCamera, SIGNAL(SetSize(int, int)), cameraViewer, SLOT(UpdateCapturedSize(int, int)));
	connect(this, SIGNAL(IsHomographyTransformChanged(cv::Mat, int)), stream, SLOT(SetHomographyTransform(cv::Mat, int)));

	// 6. Projection
	connect(this, SIGNAL(DoProjection(bool)), stream, SLOT(StartProjection(bool)));
	connect(this, SIGNAL(UpdateProjectionFile(QString)), stream, SLOT(UpdateProjectionFile(QString)));

	// 7. UV Chamber controller (Arduino controller)
	connect(ui->buttonFanOn, SIGNAL(released()), this, SLOT(HandleFanOn()));
	connect(ui->buttonFanOff, SIGNAL(released()), this, SLOT(HandleFanOff()));
	connect(ui->buttonTableManualOn, SIGNAL(released()), this, SLOT(HandleTableManualOn()));
	connect(ui->buttonTableAutoOn, SIGNAL(released()), this, SLOT(HandleTableAutoOn()));
	connect(ui->buttonTableOff, SIGNAL(released()), this, SLOT(HandleTableOff()));
	connect(this, &ColorFab::SetSerialRequest, serialController, &SerialController::Request);
	connect(serialController, &SerialController::Error, this, &ColorFab::UpdateSerialStatus);

	// 8. Painting UI
	connect(ui->actionLoadPaintingUI, SIGNAL(triggered()), this, SLOT(HandlePaintingUI()));

	// 9. Setup update timer
	connect(stream, SIGNAL(SetTimer(QString)), this, SLOT(OnScreenDisplayTimer(QString)));

	// 10. License
	connect(ui->actionLicense, SIGNAL(triggered()), this, SLOT(License()));

	// 11. Shutdown
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->buttonQuit, SIGNAL(released()), this, SLOT(close()));
}

ColorFab::~ColorFab()
{
	delete ui;

	if (realSenseCamera != NULL) delete realSenseCamera;
	if (pointGreyCamera != NULL) delete pointGreyCamera;
	if (calibration != NULL) delete calibration;
	if (stream != NULL) delete stream;

	if (cameraViewer != NULL) delete cameraViewer;
	if (screen != NULL) delete screen;
	if (serialController != NULL) delete serialController;
	if (paintingUI != NULL) delete paintingUI;
}

void ColorFab::SetupGUI()
{
	// Set default setup tab
	ui->tabWidgetSetting->setCurrentIndex(0);

	// Set camera model(s) ui
	ui->comboBoxCameraModel->addItem("Intel RealSense SR300", QVariant(def::REALSENSE_SR300));
	ui->comboBoxCameraModel->addItem("PointGrey Chameleon3", QVariant(def::POINTGREY_CHAMELEON3));
	ui->comboBoxCameraModel->setEnabled(true);
	ui->comboBoxCameraModel->setCurrentIndex(1);
	// Set camera configuration ui
	ui->checkBoxCameraFlipMode->setChecked(false);

	// Set screen ui
	// -- Validation
	ui->lineEditScreenW->setInputMask("D000");
	ui->lineEditScreenH->setInputMask("D000");
	// -- Value
	ui->lineEditScreenW->setText("1024");
	ui->lineEditScreenH->setText("768");

	// Set available screen number ui
	// -- Validation
	for (int i = 0; i < QApplication::desktop()->numScreens(); i++)
		ui->comboBoxScreenNumber->addItem(QString::number(i), QVariant(i));
	// -- Value
	ui->comboBoxScreenNumber->setCurrentIndex(QApplication::desktop()->numScreens() - 1);

	// Set Capture image ui
	// -- Manual
	ui->buttonManualCapture->setEnabled(false);

	// Set Projection image ui
	// -- Manual
	ui->tabManualProjection->setEnabled(true);
	ui->labelProjectionFileParameter->setText("(Not Select)");

	// Set arduino controller button
	ui->buttonFanOff->setEnabled(false);
	ui->buttonFanOn->setEnabled(false);
	ui->buttonTableOff->setEnabled(false);
	ui->buttonTableAutoOn->setEnabled(false);
	ui->buttonTableManualOn->setEnabled(false);

	// Set connections
	// Button
	connect(ui->buttonConnect, SIGNAL(released()), this, SLOT(Connect()));
	connect(ui->buttonProjection, SIGNAL(released()), this, SLOT(Projection()));
	connect(ui->buttonManualCapture, SIGNAL(released()), this, SLOT(SaveCameraFrame()));
	connect(ui->buttonSelectProjectionFile, SIGNAL(released()), this, SLOT(SetProjectionFile()));

	// Flip image
	connect(ui->checkBoxCameraFlipMode, SIGNAL(stateChanged(int)), realSenseCamera, SLOT(DoFlipImage(int)));
	connect(ui->checkBoxCameraFlipMode, SIGNAL(stateChanged(int)), pointGreyCamera, SLOT(DoFlipImage(int)));

	return;
}

void ColorFab::Connect()
{
	/* CONNECT TO CAMERA */
	// IF CAMERA IS NOT CONNECTED, DO CONNECT. IF ITS CONNECT, DO DISCONNECT!
	if (!isCameraConnected)
	{
		// Set Disable GUI
		ui->groupCameraSetup->setEnabled(false);
		// Setup Camera Parameter(s) and initialize
		if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
		{
			realSenseCamera->Setup(60, ui->checkBoxCameraFlipMode->isChecked());
			this->isCameraConnected = realSenseCamera->Initial();
		}
		else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
		{
			pointGreyCamera->Setup(30, 0.0, 0.90, true, true, ui->checkBoxCameraFlipMode->isChecked());
			this->isCameraConnected = pointGreyCamera->Initial();
		}
		emit IsCameraConnected(isCameraConnected);

		if (isCameraConnected)
		{
			// Set Enable GUI
			ui->buttonCalibCamToPro_input->setEnabled(true);

			// Load calibration file (if any)
			this->isCamProCalibrated = calibration->LoadCalibrateFile(def::CALIB_CAMTOPRO);
			// Set calibration points on screen
			if (isCamProCalibrated)
			{
				ui->pCalibCam_x1->setText(QString::number(calibration->camPts[0].x));
				ui->pCalibCam_y1->setText(QString::number(calibration->camPts[0].y));
				ui->pCalibCam_x2->setText(QString::number(calibration->camPts[1].x));
				ui->pCalibCam_y2->setText(QString::number(calibration->camPts[1].y));
				ui->pCalibCam_x3->setText(QString::number(calibration->camPts[2].x));
				ui->pCalibCam_y3->setText(QString::number(calibration->camPts[2].y));
				ui->pCalibCam_x4->setText(QString::number(calibration->camPts[3].x));
				ui->pCalibCam_y4->setText(QString::number(calibration->camPts[3].y));

				ui->pCalibPro_x1->setText(QString::number(calibration->proPts[0].x));
				ui->pCalibPro_y1->setText(QString::number(calibration->proPts[0].y));
				ui->pCalibPro_x2->setText(QString::number(calibration->proPts[1].x));
				ui->pCalibPro_y2->setText(QString::number(calibration->proPts[1].y));
				ui->pCalibPro_x3->setText(QString::number(calibration->proPts[2].x));
				ui->pCalibPro_y3->setText(QString::number(calibration->proPts[2].y));
				ui->pCalibPro_x4->setText(QString::number(calibration->proPts[3].x));
				ui->pCalibPro_y4->setText(QString::number(calibration->proPts[3].y));

				// Set ROI
				if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
				{
					realSenseCamera->SetCameraROI(calibration->camPts[0],
												  calibration->camPts[1],
												  calibration->camPts[2],
												  calibration->camPts[3]);
				}
				else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
				{
					pointGreyCamera->SetCameraROI(calibration->camPts[0],
												  calibration->camPts[1],
												  calibration->camPts[2],
												  calibration->camPts[3]);
				}
			}
			// Start Capturing camera image
			if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
			{
				if (!realSenseCamera->isRunning()) realSenseCamera->start();
			}
			else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
			{
				if (!pointGreyCamera->isRunning()) pointGreyCamera->start();
			}
		}
	}

	if (!isConnecting)
	{
		// Set Enable GUI
		ui->buttonProjection->setEnabled(true);
		ui->lineEditScreenW->setReadOnly(true);
		ui->lineEditScreenH->setReadOnly(true);
		ui->buttonManualCapture->setEnabled(true);

		// Initialize stream parameters
		if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
		{
			stream->Initialize(calibration->CamToPro_Matrix,
							   ui->lineEditScreenW->text().toInt(),
							   ui->lineEditScreenH->text().toInt(),
							   this->projectionFile,
							   cv::Point2f((float)realSenseCamera->cameraROI.x, (float)realSenseCamera->cameraROI.y));
		}
		else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
		{
			stream->Initialize(calibration->CamToPro_Matrix,
							   ui->lineEditScreenW->text().toInt(),
							   ui->lineEditScreenH->text().toInt(),
							   this->projectionFile,
							   cv::Point2f((float)pointGreyCamera->cameraROI.x, (float)pointGreyCamera->cameraROI.y));
		}

		// Initialize rendering
		rendering->Initialize(ui->lineEditScreenW->text().toInt(), ui->lineEditScreenH->text().toInt());

		// Start streaming
		if (!stream->isRunning())
			stream->start();

		// Show cameraView
		if (!isCameraViewVisible)
		{
			this->isCameraViewVisible = true;
			cameraViewer->show();
		}

		// Connect Serial Controller
		SerialConnect();

		// Change connected button text to 'disconnected'
		ui->buttonConnect->setText("Disconnect");

		this->isConnecting = true;
	}
	else
	{
		// Stop streaming thread (if it is not stop when 'stop' is activated)
		if (stream->isRunning())
		{
			stream->stop();
			stream->wait();
		}

		// Stop prjection
		if (this->isProjecting)
		{
			this->isProjecting = false;
			emit DoProjection(isProjecting);
		}

		// Disconnect Serial Controller
		SerialDisconnect();

		// Stop screening thread
		if (screen->isVisible())
			screen->close();

		// Stop capting thread and disconnect from camera
		if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
		{
			if (realSenseCamera->isRunning())
			{
				realSenseCamera->stop();
				realSenseCamera->wait();
			}
			isCameraConnected = realSenseCamera->Disconnect();
		}
		else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
		{
			if (pointGreyCamera->isRunning())
			{
				pointGreyCamera->stop();
				pointGreyCamera->wait();
			}
			isCameraConnected = pointGreyCamera->Disconnect();
		}

		// Close camera viewer
		if (!isCameraConnected)
		{
			if (isCameraViewVisible)
				cameraViewer->close();

			//Setup Enable/Disable GUI
			ui->groupCameraSetup->setEnabled(true);
			ui->groupCamToProCalibration->setEnabled(true);
			ui->buttonCalibCamToPro_input->setEnabled(false);
		}
		// Enable select screen number
		ui->comboBoxScreenNumber->setEnabled(true);

		ui->lineEditScreenW->setReadOnly(false);
		ui->lineEditScreenH->setReadOnly(false);
		ui->buttonProjection->setEnabled(false);
		ui->buttonManualCapture->setEnabled(false);
		ui->buttonConnect->setText("Connect");
		ui->buttonProjection->setText("Projection");
		ui->statusBar->showMessage("Disconnected");
		emit IsCameraConnected(isCameraConnected);

		this->isConnecting = false;
	}

	return;
}

void ColorFab::Projection()
{
	// Notice if the cam-pro is not calibrated, then return no-respondence
	if (!isCamProCalibrated)
	{
		QMessageBox::information(0,
			tr("Calibration Process is inactivate"),
			tr("Please Calibrate the camera-projector before start the process."),
			QMessageBox::Ok,
			QMessageBox::NoButton);

		return;
	}

	// This is to start the projection
	if (!this->isProjecting)
	{
		// Setup stream connections
		connect(stream, SIGNAL(SetOffScreenRendering()), rendering, SLOT(UpdateOffScreenRendering()), Qt::AutoConnection);
		connect(rendering, SIGNAL(SetRendering(QImage)), screen, SLOT(UpdateRendering(QImage)), Qt::DirectConnection);

		// Start image processing module, change GUI status
		ui->buttonProjection->setText("Projecting...");
		ui->buttonProjection->setEnabled(false);
		ui->buttonConnect->setEnabled(true);
		ui->groupCamToProCalibration->setEnabled(false);

		// Disable select screen number
		ui->comboBoxScreenNumber->setEnabled(false);
		// Setup 'screen' geometry to second screen
		int temp_ScreenNumber = ui->comboBoxScreenNumber->itemData(ui->comboBoxScreenNumber->currentIndex()).toInt();
		screen->setGeometry(QApplication::desktop()->screenGeometry(temp_ScreenNumber));

		// Check the size of screen and its geometry
		if (screen->size().width() != ui->lineEditScreenW->text().toInt() &&
			screen->size().height() != ui->lineEditScreenH->text().toInt())
			screen->resize(ui->lineEditScreenW->text().toInt(), ui->lineEditScreenH->text().toInt());

		// Show projection screen
		screen->showFullScreen();

		this->isProjecting = true;
		emit DoProjection(isProjecting);
	}
}

void ColorFab::SerialConnect()
{
	// This function trigger from 'CONNECT' button
	serialController->transaction(ui->inPortName->text().toLatin1().data(), ARDUINO_WAIT_TIME, "0");

	// Set Enable/Disable button
	ui->buttonFanOff->setEnabled(true);
	ui->buttonFanOn->setEnabled(true);
	ui->buttonTableOff->setEnabled(true);
	ui->buttonTableAutoOn->setEnabled(true);
	ui->buttonTableManualOn->setEnabled(true);
	ui->outPortStatus->setText(tr("Running"));

	return;
}

void ColorFab::SerialDisconnect()
{
	// This function trigger from 'CONNECT' button
	// -- Check the serial is working or not (if so, we have to turn it off first)
	if (commandStatus != 0)
	{
		emit SetSerialRequest("0");
		commandStatus = 0;
	}
	// -- Then stop the device
	serialController->stop();

	// Set Enable/Disable button
	ui->buttonFanOff->setEnabled(false);
	ui->buttonFanOn->setEnabled(false);
	ui->buttonTableOff->setEnabled(false);
	ui->buttonTableAutoOn->setEnabled(false);
	ui->buttonTableManualOn->setEnabled(false);
	ui->outPortStatus->setText(tr("Not Running"));

	return;
}

void ColorFab::HandleFanOn()
{
	if (commandStatus == 0)
	{
		emit SetSerialRequest("1");
		commandStatus = 1;
	}
	else if (commandStatus == 1)
	{
		emit SetSerialRequest("1");
		commandStatus = 1;
	}
	else if (commandStatus == 2)
	{
		emit SetSerialRequest("3");
		commandStatus = 3;
	}
	else if (commandStatus == 3)
	{
		emit SetSerialRequest("3");
		commandStatus = 3;
	}
	else if (commandStatus == 4)
	{
		emit SetSerialRequest("5");
		commandStatus = 5;
	}

	return;
}

void ColorFab::HandleFanOff()
{
	if (commandStatus == 0)
	{
		emit SetSerialRequest("0");
		commandStatus = 0;
	}
	else if (commandStatus == 1)
	{
		emit SetSerialRequest("0");
		commandStatus = 0;
	}
	else if (commandStatus == 2)
	{
		emit SetSerialRequest("2");
		commandStatus = 2;
	}
	else if (commandStatus == 3)
	{
		emit SetSerialRequest("2");
		commandStatus = 2;
	}
	else if (commandStatus == 4)
	{
		emit SetSerialRequest("4");
		commandStatus = 4;
	}
	else if (commandStatus == 5)
	{
		emit SetSerialRequest("4");
		commandStatus = 4;
	}

	return;
}

void ColorFab::HandleTableAutoOn()
{
	if (commandStatus == 0)
	{
		emit SetSerialRequest("2");
		commandStatus = 2;
	}
	else if (commandStatus == 1)
	{
		emit SetSerialRequest("3");
		commandStatus = 3;
	}
	else if (commandStatus == 2)
	{
		emit SetSerialRequest("2");
		commandStatus = 2;
	}
	else if (commandStatus == 3)
	{
		emit SetSerialRequest("3");
		commandStatus = 3;
	}
	else if (commandStatus == 4)
	{
		emit SetSerialRequest("2");
		commandStatus = 2;
	}
	else if (commandStatus == 5)
	{
		emit SetSerialRequest("3");
		commandStatus = 3;
	}

	return;
}

void ColorFab::HandleTableManualOn()
{
	if (commandStatus == 0)
	{
		emit SetSerialRequest("4");
		commandStatus = 4;
	}
	else if (commandStatus == 1)
	{
		emit SetSerialRequest("5");
		commandStatus = 5;
	}
	else if (commandStatus == 2)
	{
		emit SetSerialRequest("4");
		commandStatus = 4;
	}
	else if (commandStatus == 3)
	{
		emit SetSerialRequest("5");
		commandStatus = 5;
	}
	else if (commandStatus == 4)
	{
		emit SetSerialRequest("4");
		commandStatus = 4;
	}
	else if (commandStatus == 5)
	{
		emit SetSerialRequest("5");
		commandStatus = 5;
	}

	return;
}

void ColorFab::HandleTableOff()
{
	if (commandStatus == 1)
	{
		emit SetSerialRequest("0");
		commandStatus = 0;
	}
	else if (commandStatus == 3)
	{
		emit SetSerialRequest("1");
		commandStatus = 1;
	}
	else if (commandStatus == 4)
	{
		emit SetSerialRequest("1");
		commandStatus = 1;
	}
	else
	{
		emit SetSerialRequest("0");
		commandStatus = 0;
	}

	return;
}

void ColorFab::SetManualCalibrationProCam()
{
	/* SET RELATION BETWEEN CAMERA-PROJECTOR FOR CALIBRATION */
	// If flip image is activated, reset to unflip
	if (ui->checkBoxCameraFlipMode->isChecked())
	{
		QMessageBox::information(0,
								tr("Manual Cam-Pro Calibration"),
								tr("*FLIP IMAGE RULE WAS APPLIED!, \n"
									"Please make sure you projection setting is also FLIP or not. \n\n"
									"If YES, select 'corner' as L-UP > R-UP > R-DOWN > L-DOWN \n"
									"If NO, select 'corner' as R-DOWN > L-DOWN > L-UP > R-UP \n"),
			QMessageBox::Ok,
			QMessageBox::NoButton);
	}
	else
	{
		QMessageBox::information(0,
								tr("Manual Cam-Pro Calibration"),
								tr("*FLIP IMAGE IS NOT APPLY!, \n"
									"Please make sure you projection setting is also FLIP or not. \n\n"
									"If YES, select 'corner' as R-DOWN > L-DOWN > L-UP > R-UP \n"
									"If NO, select 'corner' as L-UP > R-UP > R-DOWN > L-DOWN \n"),
			QMessageBox::Ok,
			QMessageBox::NoButton);
	}
	// Change the button text to 'reset'
	ui->buttonCalibCamToPro_input->setText("Reset");
	cameraViewer->setFocus();

	// Reset text label onscreen
	ui->pCalibCam_x1->setText("");
	ui->pCalibCam_y1->setText("");
	ui->pCalibPro_x1->setText("");
	ui->pCalibPro_y1->setText("");
	ui->pCalibCam_x2->setText("");
	ui->pCalibCam_y2->setText("");
	ui->pCalibPro_x2->setText("");
	ui->pCalibPro_y2->setText("");
	ui->pCalibCam_x3->setText("");
	ui->pCalibCam_y3->setText("");
	ui->pCalibPro_x3->setText("");
	ui->pCalibPro_y3->setText("");
	ui->pCalibCam_x4->setText("");
	ui->pCalibCam_y4->setText("");
	ui->pCalibPro_x4->setText("");
	ui->pCalibPro_y4->setText("");

	// Reset CameraROI and its related information
	if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
	{
		realSenseCamera->SetCameraROI(cv::Point2f(0, 0),
									  cv::Point2f(640, 0),
									  cv::Point2f(640, 480),
									  cv::Point2f(0, 480));
	}
	else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
	{
		pointGreyCamera->SetCameraROI(cv::Point2f(0, 0),
									  cv::Point2f(640, 0),
									  cv::Point2f(640, 480),
									  cv::Point2f(0, 480));
	}

	// Temporary open the projection screen
	int temp_ScreenNumber = ui->comboBoxScreenNumber->itemData(ui->comboBoxScreenNumber->currentIndex()).toInt();
	screen->setGeometry(QApplication::desktop()->screenGeometry(temp_ScreenNumber));

	// Check the size of screen and its geometry
	if (screen->size().width() != ui->lineEditScreenW->text().toInt() &&
		screen->size().height() != ui->lineEditScreenH->text().toInt())
		screen->resize(ui->lineEditScreenW->text().toInt(), ui->lineEditScreenH->text().toInt());

	// Show projection screen
	screen->showFullScreen();

	// Request cameraView to read mouseevent
	emit IsRequestCamProCalibrate(true);

	return;
}

void ColorFab::OnScreenCalibCamPos(const int &X, const int &Y, const int &I)
{
	/* DISPLAY CALIBRATION POSITION ON SCREEN */
	if (I == 1)
	{
		ui->pCalibCam_x1->setText(QString::number(X));
		ui->pCalibCam_y1->setText(QString::number(Y));
		//ui->pCalibPro_x1->setText("0");
		//ui->pCalibPro_y1->setText("0");
	}
	else if (I == 2)
	{
		ui->pCalibCam_x2->setText(QString::number(X));
		ui->pCalibCam_y2->setText(QString::number(Y));
		//ui->pCalibPro_x2->setText(ui->lineEditScreenW->text());
		//ui->pCalibPro_y2->setText("0");
	}
	else if (I == 3)
	{
		ui->pCalibCam_x3->setText(QString::number(X));
		ui->pCalibCam_y3->setText(QString::number(Y));
		//ui->pCalibPro_x3->setText(ui->lineEditScreenW->text());
		//ui->pCalibPro_y3->setText(ui->lineEditScreenH->text());
	}
	else if (I == 4)
	{
		ui->pCalibCam_x4->setText(QString::number(X));
		ui->pCalibCam_y4->setText(QString::number(Y));
		//ui->pCalibPro_x4->setText("0");
		//ui->pCalibPro_y4->setText(ui->lineEditScreenH->text());

		// Enable 'calibration' button
		ui->buttonCalibCamToPro_exec->setEnabled(true);
	}

	return;
}

void ColorFab::OnScreenCalibProPos(const int &X, const int &Y, const int &I)
{
	/* DISPLAY CALIBRATION POSITION ON SCREEN */
	if (I == 1)
	{
		//ui->pCalibCam_x1->setText(QString::number(X));
		//ui->pCalibCam_y1->setText(QString::number(Y));
		ui->pCalibPro_x1->setText(QString::number(X));
		ui->pCalibPro_y1->setText(QString::number(Y));
	}
	else if (I == 2)
	{
		//ui->pCalibCam_x2->setText(QString::number(X));
		//ui->pCalibCam_y2->setText(QString::number(Y));
		ui->pCalibPro_x2->setText(QString::number(X));
		ui->pCalibPro_y2->setText(QString::number(Y));
	}
	else if (I == 3)
	{
		//ui->pCalibCam_x3->setText(QString::number(X));
		//ui->pCalibCam_y3->setText(QString::number(Y));
		ui->pCalibPro_x3->setText(QString::number(X));
		ui->pCalibPro_y3->setText(QString::number(Y));
	}
	else if (I == 4)
	{
		//ui->pCalibCam_x4->setText(QString::number(X));
		//ui->pCalibCam_y4->setText(QString::number(Y));
		ui->pCalibPro_x4->setText(QString::number(X));
		ui->pCalibPro_y4->setText(QString::number(Y));

		// Enable 'calibration' button
		ui->buttonCalibCamToPro_exec->setEnabled(true);
	}

	return;
}

void ColorFab::CalCamProCalibrationOnScreen()
{
	/* CALCULATE CAMERA-PROJECTOR CALIBRATION BY MANUAL */
	std::vector<cv::Point2f> Cam, Pro;
	Cam.push_back(cv::Point2f(ui->pCalibCam_x1->text().toFloat(), ui->pCalibCam_y1->text().toFloat()));
	Cam.push_back(cv::Point2f(ui->pCalibCam_x2->text().toFloat(), ui->pCalibCam_y2->text().toFloat()));
	Cam.push_back(cv::Point2f(ui->pCalibCam_x3->text().toFloat(), ui->pCalibCam_y3->text().toFloat()));
	Cam.push_back(cv::Point2f(ui->pCalibCam_x4->text().toFloat(), ui->pCalibCam_y4->text().toFloat()));

	Pro.push_back(cv::Point2f(ui->pCalibPro_x1->text().toFloat(), ui->pCalibPro_y1->text().toFloat()));
	Pro.push_back(cv::Point2f(ui->pCalibPro_x2->text().toFloat(), ui->pCalibPro_y2->text().toFloat()));
	Pro.push_back(cv::Point2f(ui->pCalibPro_x3->text().toFloat(), ui->pCalibPro_y3->text().toFloat()));
	Pro.push_back(cv::Point2f(ui->pCalibPro_x4->text().toFloat(), ui->pCalibPro_y4->text().toFloat()));

	// Calculate Homography matrix
	int CalibrationType = def::CALIB_CAMTOPRO;
	this->isCamProCalibrated = calibration->ProcessHomography(Cam, Pro, CalibrationType);
	if (isCamProCalibrated)
	{
		// Update Calibration matrix to 'stream'
		emit IsHomographyTransformChanged(calibration->CamToPro_Matrix, CalibrationType);
		// Enable to calibrate Depth/CamROI manual button
		ui->buttonCalibCamToPro_input->setEnabled(true);

		// Write Cam-Pro relation to file
		calibration->WriteCalibrateFile(Cam, Pro, CalibrationType);
		// Setup Camera ROI (get camera points from calibration)
		if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 1)
		{
			realSenseCamera->SetCameraROI(calibration->camPts[0],
										  calibration->camPts[1],
										  calibration->camPts[2],
										  calibration->camPts[3]);
		}
		else if (ui->comboBoxCameraModel->itemData(ui->comboBoxCameraModel->currentIndex()) == 2)
		{
			pointGreyCamera->SetCameraROI(calibration->camPts[0],
										  calibration->camPts[1],
										  calibration->camPts[2],
										  calibration->camPts[3]);
		}

		// Restore 'manual' button and disable 'calibrate' button
		ui->buttonCalibCamToPro_input->setText("Manual");
		ui->buttonCalibCamToPro_exec->setEnabled(false);
		QMessageBox::information(0,
			tr("Manual Cam-Pro Calibration"),
			tr("Calibration Process COMPLETED!"),
			QMessageBox::Ok,
			QMessageBox::NoButton);

		// Stop screening thread
		if (screen->isVisible())
			screen->close();
		emit IsRequestCamProCalibrate(false);
	}
	else
	{
		QMessageBox::warning(0,
			tr("Manual Cam-Pro Calibration"),
			tr("Calibration Process FAILED!, \n"
				"Please try again ..."),
			QMessageBox::Ok,
			QMessageBox::NoButton);
	}

	return;
}

void ColorFab::UpdateIsCameraViewVisible(const bool &IsWidgetVisible)
{
	// Update emit value from CameraView widget
	this->isCameraViewVisible = IsWidgetVisible;

	return;
}

void ColorFab::UpdateViewSizeOnScreen(const int &W, const int &H)
{
	// UPDATE VIEW SIZE GUI INFORMATION
	ui->lineEditViewW->setText(QString::number(W));
	ui->lineEditViewH->setText(QString::number(H));

	return;
}

void ColorFab::OnScreenDisplayTimer(const QString &Timer)
{
	ui->statusBar->showMessage(Timer + " fps");

	return;
}

void ColorFab::SaveCameraFrame()
{
	cameraImageFile = QFileDialog::getSaveFileName(this, 
												   "Save as", 
												   "Choose a filename", 
												   "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");
	cameraFrame.save(cameraImageFile, 0, 99);

	return;
}

void ColorFab::GetCameraFrame(const QImage &CameraFrame)
{
	// Update camera frame from stream thead
	this->cameraFrame = CameraFrame;

	return;
}

void ColorFab::SetProjectionFile()
{
	projectionFile = QFileDialog::getOpenFileName(this,
												  tr("Open Image"),
												  QDir::currentPath(),
												  tr("Image Files (*.png *.jpg)"));

	// Update selected image file path
	ui->labelProjectionFileParameter->setText(projectionFile);
	emit UpdateProjectionFile(projectionFile);

	return;
}

void ColorFab::UpdateSerialStatus(const QString &s)
{
	ui->outPortStatus->setText(tr("Not running, %1.").arg(s));

	return;
}

void ColorFab::UpdateSerialResponse(const QString &s)
{
	ui->outPortStatus->setText(tr("Running, %1.").arg(s));

	return;
}

void ColorFab::HandlePaintingUI()
{
	paintingUI->show();

	return;
}

void ColorFab::License()
{
	/* ABOUT THIS DEVELOPMENT */
	QString message("ColorFab Projection \n"
					"This application under MIT License. \n");
	QMessageBox::information(this, "ColorFab Projection", message.arg(APP_VERSION));

	return;
}