#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "gene_morp_2.tess" ]
then
  cp gene_morp_2.tess visu_sele_1.tess
else
  $NEPER -T -n 1000 -morpho gg -reg 1 -o visu_sele_1
fi

if [ -a "mesh_size_2.msh" ]
then
  cp mesh_size_2.msh visu_sele_2.msh
else
  $NEPER -M visu_sele_1.tess $C -rcl "1,body>0:0.35" -print visu_sele_2
fi

C="-cameraangle 12 -imagesize 600:600"
$NEPER -V visu_sele_1.tess $C -datacellcol id -datacelltrs 0.5 \
        -showcell "id<=50" -showedge "cell_shown||(domtype==1)" \
        -print visu_sele_1
$NEPER -V visu_sele_2.msh $C -dataelsetcol id \
        -showelt "(z<0.5)||(body>0)"  -showelt1d elt3d_shown \
        -dataelt3dedgerad 0.001 -dataelt1drad 0.0025 \
        -print visu_sele_2

convert +append visu_sele_?.png visu_sele.png

exit 0
