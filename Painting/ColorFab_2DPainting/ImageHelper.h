#pragma once
#ifndef __ASM_OPENCV_H__
#define __ASM_OPENCV_H__

/*
Functions to convert between OpenCV's cv::Mat and Qt's QImage and QPixmap.

Andy Maloney
23 November 2013
http://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap
*/

#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtDebug>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

namespace ImageHelper
{
	inline QImage cvMatToQImage(const cv::Mat &In, const bool &IsRGBformat)
	{
		switch (In.type())
		{
			// 8-bit, 4 channel
		case CV_8UC4:
		{
			//QImage image(In.data, In.cols, In.rows, In.step, QImage::Format_RGB32);
			QImage image(In.data, In.cols, In.rows, In.step, QImage::Format_ARGB32_Premultiplied);
			return image;
		}

		// 8-bit, 3 channel
		case CV_8UC3:
		{
			if (!IsRGBformat)
				cv::cvtColor(In, In, CV_BGR2RGB, 3);
			QImage image(In.data,
						 In.cols,
						 In.rows,
						 In.step,
						 QImage::Format_RGB888);
			return image;
		}

		// 8-bit, 1 channel
		case CV_8UC1:
		{
			static QVector<QRgb>  sColorTable;

			// only create our color table once
			if (sColorTable.isEmpty())
			{
				for (int i = 0; i < 256; ++i)
					sColorTable.push_back(qRgb(i, i, i));
			}

			QImage image(In.data, In.cols, In.rows, In.step, QImage::Format_Indexed8);

			image.setColorTable(sColorTable);
			return image;
		}

		default:
			qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << In.type();
			break;
		}

		return QImage();
	}

	inline QPixmap cvMatToQPixmap(const cv::Mat &In, const bool &IsRGBformat)
	{
		return QPixmap::fromImage(cvMatToQImage(In, IsRGBformat));
	}

	// If inImage exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inImage's
	// data with the cv::Mat directly
	//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
	inline cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData = true)
	{
		switch (inImage.format())
		{
			// 8-bit, 4 channel
		case QImage::Format_RGB32:
		{
			cv::Mat  mat(inImage.height(),
				inImage.width(),
				CV_8UC4,
				const_cast<uchar*>(inImage.bits()),
				inImage.bytesPerLine());
			return (inCloneImageData ? mat.clone() : mat);
		}

		// 8-bit, 3 channel
		case QImage::Format_RGB888:
		{
			if (!inCloneImageData)
				qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";
			QImage swapped = inImage.rgbSwapped();
			return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
		}

		// 8-bit, 1 channel
		case QImage::Format_Indexed8:
		{
			cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine());
			return (inCloneImageData ? mat.clone() : mat);
		}

		default:
			qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
			break;
		}

		return cv::Mat();
	}

	// If inPixmap exists for the lifetime of the resulting cv::Mat, pass false to inCloneImageData to share inPixmap's data
	// with the cv::Mat directly
	//    NOTE: Format_RGB888 is an exception since we need to use a local QImage and thus must clone the data regardless
	inline cv::Mat QPixmapToCvMat(const QPixmap &inPixmap, bool inCloneImageData = true)
	{
		return QImageToCvMat(inPixmap.toImage(), inCloneImageData);
	}
}

#endif // __ASM_OPENCV_H__