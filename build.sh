#!/bin/bash

mkdir -p build
cd build
#cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/osx.cmake .. #or, add toolchain file here
cmake ..
make VERBOSE=1 -j3
