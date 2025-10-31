#!/bin/bash

#cmake -DCMAKE_TOOLCHAIN_FILE=<your_toolchain_file>

#DEFAULTS
BUILD_GRAPHICS=1
BUILD_EXAMPLES=0
MAKE_VERBOSE=1
DO_INSTALL=0
DO_UPDATE=0
USE_DOUBLE_PRECISION=1
JOBS=4

# This environment variable is used by cmake.
export VERBOSE=1

# Decode commandline arguments.
while [ $# -gt 0 ]
do
  case $1 in
    -m|--math)        BUILD_GRAPHICS=0 ;;
    -x|--examples)    BUILD_EXAMPLES=1 ;;
    -q|--quiet)       MAKE_VERBOSE=0; unset VERBOSE ;;
    -i|--install)     DO_INSTALL=1 ;;
    -u|--update)      DO_UPDATE=1 ;;
    -f|--float)       USE_DOUBLE_PRECISION=0 ;;

    # -j with separate argument.
    -j|--parallel)
      shift || { echo "Error: missing value for $1." >&2; exit 2; }
      JOBS=$1
      ;;

    # -j<NUM> compact form.
    -j*[0-9]*)
      JOBS=${1#-j}
      ;;

    # --parallel=<NUM> form.
    --parallel=*)
      JOBS=${1#--parallel=}
      ;;

    -h|--help)
      echo "Usage: build.sh [OPTION]..."
      echo "Configure and build versor."
      echo "  -h, --help         print a short overview of options."
      echo "  -i, --install      install headers and static library in /usr/local."
      echo "  -j, --parallel=N   build using N jobs in parallel."
      echo "  -m, --math         math only (no graphics)."
      echo "  -q, --quiet        turn verbose build off."
      echo "  -u, --update       update git submodules before building."
      echo "  -x, --examples     build the examples."
      exit 0
      ;;

    --) # End of options.
      shift
      break
      ;;

    -*) # Unknown option.
      echo "Error: unknown option: $1" >&2
      exit 2
      ;;

    *) # Positional arg
      posargs="$posargs${posargs:+ }$1"
      echo "Unused positional argument: \"${posargs}\"."
      ;;
  esac
  shift
done

if [ $DO_UPDATE = 1 ]; then
  git submodule update --init --recursive
fi

cmake --version
BUILD_DIRECTORY=build

# Configure.
cmake -B $BUILD_DIRECTORY \
  -DCMAKE_VERBOSE_MAKEFILE=$MAKE_VERBOSE \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
  -DBUILD_DIRECTORY=$BUILD_DIRECTORY \
  -DBUILD_GRAPHICS=$BUILD_GRAPHICS \
  -DBUILD_EXAMPLES=$BUILD_EXAMPLES \
  -DUSE_DOUBLE_PRECISION=$USE_DOUBLE_PRECISION

# Build.
cmake --build $BUILD_DIRECTORY --parallel $JOBS

if [ $DO_INSTALL = 1 ]; then
  sudo cmake --install $BUILD_DIRECTORY --parallel $JOBS
fi
