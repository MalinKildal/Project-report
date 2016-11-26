/*	RULES:
*	Keep main clean! Object orient!
*	Never make anything completely dependant on anything else. Never add code that are impossible to remove or alter without impacting everything else.
*	Variables are camelBack named. Methods and classes are CamelCaps.
*	No namespace!
*	If any standard is uncertain, travel to Rome.
*/

#include "CaptureImage.h"
#include "CVDraw.h"
#include "ImageOperations.h"
#include "Filters.h"
#include "FourierTransform.h"
#include "FindLargestContour.h"
#include <string>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;


const string readFrom = "C:/Users/Raytrix/Desktop/Laks_Malin/Noise_Level_1/";
const string saveTo   = "Fish/Test1/";
const string ending   = ".png";

const int particleRemovalKernelSize = 1;
const int ParticleRemovalIterations = 2;
const int closingKernelSize         = 12;
const int closingIterations         = 3;
const int localThreshKernelSize     = 9;
const int localThreshC              = 10;
const int medianFilterKernelSize    = 7;
const int medianFilterIterations    = 3;

vector<int> compression_params = { CV_IMWRITE_PNG_COMPRESSION, 0 };		//Make saving lossless


void ShowAndWrite(Mat image, string displayName, string saveName)
{
	namedWindow(displayName, CV_WINDOW_NORMAL);
	imshow(displayName, image);
	imwrite(saveTo + saveName + ending, image, compression_params);
}


Mat EnhanceDepthmap(Mat src)
{
    //Filter on color
	src = ColorFilter(src);

	//Remove small particles
	src = Erode(src, particleRemovalKernelSize, ParticleRemovalIterations);
	src = Dilate(src, particleRemovalKernelSize, ParticleRemovalIterations);

	//Find largest contour
	Mat srcCopy = src.clone();
	Mat largestContour = FindLargestContourDepthmap(srcCopy);

	//Morphological closing orignal image
	src = Close(src, closingKernelSize, closingIterations);

	//Mask original image
	Mat maskedSrc;
	src.copyTo(maskedSrc, largestContour);

	//Remove wrong pixel values on edges using adaptive thresholding
	Mat evenEdges = LocalThresholding(maskedSrc, localThreshKernelSize, localThreshC);

	//Median filter
	Mat dst = MedianFilter(evenEdges, medianFilterKernelSize, medianFilterIterations);
    
    return dst;
}


int main(int argc, char* argv[])
{
	//Open .ray file
	CaptureImage ci = CaptureImage("C:/Users/Raytrix/Desktop/Laks_Malin/Test1/L2V2_20161018_141258159_0000000582.ray");
	Rx::CRxImage depthmap = ci.LoadFrame();
    //Make Mat from .ray file
	CVDraw cd2 = CVDraw(depthmap);
	Mat src = cd2.MakeMat();
	
	//Resize totalfocus image
	Mat totalfocus = imread(readFrom + "L2V2_20161018_141258159_0000000582_TotalFocus.png", 1);
	Size size(src.cols, src.rows);
	Mat totalfocusResize;
	resize(totalfocus, totalfocusResize, size);
    
    //Save original depthmap and totalfocus image
	ShowAndWrite(totalfocusResize, "Totalfocus", "totalfocus");
	ShowAndWrite(src, "Original Depthmap", "originalDepthmap");
    
    //Enhance depthmap
    Mat dst = EnhanceDepthmap(src);
	ShowAndWrite(dst, "Enhanced Depthmap", "enhancedDepthmap");
    
	//Convert to grayscale
	Mat grayImage;
	cvtColor(dst, grayImage, CV_BGR2GRAY);
	ShowAndWrite(grayImage, "Grayscale", "grayscale");
    
	waitKey(0);
	return 0;
}

