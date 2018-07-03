#include "PaintingImageAnalysis.h"

PaintingImageAnalysis::PaintingImageAnalysis() {
	//erosion_src = cv::Mat::zeros(cv::Size(1024, 768), CV_8UC3);
	//dilation_src = cv::Mat::zeros(cv::Size(1024, 768), CV_8UC3);
	erosion_src, dilation_src, erosion_dst, dilation_dst;
	//rowNum = greyscale.rows; 
	//colNum = greyscale.cols; 
	colorImg, greyscaleImg;
	erosion_size = 2; //experiment value 
	erosion_elem = 0;
	dilation_size = 3; // experiment value 
	dilation_elem = 0;
}

PaintingImageAnalysis::~PaintingImageAnalysis() {
	colorImg.release();
	greyscaleImg.release();
	erosion_src.release();
	erosion_dst.release();
	dilation_src.release();
	dilation_dst.release();
}

map<string, Mat> PaintingImageAnalysis::extractColors(Mat greyscale) {
	Mat red, blue, green;
	map<string, Mat> colorPos;
	//inRange(greyscale, 85, 90, red);
	inRange(greyscale, 90, 115, red);
	//inRange(greyscale, 81, 84, blue);
	inRange(greyscale, 30, 52, blue);
	//inRange(greyscale, 0, 80, green);
	inRange(greyscale, 81, 95, green);
	Mat red_eroded, green_eroded, blue_eroded;
	red_eroded = Erosion(erosion_size, red);
	green_eroded = Erosion(erosion_size, green);
	blue_eroded = Erosion(erosion_size, blue);
	colorPos["red"] = red_eroded;
	colorPos["green"] = green_eroded;
	colorPos["blue"] = blue_eroded;
	return colorPos;
}

map<string, vector<pair<int, int>>> PaintingImageAnalysis::matchUserPainting(Mat userPainting,
	map<string, Mat> colorPos) 
{
	int rowNum = userPainting.rows;
	int colNum = userPainting.cols;
	map<string, vector<pair<int, int>>> match;
	for (int i = 0; i < rowNum; i++) 
	{
		for (int j = 0; j < colNum; j++) 
		{
			// color format: BGR 
			if (userPainting.at<cv::Vec3b>(i, j)[0] > 150
				&& userPainting.at<cv::Vec3b>(i, j)[1] < 100
				&& userPainting.at<cv::Vec3b>(i, j)[2] < 100) 
			{
				if (colorPos["blue"].at<uchar>(i, j) == 255) 
				{
					match["blue"].push_back(make_pair(i, j));
				}
			}
			else if (userPainting.at<cv::Vec3b>(i, j)[1] > 150
				&& userPainting.at<cv::Vec3b>(i, j)[0] < 100
				&& userPainting.at<cv::Vec3b>(i, j)[2] < 100) 
			{
				if (colorPos["green"].at<uchar>(i, j) == 255) 
				{
					match["green"].push_back(make_pair(i, j));
				}
			}
			else if (userPainting.at<cv::Vec3b>(i, j)[2] > 150
				&& userPainting.at<cv::Vec3b>(i, j)[0] < 100
				&& userPainting.at<cv::Vec3b>(i, j)[1] < 100) 
			{
				if (colorPos["red"].at<uchar>(i, j) == 255) 
				{
					match["red"].push_back(make_pair(i, j));
				}
			}
		}
	}
	return match;
}

Mat PaintingImageAnalysis::createImg4Proj(map<string, vector<pair<int, int>>> match, int rowNum, int colNum) 
{
	Mat img4Proj(rowNum, colNum, CV_8UC3, Scalar(0, 0, 0));
	for (map<string, vector<pair<int, int>>>::iterator it = match.begin(); it != match.end(); it++) 
	{
		vector<pair<int, int>> coords = it->second;
		for (unsigned int p = 0; p < coords.size(); p++) 
		{
			int i = coords[p].first;
			int j = coords[p].second;
			img4Proj.at<cv::Vec3b>(i, j)[0] = 255;
			img4Proj.at<cv::Vec3b>(i, j)[1] = 255;
			img4Proj.at<cv::Vec3b>(i, j)[2] = 255;
		}
	}
	Mat img_dilated = Dilation(dilation_size, img4Proj);
	return img_dilated;
}

Mat PaintingImageAnalysis::Erosion(int erosion_size, Mat erosion_src) 
{
	Mat erosion_dst;
	int erosion_type = MORPH_RECT;
	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	erode(erosion_src, erosion_dst, element);
	return erosion_dst;
}

Mat PaintingImageAnalysis::Dilation(int dilation_size, Mat dilation_src) 
{
	Mat dilation_dst;
	int dilation_type = MORPH_RECT;
	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));

	dilate(dilation_src, dilation_dst, element);
	Mat invert;
	bitwise_not(dilation_dst, invert);
	return invert;
}