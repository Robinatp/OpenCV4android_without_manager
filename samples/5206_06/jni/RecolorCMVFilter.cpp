#include "RecolorCMVFilter.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
using namespace secondsight;

void RecolorCMVFilter::apply(cv::Mat &src, cv::Mat &dst)
{
    cv::split(src, mChannels);

    cv::Mat r = mChannels[0];
    cv::Mat g = mChannels[1];
    cv::Mat b = mChannels[2];

    // dst.b = max(dst.r, dst.g, dst.b)
    cv::max(b, r, b);
    cv::max(b, g, b);

    cv::merge(mChannels, 4, dst);
}
