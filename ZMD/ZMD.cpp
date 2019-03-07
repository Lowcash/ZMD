#include "pch.h"
#include "bayer_filter.h"
#include "rgb_to_yuv.h"
#include "hdr.h"

const enum Program { RGB_TO_YUV, BAYER_FILTER, HDR_FILTER };

std::vector<cv::Mat> inputs;
std::vector<cv::Mat> outputs;

BayerFilter *bayerFilter = nullptr;
RGBToYUV *rgbToYuv = nullptr;
HDR *hdr = nullptr;

int main()
{
	switch (Program::HDR_FILTER)
	{
		case Program::RGB_TO_YUV: {
			rgbToYuv = new RGBToYUV();

			inputs.push_back(cv::imread("img/mountains.jpg", CV_LOAD_IMAGE_COLOR));
			outputs.push_back(cv::Mat());
			outputs.push_back(cv::Mat());
			outputs.push_back(cv::Mat());

			rgbToYuv->startConversion(inputs.front(), outputs.at(0), outputs.at(1), outputs.at(2));

			std::string windowNames[] = { "Original", "Transformed Y", "Transformed U", "Transformed V" };

			cv::namedWindow(windowNames[0]);
			cv::namedWindow(windowNames[1]);
			cv::namedWindow(windowNames[2]);
			cv::namedWindow(windowNames[3]);

			cv::moveWindow(windowNames[0], 0 * inputs.at(0).cols, 0 * inputs.at(0).rows);
			cv::moveWindow(windowNames[1], 1 * outputs.at(0).cols, 0 * outputs.at(0).rows);
			cv::moveWindow(windowNames[2], 2 * outputs.at(1).cols, 0 * outputs.at(1).rows);
			cv::moveWindow(windowNames[3], 3 * outputs.at(2).cols, 0 * outputs.at(2).rows);

			cv::imshow(windowNames[0], inputs.front());
			cv::imshow(windowNames[1], outputs.at(0));
			cv::imshow(windowNames[2], outputs.at(1));
			cv::imshow(windowNames[3], outputs.at(2));

			cv::waitKey(0);

			delete rgbToYuv;
		}
		break;
		case Program::BAYER_FILTER: {
			bayerFilter = new BayerFilter();

			inputs.push_back(cv::imread("img/bayer.bmp", CV_LOAD_IMAGE_GRAYSCALE));
			outputs.push_back(cv::Mat());

			bayerFilter->startFiltration(inputs.front(), outputs.front());

			std::string windowNames[] = { "Grayscale", "Filtered" };

			cv::namedWindow(windowNames[0]);
			cv::namedWindow(windowNames[1]);

			cv::moveWindow(windowNames[0], 0 * inputs.at(0).cols, 0 * inputs.at(0).rows);
			cv::moveWindow(windowNames[1], 1 * outputs.at(0).cols, 0 * outputs.at(0).rows);

			cv::imshow(windowNames[0], inputs.front());
			cv::imshow(windowNames[1], outputs.front());

			cv::waitKey(0);

			delete bayerFilter;
		}
		break;
		case Program::HDR_FILTER: 
		{
			hdr = new HDR();

			inputs.push_back(cv::imread("img/s1_0.png", CV_LOAD_IMAGE_COLOR));
			inputs.push_back(cv::imread("img/s1_1.png", CV_LOAD_IMAGE_COLOR));
			inputs.push_back(cv::imread("img/s1_2.png", CV_LOAD_IMAGE_COLOR));
			inputs.push_back(cv::imread("img/s1_3.png", CV_LOAD_IMAGE_COLOR));
			inputs.push_back(cv::imread("img/s1_4.png", CV_LOAD_IMAGE_COLOR));

			outputs.push_back(cv::Mat());

			hdr->calculateHDR(inputs, outputs.front());

			std::string windowNames[] = { "S1", "S2", "S3", "S4", "S5", "HDR" };

			cv::namedWindow(windowNames[0]);
			cv::namedWindow(windowNames[1]);
			cv::namedWindow(windowNames[2]);
			cv::namedWindow(windowNames[3]);
			cv::namedWindow(windowNames[4]);
			cv::namedWindow(windowNames[5]);

			cv::moveWindow(windowNames[0], 0 * inputs.at(0).cols, 0 * inputs.at(0).rows);
			cv::moveWindow(windowNames[1], 1 * inputs.at(1).cols, 0 * inputs.at(1).rows);
			cv::moveWindow(windowNames[2], 2 * inputs.at(2).cols, 0 * inputs.at(2).rows);
			cv::moveWindow(windowNames[3], 3 * inputs.at(3).cols, 0 * inputs.at(3).rows);
			cv::moveWindow(windowNames[4], 4 * inputs.at(4).cols, 0 * inputs.at(4).rows);
			cv::moveWindow(windowNames[5], 2 * inputs.at(2).cols, 1 * inputs.at(2).rows + 50);

			cv::imshow(windowNames[0], inputs.at(0));
			cv::imshow(windowNames[1], inputs.at(1));
			cv::imshow(windowNames[2], inputs.at(2));
			cv::imshow(windowNames[3], inputs.at(3));
			cv::imshow(windowNames[4], inputs.at(4));
			cv::imshow(windowNames[5], outputs.front());
			
			cv::waitKey(0);

			delete hdr;
		}
		break;
	}
}
