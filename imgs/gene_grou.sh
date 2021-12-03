#!/bin/bash

NEPER="neper --rcfile none"

$NEPER -T -n 1000 -group "id<=500?1:2" -o gene_grou_1
$NEPER -T -n 1000 -group "vol>=0.001?1:2" -o gene_grou_2
$NEPER -T -n 1000 -morpho "diameq:0.8*lognormal(1,0.35)+0.2*lognormal(2,0.6),1-sphericity:lognormal(0.145,0.03)" -group mode -o gene_grou_3

C="-datacellcol group -datacelltrs 0.5 -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_grou_1.tess $C -print gene_grou_1
$NEPER -V gene_grou_2.tess $C -print gene_grou_2
$NEPER -V gene_grou_3.tess $C -print gene_grou_3

convert +append gene_grou_?.png gene_grou.png

exit 0
