#include "RenderHelper.h"
#include "Config.h"

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>

#include <omp.h>

RenderHelper::RenderHelper()
{
	// Initlaize projection screen size
	this->ScreenW = 1024;
	this->ScreenH = 768;

	this->IsDrawing = false;

	// Default parameter(s)
	renderBackground = QPixmap(ScreenW, ScreenH);
	screenBackground = QBrush();
}

RenderHelper::~RenderHelper()
{
	renderBackground.~QPixmap();
}

void RenderHelper::RenderSetup(QImage &ProjectionImage)
{
	// If the process in drawing, we ignore to update parameters
	if (IsDrawing)	return;

	// 1. Get background image to render
	if (ProjectionImage.isNull())
	{
		// -- If no input image, we render black color
		renderBackground = QPixmap(ScreenW, ScreenH);
		renderBackground.fill(Qt::darkBlue);

		// -- Set to QBrush to draw in a scene
		screenBackground = QBrush(renderBackground);
	}
	else
	{
		// -- Set input image to draw in a scene
		screenBackground = QBrush(ProjectionImage);
	}

	return;
}

void RenderHelper::paint(QPainter *painter)
{
	// 1. Set drawing status (prevent update parameters)
	this->IsDrawing = true;

	// 2. Render surface feedback (this is default mode, to turn it black, change the texture)
	painter->fillRect(0, 0, ScreenW, ScreenH, screenBackground);

	// 3. Disable drawing event to allow update parameters
	this->IsDrawing = false;

	return;
}

void RenderHelper::SetScreen(int W, int H)
{
	// Set screen size
	this->ScreenW = W;
	this->ScreenH = H;

	return;
}