/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_merge_.h"

void
neut_tess_merge_polyfaces (struct TESS *pTess, int poly, double coplanar)
{
  int i, k, npolyqty, *npolys = NULL;
  int comfaceqty, *comfaces = NULL;
  int setqty = 0, *setfaceqty = NULL, **setfaces = NULL;

  neut_tess_poly_neighseeds (*pTess, poly, &npolys, &npolyqty);

  for (i = 0; i < npolyqty; i++)
  {
    neut_tess_seedpair_commonfaces (*pTess, poly, npolys[i], &comfaces,
                                    &comfaceqty);

    neut_tess_faces_contiguousfaces (*pTess, coplanar, comfaces, comfaceqty,
                                     &setqty, &setfaces, &setfaceqty);

    for (k = 0; k < setqty; k++)
      neut_tess_faces_merge (pTess, setfaces[k], setfaceqty[k]);

    ut_free_1d_int (&setfaceqty);
    ut_free_2d_int (&setfaces, setqty);
    setqty = 0;
  }

  ut_free_1d_int (&npolys);
  ut_free_1d_int (&comfaces);

  return;
}

void
neut_tess_merge_polyedges (struct TESS *pTess, int poly, double colinear)
{
  int i, verqty, *vers = NULL, ver;
  double angle;
  int qty, **edges = NULL, *edgeqty = NULL;

  neut_tess_poly_vers (*pTess, poly, &vers, &verqty);

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    if ((*pTess).VerEdgeQty[ver] == 2)
    {
      neut_tess_edgepair_angle (*pTess, (*pTess).VerEdgeNb[ver][0],
                                (*pTess).VerEdgeNb[ver][1], &angle);

      neut_tess_edges_contiguousedges (*pTess, colinear,
                                       (*pTess).VerEdgeNb[ver], 2, &qty,
                                       &edges, &edgeqty);
      ut_free_2d_int (&edges, qty);
      ut_free_1d_int (&edgeqty);

      if (qty == 1)
        neut_tess_edges_merge (pTess, (*pTess).VerEdgeNb[ver], 2);
    }
  }

  ut_free_1d_int (&vers);

  return;
}
