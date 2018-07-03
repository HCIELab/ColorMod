#pragma once
#ifndef RENDERHELPER_H
#define RENDERHELPER_H

#include <opencv2/opencv.hpp>

#include <QtGui/QBrush>
#include <QtWidgets/QWidget>

class RenderHelper
{
public:
	RenderHelper();
	~RenderHelper();
	void RenderSetup(QImage &ProjectionImage = QImage());
	void paint(QPainter *painter, QPaintEvent *event);
	void paint(QPainter *painter);
	void SetScreen(int W, int H);

private:
	bool IsDrawing;
	int ScreenW, ScreenH;

	QPixmap renderBackground;
	QBrush screenBackground;
};
#endif // RENDERHELPER_H