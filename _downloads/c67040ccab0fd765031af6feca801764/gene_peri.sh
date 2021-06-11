#!/bin/bash

NEPER="neper --rcfile none"

# $NEPER -T -n 1000 -morpho gg -per 1   -o gene_peri_1
# $NEPER -T -n 1000 -morpho gg -per x   -o gene_peri_2
# $NEPER -T -n 1000 -morpho gg -per x,y -o gene_peri_3

C="-datacellcol id -dataedgerad 0.002 -datacelltrs 0.5 -cameraangle 14 -cameracoo x:-6.3:z -cameraprojection orthographic"
for i in 1 2 3
do
  $NEPER -V gene_peri_$i.tess $C -print gene_peri_$i
done

convert +append gene_peri_?.png gene_peri.png

exit 0
