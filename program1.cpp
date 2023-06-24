#include <iostream>
#include <string>
#include <vector>
//#include "test.h"
#include "opencv2/opencv.hpp"

struct dataStruct {
	cv::Mat* image;
	cv::Mat* image_clean;
};

void toolsEvent(int event, int x, int y, int flags, void* param);
std::string print_bgr();

// Globals
const std::string windowName = "Raster Graphics";

// No tool should be selected by default, this index maps to the tools vector
int toolIndex = -1;

// Cropping Dependencies
std::vector<int> point;

const std::vector<std::string> tools = {"Eyedropper", "Crop", "Pencil", "Paint Bucket", "Reset"};
std::vector<size_t> bgr_val = {255,255,255};


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
		cv::Mat imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);
		cv::Mat imageIn_clean = imageIn.clone();

		// check for file error
		if(!imageIn.data)
		{
			std::cout << "Error while opening file " << argv[1] << std::endl;
			return 0;
		}

		// Initialize our struct to manage image data
		dataStruct imgData;
		imgData.image = &imageIn;
		imgData.image_clean = &imageIn_clean;

		cv::imshow(windowName, imageIn);
		cv::setMouseCallback(windowName, toolsEvent, &imgData);
		std::cout << "---Right click to cycle through available tools---" << std::endl;
		cv::waitKey();

    }
}


void toolsEvent(int event, int x, int y, int flags, void* param) {

	//std::cout << "in func" << std::endl;

	dataStruct* imgData = static_cast<dataStruct*>(param);
	cv::Mat* image_ptr = imgData->image;

	if(event == cv::EVENT_RBUTTONDOWN) {
		// on event, select first tool and ensure we cycle through tools
		// multiple times
		toolIndex = (toolIndex + 1) % 5;

		std::cout << "Selected tool : " << tools[toolIndex] << std::endl;
	}
	else if (toolIndex == 0 && event == cv::EVENT_LBUTTONDOWN) {

		bgr_val[0] = image_ptr->at<cv::Vec3b>(y,x)[0];
		bgr_val[1] = image_ptr->at<cv::Vec3b>(y,x)[1];
		bgr_val[2] = image_ptr->at<cv::Vec3b>(y,x)[2];

		std::cout << print_bgr() << std::endl;
	}
	else if (toolIndex == 1) {
		
		if (event == cv::EVENT_LBUTTONDOWN) {

			// Save point of initial click
			point.push_back(x);
			point.push_back(y);
		}
		else if (event == cv::EVENT_LBUTTONUP) {

			// Save point of release and define region using the two points
			cv::Point p1(point[0], point[1]);
			cv::Point p2(x,y);
			cv::Rect region(p1,p2);
			
			cv::Mat imageROI = (*image_ptr)(region);

			// image_ptr holds the address of imageIn in the main func.
			// dereferncing this pointer variable takes us to the address of where
			// imageIn lives and we are modifying it with the newly modified ROI image
			*image_ptr = imageROI;
			cv::imshow(windowName, *image_ptr);

			point.clear();
		}
	}
	else if (toolIndex == 2) {

		if (flags == cv::EVENT_FLAG_LBUTTON) {
			image_ptr->at<cv::Vec3b>(y,x)[0] = bgr_val[0];
			image_ptr->at<cv::Vec3b>(y,x)[1] = bgr_val[1];
			image_ptr->at<cv::Vec3b>(y,x)[2] = bgr_val[2];

			cv::imshow(windowName, *image_ptr);
		}
	}
	else if (toolIndex == 3) {

		if (event == cv::EVENT_LBUTTONDOWN) {
			std::cout << print_bgr() << std::endl;

			cv::Point targetPoint(x,y);
			cv::Scalar fillColor(bgr_val[0], bgr_val[1], bgr_val[2]);

			cv::floodFill(*image_ptr, targetPoint, fillColor);
			cv::imshow(windowName, *image_ptr);
		}
	}
	else if (toolIndex == 4) {

		if(event == cv::EVENT_LBUTTONDBLCLK) {

			// Grab clean image Mat from struct, make a clone and set it to display
			// always ensuring image_clean remains untouched
			cv::Mat image_clean_temp = *(imgData->image_clean);
			*image_ptr = image_clean_temp.clone();
			
			cv::imshow(windowName, *image_ptr);
		}
	}

}


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
