#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "mesh_size_2.msh" ]
then
  cp mesh_size_2.msh visu_slic_1.msh
else
  $NEPER -T -n 1000 -morpho gg -reg 1 -o visu_slic_1
  $NEPER -M visu_slic_1.tess $C -rcl "1,body>0:0.35" -print visu_slic_1
fi

$NEPER -M -loadmesh visu_slic_1.msh -statnode 2dmeshd

C="-cameraangle 12 -imagesize 600:600"
$NEPER -V visu_slic_1.msh $C                                     \
        -dataelt3dedgerad 0.001 -dataelt1drad 0.0025             \
        -datanodecol "real:file(visu_slic_1.stnode)"             \
        -datanodescale 0.0:0.01:0.02:0.03:0.04:0.05              \
	-dataeltcol from_nodes                                   \
	-slicemesh x=0.5,y=0.5,z=0.5                             \
	-print visu_slic_1

convert +append visu_slic_?.png visu_slic.png

exit 0
