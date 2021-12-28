#!/bin/bash

write_asy ()
{
  echo "import graph;"                                                       >  gene_regu_3.asy
  echo "import stats;"                                                       >> gene_regu_3.asy
  echo "usepackage(\"mathptmx\"); defaultpen(TimesRoman());"                 >> gene_regu_3.asy
  echo "settings.outformat=\"pdf\";"                                         >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "size(600,600,IgnoreAspect);"                                         >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "defaultpen(fontsize(24pt)+black);"                                   >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "file fin=input(\"gene_regu_1.stedge\");"                             >> gene_regu_3.asy
  echo "real[] a=fin;"                                                       >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "file fin=input(\"gene_regu_2.stedge\");"                             >> gene_regu_3.asy
  echo "real[] b=fin;"                                                       >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "int N=60;"                                                           >> gene_regu_3.asy
  echo "histogram(a,0,0.5,N,normalize=true,low=0,invisible,red+linewidth(2),bars=false);" >> gene_regu_3.asy
  echo "histogram(b,0,0.5,N,normalize=true,low=0,invisible,0.8*green+0.2*black+linewidth(2),bars=false);" >> gene_regu_3.asy
  echo "draw (\"No Regularization\", (0.3, 15), 4*SW, red);"                 >> gene_regu_3.asy
  echo "draw (\"Regularization\",    (0.3, 14), 4*SW, 0.8*green+0.2*black);" >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "xlimits(0, 0.3);"                                                    >> gene_regu_3.asy
  echo "ylimits(0, 15);"                                                     >> gene_regu_3.asy
  echo "xaxis(\"Edge length\",BottomTop,LeftTicks(Step=0.1,step=0.05));"     >> gene_regu_3.asy
  echo "yaxis(\"Frequency\",LeftRight,RightTicks(Step=5,step=2.5));"         >> gene_regu_3.asy
}

NEPER="neper --rcfile none"

if [ -a "gene_morp_1.tess" ]
then
  cp gene_morp_1.tess gene_regu_1.tess
else
  $NEPER -T -n 1000 -o gene_regu_1
fi

$NEPER -T -loadtess gene_regu_1.tess -statedge length -for ""
$NEPER -T -loadtess gene_regu_1.tess -statedge length -reg 1 -o gene_regu_2

C="-datacellcol id -datacelltrs 0.5 -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_regu_1.tess $C -print gene_regu_1
$NEPER -V gene_regu_2.tess $C -print gene_regu_2
write_asy
asy gene_regu_3.asy
convert -alpha off -density 300 gene_regu_3.pdf -resize x600 gene_regu_3.png

convert +append gene_regu_?.png gene_regu.png

exit 0
