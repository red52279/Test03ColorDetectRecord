#ifndef _DETECT_H
#define _DETECT_H

#include <vector>
#include <opencv2/opencv.hpp>

cv::Mat pretreat(cv::Mat& img, cv::Scalar& AimColor, cv::Scalar& ColorRange);
void FindImgContours(cv::Mat &img, cv::Mat& ErodeKernel, cv::Mat & DilateKernel,
                        int ErodeTimes = 1, int DilateTimes = 1,int CannyLowThresh = 100, int CannyHighThresh = 200);
void DrawRect(cv::Mat& img, cv::Scalar& Color, int areaThread = 0);
std::vector<std::vector<cv::Point>> detect(cv::Mat& img, cv::Scalar& AimColor, cv::Scalar ColorRange, bool isDrawRect = true);

#endif 
