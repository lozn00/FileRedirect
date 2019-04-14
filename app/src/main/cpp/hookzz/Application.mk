APP_ABI := armeabi-v7a #奇怪设置无效了
APP_PIE:= true
 APP_PLATFORM:=android-14
#下面代码解决 Android mk   fatal error: 'string' file not found
#APP_STL := stlport_static
APP_STL := c++_shared
APP_ABI := armeabi-v7a
#fatal error: 'asm/hwcap.h' file not found

