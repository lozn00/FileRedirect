APP_ABI := armeabi-v7a x86
#APP_ABI := armeabi-v7a x86 arm64-v8a
APP_PLATFORM := android-14
APP_STL := c++_static
APP_OPTIM := release
VA_ROOT          := $(call my-dir)
NDK_MODULE_PATH  := $(NDK_MODULE_PATH):$(VA_ROOT)