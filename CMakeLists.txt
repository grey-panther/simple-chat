cmake_minimum_required(VERSION 3.5.1)
project(simple_chat)

set(CMAKE_CXX_STANDARD 11)

# Choose target platform
if (WIN32)
    set(TARGET_BUILD_PLATFORM windows)
else()
    set(TARGET_BUILD_PLATFORM linux)
endif()
message("Target platform: " ${TARGET_BUILD_PLATFORM})

# Add root paths for includes (and for CLion)
include_directories(src/common src/${TARGET_BUILD_PLATFORM})

# Add files for compilation
file(GLOB_RECURSE COMMON_SOURCES src/common/*.hpp src/common/*.cpp)
file(GLOB_RECURSE PLATFORM_SOURCES src/${TARGET_BUILD_PLATFORM}/*.hpp src/${TARGET_BUILD_PLATFORM}/*.cpp)
add_executable(simple_chat src/main.cpp ${COMMON_SOURCES} ${PLATFORM_SOURCES})

# Link platform libraries
if(WIN32)
    target_link_libraries(simple_chat ws2_32) # wsock32 ещё есть
endif()