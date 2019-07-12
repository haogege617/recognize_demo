LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := dyutils

LOCAL_SRC_FILES := dyutils.cpp

include $(BUILD_STATIC_LIBRARY)