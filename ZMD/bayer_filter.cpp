#include "pch.h"
#include "bayer_filter.h"

const cv::Vec3f BayerFilter::bayerFilter(cv::Mat source, const int col, const int row) {
	switch (getColorIndex(col, row))
	{
		case ColorIndex::B: {
			m_R1 = source.at<uchar>(col - 1, row - 1);
			m_R2 = source.at<uchar>(col + 1, row - 1);
			m_R3 = source.at<uchar>(col + 1, row + 1);
			m_R4 = source.at<uchar>(col - 1, row + 1);

			m_G1 = source.at<uchar>(col, row - 1);
			m_G2 = source.at<uchar>(col + 1, row);
			m_G3 = source.at<uchar>(col, row + 1);
			m_G4 = source.at<uchar>(col - 1, row);

			m_B1 = source.at<uchar>(col, row - 2);
			m_B2 = source.at<uchar>(col + 2, row);
			m_B3 = source.at<uchar>(col, row + 2);
			m_B4 = source.at<uchar>(col - 2, row);

			m_R = (m_R1 + m_R2 + m_R3 + m_R4) / 4;
			m_B = source.at<uchar>(col, row);

			if (std::abs(m_B1 - m_B3) < std::abs(m_B2 - m_B4)) {
				m_G = (m_G1 + m_G3) / 2;
			}
			else if (std::abs(m_B1 - m_B3) > std::abs(m_B2 - m_B4)) {
				m_G = (m_G2 + m_G4) / 2;
			}
			else {
				m_G = (m_G1 + m_G2 + m_G2 + m_G4) / 4;
			}
		}
		break;
		case ColorIndex::G_1: {
			m_R1 = source.at<uchar>(col - 1, row);
			m_R2 = source.at<uchar>(col + 1, row);

			m_B1 = source.at<uchar>(col, row - 1);
			m_B2 = source.at<uchar>(col, row + 1);

			m_R = (m_R1 + m_R2) / 2;
			m_B = (m_B1 + m_B2) / 2;
			m_G = source.at<uchar>(col, row);
		}
		break;
		case ColorIndex::G_2: {
			m_R1 = source.at<uchar>(col, row - 1);
			m_R2 = source.at<uchar>(col, row + 1);

			m_B1 = source.at<uchar>(col - 1, row);
			m_B2 = source.at<uchar>(col + 1, row);

			m_R = (m_R1 + m_R2) / 2;
			m_B = (m_B1 + m_B2) / 2;
			m_G = source.at<uchar>(col, row);
		}
		break;
		case ColorIndex::R: {
			m_G1 = source.at<uchar>(col, row - 1);
			m_G2 = source.at<uchar>(col + 1, row);
			m_G3 = source.at<uchar>(col, row + 1);
			m_G4 = source.at<uchar>(col - 1, row);

			m_B1 = source.at<uchar>(col - 1, row - 1);
			m_B2 = source.at<uchar>(col + 1, row - 1);
			m_B3 = source.at<uchar>(col + 1, row + 1);
			m_B4 = source.at<uchar>(col - 1, row + 1);

			m_R1 = source.at<uchar>(col, row - 2);
			m_R2 = source.at<uchar>(col + 2, row);
			m_R3 = source.at<uchar>(col, row + 2);
			m_R4 = source.at<uchar>(col - 2, row);

			m_R = source.at<uchar>(col, row);

			if (std::abs(m_R1 - m_R3) < std::abs(m_R2 - m_R4)) {
				m_G = (m_G1 + m_G3) / 2;
			}
			else if (std::abs(m_R1 - m_R3) > std::abs(m_R2 - m_R4)) {
				m_G = (m_G2 + m_G4) / 2;
			}
			else {
				m_G = (m_G1 + m_G2 + m_G3 + m_G4) / 4;
			}

			m_B = (m_B1 + m_B2 + m_B3 + m_B4) / 4;
		}
		break;
	}

	return cv::Vec3f(m_B, m_G, m_R);
}

const ColorIndex BayerFilter::getColorIndex(const int col, const int row) {
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

const void BayerFilter::startFiltration(cv::Mat source, cv::Mat &destination) {
	destination = cv::Mat::zeros(source.size(), CV_32FC3);

	for (int y = 2; y < destination.rows - 2; y++) {
		for (int x = 2; x < destination.cols - 2; x++) {
			destination.at<cv::Vec3f>(y, x) = bayerFilter(source, y, x) / 255.0f;
		}
	}
}