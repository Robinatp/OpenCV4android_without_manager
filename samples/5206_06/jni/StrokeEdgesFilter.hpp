#ifndef STROKE_EDGES_FILTER
#define STROKE_EDGES_FILTER

#include <opencv2/core/core.hpp>

namespace secondsight {

class StrokeEdgesFilter
{
public:
    StrokeEdgesFilter();
    void apply(cv::Mat &src, cv::Mat &dst);

private:
    cv::Mat mKernel;
    cv::Mat mEdges;
};

} // namespace secondsight

#endif // STROKE_EDGES_FILTER
