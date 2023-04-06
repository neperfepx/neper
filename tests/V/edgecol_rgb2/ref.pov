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

#declare edge1 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 0,                 1,                 0>,
              <                -0,    0.072876995892,                -0>,
              0.0118902753116875
    texture { edge1 }
  }
    sphere {<                 0,                 1,                 0>,
              0.0118902753116875
    texture { edge1 }
  }
    sphere {<                -0,    0.072876995892,                -0>,
              0.0118902753116875
    texture { edge1 }
  }
#declare edge2 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                -0,    0.072876995892,                -0>,
              <                -0,                 1,    0.523792986818>,
              0.0118902753116875
    texture { edge2 }
  }
    sphere {<                -0,    0.072876995892,                -0>,
              0.0118902753116875
    texture { edge2 }
  }
    sphere {<                -0,                 1,    0.523792986818>,
              0.0118902753116875
    texture { edge2 }
  }
#declare edge3 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                -0,                 1,    0.523792986818>,
              <                 0,                 1,                 0>,
              0.0118902753116875
    texture { edge3 }
  }
    sphere {<                -0,                 1,    0.523792986818>,
              0.0118902753116875
    texture { edge3 }
  }
    sphere {<                 0,                 1,                 0>,
              0.0118902753116875
    texture { edge3 }
  }
#declare edge4 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 0,                 0>,
              <                 1,                 1,                 0>,
              0.0118902753116875
    texture { edge4 }
  }
    sphere {<                 1,                 0,                 0>,
              0.0118902753116875
    texture { edge4 }
  }
    sphere {<                 1,                 1,                 0>,
              0.0118902753116875
    texture { edge4 }
  }
#declare edge5 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 1,                 0>,
              <                 1,                 1,    0.921109989703>,
              0.0118902753116875
    texture { edge5 }
  }
    sphere {<                 1,                 1,                 0>,
              0.0118902753116875
    texture { edge5 }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.0118902753116875
    texture { edge5 }
  }
#declare edge6 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 1,    0.921109989703>,
              <                 1,                -0,    0.356143977106>,
              0.0118902753116875
    texture { edge6 }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.0118902753116875
    texture { edge6 }
  }
    sphere {<                 1,                -0,    0.356143977106>,
              0.0118902753116875
    texture { edge6 }
  }
#declare edge7 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                -0,    0.356143977106>,
              <                 1,                 0,                 0>,
              0.0118902753116875
    texture { edge7 }
  }
    sphere {<                 1,                -0,    0.356143977106>,
              0.0118902753116875
    texture { edge7 }
  }
    sphere {<                 1,                 0,                 0>,
              0.0118902753116875
    texture { edge7 }
  }
#declare edge8 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                -0,    0.356143977106>,
              <    0.103627646137,                -0,                -0>,
              0.0118902753116875
    texture { edge8 }
  }
    sphere {<                 1,                -0,    0.356143977106>,
              0.0118902753116875
    texture { edge8 }
  }
    sphere {<    0.103627646137,                -0,                -0>,
              0.0118902753116875
    texture { edge8 }
  }
#declare edge9 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<    0.103627646137,                -0,                -0>,
              <                 1,                 0,                 0>,
              0.0118902753116875
    texture { edge9 }
  }
    sphere {<    0.103627646137,                -0,                -0>,
              0.0118902753116875
    texture { edge9 }
  }
    sphere {<                 1,                 0,                 0>,
              0.0118902753116875
    texture { edge9 }
  }
#declare edge10 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 1,                 0>,
              <                 0,                 1,                 0>,
              0.0118902753116875
    texture { edge10 }
  }
    sphere {<                 1,                 1,                 0>,
              0.0118902753116875
    texture { edge10 }
  }
    sphere {<                 0,                 1,                 0>,
              0.0118902753116875
    texture { edge10 }
  }
#declare edge11 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                -0,                 1,    0.523792986818>,
              <                 1,                 1,    0.921109989703>,
              0.0118902753116875
    texture { edge11 }
  }
    sphere {<                -0,                 1,    0.523792986818>,
              0.0118902753116875
    texture { edge11 }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.0118902753116875
    texture { edge11 }
  }
#declare edge12 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<    0.103627646137,                -0,                -0>,
              <                -0,    0.072876995892,                -0>,
              0.0118902753116875
    texture { edge12 }
  }
    sphere {<    0.103627646137,                -0,                -0>,
              0.0118902753116875
    texture { edge12 }
  }
    sphere {<                -0,    0.072876995892,                -0>,
              0.0118902753116875
    texture { edge12 }
  }
#declare edge13 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 0,                 0,                 0>,
              <                 0,                 0,                 1>,
              0.0118902753116875
    texture { edge13 }
  }
    sphere {<                 0,                 0,                 0>,
              0.0118902753116875
    texture { edge13 }
  }
    sphere {<                 0,                 0,                 1>,
              0.0118902753116875
    texture { edge13 }
  }
#declare edge14 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 0,                 0,                 1>,
              <                 0,                 1,                 1>,
              0.0118902753116875
    texture { edge14 }
  }
    sphere {<                 0,                 0,                 1>,
              0.0118902753116875
    texture { edge14 }
  }
    sphere {<                 0,                 1,                 1>,
              0.0118902753116875
    texture { edge14 }
  }
#declare edge15 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 0,                 1,                 1>,
              <                -0,                 1,    0.523792986818>,
              0.0118902753116875
    texture { edge15 }
  }
    sphere {<                 0,                 1,                 1>,
              0.0118902753116875
    texture { edge15 }
  }
    sphere {<                -0,                 1,    0.523792986818>,
              0.0118902753116875
    texture { edge15 }
  }
#declare edge16 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                -0,    0.072876995892,                -0>,
              <                 0,                 0,                 0>,
              0.0118902753116875
    texture { edge16 }
  }
    sphere {<                -0,    0.072876995892,                -0>,
              0.0118902753116875
    texture { edge16 }
  }
    sphere {<                 0,                 0,                 0>,
              0.0118902753116875
    texture { edge16 }
  }
#declare edge17 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 1,                 1>,
              <                 1,                 0,                 1>,
              0.0118902753116875
    texture { edge17 }
  }
    sphere {<                 1,                 1,                 1>,
              0.0118902753116875
    texture { edge17 }
  }
    sphere {<                 1,                 0,                 1>,
              0.0118902753116875
    texture { edge17 }
  }
#declare edge18 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 0,                 1>,
              <                 1,                -0,    0.356143977106>,
              0.0118902753116875
    texture { edge18 }
  }
    sphere {<                 1,                 0,                 1>,
              0.0118902753116875
    texture { edge18 }
  }
    sphere {<                 1,                -0,    0.356143977106>,
              0.0118902753116875
    texture { edge18 }
  }
#declare edge19 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 1,    0.921109989703>,
              <                 1,                 1,                 1>,
              0.0118902753116875
    texture { edge19 }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.0118902753116875
    texture { edge19 }
  }
    sphere {<                 1,                 1,                 1>,
              0.0118902753116875
    texture { edge19 }
  }
#declare edge20 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 0,                 1>,
              <                 0,                 0,                 1>,
              0.0118902753116875
    texture { edge20 }
  }
    sphere {<                 1,                 0,                 1>,
              0.0118902753116875
    texture { edge20 }
  }
    sphere {<                 0,                 0,                 1>,
              0.0118902753116875
    texture { edge20 }
  }
#declare edge21 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 0,                 0,                 0>,
              <    0.103627646137,                -0,                -0>,
              0.0118902753116875
    texture { edge21 }
  }
    sphere {<                 0,                 0,                 0>,
              0.0118902753116875
    texture { edge21 }
  }
    sphere {<    0.103627646137,                -0,                -0>,
              0.0118902753116875
    texture { edge21 }
  }
#declare edge22 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 0,                 1,                 1>,
              <                 1,                 1,                 1>,
              0.0118902753116875
    texture { edge22 }
  }
    sphere {<                 0,                 1,                 1>,
              0.0118902753116875
    texture { edge22 }
  }
    sphere {<                 1,                 1,                 1>,
              0.0118902753116875
    texture { edge22 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
