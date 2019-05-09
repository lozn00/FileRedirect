#include <elf.h>//
// VirtualApp Native Project
//
#include <android/Foundation/IOUniformer.h>
#include "VAJni.h"

/*
static void jni_nativeLaunchEngine(jclass clazz, JArrayClass <jobject> javaMethods,
                                   jstring packageName,
                                   jboolean isArt, jint apiLevel, jint cameraMethodType) {
    hookAndroidVM(javaMethods, packageName, isArt, apiLevel, cameraMethodType);
}*/


static void jni_nativeEnableIORedirect(jclass jclass1, jstring selfSoPath, jint apiLevel,
                                       jint preview_api_level) {
    ScopeUtfString so_path(selfSoPath);
    IOUniformer::startUniformer(so_path.c_str(), apiLevel, preview_api_level);
}

static void jni_nativeIOWhitelist(jclass jclazz, jstring _path) {
    ScopeUtfString path(_path);
    IOUniformer::whitelist(path.c_str());
}

static void jni_nativeIOForbid(jclass jclazz, jstring _path) {
    ScopeUtfString path(_path);
    IOUniformer::forbid(path.c_str());
}


static void jni_nativeIORedirect(jclass jclazz, jstring origPath, jstring newPath) {
    ScopeUtfString orig_path(origPath);
    ScopeUtfString new_path(newPath);
    IOUniformer::redirect(orig_path.c_str(), new_path.c_str());

}

static jstring jni_nativeGetRedirectedPath(JNIEnv *env, jclass jclazz, jstring origPath) {
    ScopeUtfString orig_path(origPath);
    const char *redirected_path = IOUniformer::query(orig_path.c_str());
    if (redirected_path != NULL) {
        return env->NewStringUTF(redirected_path);
    }
    return NULL;
}

static jstring jni_nativeReverseRedirectedPath(JNIEnv *env, jclass jclazz, jstring redirectedPath) {
    ScopeUtfString redirected_path(redirectedPath);
    const char *orig_path = IOUniformer::reverse(redirected_path.c_str());
    return env->NewStringUTF(orig_path);
}


