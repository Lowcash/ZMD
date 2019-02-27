#pragma once

class RGBToYUV {
private:
	const cv::Mat m_yuvTransformationMatrix = (cv::Mat_<float>(3, 3) <<
		0.299, 0.587, 0.114,
		-0.147, -0.289, 0.436,
		0.615, -0.515, -0.100);
public:
	const void startConversion(cv::Mat source, cv::Mat &destinationY, cv::Mat &destinationU, cv::Mat &destinationV);
};

