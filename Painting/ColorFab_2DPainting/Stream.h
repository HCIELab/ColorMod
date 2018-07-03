#pragma once
#include <QtCore/QThread>
#include <QtGui/QImage>
#include <QMutex>

#include <opencv2/opencv.hpp>

#include "Timer.h"

class RenderHelper;

class Stream : public QThread
{
	Q_OBJECT

	private:
		// Capture frame (infrared image, PointGreyCamera)
		cv::Mat cameraFrame;
		cv::Mat cameraView;
		cv::Mat projectionImage;
		cv::Mat projectionOnScreen;
		//QImage projectionImage;
		QString projectionFile;
		bool doStopThread;
		QMutex *IsProcessing;

		bool isCameraUpdated;
		bool isProjecting;

		// Inherit parameter from perspective homography calibration process (camera)
		cv::Mat CamToProMatrix;
		// Offset value as the minimum value of cameraROI (undertake calibration)
		cv::Point2f offset;
		int surfaceWidth, surfaceHeight;
		int screenSizeW;
		int screenSizeH;

		RenderHelper *render;
		std::string surfaceTexturePath;
		cv::Mat surfaceTexture;
		//cv::Mat deformed;
		int visualizeGradientSize;

		Timer *timer;

		inline cv::Rect setTouchDetectionRegion(cv::Point2f Proc_SelectedFingerPosition);
		void setProjectionImage();

	public:
		Stream(RenderHelper *renderHelper);
		Stream();
		~Stream();

		void Initialize(cv::Mat &CamToProMatrix,
						int ProjectionScreenW,
						int ProjectionScreenH,
						QString ProjectionFile,
						cv::Point2f Offset);
		void stop();

	protected:
		virtual void run();

	signals:
		void SetOffScreenRendering();

		void UpdateCaptureFrameToViewer(const QImage &Viewer);
		void UpdateDepthFrameToViewer(const QImage &Viewer);

		// -- To display processing time onscreen
		void SetTimer(const QString &Time);

	public slots:
		void SetHomographyTransform(const cv::Mat &Homography, const int &Mode);
		void SetCameraFrame(const cv::Mat &Frame);
		void StartProjection(const bool &StartProjecting);
		void UpdateProjectionFile(const QString &ProjectionFile);
};