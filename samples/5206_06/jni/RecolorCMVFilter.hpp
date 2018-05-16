#ifndef RECOLOR_CMV_FILTER
#define RECOLOR_CMV_FILTER

#include <opencv2/core/core.hpp>

namespace secondsight {

class RecolorCMVFilter
{
public:
    void apply(cv::Mat &src, cv::Mat &dst);

private:
    cv::Mat mChannels[4];
};

} // namespace secondsight

#endif // RECOLOR_CMV_FILTER
