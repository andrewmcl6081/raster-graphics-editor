#include <iostream>
#include <string>
#include <vector>
//#include "test.h"
#include "opencv2/opencv.hpp"

const std::string windowName = "Raster Graphics";

// No tool should be selected by default, this index maps to the tools vector
int toolIndex = -1;

const std::vector<std::string> tools = {"Eyedropper", "Crop", "Pencil", "Paint Bucket", "Reset"};

std::vector<size_t> bgr_val = {255,255,255};

// Cropping Dependencies
std::vector<std::vector<int>> cropped_points;

std::string print_bgr() {
	std::string bgr_str = 
		"(B: " + std::to_string(bgr_val[0]) + 
		", " + 
		"G: " + std::to_string(bgr_val[1]) + 
		", " + 
		"R: " + std::to_string(bgr_val[2]) + 
		")";
	
	return bgr_str;
}

void eyeDropper(int event, int x, int y, int flags, void* param) {

	cv::Mat imageIn = *(cv::Mat*)param;

	bgr_val[0] = imageIn.at<cv::Vec3b>(y,x)[0];
	bgr_val[1] = imageIn.at<cv::Vec3b>(y,x)[1];
	bgr_val[2] = imageIn.at<cv::Vec3b>(y,x)[2];

	std::cout << print_bgr() << std::endl;
}

void cycleToolsOn_RClick(int event, int x, int y, int flags, void* param) {

	cv::Mat* imageIn = (cv::Mat*)param;

	if(event == cv::EVENT_RBUTTONDOWN) {
		// on event, select first tool and ensure we cycle through tools
		// multiple times
		toolIndex = (toolIndex + 1) % 5;

		std::cout << "Selected tool : " << tools[toolIndex] << std::endl;
	}


	if (toolIndex == 0 && event == cv::EVENT_LBUTTONDOWN) {
		eyeDropper(event, x, y, flags, param);
	}
	else if (toolIndex == 1) {
		
		if (event == cv::EVENT_LBUTTONDOWN) {
			std::cout << "inside lbd" << std::endl;

			std::vector<int> point;

			point.push_back(x);
			point.push_back(y);

			cropped_points.push_back(point);
		}
		else if(event == cv::EVENT_LBUTTONUP) {
			std::cout << "inside lbu" << std::endl;
			
			std::vector<int> point;

			point.push_back(x);
			point.push_back(y);

			cropped_points.push_back(point);

			cv::Point p1(cropped_points[0][0], cropped_points[0][1]);
			cv::Point p2(cropped_points[1][0], cropped_points[1][1]);
			cv::Rect region(p1,p2);
			
			cv::Mat imageROI = imageIn(region);

			// cv::resize(imageIn, imageROI, imageROI.size());
			// *imageIn = imageROI.clone();
			// imageIn = imageIn(region);
			
			// cv::Mat imageResized;
			// cv::resize(imageIn, )

			//cv::imshow(windowName, imageIn);

			// std::cout << "image width: " << imageIn.size().width << std::endl;
			// std::cout << "image height: " << imageIn.size().height << std::endl;
			// std::cout << "image channels: " << imageIn.channels() << std::endl;
		}
	}


}

int main(int argc, char **argv)
{

    // validate and parse the command line arguments
    if (argc != 2)
    {
		std::cout << "USAGE: " << argv[0] << " " << "<image_path>" << std::endl;
        return 0;
    }
    else
    {
		cv::Mat imageIn;
		imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);
		
		// check for file error
		if(!imageIn.data)
		{
			std::cout << "Error while opening file " << argv[1] << std::endl;
			return 0;
		}
		else
		{
			cv::imshow(windowName, imageIn);

			std::cout << "image width: " << imageIn.size().width << std::endl;
			std::cout << "image height: " << imageIn.size().height << std::endl;
			std::cout << "image channels: " << imageIn.channels() << std::endl;

			std::cout << "Right click to cycle through available tools" << std::endl;
			cv::setMouseCallback(windowName, cycleToolsOn_RClick, &imageIn);
			cv::waitKey();

			std::cout << cropped_points[0][0] << " " << cropped_points[0][1] << std::endl;
			std::cout << cropped_points[1][0] << " " << cropped_points[1][1] << std::endl;
			std::cout << cropped_points.size() << std::endl;
		}
    }
}
