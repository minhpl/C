#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <iostream>
#include <string>

const int WIDTH = 256;
const int HEIGHT = 256;
const int radius = 10;

using namespace cv;
using namespace std;

/**
 * @function main
 * @brief Main function
 */ 


int main( void ){

	Mat mat(100,200,CV_8UC4);
  	mat = cv::Scalar(0,0,255,100);
    
   	circle( mat, Point( 10, 20 ), 10.0, Scalar( 255, 255, 255,255 ), -1, 8 );
	
	vector<int> compression_params;
	    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	    compression_params.push_back(9);

	
	imwrite("myimage2.png",mat,compression_params);
     	
 	return 0;  
}
