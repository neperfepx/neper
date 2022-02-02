import graph;
import stats;
usepackage("mathptmx"); defaultpen(TimesRoman());
settings.outformat="png";

size(600,600,IgnoreAspect);

defaultpen(fontsize(24pt)+black);

file in=input("gene_stat_1.stcell").line();
real[][] a=in.dimension(0,0);
a=transpose(a);
real[] x1=a[0];
real[] y1=a[1];

marker mark1 = marker(scale(1mm)*unitcircle,blue,FillDraw(blue));
draw (graph(x1,y1), invisible, "$\varepsilon=0.19$", mark1);

xlimits(0,  3);
ylimits(0, 70);
xaxis("Normalized equivalent
    diameter",BottomTop,LeftTicks(Step=.5,step=.25,trailingzero));
yaxis("Number of neighbors",LeftRight,RightTicks(Step=10));
