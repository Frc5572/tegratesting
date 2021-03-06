#include <opencv2/opencv.hpp>

#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace cv;
using namespace std;

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
		findContours(RangeMat,points,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		for(int i = 0; i < points.size();i++){
			double ax = 0, ay = 0;			
			for (int j = 0; j < points[i].size();j++){		
				ax += points[i][j].x;		
				ay += points[i][j].y;
			}
			ax /= points[i].size();
			ay /= points[i].size();
			circle(cameraFrame, Point(ax, ay), 5, Scalar(0, 0, 0), 5);
			double ang1 = 45, ang2 = 135, ang3 = 225, ang4 = 315;
			Point a1, a2, a3, a4;
			double d1 = 0, d2 = 0, d3 = 0, d4 = 0;
			for (int j = 0; j < points[i].size();j++){
				double px = points[i][j].x - ax;
				double py = points[i][j].y - ay;
				double angle = 180 * atan2(py, px)/ 3.14159;
				double dist = py*py+px*px;
				while(angle < 0) angle += 360;
				while(angle > 360) angle -= 360; //TODO Angle calculations
				if(angle - ang1 < (ang1 + ang2) / 2 && dist > d1){ // Candidate 1
					a1 = points[i][j];
					d1 = dist;
					ang1 = angle;
				}
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

