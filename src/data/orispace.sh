#!/bin/bash

set -e

echo "Building orientation spaces..."
mkdir -p orispace
for crysym in cubic hexagonal
do
  mkdir -p orispace/$crysym
  neper -T -n 1 -domain "rodrigues($crysym)" -per 1 -o orispace/$crysym/fr > /dev/null

  for size in $(seq 5 5 20)
  do
    echo "- $crysym $size..."

    neper -M orispace/$crysym/fr.tess -mesh2dalgo fron -order 1 -nset none -faset none -cl "(0.414213562373*2)/$size" \
             -o orispace/$crysym/fr$size
  done
done

exit 0
