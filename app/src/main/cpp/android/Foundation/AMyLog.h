//
// Created by luozheng on 2019/1/17.
//

#ifndef SUBSTRATEHOOKANDROID_MYLOG_H
#define SUBSTRATEHOOKANDROID_MYLOG_H

    #ifndef LOG_TAG
    #define LOG_TAG "QSSQ_DEBUG_LOG"
    #endif

    //#ifndef XXXXX
    //#if DEBUG
    //#if defined(DEBUG)
    #if defined(FUCKDEBUG)
        #pragma message("-----MY LOG_ ON----")
        #ifdef ANDROID
            #pragma message("-----ANDROID----")
            #include <android/log.h>
            #include <string.h>
            #include <dlfcn.h>
            #include <sys/stat.h>
            #define TF()  (__FILE__)//window是\\linux是/
            #define LOGD(fmt, ...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "[%s:%s]" fmt,TF(), __FUNCTION__,##__VA_ARGS__)
            #define LOGI(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "[%s:%s]" fmt,TF(),__FUNCTION__,##__VA_ARGS__)
            #define LOGV(fmt, ...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "[%s:%s]" fmt,TF(), __FUNCTION__,##__VA_ARGS__)
            #define LOGW(fmt, ...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "[%s:%s]" fmt,TF(), __FUNCTION__,##__VA_ARGS__)
            #define VLOGW(fmt, varlist) __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, "[%s:%s]" fmt,TF(), __FUNCTION__,##varlist_)
            #define LOGE(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[%s:%s]" fmt,TF(), __FUNCTION__,##__VA_ARGS__)
            #define LOGF(fmt, ...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, "[%s:%s]" fmt,TF(), __FUNCTION__,##__VA_ARGS__)
        #else
            #pragma message("-----NOT ANDROID----")
            #define LOGD(fmt,...) printf("[%s %s]" fmt, __FILE__,__FUNCTION__,##__VA_ARGS__)
            #define LOGI(fmt,...) printf("[%s %s]" fmt,__FILE__, __FUNCTION__,##__VA_ARGS__)
            #define LOGV(fmt,...) printf("[%s %s]" fmt,__FILE__, __FUNCTION__,##__VA_ARGS__)
            #define LOGW(fmt,...) printf("[%s %s]" fmt, __FILE__,__FUNCTION__,##__VA_ARGS__)
            #define LOGE(fmt,...) printf("[%s %s]" fmt,__FILE__, __FUNCTION__,##__VA_ARGS__)
            #define LOGF(fmt,...) printf("[%s %s]" fmt, __FILE__,__FUNCTION__,##__VA_ARGS__)
        #endif//android elseif


    #else //ELSE ANDROID LOG
    #define TF() ()
    #pragma message("-----NOT DEBUG_AMYLOG----")
    #define LOGD(fmt, ...) if(true){}
    #define LOGI(fmt, ...) if(true){}
    #define LOGV(fmt, ...) if(true){}
    #define LOGW(fmt, ...) if(true){}
    #define LOGE(fmt, ...) if(true){}
    #define LOGF(fmt, ...) if(true){}
    #endif //FUCKDEBUG END LOG

#endif //SUBSTRATEHOOKANDROID_MYLOG_H