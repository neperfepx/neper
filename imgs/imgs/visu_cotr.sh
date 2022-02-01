#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "gene_morp_2.tess" ]
then
  cp gene_morp_2.tess visu_cotr_1.tess
else
  $NEPER -T -n 1000 -morpho gg -o visu_cotr_1
fi

if [ -a "mesh_gene_1.msh" ]
then
  cp mesh_gene_1.msh visu_cotr_2.msh
else
  $NEPER -M visu_gene_1.tess -o visu_cotr_2
fi

C="-cameraangle 12 -imagesize 600:600"
$NEPER -V visu_cotr_1.tess $C -datacellcol  id -datacelltrs 0.5 -print visu_cotr_1
$NEPER -V visu_cotr_2.msh  $C -showelt1d all \
	  -dataelt3dedgerad 0.001 -dataelt1drad 0.0025 \
          -datanodecol "real:z" -dataeltcol from_nodes \
	  -print visu_cotr_2

convert +append visu_cotr_?.png visu_cotr.png
convert visu_cotr.png -extent 1800x600 visu_cotr.png

exit 0
