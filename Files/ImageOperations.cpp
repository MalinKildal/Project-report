#include "ImageOperations.h"

float* ImageOperations::getPData(Rx::CRxImage image, float* pData) {
	pData = (float*)image.GetDataPtr();
	return pData;
}

void ImageOperations::removeHeight(int bottom, int top) {

}

// Evaluate straight distance between two points in object space
double ImageOperations::DDistance(int Y1, int X1, int Y2, int X2, int iWidth, float* pData) {
	int iPos1 = 4 * ((Y1 * iWidth) + X1);
	int iPos2 = 4 * ((Y2 * iWidth) + X2);

	float dX1 = pData[iPos1 + 0];
	float dY1 = pData[iPos1 + 1];
	float dZ1 = pData[iPos1 + 2];

	float dX2 = pData[iPos2 + 0];
	float dY2 = pData[iPos2 + 1];
	float dZ2 = pData[iPos2 + 2];

	float dDX = dX2 - dX1;
	float dDY = dY2 - dY1;
	float dDZ = dZ2 - dZ1;

	return sqrt((double)(dDX * dDX + dDY * dDY + dDZ * dDZ));
}

//Finds Z value at a point
float ImageOperations::ZPointer(int Y, int X, int iWidth, int iHeight, float* pData) {
	int iPos = 4 * ((Y * iWidth) + X);
	if (X < 0 || Y < 0 || X >= iWidth || Y >= iHeight) {
		return 0;
	}
	float ret = pData[iPos + 2];
	return ret;
}