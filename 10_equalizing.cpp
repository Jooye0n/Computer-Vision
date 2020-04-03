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


void create_hist(Mat img, Mat &hist, Mat &hist_img) // 히스토그램 계산과 그래프 그리기 통합
{
	int  histsize = 256, range = 256;
	calc_Histo(img, hist, histsize, range);			// 히스토그램 계산
	draw_histo(hist, hist_img);							// 히스토그램 그래프 그리기
}

int main()
{
	Mat image = imread("images/histo_test1.jpg", 0);		// 명암도 영상 읽기
	CV_Assert(!image.empty());									// 영상파일 예외처리

	Mat hist, dst1, dst2, hist_img, hist_img1, hist_img2;
	create_hist(image, hist, hist_img);				// 히스토그램 및 그래프 그리기
															// 히스토그램 누적합 계산
	Mat accum_hist = Mat(hist.size(), hist.type(), Scalar(0));
	accum_hist.at<float>(0) = hist.at<float>(0);
	for (int i = 1; i < hist.rows; i++) {
		accum_hist.at<float>(i) = accum_hist.at<float>(i - 1) + hist.at<float>(i);
	}

	accum_hist /= sum(hist)[0];							// 누적합의 정규화
	accum_hist *= 255;
	dst1 = Mat(image.size(), CV_8U);
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int idx = image.at<uchar>(i, j);
			dst1.at<uchar>(i, j) = (uchar)accum_hist.at<float>(idx);
		}
	}

	equalizeHist(image, dst2);							// 히스토그램 평활화
	create_hist(dst1, hist, hist_img1);			// 히스토그램 및 그래프 그리기
	create_hist(dst2, hist, hist_img2);

	imshow("image", image),		imshow("img_hist", hist_img);	// 원본 히스토그램
	imshow("dst1-User", dst1),	imshow("User_hist", hist_img1);		// 사용자 평활화 
	imshow("dst2-OpenCV", dst2),imshow("OpenCV_hist", hist_img2);// OpenCV 평활화
	waitKey();
	return 0;
}
