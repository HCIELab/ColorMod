#pragma once
#include <opencv2/opencv.hpp>
#include <QtCore/QObject>
#include <vector>

class Calibration : public QObject
{
	Q_OBJECT

private:
	std::string CamProCalibrationFile;

public:
	bool isCamProCalibrated;
	bool isDepthCamCalibrated;

	cv::Mat CamToPro_Matrix;
	cv::Mat DepthToCam_Matrix;

	std::vector<cv::Point2f> camPts;
	std::vector<cv::Point2f> proPts;
	std::vector<cv::Point2f> depthPts;
	std::vector<cv::Point2f> camROIPts;

	Calibration();
	~Calibration();

	bool LoadCalibrateFile(int CalibrateType);
	bool ProcessHomography(std::vector<cv::Point2f> &Cam, std::vector<cv::Point2f> &Pro, int &CalibrateType);
	void WriteCalibrateFile(const std::vector<cv::Point2f> &Cam,
							const std::vector<cv::Point2f> &Pro,
							int &CalibrateType);
};