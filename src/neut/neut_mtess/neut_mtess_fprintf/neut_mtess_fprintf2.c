/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fprintf_.h"

void
neut_mtess_fprintf_header (FILE * file, struct MTESS MTess, struct TESS *Tess)
{
  int i;

  fprintf (file, "***mtess\n");
  fprintf (file, " **format\n");
  fprintf (file, "   2.0\n");
  fprintf (file, " **general\n");
  fprintf (file, " %3d\n", MTess.LevelQty);
  for (i = 1; i <= MTess.LevelQty; i++)
  {
    fprintf (file, "   %d %d ", i, MTess.LevelTessQty[i]);
    ut_array_1d_int_fprintf (file, MTess.LevelTess[i] + 1,
			     MTess.LevelTessQty[i], "%d");
  }

  fprintf (file, " %3d\n", MTess.TessQty);
  for (i = 1; i <= MTess.TessQty; i++)
  {
    fprintf (file, "   %d ", i);
    ut_array_1d_int_fprintf (file, MTess.TessDom[i], 2, "%d");
  }

  fprintf (file, " %3d\n", MTess.TessDQty);
  for (i = 1; i <= MTess.TessDQty; i++)
  {
    fprintf (file, "   %d %d ", i, Tess[i].PolyQty);
    ut_array_1d_int_fprintf (file, MTess.DomTess[i] + 1, Tess[i].PolyQty,
			     "%d");
  }

  fprintf (file, "***end\n");

  return;
}

void
neut_mtess_fprintf_tess (FILE * file, struct MTESS MTess, struct TESS *Tess)
{
  int i;

  for (i = 0; i <= MTess.TessQty; i++)
    neut_tess_fprintf (file, Tess[i]);

  return;
}
