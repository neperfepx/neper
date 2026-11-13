#!/bin/bash

NEPER="neper --rcfile none"

# Images are rendered at 1600x800 and then cropped to their content
I="-imagesize 1600:800"
crop () { convert $1.png -trim +repage -bordercolor white -border 20 $1.png; }

# Periodic tessellations are viewed along y, with an orthographic projection
C="-cameracoo x:-6.3:z -cameraprojection orthographic"

# Regularized periodic tessellation
$NEPER -T -n 300 -morpho gg -periodicity all -o reg -reg 1
$NEPER -M reg.tess -rcl 0.8 -pl 10
$NEPER -V reg.tess,reg.msh -showelt1d all -dataelt1drad "default*1.5" $C $I -print periodic-reg
crop periodic-reg

# Multiscale periodic tessellation, regularized and meshed
$NEPER -T -n "300::vol*300*3" -morpho "gg::voronoi" -periodicity all -o msper -reg 1
$NEPER -M msper.tess -rcl 0.8 -pl 10
$NEPER -V msper.tess,msper.msh -showelt1d all -dataelt1drad "default*1.5" $C $I -print multiscale-periodic
crop multiscale-periodic

# Conformal hexahedral mesh, with half of the elements removed, and the domain
$NEPER -T -n 30 -morpho gg -reg 1 -o hex
$NEPER -M hex.tess -elttype hex
$NEPER -V hex.tess,hex.msh -showelt "y>0.5" -showelt1d elt3d_shown -showdomain 1 -datadomainrad "default*0.5" \
  -imagesize 1600:1600 -print hex-mesh
crop hex-mesh

# Mesh size map: central grain (2) of a grain-growth polycrystal, cut at
# its centroid, with the 1D elements of its edges (13 to 48)
$NEPER -T -n 50 -morpho gg -reg 1 -o d
$NEPER -M d.tess -meshpoly "id==2" -faset none -rcl 1.5 -rclface 0.2 -rcledge 0.04 -pl 1.4
$NEPER -V d.tess,d.msh -showelt3d "y>0.38" -showelt1d "elset1d>=13&&elset1d<=48" \
  -dataelt3dcol lightblue -dataelt3dedgerad 0.0006 -dataelt1drad 0.0018 \
  -cameracoo 3.465:-2.05:2.91 -cameralookat 0.465:0.4:0.41 -cameraangle 8 \
  -imagesize 1600:1600 -print mesh-size-map
crop mesh-size-map

# Hourglass domain, meshed
$NEPER -T -n 1000 -morpho gg -morphooptistop val=1e-2 -reg 1 -domain "hourglass(1.5,1,0.95)" -o hg
$NEPER -M hg.tess
$NEPER -V hg.tess,hg.msh -showelt1d all -dataelt1drad 0.003 $I -print hourglass-mesh
crop hourglass-mesh

# Custom node set: node at the center of the top face of the hourglass
$NEPER -M hg.tess -nset "faces,edges,vertices,topcenter:closest(0.5,0.5,1.5)" -o hgnset
$NEPER -V hg.tess,hgnset.msh -showelt1d all -dataelt1drad 0.003 -shownode nset==topcenter -datanodecol red -datanoderad 0.04 $I -print custom-nset
crop custom-nset

# Faster execution: chart of the timings measured with 5.0.0 and the
# current version (-T -n 1000 -morpho gg, -M, -V, 12 threads)
python3 faster-execution.py
convert -density 192 faster-execution.svg faster-execution.png
rm -f faster-execution.svg

# Python interface: grain rotations of n20.sim, one color per grain (needs
# the neper Python package, e.g. PYTHONPATH=<neper>/python)
python3 python.py
rm -rf n20.sim

# Improved 3D regularization: edges below the threshold (0.025), in red,
# for a tessellation regularized by 5.0.0 (NEPER500) and the current version
if [ -n "$NEPER500" ]; then
  $NEPER500 --rcfile none -T -n 1000 -morpho gg -regularization 1 -o reg500
  $NEPER -T -n 1000 -morpho gg -reg 1 -o regnow
  for v in 500 now; do
    $NEPER -V reg$v.tess -showcell all -datacellcol lightgray -datacelltrs 0.9 \
      -showedge "length<0.025" -dataedgerad 0.01 -showdomain 1 -datadomainrad 0.002 \
      -dataedgecol "real:(length<0.025)" -dataedgecolscheme "custom(black,red)" \
      -dataedgescale 0:1 -imagesize 1200:1200 -print reg-$v
    rm -f reg-$v-scale1.png
    convert reg-$v.png -trim +repage -resize 600x600 -bordercolor white -border 20 reg-$v.png
  done
  n500=$($NEPER -T -loadtess reg500.tess -statedge length -o s500 > /dev/null; awk '$1<0.025' s500.stedge | wc -l)
  nnow=$($NEPER -T -loadtess regnow.tess -statedge length -o snow > /dev/null; awk '$1<0.025' snow.stedge | wc -l)
  convert reg-500.png -gravity north -splice 0x50 -pointsize 30 -annotate +0+10 "5.0.0: $n500 edges" reg-500.png
  convert reg-now.png -gravity north -splice 0x50 -pointsize 30 -annotate +0+10 "Now: $nnow edges" reg-now.png
  convert reg-500.png reg-now.png +append -bordercolor white -border 10 reg-edges.png
  rm -f reg-500.png reg-now.png
fi

# Orilib program: terminal image of an interactive and a scripted session
# (needs the orilib binary, built by make orilib)
if [ -n "$ORILIB" ]; then
  python3 orilib-terminal.py "$ORILIB" 7.0.0
  convert -density 192 orilib-terminal.svg orilib-terminal.png
  rm -f orilib-terminal.svg
fi

exit 0
