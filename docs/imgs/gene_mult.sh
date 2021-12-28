#!/bin/bash

NEPER="neper --rcfile none"

$NEPER -T -n 200::from_morpho -morpho 'gg::lamellar(w=0.02)' \
          -o gene_mult_1

$NEPER -T -n 200::1000*size \
          -morpho 'gg::diameq:dirac(1),1-sphericity:lognormal(0.145,0.03)' \
	  -o gene_mult_2

$NEPER -T -n 200::1000*size::from_morpho \
	  -morpho 'gg::diameq:dirac(1),1-sphericity:lognormal(0.145,0.03)::lamellar(w=0.02:0.1)' \
          -o gene_mult_3

C="-datacelltrs 0.5 -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_mult_1.tess -datacellcol 'scaleid(1)' $C -print gene_mult_1
$NEPER -V gene_mult_2.tess -datacellcol 'scaleid(1)' $C -print gene_mult_2
$NEPER -V gene_mult_3.tess -datacellcol 'scaleid(1)' $C -print gene_mult_3

convert +append gene_mult_?.png gene_mult.png

exit 0
