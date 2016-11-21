#ifndef FILTERS_H
#define FILTERS_H

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

#include <stdio.h>

using namespace cv;
using namespace std;


Mat erode(Mat src, int kernel_size, int iterations);

Mat dilate(Mat src, int kernel_size, int iterations);

Mat close(Mat src, int kernel_size, int iterations);

Mat bilateral_filter(Mat src, int kernel_size);

Mat median_filter(Mat src, int kernel_size);

Mat filterColor(Mat src);


#endif // FILTERS_H
