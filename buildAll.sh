#!/bin/bash

make -C ext/glv
mkdir -p build
mkdir -p build/doc
cd build
#cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/osx.cmake .. #or, add toolchain file here
cmake -DBUILD_GRAPHICS=1 -DBUILD_EXAMPLES=1 ..
make VERBOSE=1 -j3
