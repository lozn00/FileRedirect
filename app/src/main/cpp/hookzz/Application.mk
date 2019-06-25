APP_ABI := armeabi-v7a #奇怪设置无效了
APP_PIE:= true
 APP_PLATFORM:=android-14
#下面代码解决 Android mk   fatal error: 'string' file not found
#APP_STL := stlport_static
#APP_STL := gnustl_static
#APP_STL := libstdc++
#gnu 不需要c++ dll  但是找不到android no matching function for call to 'transform'
APP_STL := c++_shared
APP_ABI := armeabi-v7a
#fatal error: 'asm/hwcap.h' file not found

