/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_topo_.h"

int
neut_mtess_tess_level_dom (struct MTESS MTess, struct TESS Tess, int level,
                           int *dom)
{
  int i;

  if (Tess.TessId <= 0)
    // ut_print_neperbug (); // this may happen when the domain is void (empty
    // cell of the parent tessellation), which can happen when -morphooptiini
    // is used.  It seems fine to let this pass through.
    return -1;

  if (level >= Tess.Level)
    return -1;

  dom[0] = Tess.TessId;
  for (i = Tess.Level - 1; i >= level; i--)
    ut_array_1d_int_memcpy (MTess.TessDom[dom[0]], 2, dom);

  return 0;
}

int
neut_mtess_tess_doms (struct MTESS MTess, struct TESS Tess, int **doms)
{
  int i;

  if (Tess.Level == 0)
  {
    ut_array_1d_int_set_2 (doms[0], 0, 1);
    return 0;
  }

  if (Tess.TessId <= 0)
    ut_print_neperbug ();

  ut_array_1d_int_memcpy (MTess.TessDom[Tess.TessId], 2, doms[Tess.Level]);
  for (i = Tess.Level - 1; i >= 0; i--)
    ut_array_1d_int_memcpy (MTess.TessDom[doms[i + 1][0]], 2, doms[i]);

  return 0;
}

int
neut_mtess_tess_domtesses (struct MTESS MTess, struct TESS Tess,
                           int *domtesses)
{
  int i;
  int **doms = ut_alloc_2d_int (Tess.Level + 1, 2);

  neut_mtess_tess_doms (MTess, Tess, doms);
  for (i = 0; i < Tess.Level; i++)
    domtesses[i] = doms[i][0];

  ut_free_2d_int (&doms, Tess.Level + 1);

  return 0;
}

int
neut_mtess_tess_dompolys (struct MTESS MTess, struct TESS Tess, int *dompolys)
{
  int i;
  int **doms = ut_alloc_2d_int (Tess.Level + 1, 2);

  neut_mtess_tess_doms (MTess, Tess, doms);
  for (i = 0; i < Tess.Level; i++)
    dompolys[i] = doms[i][1];

  ut_free_2d_int (&doms, Tess.Level + 1);

  return 0;
}

int
neut_mtess_tess_poly_childtesses (struct MTESS MTess, struct TESS *Tess,
                                  int tessid, int **ptess, int *ptessqty)
{
  int i;

  (*ptessqty) = Tess[tessid].PolyQty;
  (*ptess) = ut_alloc_1d_int (*ptessqty);

  for (i = 0; i < Tess[tessid].PolyQty; i++)
    (*ptess)[i] = MTess.DomTess[tessid][i + 1];

  return 0;
}
