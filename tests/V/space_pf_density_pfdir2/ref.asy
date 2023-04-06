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
filldraw (scale(scale)*((-1.265,-1.192)--(2.100,-1.192)--(2.100,1.192)--(-1.265,1.192)--cycle),white,white);
label("$X_3$", scale(scale)*(1,0), E);
label("$X_2$", scale(scale)*(0,1), N);
draw (shift(O)*scale(scale)*unitcircle, black);
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

pen[] Palette=Gradient(rgb(0.266667,0.003922,0.329412),rgb(0.270588,0.019608,0.349020),rgb(0.274510,0.031373,0.360784),rgb(0.278431,0.050980,0.376471),rgb(0.278431,0.062745,0.388235),rgb(0.282353,0.078431,0.403922),rgb(0.282353,0.090196,0.411765),rgb(0.282353,0.105882,0.427451),rgb(0.282353,0.121569,0.439216),rgb(0.282353,0.129412,0.450980),rgb(0.282353,0.145098,0.462745),rgb(0.282353,0.156863,0.470588),rgb(0.278431,0.172549,0.478431),rgb(0.278431,0.180392,0.486275),rgb(0.274510,0.196078,0.494118),rgb(0.270588,0.207843,0.505882),rgb(0.270588,0.219608,0.509804),rgb(0.266667,0.231373,0.517647),rgb(0.262745,0.243137,0.521569),rgb(0.258824,0.254902,0.525490),rgb(0.250980,0.270588,0.533333),rgb(0.247059,0.278431,0.533333),rgb(0.243137,0.290196,0.537255),rgb(0.239216,0.301961,0.541176),rgb(0.235294,0.313725,0.545098),rgb(0.231373,0.321569,0.545098),rgb(0.223529,0.333333,0.549020),rgb(0.219608,0.349020,0.549020),rgb(0.215686,0.356863,0.552941),rgb(0.207843,0.368627,0.552941),rgb(0.203922,0.376471,0.552941),rgb(0.200000,0.388235,0.552941),rgb(0.196078,0.396078,0.556863),rgb(0.192157,0.407843,0.556863),rgb(0.184314,0.419608,0.556863),rgb(0.180392,0.427451,0.556863),rgb(0.176471,0.439216,0.556863),rgb(0.172549,0.443137,0.556863),rgb(0.168627,0.454902,0.556863),rgb(0.164706,0.462745,0.556863),rgb(0.160784,0.474510,0.556863),rgb(0.156863,0.486275,0.556863),rgb(0.152941,0.494118,0.556863),rgb(0.149020,0.505882,0.556863),rgb(0.149020,0.509804,0.556863),rgb(0.145098,0.521569,0.556863),rgb(0.141176,0.529412,0.556863),rgb(0.137255,0.541176,0.552941),rgb(0.133333,0.552941,0.552941),rgb(0.129412,0.560784,0.552941),rgb(0.125490,0.572549,0.549020),rgb(0.125490,0.576471,0.549020),rgb(0.121569,0.588235,0.545098),rgb(0.121569,0.600000,0.541176),rgb(0.117647,0.607843,0.541176),rgb(0.121569,0.619608,0.537255),rgb(0.121569,0.627451,0.533333),rgb(0.121569,0.635294,0.529412),rgb(0.125490,0.643137,0.525490),rgb(0.133333,0.654902,0.521569),rgb(0.141176,0.666667,0.513725),rgb(0.145098,0.674510,0.509804),rgb(0.156863,0.682353,0.501961),rgb(0.164706,0.690196,0.498039),rgb(0.180392,0.701961,0.486275),rgb(0.192157,0.709804,0.482353),rgb(0.207843,0.717647,0.474510),rgb(0.227451,0.729412,0.462745),rgb(0.239216,0.737255,0.454902),rgb(0.258824,0.745098,0.443137),rgb(0.274510,0.752941,0.435294),rgb(0.298039,0.760784,0.423529),rgb(0.313725,0.768627,0.415686),rgb(0.337255,0.776471,0.403922),rgb(0.360784,0.784314,0.388235),rgb(0.376471,0.792157,0.376471),rgb(0.403922,0.800000,0.360784),rgb(0.423529,0.803922,0.352941),rgb(0.450980,0.815686,0.337255),rgb(0.466667,0.819608,0.325490),rgb(0.498039,0.827451,0.305882),rgb(0.525490,0.835294,0.286275),rgb(0.545098,0.839216,0.274510),rgb(0.576471,0.843137,0.254902),rgb(0.596078,0.847059,0.243137),rgb(0.627451,0.854902,0.223529),rgb(0.658824,0.858824,0.203922),rgb(0.678431,0.862745,0.188235),rgb(0.709804,0.870588,0.168627),rgb(0.729412,0.870588,0.156863),rgb(0.760784,0.874510,0.137255),rgb(0.784314,0.878431,0.125490),rgb(0.815686,0.882353,0.109804),rgb(0.847059,0.886275,0.098039),rgb(0.866667,0.890196,0.094118),rgb(0.898039,0.894118,0.098039),rgb(0.917647,0.898039,0.101961),rgb(0.945098,0.898039,0.113725),rgb(0.964706,0.901961,0.125490),rgb(0.992157,0.905882,0.145098));
bounds range=image(v,scale*1.040000*(-1,-1),scale*1.040000*(1,1),Palette);
picture bar;
palette(bar,"Density",range,(0cm,0cm),(0.5cm,6cm),Right,Palette,
        PaletteTicks(N=10,n=1,"$%.1f$"));
add(bar.fit(),point(E),W+0.000000S);
draw (contour (points, values, new real[] {5.08781,10.1756,15.2634,20.3512,25.439,30.5268,35.6146,40.7024,45.7903,50.8781,}, operator --), new pen[] {black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000),black+linewidth(0.500000)});
draw (scale(scale)*(-X--X), black);
draw (scale(scale)*(-Y--Y), black);
path g = shift(O)*scale(scale*1.15)*((-X-Y)--(-X+Y)--(X+Y)--(X-Y)--cycle);
filldraw(shift(O)*scale(scale*1.00)*unitcircle^^g, evenodd+white, white);
draw (shift(O)*scale(scale)*unitcircle, black);
label("$\left\{111\right\}$", scale(scale)*(-1.265, 1.000), NE);
label("stereo. proj.", scale(scale)*(-1.255, 1.000), SE, fontsize(8));
