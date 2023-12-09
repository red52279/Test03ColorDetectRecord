#include <vector>
#include <detect/detect.h>
#include <opencv2/opencv.hpp>

std::vector<std::vector<cv::Point>> contours;
std::vector<cv::Vec4i> hierarchy;

cv::Mat pretreat(cv::Mat& img, cv::Scalar& AimColor, cv::Scalar& ColorRange)
{
	cv::Mat img32;
	cv::Scalar HSVL, HSVH;
	HSVL = AimColor - ColorRange;
	HSVH = AimColor + ColorRange;
	img.convertTo(img32, CV_32FC3, 1 / 255.0, 0);
	cv::cvtColor(img32, img32, cv::COLOR_BGR2HSV);
	cv::inRange(img32, HSVL, HSVH, img32);
	return img32;
}

void FindImgContours(cv::Mat& img, cv::Mat& ErodeKernel, cv::Mat& DilateKernel, 
				int ErodeTimes, int DilateTimes, int CannyLowThresh, int CannyHighThresh)
{
	for(int i = 0; i < ErodeTimes; i++)
		cv::morphologyEx(img, img, cv::MORPH_ERODE, ErodeKernel);
	for(int i = 0; i < DilateTimes; i++)
		cv::morphologyEx(img, img, cv::MORPH_DILATE, DilateKernel);

	cv::Mat imgCanny = cv::Mat::zeros(img.size(), CV_8U);
	cv::Canny(img, imgCanny, CannyLowThresh, CannyHighThresh);

	cv::findContours(imgCanny, contours, hierarchy, 0, 2, cv::Point());
}

void DrawRect(cv::Mat& img, cv::Scalar& Color, int areaThread)
{
    for (int i = 0; i < contours.size(); i++)
	{
		int area = cv::contourArea(contours[i]);
		if (area < areaThread)
			continue;
		cv::RotatedRect rect = cv::minAreaRect(contours[i]);
		cv::Point2f points[4];
		rect.points(points);
		for (int j = 0; j < 4; j++)
		{
			if (j == 3)
			{
				cv::line(img, points[j], points[0], Color, 2, 8, 0);
				break;
			}
			cv::line(img, points[j], points[j + 1], Color, 2, 8, 0);
		}
	}
}
std::vector<std::vector<cv::Point>> detect(cv::Mat &img, cv::Scalar &AimColor,cv::Scalar ColorRange, bool isDrawRect)
{
	cv::Mat Erode_Kernel = cv::getStructuringElement(0, cv::Size(3, 3));
	cv::Mat Dilate_Kernel = cv::getStructuringElement(0, cv::Size(3, 3));
	cv::Mat img32 = pretreat(img, AimColor, ColorRange);
	FindImgContours(img32, Erode_Kernel, Dilate_Kernel, 8, 5);
	if(isDrawRect)
		DrawRect(img, AimColor, 6000);
	return contours;
}