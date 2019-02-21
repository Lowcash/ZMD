#include "pch.h"
#include <iostream>

enum ColorIndex { R = 0 , G_1, G_2, B, NONE };

ColorIndex getColorIndex(int col, int row);
cv::Vec3f bayerFilter(cv::Mat bayer, int col, int row);

int main()
{
	cv::Mat source = cv::imread("img/bayer.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat result = cv::Mat(source.size(), CV_32FC3);

	cv::imshow("Grayscale", source);

	for (int y = 2; y < result.rows - 2; y++) {
		for (int x = 2; x < result.cols - 2; x++) {
			result.at<cv::Vec3f>(y, x) = bayerFilter(source, y, x) / 255.0f;
		}
	}

	cv::imshow("Color", result);

	cv::waitKey(0);

	system("PAUSE");
}

cv::Vec3f bayerFilter(cv::Mat bayer, int col, int row) {
	uchar R1, R2, R3, R4;
	uchar G1, G2, G3, G4;
	uchar B1, B2, B3, B4;

	uchar R, G, B;

	switch (getColorIndex(col, row))
	{
		case ColorIndex::B: {
			R1 = bayer.at<uchar>(col - 1, row - 1);
			R2 = bayer.at<uchar>(col + 1, row - 1);
			R3 = bayer.at<uchar>(col + 1, row + 1);
			R4 = bayer.at<uchar>(col - 1, row + 1);

			G1 = bayer.at<uchar>(col, row - 1);
			G2 = bayer.at<uchar>(col + 1, row);
			G3 = bayer.at<uchar>(col, row + 1);
			G4 = bayer.at<uchar>(col - 1, row);

			B1 = bayer.at<uchar>(col, row - 2);
			B2 = bayer.at<uchar>(col + 2, row);
			B3 = bayer.at<uchar>(col, row + 2);
			B4 = bayer.at<uchar>(col - 2, row);

			R = (R1 + R2 + R3 + R4) / 4;
			B = bayer.at<uchar>(col, row);

			if (std::abs(B1 - B3) < std::abs(B2 - B4)) {
				G = (G1 + G3) / 2;
			}
			else if (std::abs(B1 - B3) > std::abs(B2 - B4)) {
				G = (G2 + G4) / 2;
			}
			else {
				G = (G1 + G2 + G2 + G4) / 4;
			}
		}
		break;
		case ColorIndex::G_1: {
			R1 = bayer.at<uchar>(col - 1, row);
			R2 = bayer.at<uchar>(col + 1, row);

			B1 = bayer.at<uchar>(col, row - 1);
			B2 = bayer.at<uchar>(col, row + 1);

			R = (R1 + R2) / 2;
			B = (B1 + B2) / 2;
			G = bayer.at<uchar>(col, row);
		}
		break;
		case ColorIndex::G_2: {
			R1 = bayer.at<uchar>(col, row - 1);
			R2 = bayer.at<uchar>(col, row + 1);

			B1 = bayer.at<uchar>(col - 1, row);
			B2 = bayer.at<uchar>(col + 1, row);

			R = (R1 + R2) / 2;
			B = (B1 + B2) / 2;
			G = bayer.at<uchar>(col, row);
		}
		break;
		case ColorIndex::R: {
			G1 = bayer.at<uchar>(col, row - 1);
			G2 = bayer.at<uchar>(col + 1, row);
			G3 = bayer.at<uchar>(col, row + 1);
			G4 = bayer.at<uchar>(col - 1, row);

			B1 = bayer.at<uchar>(col - 1, row - 1);
			B2 = bayer.at<uchar>(col + 1, row - 1);
			B3 = bayer.at<uchar>(col + 1, row + 1);
			B4 = bayer.at<uchar>(col - 1, row + 1);

			R1 = bayer.at<uchar>(col, row - 2);
			R2 = bayer.at<uchar>(col + 2, row);
			R3 = bayer.at<uchar>(col, row + 2);
			R4 = bayer.at<uchar>(col - 2, row);

			R = bayer.at<uchar>(col, row);

			if (std::abs(R1 - R3) < std::abs(R2 - R4)) {
				G = (G1 + G3) / 2;
			}
			else if (std::abs(R1 - R3) > std::abs(R2 - R4)) {
				G = (G2 + G4) / 2;
			}
			else {
				G = (G1 + G2 + G3 + G4) / 4;
			}

			B = (B1 + B2 + B3 + B4) / 4;
		}
		break;
	}

	return cv::Vec3f(B, G, R);
}

ColorIndex getColorIndex(int col, int row) {
	if (row % 2 == 0 && col % 2 == 0) {
		return ColorIndex::R;
	}
	else if (row % 2 == 0 && col % 2 == 1) {
		return ColorIndex::G_1;
	}
	else if (row % 2 == 1 && col % 2 == 0) {
		return ColorIndex::G_2;
	}
	else {
		return ColorIndex::B;
	}

	return ColorIndex::NONE;
}