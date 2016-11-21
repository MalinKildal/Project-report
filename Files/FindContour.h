#ifndef FINDCONTOUR_H
#define FINDCONTOUR_H

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

#include "Filters.h"

using namespace cv;
using namespace std;


Mat findLargestContour(Mat src);

Mat sobel(Mat gray);


#endif // FINDCONTOUR_H
