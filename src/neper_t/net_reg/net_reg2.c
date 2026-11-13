/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_.h"

void
net_reg_init (struct IN_T In, struct TESS *pTess, struct REG *pReg)
{
  int i;
  double size;
  char *entity = NULL;
  double **pcellsel = NULL;

  (*pReg).mloop = In.mloop;
  (*pReg).fmax = In.fmax;
  (*pReg).maxedgedelqty = In.maxedgedelqty;
  (*pReg).delcell = In.delcell;

  if ((*pReg).delcell)
  {
    if (!strcmp ((*pTess).Type, "periodic"))
      ut_print_message (2, 2,
                        "`-regdelcell 1' is not available for a periodic tessellation.\n");
    if (In.levelqty > 1)
      ut_print_message (2, 2,
                        "`-regdelcell 1' is not available for a multiscale tessellation.\n");
  }

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

  /*
   * this was for the faceted domain
  double l, dom_min;
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
  */

  if ((*pTess).Dim == 3)
  {
    (*pReg).FaceSel = ut_alloc_1d ((*pTess).FaceQty + 1);
    neut_tess_val_poly2face ((*pTess), (*pReg).PolySel, (*pReg).FaceSel);
  }

  if ((*pTess).Dim >= 2)
  {
    (*pReg).EdgeSel = ut_alloc_1d ((*pTess).EdgeQty + 1);
    neut_tess_val_face2edge ((*pTess), (*pReg).FaceSel, (*pReg).EdgeSel);

    // the copies of an edge (periodic tessellation) get the same value
    if (!strcmp ((*pTess).Type, "periodic"))
      net_reg_init_peredgesel (*pTess, (*pReg).EdgeSel);
  }

  ut_free_1d_char (&entity);

  return;
}

// Updating the periodicity of the tessellation after the deletion of
// edges: the vertex periodicity is up to date, apart from the list of
// slave vertices, the edge periodicity is recomputed from it (as edges
// can be merged), and the deleted faces are removed from the face
// periodicity (a face and its periodic copy are deleted together).
void
net_reg_periodic (struct TESS *pTess)
{
  net_reg_periodic_ver (pTess);
  net_reg_periodic_edge (pTess);

  if ((*pTess).Dim == 3)
    net_reg_periodic_face (pTess);

  return;
}

// Updating the face equations after the deletion of edges (during
// which they are the face equations before regularization, used to
// position the new vertices), see neut_tess_face_eq
void
net_reg_faceeq (struct TESS *pTess)
{
  int i;

  for (i = 1; i <= (*pTess).FaceQty; i++)
    if ((*pTess).FaceState[i] != -1)
      neut_tess_face_eq (*pTess, i, (*pTess).FaceEq[i]);

  return;
}
