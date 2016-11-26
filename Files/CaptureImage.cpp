#include "CaptureImage.h"

Rx::CRxArrayString caliblist;
int chosenCalib;
bool m_bImageAvailable = false;

CaptureImage::CaptureImage(Rx::CRxString filePath)
{
	loadFile = true;
	sxFile = filePath;
	Setup();
}

CaptureImage::CaptureImage(int calibration)
{
	loadFile = false;
	chosenCalib = calibration;
	Setup();
}

void CaptureImage::Setup() {
	Rx::ApiLF::RxInit(true);
	Rx::ApiLF::RxCudaSelectDevice();
}


CaptureImage::~CaptureImage() {
	Rx::ApiLF::RxFinalize();
}

void CaptureImage::ProcessImage(unsigned fill)
{
	// Preprocess image
	Rx::ApiLF::RxPreProcess();

	// First set the current target view to the view camera space
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Proj_CurrentTargetView, unsigned(Rx::LF::ESpace::View_Object_Pinhole));

	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Focus_ResImgDiv, 2.0);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_ResImgDiv, 2.0);

	// Sharpen raw image
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_Sharp1_Enable, 1U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_Sharp1_BlurStdDev, 2.5);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_Sharp1_Factor, 1.5);
	// Denoise raw image
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_DenoiseNLM_Enable, 0U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_DenoiseNLM_FilterDia, 4U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_DenoiseNLM_BlendFactor, 0.2);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::PreProc_DenoiseNLM_NoiseLevel, 0.1);


	// Set depth parameters for different lens types.
	// Make depth parameters for different lens types equal.
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_NearResolutionLevel, 3U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_MinStdDeviation, 0.00);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_MinCorrelation, 0.930);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_PatchDiameter, 5U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_PatchStride, 1U);

	// Depth Fill Parameters
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_Enabled, 1U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_IterCnt, 6U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_IterSize, 5U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_Complete, fill);

	// Depth Fill Bilateral denoise filter
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_Bilateral_Enabled, 1U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_Bilateral_FilterRad, 20U);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_Bilateral_Edge, 0.1);
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Depth_Fill_Bilateral_Range, 20.0);

	// Calculate the raw depth and subsequently the fused depth map in the view camera space additionally process the depth 3D image
	Rx::ApiLF::RxDepthRay();
	Rx::ApiLF::RxDepthMap();
	Rx::ApiLF::RxDepth3D();

	// Color code the calculated depth to visualize it properly
	Rx::ApiLF::RxDepthColorCode();

	// Now calculate the total focus image
	Rx::ApiLF::RxTotalFocus();
}

void ImageCallback(unsigned uCamIdx, void* pvContext)
{
	// Retrieve the image from the image and write the image data into the bound image.
	Rx::ApiLF::RxCamRetrieveImage();
	Rx::ApiLF::RxCalibDBBindCalib(caliblist[chosenCalib], true);

	m_bImageAvailable = true;
}

int CaptureImage::CameraCapture() {
	// Initialize CLUVizTool
	printf("Initializing CLUVizTool...\n");
	CLUViz::Tool::Init();

	// Create an image view
	int iView = 0;
	CLUViz::Tool::CreateViewImage(iView, 600, 100, 800, 600, "Image");

	// Initialize cameras
	printf("Initializing camera drivers...");
	Rx::ApiLF::RxCamDriverInit();
	printf("done.\n");

	printf("Registering cameras...");
	Rx::ApiLF::RxCamRegister();
	printf("done.\n");

	// Register the image callback in the API
	Rx::ApiLF::RxCamRegisterImageCallback(Rx::ApiLF::TFuncCamImageCallback(ImageCallback), nullptr);

	// Get information on available cameras
	unsigned uCameraCount;
	Rx::ApiLF::RxGetPar(Rx::ApiLF::EPar::Cam_Count, uCameraCount);

	if (uCameraCount == 0)
	{
		printf("No cameras available. Press any key to end...\n");
		_getch();
		return -1;
	}
	printf("Number of cameras available: %d\n", uCameraCount);


	// Get information on available camera calibration settings
	// Work with camera 0
	unsigned uCamID = 0;

	// Before the calibration database for a camera can be accessed
	// the camera has to be opened.
	printf("Opening camera 0...");
	Rx::ApiLF::RxCamOpen(uCamID);
	printf("done.\n");

	printf("Binding camera 0 ...");
	Rx::ApiLF::RxCamBind(uCamID);
	printf("done.\n");

	printf("______________________________________________\n");
	printf("Camera %d:\n", uCamID);

	Rx::ApiLF::RxCamGetPar(uCamID, Rx::ApiLF::ECamPar::DriverName, sxValue);
	printf(">> Type: %s\n", sxValue.ToCString());

	Rx::ApiLF::RxCamGetPar(uCamID, Rx::ApiLF::ECamPar::HardwareId, sxValue);
	printf(">> ID  : %s\n", sxValue.ToCString());

	printf("\n\n");
	if (chosenCalib == 451) {
		Rx::ApiLF::RxCalibDBGetCameraCalibGuidList(caliblist, uCamID);

		std::cout << "Please choose your calibration settings:\n";

		while (chosenCalib == 451) {
			int ch;
			for (int i = 0; i < caliblist.Length(); i++) {
				std::cout << "'" << i << "': " << caliblist[i].ToCString() << "\n";
			}
			ch = _getch();
			ch = toupper(ch);
			for (int i = 0; i < caliblist.Length(); i++) {
				if (ch == std::to_string(i)[0]) {
					chosenCalib = i;
					break;
				}
				if (i == caliblist.Length() - 1) {
					std::cout << "Invalid input\n\n";
				}
			}
		}
	}

	// Capture images from camera
	Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Proj_CurrentTargetView, unsigned(Rx::LF::ESpace::View_Object_Pinhole));
	Rx::ApiLF::RxCamSetPar(Rx::ApiLF::ECamPar::TriggerMode, unsigned(Rx::Interop::Runtime30::Camera::ETriggerMode::Software_SnapShot));

	// Now start capturing images
	printf("Starting capture...");
	Rx::ApiLF::RxCamStartCapture();
	printf("done.\n\n");

	// Get the current camera exposure
	double dValue;
	Rx::ApiLF::RxCamGetPar(Rx::ApiLF::ECamPar::Exposure, dValue);

	//RxCamGetPar(uCamID, ECamPar::Exposure, dValue);
	printf("Init Exposure: %g\n", dValue);

	// Set the camera exposure in milliseconds
	Rx::ApiLF::RxCamSetPar(Rx::ApiLF::ECamPar::Exposure, 15.0);

	// Depending on the camera and other settings, the exposure
	// time set in the previous step may not be exactly available.
	// Read the exposure time again to see what the actual value is.
	Rx::ApiLF::RxCamGetPar(Rx::ApiLF::ECamPar::Exposure, dValue);
	printf("New Exposure: %g\n", dValue);

	bool bDoCapture = true;
	double dFocus = 0.5;

	// Loop until the user ends the program
	bool bEnd = false;
	int viewMode = 0;
	while (!bEnd)
	{
		// Set API parameters
		Rx::ApiLF::RxSetPar(Rx::ApiLF::EPar::Focus_RelativeFocusPlane, dFocus);

		if (bDoCapture)
		{
			// Trigger the camera
			Rx::ApiLF::RxCamTrigger();

			ProcessImage(false);
		}

		if (m_bImageAvailable)
		{
			ProcessImage((unsigned)infill);
			// Get focused image from CUDA device
			Rx::CRxImage xImage;
			Rx::CRxImage xDepth;
			Rx::CRxImage xDepth3dColor;
			Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::TotalFocus_View_Object_Pinhole, xImage);
			Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::Depth3D, xDepth);
			Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::DepthMapColored_View_Object_Pinhole, xDepth3dColor);

			// Display the image
			if (viewMode == 0) {
				CLUViz::Tool::ViewSetImage(iView, &xImage);
			}
			if (viewMode == 1) {
				CLUViz::Tool::ViewSetImage(iView, &xDepth3dColor);
			}

			printf("\nMenu:\n");
			printf("'c': Capture new image\n");
			printf("'d': DepthMap\n");
			printf("'t': TotalFocus\n");
			if (infill) {
				printf("'f': No Fill\n");
			}
			else {
				printf("'f': Fill\n");
			}
			printf("'q': Continue\n\n");

			switch (_getch())
			{
			case 'c':
				if (m_bImageAvailable)
				{
					bDoCapture = true;
					m_bImageAvailable = false;
				}
				break;
			case 'd':
				viewMode = 1;
				break;
			case 't':
				viewMode = 0;
				break;
			case 'f':
				infill = !infill;
				break;
			case 'q':
				bEnd = true;
			}
		}
	}
	CLUViz::Tool::Finalize();
	return 1;
}

int CaptureImage::LoadFromFile() {
	// Load a ray image
	std::cout << "Loading image " << sxFile.ToCString() << "...\n";
	unsigned uImgID = Rx::ApiLF::RxRayLoad(sxFile);

	// Bind the loaded ray image. This copies the ray image to the CUDA device.
	std::cout << "Binding image...\n";
	Rx::ApiLF::RxRayBind(uImgID);

	// Processes image with no whiteimage
	ProcessImage(0);

	return 1;
}

Rx::CRxImage CaptureImage::LoadFrame() {
	Rx::CRxImage TotalFocusImage;
	Rx::CRxImage Depth3dImage;
	Rx::CRxImage Depth3dColor;

	if (loadFile) {
		LoadFromFile();
	}
	else {
		CameraCapture();
	}

	Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::Depth3D, Depth3dImage);
	Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::TotalFocus_View_Object_Pinhole, TotalFocusImage);

	pData = (float*)Depth3dImage.GetDataPtr();

	iWidth = Depth3dImage.GetFormat().m_iWidth;
	iHeight = Depth3dImage.GetFormat().m_iHeight;
	std::cout << "Height of image: " << iHeight << ". Width of image: " << iWidth << ".\n";

	ProcessImage(0);

	Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::TotalFocus_View_Object_Pinhole, TotalFocusImage);
	Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::Depth3D, Depth3dImage);
	Rx::ApiLF::RxGetImage(Rx::ApiLF::EImgID::DepthMapColored_View_Object_Pinhole, Depth3dColor);

	return Depth3dColor;
}