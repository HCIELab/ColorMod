#pragma once
#ifndef IMAGEANALYSIS_H 
#define IMAGEANALYSIS_H 

#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>#
#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;
using namespace cv;

class ImageAnalysis {

public: 
	ImageAnalysis(); 
	map<string, Mat> extractColors(Mat greyscale);
	map<string, vector<pair<int, int>>> matchUserPainting(Mat userPainting,
		map<string, Mat> colorPos);
	Mat createImg4Proj(map<string, vector<pair<int, int>>> match, int rowNum, int colNum);
	~ImageAnalysis();

private: 
	Mat Erosion(int erosion_size, Mat erosion_src);
	Mat Dilation(int dilation_size, Mat dilation_src);


	Mat erosion_src, dilation_src, erosion_dst, dilation_dst;
	Mat colorImg, greyscaleImg;
	int erosion_size; //experiment value 
	int erosion_elem;
	int dilation_size; // experiment value 
	int dilation_elem;
};

#endif // !IMAGEANALYSIS_H 
