#pragma once
#include <QtCore/QThread>
#include <QMutex>

#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <pxcsensemanager.h>

class RealSense : public QThread
{
	Q_OBJECT

	private:
		bool isConnected;
		bool doStopThread;

		QMutex *IsProcessing;

		// RealSense buffers
		PXCSession *cameraSession;
		PXCSenseManager *cameraManager;
		PXCCapture::Sample *cameraSample;
		PXCImage::PixelFormat pixelFormat;

		// OpenCV frame buffers
		cv::Mat frameBuffer;
		cv::Mat frameROI;

		bool doFlipImage;

		void Capture();
		//cv::Mat PXCImage2CVMat(PXCImage*, PXCImage::PixelFormat);

	protected:
		virtual void run();

	public:
		cv::Rect cameraROI;
		cv::Mat frame;

		RealSense();
		~RealSense();

		void Setup(int FrameRate, bool DoFlipImage);
		bool Initial();
		void stop();
		bool Disconnect();

	signals:
		void SetFrame(const cv::Mat &Frame);
		void SetSize(const int &W, const int &H);

	public slots:
		void SetCameraROI(const cv::Point2f &p1,
						  const cv::Point2f &p2,
						  const cv::Point2f &p3,
						  const cv::Point2f &p4);
		void DoFlipImage(const int &IsFlipImage);

};