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

#declare pointedge =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare point1 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 0,                 0>,
                            0.02
    texture { point1 }
  }
#declare point2 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 1,                 0>,
                            0.02
    texture { point2 }
  }
#declare point3 =
  texture { pigment { rgbt <0.000000,0.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 0,                 0,                 1>,
                            0.02
    texture { point3 }
  }
#declare pointedge =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare point1 =
  texture { pigment { rgbt <1.000000,0.752941,0.796078,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<               0.5,               0.5,               0.5>,
                            0.02
    texture { point1 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
