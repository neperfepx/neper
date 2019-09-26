#!/bin/bash

write_asy ()
{
  echo "import graph;"                                                       >  gene_regu_3.asy
  echo "import stats;"                                                       >> gene_regu_3.asy
  echo "usepackage(\"mathptmx\"); defaultpen(TimesRoman());"                 >> gene_regu_3.asy
  echo "settings.outformat=\"png\";"                                         >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "size(600,600,IgnoreAspect);"                                         >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "defaultpen(fontsize(24pt)+black);"                                   >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "file fin=input(\"gene_regu_1.stelt\");"                             >> gene_regu_3.asy
  echo "real[] a=fin;"                                                       >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "int N=60;"                                                           >> gene_regu_3.asy
  echo "histogram(a,0,2.5,N,normalize=true,low=0,invisible,red+linewidth(2),bars=false);" >> gene_regu_3.asy
  echo "histogram(b,0,2.5,N,normalize=true,low=0,invisible,0.8*green+0.2*black+linewidth(2),bars=false);" >> gene_regu_3.asy
  echo ""                                                                    >> gene_regu_3.asy
  echo "xlimits(0, 2.5);"                                                    >> gene_regu_3.asy
  echo "ylimits(0, 1.5);"                                                    >> gene_regu_3.asy
  echo "xaxis(\"Edge length\",BottomTop,LeftTicks(Step=0.5,step=0.25));"     >> gene_regu_3.asy
  echo "yaxis(\"Frequency\",LeftRight,RightTicks(Step=.5,step=.25));"        >> gene_regu_3.asy
}

NEPER="neper --rcfile none"

if [ -a "gene_morp_1.tess" ]
then
  cp gene_morp_1.tess gene_regu_1.tess
else
  $NEPER -T -n 1000 -morpho gg -o gene_regu_1
fi

$NEPER -T -loadtess gene_regu_1.tess -statedge length:uc -for ""
$NEPER -T -loadtess gene_regu_1.tess -statedge length:uc -reg 1 -o gene_regu_2

C="-datacellcol id -datacelltrs 0.5 -cameraangle 12 -imagesize 600:600"
$NEPER -V gene_regu_1.tess $C -print gene_regu_1
$NEPER -V gene_regu_2.tess $C -print gene_regu_2
write_asy
asy gene_regu_3.asy

convert +append -flatten gene_regu_?.png gene_regu.png

exit 0
