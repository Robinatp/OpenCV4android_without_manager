#ifndef IMAGE_DETECTION_FILTER
#define IMAGE_DETECTION_FILTER

#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>

namespace secondsight {

class ImageDetectionFilter
{
public:
    ImageDetectionFilter(cv::Mat &referenceImageBGR,
            double realSize);
    float *getGLPose();
    void apply(cv::Mat &src, cv::Mat &dst, cv::Mat &projection);

private:
    void findPose(cv::Mat &projection);
    void draw(cv::Mat src, cv::Mat dst);

    // The reference image (this detector's target).
    cv::Mat mReferenceImage;
    // Features of the reference image.
    std::vector<cv::KeyPoint> mReferenceKeypoints;
    // Descriptors of the reference image's features.
    cv::Mat mReferenceDescriptors;
    // The corner coordinates of the reference image, in pixels.
    cv::Mat mReferenceCorners;
    // The reference image's corner coordinates, in 3D, in real
    // units.
    std::vector<cv::Point3f> mReferenceCorners3D;

    // Features of the scene (the current frame).
    std::vector<cv::KeyPoint> mSceneKeypoints;
    // Descriptors of the scene's features.
    cv::Mat mSceneDescriptors;
    // Tentative corner coordinates detected in the scene, in
    // pixels.
    cv::Mat mCandidateSceneCorners;

    // A grayscale version of the scene.
    cv::Mat mGraySrc;
    // Tentative matches of scene features and reference features.
    std::vector<cv::DMatch> mMatches;

    // A feature detector, which finds features in images, and
    // descriptor extractor, which creates descriptors of features.
    cv::Ptr<cv::Feature2D> mFeatureDetectorAndDescriptorExtractor;
    // A descriptor matcher, which matches features based on their
    // descriptors.
    cv::Ptr<cv::DescriptorMatcher> mDescriptorMatcher;

    // Distortion coefficients of the camera's lens.
    cv::Mat mDistCoeffs;

    // The Euler angles of the detected target.
    cv::Mat mRVec;
    // The XYZ coordinates of the detected target.
    cv::Mat mTVec;
    // The rotation matrix of the detected target.
    cv::Mat mRotation;
    // The OpenGL pose matrix of the detected target.
    float mGLPose[16];

    // Whether the target is currently detected.
    bool mTargetFound;
};

} // namespace secondsight

#endif // IMAGE_DETECTION_FILTER
