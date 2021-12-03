#!/bin/bash

NEPER="neper --rcfile none"

$NEPER -T -n 1000            -o gene_morp_1
$NEPER -T -n 1000 -morpho gg -o gene_morp_2
$NEPER -T -n 1000 -morpho "diameq:dirac(1),1-sphericity:lognormal(0.145,0.03)" \
  			     -o gene_morp_3

C="-datacellcol id -datacelltrs 0.5 -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_morp_1.tess $C -print gene_morp_1
$NEPER -V gene_morp_2.tess $C -print gene_morp_2
$NEPER -V gene_morp_3.tess $C -print gene_morp_3

convert +append gene_morp_?.png gene_morp.png

exit 0
