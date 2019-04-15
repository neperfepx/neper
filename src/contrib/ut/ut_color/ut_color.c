/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "ut.h"

int
ut_color_name_valid (char *name)
{
  int status;
  int* c = ut_alloc_1d_int (3);

  status = ut_color_name_rgb (name, c);

  ut_free_1d_int (c);

  return (status == -1) ? 0 : 1;
}

int
ut_color_name_rgb (char *name, int *c)
{
  return name2rgb (name, c);
}


int
name2rgb (char *name, int *c)
{
  int i;
  int res = 0;
  char** parts = NULL;
  int qty;

  if (name[0] >= '0' && name[0] <= '9')
  {
    if (strchr (name, ',') != NULL)
      ut_string_separate (name, ",", &parts, &qty);
    else if (strchr (name, '/') != NULL)
      ut_string_separate (name, "/", &parts, &qty);
    else if (strchr (name, '|') != NULL)
      ut_string_separate (name, "|", &parts, &qty);
    else if (strchr (name, '.') != NULL)
      ut_string_separate (name, ".", &parts, &qty);
    else if (strchr (name, ':') != NULL)
      ut_string_separate (name, ":", &parts, &qty);
    else if (strchr (name, ';') != NULL)
      ut_string_separate (name, ";", &parts, &qty);
    else
      qty = -1;

    if (qty != 3)
      res = -1;
    else
      for (i = 0; i < 3; i++)
	sscanf (parts[i], "%d", &(c[i]));

    ut_free_2d_char (parts, qty);
  }
  else if (strcmp (name, "aliceblue") == 0)
  {
    c[0] = 240;
    c[1] = 248;
    c[2] = 255;
  }
  else if (strcmp (name, "antiquewhite") == 0)
  {
    c[0] = 250;
    c[1] = 235;
    c[2] = 215;
  }
  else if (strcmp (name, "aqua") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "aquamarine") == 0)
  {
    c[0] = 127;
    c[1] = 255;
    c[2] = 212;
  }
  else if (strcmp (name, "azure") == 0)
  {
    c[0] = 240;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "beige") == 0)
  {
    c[0] = 245;
    c[1] = 245;
    c[2] = 220;
  }
  else if (strcmp (name, "bisque") == 0)
  {
    c[0] = 255;
    c[1] = 228;
    c[2] = 196;
  }
  else if (strcmp (name, "black") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "blanchedalmond") == 0)
  {
    c[0] = 255;
    c[1] = 235;
    c[2] = 205;
  }
  else if (strcmp (name, "blue") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 255;
  }
  else if (strcmp (name, "blueviolet") == 0)
  {
    c[0] = 138;
    c[1] = 43;
    c[2] = 226;
  }
  else if (strcmp (name, "brown") == 0)
  {
    c[0] = 165;
    c[1] = 42;
    c[2] = 42;
  }
  else if (strcmp (name, "burlywood") == 0)
  {
    c[0] = 222;
    c[1] = 184;
    c[2] = 135;
  }
  else if (strcmp (name, "cadetblue") == 0)
  {
    c[0] = 95;
    c[1] = 158;
    c[2] = 160;
  }
  else if (strcmp (name, "chartreuse") == 0)
  {
    c[0] = 127;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "chocolate") == 0)
  {
    c[0] = 210;
    c[1] = 105;
    c[2] = 30;
  }
  else if (strcmp (name, "coral") == 0)
  {
    c[0] = 255;
    c[1] = 127;
    c[2] = 80;
  }
  else if (strcmp (name, "cornflowerblue") == 0)
  {
    c[0] = 100;
    c[1] = 149;
    c[2] = 237;
  }
  else if (strcmp (name, "cornsilk") == 0)
  {
    c[0] = 255;
    c[1] = 248;
    c[2] = 220;
  }
  else if (strcmp (name, "crimson") == 0)
  {
    c[0] = 220;
    c[1] = 20;
    c[2] = 60;
  }
  else if (strcmp (name, "cyan") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "darkblue") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 139;
  }
  else if (strcmp (name, "darkcyan") == 0)
  {
    c[0] = 0;
    c[1] = 139;
    c[2] = 139;
  }
  else if (strcmp (name, "darkgoldenrod") == 0)
  {
    c[0] = 184;
    c[1] = 134;
    c[2] = 11;
  }
  else if (strcmp (name, "darkgray") == 0)
  {
    c[0] = 64;
    c[1] = 64;
    c[2] = 64;
  }
  else if (strcmp (name, "darkgreen") == 0)
  {
    c[0] = 0;
    c[1] = 100;
    c[2] = 0;
  }
  else if (strcmp (name, "darkgrey") == 0)
  {
    c[0] = 64;
    c[1] = 64;
    c[2] = 64;
  }
  else if (strcmp (name, "darkkhaki") == 0)
  {
    c[0] = 189;
    c[1] = 183;
    c[2] = 107;
  }
  else if (strcmp (name, "darkmagenta") == 0)
  {
    c[0] = 139;
    c[1] = 0;
    c[2] = 139;
  }
  else if (strcmp (name, "darkolivegreen") == 0)
  {
    c[0] = 85;
    c[1] = 107;
    c[2] = 47;
  }
  else if (strcmp (name, "darkorange") == 0)
  {
    c[0] = 255;
    c[1] = 140;
    c[2] = 0;
  }
  else if (strcmp (name, "darkorchid") == 0)
  {
    c[0] = 153;
    c[1] = 50;
    c[2] = 204;
  }
  else if (strcmp (name, "darkred") == 0)
  {
    c[0] = 139;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "darksalmon") == 0)
  {
    c[0] = 233;
    c[1] = 150;
    c[2] = 122;
  }
  else if (strcmp (name, "darkseagreen") == 0)
  {
    c[0] = 143;
    c[1] = 188;
    c[2] = 143;
  }
  else if (strcmp (name, "darkslateblue") == 0)
  {
    c[0] = 72;
    c[1] = 61;
    c[2] = 139;
  }
  else if (strcmp (name, "darkslategray") == 0)
  {
    c[0] = 47;
    c[1] = 79;
    c[2] = 79;
  }
  else if (strcmp (name, "darkslategrey") == 0)
  {
    c[0] = 47;
    c[1] = 79;
    c[2] = 79;
  }
  else if (strcmp (name, "darkturquoise") == 0)
  {
    c[0] = 0;
    c[1] = 206;
    c[2] = 209;
  }
  else if (strcmp (name, "darkviolet") == 0)
  {
    c[0] = 148;
    c[1] = 0;
    c[2] = 211;
  }
  else if (strcmp (name, "deeppink") == 0)
  {
    c[0] = 255;
    c[1] = 20;
    c[2] = 147;
  }
  else if (strcmp (name, "deepskyblue") == 0)
  {
    c[0] = 0;
    c[1] = 191;
    c[2] = 255;
  }
  else if (strcmp (name, "dimgray") == 0)
  {
    c[0] = 105;
    c[1] = 105;
    c[2] = 105;
  }
  else if (strcmp (name, "dimgrey") == 0)
  {
    c[0] = 105;
    c[1] = 105;
    c[2] = 105;
  }
  else if (strcmp (name, "dodgerblue") == 0)
  {
    c[0] = 30;
    c[1] = 144;
    c[2] = 255;
  }
  else if (strcmp (name, "firebrick") == 0)
  {
    c[0] = 178;
    c[1] = 34;
    c[2] = 34;
  }
  else if (strcmp (name, "floralwhite") == 0)
  {
    c[0] = 255;
    c[1] = 250;
    c[2] = 240;
  }
  else if (strcmp (name, "forestgreen") == 0)
  {
    c[0] = 34;
    c[1] = 139;
    c[2] = 34;
  }
  else if (strcmp (name, "fuchsia") == 0)
  {
    c[0] = 255;
    c[1] = 0;
    c[2] = 255;
  }
  else if (strcmp (name, "gainsboro") == 0)
  {
    c[0] = 220;
    c[1] = 220;
    c[2] = 220;
  }
  else if (strcmp (name, "ghostwhite") == 0)
  {
    c[0] = 248;
    c[1] = 248;
    c[2] = 255;
  }
  else if (strcmp (name, "gold") == 0)
  {
    c[0] = 255;
    c[1] = 215;
    c[2] = 0;
  }
  else if (strcmp (name, "goldenrod") == 0)
  {
    c[0] = 218;
    c[1] = 165;
    c[2] = 32;
  }
  else if (strcmp (name, "gray") == 0)
  {
    c[0] = 128;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "grey") == 0)
  {
    c[0] = 128;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "green") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "greenyellow") == 0)
  {
    c[0] = 173;
    c[1] = 255;
    c[2] = 47;
  }
  else if (strcmp (name, "honeydew") == 0)
  {
    c[0] = 240;
    c[1] = 255;
    c[2] = 240;
  }
  else if (strcmp (name, "hotpink") == 0)
  {
    c[0] = 255;
    c[1] = 105;
    c[2] = 180;
  }
  else if (strcmp (name, "indianred") == 0)
  {
    c[0] = 205;
    c[1] = 92;
    c[2] = 92;
  }
  else if (strcmp (name, "indigo") == 0)
  {
    c[0] = 75;
    c[1] = 0;
    c[2] = 130;
  }
  else if (strcmp (name, "ivory") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 240;
  }
  else if (strcmp (name, "khaki") == 0)
  {
    c[0] = 240;
    c[1] = 230;
    c[2] = 140;
  }
  else if (strcmp (name, "lavender") == 0)
  {
    c[0] = 230;
    c[1] = 230;
    c[2] = 250;
  }
  else if (strcmp (name, "lavenderblush") == 0)
  {
    c[0] = 255;
    c[1] = 240;
    c[2] = 245;
  }
  else if (strcmp (name, "lawngreen") == 0)
  {
    c[0] = 124;
    c[1] = 252;
    c[2] = 0;
  }
  else if (strcmp (name, "lemonchiffon") == 0)
  {
    c[0] = 255;
    c[1] = 250;
    c[2] = 205;
  }
  else if (strcmp (name, "lightblue") == 0)
  {
    c[0] = 173;
    c[1] = 216;
    c[2] = 230;
  }
  else if (strcmp (name, "lightcoral") == 0)
  {
    c[0] = 240;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "lightcyan") == 0)
  {
    c[0] = 224;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "lightgoldenrodyellow") == 0)
  {
    c[0] = 250;
    c[1] = 250;
    c[2] = 210;
  }
  else if (strcmp (name, "lightgray") == 0)
  {
    c[0] = 211;
    c[1] = 211;
    c[2] = 211;
  }
  else if (strcmp (name, "lightgreen") == 0)
  {
    c[0] = 144;
    c[1] = 238;
    c[2] = 144;
  }
  else if (strcmp (name, "lightgrey") == 0)
  {
    c[0] = 211;
    c[1] = 211;
    c[2] = 211;
  }
  else if (strcmp (name, "lightpink") == 0)
  {
    c[0] = 255;
    c[1] = 182;
    c[2] = 193;
  }
  else if (strcmp (name, "lightsalmon") == 0)
  {
    c[0] = 255;
    c[1] = 160;
    c[2] = 122;
  }
  else if (strcmp (name, "lightseagreen") == 0)
  {
    c[0] = 32;
    c[1] = 178;
    c[2] = 170;
  }
  else if (strcmp (name, "lightskyblue") == 0)
  {
    c[0] = 135;
    c[1] = 206;
    c[2] = 250;
  }
  else if (strcmp (name, "lightslategray") == 0)
  {
    c[0] = 119;
    c[1] = 136;
    c[2] = 153;
  }
  else if (strcmp (name, "lightslategrey") == 0)
  {
    c[0] = 119;
    c[1] = 136;
    c[2] = 153;
  }
  else if (strcmp (name, "lightsteelblue") == 0)
  {
    c[0] = 176;
    c[1] = 196;
    c[2] = 222;
  }
  else if (strcmp (name, "lightyellow") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 224;
  }
  else if (strcmp (name, "lime") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "limegreen") == 0)
  {
    c[0] = 50;
    c[1] = 205;
    c[2] = 50;
  }
  else if (strcmp (name, "linen") == 0)
  {
    c[0] = 250;
    c[1] = 240;
    c[2] = 230;
  }
  else if (strcmp (name, "magenta") == 0)
  {
    c[0] = 255;
    c[1] = 0;
    c[2] = 255;
  }
  else if (strcmp (name, "maroon") == 0)
  {
    c[0] = 128;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "mediumaquamarine") == 0)
  {
    c[0] = 102;
    c[1] = 205;
    c[2] = 170;
  }
  else if (strcmp (name, "mediumblue") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 205;
  }
  else if (strcmp (name, "mediumorchid") == 0)
  {
    c[0] = 186;
    c[1] = 85;
    c[2] = 211;
  }
  else if (strcmp (name, "mediumpurple") == 0)
  {
    c[0] = 147;
    c[1] = 112;
    c[2] = 219;
  }
  else if (strcmp (name, "mediumseagreen") == 0)
  {
    c[0] = 60;
    c[1] = 179;
    c[2] = 113;
  }
  else if (strcmp (name, "mediumslateblue") == 0)
  {
    c[0] = 123;
    c[1] = 104;
    c[2] = 238;
  }
  else if (strcmp (name, "mediumspringgreen") == 0)
  {
    c[0] = 0;
    c[1] = 250;
    c[2] = 154;
  }
  else if (strcmp (name, "mediumturquoise") == 0)
  {
    c[0] = 72;
    c[1] = 209;
    c[2] = 204;
  }
  else if (strcmp (name, "mediumvioletred") == 0)
  {
    c[0] = 199;
    c[1] = 21;
    c[2] = 133;
  }
  else if (strcmp (name, "midnightblue") == 0)
  {
    c[0] = 25;
    c[1] = 25;
    c[2] = 112;
  }
  else if (strcmp (name, "mintcream") == 0)
  {
    c[0] = 245;
    c[1] = 255;
    c[2] = 250;
  }
  else if (strcmp (name, "mistyrose") == 0)
  {
    c[0] = 255;
    c[1] = 228;
    c[2] = 225;
  }
  else if (strcmp (name, "moccasin") == 0)
  {
    c[0] = 255;
    c[1] = 228;
    c[2] = 181;
  }
  else if (strcmp (name, "navajowhite") == 0)
  {
    c[0] = 255;
    c[1] = 222;
    c[2] = 173;
  }
  else if (strcmp (name, "navy") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 128;
  }
  else if (strcmp (name, "oldlace") == 0)
  {
    c[0] = 253;
    c[1] = 245;
    c[2] = 230;
  }
  else if (strcmp (name, "olive") == 0)
  {
    c[0] = 128;
    c[1] = 128;
    c[2] = 0;
  }
  else if (strcmp (name, "olivedrab") == 0)
  {
    c[0] = 107;
    c[1] = 142;
    c[2] = 35;
  }
  else if (strcmp (name, "orange") == 0)
  {
    c[0] = 255;
    c[1] = 165;
    c[2] = 0;
  }
  else if (strcmp (name, "orangered") == 0)
  {
    c[0] = 255;
    c[1] = 69;
    c[2] = 0;
  }
  else if (strcmp (name, "orchid") == 0)
  {
    c[0] = 218;
    c[1] = 112;
    c[2] = 214;
  }
  else if (strcmp (name, "palegoldenrod") == 0)
  {
    c[0] = 238;
    c[1] = 232;
    c[2] = 170;
  }
  else if (strcmp (name, "palegreen") == 0)
  {
    c[0] = 152;
    c[1] = 251;
    c[2] = 152;
  }
  else if (strcmp (name, "paleturquoise") == 0)
  {
    c[0] = 175;
    c[1] = 238;
    c[2] = 238;
  }
  else if (strcmp (name, "palevioletred") == 0)
  {
    c[0] = 219;
    c[1] = 112;
    c[2] = 147;
  }
  else if (strcmp (name, "papayawhip") == 0)
  {
    c[0] = 255;
    c[1] = 239;
    c[2] = 213;
  }
  else if (strcmp (name, "peachpuff") == 0)
  {
    c[0] = 255;
    c[1] = 218;
    c[2] = 185;
  }
  else if (strcmp (name, "peru") == 0)
  {
    c[0] = 205;
    c[1] = 133;
    c[2] = 63;
  }
  else if (strcmp (name, "pink") == 0)
  {
    c[0] = 255;
    c[1] = 192;
    c[2] = 203;
  }
  else if (strcmp (name, "plum") == 0)
  {
    c[0] = 221;
    c[1] = 160;
    c[2] = 221;
  }
  else if (strcmp (name, "powderblue") == 0)
  {
    c[0] = 176;
    c[1] = 224;
    c[2] = 230;
  }
  else if (strcmp (name, "purple") == 0)
  {
    c[0] = 128;
    c[1] = 0;
    c[2] = 128;
  }
  else if (strcmp (name, "red") == 0)
  {
    c[0] = 255;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "rosybrown") == 0)
  {
    c[0] = 188;
    c[1] = 143;
    c[2] = 143;
  }
  else if (strcmp (name, "royalblue") == 0)
  {
    c[0] = 65;
    c[1] = 105;
    c[2] = 225;
  }
  else if (strcmp (name, "saddlebrown") == 0)
  {
    c[0] = 139;
    c[1] = 69;
    c[2] = 19;
  }
  else if (strcmp (name, "salmon") == 0)
  {
    c[0] = 250;
    c[1] = 128;
    c[2] = 114;
  }
  else if (strcmp (name, "sandybrown") == 0)
  {
    c[0] = 244;
    c[1] = 164;
    c[2] = 96;
  }
  else if (strcmp (name, "seagreen") == 0)
  {
    c[0] = 46;
    c[1] = 139;
    c[2] = 87;
  }
  else if (strcmp (name, "seashell") == 0)
  {
    c[0] = 255;
    c[1] = 245;
    c[2] = 238;
  }
  else if (strcmp (name, "sienna") == 0)
  {
    c[0] = 160;
    c[1] = 82;
    c[2] = 45;
  }
  else if (strcmp (name, "silver") == 0)
  {
    c[0] = 192;
    c[1] = 192;
    c[2] = 192;
  }
  else if (strcmp (name, "skyblue") == 0)
  {
    c[0] = 135;
    c[1] = 206;
    c[2] = 235;
  }
  else if (strcmp (name, "slateblue") == 0)
  {
    c[0] = 106;
    c[1] = 90;
    c[2] = 205;
  }
  else if (strcmp (name, "slategray") == 0)
  {
    c[0] = 112;
    c[1] = 128;
    c[2] = 144;
  }
  else if (strcmp (name, "slategrey") == 0)
  {
    c[0] = 112;
    c[1] = 128;
    c[2] = 144;
  }
  else if (strcmp (name, "snow") == 0)
  {
    c[0] = 255;
    c[1] = 250;
    c[2] = 250;
  }
  else if (strcmp (name, "springgreen") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 127;
  }
  else if (strcmp (name, "steelblue") == 0)
  {
    c[0] = 70;
    c[1] = 130;
    c[2] = 180;
  }
  else if (strcmp (name, "tan") == 0)
  {
    c[0] = 210;
    c[1] = 180;
    c[2] = 140;
  }
  else if (strcmp (name, "teal") == 0)
  {
    c[0] = 0;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "thistle") == 0)
  {
    c[0] = 216;
    c[1] = 191;
    c[2] = 216;
  }
  else if (strcmp (name, "tomato") == 0)
  {
    c[0] = 255;
    c[1] = 99;
    c[2] = 71;
  }
  else if (strcmp (name, "turquoise") == 0)
  {
    c[0] = 64;
    c[1] = 224;
    c[2] = 208;
  }
  else if (strcmp (name, "violet") == 0)
  {
    c[0] = 238;
    c[1] = 130;
    c[2] = 238;
  }
  else if (strcmp (name, "wheat") == 0)
  {
    c[0] = 245;
    c[1] = 222;
    c[2] = 179;
  }
  else if (strcmp (name, "white") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "whitesmoke") == 0)
  {
    c[0] = 245;
    c[1] = 245;
    c[2] = 245;
  }
  else if (strcmp (name, "yellow") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "yellowgreen") == 0)
  {
    c[0] = 154;
    c[1] = 205;
    c[2] = 50;
  }
  else
    res = -1;

  return res;
}

void
rgb_intensity (int *rgb, double intensity, int *rgb2)
{
  int i;

  /* intensity = 0 -> black */
  /* intensity = 1 -> white */
  /* intensity = 0.5 -> original color */
  if (intensity < 0)
    intensity = 0;
  if (intensity > 1)
    intensity = 1;

  if (intensity > -1e-6 && intensity < 0.5)
  {
    for (i = 0; i < 3; i++)
      rgb2[i] = (int) (2 * intensity * rgb[i]);
  }
  else if (intensity >= 0.5 && intensity < 1 + 1e-6)
  {
    for (i = 0; i < 3; i++)
      rgb2[i] =
	(int) (2 * (255 - (double) rgb[i]) * intensity - 255 +
	       2 * (double) rgb[i]);

  }

  return;
}


int
ut_color_scheme_val_color (char* scheme, double beg, double end,
			   double v, int* col)
{
  int i, pos;
  double pos2;
  char** color = NULL;
  int colqty;
  double* val = NULL;
  int* col0 = ut_alloc_1d_int (3);
  int* col1 = ut_alloc_1d_int (3);
  int status = 0;

  if (scheme != NULL)
    ut_string_separate (scheme, ",", &color, &colqty);
  else
    ut_string_separate ("blue,cyan,yellow,red", ",", &color, &colqty);

  val = ut_alloc_1d (colqty);
  for (i = 0; i < colqty; i++)
    val[i] = beg + (end - beg) * i / (colqty - 1);

  pos = ut_array_1d_valuepos (val, colqty, v);

  if (pos == -1)
  {
    status = -1;

    if (beg < end)
    {
      if (v < val[0])
	name2rgb (color[0], col);
      else
	name2rgb (color[colqty - 1], col);
    }
    else
    {
      if (v < val[0])
	name2rgb (color[colqty - 1], col);
      else
	name2rgb (color[0], col);
    }
  }
  else
  {
    if (pos < colqty - 1)
    {
      name2rgb (color[pos], col0);
      name2rgb (color[pos + 1], col1);
      if (val[pos + 1] - val[pos] != 0)
	pos2 = (v - val[pos]) / (val[pos + 1] - val[pos]);
      else
	pos2 = 0.5;

      for (i = 0; i < 3; i++)
	col[i] = ut_num_d2ri (col0[i] + pos2 * (col1[i] - col0[i]));
    }
    else
      name2rgb (color[pos], col);
  }

  ut_free_1d_int (col0);
  ut_free_1d_int (col1);
  ut_free_2d_char (color, colqty);
  ut_free_1d (val);

  return status;
}

void
ut_color_palette (int qty, int** palette, double min, double max)
{
  int i;
  int pqty = 137;
  int** p = ut_alloc_2d_int (pqty, 3);

  p[0][0] =   0; p[0][1] =   0; p[0][2] =   0;
  p[1][0] = 255; p[1][1] =   0; p[1][2] =   0;
  p[2][0] =   0; p[2][1] = 255; p[2][2] =   0;
  p[3][0] =   0; p[3][1] =   0; p[3][2] = 255;
  p[4][0] = 255; p[4][1] = 255; p[4][2] =   0;
  p[5][0] = 255; p[5][1] =   0; p[5][2] = 255;
  p[6][0] =   0; p[6][1] = 255; p[6][2] = 255;
  p[7][0] = 255; p[7][1] = 255; p[7][2] = 255;
  p[8][0] = 128; p[8][1] =   0; p[8][2] =   0;
  p[9][0] =   0; p[9][1] =   0; p[9][2] = 128;
  p[10][0] = 127; p[10][1] = 255; p[10][2] =   0;
  p[11][0] =   0; p[11][1] = 255; p[11][2] = 127;
  p[12][0] = 128; p[12][1] = 128; p[12][2] =   0;
  p[13][0] = 128; p[13][1] =   0; p[13][2] = 128;
  p[14][0] =   0; p[14][1] = 128; p[14][2] = 128;
  p[15][0] = 128; p[15][1] = 128; p[15][2] = 128;
  p[16][0] =   0; p[16][1] = 191; p[16][2] = 255;
  p[17][0] = 124; p[17][1] = 252; p[17][2] =   0;
  p[18][0] =  64; p[18][1] =  64; p[18][2] =  64;
  p[19][0] = 255; p[19][1] =  69; p[19][2] =   0;
  p[20][0] = 192; p[20][1] = 192; p[20][2] = 192;
  p[21][0] = 255; p[21][1] = 250; p[21][2] = 250;
  p[22][0] = 139; p[22][1] =   0; p[22][2] =   0;
  p[23][0] =   0; p[23][1] =   0; p[23][2] = 139;
  p[24][0] = 255; p[24][1] = 140; p[24][2] =   0;
  p[25][0] = 240; p[25][1] = 255; p[25][2] = 255;
  p[26][0] = 248; p[26][1] = 248; p[26][2] = 255;
  p[27][0] = 255; p[27][1] = 255; p[27][2] = 240;
  p[28][0] =   0; p[28][1] =   0; p[28][2] = 205;
  p[29][0] = 255; p[29][1] = 182; p[29][2] = 193;
  p[30][0] = 245; p[30][1] = 255; p[30][2] = 250;
  p[31][0] =  75; p[31][1] =   0; p[31][2] = 130;
  p[32][0] = 240; p[32][1] = 128; p[32][2] = 128;
  p[33][0] = 255; p[33][1] = 192; p[33][2] = 203;
  p[34][0] = 255; p[34][1] = 127; p[34][2] =  80;
  p[35][0] = 250; p[35][1] = 128; p[35][2] = 114;
  p[36][0] = 255; p[36][1] = 250; p[36][2] = 240;
  p[37][0] = 127; p[37][1] = 255; p[37][2] = 212;
  p[38][0] = 255; p[38][1] = 250; p[38][2] = 205;
  p[39][0] = 255; p[39][1] = 215; p[39][2] =   0;
  p[40][0] =   0; p[40][1] = 100; p[40][2] =   0;
  p[41][0] = 255; p[41][1] = 165; p[41][2] =   0;
  p[42][0] = 240; p[42][1] = 248; p[42][2] = 255;
  p[43][0] = 224; p[43][1] = 255; p[43][2] = 255;
  p[44][0] = 255; p[44][1] = 255; p[44][2] = 224;
  p[45][0] = 139; p[45][1] =   0; p[45][2] = 139;
  p[46][0] =   0; p[46][1] = 139; p[46][2] = 139;
  p[47][0] = 205; p[47][1] = 133; p[47][2] =  63;
  p[48][0] =  70; p[48][1] = 130; p[48][2] = 180;
  p[49][0] = 255; p[49][1] = 240; p[49][2] = 245;
  p[50][0] = 255; p[50][1] = 245; p[50][2] = 238;
  p[51][0] =   0; p[51][1] = 250; p[51][2] = 154;
  p[52][0] =  72; p[52][1] =  61; p[52][2] = 139;
  p[53][0] = 184; p[53][1] = 134; p[53][2] =  11;
  p[54][0] = 255; p[54][1] = 160; p[54][2] = 122;
  p[55][0] = 255; p[55][1] = 228; p[55][2] = 196;
  p[56][0] = 135; p[56][1] = 206; p[56][2] = 250;
  p[57][0] = 250; p[57][1] = 250; p[57][2] = 210;
  p[58][0] = 240; p[58][1] = 255; p[58][2] = 240;
  p[59][0] = 255; p[59][1] = 248; p[59][2] = 220;
  p[60][0] = 255; p[60][1] = 218; p[60][2] = 185;
  p[61][0] = 245; p[61][1] = 245; p[61][2] = 245;
  p[62][0] = 255; p[62][1] =  99; p[62][2] =  71;
  p[63][0] = 112; p[63][1] = 128; p[63][2] = 144;
  p[64][0] = 255; p[64][1] = 105; p[64][2] = 180;
  p[65][0] = 253; p[65][1] = 245; p[65][2] = 230;
  p[66][0] = 255; p[66][1] = 235; p[66][2] = 205;
  p[67][0] = 189; p[67][1] = 183; p[67][2] = 107;
  p[68][0] = 255; p[68][1] = 228; p[68][2] = 181;
  p[69][0] =   0; p[69][1] = 206; p[69][2] = 209;
  p[70][0] =  60; p[70][1] = 179; p[70][2] = 113;
  p[71][0] = 199; p[71][1] =  21; p[71][2] = 133;
  p[72][0] = 238; p[72][1] = 130; p[72][2] = 238;
  p[73][0] = 173; p[73][1] = 255; p[73][2] =  47;
  p[74][0] = 255; p[74][1] = 239; p[74][2] = 213;
  p[75][0] = 143; p[75][1] = 188; p[75][2] = 143;
  p[76][0] = 188; p[76][1] = 143; p[76][2] = 143;
  p[77][0] = 255; p[77][1] =  20; p[77][2] = 147;
  p[78][0] = 139; p[78][1] =  69; p[78][2] =  19;
  p[79][0] = 148; p[79][1] =   0; p[79][2] = 211;
  p[80][0] =  30; p[80][1] = 144; p[80][2] = 255;
  p[81][0] = 119; p[81][1] = 136; p[81][2] = 153;
  p[82][0] = 222; p[82][1] = 184; p[82][2] = 135;
  p[83][0] = 255; p[83][1] = 222; p[83][2] = 173;
  p[84][0] = 250; p[84][1] = 240; p[84][2] = 230;
  p[85][0] = 123; p[85][1] = 104; p[85][2] = 238;
  p[86][0] =  64; p[86][1] = 224; p[86][2] = 208;
  p[87][0] = 135; p[87][1] = 206; p[87][2] = 235;
  p[88][0] =  72; p[88][1] = 209; p[88][2] = 204;
  p[89][0] = 245; p[89][1] = 245; p[89][2] = 220;
  p[90][0] = 255; p[90][1] = 228; p[90][2] = 225;
  p[91][0] = 210; p[91][1] = 180; p[91][2] = 140;
  p[92][0] = 250; p[92][1] = 235; p[92][2] = 215;
  p[93][0] = 216; p[93][1] = 191; p[93][2] = 216;
  p[94][0] =  50; p[94][1] = 205; p[94][2] =  50;
  p[95][0] = 233; p[95][1] = 150; p[95][2] = 122;
  p[96][0] = 176; p[96][1] = 196; p[96][2] = 222;
  p[97][0] =  65; p[97][1] = 105; p[97][2] = 225;
  p[98][0] = 152; p[98][1] = 251; p[98][2] = 152;
  p[99][0] = 220; p[99][1] =  20; p[99][2] =  60;
  p[100][0] = 245; p[100][1] = 222; p[100][2] = 179;
  p[101][0] = 186; p[101][1] =  85; p[101][2] = 211;
  p[102][0] = 230; p[102][1] = 230; p[102][2] = 250;
  p[103][0] = 240; p[103][1] = 230; p[103][2] = 140;
  p[104][0] = 144; p[104][1] = 238; p[104][2] = 144;
  p[105][0] = 175; p[105][1] = 238; p[105][2] = 238;
  p[106][0] =  47; p[106][1] =  79; p[106][2] =  79;
  p[107][0] = 153; p[107][1] =  50; p[107][2] = 204;
  p[108][0] =  46; p[108][1] = 139; p[108][2] =  87;
  p[109][0] = 154; p[109][1] = 205; p[109][2] =  50;
  p[110][0] = 138; p[110][1] =  43; p[110][2] = 226;
  p[111][0] = 219; p[111][1] = 112; p[111][2] = 147;
  p[112][0] = 107; p[112][1] = 142; p[112][2] =  35;
  p[113][0] = 147; p[113][1] = 112; p[113][2] = 219;
  p[114][0] = 244; p[114][1] = 164; p[114][2] =  96;
  p[115][0] =  85; p[115][1] = 107; p[115][2] =  47;
  p[116][0] = 102; p[116][1] = 205; p[116][2] = 170;
  p[117][0] = 106; p[117][1] =  90; p[117][2] = 205;
  p[118][0] = 238; p[118][1] = 232; p[118][2] = 170;
  p[119][0] =  34; p[119][1] = 139; p[119][2] =  34;
  p[120][0] =  25; p[120][1] =  25; p[120][2] = 112;
  p[121][0] =  32; p[121][1] = 178; p[121][2] = 170;
  p[122][0] = 211; p[122][1] = 211; p[122][2] = 211;
  p[123][0] = 218; p[123][1] = 112; p[123][2] = 214;
  p[124][0] = 100; p[124][1] = 149; p[124][2] = 237;
  p[125][0] = 160; p[125][1] =  82; p[125][2] =  45;
  p[126][0] = 178; p[126][1] =  34; p[126][2] =  34;
  p[127][0] = 176; p[127][1] = 224; p[127][2] = 230;
  p[128][0] = 205; p[128][1] =  92; p[128][2] =  92;
  p[129][0] = 105; p[129][1] = 105; p[129][2] = 105;
  p[130][0] = 173; p[130][1] = 216; p[130][2] = 230;
  p[131][0] = 210; p[131][1] = 105; p[131][2] =  30;
  p[132][0] = 165; p[132][1] =  42; p[132][2] =  42;
  p[133][0] = 218; p[133][1] = 165; p[133][2] =  32;
  p[134][0] = 220; p[134][1] = 220; p[134][2] = 220;
  p[135][0] = 221; p[135][1] = 160; p[135][2] = 221;
  p[136][0] =  95; p[136][1] = 158; p[136][2] = 160;

  int id = 0;
  for (i = 0; i < qty; i++)
  {
    if (ut_array_1d_int_mean (p[id], 3) / 255 >= min
     && ut_array_1d_int_mean (p[id], 3) / 255 <= max)
      ut_array_1d_int_memcpy (palette[i], 3, p[id]);
    else
      i--;

    id = (id < pqty - 1) ? id + 1 : 0;
  }

  return;
}

void
ut_color_palette_0208 (int*** pp, int *pqty)
{
  (*pqty) = 92;

  (*pp) = ut_alloc_2d_int (*pqty, 3);

  (*pp)[0][0] = 255; (*pp)[0][1] = 0; (*pp)[0][2] = 0;
  (*pp)[1][0] = 0; (*pp)[1][1] = 255; (*pp)[1][2] = 0;
  (*pp)[2][0] = 0; (*pp)[2][1] = 0; (*pp)[2][2] = 255;
  (*pp)[3][0] = 255; (*pp)[3][1] = 255; (*pp)[3][2] = 0;
  (*pp)[4][0] = 255; (*pp)[4][1] = 0; (*pp)[4][2] = 255;
  (*pp)[5][0] = 0; (*pp)[5][1] = 255; (*pp)[5][2] = 255;
  (*pp)[6][0] = 127; (*pp)[6][1] = 255; (*pp)[6][2] = 0;
  (*pp)[7][0] = 0; (*pp)[7][1] = 255; (*pp)[7][2] = 127;
  (*pp)[8][0] = 128; (*pp)[8][1] = 128; (*pp)[8][2] = 0;
  (*pp)[9][0] = 128; (*pp)[9][1] = 0; (*pp)[9][2] = 128;
  (*pp)[10][0] = 0; (*pp)[10][1] = 128; (*pp)[10][2] = 128;
  (*pp)[11][0] = 128; (*pp)[11][1] = 128; (*pp)[11][2] = 128;
  (*pp)[12][0] = 0; (*pp)[12][1] = 191; (*pp)[12][2] = 255;
  (*pp)[13][0] = 124; (*pp)[13][1] = 252; (*pp)[13][2] = 0;
  (*pp)[14][0] = 64; (*pp)[14][1] = 64; (*pp)[14][2] = 64;
  (*pp)[15][0] = 255; (*pp)[15][1] = 69; (*pp)[15][2] = 0;
  (*pp)[16][0] = 192; (*pp)[16][1] = 192; (*pp)[16][2] = 192;
  (*pp)[17][0] = 255; (*pp)[17][1] = 140; (*pp)[17][2] = 0;
  (*pp)[18][0] = 0; (*pp)[18][1] = 0; (*pp)[18][2] = 205;
  (*pp)[19][0] = 75; (*pp)[19][1] = 0; (*pp)[19][2] = 130;
  (*pp)[20][0] = 240; (*pp)[20][1] = 128; (*pp)[20][2] = 128;
  (*pp)[21][0] = 255; (*pp)[21][1] = 127; (*pp)[21][2] = 80;
  (*pp)[22][0] = 250; (*pp)[22][1] = 128; (*pp)[22][2] = 114;
  (*pp)[23][0] = 127; (*pp)[23][1] = 255; (*pp)[23][2] = 212;
  (*pp)[24][0] = 255; (*pp)[24][1] = 215; (*pp)[24][2] = 0;
  (*pp)[25][0] = 255; (*pp)[25][1] = 165; (*pp)[25][2] = 0;
  (*pp)[26][0] = 139; (*pp)[26][1] = 0; (*pp)[26][2] = 139;
  (*pp)[27][0] = 0; (*pp)[27][1] = 139; (*pp)[27][2] = 139;
  (*pp)[28][0] = 205; (*pp)[28][1] = 133; (*pp)[28][2] = 63;
  (*pp)[29][0] = 70; (*pp)[29][1] = 130; (*pp)[29][2] = 180;
  (*pp)[30][0] = 0; (*pp)[30][1] = 250; (*pp)[30][2] = 154;
  (*pp)[31][0] = 72; (*pp)[31][1] = 61; (*pp)[31][2] = 139;
  (*pp)[32][0] = 184; (*pp)[32][1] = 134; (*pp)[32][2] = 11;
  (*pp)[33][0] = 255; (*pp)[33][1] = 160; (*pp)[33][2] = 122;
  (*pp)[34][0] = 135; (*pp)[34][1] = 206; (*pp)[34][2] = 250;
  (*pp)[35][0] = 255; (*pp)[35][1] = 99; (*pp)[35][2] = 71;
  (*pp)[36][0] = 112; (*pp)[36][1] = 128; (*pp)[36][2] = 144;
  (*pp)[37][0] = 255; (*pp)[37][1] = 105; (*pp)[37][2] = 180;
  (*pp)[38][0] = 189; (*pp)[38][1] = 183; (*pp)[38][2] = 107;
  (*pp)[39][0] = 0; (*pp)[39][1] = 206; (*pp)[39][2] = 209;
  (*pp)[40][0] = 60; (*pp)[40][1] = 179; (*pp)[40][2] = 113;
  (*pp)[41][0] = 199; (*pp)[41][1] = 21; (*pp)[41][2] = 133;
  (*pp)[42][0] = 238; (*pp)[42][1] = 130; (*pp)[42][2] = 238;
  (*pp)[43][0] = 173; (*pp)[43][1] = 255; (*pp)[43][2] = 47;
  (*pp)[44][0] = 143; (*pp)[44][1] = 188; (*pp)[44][2] = 143;
  (*pp)[45][0] = 188; (*pp)[45][1] = 143; (*pp)[45][2] = 143;
  (*pp)[46][0] = 255; (*pp)[46][1] = 20; (*pp)[46][2] = 147;
  (*pp)[47][0] = 139; (*pp)[47][1] = 69; (*pp)[47][2] = 19;
  (*pp)[48][0] = 148; (*pp)[48][1] = 0; (*pp)[48][2] = 211;
  (*pp)[49][0] = 30; (*pp)[49][1] = 144; (*pp)[49][2] = 255;
  (*pp)[50][0] = 119; (*pp)[50][1] = 136; (*pp)[50][2] = 153;
  (*pp)[51][0] = 222; (*pp)[51][1] = 184; (*pp)[51][2] = 135;
  (*pp)[52][0] = 123; (*pp)[52][1] = 104; (*pp)[52][2] = 238;
  (*pp)[53][0] = 64; (*pp)[53][1] = 224; (*pp)[53][2] = 208;
  (*pp)[54][0] = 135; (*pp)[54][1] = 206; (*pp)[54][2] = 235;
  (*pp)[55][0] = 72; (*pp)[55][1] = 209; (*pp)[55][2] = 204;
  (*pp)[56][0] = 210; (*pp)[56][1] = 180; (*pp)[56][2] = 140;
  (*pp)[57][0] = 50; (*pp)[57][1] = 205; (*pp)[57][2] = 50;
  (*pp)[58][0] = 233; (*pp)[58][1] = 150; (*pp)[58][2] = 122;
  (*pp)[59][0] = 176; (*pp)[59][1] = 196; (*pp)[59][2] = 222;
  (*pp)[60][0] = 65; (*pp)[60][1] = 105; (*pp)[60][2] = 225;
  (*pp)[61][0] = 152; (*pp)[61][1] = 251; (*pp)[61][2] = 152;
  (*pp)[62][0] = 220; (*pp)[62][1] = 20; (*pp)[62][2] = 60;
  (*pp)[63][0] = 186; (*pp)[63][1] = 85; (*pp)[63][2] = 211;
  (*pp)[64][0] = 240; (*pp)[64][1] = 230; (*pp)[64][2] = 140;
  (*pp)[65][0] = 144; (*pp)[65][1] = 238; (*pp)[65][2] = 144;
  (*pp)[66][0] = 47; (*pp)[66][1] = 79; (*pp)[66][2] = 79;
  (*pp)[67][0] = 153; (*pp)[67][1] = 50; (*pp)[67][2] = 204;
  (*pp)[68][0] = 46; (*pp)[68][1] = 139; (*pp)[68][2] = 87;
  (*pp)[69][0] = 154; (*pp)[69][1] = 205; (*pp)[69][2] = 50;
  (*pp)[70][0] = 138; (*pp)[70][1] = 43; (*pp)[70][2] = 226;
  (*pp)[71][0] = 219; (*pp)[71][1] = 112; (*pp)[71][2] = 147;
  (*pp)[72][0] = 107; (*pp)[72][1] = 142; (*pp)[72][2] = 35;
  (*pp)[73][0] = 147; (*pp)[73][1] = 112; (*pp)[73][2] = 219;
  (*pp)[74][0] = 244; (*pp)[74][1] = 164; (*pp)[74][2] = 96;
  (*pp)[75][0] = 85; (*pp)[75][1] = 107; (*pp)[75][2] = 47;
  (*pp)[76][0] = 102; (*pp)[76][1] = 205; (*pp)[76][2] = 170;
  (*pp)[77][0] = 106; (*pp)[77][1] = 90; (*pp)[77][2] = 205;
  (*pp)[78][0] = 34; (*pp)[78][1] = 139; (*pp)[78][2] = 34;
  (*pp)[79][0] = 25; (*pp)[79][1] = 25; (*pp)[79][2] = 112;
  (*pp)[80][0] = 32; (*pp)[80][1] = 178; (*pp)[80][2] = 170;
  (*pp)[81][0] = 218; (*pp)[81][1] = 112; (*pp)[81][2] = 214;
  (*pp)[82][0] = 100; (*pp)[82][1] = 149; (*pp)[82][2] = 237;
  (*pp)[83][0] = 160; (*pp)[83][1] = 82; (*pp)[83][2] = 45;
  (*pp)[84][0] = 178; (*pp)[84][1] = 34; (*pp)[84][2] = 34;
  (*pp)[85][0] = 205; (*pp)[85][1] = 92; (*pp)[85][2] = 92;
  (*pp)[86][0] = 105; (*pp)[86][1] = 105; (*pp)[86][2] = 105;
  (*pp)[87][0] = 210; (*pp)[87][1] = 105; (*pp)[87][2] = 30;
  (*pp)[88][0] = 165; (*pp)[88][1] = 42; (*pp)[88][2] = 42;
  (*pp)[89][0] = 218; (*pp)[89][1] = 165; (*pp)[89][2] = 32;
  (*pp)[90][0] = 221; (*pp)[90][1] = 160; (*pp)[90][2] = 221;
  (*pp)[91][0] = 95; (*pp)[91][1] = 158; (*pp)[91][2] = 160;

  return;
}
