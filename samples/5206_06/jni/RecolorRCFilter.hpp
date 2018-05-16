#ifndef RECOLOR_RC_FILTER
#define RECOLOR_RC_FILTER

#include <opencv2/core/core.hpp>

namespace secondsight {

class RecolorRCFilter
{
public:
    void apply(cv::Mat &src, cv::Mat &dst);

private:
    cv::Mat mChannels[4];
};

} // namespace secondsight

#endif // RECOLOR_RC_FILTER
