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

#declare face1 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 1
triangle {<                 0,                 1,                 0>,
          <                -0,    0.072876995892,                -0>,
          <                -0,                 1,    0.523792986818>
            texture { face1 }
  }
#declare face2 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 2
triangle {<                 1,                 0,                 0>,
          <                 1,                 1,                 0>,
          <                 1,                 1,    0.921109989703>
            texture { face2 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <                 1,                -0,    0.356143977106>,
          <                 1,                 0,                 0>
            texture { face2 }
  }
#declare face3 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 3
triangle {<                 1,                 0,                 0>,
          <                 1,                -0,    0.356143977106>,
          <    0.103627646137,                -0,                -0>
            texture { face3 }
  }
#declare face4 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 4
triangle {<                 1,                 1,                 0>,
          <                 0,                 1,                 0>,
          <                -0,                 1,    0.523792986818>
            texture { face4 }
  }
triangle {<                -0,                 1,    0.523792986818>,
          <                 1,                 1,    0.921109989703>,
          <                 1,                 1,                 0>
            texture { face4 }
  }
#declare face5 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 5
triangle {<                 0,                 1,                 0>,
          <                 1,                 1,                 0>,
          <                 1,                 0,                 0>
            texture { face5 }
  }
triangle {<                 1,                 0,                 0>,
          <    0.103627646137,                -0,                -0>,
          <                -0,    0.072876995892,                -0>
            texture { face5 }
  }
triangle {<                 1,                 0,                 0>,
          <                -0,    0.072876995892,                -0>,
          <                 0,                 1,                 0>
            texture { face5 }
  }
#declare face6 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 6
triangle {<                -0,    0.072876995892,                -0>,
          <                -0,                 1,    0.523792986818>,
          <                 1,                 1,    0.921109989703>
            texture { face6 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <                 1,                -0,    0.356143977106>,
          <    0.103627646137,                -0,                -0>
            texture { face6 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <    0.103627646137,                -0,                -0>,
          <                -0,    0.072876995892,                -0>
            texture { face6 }
  }
#declare face7 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 7
triangle {<                 0,                 0,                 0>,
          <                 0,                 0,                 1>,
          <                 0,                 1,                 1>
            texture { face7 }
  }
triangle {<                 0,                 1,                 1>,
          <                -0,                 1,    0.523792986818>,
          <                -0,    0.072876995892,                -0>
            texture { face7 }
  }
triangle {<                 0,                 1,                 1>,
          <                -0,    0.072876995892,                -0>,
          <                 0,                 0,                 0>
            texture { face7 }
  }
#declare face8 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 8
triangle {<                 1,                 1,                 1>,
          <                 1,                 0,                 1>,
          <                 1,                -0,    0.356143977106>
            texture { face8 }
  }
triangle {<                 1,                -0,    0.356143977106>,
          <                 1,                 1,    0.921109989703>,
          <                 1,                 1,                 1>
            texture { face8 }
  }
#declare face9 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 9
triangle {<                 1,                 0,                 1>,
          <                 0,                 0,                 1>,
          <                 0,                 0,                 0>
            texture { face9 }
  }
triangle {<                 0,                 0,                 0>,
          <    0.103627646137,                -0,                -0>,
          <                 1,                -0,    0.356143977106>
            texture { face9 }
  }
triangle {<                 0,                 0,                 0>,
          <                 1,                -0,    0.356143977106>,
          <                 1,                 0,                 1>
            texture { face9 }
  }
#declare face10 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 10
triangle {<                 0,                 1,                 1>,
          <                 1,                 1,                 1>,
          <                 1,                 1,    0.921109989703>
            texture { face10 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <                -0,                 1,    0.523792986818>,
          <                 0,                 1,                 1>
            texture { face10 }
  }
#declare face11 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 11
triangle {<                 0,                 0,                 0>,
          <                -0,    0.072876995892,                -0>,
          <    0.103627646137,                -0,                -0>
            texture { face11 }
  }
#declare face12 =
  texture { pigment { rgbt <0.039216,0.078431,0.117647,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// face 12
triangle {<                 1,                 0,                 1>,
          <                 1,                 1,                 1>,
          <                 0,                 1,                 1>
            texture { face12 }
  }
triangle {<                 0,                 1,                 1>,
          <                 0,                 0,                 1>,
          <                 1,                 0,                 1>
            texture { face12 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
