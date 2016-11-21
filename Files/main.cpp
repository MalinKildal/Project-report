/*	RULES:
*	Keep main clean! Object orient!
*	Never make anything completely dependant on anything else. Never add code that are impossible to remove or alter without impacting everything else.
*	Variables are camelBack named. Methods and classes are CamelCaps.
*	No namespace!
*	If any standard is uncertain, travel to Rome.
*/

/*	TODONE: Capture .ray image (from file or camera), show it in opencv, include all parameters (Camera untested)
*	TODO: Add basic height removal
*	TODO: Recognize object
*	TODO: Fill out object
*	TODO: Measure volume by splitting each fish into a set amount of segments in length and height, and grab box volume for each segment
*/

#include "CaptureImage.h"
#include "CVDraw.h"
#include "ImageOperations.h"
#include "Filters.h"
#include "FourierTransform.h"
#include "FindContour.h"

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;


//Works good on Laks18.10.16/...563, 573, 582 and 587. =D
//Code from https://github.com/LowWeiLin/OpenCV_ImageBackgroundRemoval/blob/master/OpenCV_ImageBackgroundRemoval/main.cpp
//Explanation at https://lowweilin.wordpress.com/2014/08/07/image-background-and-shadow-removal/

int main(int argc, char* argv[]) {
	//Open .ray file from file
	CaptureImage ci = CaptureImage("C:/Users/Raytrix/Desktop/Laks18.10.16/L2V3_20161018_141322074_0000000587.ray"); //L2V1_20161018_141158751_0000000563.ray");
	Rx::CRxImage depthmap = ci.LoadFrame();

	//Make Mat from .ray file
	CVDraw cd2 = CVDraw(depthmap);
	Mat src = cd2.MakeMat();

	namedWindow("1. Original", CV_WINDOW_NORMAL);
	imshow("1. Original", src);
	imwrite("LargestContourandClosing/Fish4_L2V3_20161018_141322074_0000000587/1_original.png", src);

	//Filter on color
	src = filterColor(src);
	namedWindow("2. Color Filtering", CV_WINDOW_NORMAL);
	imshow("2. Color Filtering", src);
	imwrite("LargestContourandClosing/Fish4_L2V3_20161018_141322074_0000000587/2_color_filtering.png", src);

	//Remove small particles
	src = erode(src, 1, 2);
	src = dilate(src, 1, 2);
	namedWindow("3. Removed Small Particles", CV_WINDOW_NORMAL);
	imshow("3. Removed Small Particles", src);
	imwrite("LargestContourandClosing/Fish4_L2V3_20161018_141322074_0000000587/3_remove_particles.png", src);

	//Find largest contour
	Mat srcCopy = src.clone();
	Mat largestContour = findLargestContour(srcCopy);
	namedWindow("4. Largest Contour", CV_WINDOW_NORMAL);
	imshow("4. Largest Contour", largestContour);
	imwrite("LargestContourandClosing/Fish4_L2V3_20161018_141322074_0000000587/4_largest_contour.png", largestContour);

	//Morphological closing orignal image
	int i = 12;
	src = close(src, i, 2);
	namedWindow("5. Closing on Color Filtered Image", CV_WINDOW_NORMAL);
	imshow("5. Closing on Color Filtered Image", src);
	imwrite("LargestContourandClosing/Fish4_L2V3_20161018_141322074_0000000587/5_closing_on_color_filtered_image.png", src);
	
	//Mask original image
	Mat maskedSrc;
	src.copyTo(maskedSrc, largestContour);
	namedWindow("6. Masked Source", CV_WINDOW_NORMAL);
	imshow("6. Masked Source", maskedSrc);
	imwrite("LargestContourandClosing/Fish4_L2V3_20161018_141322074_0000000587/6_masked_source.png", maskedSrc);

	//Closing finished image
	/*int j = 10;
	maskedSrc = close(maskedSrc, j, 2);
	namedWindow("7. Done", CV_WINDOW_NORMAL);
	imshow("7. Done", maskedSrc);*/

	
	

	waitKey(0);
	return 0;
}

