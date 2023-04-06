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

#declare seed1 =
  texture { pigment { rgbt <0.000000,0.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.588079382832,    0.389046853444,    0.159391794127>,
              0.0336358566101486
    texture { seed1 }
  }
#declare seed2 =
  texture { pigment { rgbt <0.000000,0.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<    0.452028974213,    0.195589598124,    0.501814614272>,
              0.0336358566101486
    texture { seed2 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
