#include "pch.h"
#include "hdr.h"

const float HDR::getWk(float Ik) {
	return std::exp(-(std::pow(Ik - m_mi * 255.0f, 2) / (2 * std::pow(m_sigma * 255.0f, 2))));
}

const cv::Mat HDR::getR(std::vector<cv::Mat> source, std::vector<cv::Mat> sourceGrayscale) {
	cv::Mat result = cv::Mat::zeros(cv::Size(source.at(0).cols, source.at(0).rows), source.at(0).type());

	result.forEach<cv::Vec3b>([&](cv::Vec3b &pixel, const int *position) {
		int y = position[0], x = position[1];

		float sum = 0;
		std::vector<float> Wks;
		cv::Vec3b R;

		for (int i = 0; i < sourceGrayscale.size(); i++) {
			Wks.push_back(getWk(sourceGrayscale.at(i).at<float>(y, x)));
			sum += Wks.at(i);
		}

		for (int i = 0; i < source.size(); i++) {
			Wks.at(i) /= sum;

			R += Wks.at(i) * source.at(i).at<cv::Vec3b>(y, x);
		}

		result.at<cv::Vec3b>(y, x) = R;
	});

	return result;
}

const void HDR::calculateHDR(std::vector<cv::Mat> source, cv::Mat &destination) {
	std::vector<cv::Mat> sourceGrayscale(source);

	for (int i = 0; i < source.size(); i++) {
		cv::cvtColor(sourceGrayscale.at(i), sourceGrayscale.at(i), CV_BGR2GRAY);
		sourceGrayscale.at(i).convertTo(sourceGrayscale.at(i), CV_32FC1);
	}

	destination = getR(source, sourceGrayscale);
}