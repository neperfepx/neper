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

#declare ver1 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,                 0>,
              0.0168179283050743
    texture { ver1 }
  }
#declare ver2 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,                 0>,
              0.0168179283050743
    texture { ver2 }
  }
#declare ver3 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,                 0>,
              0.0168179283050743
    texture { ver3 }
  }
#declare ver4 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                -0,    0.072876995892,                -0>,
              0.0168179283050743
    texture { ver4 }
  }
#declare ver5 =
  texture { pigment { rgbt <0.388235,0.796078,0.372549,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                -0,                 1,    0.523792986818>,
              0.0168179283050743
    texture { ver5 }
  }
#declare ver6 =
  texture { pigment { rgbt <0.898039,0.894118,0.098039,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,    0.921109989703>,
              0.0168179283050743
    texture { ver6 }
  }
#declare ver7 =
  texture { pigment { rgbt <0.227451,0.729412,0.462745,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                -0,    0.356143977106>,
              0.0168179283050743
    texture { ver7 }
  }
#declare ver8 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.103627646137,                -0,                -0>,
              0.0168179283050743
    texture { ver8 }
  }
#declare ver9 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,                 0>,
              0.0168179283050743
    texture { ver9 }
  }
#declare ver10 =
  texture { pigment { rgbt <0.992157,0.905882,0.145098,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,                 1>,
              0.0168179283050743
    texture { ver10 }
  }
#declare ver11 =
  texture { pigment { rgbt <0.992157,0.905882,0.145098,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,                 1>,
              0.0168179283050743
    texture { ver11 }
  }
#declare ver12 =
  texture { pigment { rgbt <0.992157,0.905882,0.145098,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,                 1>,
              0.0168179283050743
    texture { ver12 }
  }
#declare ver13 =
  texture { pigment { rgbt <0.992157,0.905882,0.145098,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,                 1>,
              0.0168179283050743
    texture { ver13 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
