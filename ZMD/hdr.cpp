#include "pch.h"
#include "hdr.h"

const cv::Vec3f HDR::getWk(cv::Vec3f Ik) {
	cv::Vec3f upper(
		std::pow(Ik.val[0] - m_mi * 255, 2),
		std::pow(Ik.val[1] - m_mi * 255, 2),
		std::pow(Ik.val[2] - m_mi * 255, 2)
	);

	float downer = std::pow(2 * (m_sigma * 255), 2);

	return cv::Vec3f(
		std::exp(-(upper[0] / downer)),
		std::exp(-(upper[1] / downer)),
		std::exp(-(upper[2] / downer))
	);
}

const cv::Mat HDR::getR(std::vector<cv::Mat> source) {
	for (int i = 0; i < source.size(); i++) {
		source.at(i).convertTo(source.at(i), CV_32FC3);
	}

	cv::Mat result(source.at(0).size(), source.at(0).type());

	for (int y = 0; y < result.rows; y++) {
		for (int x = 0; x < result.cols; x++) {
			for (int i = 0; i < source.size(); i++) {
				cv::Vec3f sourceColor = source[i].at<cv::Vec3f>(y, x);

				result.at<cv::Vec3f>(y, x) += getWk(sourceColor) * sourceColor;
			}
		}
	}

	return result;
}

const void HDR::calculateHDR(std::vector<cv::Mat> source, cv::Mat &destination) {
	destination = getR(source);
}