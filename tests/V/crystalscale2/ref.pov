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

#declare crystal1 =
  texture { pigment { rgbt <0.270588,0.219608,0.509804,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare crystal1edge =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.004924> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {< 0.575190123292765, 0.606939459016951, 0.265129978072642>,
               0.471839943273485
    texture { crystal1 }
  }
#declare crystal2 =
  texture { pigment { rgbt <0.129412,0.568627,0.549020,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare crystal2edge =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.004924> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {< 0.440917177254907, 0.415969214773683, 0.684555939875756>,
               0.511322091455207
    texture { crystal2 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
