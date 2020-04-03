#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{	
	cv::Mat img_color = cv::imread("images/green.jpg", CV_LOAD_IMAGE_COLOR);

	cv::Mat img_roi(img_color.rows, img_color.cols, CV_8UC3);	

	cv::rectangle(img_roi, cv::Point(0,50), cv::Point(img_color.size().width, 100),
						cv::Scalar(1, 1, 1), -1);

	cv::Mat roi;

    cv::multiply(img_color, img_roi, roi);
	//cv::bitwise_and(img_color, img_roi, roi);
    

	cv::imshow("src", img_color);	
	cv::imshow("roi", img_roi);	
	cv::imshow("result", roi);	

	cv::waitKey(0);

	return 0;
}

