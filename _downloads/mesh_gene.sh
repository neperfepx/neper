#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "gene_regu_2.tess" ]
then
  cp gene_regu_2.tess mesh_gene_1.tess
else
  $NEPER -T -n 1000 -morpho gg -reg 1 -o mesh_gene_1
fi

$NEPER -M mesh_gene_1.tess -elttype tet          -o mesh_gene_1
$NEPER -M mesh_gene_1.tess -elttype hex -cl 1/23 -o mesh_gene_2

C="-cameraangle 12 -imagesize 600:600 -dataelsetcol id -dataelt3dedgerad 0.001 -dataelt1drad 0.0025 -showelt1d all"
$NEPER -V mesh_gene_1.msh $C -print mesh_gene_1
$NEPER -V mesh_gene_2.msh $C -print mesh_gene_2

convert +append mesh_gene_?.png mesh_gene.png

exit 0
