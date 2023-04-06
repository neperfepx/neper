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
filldraw (scale(scale)*((-0.133,-0.096)--(1.550,-0.096)--(1.550,1.000)--(-0.133,1.000)--cycle),white,white);
label("$[1\,0\,0]$", scale(scale)*O, S);
label("$[1\,1\,0]$", scale(scale)*X, S);
label("$[1\,1\,1]$", scale(scale/0.414214)*(0.366025,0.366025), N);
path line110111 = (0.414214,-0.000000)--(0.414176,0.010301)--(0.414063,0.020603)--(0.413876,0.030907)--(0.413613,0.041213)--(0.413275,0.051522)--(0.412861,0.061835)--(0.412372,0.072152)--(0.411807,0.082475)--(0.411165,0.092804)--(0.410447,0.103140)--(0.409653,0.113484)--(0.408781,0.123836)--(0.407832,0.134198)--(0.406805,0.144569)--(0.405699,0.154951)--(0.404514,0.165345)--(0.403250,0.175751)--(0.401906,0.186171)--(0.400481,0.196604)--(0.398975,0.207051)--(0.397386,0.217514)--(0.395714,0.227993)--(0.393959,0.238489)--(0.392120,0.249003)--(0.390195,0.259535)--(0.388184,0.270086)--(0.386085,0.280657)--(0.383898,0.291248)--(0.381622,0.301861)--(0.379256,0.312496)--(0.376798,0.323154)--(0.374247,0.333835)--(0.371602,0.344540)--(0.368862,0.355270)--(0.366025,0.366025);
path border = (0,0)--line110111--cycle;
draw (shift(O)*scale(scale/0.414214)*border,black);
int n0 = 52;
int n1 = 52;
int n = n0*n1;
pair[]points = new pair[n];
real[]values = new real[n];

file in = (input ("test.level")).line();
real[][]tmp = in.dimension(0, 0);
for (int i = 0; i < n; ++i)
{
  points[i] = scale*(tmp[i][0], tmp[i][1]);
  values[i] = tmp[i][2];
}

real[][] v=new real[n0][n1];
int id = 0;
for (int i = 0; i < n0; ++i)
  for (int j = 0; j < n1; ++j)
  {
    v[i][j] = values[id];
    ++id;
  }

real[][] vt=new real[n0][n1];
for (int i = 0; i < n0; ++i)
  for (int j = 0; j < n1; ++j)
  {
    vt[j][i] = v[i][j];
  }

pen[] Palette=Gradient(rgb(0.933333,0.933333,0.933333),rgb(0.925490,0.933333,0.917647),rgb(0.921569,0.929412,0.901961),rgb(0.913725,0.929412,0.886275),rgb(0.909804,0.925490,0.870588),rgb(0.901961,0.925490,0.854902),rgb(0.894118,0.925490,0.839216),rgb(0.890196,0.921569,0.823529),rgb(0.882353,0.921569,0.807843),rgb(0.874510,0.917647,0.792157),rgb(0.870588,0.917647,0.776471),rgb(0.862745,0.917647,0.760784),rgb(0.858824,0.913725,0.745098),rgb(0.850980,0.913725,0.729412),rgb(0.843137,0.909804,0.713725),rgb(0.839216,0.909804,0.698039),rgb(0.831373,0.909804,0.682353),rgb(0.827451,0.905882,0.666667),rgb(0.819608,0.905882,0.650980),rgb(0.811765,0.901961,0.635294),rgb(0.807843,0.901961,0.619608),rgb(0.800000,0.901961,0.603922),rgb(0.792157,0.898039,0.588235),rgb(0.788235,0.898039,0.572549),rgb(0.780392,0.894118,0.556863),rgb(0.772549,0.894118,0.545098),rgb(0.760784,0.890196,0.541176),rgb(0.749020,0.886275,0.533333),rgb(0.737255,0.878431,0.529412),rgb(0.725490,0.874510,0.525490),rgb(0.713725,0.870588,0.521569),rgb(0.701961,0.866667,0.517647),rgb(0.690196,0.862745,0.513725),rgb(0.678431,0.858824,0.509804),rgb(0.666667,0.854902,0.505882),rgb(0.654902,0.850980,0.498039),rgb(0.643137,0.847059,0.494118),rgb(0.631373,0.843137,0.490196),rgb(0.619608,0.839216,0.486275),rgb(0.607843,0.831373,0.482353),rgb(0.596078,0.827451,0.478431),rgb(0.584314,0.823529,0.474510),rgb(0.572549,0.819608,0.466667),rgb(0.560784,0.815686,0.462745),rgb(0.549020,0.811765,0.458824),rgb(0.537255,0.807843,0.454902),rgb(0.525490,0.803922,0.450980),rgb(0.513725,0.800000,0.447059),rgb(0.501961,0.796078,0.443137),rgb(0.490196,0.792157,0.439216),rgb(0.474510,0.784314,0.431373),rgb(0.462745,0.776471,0.423529),rgb(0.447059,0.768627,0.415686),rgb(0.435294,0.760784,0.407843),rgb(0.419608,0.752941,0.400000),rgb(0.403922,0.749020,0.388235),rgb(0.392157,0.741176,0.380392),rgb(0.376471,0.733333,0.372549),rgb(0.364706,0.725490,0.364706),rgb(0.349020,0.717647,0.356863),rgb(0.337255,0.709804,0.349020),rgb(0.321569,0.701961,0.341176),rgb(0.309804,0.694118,0.333333),rgb(0.294118,0.686275,0.325490),rgb(0.278431,0.678431,0.317647),rgb(0.266667,0.674510,0.305882),rgb(0.250980,0.666667,0.298039),rgb(0.239216,0.658824,0.290196),rgb(0.223529,0.650980,0.282353),rgb(0.211765,0.643137,0.274510),rgb(0.196078,0.635294,0.266667),rgb(0.184314,0.627451,0.258824),rgb(0.168627,0.619608,0.250980),rgb(0.152941,0.611765,0.243137),rgb(0.141176,0.603922,0.235294),rgb(0.137255,0.596078,0.227451),rgb(0.133333,0.588235,0.227451),rgb(0.133333,0.580392,0.223529),rgb(0.129412,0.568627,0.219608),rgb(0.129412,0.560784,0.215686),rgb(0.129412,0.552941,0.211765),rgb(0.125490,0.541176,0.211765),rgb(0.125490,0.533333,0.207843),rgb(0.121569,0.525490,0.203922),rgb(0.121569,0.517647,0.200000),rgb(0.121569,0.505882,0.196078),rgb(0.117647,0.498039,0.196078),rgb(0.117647,0.490196,0.192157),rgb(0.113725,0.478431,0.188235),rgb(0.113725,0.470588,0.184314),rgb(0.113725,0.462745,0.180392),rgb(0.109804,0.450980,0.176471),rgb(0.109804,0.443137,0.176471),rgb(0.105882,0.435294,0.172549),rgb(0.105882,0.427451,0.168627),rgb(0.105882,0.415686,0.164706),rgb(0.101961,0.407843,0.160784),rgb(0.101961,0.400000,0.160784),rgb(0.098039,0.388235,0.156863),rgb(0.098039,0.380392,0.152941));
bounds range=image(v,scale*(-0.020000,-0.017673),scale*(1.020000,0.901337),Palette);
picture bar;
palette(bar,"Density",range,(0cm,0cm),(0.5cm,6cm),Right,Palette,
        PaletteTicks(N=10,n=1,"$%.1f$"));
add(bar.fit(),point(E),W);
draw (contour (scale(2.414214)*points, values, new real[] {1.54928,3.09855,4.64783,6.1971,7.74638,9.29566,10.8449,12.3942,13.9435,15.4928,}, operator --), new pen[] {black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000)});
filldraw(shift(O)*scale(scale/0.414214)*(scale(1+1e-3)*line110111--(0.000000,0.000000)--(0.444214,0.000000)--(0.444214,-0.030000)--(-0.030000,-0.030000)--(-0.030000,0.396025)--(0.444214,0.396025)--(0.444214,0.000000)--cycle), evenodd+white+linewidth(3pt), white);
draw (scale(scale/0.414214)*border, black);
label("Z direction", scale(scale)*(-0.083, 0.900), NE);
label("stereo. proj.", scale(scale)*(-0.083, 0.900), SE, fontsize(8));
