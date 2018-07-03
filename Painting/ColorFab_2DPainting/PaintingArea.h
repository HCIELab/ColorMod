#pragma once

#ifndef PAINTINGAREA_H
#define PAINTINGAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QtWidgets>

class PaintingArea : public QWidget
{
	Q_OBJECT

	public:
		PaintingArea(QWidget *parent = 0);

		bool openImage(const QString &fileName);
		bool saveImage(const QString &fileName, const char *fileFormat);
		void setPenColor(const QColor &newColor);
		void setPenWidth(int newWidth);
		//void setEraserSize(int newSize); 

		bool isModified() const { return modified; }
		QColor penColor() const { return myPenColor; }
		int penWidth() const { return myPenWidth; }

		public slots:
		void clearImage();

	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;
		void paintEvent(QPaintEvent *event) override;
		void resizeEvent(QResizeEvent *event) override;

	private:
		void drawLineTo(const QPoint &endPoint);
		//void erase(const QPoint &endPoint);
		void resizeImage(QImage *image, const QSize &newSize);

		bool modified;
		bool scribbling;
		//bool eraseMode; 
		//bool erasing; 
		int myPenWidth;
		//int myEraserSize; 
		QColor myPenColor;
		QImage image;
		QCursor brush;
		//QImage painting; 
		QPoint lastPoint;
		QPixmap brushIcon;
};

#endif