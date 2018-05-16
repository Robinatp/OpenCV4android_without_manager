#ifndef RECOLOR_RGV_FILTER
#define RECOLOR_RGV_FILTER

#include <opencv2/core/core.hpp>

namespace secondsight {

class RecolorRGVFilter
{
public:
    void apply(cv::Mat &src, cv::Mat &dst);

private:
    cv::Mat mChannels[4];
};

} // namespace secondsight

#endif // RECOLOR_RGV_FILTER
