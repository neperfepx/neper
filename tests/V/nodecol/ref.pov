#version 3.7;
#include "shapes.inc"
background {color rgb<1.000000,1.000000,1.000000>}
camera {
perspective
right x*image_width/image_height
location <3.962000, 4.827000, -5.270000>
look_at <0.500000, 0.500000, 0.500000>
angle 25.000000
sky <0.000000, 1.000000, 0.000000>
}
light_source { <3.962000, 4.827000, -5.270000> rgb<1.000000, 1.000000, 1.000000> shadowless}

global_settings { assumed_gamma 2.2 }
union {

#declare elt3dedge =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare elt3dedge =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare node1 =
  texture { pigment { rgb <1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,                 0>,
              0.00533952670350487
    texture { node1 }
  }
#declare node2 =
  texture { pigment { rgb <0.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,                 0>,
              0.00533952670350487
    texture { node2 }
  }
#declare node3 =
  texture { pigment { rgb <0.000000,0.000000,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,                 0>,
              0.00533952670350487
    texture { node3 }
  }
#declare node4 =
  texture { pigment { rgb <1.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00533952670350487
    texture { node4 }
  }
#declare node5 =
  texture { pigment { rgb <1.000000,0.000000,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00533952670350487
    texture { node5 }
  }
#declare node6 =
  texture { pigment { rgb <0.000000,1.000000,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00533952670350487
    texture { node6 }
  }
#declare node7 =
  texture { pigment { rgb <0.498039,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00533952670350487
    texture { node7 }
  }
#declare node8 =
  texture { pigment { rgb <0.000000,1.000000,0.498039> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00533952670350487
    texture { node8 }
  }
#declare node9 =
  texture { pigment { rgb <0.501961,0.501961,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,                 0>,
              0.00533952670350487
    texture { node9 }
  }
#declare node10 =
  texture { pigment { rgb <0.501961,0.000000,0.501961> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,                 1>,
              0.00533952670350487
    texture { node10 }
  }
#declare node11 =
  texture { pigment { rgb <0.000000,0.501961,0.501961> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,                 1>,
              0.00533952670350487
    texture { node11 }
  }
#declare node12 =
  texture { pigment { rgb <0.501961,0.501961,0.501961> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,                 1>,
              0.00533952670350487
    texture { node12 }
  }
#declare node13 =
  texture { pigment { rgb <0.000000,0.749020,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,                 1>,
              0.00533952670350487
    texture { node13 }
  }
#declare node14 =
  texture { pigment { rgb <0.486275,0.988235,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00533952670350487
    texture { node14 }
  }
#declare node15 =
  texture { pigment { rgb <0.250980,0.250980,0.250980> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00533952670350487
    texture { node15 }
  }
#declare node16 =
  texture { pigment { rgb <1.000000,0.270588,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.182654408751,                 0>,
              0.00533952670350487
    texture { node16 }
  }
#declare node17 =
  texture { pigment { rgb <0.752941,0.752941,0.752941> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00533952670350487
    texture { node17 }
  }
#declare node18 =
  texture { pigment { rgb <1.000000,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00533952670350487
    texture { node18 }
  }
#declare node19 =
  texture { pigment { rgb <0.000000,0.000000,0.803922> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00533952670350487
    texture { node19 }
  }
#declare node20 =
  texture { pigment { rgb <0.294118,0.000000,0.509804> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,    0.261896493409>,
              0.00533952670350487
    texture { node20 }
  }
#declare node21 =
  texture { pigment { rgb <0.941176,0.501961,0.501961> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.333333333333,                 0>,
              0.00533952670350487
    texture { node21 }
  }
#declare node22 =
  texture { pigment { rgb <1.000000,0.498039,0.313725> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00533952670350487
    texture { node22 }
  }
#declare node23 =
  texture { pigment { rgb <0.980392,0.501961,0.447059> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00533952670350487
    texture { node23 }
  }
#declare node24 =
  texture { pigment { rgb <0.498039,1.000000,0.831373> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00533952670350487
    texture { node24 }
  }
#declare node25 =
  texture { pigment { rgb <1.000000,0.843137,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,    0.806407465926>,
              0.00533952670350487
    texture { node25 }
  }
#declare node26 =
  texture { pigment { rgb <1.000000,0.647059,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00533952670350487
    texture { node26 }
  }
#declare node27 =
  texture { pigment { rgb <0.545098,0.000000,0.545098> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00533952670350487
    texture { node27 }
  }
#declare node28 =
  texture { pigment { rgb <0.000000,0.545098,0.545098> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00533952670350487
    texture { node28 }
  }
#declare node29 =
  texture { pigment { rgb <0.803922,0.521569,0.247059> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00533952670350487
    texture { node29 }
  }
#declare node30 =
  texture { pigment { rgb <0.274510,0.509804,0.705882> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00533952670350487
    texture { node30 }
  }
#declare node31 =
  texture { pigment { rgb <0.000000,0.980392,0.603922> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.289228159963,                 0,                 0>,
              0.00533952670350487
    texture { node31 }
  }
#declare node32 =
  texture { pigment { rgb <0.282353,0.239216,0.545098> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00533952670350487
    texture { node32 }
  }
#declare node33 =
  texture { pigment { rgb <0.721569,0.525490,0.043137> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00533952670350487
    texture { node33 }
  }
#declare node34 =
  texture { pigment { rgb <1.000000,0.627451,0.478431> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00533952670350487
    texture { node34 }
  }
#declare node35 =
  texture { pigment { rgb <0.529412,0.807843,0.980392> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00533952670350487
    texture { node35 }
  }
#declare node36 =
  texture { pigment { rgb <1.000000,0.388235,0.278431> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00533952670350487
    texture { node36 }
  }
#declare node37 =
  texture { pigment { rgb <0.439216,0.501961,0.564706> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00533952670350487
    texture { node37 }
  }
#declare node38 =
  texture { pigment { rgb <1.000000,0.411765,0.705882> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00533952670350487
    texture { node38 }
  }
#declare node39 =
  texture { pigment { rgb <0.741176,0.717647,0.419608> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00533952670350487
    texture { node39 }
  }
#declare node40 =
  texture { pigment { rgb <0.000000,0.807843,0.819608> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,    0.677609787007>,
              0.00533952670350487
    texture { node40 }
  }
#declare node41 =
  texture { pigment { rgb <0.235294,0.701961,0.443137> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00533952670350487
    texture { node41 }
  }
#declare node42 =
  texture { pigment { rgb <0.780392,0.082353,0.521569> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00533952670350487
    texture { node42 }
  }
#declare node43 =
  texture { pigment { rgb <0.933333,0.509804,0.933333> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,    0.761896493409>,
              0.00533952670350487
    texture { node43 }
  }
#declare node44 =
  texture { pigment { rgb <0.678431,1.000000,0.184314> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00533952670350487
    texture { node44 }
  }
#declare node45 =
  texture { pigment { rgb <0.560784,0.737255,0.560784> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00533952670350487
    texture { node45 }
  }
#declare node46 =
  texture { pigment { rgb <0.737255,0.560784,0.560784> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.313210379228,                 1>,
              0.00533952670350487
    texture { node46 }
  }
#declare node47 =
  texture { pigment { rgb <1.000000,0.078431,0.576471> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,    0.678071988553>,
              0.00533952670350487
    texture { node47 }
  }
#declare node48 =
  texture { pigment { rgb <0.545098,0.270588,0.074510> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00533952670350487
    texture { node48 }
  }
#declare node49 =
  texture { pigment { rgb <0.580392,0.000000,0.827451> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00533952670350487
    texture { node49 }
  }
#declare node50 =
  texture { pigment { rgb <0.117647,0.564706,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00533952670350487
    texture { node50 }
  }
#declare node51 =
  texture { pigment { rgb <0.466667,0.533333,0.600000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00533952670350487
    texture { node51 }
  }
#declare node52 =
  texture { pigment { rgb <0.870588,0.721569,0.529412> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00533952670350487
    texture { node52 }
  }
#declare node53 =
  texture { pigment { rgb <0.482353,0.407843,0.933333> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.850552308617,    0.130948246705>,
              0.00533952670350487
    texture { node53 }
  }
#declare node54 =
  texture { pigment { rgb <0.250980,0.878431,0.815686> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00533952670350487
    texture { node54 }
  }
#declare node55 =
  texture { pigment { rgb <0.529412,0.807843,0.921569> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.776565927567,    0.184454943986>,
              0.00533952670350487
    texture { node55 }
  }
#declare node56 =
  texture { pigment { rgb <0.282353,0.819608,0.800000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00533952670350487
    texture { node56 }
  }
#declare node57 =
  texture { pigment { rgb <0.823529,0.705882,0.549020> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.219942911644,    0.224539875377>,
              0.00533952670350487
    texture { node57 }
  }
#declare node58 =
  texture { pigment { rgb <0.196078,0.803922,0.196078> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00533952670350487
    texture { node58 }
  }
#declare node59 =
  texture { pigment { rgb <0.913725,0.588235,0.478431> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.764143265934,                 1,     0.19846713322>,
              0.00533952670350487
    texture { node59 }
  }
#declare node60 =
  texture { pigment { rgb <0.690196,0.768627,0.870588> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00533952670350487
    texture { node60 }
  }
#declare node61 =
  texture { pigment { rgb <0.254902,0.411765,0.882353> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00533952670350487
    texture { node61 }
  }
#declare node62 =
  texture { pigment { rgb <0.596078,0.984314,0.596078> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00533952670350487
    texture { node62 }
  }
#declare node63 =
  texture { pigment { rgb <0.862745,0.078431,0.235294> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00533952670350487
    texture { node63 }
  }
#declare node64 =
  texture { pigment { rgb <0.729412,0.333333,0.827451> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.513631647844,    0.532352374883,                 0>,
              0.00533952670350487
    texture { node64 }
  }
#declare node65 =
  texture { pigment { rgb <0.941176,0.901961,0.549020> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00533952670350487
    texture { node65 }
  }
#declare node66 =
  texture { pigment { rgb <0.564706,0.933333,0.564706> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00533952670350487
    texture { node66 }
  }
#declare node67 =
  texture { pigment { rgb <0.184314,0.309804,0.309804> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00533952670350487
    texture { node67 }
  }
#declare node68 =
  texture { pigment { rgb <0.600000,0.196078,0.800000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.191361619853,    0.787344828536,                 0>,
              0.00533952670350487
    texture { node68 }
  }
#declare node69 =
  texture { pigment { rgb <0.180392,0.545098,0.341176> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00533952670350487
    texture { node69 }
  }
#declare node70 =
  texture { pigment { rgb <0.603922,0.803922,0.196078> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.552313547477,    0.277861475044,    0.335252827167>,
              0.00533952670350487
    texture { node70 }
  }
#declare node71 =
  texture { pigment { rgb <0.541176,0.168627,0.886275> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.756556939182,    0.422670331064,    0.498214281393>,
              0.00533952670350487
    texture { node71 }
  }
#declare node72 =
  texture { pigment { rgb <0.858824,0.439216,0.576471> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.789385525492,      0.2002511586,    0.385598363923>,
              0.00533952670350487
    texture { node72 }
  }
#declare node73 =
  texture { pigment { rgb <0.419608,0.556863,0.137255> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.501108777675,    0.482732980899,    0.430653739253>,
              0.00533952670350487
    texture { node73 }
  }
#declare node74 =
  texture { pigment { rgb <0.576471,0.439216,0.858824> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.290087600133,    0.237816532566,    0.208441968213>,
              0.00533952670350487
    texture { node74 }
  }
#declare node75 =
  texture { pigment { rgb <0.956863,0.643137,0.376471> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.777952938043,    0.815811291707,    0.728826556457>,
              0.00533952670350487
    texture { node75 }
  }
#declare node76 =
  texture { pigment { rgb <0.333333,0.419608,0.184314> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.696533782562,    0.635545230496,    0.594633143815>,
              0.00533952670350487
    texture { node76 }
  }
#declare node77 =
  texture { pigment { rgb <0.400000,0.803922,0.666667> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.241871295608,    0.565076141084,    0.374175366718>,
              0.00533952670350487
    texture { node77 }
  }
#declare node78 =
  texture { pigment { rgb <0.415686,0.352941,0.803922> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.469289501086,    0.757428175138,    0.573204848214>,
              0.00533952670350487
    texture { node78 }
  }
#declare node79 =
  texture { pigment { rgb <0.133333,0.545098,0.133333> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.187478082306,    0.802844625036,    0.486895130531>,
              0.00533952670350487
    texture { node79 }
  }
#declare node80 =
  texture { pigment { rgb <0.098039,0.098039,0.439216> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00533952670350487
    texture { node80 }
  }
#declare node81 =
  texture { pigment { rgb <0.125490,0.698039,0.666667> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00533952670350487
    texture { node81 }
  }
#declare node82 =
  texture { pigment { rgb <0.854902,0.439216,0.839216> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.364272566544,    0.474425381623>,
              0.00533952670350487
    texture { node82 }
  }
#declare node83 =
  texture { pigment { rgb <0.392157,0.584314,0.929412> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00533952670350487
    texture { node83 }
  }
#declare node84 =
  texture { pigment { rgb <0.627451,0.321569,0.176471> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.250568303383,    0.784243919987>,
              0.00533952670350487
    texture { node84 }
  }
#declare node85 =
  texture { pigment { rgb <0.698039,0.133333,0.133333> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00533952670350487
    texture { node85 }
  }
#declare node86 =
  texture { pigment { rgb <0.803922,0.360784,0.360784> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00533952670350487
    texture { node86 }
  }
#declare node87 =
  texture { pigment { rgb <0.411765,0.411765,0.411765> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<     0.21833400396,                 0,    0.765928426277>,
              0.00533952670350487
    texture { node87 }
  }
#declare node88 =
  texture { pigment { rgb <0.823529,0.411765,0.117647> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00533952670350487
    texture { node88 }
  }
#declare node89 =
  texture { pigment { rgb <0.647059,0.164706,0.164706> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.156605189614,                 1,    0.852533322344>,
              0.00533952670350487
    texture { node89 }
  }
#declare node90 =
  texture { pigment { rgb <0.854902,0.647059,0.125490> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<               0.2,    0.195975409179,                 1>,
              0.00533952670350487
    texture { node90 }
  }
#declare node91 =
  texture { pigment { rgb <0.866667,0.627451,0.866667> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00533952670350487
    texture { node91 }
  }
#declare node92 =
  texture { pigment { rgb <0.372549,0.619608,0.627451> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00533952670350487
    texture { node92 }
  }
#declare node93 =
  texture { pigment { rgb <1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
              0.00533952670350487
    texture { node93 }
  }
#declare node94 =
  texture { pigment { rgb <0.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00533952670350487
    texture { node94 }
  }
#declare node95 =
  texture { pigment { rgb <0.000000,0.000000,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
              0.00533952670350487
    texture { node95 }
  }
#declare node96 =
  texture { pigment { rgb <1.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00533952670350487
    texture { node96 }
  }
#declare node97 =
  texture { pigment { rgb <1.000000,0.000000,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
              0.00533952670350487
    texture { node97 }
  }
#declare node98 =
  texture { pigment { rgb <0.000000,1.000000,1.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00533952670350487
    texture { node98 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
