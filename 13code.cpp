#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat image = imread("images/road3.jpg", IMREAD_COLOR);		// 명암도 영상 읽기
	CV_Assert(!image.empty());									// 영상파일 예외처리

	
		Mat imgproc = image.clone();
		//차선의 소실점까지의 영역만 필요하기 때문에 ROI는 하단
		Mat lineROI = imgproc(Rect(0,imgproc.rows/2,imgproc.cols,imgproc.rows/2)).clone();
		//사각형인데(좌상단부터,세로의 절반부터,오른쪽 끝까지,세로의 절반까지)

		Mat lineProc = Mat::zeros(lineROI.rows, lineROI.cols,CV_8UC3);
		//(line의 가로사이즈만큼,라인의 세로사이즈만큼,채널 3개짜리 컬러로 초기화
		Mat grayImg;
		cvtColor(lineROI,grayImg,COLOR_BGR2GRAY);
		//(imgproc를 grayimg에 그레이로 바꿈

		Mat cannyImg;//선을 그릴 메트릭스
		Mat thImg;//threshold이미지용 메트릭스

		threshold(grayImg, thImg,100,255,THRESH_BINARY);
		Canny(grayImg,cannyImg,100,150,3);


		vector<Vec2f> lines;//라인정보를 벡터에 가져온다

		int threshold =70;

		HoughLines(cannyImg,lines,1,CV_PI/180,threshold);
		//180도에 해당하는 직선 다 찾아본다.수만은점 지나는 직선들 라인 생성.

		float theta_min = 40.0f *CV_PI/180.0f;
		float theta_max = 60.0f *CV_PI/180.0f;
		float theta_min2 = 110.0f *CV_PI/180.0f;
		float theta_max2 = 140.0f *CV_PI/180.0f;

		Mat cannyColor;
		Mat ROITest = lineROI.clone();
		cvtColor(cannyImg, cannyColor, COLOR_GRAY2BGR);
		ROITest = ROITest - cannyColor;
		imshow("ROI CHECK", ROITest);

		Point leftP[2], rightP[2], banishP;//왼쪽 직선 오른쪽 직선 소실점 하나 (여러가지의 허프 라인 중 하나를 정하고, 소실점을 정하기 위함)
		bool bLFound = false, bRFound = false;
		//찾아진 line에 대해 선을 그려본다
		for(int i=0; i<lines.size(); i++){
			float rho = lines[i][0],theta = lines[i][1];//세타는 직선의 각

			Scalar color = Scalar(0,0,255);
			bool bSearch = false;

			Point pt11(0, rho / sin(theta)); 
			Point pt21(ROITest.cols, (rho - ROITest.cols*cos(theta)) / sin(theta));
			line(ROITest, pt11, pt21, color, 1);


			if(theta > theta_min && theta < theta_max  && bLFound==false){
				color = Scalar(255,0,0);
				//bSearch = true;
				bLFound = true;
				//paintLine();

				Point pt1(0,rho/sin(theta));//이미지 스페이스로 변환하는 것(허프 스페이스에서)
				Point pt2(cannyImg.cols,(rho-cannyImg.cols*cos(theta))/sin(theta));//수식
				// Scalar color = Scalar(0,0,255);//색 지정			
				line(lineProc,pt1,pt2,color,4);//라인에 저장
				leftP[0] = pt1;
				leftP[1] = pt2;
			}
			if(theta > theta_min2 && theta < theta_max2 && bRFound==false){
				color = Scalar(0,0,255);
				//bSearch = true;
				bRFound = true;
				//paintLine();
				Point pt1(0,rho/sin(theta));//이미지 스페이스로 변환하는 것(허프 스페이스에서)
				Point pt2(cannyImg.cols,(rho-cannyImg.cols*cos(theta))/sin(theta));//수식
				// Scalar color = Scalar(0,0,255);//색 지정			
				line(lineProc,pt1,pt2,color,4);//라인에 저장
				rightP[0] = pt1;
				rightP[1] = pt2;
			}

			// if(bSearch == true){
			// 	Point pt1(0,rho/sin(theta));//이미지 스페이스로 변환하는 것(허프 스페이스에서)
			// 	Point pt2(cannyImg.cols,(rho-cannyImg.cols*cos(theta)/sin(theta)));//수식
			// 	// Scalar color = Scalar(0,0,255);//색 지정			
			// 	line(lineProc,pt1,pt2,color,1);//라인에 저장
			// }
		}

	imshow("ROI CHECK1", ROITest);
	
	float leftLineA = (float)(leftP[1].y - leftP[0].y) / (float)(leftP[1].x - leftP[0].x);
	float leftLineB = leftP[1].y - leftLineA * leftP[1].x;

	float rightLineA = (float)(rightP[1].y - rightP[0].y) / (float)(rightP[1].x - rightP[0].x);
	float rightLineB = rightP[1].y - rightLineA * rightP[1].x;

	banishP.x = (int)((rightLineB - leftLineB) / (leftLineA - rightLineA));
	banishP.y = (int)(leftLineA * banishP.x + leftLineB);
	
	
	Scalar color = Scalar(0,255,0);
	line(lineProc, banishP, Point(banishP.x, cannyImg.rows), color, 4);
		
	lineROI = lineROI + lineProc;
	
	imshow("gray", grayImg);
	imshow("canny", cannyImg);
	imshow("th", thImg);
	imshow("line", lineProc);
	imshow("ROI", lineROI);

		//(100을 기준으로)
	waitKey();
	return 0;
}
