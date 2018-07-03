#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ColorFab.h"

#include "RealSenseSR300.h"
#include "PointGreyCamera.h"
#include "Calibration.h"
#include "RenderHelper.h"
#include "Rendering.h"
#include "Stream.h"

#include "CameraViewer.h"
#include "Screen.h"

#include "SerialController.h"

#include "PaintingUI.h"

class ColorFab : public QMainWindow
{
	Q_OBJECT

	public:
		ColorFab(QWidget *parent = Q_NULLPTR);
		~ColorFab();

	private:
		Ui::ColorFabClass *ui;

		RealSense *realSenseCamera;
		PointGreyCamera *pointGreyCamera;
		bool isCameraConnected;
		Calibration *calibration;
		bool isCamProCalibrated;
		RenderHelper *renderHelper;
		Rendering *rendering;
		Stream *stream;
		bool isConnecting;
		bool isProjecting;

		CameraViewer *cameraViewer;
		bool isCameraViewVisible;
		Screen *screen;
		QString cameraImageFile;
		QImage cameraFrame;
		QString projectionFile;

		SerialController *serialController;
		int transactionCount;
		int commandStatus;

		PaintingUI *paintingUI;

		void SetupGUI();

	signals:
		void DoProjection(const bool &DoProjecting);
		void UpdateProjectionFile(const QString &ProjectionFile);

		void IsCameraConnected(const bool &IsCameraConnected);
		void IsRequestCamProCalibrate(const bool &IsRequestCamProCalibrate);
		void IsHomographyTransformChanged(const cv::Mat &Homography, const int &Mode);

		void SetSerialRequest(const QString &s);

	private slots:
		void Connect();
		void Projection();
		void SerialConnect();
		void SerialDisconnect();

		void SetManualCalibrationProCam();
		void OnScreenCalibCamPos(const int &X, const int &Y, const int &I);
		void OnScreenCalibProPos(const int &X, const int &Y, const int &I);

		void CalCamProCalibrationOnScreen();

		void UpdateIsCameraViewVisible(const bool &IsWidgetVisible);
		void UpdateViewSizeOnScreen(const int &W, const int &H);

		void OnScreenDisplayTimer(const QString &Timer);

		void SaveCameraFrame();
		void GetCameraFrame(const QImage &CameraFrame);
		void SetProjectionFile();

		void HandleFanOn();
		void HandleFanOff();
		void HandleTableManualOn();
		void HandleTableAutoOn();
		void HandleTableOff();
		void UpdateSerialStatus(const QString &s);
		void UpdateSerialResponse(const QString &s);

		void HandlePaintingUI();

		void License();
};
