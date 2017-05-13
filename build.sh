#!/bin/bash

#cmake -DCMAKE_TOOLCHAIN_FILE=<your_toolchain_file>

#DEFAULTS
BUILD_GRAPHICS=1
BUILD_EXAMPLES=0
BUILD_SCRATCH=1
MAKE_VERBOSE=1
#RUN_CMAKE=0

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
  esac
done

if [ $BUILD_GRAPHICS = 1 ]; then
  echo "BUILDING SUBMODULE GLV LIBRARY...."
  echo "please make sure you have run:"
  echo "  git submodule init"
  echo "  git submodule update"
  make -C ext/glv
fi

cmake --version
mkdir -p build
cd build
cmake -DBUILD_GRAPHICS=$BUILD_GRAPHICS -DBUILD_EXAMPLES=$BUILD_EXAMPLES -DBUILD_SCRATCH=$BUILD_SCRATCH -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..

make VERBOSE=$MAKE_VERBOSE -j3
