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
#declare node10 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,                 1>,
                            0.05
    texture { node10 }
  }
#declare node11 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,                 1>,
                            0.05
    texture { node11 }
  }
#declare node12 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,                 1>,
                            0.05
    texture { node12 }
  }
#declare node13 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,                 1>,
                            0.05
    texture { node13 }
  }
#declare node41 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.333333333333,                 1>,
                            0.05
    texture { node41 }
  }
#declare node42 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,    0.666666666667,                 1>,
                            0.05
    texture { node42 }
  }
#declare node44 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.875473586152,                 1>,
                            0.05
    texture { node44 }
  }
#declare node45 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.626420758457,                 1>,
                            0.05
    texture { node45 }
  }
#declare node46 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,    0.313210379228,                 1>,
                            0.05
    texture { node46 }
  }
#declare node48 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.666666666667,                 0,                 1>,
                            0.05
    texture { node48 }
  }
#declare node49 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.333333333333,                 0,                 1>,
                            0.05
    texture { node49 }
  }
#declare node50 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.313210379228,                 1,                 1>,
                            0.05
    texture { node50 }
  }
#declare node51 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.626420758457,                 1,                 1>,
                            0.05
    texture { node51 }
  }
#declare node52 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.875473586152,                 1,                 1>,
                            0.05
    texture { node52 }
  }
#declare node90 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<               0.2,    0.195975409179,                 1>,
                            0.05
    texture { node90 }
  }
#declare node91 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
                            0.05
    texture { node91 }
  }
#declare node92 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
                            0.05
    texture { node92 }
  }
#declare node93 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
                            0.05
    texture { node93 }
  }
#declare node94 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
                            0.05
    texture { node94 }
  }
#declare node95 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
                            0.05
    texture { node95 }
  }
#declare node96 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
                            0.05
    texture { node96 }
  }
#declare node97 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
                            0.05
    texture { node97 }
  }
#declare node98 =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
                            0.05
    texture { node98 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
