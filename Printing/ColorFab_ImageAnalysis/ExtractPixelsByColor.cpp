#include <opencv2/opencv.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;

Mat erosion_src, dilation_src, erosion_dst, dilation_dst;
int erosion_size = 0;
int erosion_elem = 0;
int dilation_elem = 0;
int dilation_size = 0;
int const max_elem = 2;
int const max_kernel_size = 21;

void optimizeErosion(Mat src);
void generateFile(Mat image); 
void Erosion(int, void*);
void Dilation(int, void*);
void matchUserPainting(Mat userPainting); 
Mat createImg4Proj(int width, int height); 

int main() {
	Mat original = imread("grid_RGBW.jpg", CV_LOAD_IMAGE_COLOR);
	Mat img;
	for (int i = 1; i < 9; i = i + 2) {
		GaussianBlur(original, img, Size(i, i), 0, 0);
	}
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	Mat greyscaleimage;
	cv::cvtColor(img, greyscaleimage, CV_BGR2GRAY);
	imwrite("greyscale.jpg", greyscaleimage); 
	namedWindow("greyscale", WINDOW_NORMAL);
	imshow("greyscale", greyscaleimage);

	Mat red, blue, green;
	//inRange(greyscaleimage, 0, 80, green); 
	//inRange(greyscaleimage, 82, 84, blue); 
	//inRange(greyscaleimage, 84, 90, red);
	//src = red;
	//cv::erode(red, erosion_dst, Mat()); 
	//generateFile(erosion_dst);

	//Mat userPainting = imread("test.jpg", CV_LOAD_IMAGE_COLOR); 
	//matchUserPainting(userPainting); 

	int width = img.cols; 
	int height = img.rows; 
	Mat img4Proj = createImg4Proj(width, height); 

	namedWindow("Dilation Demo", CV_WINDOW_AUTOSIZE);
	//namedWindow("Before Dilation", CV_WINDOW_AUTOSIZE); 
	//imshow("Dilation", img4Proj); 
	dilation_src = img4Proj;
	/// Create Dilation Trackbar
	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo",
		&dilation_elem, max_elem,
		Dilation);

	createTrackbar("Kernel size:\n 2n +1", "Dilation Demo",
		&dilation_size, max_kernel_size,
		Dilation);
	Dilation(0, 0); 
	////namedWindow("Green", WINDOW_NORMAL); 
	////namedWindow("Blue", WINDOW_NORMAL);
	//namedWindow("Red", WINDOW_NORMAL);
	////imshow("Green", green); 
	////imshow("Blue", blue);
	//imshow("Red", red); 
	//namedWindow("erode", WINDOW_AUTOSIZE); 
	//imshow("erode", erosion_dst); 
	waitKey(0);
	return 0;

}


void optimizeErosion(Mat src) {
	Mat erosion_red;
	namedWindow("Erosion Demo", WINDOW_AUTOSIZE);

	createTrackbar("Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo",
		&erosion_elem, max_elem,
		Erosion);

	createTrackbar("Kernel size:\n 2n +1", "Erosion Demo",
		&erosion_size, max_kernel_size,
		Erosion);

	Erosion(0, 0);
}

void Erosion(int, void*){
	int erosion_type;
	if (erosion_elem == 0) { erosion_type = MORPH_RECT; }
	else if (erosion_elem == 1) { erosion_type = MORPH_CROSS; }
	else if (erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(erosion_type,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));

	/// Apply the erosion operation
	erode(erosion_src, erosion_dst, element);
	imshow("Erosion Demo", erosion_dst);
}

void generateFile(Mat image) {
	set<pair<int, int>> redPixels; 
	ofstream red("red.txt");
	if (red.is_open()) {
		for (int i = 0; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				if (image.at<uchar>(i, j) == 255) {
					red << i << " " << j << "\n";
				}
			}
		}
	}
	red.close();
}

void matchUserPainting(Mat userPainting) {
	ofstream redMatch("redMatch.txt"); 
	ifstream red("red.txt"); 
	int c = userPainting.cols; 
	int r = userPainting.rows; 
	int i, j; 
	if (redMatch.is_open()) {
		if (red.is_open()) {
			while (red >> i >> j) {
				if (userPainting.at<cv::Vec3b>(i, j)[2] > 150 && userPainting.at<cv::Vec3b>(i, j)[1] <100 && userPainting.at<cv::Vec3b>(i, j)[0] <100){
					redMatch << i << " " << j << "\n"; 
				}
			}
			red.close(); 
		}
		
	}
	redMatch.close();
}

Mat createImg4Proj(int width, int height) {
	ifstream redMatch("redMatch.txt"); 
	Mat img4Proj(width, height, CV_8UC3, Scalar(0, 0, 0)); 
	int i, j; 
	if (redMatch.is_open()) {
		while (redMatch >> i >> j) {
			img4Proj.at<cv::Vec3b>(i, j)[0] = 255; 
			img4Proj.at<cv::Vec3b>(i, j)[1] = 255;
			img4Proj.at<cv::Vec3b>(i, j)[2] = 255;
		}
	}
	redMatch.close(); 
	//namedWindow("projection", WINDOW_AUTOSIZE); 
	//imshow("projection", img4Proj); 

	return img4Proj; 
}

void Dilation(int, void*){
	int dilation_type;
	if (dilation_elem == 0) { dilation_type = MORPH_RECT; }
	else if (dilation_elem == 1) { dilation_type = MORPH_CROSS; }
	else if (dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

	Mat element = getStructuringElement(dilation_type,
		Size(2 * dilation_size + 1, 2 * dilation_size + 1),
		Point(dilation_size, dilation_size));
	/// Apply the dilation operation
	dilate(dilation_src, dilation_dst, element);
	Mat show; 
	bitwise_not(dilation_dst, show);
	imshow("Dilation Demo", show);
}
