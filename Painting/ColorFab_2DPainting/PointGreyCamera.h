#pragma once
#include <QtCore/QThread>
#include <QMutex>

#include <opencv2/opencv.hpp>
#include <FlyCapture2.h>

class PointGreyCamera : public QThread
{
	Q_OBJECT

	private:
		bool isConnected;

		bool doStopThread;
		QMutex *IsProcessing;

		FlyCapture2::Camera fle3camera;
		FlyCapture2::PGRGuid guid;
		FlyCapture2::BusManager busMgr;
		FlyCapture2::Property prop;

		// PointGreyCamera frame buffers
		FlyCapture2::Image rawImage;
		size_t imageByte;

		// OpenCV frame buffers
		cv::Mat matBuffer;
		cv::Mat frameROI;

		// PointGreyCamera setting parameters
		int videoMode;
		int camWidth;
		int camHeight;
		int camChannel;
		int frameRate;
		double gain;
		double brightness;
		bool doAutoexposure;
		bool doAutogamma;
		bool doFlipImage;

		void Capture();

	protected:
		virtual void run();

	public:
		FlyCapture2::Error error;
		cv::Mat frame;
		cv::Rect cameraROI;

		PointGreyCamera();
		~PointGreyCamera();

		void Setup(int Framerate,
				   double Gain,
				   double Brightness,
				   bool DoAutoexposure,
				   bool DoAutogamma,
				   bool DoFlipImage);
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