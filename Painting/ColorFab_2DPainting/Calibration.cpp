#include <opencv2/calib3d/calib3d.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Calibration.h"
#include "Config.h"

using namespace std;
using namespace cv;

Calibration::Calibration()
{
	// Setup calibration file
	CamProCalibrationFile = "Data/Calibration/CamProCalibration.txt";
}

Calibration::~Calibration()
{
	camPts.clear();
	proPts.clear();
	CamToPro_Matrix.release();

	depthPts.clear();
	camROIPts.clear();
	DepthToCam_Matrix.release();
};

bool Calibration::LoadCalibrateFile(int CalibrateType)
{
	/* LOAD CALIBRATE FILE */
	float cam_x, cam_y, pro_x, pro_y;
	vector<cv::Point2f> tmp_camPts;
	vector<cv::Point2f> tmp_proPts;

	string file;
	if (CalibrateType == def::CALIB_CAMTOPRO) file = CamProCalibrationFile;

	ifstream input(file);
	// If input file is founded, load them to process
	if (input.is_open())
	{
		for (int i = 0; i < 4; i++)
		{
			input >> cam_x;
			input.ignore();
			input >> cam_y;
			input.ignore();
			input >> pro_x;
			input.ignore();
			input >> pro_y;

			// Save to vector matrix (used in calibration and cameraROI process)
			tmp_camPts.push_back(Point2f(cam_x, cam_y));
			tmp_proPts.push_back(Point2f(pro_x, pro_y));
		}
		input.close();

		// Process homography matrix from file
		return ProcessHomography(tmp_camPts, tmp_proPts, CalibrateType);
	}
	else
	{
		return false;
	}
}

void Calibration::WriteCalibrateFile(const std::vector<cv::Point2f> &Cam,
									 const std::vector<cv::Point2f> &Pro,
									 int &CalibrateType)
{
	/* WRITE CALIBRATE FILE */
	// Check valid data
	if (Cam.size() != 4)
		return;
	if (Pro.size() != 4)
		return;

	std::string file;
	if (CalibrateType == def::CALIB_CAMTOPRO)
	{
		file = CamProCalibrationFile;

		// Save to Cam/Pro matrix for other process
		camPts.clear();
		camPts = Cam;
		proPts.clear();
		proPts = Pro;
	}

	// Write calibrate information to a file, by transcate previous file
	ofstream output(file, ios::trunc);
	if (output.is_open())
	{
		for (int i = 0; i < Cam.size(); i++)
			output << Cam[i].x << "	" << Cam[i].y << "	" << Pro[i].x << "	" << Pro[i].y << "\n";

		output.close();
	}

	return;
}

bool Calibration::ProcessHomography(vector<cv::Point2f> &Cam,
									vector<cv::Point2f> &Pro,
									int &CalibrateType)
{
	/* CALCULATE HOMOGRAPHY MATRIX TRANSFORM */
	// Check valid data
	if (Cam.size() != 4)
		return false;
	if (Pro.size() != 4)
		return false;

	if (CalibrateType == def::CALIB_CAMTOPRO)
	{
		// Calculate Homography matrix between camera and projector
		CamToPro_Matrix = findHomography(Cam, Pro, 0);
		this->isCamProCalibrated = true;

		// Clear current temporary vector
		camPts = Cam;
		proPts = Pro;

		return true;
	}
	else
		return false;
}