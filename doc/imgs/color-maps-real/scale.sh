#!/bin/bash

for scale in viridis cividis magma inferno plasma parula heat hot jet gray github legacy viridis:fade "viridis:fade(0.2)"
do
  scale2=`echo $scale | sed -e "s/:/-/g" | sed -e "s/(//g" | sed -e "s/)//g" | sed -e "s/0\./0p/g"`

  neper -V scale.tess,scale.msh           \
           -dataeltcol x                  \
           -dataelt2dedgerad 0            \
           -imagesize 300:30              \
           -cameraprojection orthographic \
           -cameracoo x:y:150             \
           -cameraangle 90                \
           -dataeltcolscheme $scale       \
           -print $scale2

  rm $scale2-scale2d.png
done

exit 0
