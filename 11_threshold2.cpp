#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("images/red.jpg", 0);		// 명암도 영상 읽기
	CV_Assert(!image.empty());	// 영상파일 예외처리

	Rect rect(0,0,400,150);
	Mat ROI = image(rect).clone();	
	//imshow("ROI",ROI);							

	Mat Result;
	Mat Result2;
	Mat Result3;
	Mat Result_otsu;
	Mat Result_mapping;
	int rtn = threshold(ROI,Result_otsu,150,255,THRESH_OTSU);
	cout<<rtn<<'\n';
	threshold(ROI, Result, 50, 255, THRESH_BINARY);
	threshold(ROI, Result2, 144, 255, THRESH_BINARY);

	//bitwise_and(image,Result,Result_mapping);

	bitwise_and(Result, Result2, Result_mapping);
	Result3 = Result_otsu - Result2;//
	Result_mapping = Result3-Result2;
	 imshow("Result_otsu", Result_otsu);
	 //imshow("Result", Result);
	 imshow("Result2", Result2);
	 imshow("Result_mapping", Result_mapping);
	imshow("Result3", Result3);

	waitKey();
	return 0;
}
