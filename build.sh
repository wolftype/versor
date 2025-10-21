#!/bin/bash

#cmake -DCMAKE_TOOLCHAIN_FILE=<your_toolchain_file>

#DEFAULTS
BUILD_GRAPHICS=1
BUILD_EXAMPLES=0
MAKE_VERBOSE=1
DO_INSTALL=0
DO_UPDATE=0
USE_DOUBLE_PRECISION=1
VERSOR_EXPORT_LEGACY_GLOBALS=1

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
  ;;
  -f | --float)
    USE_DOUBLE_PRECISION=0
  ;;
  --no-legacy)
    VERSOR_EXPORT_LEGACY_GLOBALS=0
  esac
done

if [ $DO_UPDATE = 1 ]; then
  git submodule update --init --recursive
fi

cmake --version
BUILD_DIRECTORY=build
cmake -B $BUILD_DIRECTORY -S . -DBUILD_GRAPHICS=$BUILD_GRAPHICS -DBUILD_EXAMPLES=$BUILD_EXAMPLES -DUSE_DOUBLE_PRECISION=$USE_DOUBLE_PRECISION -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DVERSOR_EXPORT_LEGACY_GLOBALS=${VERSOR_EXPORT_LEGACY_GLOBALS}

make -C $BUILD_DIRECTORY VERBOSE=$MAKE_VERBOSE

if [ $DO_INSTALL = 1 ]; then
  make -C $BUILD_DIRECTORY install
fi
