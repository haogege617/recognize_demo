LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := md5

LOCAL_SRC_FILES := md5.cpp

include $(BUILD_STATIC_LIBRARY)