#include <opencv2/imgproc/imgproc.hpp>

#include <QtGlobal>
#include <QtWidgets/QMessageBox>

#include "PointGreyCamera.h"
#include "ImageHelper.h"
#include "Config.h"

using namespace cv;
using namespace std;
using namespace FlyCapture2;

PointGreyCamera::PointGreyCamera()
	: IsProcessing(NULL)
{
	this->doStopThread = false;
	IsProcessing = new QMutex(QMutex::Recursive);

	// Default parameters
	cameraROI = Rect(0, 0, 640, 480);
	this->isConnected = false;
}

PointGreyCamera::~PointGreyCamera()
{
	if (isConnected) Disconnect();

	if (frame.data != NULL) frame.release();
	if (frameROI.data != NULL) frameROI.release();
	if (matBuffer.data != NULL) matBuffer.release();
	if (IsProcessing != NULL) delete IsProcessing;
}

void PointGreyCamera::Setup(int Framerate,
							double Gain,
							double Brightness,
							bool DoAutoexposure,
							bool DoAutogamma,
							bool DoFlipImage)
{
	// Setup parameters
	this->camWidth = 1288;
	this->camHeight = 964;
	this->frameRate = Framerate;
	this->gain = Gain;
	this->brightness = Brightness;
	this->doAutoexposure = DoAutoexposure;
	this->doAutogamma = DoAutogamma;
	this->doFlipImage = DoFlipImage;

	this->imageByte = camWidth * camHeight * camChannel;

	return;
}

bool PointGreyCamera::Initial()
{
	/* Setup frame buffer */
	// Size of frame buffers depends on the camera setting
	frame.create(camHeight, camWidth, CV_8UC3);
	matBuffer.create(camHeight, camWidth, CV_8UC3);

	/* Initialize PointGrey Camera */
	isConnected = false;
	// Get Camera and set the connection (+ check error handle)
	error = busMgr.GetCameraFromIndex(0, &guid);
	if (error != PGRERROR_OK)
	{
		QMessageBox::information(0,
								tr("PointGrey Camera Error"),
								tr(error.GetDescription()),
								QMessageBox::Ok,
								QMessageBox::NoButton);
		return isConnected;
	}
	fle3camera.Connect(&guid);

	FlyCapture2::VideoMode mode;
	FlyCapture2::FrameRate fps;
	fle3camera.GetVideoModeAndFrameRate(&mode, &fps);

	// Set Video mode and framerate
	//if (frameRate == def::FPS015)
	//{
	//	error = fle3camera.SetVideoModeAndFrameRate(FlyCapture2::VIDEOMODE_1280x960YUV422, FlyCapture2::FRAMERATE_15);
	//	if (error != PGRERROR_OK)
	//	{
	//		QMessageBox::information(0,
	//								tr("PointGrey Camera Error"),
	//								tr(error.GetDescription()),
	//								QMessageBox::Ok,
	//								QMessageBox::NoButton);
	//		return isConnected;
	//	}
	//}
	//else if (frameRate == def::FPS030)
	//{
	//	error = fle3camera.SetVideoModeAndFrameRate(FlyCapture2::VIDEOMODE_1280x960Y8, FlyCapture2::FRAMERATE_15);
	//	if (error != PGRERROR_OK)
	//	{
	//		QMessageBox::information(0,
	//								tr("PointGrey Camera Error"),
	//								tr(error.GetDescription()),
	//								QMessageBox::Ok,
	//								QMessageBox::NoButton);
	//		return isConnected;
	//	}
	//}
	prop.absControl = true;
	prop.onePush = false;
	prop.onOff = true;
	prop.autoManualMode = false;

	// Set shutter speed
	prop.type = FlyCapture2::SHUTTER;
	// Ensure the property is on
	prop.onOff = true;
	prop.absValue = ((float)(1.0 / 33.0*1000.0));
	error = fle3camera.SetProperty(&prop);
	if (error != PGRERROR_OK)
	{
		QMessageBox::information(0,
								tr("PointGrey Camera Error"),
								tr(error.GetDescription()),
								QMessageBox::Ok,
								QMessageBox::NoButton);
		return isConnected;
	}

	// Set gain
	prop.type = FlyCapture2::GAIN;
	prop.absValue = (float)gain;
	error = fle3camera.SetProperty(&prop);
	if (error != PGRERROR_OK)
	{
		QMessageBox::information(0,
								tr("PointGrey Camera Error"),
								tr(error.GetDescription()),
								QMessageBox::Ok,
								QMessageBox::NoButton);
		return isConnected;
	}

	// Set Brightness
	prop.type = FlyCapture2::BRIGHTNESS;
	// Ensure the property is on
	prop.onOff = true;
	// Ensure auto-adjust mode is on
	prop.autoManualMode = false;
	// Ensure the property is set up to use absolute value control
	prop.absControl = true;
	// Set the absolute value of brightness to (number)%
	prop.absValue = (float)brightness;
	error = fle3camera.SetProperty(&prop);
	if (error != PGRERROR_OK)
	{
		QMessageBox::information(0,
								tr("PointGrey Camera Error"),
								tr(error.GetDescription()),
								QMessageBox::Ok,
								QMessageBox::NoButton);
		return isConnected;
	}

	// Set Exposure
	if (doAutoexposure)
	{
		prop.type = FlyCapture2::AUTO_EXPOSURE;
		// Ensure the property is on
		prop.onOff = true;
		// Ensure auto-adjust mode is on
		prop.autoManualMode = true;
		error = fle3camera.SetProperty(&prop);
		if (error != PGRERROR_OK)
		{
			QMessageBox::information(0,
									tr("PointGrey Camera Error"),
									tr(error.GetDescription()),
									QMessageBox::Ok,
									QMessageBox::NoButton);
			return isConnected;
		}
	}

	// Set Gamma
	if (doAutogamma)
	{
		prop.type = FlyCapture2::GAMMA;
		// Ensure the property is on
		prop.onOff = true;
		// Ensure auto-adjust mode is on
		prop.autoManualMode = true;
		error = fle3camera.SetProperty(&prop);
		if (error != PGRERROR_OK)
		{
			QMessageBox::information(0,
									tr("PointGrey Camera Error"),
									tr(error.GetDescription()),
									QMessageBox::Ok,
									QMessageBox::NoButton);
			return isConnected;
		}
	}

	// Start the capturing process (+ makesure, camera is start)
	error = fle3camera.StartCapture();
	if (error != PGRERROR_OK)
	{
		QMessageBox::information(0,
								tr("PointGrey Camera Error"),
								tr(error.GetDescription()),
								QMessageBox::Ok,
								QMessageBox::NoButton);
		isConnected = false;
	}
	else if (error == PGRERROR_OK) isConnected = true;

	return isConnected;
}

void PointGreyCamera::run()
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
		if (frame.data != NULL)
			emit SetFrame(frame);
	}
}

void PointGreyCamera::Capture()
{
	// Retrive current frame from the camera buffer (camera capture every timestep)
	error = fle3camera.RetrieveBuffer(&rawImage);
	if (error == PGRERROR_OK && rawImage.GetData() != NULL)
	{
		// Apply the method from https://gist.github.com/kevinhughes27/5543668
		// For operate Format7 image
		FlyCapture2::Image rawBuffer;
		rawImage.Convert(FlyCapture2::PIXEL_FORMAT_BGR, &rawBuffer);
		unsigned int rowBytes = (double)rawBuffer.GetReceivedDataSize() / (double)rawBuffer.GetRows();
		matBuffer = cv::Mat(rawBuffer.GetRows(), rawBuffer.GetCols(), CV_8UC3, rawBuffer.GetData(), rowBytes);
		resize(matBuffer, matBuffer, Size(640, 480));

		// If image flip is set (depends on camera setup)
		if (this->doFlipImage)	flip(matBuffer, matBuffer, -1);

		frameROI = matBuffer(cameraROI);
		frameROI.copyTo(frame);
	}

	return;
}

void PointGreyCamera::stop()
{
	// Qt will stop thread after Mutex is free (auto-checker)
	//QMutexLocker locker(&DuringProcess);

	// Set flag stop thread, and break out
	this->doStopThread = true;
}

bool PointGreyCamera::Disconnect()
{
	/* Disconnect PointGrey camera */
	// Qt will disconnect device after Mutex is free (auto-checker)
	//QMutexLocker locker(&DuringProcess);

	if (isConnected)
	{
		// Release captured storage buffer
		frameROI.release();
		frame.release();
		matBuffer.release();

		// Release camera frame buffer
		rawImage.ReleaseBuffer();
		// Stop Capturing process
		error = fle3camera.StopCapture();
		if (error != PGRERROR_OK)
		{
			QMessageBox::information(0,
									tr("PointGrey Camera Error"),
									tr(error.GetDescription()),
									QMessageBox::Ok,
									QMessageBox::NoButton);
		}
		// Disconnect camera
		fle3camera.Disconnect();
		if (error == PGRERROR_OK) isConnected = false;
	}

	return isConnected;
}

void PointGreyCamera::SetCameraROI(const cv::Point2f &p1,
								   const cv::Point2f &p2,
								   const cv::Point2f &p3,
								   const cv::Point2f &p4)
{
	/* SETUP CAMERA ROI */
	// Qt will update variable after Mutex is free (auto-checker)
	//QMutexLocker locker(&DuringProcess);

	// Find the region of interesting area 
	// (by find the min/max points of projection screen on the camera coodinate)
	// NOTE: Clockwise coordinate (left, right, right, left)
	int x, y;
	// Left(x,y)
	x = p1.x;
	if (p4.x < x) x = p4.x;
	y = p1.y; 
	if (p4.y < y) y = p4.y;
	Point2f camROI_pos_min = Point2f(x, y);

	// Right(x,y)
	x = p2.x;
	if (p3.x > x) x = p3.x;
	y = p2.y;
	if (p3.y > y) y = p3.y;
	Point2f camROI_pos_max = Point2f(x, y);

	if (camROI_pos_min.x + abs(camROI_pos_max.x - camROI_pos_min.x) > 640 ||
		camROI_pos_min.y + abs(camROI_pos_max.y - camROI_pos_min.y) > 480)
	{
		QMessageBox::warning(0,
							 tr("PointGrey Camera Error"),
							 tr("Calibration Process FAILED!, \n"
								"ROI setting error, please re-calibration!"),
							 QMessageBox::Ok,
							 QMessageBox::NoButton);
		cameraROI = Rect(0, 0, 640, 480);
	}
	else
	{
		cameraROI = Rect(camROI_pos_min.x,
						 camROI_pos_min.y,
						 abs(camROI_pos_max.x - camROI_pos_min.x),
						 abs(camROI_pos_max.y - camROI_pos_min.y));
	}

	// Update captured frame size to change the cameraViewer size
	emit SetSize(cameraROI.width, cameraROI.height);

	return;
}

void PointGreyCamera::DoFlipImage(const int &IsFlipImage)
{
	// Qt will update variable after Mutex is free (auto-checker)
	//QMutexLocker locker(&DuringProcess);

	// Update flip input image (depends on camera setting)
	if (IsFlipImage == 0)
		this->doFlipImage = false;
	else
		this->doFlipImage = true;
	return;
}