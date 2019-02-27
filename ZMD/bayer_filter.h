#pragma once

const enum ColorIndex { R = 0, G_1, G_2, B, NONE };

class BayerFilter {
private:
	uchar m_R1, m_R2, m_R3, m_R4;
	uchar m_G1, m_G2, m_G3, m_G4;
	uchar m_B1, m_B2, m_B3, m_B4;

	uchar m_R, m_G, m_B;
private: 
	const cv::Vec3f bayerFilter(cv::Mat source, const int col, const int row);
	const ColorIndex getColorIndex(const int col, const int row);
public :
	const void startFiltration(cv::Mat source, cv::Mat &destination);
};