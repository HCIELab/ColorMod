#pragma once

namespace def
{
	// Camera Model(s)
	enum
	{
		REALSENSE_SR300 = 1,
		POINTGREY_CHAMELEON3 = 2,
	};

	// FPS
	enum
	{
		FPS015 = 15,
		FPS030 = 30,
		FPS060 = 60,
		FPS120 = 120,
	};

	// Calibration
	enum
	{
		CALIB_CAMTOPRO = 1,
	};

	// Render
	enum
	{
		NO_RENDER_FINGER = 0,
	};
}

// -- Application version
#define APP_VERSION 1.0
// -- Distance from border of desktop to panel (x, y)
#define M_APP_QT_POS_X 10
#define M_APP_QT_POS_Y 40
// -- Camera size (i.e. for processing image -> map to projection image)
#define M_APP_CAM_WIN_SIZE_W 640
#define M_APP_CAM_WIN_SIZE_H 480
// -- Anduino handle time
#define ARDUINO_WAIT_TIME 1000
// -- PaintingUI (number of image that captured)
#define N_IMAGE_CAPTURED_FOR_PAINT 4