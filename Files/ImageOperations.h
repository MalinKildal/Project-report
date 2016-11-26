#ifndef IMAGEOPERATIONS_H
#define IMAGEOPERATIONS_H

// The Raytrix Light Field API
#include "Rx.ApiLF\Rx.ApiLF.h"
#include "Rx.FileIO\Rx.FileIO.Image.h"

// Include opencv for drawing bodge bodge
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>


class ImageOperations {
public:
	static float* getPData(Rx::CRxImage image, float* pData);
	static void removeHeight(int bottom, int top);
	static double DDistance(int Y1, int X1, int Y2, int X2, int iWidth, float* pData);
	static float ZPointer(int Y, int X, int iWidth, int iHeight, float* pData);
};

#endif // IMAGEOPERATIONS_H