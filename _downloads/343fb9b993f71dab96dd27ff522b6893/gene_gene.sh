#!/bin/bash

NEPER="neper --rcfile none"

$NEPER -T -n 1000 -domain "cube(1,1,1)"       -o gene_gene_1
$NEPER -T -n 1000 -domain "cylinder(1,1,200)" -o gene_gene_2
$NEPER -T -n 1000 -domain "sphere(1,1000)"    -o gene_gene_3

C="-datacellcol id -datacelltrs 0.5 -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_gene_1.tess $C -print gene_gene_1
$NEPER -V gene_gene_2.tess $C -showedge "cyl==0"   -cameraangle 11 -print gene_gene_2
$NEPER -V gene_gene_3.tess $C -showedge "polynb>1" -cameraangle 9  -print gene_gene_3

convert +append gene_gene_?.png gene_gene.png

exit 0
