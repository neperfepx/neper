/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_mtess_flatten_face_.h"

int
net_mtess_flatten_face_domface_body_nextface (int *edges, int edgeqty,
					int *edgesinv,
					int ***edgetessfacenb,
					int **edgetessfaceqty,
					int ***pfacepairlist,
					int *pfacepairqty, int *pedgepos,
					int *facepair)
{
  int i, j, k, edge, edgeinv;

  for (i = (*pedgepos); i < edgeqty; i++)
  {
    edge = edges[i];
    edgeinv = edgesinv[edge];

    for (j = 0; j < edgetessfaceqty[edgeinv][0]; j++)
    {
      facepair[0] = edgetessfacenb[edgeinv][0][j];
      for (k = 0; k < edgetessfaceqty[edgeinv][1]; k++)
      {
	facepair[1] = edgetessfacenb[edgeinv][1][k];

	if (ut_array_2d_int_linepos (*pfacepairlist, *pfacepairqty, 2,
				     facepair) == -1)
	{
	  (*pfacepairlist) = ut_realloc_2d_int_addline (*pfacepairlist,
							++(*pfacepairqty), 2);
	  ut_array_1d_int_memcpy ((*pfacepairlist)[(*pfacepairqty) - 1], 2,
				  facepair);
	  (*pedgepos) = i;
	  return 1;
	}
      }
    }
  }

  return -1;
}

int
net_mtess_flatten_face_domface_edge_tess_faces (struct FLATTEN Flatten,
					  int fedge, double *fedgecoo,
					  struct TESS Tess,
					  int domface, int **pfaces,
					  int *pfaceqty)
{
  int pos, i, edge, face;
  int tess = Tess.TessId;

  ut_free_1d_int (*pfaces);
  (*pfaces) = NULL;
  (*pfaceqty) = 0;

  pos = -1;
  for (i = 0; i < Flatten.EdgeTessEdgeQty[fedge]; i++)
    if (Flatten.EdgeTessEdgeNb[fedge][i][0] == tess)
    {
      pos = i;
      break;
    }

  if (pos != -1)
  {
    edge = Flatten.EdgeTessEdgeNb[fedge][pos][1];
    neut_tess_edge_domface_faces (Tess, edge, domface, pfaces, pfaceqty);
  }

  // no topology relation; using geometry to find the face the edge is in
  else
  {
    for (i = 1; i <= Tess.DomTessFaceQty[domface]; i++)
    {
      face = Tess.DomTessFaceNb[domface][i];
      if (neut_tess_point_inface (Tess, fedgecoo, face))
      {
	(*pfaceqty) = 1;
	(*pfaces) = ut_alloc_1d_int (1);
	(*pfaces)[0] = face;
	break;
      }
    }
    if (*pfaceqty != 1)
      ut_error_reportbug ();
  }

  return 0;
}

int
net_mtess_flatten_face_domface_ver_tess_edges (struct FLATTEN Flatten,
					 int fver, struct TESS Tess,
					 int domface, int **pedges,
					 int *pedgeqty)
{
  int pos, i, ver;
  int tess = Tess.TessId;
  if (tess == 0)
  {
    printf ("tess id is zero\n");
    ut_error_reportbug ();
  }

  ut_free_1d_int (*pedges);
  (*pedges) = NULL;
  (*pedgeqty) = 0;

  pos = -1;
  for (i = 0; i < Flatten.VerTessVerQty[fver]; i++)
    if (Flatten.VerTessVerNb[fver][i][0] == tess)
    {
      pos = i;
      break;
    }

  if (pos == -1)
  {
    (*pedgeqty) = 0;
    return -1;
  }

  else
  {
    ver = Flatten.VerTessVerNb[fver][pos][1];
    neut_tess_ver_domface_edges (Tess, ver, domface, pedges, pedgeqty);
  }

  return 0;
}

void
net_mtess_flatten_face_domface_body_tessfaces (struct MTESS MTess,
					 struct TESS *Tess, int TessId,
					 int dface,
					 int **tessface)
{
  int i;

  // recording the numbers of the tesses that meet at the face, and
  // the domface numbers in the tesses.
  for (i = 0; i < 2; i++)
  {
    tessface[i][0] = MTess.DomTess[TessId][Tess[TessId].FacePoly[dface][i]];
    tessface[i][1] = MTess.DomTessFaceNb[TessId][dface][i];
  }

  return;
}

void
net_mtess_flatten_face_domface_body_nextedge (struct FLATTEN Flatten, struct
					TESS DTess, int dface,
					struct TESS FTess, int *tessfaces,
					int fedge, int fver,
					int *edgesinv,
					int ***edgetessfacenb,
					int **edgetessfaceqty, int
					*pnedge, int *pnedgeori)
{
  int status, qty, j, k, pos, edge, edgeinv;
  int *tmp = NULL;

  // the next edge must belong to the domain face
  neut_flatten_domface_ver_edges (Flatten, DTess, dface, FTess, fver, &tmp,
				  &qty);

  status = ut_array_1d_int_deletencompress (tmp, qty, fedge, 1);
  if (status != 1)
    abort ();
  qty--;

  (*pnedge) = -1;
  (*pnedgeori) = 0;
  // looking for the edge that has the correct tessfaces
  for (j = 0; j < qty; j++)
  {
    status = 0;
    edge = tmp[j];
    edgeinv = edgesinv[edge];
    for (k = 0; k < 2; k++)
      if (ut_array_1d_int_eltpos (edgetessfacenb[edgeinv][k],
				  edgetessfaceqty[edgeinv][k],
				  tessfaces[k]) != -1)
	status++;

    if (status == 2)
    {
      (*pnedge) = edge;
      pos = ut_array_1d_int_eltpos (FTess.EdgeVerNb[*pnedge], 2, fver);
      if (pos == -1)
	abort ();
      (*pnedgeori) = (pos == 0) ? 1 : -1;
      break;
    }
  }

  ut_free_1d_int (tmp);

  return;
}
