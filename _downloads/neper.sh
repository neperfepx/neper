#!/bin/bash

NEPER="neper --rcfile none"

$NEPER -T -n from_morpho                                                                \
          -morpho "diameq:lognormal(0.035,0.01),sphericity:lognormal(0.145,0.03,1-x)"   \
           -id 3                                                                        \
           -domain "cube(0.3,0.075,1.5)"                                                \
           -transform "cut(cylinder(0.15,0,0.15,0,1,0,0.075),cylinder(0.15,0,1.35,0,1,0,0.075),cylinder(-1.3,0,0.75,0,1,0,1.37),cylinder(1.6,0,0.75,0,1,0,1.37))" \
           -reg 1 -rsel 0.65                                                            \
           -morphooptistop "val=1e-2"                                                   \
           -o specimen

$NEPER -M specimen.tess -rcl 0.65

$NEPER -V specimen.tess                                                                 \
          -datacellcol id                                                               \
          -datacelltrs 0.5                                                              \
          -cameraangle 9.0                                                              \
          -imagesize 1000:2000                                                          \
          -imageformat pov,png                                                          \
           -print specimen-tess

$NEPER -V specimen.tess,specimen.msh                                                    \
          -dataelsetcol id                                                              \
          -dataelt3dedgecol 16:16:16                                                    \
          -showelt1d all                                                                \
          -cameraangle 9.0                                                              \
          -imagesize 1000:2000                                                          \
          -imageformat pov,png                                                          \
          -print specimen-mesh

exit 0

# previous version

$NEPER -T -n 150 -reg 1 -domain "cylinder(1,0.5,150)" 	\
          -morpho gg -rsel 0.25				\
	  -o neper

$NEPER -M neper.tess -rcl 0.35 -pl 3
$NEPER -M -loadmesh "neper.msh:scale(1.001,1.001,1.001)" -o tmp

$NEPER -V "neper.tess,tmp.msh" 		 	 \
	  -showtess 1 -showcell "all" 		 \
	  -showelt "x<0.25" -datacellcol id 	 \
	  -dataelsetcol id -datacelltrs 0.67 	 \
	  -dataedgerad 0.002 -dataelt1drad 0.002 \
	  -dataelt3dedgerad 0.0008 		 \
	  -showelt1d "(cyl==0)&&(elt3d_shown)" 	 \
	  -showedge "(cyl==0)&&(poly_shown)" 	 \
	  -imagesize 600:1050 -cameraangle 28 	 \
	  -cameracoo x+0.7:y+1:z+0.75		 \
	  -print neper

exit 0
