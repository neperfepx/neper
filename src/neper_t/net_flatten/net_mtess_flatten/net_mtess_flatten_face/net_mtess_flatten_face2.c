/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_face_.h"

void
net_mtess_flatten_face_body (struct MTESS MTess, struct TESS *Tess,
                             int *CTessIds, int CTessQty, struct TESS *pFTess,
                             struct TESSE **pTessE, struct FLATTEN *pFlatten)
{
  int i, j, k, l, id, face, edge, ori;
  int *dom = ut_alloc_1d_int (2);

  for (i = 0; i < CTessQty; i++)
  {
    id = CTessIds[i];

    ut_array_1d_int_set_2 (dom, 3, MTess.TessDom[id][1]);
    // Body faces
    for (j = 1; j <= Tess[id].FaceQty; j++)
      if (Tess[id].FaceDom[j][0] == -1)
      {
        face = neut_tess_addface_alloc (pFTess);
        neut_flatten_addface (pFlatten, dom);

        neut_tess_face_addeq (pFTess, face, Tess[id].FaceEq[j]);
        for (k = 1; k <= Tess[id].FaceVerQty[j]; k++)
        {
          edge = Tess[id].FaceEdgeNb[j][k];
          ori = Tess[id].FaceEdgeOri[j][k];
          if (ori == 1)
            for (l = 0; l < (*pTessE)[id].EdgeFEdgeQty[edge]; l++)
              neut_tess_face_addedge (pFTess, face,
                                      (*pTessE)[id].EdgeFEdgeNb[edge][l],
                                      Tess[id].FaceEdgeOri[j][k]);
          else
            for (l = (*pTessE)[id].EdgeFEdgeQty[edge] - 1; l >= 0; l--)
              neut_tess_face_addedge (pFTess, face,
                                      (*pTessE)[id].EdgeFEdgeNb[edge][l],
                                      Tess[id].FaceEdgeOri[j][k]);
        }

        neut_tesse_face_addface (&((*pTessE)[id]), j, face, 1);
      }
  }

  neut_tess_init_edgeface (pFTess);

  ut_free_1d_int (&dom);

  return;
}

void
net_mtess_flatten_face_domface (struct MTESS MTess, struct TESS *Tess,
                                int TessId, struct TESS *pFTess,
                                struct TESSE **pTessE,
                                struct FLATTEN *pFlatten)
{
  int i;
  int *dom = ut_alloc_1d_int (2);

  for (i = 1; i <= Tess[TessId].FaceQty; i++)
  {
    ut_array_1d_int_set_2 (dom, 2, i);

    // Face is on the domain boundary (1 tess only)
    if (Tess[TessId].FaceDom[i][0] == 2)
      net_mtess_flatten_face_domface_dom (MTess, Tess, TessId, i, pFTess,
                                          pTessE, dom, pFlatten);

    // Face is not on the domain boundary (2 tesses meet)
    else
      net_mtess_flatten_face_domface_body (MTess, Tess, TessId, i, pFTess,
                                           pTessE, dom, pFlatten);
  }

  ut_free_1d_int (&dom);

  return;
}
