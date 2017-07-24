#!/bin/bash
#run this script from folder to replace "" includes with <> includes

INPLACE=0
RENAME=0
FILETYPE="md"
arg=""
for i
do
  case $i in
  -i)
    INPLACE=1
  ;;
  -r)
    RENAME=1
  ;;
  -c)
  FILETYPE="cpp"
  ;;
  -h)
  FILETYPE="h"
  ;;
  esac
done

DIRECTORY=${PWD##*/}
echo Directory is $DIRECTORY

com=""
if [ $RENAME = 1 ]; then
  com='s/#include "(.*)vsr_(.+)"/#include <\1\2>/g; s/#include (.*)vsr_(.+)/#include \1\2/g; s/<\.\./<vsr/g'
else
  com="s/#include \"(.*\/.*)\"/#include <\1>/g;
       s/#include <\.\./#include <vsr/g;
       s/#include <([usdf].*\/.*)/#include <vsr\/\1/g;
       s/#include [<\"]vsr_app.h[>\"]/#include <vsr\/vsr_app.h>/g;
       s/#include <vsr.h/#include <vsr\/vsr.h/g;
       s/#include \"(vsr_.*)\"/#include <vsr\/$DIRECTORY\/\1>/g"
fi
for f in *.$FILETYPE;
do
  echo "**************\nprocessing $f\n***************";
  echo $com
  if [ $INPLACE = 1 ]; then
      sed -E -i '' -e "$com" $f
  else
      sed -E "$com" $f
  fi
 done
