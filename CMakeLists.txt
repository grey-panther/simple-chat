cmake_minimum_required(VERSION 3.5.1)
project(simple_chat)

set(CMAKE_CXX_STANDARD 11)

# Choose target platform
if (WIN32)
    set(TARGET_BUILD_PLATFORM windows)
else()
    set(TARGET_BUILD_PLATFORM linux)

    # TODO Make linux implementation.
    message(FATAL_ERROR "Linux implementation has not been supported yet. CMake will exit.")
endif()
message("Target platform: " ${TARGET_BUILD_PLATFORM})

# Add root paths for includes (and for CLion)
include_directories(src/common src/${TARGET_BUILD_PLATFORM})

# Headers for using std::thread on WIN32 platform
if(WIN32)
    include_directories(libs/mingw-std-threads)
endif()

# Add files for compilation
file(GLOB_RECURSE COMMON_SOURCES src/common/*.hpp src/common/*.cpp)
file(GLOB_RECURSE PLATFORM_SOURCES src/${TARGET_BUILD_PLATFORM}/*.hpp src/${TARGET_BUILD_PLATFORM}/*.cpp)
add_executable(simple_chat src/main.cpp ${COMMON_SOURCES} ${PLATFORM_SOURCES})
add_executable(chat_server src/server_main.cpp ${COMMON_SOURCES} ${PLATFORM_SOURCES})
add_executable(chat_client src/client_main.cpp ${COMMON_SOURCES} ${PLATFORM_SOURCES})

# Link threads support library
#target_link_libraries(simple_chat pthread)

# Link platform libraries
if(WIN32)
    target_link_libraries(simple_chat ws2_32)
    target_link_libraries(chat_server ws2_32)
    target_link_libraries(chat_client ws2_32)
endif()
