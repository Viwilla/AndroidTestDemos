#ifndef JNI_COMMON_H
#define JNI_COMMON_H

#include <android/log.h>

#define DEBUG 1
#ifndef LOG_TAG
#define LOG_TAG "Hello"
#endif

#if DEBUG
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__)
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG ,__VA_ARGS__)
#else
#define LOGD(...) while(0){}
#define LOGI(...) while(0){}
#define LOGV(...) while(0){}
#define LOGW(...) while(0){}
#define LOGE(...) while(0){}
#define LOGW(...) while(0){}
#endif

#ifndef __cplusplus
#define bool unsigned char
#define false 0
#define true 1
#endif

#endif
