#include <QMessageBox>

#include "RealSenseSR300.h"
#include "ImageHelper.h"

RealSense::RealSense()
	: IsProcessing(NULL)
{
	this->doStopThread = false;
	IsProcessing = new QMutex(QMutex::Recursive);

	// Default parameters
	cameraROI = cv::Rect(0, 0, 640, 480);
	this->isConnected = false;
}

RealSense::~RealSense()
{
	if (isConnected) Disconnect();

	if (frameROI.data != NULL) frameROI.release();
	if (frameBuffer.data != NULL) frameBuffer.release();
	if (IsProcessing != NULL) delete IsProcessing;

	if (isConnected)
	{
		if (cameraSession != NULL) cameraSession->Release();
		if (cameraManager != NULL) cameraManager->Release();
	}
}

void RealSense::Setup(int FrameRate, bool DoFlipImage)
{
	// Setup parameters
	this->cameraSession = PXCSession::CreateInstance();
	this->cameraManager = PXCSenseManager::CreateInstance();
	this->cameraManager->EnableStream(PXCCapture::STREAM_TYPE_COLOR, 
									  cameraROI.width, 
									  cameraROI.height, 
									  FrameRate);

	this->doFlipImage = DoFlipImage;

	return;
}

bool RealSense::Initial()
{
	if (cameraManager->Init() == STATUS_NO_ERROR)
	{
		this->isConnected = true;
		return true;
	}
	else
	{
		this->isConnected = false;
		return false;
	}
}

void RealSense::run()
{
	while (true)
	{
		// Lock to prevent asynchonize process
		IsProcessing->lock();
		// Thread will stop when 'doStopThread' is true!
		if (doStopThread)
		{
			this->doStopThread = false;
			IsProcessing->unlock();

			break;
		}

		// Capture imageframe
		Capture();

		// Unlock to allow data to be updated
		IsProcessing->unlock();

		// Update Capturing frame to Stream
		if (frame.data != NULL) emit SetFrame(frame);
	}
}

void RealSense::Capture()
{
	if (cameraManager->AcquireFrame(true) < PXC_STATUS_NO_ERROR) return;

	this->cameraSample = cameraManager->QuerySample();
	if (cameraSample->color)
	{
		PXCImage::ImageData data;
		cameraSample->color->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::PIXEL_FORMAT_RGB24, &data);

		int width = cameraSample->color->QueryInfo().width;
		int height = cameraSample->color->QueryInfo().height;
		pixelFormat = cameraSample->color->QueryInfo().format;

		int type = 0;
		if (pixelFormat == PXCImage::PIXEL_FORMAT_Y8) type = CV_8UC1;
		else if (pixelFormat == PXCImage::PIXEL_FORMAT_RGB24) type = CV_8UC3;
		else if (pixelFormat == PXCImage::PIXEL_FORMAT_DEPTH_F32) type = CV_32FC1;
		else type = CV_8UC3;

		frameBuffer = cv::Mat(cv::Size(width, height), type, data.planes[0]);
		cameraSample->color->ReleaseAccess(&data);

		// If image flip is set (depends on camera setup)
		if (this->doFlipImage)	cv::flip(frameBuffer, frameBuffer, -1);
		frameROI = frameBuffer(cameraROI);

		// Export final frame image
		frameROI.copyTo(frame);
	}

	cameraManager->ReleaseFrame();
	return;
}

void RealSense::stop()
{
	// Set flag stop thread, and break out
	this->doStopThread = true;
}

bool RealSense::Disconnect()
{
	if (isConnected)
	{
		// Release captured storage buffer
		frameROI.release();
		frame.release();
		frameBuffer.release();

		// Release camera frame buffer
		cameraManager->ReleaseFrame();
		// Stop Capturing process
		cameraSession->Release();
		isConnected = false;
	}

	return isConnected;
}

void RealSense::SetCameraROI(const cv::Point2f &p1,
							 const cv::Point2f &p2,
							 const cv::Point2f &p3,
							 const cv::Point2f &p4)
{
	/* SETUP CAMERA ROI */
	// Find the region of interesting area 
	// (by find the min/max points of projection screen on the camera coodinate)
	// NOTE: Clockwise coordinate (left, right, right, left)
	int x, y;
	// Left(x,y)
	x = p1.x;
	if (p4.x < x) x = p4.x;
	y = p1.y;
	if (p4.y < y) y = p4.y;
	cv::Point2f camROI_pos_min = cv::Point2f(x, y);

	// Right(x,y)
	x = p2.x;
	if (p3.x > x)
		x = p3.x;
	y = p2.y;
	if (p3.y > y)
		y = p3.y;
	cv::Point2f camROI_pos_max = cv::Point2f(x, y);

	if (camROI_pos_min.x + abs(camROI_pos_max.x - camROI_pos_min.x) > 640 ||
		camROI_pos_min.y + abs(camROI_pos_max.y - camROI_pos_min.y) > 480)
	{
		QMessageBox::warning(0,
							 tr("Camera Error"),
							 tr("Calibration Process FAILED!, \n"
								"ROI setting error, please re-calibration!"),
							 QMessageBox::Ok,
							 QMessageBox::NoButton);
		cameraROI = cv::Rect(0, 0, 640, 480);
	}
	else
	{
		cameraROI = cv::Rect(camROI_pos_min.x,
							 camROI_pos_min.y,
							 abs(camROI_pos_max.x - camROI_pos_min.x),
							 abs(camROI_pos_max.y - camROI_pos_min.y));
	}

	// Update captured frame size to change the cameraViewer size
	emit SetSize(cameraROI.width, cameraROI.height);

	return;
}

void RealSense::DoFlipImage(const int &IsFlipImage)
{
	// Qt will update variable after Mutex is free (auto-checker)
	//QMutexLocker locker(&DuringProcess);

	// Update flip input image (depends on camera setting)
	if (IsFlipImage == 0) this->doFlipImage = false;
	else this->doFlipImage = true;

	return;
}