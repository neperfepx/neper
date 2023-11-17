/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fprintf_.h"

void
neut_tess_fprintf_head (struct TESS Tess, FILE * file)
{
  fprintf (file, "***tess\n");
  fprintf (file, " **format\n");
  fprintf (file, "   3.4\n");
  fprintf (file, " **general\n");
  fprintf (file, "   %d %s\n", Tess.Dim, Tess.Type);

  return;
}

void
neut_tess_fprintf_foot (FILE * file)
{
  fprintf (file, "***end\n");

  return;
}

void
neut_tess_fprintf_cell (struct TESS Tess, FILE * file)
{
  int i, qty;
  double *e = ol_e_alloc ();

  fprintf (file, " **cell\n");
  fprintf (file, "  %d\n", Tess.CellQty);

  if (Tess.CellId)
  {
    fprintf (file, "  *id\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellLamId)
  {
    fprintf (file, "  *lam\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellLamId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellModeId
      && ut_array_1d_int_valnb (Tess.CellModeId + 1, Tess.CellQty, 1) != Tess.CellQty)
  {
    fprintf (file, "  *mode\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellModeId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellGroup)
  {
    fprintf (file, "  *group\n");
    qty = 0;
    for (i = 1; i <= Tess.CellQty; i++)
      ut_print_wnc_int_header (file, Tess.CellGroup[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.CellCrySym)
  {
    fprintf (file, "  *crysym\n");
    fprintf (file, "   %s\n", Tess.CellCrySym);
  }

  if (Tess.SeedCoo && Tess.SeedWeight)
  {
    fprintf (file, "  *seed\n");
    // do not change CellQty into SeedQty below, to handle properly the
    // periodic case.
    for (i = 1; i <= Tess.CellQty; i++)
    {
      fprintf (file, " %3d ", i);
      ut_array_1d_fprintf_nonl (file, Tess.SeedCoo[i], 3, REAL_PRINT_FORMAT);
      fprintf (file, " " REAL_PRINT_FORMAT "\n", Tess.SeedWeight[i]);
    }
  }

  if (Tess.CellOri)
  {
    fprintf (file, "  *ori\n");
    fprintf (file, "   %s\n", Tess.CellOriDes);
    neut_ori_fprintf (file, Tess.CellOriDes, "ascii", Tess.CellOri + 1, NULL, NULL, Tess.CellQty, NULL);
  }

  if (neut_tess_hascellorispread (Tess))
  {
    fprintf (file, "  *orispread\n");
    for (i = 1; i <= Tess.CellQty; i++)
      fprintf (file, "   %s\n", Tess.CellOriDistrib[i]);
  }

  if (Tess.CellWeight)
  {
    fprintf (file, "  *weight\n");
    for (i = 1; i <= Tess.CellQty; i++)
      fprintf (file, "   " REAL_PRINT_FORMAT "\n", Tess.CellWeight[i]);
  }

  ol_e_free (e);

  return;
}

void
neut_tess_fprintf_ver (struct TESS Tess, FILE * file)
{
  int i;

  fprintf (file, " **vertex\n");
  fprintf (file, " %d\n", Tess.VerQty);

  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, " %3d  ", i);
    ut_array_1d_fprintf_nonl (file, Tess.VerCoo[i], 3, REAL_PRINT_FORMAT);
    fprintf (file, "     %d\n", Tess.VerState[i]);
  }

  return;
}

void
neut_tess_fprintf_edge (struct TESS Tess, FILE * file)
{
  int i;

  fprintf (file, " **edge\n");
  fprintf (file, " %d\n", Tess.EdgeQty);

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, " %d %d %d\n", Tess.EdgeVerNb[i][0], Tess.EdgeVerNb[i][1],
             Tess.EdgeState[i]);
  }

  return;
}

void
neut_tess_fprintf_face (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **face\n");
  fprintf (file, " %d\n", Tess.FaceQty);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, " %3d", i);

    fprintf (file, " %d%s", Tess.FaceVerQty[i],
             Tess.FaceVerQty[i] ? " " : "");
    ut_array_1d_int_fprintf (file, Tess.FaceVerNb[i] + 1, Tess.FaceVerQty[i],
                             "%d");

    fprintf (file, "     %d", Tess.FaceVerQty[i]);
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, " %d", Tess.FaceEdgeOri[i][j] * Tess.FaceEdgeNb[i][j]);
    fprintf (file, "\n");

    fprintf (file, "    ");
    ut_array_1d_fprintf (file, Tess.FaceEq[i], 4, REAL_PRINT_FORMAT);

    fprintf (file, "    ");
    fprintf (file, " %d", Tess.FaceState[i]);
    fprintf (file, " %d ", Tess.FacePt[i]);
    ut_array_1d_fprintf (file, Tess.FacePtCoo[i], 3, REAL_PRINT_FORMAT);
  }

  return;
}

void
neut_tess_fprintf_poly (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **polyhedron\n");
  fprintf (file, " %d\n", Tess.PolyQty);

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, " %3d %d", i, Tess.PolyFaceQty[i]);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      fprintf (file, " %d", Tess.PolyFaceOri[i][j] * Tess.PolyFaceNb[i][j]);
    fprintf (file, "\n");
  }

  return;
}

void
neut_tess_fprintf_dom (struct TESS Tess, FILE * file)
{
  int i;

  if (Tess.Level == 0)
    return;

  fprintf (file, " **domain\n");
  fprintf (file, "  *general\n");
  fprintf (file, "   %s\n", Tess.DomType);

  fprintf (file, "  *vertex\n");
  fprintf (file, "   %d\n", Tess.DomVerQty);

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    fprintf (file, " %3d  ", i);
    ut_array_1d_fprintf_nonl (file, Tess.DomVerCoo[i], 3, REAL_PRINT_FORMAT);
    fprintf (file, " %s\n", Tess.DomVerLabel[i]);
    fprintf (file, "    %3d %d\n", 1, Tess.DomTessVerNb[i]);
  }

  if (Tess.Dim > 1)
  {
    fprintf (file, "  *edge\n");
    fprintf (file, "   %d\n", Tess.DomEdgeQty);

    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      fprintf (file, " %3d ", i);
      fprintf (file, "%d%s", Tess.DomEdgeVerQty[i],
               Tess.DomEdgeVerQty[i] ? " " : "");
      ut_array_1d_int_fprintf (file, Tess.DomEdgeVerNb[i],
                               Tess.DomEdgeVerQty[i], "%d");
      fprintf (file, "     %s\n", Tess.DomEdgeLabel[i]);
      fprintf (file, "     %d%s", Tess.DomTessEdgeQty[i],
               Tess.DomTessEdgeQty[i] ? " " : "");
      ut_array_1d_int_fprintf (file, Tess.DomTessEdgeNb[i] + 1,
                               Tess.DomTessEdgeQty[i], "%d");
    }
  }

  if (Tess.Dim > 2)
  {
    fprintf (file, "  *face\n");
    fprintf (file, "   %d\n", Tess.DomFaceQty);

    for (i = 1; i <= Tess.DomFaceQty; i++)
    {
      fprintf (file, " %3d ", i);

      fprintf (file, "%d%s", Tess.DomFaceVerQty[i],
               Tess.DomFaceVerQty[i] ? " " : "");
      ut_array_1d_int_fprintf (file, Tess.DomFaceVerNb[i] + 1,
                               Tess.DomFaceVerQty[i], "%d");
      fprintf (file, "     ");
      fprintf (file, "%d%s", Tess.DomFaceEdgeQty[i],
               Tess.DomFaceEdgeQty[i] ? " " : "");
      ut_array_1d_int_fprintf (file, Tess.DomFaceEdgeNb[i] + 1,
                               Tess.DomFaceEdgeQty[i], "%d");

      fprintf (file, "     %s\n", Tess.DomFaceType[i]);

      fprintf (file, "     %d ", Tess.DomFaceParmQty[i]);

      if (!strcmp (Tess.DomFaceType[i], "plane"))
        ut_array_1d_fprintf (file, Tess.DomFaceEq[i], 4, REAL_PRINT_FORMAT);
      else
        ut_array_1d_fprintf (file, Tess.DomFaceParms[i],
                             Tess.DomFaceParmQty[i], REAL_PRINT_FORMAT);

      fprintf (file, "     %s\n", Tess.DomFaceLabel[i]);

      fprintf (file, "   %3d ", Tess.DomTessFaceQty[i]);
      ut_array_1d_int_fprintf (file, Tess.DomTessFaceNb[i] + 1,
                               Tess.DomTessFaceQty[i], "%d");
    }
  }

  return;
}

void
neut_tess_fprintf_per (struct TESS Tess, FILE * file)
{
  neut_tess_fprintf_per_gen (Tess, file);
  neut_tess_fprintf_per_seed (Tess, file);
  neut_tess_fprintf_per_ver (Tess, file);
  neut_tess_fprintf_per_edge (Tess, file);
  neut_tess_fprintf_per_face (Tess, file);

  return;
}

void
neut_tess_fprintf_scale (struct TESS Tess, FILE * file)
{
  neut_tess_fprintf_scale_gen (Tess, file);
  neut_tess_fprintf_scale_cellid (Tess, file);

  return;
}
