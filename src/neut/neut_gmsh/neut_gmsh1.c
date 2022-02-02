/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_gmsh_.h"

/* This is to deal with the default config and option files of gmsh. */
void
neut_gmsh_rc (char *action)
{
  char *s1 = ut_alloc_1d_char (1000);
  char *s2 = ut_alloc_1d_char (1000);

  sprintf (s1, "%s/.gmshrc", getenv ("HOME"));
  sprintf (s2, "%s.bak", s1);

  if (strcmp (action, "bak") == 0)
    rename (s1, s2);
  else if (strcmp (action, "unbak") == 0)
    rename (s2, s1);
  else
    abort ();

  sprintf (s1, "%s/.gmsh-options", getenv ("HOME"));
  sprintf (s2, "%s.bak", s1);

  if (strcmp (action, "bak") == 0)
    rename (s1, s2);
  else if (strcmp (action, "unbak") == 0)
    rename (s2, s1);
  else
    abort ();

  ut_free_1d_char (&s1);
  ut_free_1d_char (&s2);

  return;
}

int
neut_gmsh_meshalgo2d_nb (char *algo, int *pnb)
{
  if (algo == NULL)
    (*pnb) = -1;
  else if (strcmp (algo, "mead") == 0)
    (*pnb) = 1;
  else if (strcmp (algo, "dela") == 0)
    (*pnb) = 5;
  else if (strcmp (algo, "fron") == 0)
    (*pnb) = 6;
  else
    (*pnb) = -1;

  return ((*pnb) < 0) ? -1 : 0;
}

int
neut_gmsh_meshalgo3d_nb (char *algo, int *pnb)
{
  if (algo == NULL)
    (*pnb) = -1;
  else if (strcmp (algo, "dela") == 0)
    (*pnb) = 1;
  else if (strcmp (algo, "fron") == 0)
    (*pnb) = 4;
  else if (strcmp (algo, "netg") == 0)
    (*pnb) = 4;
  else
    (*pnb) = -1;

  return ((*pnb) < 0) ? -1 : 0;
}

int
neut_gmsh_optialgo_nb (char *algo, int *pnb)
{
  if (algo == NULL || strcmp (algo, "none") == 0)
    (*pnb) = -1;
  else if (strcmp (algo, "gmsh") == 0)
    (*pnb) = 1;
  else if (strcmp (algo, "netg") == 0)
    (*pnb) = 2;
  else if (strcmp (algo, "gmne") == 0)
    (*pnb) = 3;
  else
    (*pnb) = -2;

  return ((*pnb) == -2) ? -1 : 0;
}

int
neut_gmsh_elt_nb (char *type, int dim, int order, int *pnb)
{
  if (strcmp (type, "tri") == 0)
  {
    if (dim == 0 && order == 1)
      (*pnb) = 15;
    else if (dim == 0 && order == 2)
      (*pnb) = 15;
    else if (dim == 1 && order == 1)
      (*pnb) = 1;
    else if (dim == 1 && order == 2)
      (*pnb) = 8;
    else if (dim == 2 && order == 1)
      (*pnb) = 2;
    else if (dim == 2 && order == 2)
      (*pnb) = 9;
    else if (dim == 3 && order == 1)
      (*pnb) = 4;
    else if (dim == 3 && order == 2)
      (*pnb) = 11;
    else
      (*pnb) = -1;
  }
  else if (strcmp (type, "quad") == 0)
  {
    if (dim == 0 && order == 1)
      (*pnb) = 15;
    else if (dim == 0 && order == 2)
      (*pnb) = 15;
    else if (dim == 1 && order == 1)
      (*pnb) = 1;
    else if (dim == 1 && order == 2)
      (*pnb) = 8;
    else if (dim == 2 && order == 1)
      (*pnb) = 3;
    else if (dim == 2 && order == 2)
      (*pnb) = 16;
    else if (dim == 3 && order == 1)
      (*pnb) = 5;
    else if (dim == 3 && order == 2)
      (*pnb) = 17;
    else
      (*pnb) = -1;
  }
  else if (strcmp (type, "quad9") == 0)
  {
    if (dim == 2 && order == 1)
      (*pnb) = 3;
    else if (dim == 2 && order == 2)
      (*pnb) = 10;
    else
      (*pnb) = -1;
  }
  else if (strcmp (type, "quad6") == 0)
  {
    if (dim == 2)
      (*pnb) = 100;
    else
      (*pnb) = -1;
  }
  else if (strcmp (type, "triprism") == 0)
  {
    if (dim == 0 && order == 1)
      (*pnb) = 0;
    else if (dim == 0 && order == 2)
      (*pnb) = 0;
    else if (dim == 1 && order == 1)
      (*pnb) = 0;
    else if (dim == 1 && order == 2)
      (*pnb) = 0;
    else if (dim == 2 && order == 1)
      (*pnb) = 4;
    else if (dim == 2 && order == 2)
      (*pnb) = 8;
    else if (dim == 3 && order == 1)
      (*pnb) = 6;
    else if (dim == 3 && order == 2)
      (*pnb) = 18;
    else
      (*pnb) = -1;
  }
  else
    (*pnb) = -1;

  return ((*pnb) > 0) ? 0 : -1;
}

int
neut_gmsh_sphere_tess_fprintf (FILE * file, double Rad, double cl)
{
  fprintf (file, "Rad = %.12f;\n", Rad);
  fprintf (file, "cl = %.12f;\n", cl);

  fprintf (file, "Point(1) = {0, 0, 0, cl};\n");
  fprintf (file, "Point(2) = {Rad, 0, 0, cl};\n");
  fprintf (file, "Point(3) = {-Rad, 0, 0, cl};\n");
  fprintf (file, "Point(4) = {0, Rad, 0, cl};\n");
  fprintf (file, "Point(5) = {0, -Rad, 0, cl};\n");
  fprintf (file, "Point(6) = {0, 0, Rad, cl};\n");
  fprintf (file, "Point(7) = {0, 0, -Rad, cl};\n");
  fprintf (file, "Circle(1) = {2, 1, 4};\n");
  fprintf (file, "Circle(2) = {4, 1, 3};\n");
  fprintf (file, "Circle(3) = {3, 1, 5};\n");
  fprintf (file, "Circle(4) = {5, 1, 2};\n");
  fprintf (file, "Circle(5) = {2, 1, 6};\n");
  fprintf (file, "Circle(6) = {6, 1, 3};\n");
  fprintf (file, "Circle(7) = {3, 1, 7};\n");
  fprintf (file, "Circle(8) = {7, 1, 2};\n");
  fprintf (file, "Circle(9) = {6, 1, 4};\n");
  fprintf (file, "Circle(10) = {4, 1, 7};\n");
  fprintf (file, "Circle(11) = {7, 1, 5};\n");
  fprintf (file, "Circle(12) = {5, 1, 6};\n");
  fprintf (file, "Line Loop(14) = {1, -9, -5};\n");
  fprintf (file, "Ruled Surface(14) = {14};\n");
  fprintf (file, "Line Loop(16) = {5, -12, 4};\n");
  fprintf (file, "Ruled Surface(16) = {16};\n");
  fprintf (file, "Line Loop(18) = {12, 6, 3};\n");
  fprintf (file, "Ruled Surface(18) = {18};\n");
  fprintf (file, "Line Loop(20) = {6, -2, -9};\n");
  fprintf (file, "Ruled Surface(20) = {20};\n");
  fprintf (file, "Line Loop(22) = {4, -8, 11};\n");
  fprintf (file, "Ruled Surface(22) = {22};\n");
  fprintf (file, "Line Loop(24) = {1, 10, 8};\n");
  fprintf (file, "Ruled Surface(24) = {24};\n");
  fprintf (file, "Line Loop(26) = {10, -7, -2};\n");
  fprintf (file, "Ruled Surface(26) = {26};\n");
  fprintf (file, "Line Loop(28) = {7, 11, -3};\n");
  fprintf (file, "Ruled Surface(28) = {28};\n");
  fprintf (file, "Surface Loop(30) = {14, 24, 26, 28, 22, 16, 18, 20};\n");
  fprintf (file, "Volume(30) = {30};\n");

  return 0;
}
