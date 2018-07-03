#include "PaintingArea.h"

PaintingArea::PaintingArea(QWidget *parent)
	: QWidget(parent)
{
	//brushIcon = QPixmap("brush.png"); 
	brush = QCursor(Qt::CrossCursor);
	setAttribute(Qt::WA_StaticContents);
	setCursor(brush);
	modified = false;
	scribbling = false;
	//erasing = false; 
	myPenWidth = 20;
	myPenColor = Qt::blue;
}

bool PaintingArea::openImage(const QString &fileName)
{
	QImage loadedImage;
	if (!loadedImage.load(fileName)) return false;

	QSize newSize = loadedImage.size().expandedTo(size());
	resizeImage(&loadedImage, newSize);
	image = loadedImage;
	modified = false;
	update();
	return true;
}

bool PaintingArea::saveImage(const QString &fileName, const char *fileFormat)
{
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());

	if (visibleImage.save(fileName, fileFormat)) {
		modified = false;
		return true;
	}
	else {
		return false;
	}
}

void PaintingArea::setPenColor(const QColor &newColor)
{
	myPenColor = newColor;
}

void PaintingArea::setPenWidth(int newWidth)
{
	myPenWidth = newWidth;
}

void PaintingArea::clearImage()
{
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();
}

void PaintingArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) 
	{
		lastPoint = event->pos();
		scribbling = true;
	}
}

void PaintingArea::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) && scribbling)
		drawLineTo(event->pos());
}

void PaintingArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && scribbling) 
	{
		drawLineTo(event->pos());
		scribbling = false;
	}
}

void PaintingArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect, image, dirtyRect);
}

void PaintingArea::resizeEvent(QResizeEvent *event)
{
	if (width() > image.width() || height() > image.height()) 
	{
		int newWidth = qMax(width() + 128, image.width());
		int newHeight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHeight));
		update();
	}

	QWidget::resizeEvent(event);
}

void PaintingArea::drawLineTo(const QPoint &endPoint)
{
	QPainter painter(&image);
	//QPainter painter(&painting);
	painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endPoint);
	modified = true;

	int rad = (myPenWidth / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}

void PaintingArea::resizeImage(QImage *image, const QSize &newSize)
{
	if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}