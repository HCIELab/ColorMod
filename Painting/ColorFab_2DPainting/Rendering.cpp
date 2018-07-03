#include "Rendering.h"
#include "RenderHelper.h"
#include "Config.h"

#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

// Implementation technique based on : http://sysmagazine.com/posts/168911/
Rendering::Rendering(RenderHelper *renderHelper, QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
	, helper(renderHelper)
	, fbo(NULL)
	, context(NULL)
{
	// 1. Set default instance
	helper = renderHelper;

	// 2. Set surface object to draw as offscreen
	// -- We can use  QWindow or QGLWidget instead
	format.setMajorVersion(3);
	format.setMinorVersion(3);
	window.setSurfaceType(QWindow::OpenGLSurface);
	window.setFormat(format);
	window.create();

	// 3. Create GL context
	context = new QOpenGLContext();
	context->setFormat(format);

	// 4. Set screen size
	drawRect.setRect(0, 0, 1024, 768);
	drawRectSize = drawRect.size();

	// 5. Set framebuffer format, to draw as image 'combineddepthstencil' is required
	fboFormat.setSamples(16);
	fboFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
}

Rendering::~Rendering()
{
	if (helper != NULL) delete helper;
	if (fbo != NULL) delete fbo;
	if (context != NULL) delete context;
}

void Rendering::Initialize(int ScreenSize_W, int ScreenSize_H)
{
	// Set screen size
	drawRect.setRect(0, 0, ScreenSize_W, ScreenSize_H);
	drawRectSize = drawRect.size();

	return;
}

void Rendering::UpdateOffScreenRendering()
{
	// 1. Make the context current on the created window
	context->makeCurrent(&window);
	// 2. Create frame buffer on the image size and its format
	fbo = new QOpenGLFramebufferObject(drawRectSize, fboFormat);
	fbo->bind();

	// 3. Off-screen painting
	QOpenGLPaintDevice device(drawRectSize);
	QPainter painter;
	painter.begin(&device);
	painter.setRenderHints(QPainter::Antialiasing);
	helper->paint(&painter);
	painter.end();

	// 5. We exlicitely have to end the painting, all paintin commands have been flushed
	// -- Holds FBO
	fbo->release();

	// 6. Save FBO to QImage
	offscreen = fbo->toImage();
	delete fbo;

	emit SetRendering(offscreen);
}