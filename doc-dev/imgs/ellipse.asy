unitsize (4cm);
settings.outformat="pdf";
usepackage("mathptmx"); defaultpen(TimesRoman());

pair O = (0,0);
pair B = (1,0);

real r1 = 1;
real r2 = 1./3;
real xI;
int qty = 10;

filldraw (ellipse (O, r1, r2), lightblue, blue);

draw ((-r1-0.2,0)--(r1+0.2,0), Arrow);
label ("$x$", (r1+0.2,0), S);
draw ((0,-r2-0.2)--(0,r2+0.2), Arrow);
label ("$y$", (0,r2+0.2), W);

dot (O);
dot ((r1,0));
dot ((0,r2));
label ("$O$", O, SW);
label ("$r_1$", (r1,0), SE);
label ("$r_2$", (0,r2), NW);

for (int i = -qty; i <= qty; ++i)
{
  xI = r1 * i / qty;
  real xC = xI * (1 - r2^2/r1^2);
  real rC = sqrt (r2^2 * (1 - xC^2 / (r1^2 - r2^2)));

  draw (circle ((xC,0), rC), red);
}
