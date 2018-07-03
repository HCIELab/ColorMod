#include "CameraViewer.h"
#include "Config.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtGui/QMouseEvent>

CameraViewer::CameraViewer(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::CameraViewer)
{
	// Setup UI
	ui->setupUi(this);
	// Setup geometry of the camera viewer
	this->setGeometry((M_APP_QT_POS_X * 2) + parent->size().width(),
					   M_APP_QT_POS_Y,
					   M_APP_CAM_WIN_SIZE_W,
					   M_APP_CAM_WIN_SIZE_H);
	// Set viewer style to black color (using QStyle)
	ui->Display->setStyleSheet("QLabel { background-color : black; }");
	// Initialize camera connection status
	this->isCameraConnected = false;
	// Initialize camera-projector calibration request status
	this->isRequestCamProCalibrate = false;
	// Initialize surface model points request
	this->isRequestSurfaceModelBorderPoints = false;
	this->isRequestGetSurfaceModelPoints = false;
	this->isRequestGetFinalizePushingPosition = false;
	this->numberOfPointRequest = 0;
	// Counting for mouseevent (Maximum count is 4)
	this->mousePressCount = 0;
}

CameraViewer::~CameraViewer()
{
	delete ui;
}

void CameraViewer::closeEvent(QCloseEvent *event)
{
	// For normal close (when disconnected), we not change any status
	if (!isCameraConnected)
		event->accept();
	else
	{
		// Submit signal to invoke other GUI that this widget will close()
		emit IsWidgetVisible(false);
		event->accept();
	}
}

void CameraViewer::mousePressEvent(QMouseEvent *event)
{
	// Set Mouseevent for check the position of screen cursor, only when its reqeusted
	if (event->button() == Qt::LeftButton && this->isRequestCamProCalibrate)
	{
		if (this->mousePressCount < 4)
		{
			this->mousePressCount += 1;
			emit MousePressPosition(event->pos().x(), event->pos().y(), mousePressCount);
		}
		else
			this->isRequestCamProCalibrate = false;
	}
	else if (event->button() == Qt::LeftButton && this->isRequestGetSurfaceModelPoints)
	{
		if (this->mousePressCount < numberOfPointRequest)
		{
			this->mousePressCount += 1;
			emit MousePressSurfacePoint(event->pos().x(), event->pos().y(), mousePressCount);
		}
		else
			this->isRequestGetSurfaceModelPoints = false;
	}
	else if (event->button() == Qt::LeftButton && this->isRequestSurfaceModelBorderPoints)
	{
		if (this->mousePressCount < numberOfPointRequest)
		{
			this->mousePressCount += 1;
			emit MousePressSurfaceBorderPoint(event->pos().x(), event->pos().y(), mousePressCount);
		}
		else
			this->isRequestSurfaceModelBorderPoints = false;
	}
	else if (event->button() == Qt::LeftButton && this->isRequestGetFinalizePushingPosition)
	{
		emit MousePressSurfacePushingPoint(event->pos().x(), event->pos().y());
		this->isRequestGetFinalizePushingPosition = false;
	}

	return;
}

void CameraViewer::UpdateFrame(const QImage &Frame)
{
	// 1. Check the size of display, automatic resize
	if (Frame.size().width() != ui->Display->width() || 
		Frame.size().height() != ui->Display->height())
	{
		ui->Display->resize(Frame.size().width(), Frame.size().height());
		this->resize(Frame.size().width(), Frame.size().height());
	}

	// 2. Display image to cameraView
	ui->Display->setPixmap(QPixmap::fromImage(Frame));

	return;
}

void CameraViewer::UpdateCapturedSize(const int &Width, const int &Height)
{
	// Update window size to match with capturing frame
	ui->Display->resize(Width, Height);
	this->resize(Width, Height);

	return;
}

void CameraViewer::UpdateIsCameraConnected(const bool &IsCameraConnected)
{
	// Update status of camera connection, to handle force close process
	this->isCameraConnected = IsCameraConnected;

	return;
}

void CameraViewer::UpdateIsRequestCamProCalibrate(const bool &IsRequestCamProCalibrate)
{
	// Reset mouseevent pressed count
	this->mousePressCount = 0;
	// Update status of camera-projector calibration request
	this->isRequestCamProCalibrate = IsRequestCamProCalibrate;
	// In other word, the request for set surface model points should be disabled
	this->isRequestSurfaceModelBorderPoints = false;
	this->isRequestGetSurfaceModelPoints = false;

	return;
}

void CameraViewer::UpdateIsRequestGetSurfaceModelBorderPoints(const bool &IsRequestGetSurfaceModelBorderPoints,
															  const int &NumberOfPointsRequest)
{
	// Reset mouseevent pressed count
	this->mousePressCount = 0;
	// Set number of points request by surface model calculation function
	this->numberOfPointRequest = NumberOfPointsRequest;
	// Update status of surface model points request
	this->isRequestSurfaceModelBorderPoints = IsRequestGetSurfaceModelBorderPoints;
	// In other word, the request for calibration should be disabled
	this->isRequestGetSurfaceModelPoints = false;
	this->isRequestCamProCalibrate = false;

	return;
}

void CameraViewer::UpdateIsRequestGetSurfaceModelPoints(const bool &IsRequestGetSurfaceModelPoints,
														const int &NumberOfPointsRequest)
{
	// Reset mouseevent pressed count
	this->mousePressCount = 0;
	// Set number of points request by surface model calculation function
	this->numberOfPointRequest = NumberOfPointsRequest;
	// Update status of surface model points request
	this->isRequestGetSurfaceModelPoints = IsRequestGetSurfaceModelPoints;
	this->isRequestSurfaceModelBorderPoints = false;
	// In other word, the request for calibration should be disabled
	this->isRequestCamProCalibrate = false;

	return;
}

void CameraViewer::UpdateIsRequestGetFinalizePushingPosition(const bool &IsReqeustGetFinalizePusingPosition)
{
	// Reset mouseevent pressed count
	this->mousePressCount = 0;
	// Update status of final pushing position request
	this->isRequestGetFinalizePushingPosition = IsReqeustGetFinalizePusingPosition;
	// Update status of surface model points request
	this->isRequestGetSurfaceModelPoints = false;
	this->isRequestSurfaceModelBorderPoints = false;
	// In other word, the request for calibration should be disabled
	this->isRequestCamProCalibrate = false;

	return;
}