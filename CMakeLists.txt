cmake_minimum_required(VERSION 2.8)
set(CMAKE_VERBOSE_MAKEFILE on)
project(txz_webrtcvad)
set(CMAKE_BUILD_TYPE DEBUG)

set(LIBRARY_OUTPUT_PATH ./lib)
set(EXECUTABLE_OUTPUT_PATH ./execute)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_STANDARD 99)
file(GLOB SRC_FILES src/cxx/*.c src/cxx/*.h src/cxx/thirdparty/webrtc/common_audio/*/*.c src/cxx/thirdparty/webrtc/rtc_base/*.c*)
include_directories(src/cxx/thirdparty/webrtc) #thirdparty/include/webrtc
list(REMOVE_ITEM SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/cxx/main.c)
# message("--------------all file = "${SRC_FILES})

add_library(txz_webrtcvad SHARED ${SRC_FILES})
# target_link_libraries(txz_webrtcvad)

project(vad_demo)

link_directories(vad_demo ${CMAKE_CURRENT_SOURCE_DIR}/lib)


add_executable(vad_demo src/cxx/main.c)
target_link_libraries(vad_demo txz_webrtcvad)
