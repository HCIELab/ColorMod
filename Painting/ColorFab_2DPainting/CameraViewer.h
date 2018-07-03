#pragma once
#ifndef CAMERAVIEWER_H
#define CAMERAVIEWER_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QPainter>
#include <opencv2/opencv.hpp>
#include "ui_CameraViewer.h"

class CameraViewer : public QMainWindow
{
	Q_OBJECT

	public:
		explicit CameraViewer(QWidget *parent);
		~CameraViewer();

	protected:
		// Override method inherit from QGui::QCloseEvent
		// NOTE: we override for proof the system work fine in almost conditions
		virtual void closeEvent(QCloseEvent *event);
		void mousePressEvent(QMouseEvent *event);

	private:
		Ui::CameraViewer *ui;
		bool isCameraConnected;
		bool isRequestCamProCalibrate;
		bool isRequestSurfaceModelBorderPoints;
		bool isRequestGetSurfaceModelPoints;
		bool isRequestGetFinalizePushingPosition;
		int numberOfPointRequest;
		int mousePressCount;

	signals:
		void IsWidgetVisible(const bool &closeEvent);
		void MousePressPosition(const int &X, const int &Y, const int &I);
		void MousePressSurfaceBorderPoint(const int &X, const int &Y, const int &I);
		void MousePressSurfacePoint(const int &X, const int &Y, const int &I);
		void MousePressSurfacePushingPoint(const int &X, const int &Y);

	private slots:
		void UpdateFrame(const QImage &Frame);
		void UpdateCapturedSize(const int &Width, const int &Height);
		void UpdateIsCameraConnected(const bool &IsCameraConnected);
		void UpdateIsRequestCamProCalibrate(const bool &IsRequestCamProCalibrate);
		void UpdateIsRequestGetSurfaceModelBorderPoints(const bool &IsRequestGetSurfaceModelBorderPoints,
														const int &NumberOfPointsRequest);
		void UpdateIsRequestGetSurfaceModelPoints(const bool &IsRequestGetSurfaceModelPoints,
												  const int &NumberOfPointsRequest);
		void UpdateIsRequestGetFinalizePushingPosition(const bool &IsReqeustGetFinalizePusingPosition);
};
#endif // CAMERAVIEWER_H