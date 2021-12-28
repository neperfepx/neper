import graph;
import stats;
usepackage("mathptmx"); defaultpen(TimesRoman());
settings.outformat="png";

size(600,600,IgnoreAspect);

defaultpen(fontsize(24pt)+black);

file fin=input("mesh_stat_1.stelt");
real[] a=fin;

int N=40;
histogram(a,0,1,N,normalize=true,low=0,invisible,red+linewidth(2),bars=false);

xlimits(0, 1);
ylimits(0, 3.5);
xaxis("Radius ratio",BottomTop,LeftTicks(Step=0.2,step=0.1));
yaxis("Frequency",LeftRight,RightTicks(Step=.5,step=.25,trailingzero));
