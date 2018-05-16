#include <jni.h>

#include "RecolorCMVFilter.hpp"
#include "RecolorRCFilter.hpp"
#include "RecolorRGVFilter.hpp"
#include "StrokeEdgesFilter.hpp"
#include "ImageDetectionFilter.hpp"

using namespace secondsight;

extern "C" {

JNIEXPORT jlong JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorCMVFilter_newSelf(
        JNIEnv *env, jclass clazz)
{
    RecolorCMVFilter *self = new RecolorCMVFilter();
    return (jlong)self;
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorCMVFilter_deleteSelf(
        JNIEnv *env, jclass clazz, jlong selfAddr)
{
    if (selfAddr != 0)
    {
        RecolorCMVFilter *self = (RecolorCMVFilter *)selfAddr;
        delete self;
    }
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorCMVFilter_apply(
        JNIEnv *env, jclass clazz, jlong selfAddr, jlong srcAddr,
        jlong dstAddr)
{
    if (selfAddr != 0)
    {
        RecolorCMVFilter *self = (RecolorCMVFilter *)selfAddr;
        cv::Mat &src = *(cv::Mat *)srcAddr;
        cv::Mat &dst = *(cv::Mat *)dstAddr;
        self->apply(src, dst);
    }
}

JNIEXPORT jlong JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorRCFilter_newSelf(
        JNIEnv *env, jclass clazz)
{
    RecolorRCFilter *self = new RecolorRCFilter();
    return (jlong)self;
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorRCFilter_deleteSelf(
        JNIEnv *env, jclass clazz, jlong selfAddr)
{
    if (selfAddr != 0)
    {
        RecolorRCFilter *self = (RecolorRCFilter *)selfAddr;
        delete self;
    }
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorRCFilter_apply(
        JNIEnv *env, jclass clazz, jlong selfAddr, jlong srcAddr,
        jlong dstAddr)
{
    if (selfAddr != 0)
    {
        RecolorRCFilter *self = (RecolorRCFilter *)selfAddr;
        cv::Mat &src = *(cv::Mat *)srcAddr;
        cv::Mat &dst = *(cv::Mat *)dstAddr;
        self->apply(src, dst);
    }
}

JNIEXPORT jlong JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorRGVFilter_newSelf(
        JNIEnv *env, jclass clazz)
{
    RecolorRGVFilter *self = new RecolorRGVFilter();
    return (jlong)self;
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorRGVFilter_deleteSelf(
        JNIEnv *env, jclass clazz, jlong selfAddr)
{
    if (selfAddr != 0)
    {
        RecolorRGVFilter *self = (RecolorRGVFilter *)selfAddr;
        delete self;
    }
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_mixer_RecolorRGVFilter_apply(
        JNIEnv *env, jclass clazz, jlong selfAddr, jlong srcAddr,
        jlong dstAddr)
{
    if (selfAddr != 0)
    {
        RecolorRGVFilter *self = (RecolorRGVFilter *)selfAddr;
        cv::Mat &src = *(cv::Mat *)srcAddr;
        cv::Mat &dst = *(cv::Mat *)dstAddr;
        self->apply(src, dst);
    }
}

JNIEXPORT jlong JNICALL
Java_com_nummist_secondsight_filters_convolution_StrokeEdgesFilter_newSelf(
        JNIEnv *env, jclass clazz)
{
    StrokeEdgesFilter *self = new StrokeEdgesFilter();
    return (jlong)self;
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_convolution_StrokeEdgesFilter_deleteSelf(
        JNIEnv *env, jclass clazz, jlong selfAddr)
{
    if (selfAddr != 0)
    {
        StrokeEdgesFilter *self = (StrokeEdgesFilter *)selfAddr;
        delete self;
    }
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_convolution_StrokeEdgesFilter_apply(
        JNIEnv *env, jclass clazz, jlong selfAddr, jlong srcAddr,
        jlong dstAddr)
{
    if (selfAddr != 0)
    {
        StrokeEdgesFilter *self = (StrokeEdgesFilter *)selfAddr;
        cv::Mat &src = *(cv::Mat *)srcAddr;
        cv::Mat &dst = *(cv::Mat *)dstAddr;
        self->apply(src, dst);
    }
}

JNIEXPORT jlong JNICALL
Java_com_nummist_secondsight_filters_ar_ImageDetectionFilter_newSelf(
        JNIEnv *env, jclass clazz, jlong referenceImageBGRAddr,
        jdouble realSize)
{
    cv::Mat &referenceImageBGR =  *(cv::Mat *)referenceImageBGRAddr;
    ImageDetectionFilter *self = new ImageDetectionFilter(
            referenceImageBGR, realSize);
    return (jlong)self;
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_ar_ImageDetectionFilter_deleteSelf(
        JNIEnv *env, jclass clazz, jlong selfAddr)
{
    if (selfAddr != 0)
    {
        ImageDetectionFilter *self = (ImageDetectionFilter *)selfAddr;
        delete self;
    }
}

JNIEXPORT jfloatArray JNICALL
Java_com_nummist_secondsight_filters_ar_ImageDetectionFilter_getGLPose(
        JNIEnv *env, jclass clazz, jlong selfAddr)
{
    if (selfAddr == 0)
    {
        return NULL;
    }

    ImageDetectionFilter *self = (ImageDetectionFilter *)selfAddr;
    float *glPoseNative = self->getGLPose();
    if (glPoseNative == NULL)
    {
        return NULL;
    }

    jfloatArray glPoseJava = env->NewFloatArray(16);
    if (glPoseJava != NULL)
    {
        env->SetFloatArrayRegion(glPoseJava, 0, 16, glPoseNative);
    }
    return glPoseJava;
}

JNIEXPORT void JNICALL
Java_com_nummist_secondsight_filters_ar_ImageDetectionFilter_apply(
        JNIEnv *env, jclass clazz, jlong selfAddr, jlong srcAddr,
        jlong dstAddr, jlong projectionAddr)
{
    if (selfAddr != 0)
    {
        ImageDetectionFilter *self = (ImageDetectionFilter *)selfAddr;
        cv::Mat &src = *(cv::Mat *)srcAddr;
        cv::Mat &dst = *(cv::Mat *)dstAddr;
        cv::Mat &projection = *(cv::Mat *)projectionAddr;
        self->apply(src, dst, projection);
    }
}

} // extern "C"
