/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_flatten_.h"

void
net_flatten (struct MTESS MTess, struct TESS *Tess, struct TESS *pFTess)
{
  int i, j, id, tess;
  struct TESS *Tess2 = (TESS *) calloc (MTess.TessQty + 1, sizeof (TESS));
  int *tmp = ut_alloc_1d_int (MTess.LevelQty + 1);
  int *dom = ut_alloc_1d_int (2);

  for (i = 0; i <= MTess.TessQty; i++)
    neut_tess_tess (Tess[i], Tess2 + i);

  ut_print_message (0, 1, "  [level] Flattening %d-scale tessellation...\n",
                    MTess.LevelQty);
  for (i = MTess.LevelQty - 1; i >= 1; i--)
    for (j = 1; j <= MTess.LevelTessQty[i]; j++)
    {
      id = MTess.LevelTess[i][j];

      ut_print_clearline (stdout, 72);
      ut_print_message (0, 1, "  [%5d]  %d/%d: id=%d%s", i, j,
                        MTess.LevelTessQty[i], id,
                        (j < MTess.LevelTessQty[i]) ? " " : "\n");
      net_mtess_flatten (MTess, Tess2, id, Tess2 + id);
    }

  neut_tess_tess (Tess2[1], pFTess);

  (*pFTess).ScaleQty = MTess.LevelQty;
  (*pFTess).ScaleCellId =
    ut_alloc_2d_int ((*pFTess).CellQty + 1, MTess.LevelQty + 1);

  // Recording ScaleCellId

  id = 0;
  for (i = 1; i <= MTess.LevelTessQty[MTess.LevelQty]; i++)
  {
    tess = MTess.LevelTess[MTess.LevelQty][i];

    for (j = MTess.LevelQty - 1; j >= 0; j--)
    {
      neut_mtess_tess_level_dom (MTess, Tess[tess], j, dom);
      tmp[j] = dom[1];
    }

    for (j = 1; j <= Tess[tess].PolyQty; j++)
    {
      tmp[MTess.LevelQty] = j;
      ut_array_1d_int_memcpy (tmp, MTess.LevelQty + 1,
                              (*pFTess).ScaleCellId[++id]);
    }
  }

  for (i = 0; i <= MTess.TessQty; i++)
    neut_tess_free (Tess2 + i);
  free (Tess2);
  ut_free_1d_int (&dom);
  ut_free_1d_int (&tmp);

  return;
}
