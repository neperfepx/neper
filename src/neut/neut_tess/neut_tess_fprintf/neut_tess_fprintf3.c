/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fprintf_.h"

void
neut_tess_fprintf_per_gen (struct TESS Tess, FILE * file)
{
  if (Tess.Level == 0)
    return;

  if (Tess.Type[0] != 'p')
    return;

  fprintf (file, " **periodicity\n");
  fprintf (file, "  *general\n");
  fprintf (file, "   ");
  ut_array_1d_int_fprintf (file, Tess.Periodic, 3, "%d");
  fprintf (file, "   ");
  ut_array_1d_fprintf (file, Tess.PeriodicDist, 3, REAL_PRINT_FORMAT);

  return;
}

void
neut_tess_fprintf_per_seed (struct TESS Tess, FILE * file)
{
  int i, shift, id;

  shift = Tess.SeedQty - Tess.PerSeedQty;

  fprintf (file, "  *seed\n");
  fprintf (file, "   %d\n", Tess.PerSeedQty);
  for (i = 1; i <= Tess.PerSeedQty; i++)
  {
    id = i + shift;
    fprintf (file, "   %d ", id);
    fprintf (file, "%d ", Tess.PerSeedMaster[id]);
    ut_array_1d_int_fprintf (file, Tess.PerSeedShift[id], 3, "%d");
  }

  return;
}

void
neut_tess_fprintf_per_ver (struct TESS Tess, FILE * file)
{
  int i, id;

  fprintf (file, "  *vertex\n");
  fprintf (file, "   %d\n", Tess.PerVerQty);
  for (i = 1; i <= Tess.PerVerQty; i++)
  {
    id = Tess.PerVerNb[i];
    fprintf (file, "   %d ", id);
    fprintf (file, "%d ", Tess.PerVerMaster[id]);
    ut_array_1d_int_fprintf (file, Tess.PerVerShift[id], 3, "%d");
  }

  return;
}

void
neut_tess_fprintf_per_edge (struct TESS Tess, FILE * file)
{
  int i, id;

  fprintf (file, "  *edge\n");
  fprintf (file, "   %d\n", Tess.PerEdgeQty);
  for (i = 1; i <= Tess.PerEdgeQty; i++)
  {
    id = Tess.PerEdgeNb[i];
    fprintf (file, "   %d ", id);
    fprintf (file, "%d ", Tess.PerEdgeMaster[id]);
    ut_array_1d_int_fprintf_nonl (file, Tess.PerEdgeShift[id], 3, "%d");
    fprintf (file, " %d\n", Tess.PerEdgeOri[id]);
  }

  return;
}

void
neut_tess_fprintf_per_face (struct TESS Tess, FILE * file)
{
  int i, id;

  if (Tess.Dim == 2)
    return;

  fprintf (file, "  *face\n");
  fprintf (file, "   %d\n", Tess.PerFaceQty);
  for (i = 1; i <= Tess.PerFaceQty; i++)
  {
    id = Tess.PerFaceNb[i];
    fprintf (file, "   %d ", id);
    fprintf (file, "%d ", Tess.PerFaceMaster[id]);
    ut_array_1d_int_fprintf_nonl (file, Tess.PerFaceShift[id], 3, "%d");
    fprintf (file, " %d\n", Tess.PerFaceOri[id]);
  }

  return;
}

void
neut_tess_fprintf_scale_gen (struct TESS Tess, FILE * file)
{
  fprintf (file, " **scale\n");
  fprintf (file, "  *general\n");
  fprintf (file, "   %d\n", Tess.ScaleQty);

  return;
}

void
neut_tess_fprintf_scale_cellid (struct TESS Tess, FILE * file)
{
  int i;

  fprintf (file, "  *cellid\n");
  for (i = 1; i <= Tess.CellQty; i++)
  {
    fprintf (file, "   %d ", i);
    ut_array_1d_int_fprintf (file, Tess.ScaleCellId[i] + 1, Tess.ScaleQty,
                             "%d");
  }

  return;
}
