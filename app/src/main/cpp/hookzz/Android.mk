
define walk
$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef


$(warning "projectdir ${NDK_PROJECT_PATH}")#通常是 绝对路径 项目名称/app/src/main/jni
LOCAL_PATH:= $(call my-dir)
#构建静态
#include $(CLEAR_VARS)
#LOCAL_MODULE    = hookzz
#LOCAL_SRC_FILES:=$(LOCAL_PATH)/static/libhookzz.a
#include $(PREBUILT_STATIC_LIBRARY)



include $(CLEAR_VARS) #清空之后 LOCAL_MODULE得重新弄一个.


LOCAL_MODULE    = native-lib
#LOCAL_LDFLAGS += -shared

#添加模块使用宏定义
LOCAL_CFLAGS += -DMODULE_FLAG

#添加需要包含的头文件路径，会依次遍历向下所include有目录，
LOCAL_C_INCLUDES+= $(call walk, ${LOCAL_PATH}/include)
LOCAL_C_INCLUDES+= $(call walk, ${LOCAL_PATH}/srcxx)

$(warning " c include head files =$(LOCAL_C_INCLUDES)")



LOCAL_MODULE    := native-lib


# 扫描目录下的所有源文件，会向下依次遍历

MY_FILES_PATH  := $(LOCAL_PATH)
# 添加指定C/CPP文件，只添加某个
$(warning "local_path $(LOCAL_PATH)")

MY_FILES_SUFFIX := %.cpp %.c %.cc
# My_All_Files := $(foreach src_path,$(MY_FILES_PATH), $(shell find "$(src_path)" -type f) )只适合linux find -type f



My_All_Files := $(call walk, $(LOCAL_PATH))

My_All_Files := $(My_All_Files:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(My_All_Files))#过滤 只保留.c
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%) #替换路径

LOCAL_SRC_FILES += $(MY_SRC_LIST)
$(warning "all file:$(LOCAL_SRC_FILES)")
#添加需要链接的静态库
#LOCAL_STATIC_LIBRARIES := hookzz #hookzz是上面构建的
$(warning "static libs=$(LOCAL_STATIC_LIBRARIES)")
#添加需要链接的动态库
LOCAL_SHARED_LIBRARIES  :=
$(warning "$(LOCAL_MODULE): LOCAL_SHARED_LIBRARIES=$(LOCAL_SHARED_LIBRARIES)")
#添加需要链接的系统库，如ndk编译，需要链接的log/android等
LOCAL_LDLIBS    += -llog -landroid -lc
#指定编译目标，这边编译动态库
#include $(BUILD_STATIC_LIBRARY)
#include $(BUILD_EXECUTABLE)#解决不了错误
include $(BUILD_SHARED_LIBRARY)
#APP_ALLOW_MISSING_DEPS=true

#BUILD_STATIC_LIBRARY：编译为静态库。
#BUILD_SHARED_LIBRARY ：编译为动态库
#BUILD_EXECUTABLE：编译为Native C可执行程序
LOCAL_LDLIBS:=-L$(SYSROOT)/usr/lib -llog



# :=和 =是一样的用法， 是类似普通的变量赋值按顺序流程 = 是有一点区别某个变量用:=获取其他变量的时候， 会有区别=的区别是以最后整个文件走完之后的变量为准。








