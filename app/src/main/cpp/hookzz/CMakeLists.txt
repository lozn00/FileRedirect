cmake_minimum_required(VERSION 3.5)
project(HookZz)

include(cmake/Util.cmake)
include(cmake/Macros.cmake)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
enable_language(ASM)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -stdlib=libc++ -fvisibility=hidden")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")
SET(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}")
SET(CMAKE_ASM_FLAGS "${CFLAGS}  -x assembler-with-cpp")
    MESSAGE(STATUS "XXXXXXXXXXXXXXXXXXXXXXXX")
option(SHARED "Build shared library" ON)
option(DEBUG "Enable debug log" ON)
option(CLOSURE_BRIDGE_TEMPLATE "Enable closure bridge assembly template" OFF)

if(CLOSURE_BRIDGE_TEMPLATE)
    add_definitions(-DENABLE_CLOSURE_BRIDGE_TEMPLATE)
endif()

if(DEBUG)
   add_definitions(-DDEBUG)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# Platform
if(PLATFORM STREQUAL "iOS")
    set(SYSTEM.iOS 1)
    MESSAGE(STATUS "[*] Compile for iOS")
elseif(PLATFORM STREQUAL "Android")
    set(SYSTEM.Android 1)
    MESSAGE(STATUS "[*] Compile for Android")
else()
    if(SYSTEM.Android OR SYSTEM.Linux)
        set(SYSTEM.Android 1)
    elseif(SYSTEM.Darwin)
        set(SYSTEM.iOS 1)
    else()
        message(FATAL_ERROR "[!] ONLY SUPPORT [iOS|Android] PLATFORM")
    endif()
endif()

if(SYSTEM.iOS)
    # -lstdc++
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -stdlib=libc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif(SYSTEM.Android)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -Wl,--gc-sections")
endif()

# Architecture
if(ARCH STREQUAL "arm" OR ARCH STREQUAL "armv7")
    set(CMAKE_SYSTEM_PROCESSOR arm)
    set(PROCESSOR.arm 1)
    MESSAGE(STATUS "[*] Compile for arm")
elseif(ARCH STREQUAL "aarch64" OR ARCH STREQUAL "arm64" OR ARCH STREQUAL "armv8")
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    set(PROCESSOR.aarch64 1)
    MESSAGE(STATUS "[*] Compile for aarch64")
else()
    if(PROCESSOR.arm)
        set(CMAKE_SYSTEM_PROCESSOR arm)
        MESSAGE(STATUS "[*] Compile for arm")
    elseif(PROCESSOR.aarch64)
        set(CMAKE_SYSTEM_PROCESSOR aarch64)
        MESSAGE(STATUS "[*] Compile for aarch64")
    else()
        message(FATAL_ERROR "[!] ONLY SUPPORT [[arm|armv7]|[aarch64|arm64|armv8]] ARCH")
    endif()
endif()

if(PROCESSOR.arm)
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch armv7")
elseif(PROCESSOR.aarch64)
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch arm64")
endif()

set(Target.path .)
if(PROCESSOR.arm)
    set(Target.arch arm)
    set(Target.cpp_prefix ARM)
    set(Target.cc_suffix arm)
elseif(PROCESSOR.aarch64)
    set(Target.arch arm64)
    set(Target.cpp_prefix ARM64)
    set(Target.cc_suffix arm64)
endif()

set(Target.SOURCE_DIR
    ${Target.path}
    ${Target.path}/srcxx
    ${Target.path}/srcxx/arch
)

# vm_core
set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
    ${Target.path}/srcxx/vm_core
    ${Target.path}/srcxx/vm_core/arch
    ${Target.path}/srcxx/vm_core/base
    ${Target.path}/srcxx/vm_core/objects
)

# vm_core_extra
set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
    ${Target.path}/srcxx/vm_core_extra
)

if(PROCESSOR.arm)
set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
    ${Target.path}/srcxx/arch/arm
    ${Target.path}/srcxx/vm_core/arch/arm
)
elseif(PROCESSOR.aarch64)
set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
    ${Target.path}/srcxx/arch/arm64
    ${Target.path}/srcxx/vm_core/arch/arm64
)
endif()
# =====

# *.cc
search_suffix_files("cc" Target.SOURCE_DIR Target.SOURCE_CC)

# *.cpp
search_suffix_files("cpp" Target.SOURCE_DIR Target.SOURCE_CPP)

# *.c
search_suffix_files("c" Target.SOURCE_DIR Target.SOURCE_C)

# *.S
search_suffix_files("S" Target.SOURCE_DIR Target.SOURCE_ASSEMBLY)

# *.h
search_suffix_files("h" Target.SOURCE_DIR Target.HEADER_H)

set(Target.SOURCE_FILE ${Target.SOURCE_FILE} ${Target.SOURCE_CC} ${Target.SOURCE_CPP} ${Target.SOURCE_C} ${Target.SOURCE_ASSEMBLY})
set(Target.HEADER_FILE ${Target.HEADER_FILE} ${Target.HEADER_H} include/hookzz.h)

# =====
# vm_core modules
# vm_core with specific architecture
set(MODULES.path ${Target.path}/srcxx/vm_core/modules)
set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
    ${MODULES.path}
    ${MODULES.path}/assembler
    ${MODULES.path}/codegen
)
# modules/assembler
set(ASSEMBLER.HEADER_FILE ${ASSEMBLER.HEADER_FILE}
    ${MODULES.path}/assembler/assembler.h
)
# modules/codegen
set(CODEGEN.HEADER_FILE ${ASSEMBLER.HEADER_FILE}
    ${MODULES.path}/codegen/codegen.h
)

# assembler
set(ASSEMBLER.SOURCE_FILE ${ASSEMBLER.SOURCE_FILE}
    ${MODULES.path}/assembler/assembler-${Target.cc_suffix}.cc
)
set(ASSEMBLER.HEADER_FILE ${ASSEMBLER.HEADER_FILE}
    ${MODULES.path}/assembler/assembler-${Target.cc_suffix}.h
)
# codegen
set(CODEGEN.SOURCE_FILE ${CODEGEN.SOURCE_FILE}
    ${MODULES.path}/codegen/codegen-${Target.cc_suffix}.cc
)
set(CODEGEN.HEADER_FILE ${CODEGEN.HEADER_FILE}
    ${MODULES.path}/codegen/codegen-${Target.cc_suffix}.h
)

set(Target.SOURCE_FILE ${Target.SOURCE_FILE}
    ${ASSEMBLER.SOURCE_FILE}
    ${CODEGEN.SOURCE_FILE}
)

set(Target.HEADER_FILE ${Target.HEADER_FILE}
    ${ASSEMBLER.HEADER_FILE}
    ${CODEGEN.HEADER_FILE}
)

# =====

# vm_core platforms
# vm_core with specific platform
set(PLATFORM.path ${Target.path}/srcxx/vm_core/platform)
set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
    ${PLATFORM.path}
)
set(Target.HEADER_FILE ${Target.HEADER_FILE}
    ${PLATFORM.path}/platform.h
)
if(SYSTEM.iOS)
    set(Target.SOURCE_FILE ${Target.SOURCE_FILE}
        ${PLATFORM.path}/platform-posix.cc
        ${PLATFORM.path}/platform-darwin.cc
    )
    set(Target.SOURCE_DIR ${Target.SOURCE_DIR}
        ${PLATFORM.path}/platform-darwin
    )
elseif(SYSTEM.Android)
    set(Target.SOURCE_FILE ${Target.SOURCE_FILE}
        ${PLATFORM.path}/platform-posix.cc
        ${PLATFORM.path}/platform-linux.cc
    )
endif()

# =====

# message(STATUS "Target.HEADER_FILE = ${Target.HEADER_FILE}\n")

include_directories(${Target.HEADER_DIR} ${Target.SOURCE_DIR})

if(SHARED)
    MESSAGE("share library")
    # build shared library
    add_library(hookzz SHARED ${Target.SOURCE_FILE} ${Target.HEADER_FILE} include/hookzz.h)
else()
    MESSAGE("static library")
    # build static library
    add_library(hookzz STATIC ${Target.SOURCE_FILE} ${Target.HEADER_FILE} include/hookzz.h)
endif()

target_include_directories(hookzz PUBLIC ./include)

if(CMAKE_SYSTEM_NAME MATCHES "^Android")
    target_link_libraries(hookzz log)
endif()


#[[

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)       #设置可执行文件的输出目录
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)           #设置库文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
message("dir ${PROJECT_SOURCE_DIR}")



add_library(dynamiclib SHARED dynamiclib.cpp)
find_library(
        log-lib
        log)


target_link_libraries(
        hookzz
        dynamiclib
        ${log-lib})]]
