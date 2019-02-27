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
	switch (Program::RGB_TO_YUV)
	{
		case Program::RGB_TO_YUV: {
			rgbToYuv = new RGBToYUV();

			inputs.push_back(cv::imread("img/mountains.jpg", CV_LOAD_IMAGE_COLOR));
			outputs.push_back(cv::Mat());
			outputs.push_back(cv::Mat());
			outputs.push_back(cv::Mat());

			rgbToYuv->startConversion(inputs.front(), outputs.at(0), outputs.at(1), outputs.at(2));

			cv::imshow("Original", inputs.front());
			cv::imshow("Transformed Y", outputs.at(0));
			cv::imshow("Transformed U", outputs.at(1));
			cv::imshow("Transformed V", outputs.at(2));

			cv::waitKey(0);

			delete rgbToYuv;
		}
		break;
		case Program::BAYER_FILTER: {
			bayerFilter = new BayerFilter();

			inputs.push_back(cv::imread("img/bayer.bmp", CV_LOAD_IMAGE_GRAYSCALE));
			outputs.push_back(cv::Mat());

			bayerFilter->startFiltration(inputs.front(), outputs.front());

			cv::imshow("Grayscale", inputs.front());
			cv::imshow("Filtered", outputs.front());

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

			cv::imshow("S1", inputs.at(0));
			cv::imshow("S2", inputs.at(1));
			cv::imshow("S3", inputs.at(2));
			cv::imshow("S4", inputs.at(3));
			cv::imshow("S5", inputs.at(4));

			cv::imshow("HDR", outputs.front());
			
			cv::waitKey(0);

			delete hdr;
		}
		break;
	}
}
