#!/bin/bash

write_asy ()
{
  echo "import graph;"                                                       >  mesh_stat_1.asy
  echo "import stats;"                                                       >> mesh_stat_1.asy
  echo "usepackage(\"mathptmx\"); defaultpen(TimesRoman());"                 >> mesh_stat_1.asy
  echo "settings.outformat=\"png\";"                                         >> mesh_stat_1.asy
  echo ""                                                                    >> mesh_stat_1.asy
  echo "size(600,600,IgnoreAspect);"                                         >> mesh_stat_1.asy
  echo ""                                                                    >> mesh_stat_1.asy
  echo "defaultpen(fontsize(24pt)+black);"                                   >> mesh_stat_1.asy
  echo ""                                                                    >> mesh_stat_1.asy
  echo "file fin=input(\"mesh_stat_1.stelt\");"                             >> mesh_stat_1.asy
  echo "real[] a=fin;"                                                       >> mesh_stat_1.asy
  echo ""                                                                    >> mesh_stat_1.asy
  echo "int N=40;"                                                           >> mesh_stat_1.asy
  echo "histogram(a,0,1,N,normalize=true,low=0,invisible,red+linewidth(2),bars=false);" >> mesh_stat_1.asy
  echo ""                                                                    >> mesh_stat_1.asy
  echo "xlimits(0, 1);"                                                    >> mesh_stat_1.asy
  echo "ylimits(0, 3.5);"                                                    >> mesh_stat_1.asy
  echo "xaxis(\"Radius ratio\",BottomTop,LeftTicks(Step=0.2,step=0.1));"     >> mesh_stat_1.asy
  echo "yaxis(\"Frequency\",LeftRight,RightTicks(Step=.5,step=.25,trailingzero));"        >> mesh_stat_1.asy
}

NEPER="neper --rcfile none"

cp mesh_size_1.msh mesh_stat_1.msh

$NEPER -M -loadmesh mesh_stat_1.msh -statelt rr -for ""

write_asy
asy mesh_stat_1.asy

convert +append -flatten mesh_stat_?.png mesh_stat.png
convert mesh_stat.png -extent 1200x600 mesh_stat.png

exit 0
