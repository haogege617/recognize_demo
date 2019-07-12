/*
 * dylog.h
 *
 *  Created on: 2016??8??10??
 *      Author: Yongpan Song
 */

#ifndef UTILS_DYUTILS_H_
#define UTILS_DYUTILS_H_

#include <string.h>
#include <android/log.h>

#define TAG "ALLTAG"

#define LOGD(format, args...) do { \
  __android_log_print(ANDROID_LOG_DEBUG, TAG, "[D][%.20s:%d]:" format "\n", __FILE__+strlen(__FILE__)-20, __LINE__, ##args); \
} while (0)

#define LOGI(format, args...) do { \
  __android_log_print(ANDROID_LOG_INFO, TAG, "[I][%.20s:%d]:" format "\n", __FILE__+strlen(__FILE__)-20, __LINE__, ##args); \
} while (0)

#define LOGW(format, args...) do { \
  __android_log_print(ANDROID_LOG_WARNNING, TAG, "[W][%.20s:%d]:" format "\n", __FILE__+strlen(__FILE__)-20, __LINE__, ##args); \
} while (0)

#define LOGE(format, args...) do { \
  __android_log_print(ANDROID_LOG_ERROR, TAG, "[E][%.20s:%d]:" format "\n", __FILE__+strlen(__FILE__)-20, __LINE__, ##args); \
} while (0)

#define LOGF(format, args...) do { \
  __android_log_print(ANDROID_LOG_FATAL, TAG, "[F][%.20s:%d]:" format "\n", __FILE__+strlen(__FILE__)-20, __LINE__, ##args); \
} while (0)

unsigned long getTimestamp();

#endif /* UTILS_DYUTILS_H_ */
