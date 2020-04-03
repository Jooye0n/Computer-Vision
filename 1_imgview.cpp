#include <opencv2/opencv.hpp>

int main(void)
{
	cv::Mat img = cv::imread("images/1.jpg", CV_LOAD_IMAGE_COLOR);
	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
	cv::imshow("Image", img);
	cv::waitKey(0);
	return 0;
}
