#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace cv;
using namespace std;

struct dPoint {
	double x, y;
	dPoint(double mx, double my) : x(mx), y(my){}
};

int main() {	
	VideoCapture stream1(0);   //0 is the id of video device.0 if you have only one camera.
	if (!stream1.isOpened()) { //check if video device has been initialised
		cerr << "cannot open camera" << endl;
		return -1;
	}
	//unconditional loop
	while (true) {
		Mat cameraFrame; //Prepare space for camera feed
		stream1.read(cameraFrame); //Read camera feed
		Mat HSVMat; //Prepare space for HSV feed
		cvtColor(cameraFrame,HSVMat,COLOR_BGR2HSV); //Convert to HSV feed		
		Mat RangeMat; //Prepare space for exposure feed
		inRange(HSVMat, Scalar(28,189,31), Scalar(128,255,131), RangeMat); //Convert to exposure feed
		vector<vector<Point>> points;
		vector<Vec4i> hierarchy;
		findContours(RangeMat,points,hierarchy,CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
		for(int i = 0; i < points.size();i++){
			for (int j = 0; j < points[i].size();j++){		
				
				Point a = points[i][j-1 < 0 ? points[i].size()-1 : j-1];
				Point b = points[i][j];
				Point c = points[i][j+1 >= points[i].size() ? 0 : j+1];
				dPoint va = dPoint(b.x-a.x, b.y-a.y);
				dPoint vb = dPoint(c.x-b.x, c.y-b.y);
				double val = sqrt((va.x*va.x)+(va.y*va.y));
				double vbl = sqrt((vb.x*vb.x)+(vb.y*vb.y));
				dPoint na = dPoint(((double)va.x)/val, ((double)va.y)/val);
				dPoint nb = dPoint(((double)vb.x)/vbl, ((double)vb.y)/vbl);
				double angle = na.x*nb.x+na.y*nb.y;
				if(angle < .1)
				circle(cameraFrame, points[i][j], 5, Scalar(angle*255.0,0,0), 5); std::cout << angle << std::endl;
			}
		}
		if (waitKey(30) >= 0){		
			//Vec3b v = HSVMat.at<Vec3b>(240,320);
			//print(v);			
			break; //End program on key press
		}		
		imshow("cam", cameraFrame); //Display exposure feed
	}
	return 0;
}

