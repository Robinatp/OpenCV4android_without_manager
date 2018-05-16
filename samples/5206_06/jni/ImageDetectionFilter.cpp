#include <float.h>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "ImageDetectionFilter.hpp"

using namespace secondsight;

ImageDetectionFilter::ImageDetectionFilter(
        cv::Mat &referenceImageBGR, double realSize)
{
    // Create grayscale and RGBA versions of the reference image.
    cv::Mat referenceImageGray;
    cv::cvtColor(referenceImageBGR, referenceImageGray,
            cv::COLOR_BGR2GRAY);
    cv::cvtColor(referenceImageBGR, mReferenceImage,
            cv::COLOR_BGR2RGBA);

    int cols = referenceImageGray.cols;
    int rows = referenceImageGray.rows;

    // Store the reference image's corner coordinates, in pixels.
    mReferenceCorners.create(4, 1, CV_32FC2);
    mReferenceCorners.at<cv::Vec2f>(0, 0)[0] = 0.0f;
    mReferenceCorners.at<cv::Vec2f>(0, 0)[1] = 0.0f;
    mReferenceCorners.at<cv::Vec2f>(1, 0)[0] = cols;
    mReferenceCorners.at<cv::Vec2f>(1, 0)[1] = 0.0f;
    mReferenceCorners.at<cv::Vec2f>(2, 0)[0] = cols;
    mReferenceCorners.at<cv::Vec2f>(2, 0)[1] = rows;
    mReferenceCorners.at<cv::Vec2f>(3, 0)[0] = 0.0f;
    mReferenceCorners.at<cv::Vec2f>(3, 0)[1] = rows;

    // Compute the image's width and height in real units, based
    // on the specified real size of the image's smaller dimension.
    float aspectRatio = (float)cols /(float)rows;
    float halfRealWidth;
    float halfRealHeight;
    if (cols > rows) {
        halfRealHeight = 0.5f * realSize;
        halfRealWidth = halfRealHeight * aspectRatio;
    } else {
        halfRealWidth = 0.5f * realSize;
        halfRealHeight = halfRealWidth / aspectRatio;
    }

    // Define the real corner coordinates of the printed image
    // so that it normally lies in the xy plane (like a painting
    // or poster on a wall).
    // That is, +z normally points out of the page toward the
    // viewer.
    mReferenceCorners3D.push_back(
            cv::Point3f(-halfRealWidth, -halfRealHeight, 0.0f));
    mReferenceCorners3D.push_back(
            cv::Point3f( halfRealWidth, -halfRealHeight, 0.0f));
    mReferenceCorners3D.push_back(
            cv::Point3f( halfRealWidth,  halfRealHeight, 0.0f));
    mReferenceCorners3D.push_back(
            cv::Point3f(-halfRealWidth,  halfRealHeight, 0.0f));

    // Create the feature detector, descriptor extractor, and
    // descriptor matcher.
    mFeatureDetectorAndDescriptorExtractor = cv::ORB::create();
    mDescriptorMatcher = cv::DescriptorMatcher::create(
            "BruteForce-HammingLUT");

    // Detect the reference features and compute their descriptors.
    mFeatureDetectorAndDescriptorExtractor->detect(referenceImageGray,
            mReferenceKeypoints);
    mFeatureDetectorAndDescriptorExtractor->compute(referenceImageGray,
            mReferenceKeypoints, mReferenceDescriptors);

    mCandidateSceneCorners.create(4, 1, CV_32FC2);

    // Assume no distortion.
    mDistCoeffs.zeros(4, 1, CV_64F);

    mTargetFound = false;
}

float *ImageDetectionFilter::getGLPose()
{
    return (mTargetFound ? mGLPose : NULL);
}

void ImageDetectionFilter::apply(cv::Mat &src, cv::Mat &dst,
        cv::Mat &projection)
{
    // Convert the scene to grayscale.
    cv::cvtColor(src, mGraySrc, cv::COLOR_RGBA2GRAY);

    // Detect the scene features, compute their descriptors,
    // and match the scene descriptors to reference descriptors.
    mFeatureDetectorAndDescriptorExtractor->detect(mGraySrc, mSceneKeypoints);
    mFeatureDetectorAndDescriptorExtractor->compute(mGraySrc, mSceneKeypoints,
            mSceneDescriptors);
    mDescriptorMatcher->match(mSceneDescriptors,
            mReferenceDescriptors, mMatches);

    // Attempt to find the target image's 3D pose in the scene.
    findPose(projection);

    // If the pose has not been found, draw a thumbnail of the
    // target image.
    draw(src, dst);
}

void ImageDetectionFilter::findPose(cv::Mat &projection)
{
    if (mMatches.size() < 4) {
        // There are too few matches to find the pose.
        return;
    }

    // Calculate the max and min distances between keypoints.
    float maxDist = 0.0f;
    float minDist = FLT_MAX;
    for (int i = 0; i < mMatches.size(); i++) {
        cv::DMatch match = mMatches[i];
        float dist = match.distance;
        if (dist < minDist) {
            minDist = dist;
        }
        if (dist > maxDist) {
            maxDist = dist;
        }
    }

    // The thresholds for minDist are chosen subjectively
    // based on testing. The unit is not related to pixel
    // distances; it is related to the number of failed tests
    // for similarity between the matched descriptors.
    if (minDist > 50.0) {
        // The target is completely lost.
        mTargetFound = false;
        return;
    } else if (minDist > 25.0) {
        // The target is lost but maybe it is still close.
        // Keep using any previously found pose.
        return;
    }

    // Identify "good" keypoints based on match distance.
    std::vector<cv::Point2f> goodReferencePoints;
    std::vector<cv::Point2f> goodScenePoints;
    double maxGoodMatchDist = 1.75 * minDist;
    for(int i = 0; i < mMatches.size(); i++) {
        cv::DMatch match = mMatches[i];
        if (match.distance < maxGoodMatchDist) {
            goodReferencePoints.push_back(
                    mReferenceKeypoints[match.trainIdx].pt);
            goodScenePoints.push_back(
                    mSceneKeypoints[match.queryIdx].pt);
        }
    }

    if (goodReferencePoints.size() < 4 ||
            goodScenePoints.size() < 4) {
        // There are too few good points to find the pose.
        return;
    }

    // There are enough good points to find the pose.
    // (Otherwise, the method would have already returned.)

    // Find the homography.
    cv::Mat homography = cv::findHomography(
            goodReferencePoints, goodScenePoints);

    // Use the homography to project the reference corner
    // coordinates into scene coordinates.
    cv::perspectiveTransform(mReferenceCorners,
            mCandidateSceneCorners, homography);

    // Check whether the corners form a convex polygon. If not,
    // (that is, if the corners form a concave polygon), the
    // detection result is invalid because no real perspective can
    // make the corners of a rectangular image look like a concave
    // polygon!
    if (!cv::isContourConvex(mCandidateSceneCorners)) {
        return;
    }

    // Find the target's Euler angles and XYZ coordinates.
    cv::solvePnP(mReferenceCorners3D, mCandidateSceneCorners,
            projection, mDistCoeffs, mRVec, mTVec);

    // Positive y is up in OpenGL, down in OpenCV.
    // Positive z is backward in OpenGL, forward in OpenCV.
    // Positive angles are counter-clockwise in OpenGL,
    // clockwise in OpenCV.
    // Thus, x angles are negated but y and z angles are
    // double-negated (that is, unchanged).
    // Meanwhile, y and z positions are negated.

    mRVec.at<double>(0, 0) *= -1.0; // negate x angle

    // Convert the Euler angles to a 3x3 rotation matrix.
    cv::Rodrigues(mRVec, mRotation);

    // OpenCV's matrix format is transposed, relative to
    // OpenGL's matrix format.
    mGLPose[0]  =  (float)mRotation.at<double>(0, 0);
    mGLPose[1]  =  (float)mRotation.at<double>(0, 1);
    mGLPose[2]  =  (float)mRotation.at<double>(0, 2);
    mGLPose[3]  =  0.0f;
    mGLPose[4]  =  (float)mRotation.at<double>(1, 0);
    mGLPose[5]  =  (float)mRotation.at<double>(1, 1);
    mGLPose[6]  =  (float)mRotation.at<double>(1, 2);
    mGLPose[7]  =  0.0f;
    mGLPose[8]  =  (float)mRotation.at<double>(2, 0);
    mGLPose[9]  =  (float)mRotation.at<double>(2, 1);
    mGLPose[10] =  (float)mRotation.at<double>(2, 2);
    mGLPose[11] =  0.0f;
    mGLPose[12] =  (float)mTVec.at<double>(0, 0);
    mGLPose[13] = -(float)mTVec.at<double>(1, 0); // negate y position
    mGLPose[14] = -(float)mTVec.at<double>(2, 0); // negate z position
    mGLPose[15] =  1.0f;

    mTargetFound = true;
}

void ImageDetectionFilter::draw(cv::Mat src, cv::Mat dst)
{
    if (&src != &dst) {
        src.copyTo(dst);
    }

    if (!mTargetFound) {
        // The target has not been found.

        // Draw a thumbnail of the target in the upper-left
        // corner so that the user knows what it is.

        // Compute the thumbnail's larger dimension as half the
        // video frame's smaller dimension.
        int height = mReferenceImage.rows;
        int width = mReferenceImage.cols;
        int maxDimension = MIN(dst.rows, dst.cols) / 2;
        double aspectRatio = width / (double)height;
        if (height > width) {
            height = maxDimension;
            width = (int)(height * aspectRatio);
        } else {
            width = maxDimension;
            height = (int)(width / aspectRatio);
        }

        // Select the region of interest (ROI) where the thumbnail
        // will be drawn.
        int offsetY = height - dst.rows;
        int offsetX = width - dst.cols;
        dst.adjustROI(0, offsetY, 0, offsetX);

        // Copy a resized reference image into the ROI.
        cv::resize(mReferenceImage, dst, dst.size(), 0.0, 0.0,
                cv::INTER_AREA);

        // Deselect the ROI.
        dst.adjustROI(0, -offsetY, 0, -offsetX);
    }
}
