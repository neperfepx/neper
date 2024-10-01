/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_.h"

void
net_reg_init (struct IN_T In, struct TESS *pTess, struct REG *pReg)
{
  int i;
  double size;
  char *entity = NULL;
  double **pcellsel = NULL;
  double dom_min, l;

  (*pReg).mloop = In.mloop;
  (*pReg).fmax = In.fmax;
  (*pReg).maxedgedelqty = In.maxedgedelqty;

  if ((*pTess).Dim == 3)
    pcellsel = &((*pReg).PolySel);
  else if ((*pTess).Dim == 2)
    pcellsel = &((*pReg).FaceSel);
  else
    abort ();

  neut_tess_size ((*pTess), &size);

  neut_tess_cell ((*pTess), &entity);

  (*pcellsel) = ut_alloc_1d ((*pTess).CellQty + 1);
  neut_tess_entity_expr_val (pTess, entity, In.selstring, *pcellsel, NULL);

  if (!strcmp (In.seltype, "rel"))
    for (i = 1; i <= (*pTess).CellQty; i++)
      rsel2sel ((*pcellsel)[i], In.dim, size, (*pTess).CellQty,
                &((*pcellsel)[i]));

  if (!strcmp ((*pTess).DomType, "cylinder"))
  {
    dom_min = DBL_MAX;
    for (i = 1; i <= (*pTess).DomEdgeQty; i++)
    {
      neut_tess_domedge_length ((*pTess), i, &l);
      dom_min = ut_num_min (dom_min, l);
    }

    for (i = 1; i <= (*pTess).CellQty; i++)
      (*pcellsel)[i] = ut_num_min ((*pcellsel)[i], .5 * dom_min);
  }

  if ((*pTess).Dim == 3)
  {
    (*pReg).FaceSel = ut_alloc_1d ((*pTess).FaceQty + 1);
    neut_tess_val_poly2face ((*pTess), (*pReg).PolySel, (*pReg).FaceSel);
  }

  if ((*pTess).Dim >= 2)
  {
    (*pReg).EdgeSel = ut_alloc_1d ((*pTess).EdgeQty + 1);
    neut_tess_val_face2edge ((*pTess), (*pReg).FaceSel, (*pReg).EdgeSel);
  }

  ut_free_1d_char (&entity);

  return;
}
