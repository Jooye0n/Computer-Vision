#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

int main(){
	Mat cam_data; //카메라에서 받아온 영상 저장 
	VideoCapture cap(0); //웹캠 연결(video0)

	//1 fps로 설정
	double fps = cap.set(CV_CAP_PROP_FPS, 1);
	cout<<"fps: "<<fps<<'\n';

	if(!cap.isOpened()) {
		serr << "error conn fail\n";
		return -1;
	}

	while(1){
		cap.read(cam_data);//캡처 한장씩 가져온다.
		if(cam_data.empty()){
			cerr << "cam data empty\n";
			break;
		}

		Mat imgproc = cam_data.clone();
		Mat imgsplit[3]; //C1 R G B

		split(imgproc, imgsplit); //B(0) G(1) R(2)

		Mat red_find_res,green_find_res,blue_find_res;
		threshold(imgsplit[2], red_find_res, 200, 255, THRESH_BINARY);//CR 200-255 -> red_find_res
		threshold(imgsplit[1], green_find_res, 200, 255, THRESH_BINARY);
		threshold(imgsplit[0], blue_find_res, 200, 255, THRESH_BINARY);

		Mat element = getStructuringElement(MORPH_RECT, Size(ele_size*2+1, ele_size*2+1), Point(1,1));
		int sle_size = 1;
		Mat erode_res, dil_res;

		erode(red_find_res, erode_res, element);
		dilate(red_find_res, dil_res, element);
		dilate(red_find_res, dil_res, element);
		dilate(red_find_res, dil_res, element);
		erode(red_find_res, erode_res, element);
		erode(red_find_res, erode_res, element);
		//잡음이 없어진다. 
		
		imgshow("erd",erode_res);
		imgshow("dil", dil_res);

		red_find_res -=green_find_res;
		red_find_res -=blue_find_res;

		Mat result = imgproc.clone();
		//Mat result = Mat::zeros(last_find_res.rows, last_find_res.cols, CV_8UC3);
		vector<vector<Point> > contours;
		vector<vec4i> hierarchy;

		findcontours(last_find_res,contours, hierarchy,RETP_TREE, CHAIN_APPROX_SIMPLE);

		for(int i=0; i<contours.size(); i++){
			Scalar line(0,0,255);
			drawContours(result, contours, i line);
			RotatedRect rrect = minAreaRect(contours[i]);
			Rect boundingrect = rrect.boundingrect();
			if(boundingrect.width * 0.6 <= boundingrect.height && boundingrect.width * 0.8 >= boundingrect.height){
				rectangle(result, boundingrect, line);
			}
			
		}

		if(!result.empty()){
			imgshow("result", result);
		}

		imgshow("img_350_360",img_H_Red_350_360);
		imgshow("img_res",img_H_Red_res);

		imgshow("img_HLS", img_HLS);
		imgshow("cam",can_data);
		imgshow("th_r_200", red_find_res);

		imgshow("r", imgsplit[2]);
		imgshow("g", imgsplit[1]);
		imgshow("b", imgsplit[0]);

		if(wailkey(25)>=0) break;
	}
	return 0;
}