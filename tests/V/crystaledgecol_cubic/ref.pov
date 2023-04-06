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
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare crystal1edge =
  texture { pigment { rgbt <0.647059,0.164706,0.164706,0.004924> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    box {<-.5,-.5,-.5>, <.5,.5,.5> scale  0.760602192140445 matrix <-0.0919661180482978, 0.171611357975663,-0.980862770699793,-0.565166903473556,-0.820000215341766,-0.0904766160815561, -0.81983449814535, 0.546030391693727, 0.172401296391748,0.,0.,0.> translate < 0.575190123292765, 0.606939459016951, 0.265129978072642>
    texture { crystal1 }
  }
union
{
    cylinder {<              -0.5,              -0.5,              -0.5>,
              <               0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<               0.5,              -0.5,              -0.5>,
              <               0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<               0.5,               0.5,              -0.5>,
              <              -0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<              -0.5,               0.5,              -0.5>,
              <              -0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<              -0.5,              -0.5,               0.5>,
              <               0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<               0.5,              -0.5,               0.5>,
              <               0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<               0.5,               0.5,               0.5>,
              <              -0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<              -0.5,               0.5,               0.5>,
              <              -0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<              -0.5,              -0.5,              -0.5>,
              <              -0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<               0.5,              -0.5,              -0.5>,
              <               0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,              -0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,              -0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<               0.5,               0.5,              -0.5>,
              <               0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<               0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    cylinder {<              -0.5,               0.5,              -0.5>,
              <              -0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,               0.5,              -0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    sphere {<              -0.5,               0.5,               0.5>,
              0.00647345637455685
    texture { crystal1edge }
  }
    scale  0.760602192140445 matrix <-0.0919661180482978, 0.171611357975663,-0.980862770699793,-0.565166903473556,-0.820000215341766,-0.0904766160815561, -0.81983449814535, 0.546030391693727, 0.172401296391748,0.,0.,0.> translate < 0.575190123292765, 0.606939459016951, 0.265129978072642>
}
#declare crystal2 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
#declare crystal2edge =
  texture { pigment { rgbt <0.647059,0.164706,0.164706,0.004924> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    box {<-.5,-.5,-.5>, <.5,.5,.5> scale  0.824247097336667 matrix < 0.310140696645615,0.0165345356374687,-0.950546872813448, 0.690776641998726, 0.683032150326754, 0.237265067991424, 0.653177142246372,-0.730201130340993, 0.200414395929033,0.,0.,0.> translate < 0.440917177254907, 0.415969214773683, 0.684555939875756>
    texture { crystal2 }
  }
union
{
    cylinder {<              -0.5,              -0.5,              -0.5>,
              <               0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<               0.5,              -0.5,              -0.5>,
              <               0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<               0.5,               0.5,              -0.5>,
              <              -0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<              -0.5,               0.5,              -0.5>,
              <              -0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<              -0.5,              -0.5,               0.5>,
              <               0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<               0.5,              -0.5,               0.5>,
              <               0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<               0.5,               0.5,               0.5>,
              <              -0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<              -0.5,               0.5,               0.5>,
              <              -0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<              -0.5,              -0.5,              -0.5>,
              <              -0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<               0.5,              -0.5,              -0.5>,
              <               0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,              -0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,              -0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<               0.5,               0.5,              -0.5>,
              <               0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<               0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    cylinder {<              -0.5,               0.5,              -0.5>,
              <              -0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,               0.5,              -0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    sphere {<              -0.5,               0.5,               0.5>,
              0.0059736032133121
    texture { crystal2edge }
  }
    scale  0.824247097336667 matrix < 0.310140696645615,0.0165345356374687,-0.950546872813448, 0.690776641998726, 0.683032150326754, 0.237265067991424, 0.653177142246372,-0.730201130340993, 0.200414395929033,0.,0.,0.> translate < 0.440917177254907, 0.415969214773683, 0.684555939875756>
}
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
