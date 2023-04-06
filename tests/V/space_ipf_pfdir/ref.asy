import graph;
import palette;
import contour;
usepackage("mathptmx"); defaultpen(TimesRoman());
unitsize(1mm);

real scale = 60;
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
filldraw (scale(scale)*((-0.133,-0.096)--(1.133,-0.096)--(1.133,1.000)--(-0.133,1.000)--cycle),white,white);
label("$[1\,0\,0]$", scale(scale)*O, S);
label("$[1\,1\,0]$", scale(scale)*X, S);
label("$[1\,1\,1]$", scale(scale/0.414214)*(0.366025,0.366025), N);
path line110111 = (0.414214,-0.000000)--(0.414176,0.010301)--(0.414063,0.020603)--(0.413876,0.030907)--(0.413613,0.041213)--(0.413275,0.051522)--(0.412861,0.061835)--(0.412372,0.072152)--(0.411807,0.082475)--(0.411165,0.092804)--(0.410447,0.103140)--(0.409653,0.113484)--(0.408781,0.123836)--(0.407832,0.134198)--(0.406805,0.144569)--(0.405699,0.154951)--(0.404514,0.165345)--(0.403250,0.175751)--(0.401906,0.186171)--(0.400481,0.196604)--(0.398975,0.207051)--(0.397386,0.217514)--(0.395714,0.227993)--(0.393959,0.238489)--(0.392120,0.249003)--(0.390195,0.259535)--(0.388184,0.270086)--(0.386085,0.280657)--(0.383898,0.291248)--(0.381622,0.301861)--(0.379256,0.312496)--(0.376798,0.323154)--(0.374247,0.333835)--(0.371602,0.344540)--(0.368862,0.355270)--(0.366025,0.366025);
path border = (0,0)--line110111--cycle;
draw (shift(O)*scale(scale/0.414214)*border,black);
sphere (scale(scale/0.414214)*( 0.310560,  0.050535), 0.600000, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/0.414214)*( 0.386318,  0.183431), 0.600000, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
label("Z direction", scale(scale)*(-0.083, 0.900), NE);
label("stereo. proj.", scale(scale)*(-0.083, 0.900), SE, fontsize(8));
