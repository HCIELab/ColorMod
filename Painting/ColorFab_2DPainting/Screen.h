#pragma once
#ifndef SCREEN_H
#define SCREEN_H

#include <QtWidgets/QMainWindow>
#include <QtGui/QPainter>
#include <opencv2/opencv.hpp>
#include "ui_Screen.h"

class Screen : public QMainWindow
{
	Q_OBJECT

	public:
		explicit Screen(QWidget *parent = 0);
		~Screen();

	protected:
		void mousePressEvent(QMouseEvent *event);

	private:
		Ui::Screen *ui;
		bool isRequestCamProCalibrate;
		int mousePressCount;

	signals:
		void MousePressPosition(const int &X, const int &Y, const int &I);

	private slots :
		void UpdateRendering(const QImage &Frame);
		void UpdateIsRequestCamProCalibrate(const bool &IsRequestCamProCalibrate);
};
#endif // SCREEN_H