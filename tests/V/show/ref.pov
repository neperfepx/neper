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

#declare grainface0 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 1
triangle {<                 0,                 1,                 0>,
          <                -0,    0.072876995892,                -0>,
          <                -0,                 1,    0.523792986818>
            texture { grainface0 }
  }
#declare grainface1 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 2
triangle {<                 1,                 0,                 0>,
          <                 1,                 1,                 0>,
          <                 1,                 1,    0.921109989703>
            texture { grainface1 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <                 1,                -0,    0.356143977106>,
          <                 1,                 0,                 0>
            texture { grainface1 }
  }
#declare grainface2 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 3
triangle {<                 1,                 0,                 0>,
          <                 1,                -0,    0.356143977106>,
          <    0.103627646137,                -0,                -0>
            texture { grainface2 }
  }
#declare grainface3 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 4
triangle {<                 1,                 1,                 0>,
          <                 0,                 1,                 0>,
          <                -0,                 1,    0.523792986818>
            texture { grainface3 }
  }
triangle {<                -0,                 1,    0.523792986818>,
          <                 1,                 1,    0.921109989703>,
          <                 1,                 1,                 0>
            texture { grainface3 }
  }
#declare grainface4 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 5
triangle {<                 0,                 1,                 0>,
          <                 1,                 1,                 0>,
          <                 1,                 0,                 0>
            texture { grainface4 }
  }
triangle {<                 1,                 0,                 0>,
          <    0.103627646137,                -0,                -0>,
          <                -0,    0.072876995892,                -0>
            texture { grainface4 }
  }
triangle {<                 1,                 0,                 0>,
          <                -0,    0.072876995892,                -0>,
          <                 0,                 1,                 0>
            texture { grainface4 }
  }
#declare grainface5 =
  texture { pigment { rgbt <1.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 6
triangle {<                -0,    0.072876995892,                -0>,
          <                -0,                 1,    0.523792986818>,
          <                 1,                 1,    0.921109989703>
            texture { grainface5 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <                 1,                -0,    0.356143977106>,
          <    0.103627646137,                -0,                -0>
            texture { grainface5 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <    0.103627646137,                -0,                -0>,
          <                -0,    0.072876995892,                -0>
            texture { grainface5 }
  }
#declare face12 =
  texture { pigment { rgbt <1.000000,1.000000,1.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
#declare edge3 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
#declare edge15 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
#declare ver6 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,    0.921109989703>,
              0.0168179283050743
    texture { ver6 }
  }
#declare ver12 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    sphere {<                 1,                 1,                 1>,
              0.0168179283050743
    texture { ver12 }
  }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
