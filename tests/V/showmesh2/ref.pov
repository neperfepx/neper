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
  texture { pigment { rgbt <0.498039,0.498039,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
#declare grainface6 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 7
triangle {<                 0,                 0,                 0>,
          <                 0,                 0,                 1>,
          <                 0,                 1,                 1>
            texture { grainface6 }
  }
triangle {<                 0,                 1,                 1>,
          <                -0,                 1,    0.523792986818>,
          <                -0,    0.072876995892,                -0>
            texture { grainface6 }
  }
triangle {<                 0,                 1,                 1>,
          <                -0,    0.072876995892,                -0>,
          <                 0,                 0,                 0>
            texture { grainface6 }
  }
#declare grainface7 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 8
triangle {<                 1,                 1,                 1>,
          <                 1,                 0,                 1>,
          <                 1,                -0,    0.356143977106>
            texture { grainface7 }
  }
triangle {<                 1,                -0,    0.356143977106>,
          <                 1,                 1,    0.921109989703>,
          <                 1,                 1,                 1>
            texture { grainface7 }
  }
#declare grainface8 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 9
triangle {<                 1,                 0,                 1>,
          <                 0,                 0,                 1>,
          <                 0,                 0,                 0>
            texture { grainface8 }
  }
triangle {<                 0,                 0,                 0>,
          <    0.103627646137,                -0,                -0>,
          <                 1,                -0,    0.356143977106>
            texture { grainface8 }
  }
triangle {<                 0,                 0,                 0>,
          <                 1,                -0,    0.356143977106>,
          <                 1,                 0,                 1>
            texture { grainface8 }
  }
#declare grainface9 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 10
triangle {<                 0,                 1,                 1>,
          <                 1,                 1,                 1>,
          <                 1,                 1,    0.921109989703>
            texture { grainface9 }
  }
triangle {<                 1,                 1,    0.921109989703>,
          <                -0,                 1,    0.523792986818>,
          <                 0,                 1,                 1>
            texture { grainface9 }
  }
#declare grainface10 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 11
triangle {<                 0,                 0,                 0>,
          <                -0,    0.072876995892,                -0>,
          <    0.103627646137,                -0,                -0>
            texture { grainface10 }
  }
#declare grainface11 =
  texture { pigment { rgbt <0.000000,1.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
// poly face 12
triangle {<                 1,                 0,                 1>,
          <                 1,                 1,                 1>,
          <                 0,                 1,                 1>
            texture { grainface11 }
  }
triangle {<                 0,                 1,                 1>,
          <                 0,                 0,                 1>,
          <                 1,                 0,                 1>
            texture { grainface11 }
  }
#declare edge1 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
#declare edge4 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
#declare edge16 =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
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
mesh2 {
  vertex_vectors {
    88,
    <1.000000000000,0.000000000000,0.000000000000>,
    <1.000000000000,1.000000000000,0.000000000000>,
    <0.000000000000,1.000000000000,0.000000000000>,
    <0.000000000000,0.072876995892,0.000000000000>,
    <0.000000000000,1.000000000000,0.523792986818>,
    <1.000000000000,1.000000000000,0.921109989703>,
    <1.000000000000,0.000000000000,0.356143977106>,
    <0.103627646137,0.000000000000,0.000000000000>,
    <0.000000000000,0.000000000000,0.000000000000>,
    <0.000000000000,0.000000000000,1.000000000000>,
    <0.000000000000,1.000000000000,1.000000000000>,
    <1.000000000000,1.000000000000,1.000000000000>,
    <1.000000000000,0.000000000000,1.000000000000>,
    <0.000000000000,0.701104617235,0.000000000000>,
    <0.000000000000,0.402209234470,0.000000000000>,
    <0.000000000000,0.182654408751,0.000000000000>,
    <0.000000000000,0.182654408751,0.062020507216>,
    <0.000000000000,0.402209234470,0.186061521649>,
    <0.000000000000,0.701104617235,0.354927254234>,
    <0.000000000000,1.000000000000,0.261896493409>,
    <1.000000000000,0.333333333333,0.000000000000>,
    <1.000000000000,0.666666666667,0.000000000000>,
    <1.000000000000,1.000000000000,0.288501209186>,
    <1.000000000000,1.000000000000,0.577002418372>,
    <1.000000000000,1.000000000000,0.806407465926>,
    <1.000000000000,0.875473586152,0.850756798208>,
    <1.000000000000,0.626420758457,0.710050415219>,
    <1.000000000000,0.313210379228,0.533097196163>,
    <0.644614079982,0.000000000000,0.214943108497>,
    <0.289228159963,0.000000000000,0.073742239887>,
    <0.289228159963,0.000000000000,0.000000000000>,
    <0.644614079982,0.000000000000,0.000000000000>,
    <0.666666666667,1.000000000000,0.000000000000>,
    <0.333333333333,1.000000000000,0.000000000000>,
    <0.313210379228,1.000000000000,0.648236795966>,
    <0.626420758457,1.000000000000,0.772680605113>,
    <0.875473586152,1.000000000000,0.871633528173>,
    <0.000000000000,0.000000000000,0.118406524671>,
    <0.000000000000,0.000000000000,0.355219574014>,
    <0.000000000000,0.000000000000,0.677609787007>,
    <0.000000000000,0.333333333333,1.000000000000>,
    <0.000000000000,0.666666666667,1.000000000000>,
    <0.000000000000,1.000000000000,0.761896493409>,
    <1.000000000000,0.875473586152,1.000000000000>,
    <1.000000000000,0.626420758457,1.000000000000>,
    <1.000000000000,0.313210379228,1.000000000000>,
    <1.000000000000,0.000000000000,0.678071988553>,
    <0.666666666667,0.000000000000,1.000000000000>,
    <0.333333333333,0.000000000000,1.000000000000>,
    <0.313210379228,1.000000000000,1.000000000000>,
    <0.626420758457,1.000000000000,1.000000000000>,
    <0.875473586152,1.000000000000,1.000000000000>,
    <0.000000000000,0.850552308617,0.130948246705>,
    <1.000000000000,0.511807085053,0.219380169554>,
    <1.000000000000,0.776565927567,0.184454943986>,
    <1.000000000000,0.704667358384,0.418747725413>,
    <1.000000000000,0.219942911644,0.224539875377>,
    <0.699719460231,1.000000000000,0.458260958483>,
    <0.764143265934,1.000000000000,0.198467133220>,
    <0.494542352852,1.000000000000,0.264677588646>,
    <0.190181010902,1.000000000000,0.283100644140>,
    <0.425030917231,0.276352568235,0.000000000000>,
    <0.244606163224,0.524348277188,0.000000000000>,
    <0.513631647844,0.532352374883,0.000000000000>,
    <0.729981213298,0.720142398633,0.000000000000>,
    <0.759036836908,0.361263905965,0.000000000000>,
    <0.151784698079,0.208348783505,0.000000000000>,
    <0.191361619853,0.787344828536,0.000000000000>,
    <0.438824267986,0.761363336170,0.000000000000>,
    <0.000000000000,0.813535924694,0.725023285080>,
    <0.000000000000,0.201841694023,0.702796286444>,
    <0.000000000000,0.364272566544,0.474425381623>,
    <0.000000000000,0.513444264259,0.709522976020>,
    <1.000000000000,0.250568303383,0.784243919987>,
    <0.800121033704,0.000000000000,0.655359019303>,
    <0.489445455573,0.000000000000,0.682995041661>,
    <0.218334003960,0.000000000000,0.765928426277>,
    <0.273603616186,0.000000000000,0.461739696057>,
    <0.156605189614,1.000000000000,0.852533322344>,
    <0.200000000000,0.195975409179,1.000000000000>,
    <0.464064829583,0.237924129741,1.000000000000>,
    <0.267622477801,0.475842234965,1.000000000000>,
    <0.546205218937,0.442045370203,1.000000000000>,
    <0.486325247700,0.704090536183,1.000000000000>,
    <0.215401594113,0.761343459969,1.000000000000>,
    <0.770561295737,0.313681779957,1.000000000000>,
    <0.716992354972,0.576171822069,1.000000000000>,
    <0.783006532076,0.795824001443,1.000000000000> 
  }
  texture_list {
    172,
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<1.000000000000,0.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
    texture{pigment{rgb<0.000000000000,1.000000000000,0.000000000000>} finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000}}
  }
  face_indices {
    172,
    <22,54,1>,0,
    <58,22,1>,1,
    <33,60,59>,2,
    <59,58,32>,3,
    <62,63,61>,4,
    <66,62,61>,5,
    <61,66,30>,6,
    <18,52,19>,7,
    <63,65,61>,8,
    <23,26,25>,9,
    <62,67,13>,10,
    <28,29,31>,11,
    <65,31,61>,12,
    <34,60,4>,13,
    <32,58,1>,14,
    <64,32,1>,15,
    <4,18,19>,16,
    <60,19,4>,17,
    <54,1,21>,18,
    <21,64,1>,19,
    <26,23,55>,20,
    <27,26,55>,21,
    <53,27,55>,22,
    <56,0,20>,23,
    <0,20,65>,24,
    <7,30,66>,25,
    <7,30,29>,26,
    <65,20,21>,27,
    <21,53,20>,28,
    <16,3,15>,29,
    <3,15,66>,30,
    <7,3,66>,31,
    <23,35,36>,32,
    <62,63,68>,33,
    <64,63,65>,34,
    <64,63,68>,35,
    <52,2,19>,36,
    <19,60,2>,37,
    <60,33,2>,38,
    <33,2,67>,39,
    <67,13,2>,40,
    <13,2,52>,41,
    <53,54,55>,42,
    <27,53,56>,43,
    <22,55,54>,44,
    <56,6,27>,45,
    <14,62,13>,46,
    <13,17,14>,47,
    <25,24,5>,48,
    <24,5,36>,49,
    <0,31,28>,50,
    <23,57,35>,51,
    <24,36,23>,52,
    <16,15,14>,53,
    <15,14,66>,54,
    <30,61,31>,55,
    <29,30,31>,56,
    <23,24,25>,57,
    <54,21,53>,58,
    <57,35,34>,59,
    <34,59,57>,60,
    <65,0,31>,61,
    <57,58,22>,62,
    <59,57,58>,63,
    <14,17,16>,64,
    <65,21,64>,65,
    <18,13,52>,66,
    <6,56,0>,67,
    <0,28,6>,68,
    <53,56,20>,69,
    <68,32,33>,70,
    <33,59,32>,71,
    <64,68,32>,72,
    <22,23,57>,73,
    <22,23,55>,74,
    <66,14,62>,75,
    <67,62,68>,76,
    <34,60,59>,77,
    <68,33,67>,78,
    <13,17,18>,79,
    <87,86,83>,80,
    <81,82,83>,81,
    <35,50,36>,82,
    <81,80,82>,83,
    <73,12,46>,84,
    <12,46,74>,85,
    <26,27,73>,86,
    <76,77,39>,87,
    <8,7,3>,88,
    <3,37,8>,89,
    <8,7,37>,90,
    <16,3,37>,91,
    <34,35,49>,92,
    <79,40,81>,93,
    <28,6,74>,94,
    <80,79,81>,95,
    <34,42,4>,96,
    <4,69,42>,97,
    <80,85,47>,98,
    <74,12,47>,99,
    <80,85,82>,100,
    <45,73,12>,101,
    <12,85,45>,102,
    <7,29,37>,103,
    <83,49,84>,104,
    <49,50,83>,105,
    <49,50,35>,106,
    <49,78,34>,107,
    <81,83,84>,108,
    <80,79,48>,109,
    <77,76,75>,110,
    <78,42,10>,111,
    <42,10,69>,112,
    <40,79,9>,113,
    <9,70,40>,114,
    <78,34,42>,115,
    <51,43,87>,116,
    <5,25,43>,117,
    <69,72,18>,118,
    <74,47,75>,119,
    <75,74,28>,120,
    <25,26,44>,121,
    <70,71,72>,122,
    <50,51,87>,123,
    <50,51,36>,124,
    <39,38,70>,125,
    <39,38,77>,126,
    <81,41,84>,127,
    <6,46,74>,128,
    <39,70,9>,129,
    <9,76,39>,130,
    <82,83,86>,131,
    <51,5,36>,132,
    <18,4,69>,133,
    <77,75,28>,134,
    <50,83,87>,135,
    <28,29,77>,136,
    <51,43,11>,137,
    <43,11,5>,138,
    <5,51,11>,139,
    <29,38,37>,140,
    <78,49,10>,141,
    <49,10,84>,142,
    <38,17,71>,143,
    <38,16,37>,144,
    <29,38,77>,145,
    <76,48,75>,146,
    <18,71,72>,147,
    <26,73,45>,148,
    <41,40,72>,149,
    <41,40,81>,150,
    <75,47,48>,151,
    <47,48,80>,152,
    <17,38,16>,153,
    <38,70,71>,154,
    <85,82,86>,155,
    <41,69,72>,156,
    <18,17,71>,157,
    <73,27,46>,158,
    <44,26,45>,159,
    <45,85,44>,160,
    <27,6,46>,161,
    <85,86,44>,162,
    <12,47,85>,163,
    <41,69,10>,164,
    <10,84,41>,165,
    <43,87,44>,166,
    <25,43,44>,167,
    <40,70,72>,168,
    <9,76,48>,169,
    <79,9,48>,170,
    <86,44,87>,171 
  }
}
#declare elt3dedge =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
    cylinder {<                 1,                 1,    0.288501209186>,
              <                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.776565927567,    0.184454943986>,
              <                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.288501209186>,
              <                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.764143265934,                 1,     0.19846713322>,
              <                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.764143265934,                 1,     0.19846713322>,
              <                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.333333333333,                 1,                 0>,
              <    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.190181010902,                 1,     0.28310064414>,
              <    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.333333333333,                 1,                 0>,
              <    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.494542352852,                 1,    0.264677588646>,
              <    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.764143265934,                 1,     0.19846713322>,
              <    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.494542352852,                 1,    0.264677588646>,
              <    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.244606163224,    0.524348277188,                 0>,
              <    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.513631647844,    0.532352374883,                 0>,
              <    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.244606163224,    0.524348277188,                 0>,
              <    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.151784698079,    0.208348783505,                 0>,
              <    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.151784698079,    0.208348783505,                 0>,
              <    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.151784698079,    0.208348783505,                 0>,
              <    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.425030917231,    0.276352568235,                 0>,
              <    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,    0.354927254234>,
              <                 0,    0.850552308617,    0.130948246705>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.850552308617,    0.130948246705>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.850552308617,    0.130948246705>,
              <                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.850552308617,    0.130948246705>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,    0.354927254234>,
              <                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.513631647844,    0.532352374883,                 0>,
              <    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.759036836908,    0.361263905965,                 0>,
              <    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.577002418372>,
              <                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.626420758457,    0.710050415219>,
              <                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.577002418372>,
              <                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.244606163224,    0.524348277188,                 0>,
              <    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.191361619853,    0.787344828536,                 0>,
              <                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.244606163224,    0.524348277188,                 0>,
              <                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.644614079982,                 0,    0.214943108497>,
              <    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.289228159963,                 0,    0.073742239887>,
              <    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.644614079982,                 0,    0.214943108497>,
              <    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.759036836908,    0.361263905965,                 0>,
              <    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.644614079982,                 0,                 0>,
              <    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.425030917231,    0.276352568235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,    0.648236795966>,
              <    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.190181010902,                 1,     0.28310064414>,
              <                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,    0.648236795966>,
              <                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.666666666667,                 1,                 0>,
              <                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.729981213298,    0.720142398633,                 0>,
              <    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.729981213298,    0.720142398633,                 0>,
              <                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,    0.523792986818>,
              <                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,    0.523792986818>,
              <                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.190181010902,                 1,     0.28310064414>,
              <                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,                 0>,
              <                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.776565927567,    0.184454943986>,
              <                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.666666666667,                 0>,
              <    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.577002418372>,
              <                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.626420758457,    0.710050415219>,
              <                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,    0.533097196163>,
              <                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,    0.533097196163>,
              <                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.511807085053,    0.219380169554>,
              <                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.511807085053,    0.219380169554>,
              <                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.219942911644,    0.224539875377>,
              <                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 0>,
              <                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.219942911644,    0.224539875377>,
              <                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.333333333333,                 0>,
              <    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 0>,
              <    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.103627646137,                 0,                 0>,
              <    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.103627646137,                 0,                 0>,
              <    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.289228159963,                 0,                 0>,
              <    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.103627646137,                 0,                 0>,
              <    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.333333333333,                 0>,
              <                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.759036836908,    0.361263905965,                 0>,
              <                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.666666666667,                 0>,
              <                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.666666666667,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.511807085053,    0.219380169554>,
              <                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.333333333333,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.182654408751,    0.062020507216>,
              <                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.072876995892,                 0>,
              <                 0,    0.182654408751,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.182654408751,    0.062020507216>,
              <                 0,    0.182654408751,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.182654408751,                 0>,
              <    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.072876995892,                 0>,
              <    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.103627646137,                 0,                 0>,
              <                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.577002418372>,
              <    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,    0.772680605113>,
              <    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.577002418372>,
              <    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.513631647844,    0.532352374883,                 0>,
              <    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.244606163224,    0.524348277188,                 0>,
              <    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.729981213298,    0.720142398633,                 0>,
              <    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.513631647844,    0.532352374883,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.729981213298,    0.720142398633,                 0>,
              <    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.759036836908,    0.361263905965,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.729981213298,    0.720142398633,                 0>,
              <    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.729981213298,    0.720142398633,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.850552308617,    0.130948246705>,
              <                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.850552308617,    0.130948246705>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,                 0>,
              <                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.261896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.190181010902,                 1,     0.28310064414>,
              <                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.190181010902,                 1,     0.28310064414>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.333333333333,                 1,                 0>,
              <                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,                 0>,
              <    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.333333333333,                 1,                 0>,
              <    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,                 0>,
              <                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,                 0>,
              <                 0,    0.850552308617,    0.130948246705>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.850552308617,    0.130948246705>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.511807085053,    0.219380169554>,
              <                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.776565927567,    0.184454943986>,
              <                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.776565927567,    0.184454943986>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.511807085053,    0.219380169554>,
              <                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.511807085053,    0.219380169554>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,    0.533097196163>,
              <                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.288501209186>,
              <                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.704667358384,    0.418747725413>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.219942911644,    0.224539875377>,
              <                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.219942911644,    0.224539875377>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,    0.356143977106>,
              <                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,                 0>,
              <    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.244606163224,    0.524348277188,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,                 0>,
              <                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,                 0>,
              <                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,    0.186061521649>,
              <                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,    0.850756798208>,
              <                 1,                 1,    0.806407465926>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.806407465926>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.806407465926>,
              <                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.806407465926>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,    0.850756798208>,
              <                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.921109989703>,
              <    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.806407465926>,
              <    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.806407465926>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 0>,
              <    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 0>,
              <    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.577002418372>,
              <    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.699719460231,                 1,    0.458260958483>,
              <    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.806407465926>,
              <                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.806407465926>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.182654408751,                 0>,
              <                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.182654408751,    0.062020507216>,
              <                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,                 0>,
              <    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.151784698079,    0.208348783505,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.289228159963,                 0,                 0>,
              <    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,    0.772680605113>,
              <    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.699719460231,                 1,    0.458260958483>,
              <    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,    0.648236795966>,
              <    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.494542352852,                 1,    0.264677588646>,
              <    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.494542352852,                 1,    0.264677588646>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.699719460231,                 1,    0.458260958483>,
              <    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.764143265934,                 1,     0.19846713322>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.699719460231,                 1,    0.458260958483>,
              <                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.699719460231,                 1,    0.458260958483>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,    0.186061521649>,
              <                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,    0.354927254234>,
              <                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,    0.356143977106>,
              <                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.644614079982,                 0,    0.214943108497>,
              <                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.438824267986,     0.76136333617,                 0>,
              <    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.666666666667,                 1,                 0>,
              <    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.438824267986,     0.76136333617,                 0>,
              <    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 1,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.288501209186>,
              <                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.288501209186>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.577002418372>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.191361619853,    0.787344828536,                 0>,
              <    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.191361619853,    0.787344828536,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.438824267986,     0.76136333617,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,    0.186061521649>,
              <                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.783006532076,    0.795824001443,                 1>,
              <    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.716992354972,    0.576171822069,                 1>,
              <      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.783006532076,    0.795824001443,                 1>,
              <      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.267622477801,    0.475842234965,                 1>,
              <    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.546205218937,    0.442045370203,                 1>,
              <      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.267622477801,    0.475842234965,                 1>,
              <      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,    0.772680605113>,
              <    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,                 1>,
              <    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.267622477801,    0.475842234965,                 1>,
              <    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.464064829583,    0.237924129741,                 1>,
              <    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.250568303383,    0.784243919987>,
              <                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 1>,
              <                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.250568303383,    0.784243919987>,
              <                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,    0.678071988553>,
              <    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 1>,
              <    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,    0.533097196163>,
              <                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.626420758457,    0.710050415219>,
              <                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<     0.21833400396,                 0,    0.765928426277>,
              <    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<     0.21833400396,                 0,    0.765928426277>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.273603616186,                 0,    0.461739696057>,
              <                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<     0.21833400396,                 0,    0.765928426277>,
              <                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<     0.21833400396,                 0,    0.765928426277>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,                 0>,
              <    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,                 0>,
              <                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.072876995892,                 0>,
              <                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.072876995892,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.118406524671>,
              <                 0,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.103627646137,                 0,                 0>,
              <                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.103627646137,                 0,                 0>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.182654408751,    0.062020507216>,
              <                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,    0.772680605113>,
              <    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,    0.772680605113>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,    0.648236795966>,
              <    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<               0.2,    0.195975409179,                 1>,
              <                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<               0.2,    0.195975409179,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.333333333333,                 1>,
              <    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<               0.2,    0.195975409179,                 1>,
              <    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<               0.2,    0.195975409179,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,    0.356143977106>,
              <    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.644614079982,                 0,    0.214943108497>,
              <    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.464064829583,    0.237924129741,                 1>,
              <               0.2,    0.195975409179,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<               0.2,    0.195975409179,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,    0.648236795966>,
              <                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,    0.761896493409>,
              <                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,    0.523792986818>,
              <                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.523792986818>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.813535924694,     0.72502328508>,
              <                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.464064829583,    0.237924129741,                 1>,
              <    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.770561295737,    0.313681779957,                 1>,
              <    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.464064829583,    0.237924129741,                 1>,
              <    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 1>,
              <    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.800121033704,                 0,    0.655359019303>,
              <    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.770561295737,    0.313681779957,                 1>,
              <    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,                 1>,
              <                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.250568303383,    0.784243919987>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,                 1>,
              <                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,                 1>,
              <    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.770561295737,    0.313681779957,                 1>,
              <                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.289228159963,                 0,    0.073742239887>,
              <                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<      0.4863252477,    0.704090536183,                 1>,
              <    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,                 1>,
              <    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<      0.4863252477,    0.704090536183,                 1>,
              <    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,                 1>,
              <    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,                 1>,
              <      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<      0.4863252477,    0.704090536183,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,                 1>,
              <    0.156605189614,                 1,    0.852533322344>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.156605189614,                 1,    0.852533322344>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.156605189614,                 1,    0.852533322344>,
              <    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.156605189614,                 1,    0.852533322344>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,    0.648236795966>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.267622477801,    0.475842234965,                 1>,
              <    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<               0.2,    0.195975409179,                 1>,
              <    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<               0.2,    0.195975409179,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.464064829583,    0.237924129741,                 1>,
              <    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.464064829583,    0.237924129741,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<     0.21833400396,                 0,    0.765928426277>,
              <    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<     0.21833400396,                 0,    0.765928426277>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.273603616186,                 0,    0.461739696057>,
              <    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.156605189614,                 1,    0.852533322344>,
              <                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.156605189614,                 1,    0.852533322344>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,    0.761896493409>,
              <                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,    0.761896493409>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.156605189614,                 1,    0.852533322344>,
              <                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.156605189614,                 1,    0.852533322344>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,                 1>,
              <                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<               0.2,    0.195975409179,                 1>,
              <                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<               0.2,    0.195975409179,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.333333333333,                 1>,
              <                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,                 1>,
              <                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.201841694023,    0.702796286444>,
              <                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.875473586152,                 1,                 1>,
              <                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,                 1>,
              <    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.875473586152,                 1,                 1>,
              <    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,    0.850756798208>,
              <                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,    0.921109989703>,
              <                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.813535924694,     0.72502328508>,
              <                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.513444264259,     0.70952297602>,
              <                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.813535924694,     0.72502328508>,
              <                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.666666666667,                 0,                 1>,
              <    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.800121033704,                 0,    0.655359019303>,
              <    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.800121033704,                 0,    0.655359019303>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.489445455573,                 0,    0.682995041661>,
              <    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.626420758457,    0.710050415219>,
              <                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,    0.850756798208>,
              <                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,    0.850756798208>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.201841694023,    0.702796286444>,
              <                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.364272566544,    0.474425381623>,
              <                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.201841694023,    0.702796286444>,
              <                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,                 1>,
              <    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.626420758457,                 1,                 1>,
              <    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.626420758457,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.875473586152,                 1,                 1>,
              <    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,    0.871633528173>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.677609787007>,
              <                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.355219574014>,
              <                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.677609787007>,
              <                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.201841694023,    0.702796286444>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.355219574014>,
              <    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.267622477801,    0.475842234965,                 1>,
              <                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.267622477801,    0.475842234965,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.666666666667,                 1>,
              <    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 0,    0.356143977106>,
              <                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.356143977106>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.677609787007>,
              <                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.677609787007>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,                 1>,
              <     0.21833400396,                 0,    0.765928426277>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<     0.21833400396,                 0,    0.765928426277>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.546205218937,    0.442045370203,                 1>,
              <    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.546205218937,    0.442045370203,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.875473586152,                 1,                 1>,
              <                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.273603616186,                 0,    0.461739696057>,
              <    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.644614079982,                 0,    0.214943108497>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.289228159963,                 0,    0.073742239887>,
              <    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.273603616186,                 0,    0.461739696057>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,                 1>,
              <                 1,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.875473586152,                 1,                 1>,
              <                 1,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.875473586152,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,                 1,                 1>,
              <                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 1,    0.921109989703>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.289228159963,                 0,    0.073742239887>,
              <                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.289228159963,                 0,    0.073742239887>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.355219574014>,
              <                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.118406524671>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.313210379228,                 1,                 1>,
              <                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.313210379228,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 1,                 1>,
              <    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.215401594113,    0.761343459969,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.355219574014>,
              <                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,     0.40220923447,    0.186061521649>,
              <                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,     0.40220923447,    0.186061521649>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.355219574014>,
              <                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,    0.355219574014>,
              <                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,    0.355219574014>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.182654408751,    0.062020507216>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<     0.21833400396,                 0,    0.765928426277>,
              <    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<     0.21833400396,                 0,    0.765928426277>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.333333333333,                 0,                 1>,
              <    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.489445455573,                 0,    0.682995041661>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.701104617235,    0.354927254234>,
              <                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.701104617235,    0.354927254234>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.364272566544,    0.474425381623>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.626420758457,    0.710050415219>,
              <                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,    0.710050415219>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.666666666667,                 1>,
              <                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.333333333333,                 1>,
              <                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.333333333333,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.666666666667,                 1>,
              <                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.513444264259,     0.70952297602>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.666666666667,                 0,                 1>,
              <    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.666666666667,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.770561295737,    0.313681779957,                 1>,
              <    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.666666666667,                 1>,
              <                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.813535924694,     0.72502328508>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.313210379228,    0.533097196163>,
              <                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,    0.533097196163>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,                 0,    0.678071988553>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.626420758457,                 1>,
              <                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.313210379228,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.770561295737,    0.313681779957,                 1>,
              <                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.770561295737,    0.313681779957,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.716992354972,    0.576171822069,                 1>,
              <                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.716992354972,    0.576171822069,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,    0.666666666667,                 1>,
              <                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,    0.666666666667,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 1,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<    0.783006532076,    0.795824001443,                 1>,
              <                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.783006532076,    0.795824001443,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 1,    0.875473586152,                 1>,
              <                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.875473586152,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 1,    0.626420758457,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    cylinder {<                 0,                 0,                 1>,
              <    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<                 0,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
    sphere {<    0.333333333333,                 0,                 1>,
              0.00594513765584376
    texture { elt3dedge }
  }
#declare elt3dedge =
  texture { pigment { rgb <0.000000,0.000000,0.000000> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
