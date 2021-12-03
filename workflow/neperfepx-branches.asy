import geometry;

size(40cm,20cm);
settings.outformat="pdf";

real h = 5;
real dh = 0.2;
real w = 1.2;

real lw=0.7;

dotfactor=10;

dot("\tt v1.0.0", (0, 0), W, 0.8*green);
label ("\tt main", (0,h), NW, blue);

// label ("bug fixes", (-2*dh,2*dh), W);
// draw ((-2*dh,2*dh)--(0,1*dh), Arrow);
// draw ((-2*dh,2*dh)--(0,2*dh), Arrow);
// draw ((-2*dh,2*dh)--(0,3*dh), Arrow);

dot((0, dh));
dot((0, 2*dh));
dot((0, 3*dh));

dot("\tt v1.0.1", (0, 2), W, 0.8*green);

draw ((0,0)--(0,h), blue+linewidth(lw), Arrow);

draw ((0,0)--(w-dh,0)..(w,dh)--(w,1.6), lightblue+linewidth(lw), Arrow);
label ("\tt devel-1.0.1", (w,1.6), NW, lightblue);

draw ((0,2)--(w-dh,2)..(w,2+dh)--(w,h), blue+linewidth(lw), Arrow);
label ("\tt devel", (w,h), NW, blue);

label ("\bf Public repository", (-dh,h+5*dh), N);
draw ((-4.5*dh,h+4.5*dh)--(-4*dh,h+5*dh)--(2*dh,h+5*dh)--(2.5*dh,h+4.5*dh), linewidth(1));

label ("\bf Private repository", (7.5*dh,h+7*dh), N);
draw ((-4.5*dh,h+6.5*dh)--(-4*dh,h+7*dh)--(17*dh,h+7*dh)--(17.5*dh,h+6.5*dh), linewidth(1));

label ("Users'", (-dh,h+3*dh), N, blue);
label ("branch", (-dh,h+2*dh), N, blue);
label ("Developers'", (w,h+3*dh), N, blue);
label ("leading branch", (w,h+2*dh), N, blue);
label ("New-feature", (2*w+dh,h+3*dh), N, blue);
label ("branches", (2*w+dh,h+2*dh), N, blue);

dot((w, 2 + dh));
dot((w, 2 + 2*dh));
dot((w, 2 + 4*dh), red);

draw (shift(w,2*dh)*((0,2)--(w-dh,2)..(w,2+dh)--(w,2+2*dh)), blue+linewidth(lw), Arrow);
dot((2*w, 2 + 3*dh));
dot((2*w, 2 + 4*dh));
label ("\tt add-feature", (2*w,2+4*dh), NE, blue);


draw ((w,3+2*dh)--(2*w-dh,3+2*dh)..(2*w,3+3*dh)--(2*w,h), blue+linewidth(lw), Arrow);
dot((2*w, 3 + 3*dh));
dot((2*w, 3 + 4*dh));
dot((2*w, 3 + 6*dh));
label ("\tt add-feature2", (2*w,5), NE, blue);

draw (shift(w,2*dh)*((w,2+2*dh)--(0,2+2*dh)), red, Arrow);
label ("new-feature", (w*1.5,2+4*dh+0.17), N, red);
label ("merge", (w*1.5,2+4*dh), N, red);

dot((w, 3+2*dh));

dot((0, 3+5*dh));
dot((w, 3+5*dh), red);
// label ("bug fix", (-2*dh, 3+5*dh), W);
// draw ((-2*dh,3+5*dh)--(0,3+5*dh), Arrow);
draw ((0, 3+5*dh)--(w, 3+5*dh), red, Arrow);
label ("update merge", (w/2,3+5*dh), N, red);

dot((2*w, 3+5*dh), red);
draw ((w, 3+5*dh)--(2*w, 3+5*dh), red, Arrow);
label ("update merge", (3*w/2,3+5*dh), N, red);

dot((w, 3+7*dh));
dot((2*w, 3+7*dh), red);
draw ((w, 3+7*dh)--(2*w, 3+7*dh), red, Arrow);
label ("update merge", (3*w/2,3+7*dh), N, red);

// label ("improvements,", (3.25*w,2+h/4+0.17), E);
// label ("new features,", (3.25*w,2+h/4), E);
// label ("or local bug fixes", (3.25*w,2+h/4-0.17), E);
// draw ((3.25*w,2+h/4)--(w, 2 + dh), Arrow);
// draw ((3.25*w,2+h/4)--(w, 2 + 2*dh), Arrow);
// draw ((3.25*w,2+h/4)--(2*w, 2 + 3*dh), Arrow);
// draw ((3.25*w,2+h/4)--(2*w, 2 + 4*dh), Arrow);
