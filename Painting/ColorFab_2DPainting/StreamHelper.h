#pragma once
#define __CONVERSION_OPENCV_H__
#ifdef  __CONVERSION_OPENCV_H__

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>

namespace Conversion
{
	/* Function to convert Camera points to Projector points using homography transform */
	inline std::vector<cv::Point2f> CtoPvec2f(const std::vector<cv::Point2f> &CamPts, const cv::Mat &Homography)
	{
		if (CamPts.size() > 0)
		{
			std::vector<cv::Point2f> ProPts(CamPts.size());
			// Process perspective transform using homography matrix
			// NOTE: This function convert to Point2f (float)
			cv::perspectiveTransform(CamPts, ProPts, Homography);
			return ProPts;
		}
		else
			return std::vector<cv::Point2f>();
	}

	/* Function to convert Camera Rect (ROI) to Projector Rect (ROI) using homography transform */
	inline cv::Rect CtoPRect(const cv::Rect &CamRect, const cv::Mat &Homography)
	{
		std::vector<cv::Point2f> VecCamRect;
		VecCamRect.push_back(cv::Point2f((float)CamRect.x, (float)CamRect.y));
		VecCamRect.push_back(cv::Point2f((float)CamRect.x + (float)CamRect.width,
			(float)CamRect.y + (float)CamRect.height));
		std::vector<cv::Point2f> VecProRect(VecCamRect.size());
		// Process perspective transform using homography matrix
		// NOTE: This function convert to Point2f (float)
		cv::perspectiveTransform(VecCamRect, VecProRect, Homography);

		// Convert vector to Rect of ROI
		cv::Rect ProRect;
		ProRect.x = VecProRect[0].x;
		ProRect.y = VecProRect[0].y;
		ProRect.width = abs(VecProRect[1].x - VecProRect[0].x);
		ProRect.height = abs(VecProRect[1].y - VecProRect[0].y);

		return ProRect;
	}

	inline cv::Point2f CtoPpoint2f(const cv::Point2f &CamPoint, const cv::Mat &Homography)
	{
		std::vector<cv::Point2f> VecCam;
		VecCam.push_back(CamPoint);
		std::vector<cv::Point2f> VecPro(VecCam.size());
		// Process perspective transform using homography matrix
		// NOTE: This function convert to Point2f (float)
		cv::perspectiveTransform(VecCam, VecPro, Homography);

		// Convert Back to point2f
		return VecPro[0];
	}
}
#endif // __CONVERSION_OPENCV_H__