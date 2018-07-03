#include "Stream.h"
#include "StreamHelper.h"
#include "ImageHelper.h"
#include "RenderHelper.h"
#include "Config.h"

#include <QtGui/QImage>
#include <QtCore/QDateTime>
#include <QtWidgets/QMessageBox>

#include <omp.h>

Stream::Stream(RenderHelper *renderHelper)
	: IsProcessing(NULL)
	, render(NULL)
	, timer(NULL)
{
	this->doStopThread = false;
	IsProcessing = new QMutex(QMutex::Recursive);

	// Default frames parameter(s) to synchonize multiple threads
	this->isCameraUpdated = false;
	this->isProjecting = false;

	// Default parameters for instance(s)
	this->surfaceWidth = 640;
	this->surfaceHeight = 480;
	render = renderHelper;

	timer = new Timer();

	// Surface texture
	this->surfaceTexturePath = "Data/SurfaceTextures/Default.jpg";
	this->surfaceTexture = cv::imread(surfaceTexturePath, CV_LOAD_IMAGE_UNCHANGED);
}

Stream::~Stream()
{
	cameraFrame.release();
	cameraView.release();
	CamToProMatrix.release();
	surfaceTexture.release();

	if (IsProcessing != NULL) delete IsProcessing;
	if (timer != NULL)
		delete timer;
}

void Stream::Initialize(cv::Mat &CamToProMatrix,
						int ProjectionScreenW,
						int ProjectionScreenH,
						QString ProjectionFile,
						cv::Point2f Offset)
{
	// Setup initial parameter(s)
	this->CamToProMatrix = CamToProMatrix;
	this->screenSizeW = ProjectionScreenW;
	this->screenSizeH = ProjectionScreenH;

	// Setup projection screen size to rendering instance
	render->SetScreen(ProjectionScreenW, ProjectionScreenH);

	// Setup projection file
	if (ProjectionFile.size() == 0)
		this->projectionFile = QString("Data//Projection//640x480_projection_yellow.jpg");
	else
		this->projectionFile = ProjectionFile;
	setProjectionImage();
	
	// Setup calibration offset since we calibrate original point not from (0,0)
	this->offset = Offset;

	return;
}

void Stream::run()
{
	while (true)
	{
		// [Timer]
		timer->start();

		// 1. Check is thread need requested to stop or not
		if (doStopThread)
		{
			this->doStopThread = false;
			break;
		}

		// 2. Check is new frame has been update from camera thread or not
		if (!this->isCameraUpdated)
			continue;

		// 3. Lock the thread to update external parameters
		IsProcessing->lock();

		// [A] Copy cameraFrame to cameraView frame, to prevent flicker on the viewer
		if (cameraFrame.type() == CV_8UC1)
			cv::cvtColor(cameraFrame, cameraView, CV_GRAY2BGR);
		else
			cameraFrame.copyTo(cameraView);

		// This process projecting an image on the projection screen
		if (isProjecting)
		{
			// Setup rendering
			if (!projectionFile.isNull())
			{
				//render->RenderSetup(projectionImage);
				render->RenderSetup(ImageHelper::cvMatToQImage(projectionOnScreen, true));
			}
			else
				render->RenderSetup();

			emit SetOffScreenRendering();
		}

		// -- Display CameraViewer
		emit UpdateCaptureFrameToViewer(ImageHelper::cvMatToQImage(cameraView, false));

		// Reset status of grasping frame/data
		this->isCameraUpdated = false;

		// [Timer]
		timer->stop();
		emit SetTimer(QString().number(1 / timer->getElapsedTimeInSec()));

		// Unlock to allow data to be updated
		IsProcessing->unlock();
	}
}

cv::Rect Stream::setTouchDetectionRegion(cv::Point2f Proc_SelectedFingerPosition)
{
	// Set selected finger position region to measure by depth sensing
	cv::Rect proc_selectedFingerRegion;
	proc_selectedFingerRegion.x = Proc_SelectedFingerPosition.x - 20;
	proc_selectedFingerRegion.y = Proc_SelectedFingerPosition.y - 5;
	proc_selectedFingerRegion.width = 40;
	proc_selectedFingerRegion.height = 40;

	return proc_selectedFingerRegion;
}

void Stream::stop()
{
	// Stop thread
	this->doStopThread = true;
}

void Stream::SetHomographyTransform(const cv::Mat &Homography, const int &Mode)
{
	// Update value of homography transform parameter
	if (Mode == def::CALIB_CAMTOPRO) this->CamToProMatrix = Homography;

	return;
}

void Stream::SetCameraFrame(const cv::Mat &Frame)
{
	// Update capturing frame from 'camera'
	Frame.copyTo(cameraFrame);
	// Copy image to view (debug) for 'viewer'
	// -- Image to viewer is copy from cameraFrame in a thread (prevent flicker)
	// Update status of capturing, that new frame is come
	this->isCameraUpdated = true;

	return;
}

void Stream::StartProjection(const bool &StartProjecting)
{
	// Update status of stream 'projection'
	this->isProjecting = StartProjecting;

	return;
}

void Stream::UpdateProjectionFile(const QString &ProjectionFile)
{
	// Update projection file from user interface
	this->projectionFile = ProjectionFile;
	//this->projectionImage = QImage(projectionFile);
	setProjectionImage();

	return;
}

void Stream::setProjectionImage()
{
	// Update image to cv::Mat
	this->projectionImage = cv::imread(projectionFile.toStdString(), cv::IMREAD_COLOR);
	// Applies a perspective transformation to an image
	// http://docs.opencv.org/3.0-beta/modules/imgproc/doc/geometric_transformations.html
	cv::warpPerspective(projectionImage,
						projectionOnScreen,
						CamToProMatrix,
						cv::Size(screenSizeW, screenSizeH),
						cv::INTER_NEAREST,
						cv::BORDER_CONSTANT,
						cv::Scalar());
	// conversion from BGR (opencv format) to RGB (QImage format)
	cv::cvtColor(projectionOnScreen, projectionOnScreen, CV_BGR2RGB, 3);
	return; 
}