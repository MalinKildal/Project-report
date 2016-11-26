#include "FindLargestContour.h"

Mat FindLargestContourDepthmap(Mat src)
{
	//1. Convert to gray and normalize
	Mat gray(src.rows, src.cols, CV_8UC1);
	cvtColor(src, gray, CV_BGR2GRAY);
	normalize(gray, gray, 0, 255, NORM_MINMAX, CV_8UC1);

	//2. Edge detector
	GaussianBlur(gray, gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
	Mat edges;
	//Use Sobel filter and thresholding.
	edges = Sobel(gray);
	//Manual thresholding
	threshold(edges, edges, 25, 255, CV_THRESH_BINARY);

	//3. Dilate
	Mat dilateMat;
	int dilateSize = 3;
	int dilteIterations = 2;
	dilateMat = dilate(edges, dilateSize, dilteIterations);

	//4. Floodfill
	Mat floodFilled = Mat::zeros(dilateMat.rows + 2, dilateMat.cols + 2, CV_8U);
	floodFill(dilateMat, floodFilled, Point(0, 0), 0, 0, Scalar(), Scalar(), 4 + (255 << 8) + FLOODFILL_MASK_ONLY);
	floodFilled = Scalar::all(255) - floodFilled;
	Mat temp;
	floodFilled(Rect(1, 1, dilateMat.cols - 2, dilateMat.rows - 2)).copyTo(temp);
	floodFilled = temp;

	//5. Erode
	Mat erodeMat;
	int erosionSize = 3;
	int erodeIterations = 2;
	erodeMat = erode(floodFilled, erosionSize, erodeIterations);

	//6. Find largest contour
	int largestArea = 0;
	int largestContourIndex = 0;
	Rect boundingRectangle;
	Mat largestContour(src.rows, src.cols, CV_8UC1, Scalar::all(0));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(erodeMat, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i<contours.size(); i++)
	{
		int contour = (int)contourArea(contours[i], false);
		if (contour > largestArea)
		{
			largestArea = contour;
			largestContourIndex = i;
			boundingRectangle = boundingRect(contours[i]);
		}
	}
	
	Scalar color(255, 255, 255);
	drawContours(largestContour, contours, largestContourIndex, color, CV_FILLED, 8, hierarchy); //Draw the largest contour using previously stored index.
	rectangle(src, boundingRectangle, Scalar(0, 0, 255), 1, 8, 0);

	return largestContour;
}


Mat FindLargestContourTotalfocus(Mat src)
{
	//1. Convert to gray and normalize
	Mat gray(src.rows, src.cols, CV_8UC1);
	cvtColor(src, gray, CV_BGR2GRAY);
	normalize(gray, gray, 0, 255, NORM_MINMAX, CV_8UC1);
	namedWindow("1. Grayscale", CV_WINDOW_NORMAL);
	imshow("1. Grayscale", gray);

	//2. Edge detector
	GaussianBlur(gray, gray, Size(3, 3), 0, 0, BORDER_DEFAULT);
	Mat edges;
	//Use Sobel filter and thresholding.
	edges = Sobel(gray);
	//Manual thresholding
	threshold(edges, edges, 9, 255, CV_THRESH_BINARY);

	//3. Dilate
	Mat dilateMat;
	int dilateSize = 3;
	int dilteIterations = 3;
	dilateMat = dilate(edges, dilateSize, dilteIterations);

	//4. Floodfill
	Mat floodFilled = Mat::zeros(dilateMat.rows + 2, dilateMat.cols + 2, CV_8U);
	floodFill(dilateMat, floodFilled, Point(0, 0), 0, 0, Scalar(), Scalar(), 4 + (255 << 8) + FLOODFILL_MASK_ONLY);
	floodFilled = Scalar::all(255) - floodFilled;
	Mat temp;
	floodFilled(Rect(1, 1, dilateMat.cols - 2, dilateMat.rows - 2)).copyTo(temp);
	floodFilled = temp;

	//5. Erode
	Mat erodeMat;
	int erosionSize = 3;
	int erodeIterations = 3;
	erodeMat = erode(floodFilled, erosionSize, erodeIterations);

	//6. Find largest contour
	int largestArea = 0;
	int largestContourIndex = 0;
	Rect boundingRectangle;
	Mat largestContour(src.rows, src.cols, CV_8UC1, Scalar::all(0));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(erodeMat, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i<contours.size(); i++)
	{
		int contour = (int)contourArea(contours[i], false);
		if (contour > largestArea)
		{
			largestArea = contour;
			largestContourIndex = i;
			boundingRectangle = boundingRect(contours[i]);
		}
	}

	Scalar color(255, 255, 255);
	drawContours(largestContour, contours, largestContourIndex, color, CV_FILLED, 8, hierarchy); //Draw the largest contour using previously stored index.
	rectangle(src, boundingRectangle, Scalar(0, 0, 255), 1, 8, 0);

	return largestContour;
}


Mat Sobel(Mat gray) {
	Mat edges;

	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat edges_x, edges_y;
	Mat abs_edges_x, abs_edges_y;
	Sobel(gray, edges_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(edges_x, abs_edges_x);
	Sobel(gray, edges_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(edges_y, abs_edges_y);
	addWeighted(abs_edges_x, 0.5, abs_edges_y, 0.5, 0, edges);

	return edges;
}