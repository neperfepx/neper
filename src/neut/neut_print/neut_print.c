/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_print_.h"

void
neut_print_set_default (struct PRINT *pPrint)
{
  // povray ------------------------------------------------------------
  (*pPrint).povray = NULL;
  ut_string_string ("povray", &((*pPrint).povray));

  // camera ------------------------------------------------------------
  (*pPrint).cameracoostring = ut_alloc_1d_char (100);
  sprintf ((*pPrint).cameracoostring, "x+length*vx%sy+length*vy%sz+length*vz",
	   NEUT_SEP_DEP, NEUT_SEP_DEP);
  (*pPrint).cameracoo = ut_alloc_1d (3);

  (*pPrint).cameralookatstring = ut_alloc_1d_char (100);
  sprintf ((*pPrint).cameralookatstring, "x%sy%sz",
	   NEUT_SEP_DEP, NEUT_SEP_DEP);
  (*pPrint).cameralookat = ut_alloc_1d (3);

  (*pPrint).cameraskystring = ut_alloc_1d_char (100);
  sprintf ((*pPrint).cameraskystring, "0%s0%s1", NEUT_SEP_DEP, NEUT_SEP_DEP);
  (*pPrint).camerasky = ut_alloc_1d (3);
  (*pPrint).camerasky[2] = 1;

  (*pPrint).cameraangle = 25;

  (*pPrint).cameraprojection = ut_alloc_1d_char (100);
  strcpy ((*pPrint).cameraprojection, "default");

  // image -------------------------------------------------------------
  (*pPrint).imagesize = ut_alloc_1d_char (100);
  sprintf ((*pPrint).imagesize, "1200%s900", NEUT_SEP_DEP);
  (*pPrint).imagebackground = ut_alloc_1d_char (6);
  strcpy ((*pPrint).imagebackground, "white");
  (*pPrint).imageantialias = 1;

  (*pPrint).showtess = -1;
  (*pPrint).showtesr = -1;
  (*pPrint).showmesh = -1;
  (*pPrint).showslice = -1;

  (*pPrint).showseed = ut_alloc_1d_int (1);
  (*pPrint).showver = ut_alloc_1d_int (1);
  (*pPrint).showedge = ut_alloc_1d_int (1);
  (*pPrint).showface = ut_alloc_1d_int (1);
  (*pPrint).showfaceinter = 0;
  (*pPrint).showpoly = ut_alloc_1d_int (1);
  (*pPrint).showelt3d = ut_alloc_1d_int (1);
  (*pPrint).shownode = ut_alloc_1d_int (1);
  (*pPrint).showelt0d = ut_alloc_1d_int (1);
  (*pPrint).showelt1d = ut_alloc_1d_int (1);
  (*pPrint).showelt2d = ut_alloc_1d_int (1);
  (*pPrint).showpoint = ut_alloc_1d_int (1);
  (*pPrint).showseed[0] = -1;
  (*pPrint).showver[0] = -1;
  (*pPrint).showedge[0] = -1;
  (*pPrint).showface[0] = -1;
  (*pPrint).showpoly[0] = -1;
  (*pPrint).showelt3d[0] = -1;
  (*pPrint).shownode[0] = -1;
  (*pPrint).showelt0d[0] = -1;
  (*pPrint).showelt1d[0] = -1;
  (*pPrint).showelt2d[0] = -1;
  (*pPrint).showpoint[0] = -1;
  (*pPrint).showshadow = -1;
  (*pPrint).showcsys = -1;
  (*pPrint).showvox = NULL;

  (*pPrint).datareduction = 1;

  (*pPrint).format = ut_alloc_1d_char (10);
  sprintf ((*pPrint).format, "png");

  (*pPrint).includepov = NULL;

  (*pPrint).slice = NULL;

  return;
}

void
neut_print_free (struct PRINT *pPrint)
{
  // camera ------------------------------------------------------------
  ut_free_1d_char ((*pPrint).cameracoostring);
  ut_free_1d ((*pPrint).cameracoo);

  ut_free_1d_char ((*pPrint).cameralookatstring);
  ut_free_1d ((*pPrint).cameralookat);

  ut_free_1d_char ((*pPrint).cameraskystring);
  ut_free_1d ((*pPrint).camerasky);

  ut_free_1d_char ((*pPrint).cameraprojection);

  // image -------------------------------------------------------------
  ut_free_1d_char ((*pPrint).imagesize);
  ut_free_1d_char ((*pPrint).imagebackground);
  ut_free_1d_int ((*pPrint).showseed);
  ut_free_1d_int ((*pPrint).showver);
  ut_free_1d_int ((*pPrint).showedge);
  ut_free_1d_int ((*pPrint).showface);
  ut_free_1d_int ((*pPrint).showpoly);
  ut_free_1d_int ((*pPrint).shownode);
  ut_free_1d_int ((*pPrint).showelt3d);
  ut_free_1d_int ((*pPrint).showelt2d);
  ut_free_1d_int ((*pPrint).showelt1d);
  ut_free_1d_int ((*pPrint).showelt0d);
  ut_free_1d_int ((*pPrint).showpoint);

  ut_free_1d_char ((*pPrint).format);
  ut_free_1d_char ((*pPrint).includepov);

  return;
}
