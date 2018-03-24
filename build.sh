#!/bin/bash

#cmake -DCMAKE_TOOLCHAIN_FILE=<your_toolchain_file>

#DEFAULTS
BUILD_GRAPHICS=1
BUILD_EXAMPLES=0
MAKE_VERBOSE=1
DO_INSTALL=0
DO_UPDATE=0

for i
do
  case $i in
  -m | --math)
    BUILD_GRAPHICS=0
  ;;
  -x | --examples)
    BUILD_EXAMPLES=1
  ;;
  -q | --quiet)
    MAKE_VERBOSE=0
  ;;
  -i | --install)
    DO_INSTALL=1
  ;;
  -u | --update)
    DO_UPDATE=1
  esac
done


if [ $DO_UPDATE = 1 ]; then
  git submodule update --init --recursive
fi

cmake --version
mkdir -p build
cd build
cmake -DBUILD_GRAPHICS=$BUILD_GRAPHICS -DBUILD_EXAMPLES=$BUILD_EXAMPLES -DBUILD_SCRATCH=$BUILD_SCRATCH -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

make VERBOSE=$MAKE_VERBOSE -j3

if [ $DO_INSTALL = 1 ]; then
  make install
fi
