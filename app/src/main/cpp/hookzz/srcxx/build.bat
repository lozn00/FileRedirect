set ANDROID_NDK=%ANDROID_SDK_HOME%
%ANDROID_NDK%\cmake\3.6.4111459\bin\cmake.exe .. -DCMAKE_TOOLCHAIN_FILE=%ANDROID_NDK%\cmake\3.10.2.4988404/android.toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DANDROID_ABI="armeabi-v7a" -DANDROID_STL=c++_static -DANDROID_NATIVE_API_LEVEL=android-14 -DDEBUG=OFF -DSHARED=ON
%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe -j4