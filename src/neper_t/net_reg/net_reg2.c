/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_.h"

void
net_reg_init (struct IN_T In, struct TESS Tess, struct REG *pReg)
{
  int i;
  double size;
  char *entity = NULL;
  double **pcellsel = NULL;
  double dom_min, l;

  (*pReg).mloop = In.mloop;
  (*pReg).fmax = In.fmax;
  (*pReg).maxedgedelqty = In.maxedgedelqty;

  if (Tess.Dim == 3)
    pcellsel = &((*pReg).PolySel);
  else if (Tess.Dim == 2)
    pcellsel = &((*pReg).FaceSel);
  else
    abort ();

  neut_tess_size (Tess, &size);

  neut_tess_cell (Tess, &entity);

  (*pcellsel) = ut_alloc_1d (Tess.CellQty + 1);
  neut_tess_entity_expr_val (Tess, entity, In.selstring, *pcellsel);

  if (!strcmp (In.seltype, "rel"))
    for (i = 1; i <= Tess.CellQty; i++)
      rsel2sel ((*pcellsel)[i], In.dim, size, Tess.CellQty,
		&((*pcellsel)[i]));

  if (!strcmp (Tess.DomType, "cylinder"))
  {
    dom_min = DBL_MAX;
    for (i = 1; i <= Tess.DomEdgeQty; i++)
    {
      neut_tess_domedge_length (Tess, i, &l);
      dom_min = ut_num_min (dom_min, l);
    }

    for (i = 1; i <= Tess.CellQty; i++)
      (*pcellsel)[i] = ut_num_min ((*pcellsel)[i], .5 * dom_min);
  }

  if (Tess.Dim == 3)
  {
    (*pReg).FaceSel = ut_alloc_1d (Tess.FaceQty + 1);
    neut_tess_val_poly2face (Tess, (*pReg).PolySel, (*pReg).FaceSel);
  }

  if (Tess.Dim >= 2)
  {
    (*pReg).EdgeSel = ut_alloc_1d (Tess.EdgeQty + 1);
    neut_tess_val_face2edge (Tess, (*pReg).FaceSel, (*pReg).EdgeSel);
  }

  ut_free_1d_char (entity);

  return;
}
