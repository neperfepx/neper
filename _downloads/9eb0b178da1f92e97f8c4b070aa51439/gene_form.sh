#!/bin/bash

NEPER="neper --rcfile none"

if [ -a "gene_morp_2.tess" ]
then
  $NEPER -T -loadtess gene_morp_2.tess -format tess,tesr -tesrsize 64 -o gene_form_1
else
  $NEPER -T -n 1000 -morpho gg -o gene_form_1 -format tess,tesr -tesrsize 64
fi

mv gene_form_1.tesr gene_form_2.tesr

C="-datacellcol id -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_form_1.tess $C                   -print gene_form_1
$NEPER -V gene_form_2.tesr $C -datavoxedgerad 0 -print gene_form_2

convert +append gene_form_?.png gene_form.png

exit 0
