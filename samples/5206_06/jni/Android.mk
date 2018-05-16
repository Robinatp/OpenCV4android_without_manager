LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifdef OPENCV_ANDROID_SDK
  ifneq ("","$(wildcard $(OPENCV_ANDROID_SDK)/OpenCV.mk)")
    include ${OPENCV_ANDROID_SDK}/OpenCV.mk
  else
    include ${OPENCV_ANDROID_SDK}/sdk/native/jni/OpenCV.mk
  endif
else
   include ../../sdk/native/jni/OpenCV.mk
endif

LOCAL_SRC_FILES := SecondSightJNI.cpp
LOCAL_SRC_FILES += RecolorCMVFilter.cpp
LOCAL_SRC_FILES += RecolorRCFilter.cpp
LOCAL_SRC_FILES += RecolorRGVFilter.cpp
LOCAL_SRC_FILES += StrokeEdgesFilter.cpp
LOCAL_SRC_FILES += ImageDetectionFilter.cpp
LOCAL_C_INCLUDES += $(LOCAL_PATH)

LOCAL_LDLIBS += -llog -ldl 

LOCAL_MODULE := SecondSight
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_MODULE := libopencv_java3
LOCAL_SRC_FILES := prebuilt/arm64-v8a/libopencv_java3.so
include $(PREBUILT_SHARED_LIBRARY)