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

#declare csys =
  texture { pigment { rgbt <0.000000,0.000000,0.000000,0.> } finish {ambient 0.600000 diffuse 0.600000 reflection 0.000000} }    cylinder {<                 0,                 0,                 0>,
              <              0.14,                 0,                 0>,
                            0.01
    texture { csys }
  }
    cone {<              0.14,                 0,                 0>,
                            0.03,
              <               0.2,                 0,                 0>,
              0
    texture { csys }
  }
#declare CamLoc = <3.962000,-5.270000,4.827000>;
#declare CamSky = <0.000000,0.000000,1.000000>;
#declare CamLA = <0.500000,0.500000,0.500000>;
#declare TextLowerLeft=<0.200000, 0.000000, 0.000000>;
#declare TransVec1=vnormalize(TextLowerLeft-CamLoc);
#declare TransVec2=vnormalize(vcross(TransVec1,CamSky));
#declare TransVec3=vnormalize(vcross(TransVec1,TransVec2));
#declare X1 =
text {ttf "timrom.ttf" "foo" 0, 0
pigment {rgb<0,0,0>}
scale 0.100000
Shear_Trans(TransVec2,-TransVec3,TransVec1) translate TextLowerLeft
}
#declare Min = min_extent ( X1 );
#declare Max = max_extent ( X1 );
object { X1 translate (Min - Max) / 2 + <0.100000,0.000000,0.000000>}
    cylinder {<                 0,                 0,                 0>,
              <                 0,              0.14,                 0>,
                            0.01
    texture { csys }
  }
    cone {<                 0,              0.14,                 0>,
                            0.03,
              <                 0,               0.2,                 0>,
              0
    texture { csys }
  }
#declare CamLoc = <3.962000,-5.270000,4.827000>;
#declare CamSky = <0.000000,0.000000,1.000000>;
#declare CamLA = <0.500000,0.500000,0.500000>;
#declare TextLowerLeft=<0.000000, 0.200000, 0.000000>;
#declare TransVec1=vnormalize(TextLowerLeft-CamLoc);
#declare TransVec2=vnormalize(vcross(TransVec1,CamSky));
#declare TransVec3=vnormalize(vcross(TransVec1,TransVec2));
#declare X2 =
text {ttf "timrom.ttf" "bar" 0, 0
pigment {rgb<0,0,0>}
scale 0.100000
Shear_Trans(TransVec2,-TransVec3,TransVec1) translate TextLowerLeft
}
#declare Min = min_extent ( X2 );
#declare Max = max_extent ( X2 );
object { X2 translate (Min - Max) / 2 + <0.000000,0.100000,0.000000>}
    cylinder {<                 0,                 0,                 0>,
              <                 0,                 0,              0.14>,
                            0.01
    texture { csys }
  }
    cone {<                 0,                 0,              0.14>,
                            0.03,
              <                 0,                 0,               0.2>,
              0
    texture { csys }
  }
#declare CamLoc = <3.962000,-5.270000,4.827000>;
#declare CamSky = <0.000000,0.000000,1.000000>;
#declare CamLA = <0.500000,0.500000,0.500000>;
#declare TextLowerLeft=<0.000000, 0.000000, 0.200000>;
#declare TransVec1=vnormalize(TextLowerLeft-CamLoc);
#declare TransVec2=vnormalize(vcross(TransVec1,CamSky));
#declare TransVec3=vnormalize(vcross(TransVec1,TransVec2));
#declare X3 =
text {ttf "timrom.ttf" "baz" 0, 0
pigment {rgb<0,0,0>}
scale 0.100000
Shear_Trans(TransVec2,-TransVec3,TransVec1) translate TextLowerLeft
}
#declare Min = min_extent ( X3 );
#declare Max = max_extent ( X3 );
object { X3 translate (Min - Max) / 2 + <0.000000,0.000000,0.100000>}
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
rotate<-90,  0,  0>
scale <  1,  1, -1>
}
