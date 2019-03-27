#include "pch.h"
#include "bayer_filter.h"
#include "rgb_to_yuv.h"
#include "hdr.h"

const enum Program { RGB_TO_YUV, BAYER_FILTER, HDR_FILTER, MOSAIK_VID };

std::vector<cv::Mat> inputs;
std::vector<cv::Mat> outputs;

BayerFilter *bayerFilter = nullptr;
RGBToYUV *rgbToYuv = nullptr;
HDR *hdr = nullptr;

int main(int argc, char *argv[])
{
	switch(Program::MOSAIK_VID) {
		case Program::RGB_TO_YUV:
		{
			rgbToYuv = new RGBToYUV();

			inputs.push_back(cv::imread("img/mountains.jpg", CV_LOAD_IMAGE_COLOR));
			outputs.push_back(cv::Mat());
			outputs.push_back(cv::Mat());
			outputs.push_back(cv::Mat());

			rgbToYuv->startConversion(inputs.front(), outputs.at(0), outputs.at(1), outputs.at(2));

			delete rgbToYuv;

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
		}
		break;
		case Program::BAYER_FILTER:
		{
			bayerFilter = new BayerFilter();

			inputs.push_back(cv::imread("img/bayer.bmp", CV_LOAD_IMAGE_GRAYSCALE));
			outputs.push_back(cv::Mat());

			bayerFilter->startFiltration(inputs.front(), outputs.front());

			delete bayerFilter;

			std::string windowNames[] = { "Grayscale", "Filtered" };

			cv::namedWindow(windowNames[0]);
			cv::namedWindow(windowNames[1]);

			cv::moveWindow(windowNames[0], 0 * inputs.at(0).cols, 0 * inputs.at(0).rows);
			cv::moveWindow(windowNames[1], 1 * outputs.at(0).cols, 0 * outputs.at(0).rows);

			cv::imshow(windowNames[0], inputs.front());
			cv::imshow(windowNames[1], outputs.front());

			cv::waitKey(0);
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

			delete hdr;

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
		}
		break;
		case Program::MOSAIK_VID:
		{
			if(argc != 6) {
				printf("Not enought arguments\n");
				exit(1);
			}

			std::string videoPath = argv[1];
			int gridXSize = atoi(argv[2]);
			int gridYSize = atoi(argv[3]);
			int resolutionX = atoi(argv[4]);
			int resolutionY = atoi(argv[5]);

			cv::VideoCapture cap(videoPath);
			
			if(!cap.isOpened()) {
				printf("Error opening video stream or file");
				exit(1);
			}

			int numFrames = (int)cap.get(CV_CAP_PROP_FRAME_COUNT);
			int frameOffset = numFrames / (gridXSize * gridYSize);

			cv::Mat frame;
			cap >> frame;

			cv::Mat outputFrame = cv::Mat::zeros(cv::Size(resolutionX, resolutionY), frame.type());
			for(int i = 0; i < frameOffset; i++) {
				for(int j = 0; j < gridXSize * gridYSize; j++) {
					cap.set(CV_CAP_PROP_POS_FRAMES, i + j * frameOffset);
					cap >> frame;

					cv::resize(frame, frame, cv::Size(resolutionX / gridXSize, resolutionY / gridYSize));
					
					int outputFrameOffsetX = (j % gridXSize);
					int outputFrameOffsetY = (j / gridYSize);

					frame.forEach<cv::Vec3b>([&](cv::Vec3b &pixel, const int *position) {
						int y = position[0], x = position[1];

						outputFrame.at<cv::Vec3b>(y + (outputFrameOffsetY * frame.rows), x + (outputFrameOffsetX * frame.cols)) = frame.at<cv::Vec3b>(y, x);
					});
				}

				if(frame.empty()) { break; }

				cv::imshow(videoPath, outputFrame);
				
				char c = (char)cv::waitKey(25);
				if(c == 27) { break; }
			}

			cap.release();

			cv::destroyAllWindows();
		}
		break;
	}
}
