LOCAL_PATH := $(call my-dir)

ROOT_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := jniwrapper/com_duoyi_dyhttp_DuoyiHttpApi.cpp

LOCAL_MODULE := dyhttpsdk

LOCAL_STATIC_LIBRARIES := curl ssl crypto dyutils md5

LOCAL_C_INCLUDES := $(LOCAL_PATH)/network/curl/include

LOCAL_LDLIBS := -llog -lz -landroid  -fPIC -latomic

include $(BUILD_SHARED_LIBRARY)

include $(ROOT_PATH)/network/curl/Android.mk
include $(ROOT_PATH)/network/openssl/Android.mk
include $(ROOT_PATH)/utils/Android.mk
include $(ROOT_PATH)/safety/md5/Android.mk