import graph;
import palette;
import contour;
usepackage("mathptmx"); defaultpen(TimesRoman());
unitsize(1mm);

real scale = 80;
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
filldraw (scale(scale)*((-0.133,-0.116)--(1.133,-0.116)--(1.133,0.620)--(-0.133,0.620)--cycle),white,white);
label("$[0\,0\,0\,1]$", scale(scale)*O, S);
label("$[2\\\overline{1}\\\overline{1}\,0]$", scale(scale)*X, S);
label("$[1\,0\,\\\overline{1}\,0]$", scale(scale/1.000000)*(0.866025,0.500000), N);
path line110111 = (1.000000,-0.000000)--(0.999848,0.017452)--(0.999391,0.034899)--(0.998630,0.052336)--(0.997564,0.069756)--(0.996195,0.087156)--(0.994522,0.104528)--(0.992546,0.121869)--(0.990268,0.139173)--(0.987688,0.156434)--(0.984808,0.173648)--(0.981627,0.190809)--(0.978148,0.207912)--(0.974370,0.224951)--(0.970296,0.241922)--(0.965926,0.258819)--(0.961262,0.275637)--(0.956305,0.292372)--(0.951057,0.309017)--(0.945519,0.325568)--(0.939693,0.342020)--(0.933580,0.358368)--(0.927184,0.374607)--(0.920505,0.390731)--(0.913545,0.406737)--(0.906308,0.422618)--(0.898794,0.438371)--(0.891007,0.453990)--(0.882948,0.469472)--(0.874620,0.484810)--(0.866025,0.500000);
path border = (0,0)--line110111--cycle;
draw (shift(O)*scale(scale/1.000000)*border,black);
sphere (scale(scale/1.000000)*( 0.836627,  0.077172), 0.600000, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/1.000000)*( 0.791849,  0.197653), 0.600000, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
label("Z direction", scale(scale)*(-0.083, 0.500), NE);
label("stereo. proj.", scale(scale)*(-0.083, 0.500), SE, fontsize(8));
