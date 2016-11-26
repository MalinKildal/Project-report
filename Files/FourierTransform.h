#ifndef FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_H

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

using namespace cv;


Mat DFT(Mat src, float threshold_value);

Mat updateMag(Mat complex, float threshold_value);

Mat updateResult(Mat complex);

Mat computeDFT(Mat image);

Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert);

void shift(Mat magI);


#endif // FOURIER_TRANSFORM_H


//Fourier main function

//// Fourier Transform Filtering
//int main(int argc, char** argv)
//{
//	Mat image = imread("C:/Users/Raytrix/Desktop/Laks18.10.16/L1V2_20161018_135826839_0000000508_TotalFocus.png", CV_LOAD_IMAGE_GRAYSCALE);
//	//Mat image = imread("C:/Users/Raytrix/Desktop/Laks18.10.16/after_DFT.png", CV_LOAD_IMAGE_GRAYSCALE);
//	namedWindow("Orginal window", CV_WINDOW_NORMAL);
//	imshow("Orginal window", image);
//
//	DFT(image, 0.5);
//
//	waitKey(0);
//
//	return 0;
//}
