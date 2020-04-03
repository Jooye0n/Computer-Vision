#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void  calc_Histo(const Mat& image, Mat& hist, int bins, int range_max = 256)
{
	int		histSize[] = { bins };			// 히스토그램 계급개수
	float   range[] = { 0, (float)range_max };		// 히스토그램 범위
	int		channels[] = { 0 };				// 채널 목록
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
		float idx1 = i * bin;
		float idx2 = (i + 1) * bin;
		Point2f pt1 = Point2f(idx1, 0);
		Point2f pt2 = Point2f(idx2, hist.at <float>(i));

		if (pt2.y > 0)
			rectangle(hist_img, pt1, pt2, Scalar(0), -1);
	}
	flip(hist_img, hist_img, 0);
}

int search_valueIdx(Mat hist, int bias = 0)
{
	for (int i = 0; i < hist.rows; i++)
	{
		int idx = abs(bias - i);
		if (hist.at<float>(idx) > 0)	return idx;
	}
	return -1;
}

int main()
{
	Mat image = imread("images/histo_test1.jpg", 0);
	CV_Assert(!image.empty());

	Mat hist, hist_dst, hist_img, hist_dst_img;
	int  histsize = 64, ranges = 256;
	calc_Histo(image, hist, histsize, ranges);

	float bin_width = (float)ranges / histsize;
	int low_value   = (int)(search_valueIdx(hist, 0) * bin_width);
	int high_value  = (int)(search_valueIdx(hist, hist.rows)* bin_width);
	cout << "high_value = " << high_value << endl;
	cout << "low_value = " << low_value << endl;

	int d_value = high_value - low_value;				 
	Mat  dst = (image - low_value) * (255.0 / d_value);

	calc_Histo(dst, hist_dst, histsize, ranges);
	draw_histo(hist, hist_img);
	draw_histo(hist_dst, hist_dst_img);

	imshow("image", image);
	imshow("dst-stretching", dst);
	imshow("img_hist", hist_img);
	imshow("dst_hist", hist_dst_img);
	waitKey();
	return 0;
}
