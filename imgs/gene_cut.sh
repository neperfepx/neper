#!/bin/bash

NEPER="neper --rcfile none"

$NEPER -T -n 1000 -morpho gg -transform "cut(sphere(1,0.5,1,0.7))" -o gene_cut_1
$NEPER -T -n 1000 -morpho gg -domain "cube(1,0.1,1)" -transform "cut(cylinder(0.25,0,0.25,0,1,0,0.15),cylinder(0.25,0,0.75,0,1,0,0.15),cylinder(0.75,0,0.25,0,1,0,0.15),cylinder(0.75,0,0.75,0,1,0,0.15))" -o gene_cut_2
$NEPER -T -n 1000 -morpho gg -domain "cube(0.6,0.6,1.4):translate(-0.3,-0.3,-0.7)" \
  -transform "cut(cylinder(-0.9,0,0,0,1,0,0.8),cylinder(0.9,0,0,0,1,0,0.8),cylinder(0,-0.9,0,1,0,0,0.8),cylinder(0,0.9,0,1,0,0,0.8))" -o gene_cut_3

C="-datacellcol id -datacelltrs 0.5 -cameracoo 3.962000:-5.270000:4.827000 -cameraangle 12 -imagesize 600:600"
for i in 1 2 3
do
  $NEPER -V gene_cut_$i.tess $C -cameracoo 3.962000:-5.270000:4.827000 -print gene_cut_$i
done

convert +append gene_cut_?.png gene_cut.png

exit 0
