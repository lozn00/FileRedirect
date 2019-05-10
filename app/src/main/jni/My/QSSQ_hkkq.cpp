#include<jni.h>
#include "Helper.h"

#include<stdio.h>
#include<iostream>
#include<string.h>

//#include"QSSQ_Constant.cpp"
#include <utility>
#include "QSSQ_CheckEnvImpl.cpp"
#include <android/log.h>
#include <unistd.h>
#include "IOUniformer.h"
#include "JniHelper.h"
#include "AMyLog.h"

//#include<Windows.h>
//#include<>
int main() {
    //cout<<"333"<<endl
    return 0;
}

// yptUtilN"//指定要注册的类

static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods,
                                 int numMethods) {
    jclass clazz;
    //LOGI("registerNativeMethods  count %d ", numMethods);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        //LOGI("注册失败,找不到类");
        return JNI_FALSE;
    } else {
        //LOGI("成功找到了类!");
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {

        //LOGI("注册失败,总数小雨00");
        return JNI_FALSE;
    }
    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        //LOGI("出现异常，执行清理");
        return JNI_TRUE;
    }
    //LOGI("注册成功准备返回");
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
callParentVoidParamMethod(JNIEnv *env, jobject thiz, jobject currentObj, jstring methodName,
                          jstring parentClassName) {


    const char *methodSignChars = QSSQ_Constant::jstringTochars(env, methodName);
    const char *classNameChars = QSSQ_Constant::jstringTochars(env, parentClassName);

    //   printf("签名 %s 父类名 %s", methodSignChars, classNameChars);
    jclass fclass = (jclass) QSSQ_Constant::classForNameX(env, thiz, classNameChars,
                                                          QSSQ_Constant::getClassLoaderFromObject(
                                                                  env,
                                                                  currentObj)); //env->FindClass(methodSignChars);
    if (fclass == NULL) {

        //MessageBoxA(0, "父类为空!", "提示", 0);
        //        printf("父类方法为空!");
        return;

    }
    jmethodID jmethodID = env->GetMethodID(fclass, methodSignChars, "()V");
    if (jmethodID == NULL) {
        //MessageBoxA(0, "找不到方法!", "提示", 0);
        printf("找不到方法");
        return;
    }
    /* QSSQ_CheckEnvImpl impl;
     impl.hKillerLZ(env);
     impl.checkMemoryLZ(env);*/
    env->CallNonvirtualVoidMethod(currentObj, fclass, jmethodID);
}

extern "C" JNIEXPORT void JNICALL aaaa(JNIEnv *env, jobject thiz) {
    QSSQ_CheckEnvImpl impl;
//  impl.hKillerLZ(env);

//   impl.checkSignLZ(env);

}

#define JNI_START JNIEnv *env, jclass cl
//=========================================
extern "C" __attribute__((visibility("default"))) void fixAndroidO(JNI_START) {

    QSSQ_CheckEnvImpl impl;
    impl.hKillerLZ(env);
    impl.checkMemoryLZ(env, 694886);
}

static void
nativeEnableIORedirect(JNIEnv *env, jclass jclass1, jstring selfSoPath, jint apiLevel,
                       jint preview_api_level) {
    ScopeUtfString so_path( selfSoPath);
    IOUniformer::startUniformer(so_path.c_str(), apiLevel, preview_api_level);
    QSSQ_CheckEnvImpl impl;
    impl.hKillerLZ(env);
    impl.checkMemoryLZ(env, 694886);


}

static void nativeIOWhitelist(JNIEnv *env, jclass jclazz, jstring _path) {
    ScopeUtfString path(_path);
    IOUniformer::whitelist(path.c_str());
}

static void nativeIOForbid(JNIEnv *env, jclass jclazz, jstring _path) {
    ScopeUtfString path(_path);
    IOUniformer::forbid(path.c_str());
}


static void
nativeIORedirect(JNIEnv *env, jclass jclazz, jstring origPath, jstring newPath) {
    ScopeUtfString orig_path(origPath);
    ScopeUtfString new_path(newPath);
    IOUniformer::redirect(orig_path.c_str(), new_path.c_str());

}

static jstring nativeGetRedirectedPath(JNIEnv *env, jclass jclazz, jstring origPath) {
    ScopeUtfString orig_path( origPath);
    const char *redirected_path = IOUniformer::query(orig_path.c_str());
    if (redirected_path != NULL) {
        return env->NewStringUTF(redirected_path);
    }
    return NULL;
}

static jstring
nativeReverseRedirectedPath(JNIEnv *env, jclass jclazz, jstring redirectedPath) {
    ScopeUtfString redirected_path( redirectedPath);
    const char *orig_path = IOUniformer::reverse(redirected_path.c_str());

    return env->NewStringUTF(orig_path);
}


#define ENCODECLASS "cn/qssq666/QSSQHook"//指定要注册的

//#define ENCODECLASS "android/support/v4/app/gnehzoul"//指定要注册的
__attribute__((section (".text")))
static int registerNatives(JNIEnv *env) {
    JNINativeMethod gMethods[] = {
            {"bwn6a6b",                     "()V",                                     (void *) aaaa},
            {"nativeEnableIORedirect",      "(Ljava/lang/String;II)V",                 (void *) nativeEnableIORedirect},
            {"nativeIOWhitelist",           "(Ljava/lang/String;)V",                   (void *) nativeIOWhitelist},
            {"nativeIOForbid",              "(Ljava/lang/String;)V",                   (void *) nativeIOForbid},
            {"nativeIORedirect",            "(Ljava/lang/String;Ljava/lang/String;)V", (void *) nativeIORedirect},
            {"nativeGetRedirectedPath",     "(Ljava/lang/String;)Ljava/lang/String;",  (void *) nativeGetRedirectedPath,},
            {"fixAndroidO",                 "()V",                                     (void *) fixAndroidO,},
            {"nativeReverseRedirectedPath", "(Ljava/lang/String;)Ljava/lang/String;",  (void *) nativeReverseRedirectedPath}
    };
//    JNINativeMethod gMethods[] = {currentMethod};
    //FFF define ENCODECLASS "cn/qssq666/EncryptUtilN"//指定要注册的类
    //android.support.v4.app.gnehzoul;
    return registerNativeMethods(env, ENCODECLASS, gMethods,
                                 sizeof(gMethods) / sizeof(gMethods[0]));
}

//重写
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    LOGW_("JNI-LOAD");
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    JniHelper::setJavaVM(vm);
    assert(env != NULL);
    if (!registerNatives(env)) {//注册


        LOGW_("注册失败！");
        return JNI_ERR;
    }

    QSSQ_CheckEnvImpl impl;
    impl.hKillerLZ(env);
    impl.checkMemoryLZ(env, 64);

//    std::utility
    LOGW_("JNI-LOAD-------over");
    return JNI_VERSION_1_6;


}


extern "C" __attribute__((constructor)) void _init(void) {
    LOGW("init_env_before_all");
    IOUniformer::init_env_before_all();
}

/*

通过 new int[]指定具体数组 获取修改参数加上const 修复字符串的问题，而重复定义则通过把所有  全局改成单文件可访问的，避免被包含后导致冲突。重复

*/