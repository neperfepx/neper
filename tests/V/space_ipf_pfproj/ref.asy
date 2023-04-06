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
label("$[1\,1\,1]$", scale(scale/0.541196)*(0.459701,0.459701), N);
path line110111 = (0.541196,-0.000000)--(0.541130,0.013459)--(0.540931,0.026916)--(0.540599,0.040370)--(0.540134,0.053820)--(0.539537,0.067263)--(0.538807,0.080698)--(0.537945,0.094123)--(0.536949,0.107538)--(0.535821,0.120940)--(0.534560,0.134328)--(0.533166,0.147700)--(0.531639,0.161055)--(0.529979,0.174390)--(0.528186,0.187705)--(0.526261,0.200998)--(0.524202,0.214267)--(0.522010,0.227511)--(0.519685,0.240728)--(0.517226,0.253916)--(0.514634,0.267074)--(0.511909,0.280200)--(0.509051,0.293293)--(0.506058,0.306350)--(0.502933,0.319371)--(0.499673,0.332354)--(0.496280,0.345296)--(0.492752,0.358197)--(0.489091,0.371054)--(0.485296,0.383867)--(0.481366,0.396632)--(0.477302,0.409350)--(0.473104,0.422017)--(0.468771,0.434632)--(0.464303,0.447194)--(0.459701,0.459701);
path border = (0,0)--line110111--cycle;
draw (shift(O)*scale(scale/0.541196)*border,black);
sphere (scale(scale/0.541196)*( 0.122494,  0.064285), 0.600000, 0.000000, rgb(1.000000,0.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
sphere (scale(scale/0.541196)*( 0.169885,  0.143500), 0.600000, 0.000000, rgb(0.000000,1.000000,0.000000), 0.000000, rgb(0.000000,0.000000,0.000000), 0.300000);
label("Z direction", scale(scale)*(-0.083, 0.900), NE);
label("equal-area proj.", scale(scale)*(-0.083, 0.900), SE, fontsize(8));
