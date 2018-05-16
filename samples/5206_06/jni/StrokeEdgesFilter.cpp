#include <opencv2/imgproc/imgproc.hpp>

#include "StrokeEdgesFilter.hpp"

using namespace secondsight;

StrokeEdgesFilter::StrokeEdgesFilter()
{
    mKernel.create(5, 5, CV_8S);
    mKernel.at<char>(0, 0) = 0;
    mKernel.at<char>(0, 1) = 0;
    mKernel.at<char>(0, 2) = 1;
    mKernel.at<char>(0, 3) = 0;
    mKernel.at<char>(0, 4) = 0;
    mKernel.at<char>(1, 0) = 0;
    mKernel.at<char>(1, 1) = 1;
    mKernel.at<char>(1, 2) = 2;
    mKernel.at<char>(1, 3) = 1;
    mKernel.at<char>(1, 4) = 0;
    mKernel.at<char>(2, 0) = 1;
    mKernel.at<char>(2, 1) = 2;
    mKernel.at<char>(2, 2) = -16;
    mKernel.at<char>(2, 3) = 2;
    mKernel.at<char>(2, 4) = 1;
    mKernel.at<char>(3, 0) = 0;
    mKernel.at<char>(3, 1) = 1;
    mKernel.at<char>(3, 2) = 2;
    mKernel.at<char>(3, 3) = 1;
    mKernel.at<char>(3, 4) = 0;
    mKernel.at<char>(4, 0) = 0;
    mKernel.at<char>(4, 1) = 0;
    mKernel.at<char>(4, 2) = 1;
    mKernel.at<char>(4, 3) = 0;
    mKernel.at<char>(4, 4) = 0;
}

void StrokeEdgesFilter::apply(cv::Mat &src, cv::Mat &dst)
{
    cv::filter2D(src, mEdges, -1, mKernel);
    cv::bitwise_not(mEdges, mEdges);
    cv::multiply(src, mEdges, dst, 1.0/255.0);
}
