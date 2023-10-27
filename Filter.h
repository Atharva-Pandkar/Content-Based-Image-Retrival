#pragma once
int blur5x5(cv::Mat& src, cv::Mat& dst);
int sobelY3x3(cv::Mat& src, cv::Mat& dst);
int sobelX3x3(cv::Mat& src, cv::Mat& dst);
int blurQuantize(cv::Mat& src, cv::Mat& dst, int levels);
int Neg(cv::Mat& src, cv::Mat& dst);
int greyscale(cv::Mat& src, cv::Mat& dst);
int magnitude(cv::Mat& sx, cv::Mat& sy, cv::Mat& dst);
int cartoon(cv::Mat& src, cv::Mat& dst, int levels = 10, int magThreshold = 8);