#ifndef CVDRAW_H
#define CVDRAW_H

// The Raytrix Light Field API
#include "Rx.ApiLF\Rx.ApiLF.h"
#include "Rx.FileIO\Rx.FileIO.Image.h"

// Include opencv for drawing bodge bodge
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\opencv.hpp>

class CVDraw
{
public:
	CVDraw(Rx::CRxImage);
	~CVDraw();
	void DrawImage();
	cv::Mat MakeMat();

private:
	Rx::CRxImage frame;
	float* pData;
	int iWidth;
	int iHeight;
};

#endif // CVDRAW_H