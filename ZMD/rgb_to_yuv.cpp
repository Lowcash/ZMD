#include "pch.h"
#include "rgb_to_yuv.h"

const void RGBToYUV::startConversion(cv::Mat source, cv::Mat &destinationY, cv::Mat &destinationU, cv::Mat &destinationV) {
	source.convertTo(source, CV_32FC3);

	destinationY = cv::Mat(source.size(), source.type());
	destinationU = cv::Mat(source.size(), source.type());
	destinationV = cv::Mat(source.size(), source.type());

	for (int y = 0; y < source.rows; y++) {
		for (int x = 0; x < source.cols; x++) {
			cv::Mat transformedValue = m_yuvTransformationMatrix * (cv::Mat)source.at<cv::Vec3f>(y, x);

			float Y = (transformedValue.at<float>(0, 0) / 255.0f);
			float U = (transformedValue.at<float>(1, 0) / 255.0f);
			float V = (transformedValue.at<float>(2, 0) / 255.0f);

			destinationY.at<cv::Vec3f>(y, x) = cv::Vec3f(Y, Y, Y);

			destinationU.at<cv::Vec3f>(y, x) = cv::Vec3f(0.5f - U, 0.5f + U, 0);
			destinationV.at<cv::Vec3f>(y, x) = cv::Vec3f(0, 0.5f + V, 0.5f - V);
		}
	}
}