import graph;
import stats;
usepackage("mathptmx"); defaultpen(TimesRoman());
settings.outformat="png";

size(600,600,IgnoreAspect);

defaultpen(fontsize(24pt)+black);

file fin=input("gen_reg_1.stedge");
real[] a=fin;

file fin=input("gen_reg_2.stedge");
real[] b=fin;

int N=60;
histogram(a,0,2.5,N,normalize=true,low=0,invisible,red+linewidth(2),bars=false);
histogram(b,0,2.5,N,normalize=true,low=0,invisible,0.8*green+0.2*black+linewidth(2),bars=false);

xlimits(0, 2.5);
ylimits(0, 1.5);
xaxis("Edge length",BottomTop,LeftTicks(Step=0.5,step=0.25));
yaxis("Frequency",LeftRight,RightTicks(Step=.5,step=.25));
