#!/bin/bash

mkdir -p build
cd build
#cmake -DCMAKE_TOOLCHAIN_FILE=<your_toolchain_file>

#DEFAULTS
BUILD_GRAPHICS=1
BUILD_EXAMPLES=1
MAKE_VERBOSE=1

for i
do
  case $i in
  -m | --math)
    BUILD_GRAPHICS=0
  ;;
  -l | --lib)
    BUILD_EXAMPLES=0
  ;;
  -q | --quiet)
    MAKE_VERBOSE=0
  ;;
  esac
done

cmake -DBUILD_GRAPHICS=$BUILD_GRAPHICS -DBUILD_EXAMPLES=$BUILD_EXAMPLES ..

make VERBOSE=$MAKE_VERBOSE -j3
