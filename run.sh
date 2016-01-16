#!/bin/bash

#BUILDS AND RUNS ONLY SINGLE FILE 
echo Compiling and Running $1

DIRECTORY=`dirname $1`
TARGET=`basename $1|cut -d'.' -f1 | sed -e "s|/|_|g"`

echo Directory is $DIRECTORY
echo Target is $TARGET

mkdir -p scratch/build
cd scratch/build

rm bin/$TARGET

if [ $2 = "--configure" ] || [ $2 = "-c" ]; then
  cmake ..
fi

make $TARGET
./bin/$TARGET
