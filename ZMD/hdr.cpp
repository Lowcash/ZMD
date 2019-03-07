#include "pch.h"
#include "hdr.h"

const cv::Vec3f HDR::getWk(cv::Vec3f Ik) {
	cv::Vec3f upper(
		std::pow(Ik.val[0] - m_mi * 255.0f, 2),
		std::pow(Ik.val[1] - m_mi * 255.0f, 2),
		std::pow(Ik.val[2] - m_mi * 255.0f, 2)
	);

	float downer = std::pow(2 * (m_sigma * 255.0f), 2);
	
	cv::Vec3f beforeNormalization(
		std::exp(-(upper[0] / downer)),
		std::exp(-(upper[1] / downer)),
		std::exp(-(upper[2] / downer))
	);

	float sum = beforeNormalization.val[0] + beforeNormalization.val[1] + beforeNormalization.val[2];
	
	return beforeNormalization / sum;
}

const cv::Mat HDR::getR(std::vector<cv::Mat> source) {
	cv::Mat result(source.at(0).size(), source.at(0).type());

	result.forEach<cv::Vec3f>([&](cv::Vec3f &pixel, const int *position) {
		int y = position[0], x = position[1];

		for (int i = 0; i < source.size(); i++) {
			cv::Vec3f sourceColor = source[i].at<cv::Vec3f>(y, x);
			cv::Vec3f Wk = getWk(sourceColor);

			result.at<cv::Vec3f>(y, x) += cv::Vec3f(
				Wk.val[0] * sourceColor.val[0],
				Wk.val[1] * sourceColor.val[1],
				Wk.val[2] * sourceColor.val[2]
			);
		}
	});

	return result / 255.0f;
}

const void HDR::calculateHDR(std::vector<cv::Mat> source, cv::Mat &destination) {
	for (int i = 0; i < source.size(); i++) {
		source.at(i).convertTo(source.at(i), CV_32FC3);
	}

	destination = getR(source);
}