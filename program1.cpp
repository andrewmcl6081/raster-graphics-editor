#include <iostream>
#include <string>
#include <vector>
//#include "test.h"
#include "opencv2/opencv.hpp"

struct dataStruct {
	cv::Mat* image;
	bool needsUpdate;
};


//void eyeDropper(int event, int x, int y, int flags, void* param);
void toolsEvent(int event, int x, int y, int flags, void* param);
std::string print_bgr();

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
		cv::Mat imageIn;
		imageIn = cv::imread(argv[1], cv::IMREAD_COLOR);
		
		// check for file error
		if(!imageIn.data)
		{
			std::cout << "Error while opening file " << argv[1] << std::endl;
			return 0;
		}

		// Initialize our struct to manage image data
		// dataStruct imgData;
		// imgData.image = &imageIn;
		// imgData.needsUpdate = false;


		cv::imshow(windowName, imageIn);
		cv::setMouseCallback(windowName, toolsEvent, &imageIn);
		cv::waitKey();

    }
}


void toolsEvent(int event, int x, int y, int flags, void* param) {

	//std::cout << "in func" << std::endl;

	cv::Mat* imageIn_ptr = (cv::Mat*)param;

	// Pointer to the struct created in main
	// dataStruct* imgData = static_cast<dataStruct*>(param);

	// // pointer to the original imageIn
	// cv::Mat* image_ptr = imgData->image;

	if(event == cv::EVENT_RBUTTONDOWN) {
		// on event, select first tool and ensure we cycle through tools
		// multiple times
		toolIndex = (toolIndex + 1) % 5;

		std::cout << "Selected tool : " << tools[toolIndex] << std::endl;
	}
	else if (toolIndex == 0 && event == cv::EVENT_LBUTTONDOWN) {

		bgr_val[0] = imageIn_ptr->at<cv::Vec3b>(y,x)[0];
		bgr_val[1] = imageIn_ptr->at<cv::Vec3b>(y,x)[1];
		bgr_val[2] = imageIn_ptr->at<cv::Vec3b>(y,x)[2];

		std::cout << print_bgr() << std::endl;
	}
	else if (toolIndex == 1) {
		
		if (event == cv::EVENT_LBUTTONDOWN) {

			point.push_back(x);
			point.push_back(y);
		}
		else if (event == cv::EVENT_LBUTTONUP) {

			cv::Point p1(point[0], point[1]);
			cv::Point p2(x,y);
			cv::Rect region(p1,p2);

			//std::cout << "p1 : " << p1 << " " << "p2 : " << p2 << std::endl;
			
			cv::Mat imageROI = (*imageIn_ptr)(region);

			// imageIn_ptr holds the address of imageIn in the main func.
			// dereferncing this pointer variable takes us to the address of where
			// imageIn lives and we are modifying it with the newly modified ROI image
			*imageIn_ptr = imageROI;
			cv::imshow(windowName, imageROI);

			//works as well
			// Grabbing ROI from original imageIn and updating imageIn with new ROI image
			// cv::Mat imageROI = (*image_ptr)(region);
			// *image_ptr = imageROI;
			// cv::imshow(windowName, imageROI);

			point.clear();
		}
	}
	else if (toolIndex == 2) {

		if (flags == cv::EVENT_FLAG_LBUTTON) {
			(*imageIn_ptr).at<cv::Vec3b>(y,x)[0] = bgr_val[0];
			(*imageIn_ptr).at<cv::Vec3b>(y,x)[1] = bgr_val[1];
			(*imageIn_ptr).at<cv::Vec3b>(y,x)[2] = bgr_val[2];
			//std::cout << imageIn_ptr->at<cv::Vec3b>(y,x) << std::endl;

			cv::imshow(windowName, *imageIn_ptr);
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
