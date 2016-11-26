#ifndef FILTERS_H
#define FILTERS_H

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

#include <stdio.h>

using namespace cv;
using namespace std;


Mat Erode(Mat src, int kernelSize, int iterations);

Mat Dilate(Mat src, int kernelSize, int iterations);

Mat Close(Mat src, int kernelSize, int iterations);

Mat Open(Mat src, int kernelSize, int iterations);

Mat BilateralFilter(Mat src, int kernelSize);

Mat MedianFilter(Mat src, int kernelSize, int iterations);

Mat ColorFilter(Mat src);

Mat LocalThresholding(Mat src, int kernelSize, int c);


#endif // FILTERS_H
