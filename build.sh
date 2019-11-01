#!/usr/bin/env bash
set -uex
check_version(){
    CMD=$1
    OPT=$2
    VERSION=$3

    #$CMD $OPT
    RES=$($CMD $OPT |head -1)
    echo $RES
   
}

check_version "gcc" "--version" "4.8.2"
check_version "cmake" "--version" "2.8"
cmake .
make 
cp ./lib/libtxz_webrtcvad.so ./src/java/libtxz_webrtcvad.so
rm -rf ./build
rm -rf CMakeCache.txt
rm -rf CMakeFiles
rm -rf cmake_install.cmake
rm -rf Makefile
rm -rf CTestTestfile.cmake