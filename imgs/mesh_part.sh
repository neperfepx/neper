#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "mesh_gene_1.tess" ]
then
  cp mesh_gene_1.tess mesh_part_1.tess
else
  $NEPER -T -n 1000 -morpho gg -reg 1 -o mesh_part_1
fi

$NEPER -M mesh_part_1.tess $C -part 8:8 -o mesh_part_1 -statelt part

C="-cameraangle 12 -imagesize 600:600 -dataelsetcol id -dataelt3dedgerad 0.001 -dataelt1drad 0.0025 -showelt1d all"

exit 0
