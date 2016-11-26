#ifndef CAPTUREIMAGE_H
#define CAPTUREIMAGE_H

#include <conio.h>
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

// The Raytrix Light Field API
#include "Rx.ApiLF\Rx.ApiLF.h"
#include "Rx.FileIO\Rx.FileIO.Image.h"

// Include CLUViz Tool DLL to display/load/save images.
#include "Rx.CluViz.Core.CluVizTool\CvCluVizTool.h"

class CaptureImage
{
public:
	CaptureImage::CaptureImage(Rx::CRxString filePath);
	CaptureImage::CaptureImage(int calibration);
	~CaptureImage();
	Rx::CRxImage LoadFrame();

private:
	void Setup();
	void ProcessImage(unsigned fill);
	int CameraCapture();
	int LoadFromFile();

	bool loadFile;
	Rx::CRxString sxFile;
	int iWidth;
	int iHeight;
	float* pData;
	Rx::CRxString sxValue;
	unsigned infill = 1;
};

#endif // CAPTUREIMAGE_H