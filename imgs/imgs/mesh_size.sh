#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "mesh_gene_1.tess" ]
then
  cp mesh_gene_1.tess mesh_size_1.tess
else
  $NEPER -T -n 1000 -morpho gg -reg 1 -o mesh_size_1
fi

$NEPER -M mesh_size_1.tess $C -rcl 0.5             -o mesh_size_1
$NEPER -M mesh_size_1.tess $C -rcl "body>0?0.35:1" -o mesh_size_2

C="-cameraangle 12 -imagesize 600:600 -dataelsetcol id -dataelt3dedgerad 0.001 -dataelt1drad 0.0025 -showelt1d all"
$NEPER -V mesh_size_1.msh $C -print mesh_size_1
$NEPER -V mesh_size_2.msh $C                        \
          -showelt "x<0.35||y>0.65||z<0.35"         \
	  -showelt1d elt3d_shown                    \
          -print mesh_size_2

convert +append mesh_size_?.png -extent 1800x600 mesh_size.png

exit 0
