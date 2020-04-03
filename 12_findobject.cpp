#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("images/rgb.png", IMREAD_COLOR);		// 명암도 영상 읽기
	CV_Assert(!image.empty());									// 영상파일 예외처리

	Mat image_ch[3];
	split(image, image_ch);
	Mat red_find;
	threshold(image_ch[2], red_find, 200, 255, THRESH_BINARY);
	
	Mat result(red_find.cols, red_find.rows, CV_8UC3);

	vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( red_find, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );

	for (int i = 0; i< contours.size(); i++)
    {
        Scalar box(  0, 0, 255 );
        Scalar line(  255, 0, 0 );
        drawContours( result, contours, i, line);

		RotatedRect rrect = minAreaRect(contours[i]);
		Rect boundingrect = rrect.boundingRect();
		cout << contours[i] << endl;
		cout << contourArea(contours[i]) << endl;
		cout << boundingrect.width << endl;
		rectangle(result, boundingrect, box);
    }

	cout << contours.size() << endl;

	imshow("image", image);
	imshow("Result", result);

	waitKey();
	return 0;
}
