#pragma once

#include <OpenNI.h> // OpenNI2 Header file
#include <NiTE.h> // NITE2 Header file
#include <opencv2/opencv.hpp>
#include "MotionData.h"


class KinectIO
{
public:
	KinectIO(void);
	~KinectIO(void);
	bool init();
	int getUserNumber();
	void setVideo(cv::Mat& mat);
	void rec(std::vector<MotionData>& data, cv::Mat& colorMat, cv::Mat&depthMat, bool show);
	nite::UserTracker& getUserTracker();

private:
	openni::Status statusOpenNI;
	openni::Device device;
	openni::VideoStream colorStream;
	openni::VideoStream depthStream;
	nite::Status statusNITE;
	nite::UserTracker userTracker;

};

