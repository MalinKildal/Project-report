#include "Filters.h"


Mat erode(Mat src, int kernel_size, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernel_size + 1, 2 * kernel_size + 1),
		Point(kernel_size, kernel_size));

	erode(src, dst, element, Point(-1, -1), iterations);

	return dst;
}


Mat dilate(Mat src, int kernel_size, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernel_size + 1, 2 * kernel_size + 1),
		Point(kernel_size, kernel_size));

	dilate(src, dst, element, Point(-1,-1), iterations);

	return dst;
}


Mat close(Mat src, int kernel_size, int iterations)
{
	Mat dst;
	Mat element = getStructuringElement(MORPH_ELLIPSE,
		Size(2 * kernel_size + 1, 2 * kernel_size + 1),
		Point(kernel_size, kernel_size));

	morphologyEx(src, dst, MORPH_CLOSE, element, Point(-1, -1), iterations);

	return dst;
}


Mat bilateral_filter(Mat src, int kernel_size)
{
	Mat dst;
	bilateralFilter(src, dst, kernel_size, kernel_size*2, kernel_size/2);
	return dst;
}


Mat median_filter(Mat src, int kernel_size)
{
	Mat dst;
	medianBlur(src, dst, kernel_size);
	return dst;
}


Mat filterColor(Mat src)
{
	Mat channel[3];
	split(src, channel);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			Vec3b color = src.at<Vec3b>(i, j);
			if (color[0] > 55)  //Set all pixels with high blue values to 0 (black)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
			if (color[1] < 220 && color[1] > 1)  //Set all pixels with low green values to 0 (black)
			{
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
			if (color[2] < 200 && color[2] > 1)  //Set all pixels with low red values to 0 (black)
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
