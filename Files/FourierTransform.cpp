#include "FourierTransform.h"


Mat DFT(Mat src, float threshold_value)
{
	Mat complex = computeDFT(src);

	namedWindow("spectrum", CV_WINDOW_NORMAL);
	namedWindow("result", CV_WINDOW_NORMAL);

	Mat mask = updateMag(complex, threshold_value);         // compute magnitude of complex, switch to logarithmic scale and display...
	//mask = Mat::ones(mask.size(), CV_32F) - mask;

	//Mat mask = createGausFilterMask(complex.size(), 1944, 1350, 11, true, true);
	shift(mask);  // rearrange quadrants of mask

	Mat planes[] = {
		Mat::zeros(complex.size(), CV_32F),
		Mat::zeros(complex.size(), CV_32F)
	};
	Mat kernel_spec;
	planes[0] = mask; // real
	planes[1] = mask; // imaginar
	merge(planes, 2, kernel_spec);

	mulSpectrums(complex, kernel_spec, complex, DFT_ROWS);

	Mat dst = updateResult(complex);      // do inverse transform and display the result image

	return dst;
}

Mat updateResult(Mat complex)
{
	Mat work;
	idft(complex, work);
	//  dft(complex, work, DFT_INVERSE + DFT_SCALE);
	Mat planes[] = { Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F) };
	split(work, planes);                // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

	magnitude(planes[0], planes[1], work);    // === sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)
	normalize(work, work, 0, 1, NORM_MINMAX);
	imshow("result", work);

	work.convertTo(work, CV_8UC3, 255, 0);
	imwrite("after_DFT.png", work);

	return work;
}

Mat updateMag(Mat complex, float threshold_value)
{

	Mat magI;
	Mat planes[] = { Mat::zeros(complex.size(), CV_32F), Mat::zeros(complex.size(), CV_32F) };
	split(complex, planes);                // planes[0] = Re(DFT(I)), planes[1] = Im(DFT(I))

	magnitude(planes[0], planes[1], magI);    // sqrt(Re(DFT(I))^2 + Im(DFT(I))^2)

											  // switch to logarithmic scale: log(1 + magnitude)
	magI += Scalar::all(1);
	log(magI, magI);

	shift(magI);
	normalize(magI, magI, 1, 0, NORM_INF); // Transform the matrix with float values into a
										   // viewable image form (float between values 0 and 1).

	namedWindow("mag", CV_WINDOW_NORMAL);
	imshow("mag", magI);

	Mat spectrum = magI;
	spectrum.convertTo(spectrum, CV_8UC3, 255, 0);
	imwrite("spectrum.png", spectrum);


	Mat thres_lower = magI.clone();
	Mat thres_higher = magI.clone();
	//inRange(magI, Scalar(0.2), Scalar(0.4), thres);
	threshold(thres_lower, thres_lower, threshold_value, 1, THRESH_BINARY);
	threshold(thres_higher, thres_higher, 0.8, 1, THRESH_BINARY);
	//threshold(magI, magI, 0.6, 1, 0);
	magI = thres_lower - thres_higher;

	imshow("spectrum", magI);

	Mat mask = magI;
	mask.convertTo(mask, CV_8UC3, 255, 0);
	imwrite("mask2.png", mask);

	return magI;
}


Mat computeDFT(Mat image) {
	Mat padded;                            //expand input image to optimal size
	int m = getOptimalDFTSize(image.rows);
	int n = getOptimalDFTSize(image.cols); // on the border add zero values
	copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, BORDER_CONSTANT, Scalar::all(0));
	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F) };
	Mat complex;
	merge(planes, 2, complex);         // Add to the expanded another plane with zeros
	dft(complex, complex, DFT_COMPLEX_OUTPUT);  // furier transform
	return complex;
}



void shift(Mat magI) {

	// crop if it has an odd number of rows or columns
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
	Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
	Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
	Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

	Mat tmp;                            // swap quadrants (Top-Left with Bottom-Right)
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);
	q1.copyTo(tmp);                     // swap quadrant (Top-Right with Bottom-Left)
	q2.copyTo(q1);
	tmp.copyTo(q2);
}




Mat createGausFilterMask(Size mask_size, int x, int y, int ksize, bool normalization, bool invert) {
	// Some corrections if out of bounds
	if (x < (ksize / 2)) {
		ksize = x * 2;
	}
	if (y < (ksize / 2)) {
		ksize = y * 2;
	}
	if (mask_size.width - x < ksize / 2) {
		ksize = (mask_size.width - x) * 2;
	}
	if (mask_size.height - y < ksize / 2) {
		ksize = (mask_size.height - y) * 2;
	}

	// call openCV gaussian kernel generator
	double sigma = -1;
	Mat kernelX = getGaussianKernel(ksize, sigma, CV_32F);
	Mat kernelY = getGaussianKernel(ksize, sigma, CV_32F);
	// create 2d gaus
	Mat kernel = kernelX * kernelY.t();
	// create empty mask
	Mat mask = Mat::zeros(mask_size, CV_32F);
	Mat maski = Mat::zeros(mask_size, CV_32F);

	// copy kernel to mask on x,y
	Mat pos(mask, Rect(x - ksize / 2, y - ksize / 2, ksize, ksize));
	kernel.copyTo(pos);

	// create mirrored mask
	Mat posi(maski, Rect((mask_size.width - x) - ksize / 2, (mask_size.height - y) - ksize / 2, ksize, ksize));
	kernel.copyTo(posi);
	// add mirrored to mask
	add(mask, maski, mask);

	// transform mask to range 0..1
	if (normalization) {
		normalize(mask, mask, 0, 1, NORM_MINMAX);
	}

	// invert mask
	if (invert) {
		mask = Mat::ones(mask.size(), CV_32F) - mask;
	}

	namedWindow("mask", CV_WINDOW_NORMAL);
	imshow("mask", mask);

	return mask;
}
