/* This file is part of the Neper software package. */
/* Ctopoyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the ttopo-level directory. */

#include "neut_flatten_topo_.h"

void
neut_flatten_domface_edges (struct FLATTEN Flatten, struct TESS DTess,
                            int dface, int **pedges, int *pedgeqty)
{
  ut_free_1d_int (pedges);
  (*pedgeqty) = 0;

  (void) DTess;

  (*pedgeqty) = Flatten.DomFaceTessEdgeQty[dface];
  (*pedges) = ut_alloc_1d_int (*pedgeqty);
  ut_array_1d_int_memcpy (Flatten.DomFaceTessEdgeNb[dface],
                          Flatten.DomFaceTessEdgeQty[dface], *pedges);

  return;
}

void
neut_flatten_domface_ver_edges (struct FLATTEN Flatten, struct TESS DTess,
                                int dface, struct TESS FTess, int ver,
                                int **pedges, int *pedgeqty)
{
  int i, edge, status, pos, dedge;

  // could have used DomFaceTessEdgeNb, but the above algorithm is
  // (probably much) faster - almost no search

  ut_free_1d_int (pedges);
  (*pedgeqty) = 0;

  for (i = 0; i < FTess.VerEdgeQty[ver]; i++)
  {
    edge = FTess.VerEdgeNb[ver][i];

    status = 0;
    if (Flatten.EdgeDom[edge][0] == 2 && Flatten.EdgeDom[edge][1] == dface)
    {
      status = 1;
    }
    else if (Flatten.EdgeDom[edge][0] == 1)
    {
      dedge = Flatten.EdgeDom[edge][1];
      // belongs to a domain edge; checking that the domain edge belongs
      // to the domain face.
      pos =
        ut_array_1d_int_eltpos (DTess.EdgeFaceNb[dedge],
                                DTess.EdgeFaceQty[dedge], dface);
      status = (pos != -1) ? 1 : 0;
    }

    if (status)
    {
      (*pedges) = ut_realloc_1d_int (*pedges, ++(*pedgeqty));
      (*pedges)[(*pedgeqty) - 1] = edge;
    }
  }

  return;
}
