#!/bin/bash

write_asy ()
{
  echo "import graph;"                                                       >  gene_stat_1.asy
  echo "import stats;"                                                       >> gene_stat_1.asy
  echo "usepackage(\"mathptmx\"); defaultpen(TimesRoman());"                 >> gene_stat_1.asy
  echo "settings.outformat=\"png\";"                                         >> gene_stat_1.asy
  echo ""                                                                    >> gene_stat_1.asy
  echo "size(600,600,IgnoreAspect);"                                         >> gene_stat_1.asy
  echo ""                                                                    >> gene_stat_1.asy
  echo "defaultpen(fontsize(24pt)+black);"                                   >> gene_stat_1.asy
  echo ""                                                                    >> gene_stat_1.asy
  echo "file in=input(\"gene_stat_1.stcell\").line();"                       >> gene_stat_1.asy
  echo "real[][] a=in.dimension(0,0);"                                       >> gene_stat_1.asy
  echo "a=transpose(a);"                                                     >> gene_stat_1.asy
  echo "real[] x1=a[0];"                                                     >> gene_stat_1.asy
  echo "real[] y1=a[1];"                                                     >> gene_stat_1.asy
  echo ""                                                                    >> gene_stat_1.asy
  echo "marker mark1 = marker(scale(1mm)*unitcircle,blue,FillDraw(blue));"   >> gene_stat_1.asy
  echo "draw (graph(x1,y1), invisible, \"$\varepsilon=0.19$\", mark1);"      >> gene_stat_1.asy
  echo ""                                                                    >> gene_stat_1.asy
  echo "xlimits(0,  3);"                                                     >> gene_stat_1.asy
  echo "ylimits(0, 70);"                                                     >> gene_stat_1.asy
  echo "xaxis(\"Normalized equivalent"                                       >> gene_stat_1.asy
  echo "    diameter\",BottomTop,LeftTicks(Step=.5,step=.25,trailingzero));" >> gene_stat_1.asy
  echo "yaxis(\"Number of neighbors\",LeftRight,RightTicks(Step=10));"       >> gene_stat_1.asy
}

NEPER="neper --rcfile none"

if [ -a "gene_peri_1.tess" ]
then
  cp gene_peri_1.tess gene_stat_1.tess
else
  $NEPER -T -n 1000 -morpho gg -per 1 -o gene_stat_1
fi

$NEPER -T -loadtess gene_stat_1.tess -statcell diameq,facenb -for ""
avg=`awk '{sum+=$1;} END {print sum/NR}' gene_stat_1.stcell`
awk -v avg=$avg '{print $1/avg}' gene_stat_1.stcell > diameq
awk '{print $2}' gene_stat_1.stcell > facenb
paste diameq facenb > gene_stat_1.stcell

write_asy
asy gene_stat_1.asy

convert +append -flatten gene_stat_?.png -extent 1200x600 gene_stat.png

exit 0
