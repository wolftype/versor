
#!/bin/bash

#cmake -DCMAKE_TOOLCHAIN_FILE=<your_toolchain_file>

#DEFAULTS
BUILD_GRAPHICS=0
BUILD_EXAMPLES=0
MAKE_VERBOSE=1
DO_INSTALL=0
DO_UPDATE=0
USE_DOUBLE_PRECISION=0

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
  esac
done

if [ $DO_UPDATE = 1 ]; then
  git submodule update --init --recursive
fi

cmake --version

#ANDROID_ABI=armeabi-v7a
ANDROID_ABI=arm64-v8a

BUILD_DIRECTORY=build/android
mkdir -p $BUILD_DIRECTORY
cd $BUILD_DIRECTORY
cmake -DBUILD_DIRECTORY=$BUILD_DIRECTORY -DBUILD_GRAPHICS=$BUILD_GRAPHICS -DBUILD_EXAMPLES=$BUILD_EXAMPLES -DBUILD_SCRATCH=$BUILD_SCRATCH -DUSE_DOUBLE_PRECISION=$USE_DOUBLE_PRECISION -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DANDROID_ABI=$ANDROID_ABI -DANDROID_PLATFORM=android-29 -DANDROID_NDK=/Applications/2022.3.12f1/PlaybackEngines/AndroidPlayer/NDK -DCMAKE_TOOLCHAIN_FILE=/Applications/2022.3.12f1/PlaybackEngines/AndroidPlayer/NDK/build/cmake/android.toolchain.cmake ../..

make VERBOSE=$MAKE_VERBOSE -j3

if [ $DO_INSTALL = 1 ]; then
  make install
fi
