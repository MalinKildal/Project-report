#include "Filters.h"


Mat Erode(Mat src, int kernelSize, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		Point(kernelSize, kernelSize));

	erode(src, dst, element, Point(-1, -1), iterations);

	return dst;
}


Mat Dilate(Mat src, int kernelSize, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		Point(kernelSize, kernelSize));

	dilate(src, dst, element, Point(-1,-1), iterations);

	return dst;
}


Mat Close(Mat src, int kernelSize, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		Point(kernelSize, kernelSize));

	morphologyEx(src, dst, MORPH_CLOSE, element, Point(-1, -1), iterations);

	return dst;
}


Mat Open(Mat src, int kernelSize, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernelSize + 1, 2 * kernelSize + 1),
		Point(kernelSize, kernelSize));

	morphologyEx(src, dst, MORPH_OPEN, element, Point(-1, -1), iterations);

	return dst;
}


Mat BilateralFilter(Mat src, int kernelSize)
{
	Mat dst;
	bilateralFilter(src, dst, kernelSize, kernelSize*2, kernelSize/2);
	return dst;
}


Mat MedianFilter(Mat src, int kernelSize, int iterations)
{
	for (int i = 0; i < iterations; i++)
	{
		medianBlur(src, src, kernel_size);
	}
	
	return src;
}


Mat ColorFilter(Mat src)
{
	Mat channel[3];
	split(src, channel);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			Vec3b color = src.at<Vec3b>(i, j);
			if (color[0] > 150)  //Set all pixels with high blue values to 0 (black)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
			if (color[1] < 220 && color[1] > 0)  //Set all pixels with low green values to 0 (black)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
			if (color[2] < 200 && color[2] > 0)  //Set all pixels with low red values to 0 (black)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
			src.at<Vec3b>(i, j) = color;
		}
	}

	return src;
}


Mat LocalThresholding(Mat src, int kernelSize, int c)
{
	Mat copy = src.clone();

	Mat gray;
	cvtColor(src, gray, CV_BGR2GRAY);

	Mat thresh;
	adaptiveThreshold(gray, thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, kernelSize, c);
	
	Mat eroded = Erode(thresh, 3, 1);

	Mat masked;
	copy.copyTo(masked, eroded);

	Mat dilated = Dilate(masked, 2, 1);

	return dilated;
}
