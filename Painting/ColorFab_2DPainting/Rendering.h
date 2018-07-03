#pragma once
#ifndef RENDERSCREEN_H
#define RENDERSCREEN_H

#include <QtOpenGL/QGLWidget>
#include <QOpenGLPaintDevice>
#include <qlabel.h>

#include <qopenglframebufferobject.h>
#include <qsurfaceformat.h>
#include <qwindow.h>

class RenderHelper;

class Rendering : public QGLWidget
{
	Q_OBJECT

	public:
		QImage offscreen;

		Rendering(RenderHelper *renderHelper, QWidget *parent);
		~Rendering();

		void Initialize(int ScreenSize_W, int ScreenSize_H);

	signals:
		void SetRendering(const QImage &OffscreenRendered);

		public slots:
		void UpdateOffScreenRendering();

	private:
		RenderHelper *helper;
		QPixmap offrender;

		QSurfaceFormat format;
		QWindow window;
		QOpenGLContext *context;
		QOpenGLFramebufferObject *fbo;
		QOpenGLFramebufferObjectFormat fboFormat;
		QRect drawRect;
		QSize drawRectSize;
};
#endif // RENDERSCREEN_H