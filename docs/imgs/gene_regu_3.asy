import graph;
import stats;
usepackage("mathptmx"); defaultpen(TimesRoman());
settings.outformat="pdf";

size(600,600,IgnoreAspect);

defaultpen(fontsize(24pt)+black);

file fin=input("gene_regu_1.stedge");
real[] a=fin;

file fin=input("gene_regu_2.stedge");
real[] b=fin;

int N=60;
histogram(a,0,0.5,N,normalize=true,low=0,invisible,red+linewidth(2),bars=false);
histogram(b,0,0.5,N,normalize=true,low=0,invisible,0.8*green+0.2*black+linewidth(2),bars=false);
draw ("No Regularization", (0.3, 15), 4*SW, red);
draw ("Regularization",    (0.3, 14), 4*SW, 0.8*green+0.2*black);

xlimits(0, 0.3);
ylimits(0, 15);
xaxis("Edge length",BottomTop,LeftTicks(Step=0.1,step=0.05));
yaxis("Frequency",LeftRight,RightTicks(Step=5,step=2.5));
