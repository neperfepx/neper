/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

void
DeleteFaceFromItsPoly (struct TESS *pTess, int face)
{
  int i;
  int k;
  int tmppoly;
  int qty;
  int pos;

  /*ut_print_lineheader(-1); printf("    Deleting face from its parent polyhedra: "); */

  /* it is removed from its one/two parent polyhedron/a. */
  for (i = 0; i <= 1; i++)
  {
    tmppoly = (*pTess).FacePoly[face][i];
    if (tmppoly >= 1)		/* ie real tmppoly */
    {
      /* printf("%d ",tmppoly); */

      pos = oneDIntEltPos ((*pTess).PolyFaceNb[tmppoly], 1,
			   (*pTess).PolyFaceQty[tmppoly], face);

      qty =
	ut_array_1d_int_deletencompress ((*pTess).PolyFaceNb[tmppoly] + 1,
					 (*pTess).PolyFaceQty[tmppoly], face,
					 1);
      for (k = pos; k <= (*pTess).PolyFaceQty[tmppoly] - 1; k++)
	(*pTess).PolyFaceOri[tmppoly][k] =
	  (*pTess).PolyFaceOri[tmppoly][k + 1];

      (*pTess).PolyFaceQty[tmppoly]--;

      if (qty != 1)
      {
	ut_print_lineheader (2);
	printf ("face %d not found in poly %d!\n", face, tmppoly);
	abort ();
      }
    }
  }

  return;
}

void
SearchNewNOldEdges (struct TESS *pTess, int face, int edge, int *pnew,
		    int *pold)
{
  ut_array_1d_int_deletencompress ((*pTess).FaceEdgeNb[face] + 1, 3, edge, 1);

  (*pnew) = ut_array_1d_int_min ((*pTess).FaceEdgeNb[face] + 1, 2);
  (*pold) = ut_array_1d_int_max ((*pTess).FaceEdgeNb[face] + 1, 2);

  return;
}
