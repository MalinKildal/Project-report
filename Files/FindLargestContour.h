//Code from:        https://github.com/LowWeiLin/OpenCV_ImageBackgroundRemoval/blob/master/OpenCV_ImageBackgroundRemoval/main.cpp
//Code explanation: https://lowweilin.wordpress.com/2014/08/07/image-background-and-shadow-removal/

#ifndef FINDLARGESTCONTOUR_H
#define FINDLARGESTCONTOUR_H

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

#include "Filters.h"

using namespace cv;
using namespace std;


Mat FindLargestContourDepthmap(Mat src);

Mat FindLargestContourTotalfocus(Mat src);

Mat Sobel(Mat gray);


#endif // FINDLARGESTCONTOUR_H
