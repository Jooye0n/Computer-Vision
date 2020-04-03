#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int minNum = 987987987;

void calc_histo(Mat image, Mat &hist, int bins, int range_max = 256)
{
	hist = Mat(bins, 1, CV_32F, Scalar(0));
	float gap = range_max / (float)bins;

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++)
		{
			int idx = int(image.at<uchar>(i, j) / gap);
			hist.at<float>(idx)++;
		}
	}
}

void  calc_Histo(const Mat& image, Mat& hist, int bins, int range_max = 256)
{
	int		histSize[] = { bins };					// 히스토그램 계급개수
	float   range[] = { 0, (float)range_max };		// 채널 화소값 범위
	int		channels[] = { 0 };						// 채널 목록
	const float* ranges[] = { range };

	calcHist(&image, 1, channels, Mat(), hist, 1, histSize, ranges);
}

void draw_histo(Mat hist, Mat &hist_img, Size size = Size(256, 200))
{
	hist_img = Mat(size, CV_8U, Scalar(255));
	float  bin = (float)hist_img.cols / hist.rows;
	normalize(hist, hist, 0, size.height, NORM_MINMAX);
	
	for (int i = 0; i<hist.rows; i++)
	{
		float  start_x = (i * bin);
		float  end_x = (i + 1) * bin;
		Point2f pt1(start_x, 0);
		Point2f pt2(end_x, hist.at <float>(i));

		if (pt2.y > 0){
			rectangle(hist_img, pt1, pt2, Scalar(0), -1);
			if(minNum>pt2.y) minNum = pt2.y;
		}
	}
	cout<<minNum;
	flip(hist_img, hist_img, 0);
}

int main()
{
	Mat Result, Result2;
	Mat image = imread("images/histo.jpg", IMREAD_GRAYSCALE);
	CV_Assert(!image.empty());

	Mat hist, hist_img;
	calc_Histo(image, hist, 256);
	draw_histo(hist, hist_img);

	int rtn = threshold(image, Result, 150,255,THRESH_OTSU);
	threshold(image, Result2, 0, minNum, THRESH_BINARY);
	cout<<"otsu:"<<rtn<<'\n';

	imshow("hist_img", hist_img);
	imshow("image", image);
	imshow("Result", Result);
	imshow("Result2", Result2);
	waitKey();
	return 0;
}

/*
	Mat image = imread("images/1.jpg", 0);		// 명암도 영상 읽기
	CV_Assert(!image.empty());									// 영상파일 예외처리

	Mat Result;
	Mat Result2;
	Mat Result3;
	threshold(image, Result, 100, 255, THRESH_BINARY);
	threshold(image, Result2, 150, 255, THRESH_BINARY);

	//bitwise_and(Result, Result2, Result3);
	Result3 = Result - Result2;
	imshow("image", image);
	imshow("Result", Result);
	imshow("Result2", Result3);
	*/
