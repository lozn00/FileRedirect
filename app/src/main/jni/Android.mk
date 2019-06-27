LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := prncon
# -DFUCKDEBUG open log
LOCAL_CFLAGS := -Wno-error=format-security -fpermissive -DLOG_TAG=\"FUCK_QSSQ\" -DQSSQ_SAFE
LOCAL_CFLAGS += -fno-rtti -fno-exceptions
LOCAL_CPPFLAGS += -std=c++14
LOCAL_CFLAGS += -fvisibility=hidden # hidden
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)/Foundation
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)/Jni
LOCAL_C_INCLUDES += $(MAIN_LOCAL_PATH)/Include

LOCAL_SRC_FILES := Jni/VAJni.cpp \
				   Foundation/IOUniformer.cpp \
				   Foundation/VMPatch.cpp \
				   Foundation/SymbolFinder.cpp \
				   Foundation/Path.cpp \
				   Foundation/SandboxFs.cpp \
				   Substrate/hde64.c \
                   Substrate/SubstrateDebug.cpp \
                   Substrate/SubstrateHook.cpp \
                   Substrate/SubstratePosixMemory.cpp \
                    My/JniHelper.cpp \
                    My/QSSQ_hkkq.cpp \
                    My/base64.c \
                    My/base6_with_test.c \
                    My/bytesencrypt.cpp \

LOCAL_LDLIBS := -llog -latomic
LOCAL_STATIC_LIBRARIES := fb

#https://blog.csdn.net/zhang_jun_ling/article/details/79109473
include $(BUILD_SHARED_LIBRARY)
include $(MAIN_LOCAL_PATH)/fb/Android.mk