#include "CVDraw.h"

CVDraw::CVDraw(Rx::CRxImage image) {
	frame = image;
	pData = (float*)frame.GetDataPtr();
	iWidth = frame.GetFormat().m_iWidth;
	iHeight = frame.GetFormat().m_iHeight;
}

CVDraw::~CVDraw() {
}

void CVDraw::DrawImage() {
	// Save image with the exporter
	Rx::CRxString sxFile2 = "Demo_Depth.png";
	Rx::FileIO::CImage xImageFile;
	std::cout << "Saving depth image as " << sxFile2.ToCString() << "...\n";
	xImageFile.Write(&frame, sxFile2);

	//Displaying the image
	//TODO: Doc
	const std::string windowName = "Image";
	cv::Size size(iWidth, iHeight);
	std::string filename = "Demo_Depth.png";
	cv::Mat imgSrc = cv::imread(filename, 1);
	cv::Mat imgRaySized;
	resize(imgSrc, imgRaySized, size);

	cv::Size size2(1280, 720);
	cv::Mat imgResized;
	resize(imgRaySized, imgResized, size2);

	std::cout << "Press any key to exit.\n";
	imshow(windowName, imgResized);
	//cv::waitKey(0);
}

cv::Mat CVDraw::MakeMat() {
	// Save image with the exporter
	Rx::CRxString sxFile2 = "Demo_Depth.png";
	Rx::FileIO::CImage xImageFile;
	std::cout << "Saving depth image as " << sxFile2.ToCString() << "...\n";
	xImageFile.Write(&frame, sxFile2);

	//Displaying the image
	//TODO: Doc
	const std::string windowName = "Image";
	cv::Size size(iWidth, iHeight);
	std::string filename = "Demo_Depth.png";
	cv::Mat imgSrc = cv::imread(filename, 1);
	cv::Mat imgRaySized;
	resize(imgSrc, imgRaySized, size);

	cv::Size size2(1280, 720);
	cv::Mat imgResized;
	resize(imgRaySized, imgResized, size2);

	return imgResized;
}