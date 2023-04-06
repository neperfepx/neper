import graph;
import palette;
import contour;
usepackage("mathptmx"); defaultpen(TimesRoman());
unitsize(1mm);

real scale = 30;
pair O  = (0,0);
pair X  = (1,0);
pair Y  = (0,1);

void circle (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{
  if (w > 0) filldraw(shift(p)*circle(O, s), rgba+opacity(1-transa), rgbb+w);
  else filldraw(shift(p)*circle(O, s), rgba, invisible);
}
void sphere (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{ circle (p, s, theta, rgba, transa, rgbb, w); }
void square (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{ real a = 1.77245385090551602729 * s;
  if (w > 0) filldraw(shift(p)*rotate(theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), rgbb+w);
  else filldraw(shift(p)*rotate(theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), invisible);
}
void cube (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{ square (p, s, theta, rgba, transa, rgbb, w); }
void triangle (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{ real a = 2.69354737417719672123 * s;
  if (w > 0) filldraw(shift(p)*rotate(theta)*scale(a)*((-0.5,-0.289)--(0.5,-0.289)--(0,0.577)--cycle), rgba+opacity(1-transa), rgbb+w);
  else filldraw(shift(p)*rotate(theta)*scale(a)*((-0.5,-0.289)--(0.5,-0.289)--(0,0.577)--cycle), rgba+opacity(1-transa), invisible);
}
void diamond (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{ real a = 1.77245385090551602729 * s;
  if (w > 0) filldraw(shift(p)*rotate(45+theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), rgbb+w);
  else filldraw(shift(p)*rotate(45+theta)*scale(a)*shift((-1/2,-1/2))*unitsquare, rgba+opacity(1-transa), invisible);
}
void star (pair p, real s, real theta, pen rgba, real transa, pen rgbb, real w)
{
  pair pair0 = (0.000000,1.000000);
  pair pair1 = (-0.951057,0.309017);
  pair pair2 = (-0.587785,-0.809017);
  pair pair3 = (0.587785,-0.809017);
  pair pair4 = (0.951057,0.309017);
  pair pair5 = (-0.224514,0.309017);
  pair pair6 = (-0.363271,-0.118034);
  pair pair7 = (-0.000000,-0.381966);
  pair pair8 = (0.363271,-0.118034);
  pair pair9 = (0.224514,0.309017);
if (w > 0) filldraw(shift(p)*rotate(theta)*scale(1.895 * s)*(pair0--pair5--pair1--pair6--pair2--pair7--pair3--pair8--pair4--pair9--cycle), rgba+opacity(1-transa), rgbb+w);
      else filldraw(shift(p)*rotate(theta)*scale(1.895 * s)*(pair0--pair5--pair1--pair6--pair2--pair7--pair3--pair8--pair4--pair9--cycle), rgba+opacity(1-transa), invisible);
}
filldraw (scale(scale)*((-1.265,-1.192)--(1.265,-1.192)--(1.265,1.192)--(-1.265,1.192)--cycle),white,white);
draw (shift(O)*scale(scale)*(-X--X), black);
draw (shift(O)*scale(scale)*(-Y--Y), black);
label("$X_2$", scale(scale)*(1,0), E);
label("$X_1$", scale(scale)*(0,-1), S);
draw (shift(O)*scale(scale)*unitcircle, black);
sphere (scale(scale/1.000000)*( 0.038905, -0.561374), 1.415520, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*( 0.401379,  0.054675), 1.415520, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*(-0.159430,  0.462651), 1.415520, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*(-0.627600, -0.141481), 1.415520, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*( 0.013646,  0.736817), 1.533966, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*(-0.540402,  0.131434), 1.533966, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*(-0.020416, -0.331321), 1.533966, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*( 0.513596,  0.100219), 1.533966, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
label("$\left\{111\right\}$", scale(scale)*(-1.265, 1.000), NE);
label("stereo. proj.", scale(scale)*(-1.255, 1.000), SE, fontsize(8));
