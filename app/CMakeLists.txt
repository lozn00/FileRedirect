
cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_CXX_STANDARD 14)

#include_directories(src/main/cpp/Foundation)
include_directories(src/main/cpp/include)
include_directories(src/main/cpp/include_io_redirect)
include_directories(src/main/cpp)
add_subdirectory("src/main/cpp/hookzz")


#[[message("compile  static lib ${PROJECT_SOURCE_DIR}")
add_library(hookzzstatic STATIC  IMPORTED)
set_target_properties(hookzzstatic
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/libhookzz.a)]]
message("------------------------------------------------")

add_library( # Sets the name of the library.
        qssqredirect
        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        src/main/cpp/android/Foundation/Environment.cpp
        src/main/cpp/android/Foundation/IOUniformer.cpp
        src/main/cpp/android/Foundation/Path.cpp
        src/main/cpp/android/Foundation/SandboxFs.cpp
        src/main/cpp/android/Foundation/SymbolFinder.cpp
        src/main/cpp/android/Foundation/JniHelper.cpp
        src/main/cpp/QSSQ_hkkq.cpp
        )

find_library( # Sets the name of the path variable.
        log-lib
        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        qssqredirect
        hookzz
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})


add_definitions(-std=c++11)
ENABLE_LANGUAGE(ASM)