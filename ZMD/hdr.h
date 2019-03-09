#pragma once

class HDR {
private:
	const float m_mi = 0.5f;
	const float m_sigma = 0.2f;
private:
	const float getWk(float Ik);
	const cv::Mat getR(std::vector<cv::Mat> source, std::vector<cv::Mat> sourceGrayscale);
public:
	const void calculateHDR(std::vector<cv::Mat> source, cv::Mat &destination);
};

