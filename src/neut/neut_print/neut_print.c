/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_print_.h"

void
neut_print_set_default (struct PRINT *pPrint)
{
  // camera ------------------------------------------------------------
  (*pPrint).cameracoo = ut_alloc_1d (3);
  (*pPrint).cameralookat = ut_alloc_1d (3);
  (*pPrint).camerasky = ut_alloc_1d (3);
  (*pPrint).cameraangle = 25;
  (*pPrint).cameraprojection = ut_alloc_1d_char (100);

  // light ------------------------------------------------------------
  (*pPrint).lightsourceqty = 0;
  (*pPrint).lightsources = NULL;
  (*pPrint).lightambient = 0;
  (*pPrint).lightdiffuse = 0;
  (*pPrint).lightreflection = 0;

  (*pPrint).showtess = -1;
  (*pPrint).showtesr = -1;
  (*pPrint).showmesh = -1;
  (*pPrint).showslice = -1;
  (*pPrint).showsliceelt1d = -1;

  (*pPrint).inputqty = 0;
  (*pPrint).inputs = NULL;

  (*pPrint).showseed = ut_alloc_1d_int (1);
  (*pPrint).showcrystal = ut_alloc_1d_int (1);
  (*pPrint).showver = ut_alloc_1d_int (1);
  (*pPrint).showedge = ut_alloc_1d_int (1);
  (*pPrint).showface = ut_alloc_1d_int (1);
  (*pPrint).showfaceinter = 0;
  (*pPrint).showpoly = ut_alloc_1d_int (1);
  (*pPrint).shownode = ut_alloc_1d_int (1);
  (*pPrint).showelset0d = ut_alloc_1d_int (1);
  (*pPrint).showelset1d = ut_alloc_1d_int (1);
  (*pPrint).showelset2d = ut_alloc_1d_int (1);
  (*pPrint).showelset3d = ut_alloc_1d_int (1);
  (*pPrint).showelt0d = ut_alloc_1d_int (1);
  (*pPrint).showelt1d = ut_alloc_1d_int (1);
  (*pPrint).showelt2d = ut_alloc_1d_int (1);
  (*pPrint).showelt3d = ut_alloc_1d_int (1);
  (*pPrint).showpoint = NULL;
  (*pPrint).showcrystal[0] = -1;
  (*pPrint).showseed[0] = -1;
  (*pPrint).showver[0] = -1;
  (*pPrint).showedge[0] = -1;
  (*pPrint).showface[0] = -1;
  (*pPrint).showpoly[0] = -1;
  (*pPrint).shownode[0] = -1;
  (*pPrint).showelt0d[0] = -1;
  (*pPrint).showelt1d[0] = -1;
  (*pPrint).showelt2d[0] = -1;
  (*pPrint).showelt3d[0] = -1;
  (*pPrint).showelset0d[0] = -1;
  (*pPrint).showelset1d[0] = -1;
  (*pPrint).showelset2d[0] = -1;
  (*pPrint).showelset3d[0] = -1;
  (*pPrint).showcsys = -1;
  (*pPrint).showvox = NULL;
  (*pPrint).showvoxstring = NULL;
  (*pPrint).showvoidvoxstring = NULL;
  (*pPrint).showedgestring = NULL;
  (*pPrint).showscale = -1;

  (*pPrint).datareduction = 1;

  return;
}

void
neut_print_free (struct PRINT *pPrint)
{
  // camera ------------------------------------------------------------
  ut_free_1d (&(*pPrint).cameracoo);

  ut_free_1d (&(*pPrint).cameralookat);

  ut_free_1d (&(*pPrint).camerasky);

  ut_free_1d_char (&(*pPrint).cameraprojection);

  // light ------------------------------------------------------------
  ut_free_2d_char (&(*pPrint).lightsources, (*pPrint).lightsourceqty);

  // image -------------------------------------------------------------
  ut_free_1d_int (&(*pPrint).showseed);
  ut_free_1d_int (&(*pPrint).showcrystal);
  ut_free_1d_int (&(*pPrint).showver);
  ut_free_1d_int (&(*pPrint).showedge);
  ut_free_1d_int (&(*pPrint).showface);
  ut_free_1d_int (&(*pPrint).showpoly);
  ut_free_1d_int (&(*pPrint).shownode);
  ut_free_1d_int (&(*pPrint).showelt3d);
  ut_free_1d_int (&(*pPrint).showelt2d);
  ut_free_1d_int (&(*pPrint).showelt1d);
  ut_free_1d_int (&(*pPrint).showelt0d);
  ut_free_2d_int (&(*pPrint).showpoint, (*pPrint).inputqty);
  ut_free_2d_char (&(*pPrint).inputs, (*pPrint).inputqty);

  return;
}

void
neut_print_outdir (char *outdir, struct SIM Sim, char *format, char **pdir)
{
  int i, qty;
  char **parts = NULL;

  ut_list_break (outdir, NEUT_SEP_NODEP, &parts, &qty);

  ut_string_string (".", pdir);

  for (i = 0; i < qty; i++)
  {
    if (!strcmp (parts[i], "sim_dir"))
    {
      if (!neut_sim_isvoid (Sim))
      {
        (*pdir) = ut_string_paste3 (Sim.simdir, "/images/", format);
        break;
      }
    }

    else
    {
      ut_string_string (parts[i], pdir);
      break;
    }
  }

  ut_free_2d_char (&parts, qty);

  return;
}

void
neut_print_imagesize (char *imagesize, int *pwidth, int *pheight)
{
  int valqty;
  char **vals = NULL;

  ut_list_break (imagesize, NEUT_SEP_DEP, &vals, &valqty);
  if (valqty != 2)
    ut_print_message (2, 2, "Expression `%s' could not be processed.\n",
                      imagesize);
  if (pwidth)
    ut_string_int (vals[0], pwidth);
  if (pheight)
    ut_string_int (vals[1], pheight);
  ut_free_2d_char (&vals, valqty);

  return;
}

int
neut_print_cell_show (struct PRINT Print, struct TESS Tess, int cell)
{
  if (Tess.Dim == 2)
    return Print.showface[cell];
  else if (Tess.Dim == 3)
    return Print.showpoly[cell];
  else
    abort ();
}
