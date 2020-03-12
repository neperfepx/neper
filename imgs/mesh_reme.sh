#!/bin/bash

NEPER="neper --rcfile none"
# 
# if [ -a "neper.msh" ]
# then
#   cp neper.msh mesh_reme_t.msh
# else
#   exit -1
# fi
# 
eps1="1"
z1="0.5"
# 
# # $NEPER -M -loadmesh mesh_reme_t.msh -statnode x,y,z -for ""
awk '{print $1-0.25,$2-0.25,$3-0.5}'      mesh_reme_t.stnode > xyz
awk -v eps1="$eps1" -v z1="$z1" '{print 0}' xyz > dx
awk -v eps1="$eps1" -v z1="$z1" '{print 0}' xyz > dy
awk -v eps1="$eps1" -v z1="$z1" '{if ($3<0) {print -eps1*(-$3-$3*$3/(2*z1))} else {print eps1*($3-$3*$3/(2*z1))}}' xyz > dz

paste xyz dx dy dz | awk '{print $1+$4,$2+$5,$3+$6}' > xyz-new

$NEPER -M -loadmesh mesh_reme_t.msh:xyz -o mesh_reme_1
# $NEPER -M mesh_reme_1.msh         -rcl 0.35 -o mesh_reme_2

for i in 1 2
do
  $NEPER -V "neper.tess,mesh_reme_${i}.msh"  	 \
            -showtess 0 -showcell 0	         \
	    -showedge "(domtype==1)&&(cyl==0)" 	 \
	    -showface "domtype==2"		 \
	    -datafacetrs 0.5			 \
	    -datanodecol "scal:file(epsz)" 	 \
	    -datanodescale 0:1		 \
	    -dataeltscale 0:1		 \
	    -dataeltcol from_nodes		 \
	    -dataelt3dedgerad 0.0008 		 \
	    -showelt1d cyl==0                	 \
	    -imagesize 600:1050 -cameraangle 12  \
	    -cameracoo x:y+4.5:z		 \
	    -print mesh_reme_$i
done
	   
convert +append mesh_reme_?.png mesh_reme.png

exit 0
