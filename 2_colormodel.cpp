#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{	
	cv::Mat img_color = cv::imread("images/rgb.png", CV_LOAD_IMAGE_COLOR);
	cv::Mat imgbufgray; 
	cv::Mat imgbufhls;
	cv::Mat imgbufch[3];

	cv::cvtColor(img_color, imgbufgray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(img_color, imgbufhls, cv::COLOR_BGR2HLS);
	
	cv::split(img_color, imgbufch);
	
	cv::imshow("src", img_color);
	
	cv::imshow("gray", imgbufgray);
	cv::imshow("hls", imgbufhls);
	
	cv::imshow("chB", imgbufch[0]);		
	cv::imshow("chG", imgbufch[1]);		
	cv::imshow("chR", imgbufch[2]);	

	cv::waitKey(0);

	return 0;
}

