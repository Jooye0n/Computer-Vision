#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("images/1.jpg", 0);		// 명암도 영상 읽기
	CV_Assert(!image.empty());// 영상파일 예외처리( CV_8U, 즉 8비트의 Unsigned 형태인지를 보고, 아니라면 ERROR )

	Mat Result;
	threshold(image, Result, 100, 255, THRESH_BINARY);//그레이영상을 입력으로 받아 이진화 영상을 결과로

	imshow("image", image);
	imshow("Result", Result);

	waitKey();
	return 0;
}
