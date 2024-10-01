/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_flatten_op_.h"

void
neut_flatten_set_zero (struct FLATTEN *pFlatten)
{
  (*pFlatten).VerQty = 0;
  (*pFlatten).VerDom = NULL;
  (*pFlatten).EdgeQty = 0;
  (*pFlatten).EdgeDom = NULL;
  (*pFlatten).FaceQty = 0;
  (*pFlatten).FaceDom = NULL;
  (*pFlatten).PolyQty = 0;
  (*pFlatten).PolyDom = NULL;

  (*pFlatten).DomVerTessVerNb = NULL;
  (*pFlatten).DomEdgeBTessVerQty = NULL;
  (*pFlatten).DomEdgeBTessVerNb = NULL;
  (*pFlatten).DomEdgeTessVerQty = NULL;
  (*pFlatten).DomEdgeTessVerNb = NULL;
  (*pFlatten).DomFaceTessVerQty = NULL;
  (*pFlatten).DomFaceTessVerNb = NULL;
  (*pFlatten).DomFaceBTessVerQty = NULL;
  (*pFlatten).DomFaceBTessVerNb = NULL;
  (*pFlatten).DomPolyBTessVerQty = NULL;
  (*pFlatten).DomPolyBTessVerNb = NULL;

  (*pFlatten).DomEdgeTessEdgeQty = NULL;
  (*pFlatten).DomEdgeTessEdgeNb = NULL;
  (*pFlatten).DomFaceBTessEdgeQty = NULL;
  (*pFlatten).DomFaceBTessEdgeNb = NULL;
  (*pFlatten).DomFaceTessEdgeQty = NULL;
  (*pFlatten).DomFaceTessEdgeNb = NULL;
  (*pFlatten).DomPolyBTessEdgeQty = NULL;
  (*pFlatten).DomPolyBTessEdgeNb = NULL;

  (*pFlatten).DomFaceTessFaceQty = NULL;
  (*pFlatten).DomFaceTessFaceNb = NULL;
  (*pFlatten).DomPolyBTessFaceQty = NULL;
  (*pFlatten).DomPolyBTessFaceNb = NULL;

  (*pFlatten).VerTessVerQty = NULL;
  (*pFlatten).VerTessVerNb = NULL;
  (*pFlatten).EdgeTessEdgeQty = NULL;
  (*pFlatten).EdgeTessEdgeNb = NULL;

  return;
}

void
neut_flatten_addver (struct FLATTEN *pFlatten, int *dom)
{
  (*pFlatten).VerQty++;

  (*pFlatten).VerDom =
    ut_realloc_2d_int_addline ((*pFlatten).VerDom, (*pFlatten).VerQty + 1, 2);
  ut_array_1d_int_memcpy (dom, 2, (*pFlatten).VerDom[(*pFlatten).VerQty]);

  (*pFlatten).VerTessVerQty =
    ut_realloc_1d_int ((*pFlatten).VerTessVerQty, (*pFlatten).VerQty + 1);
  (*pFlatten).VerTessVerNb =
    ut_realloc_1d_ppint ((*pFlatten).VerTessVerNb, (*pFlatten).VerQty + 1);
  (*pFlatten).VerTessVerNb[(*pFlatten).VerQty] = NULL;

  (*pFlatten).VerDom[0] = NULL;

  return;
}

void
neut_flatten_addedge (struct FLATTEN *pFlatten, int *dom)
{
  (*pFlatten).EdgeQty++;

  (*pFlatten).EdgeDom =
    ut_realloc_2d_int_addline ((*pFlatten).EdgeDom, (*pFlatten).EdgeQty + 1,
                               2);
  ut_array_1d_int_memcpy (dom, 2, (*pFlatten).EdgeDom[(*pFlatten).EdgeQty]);

  (*pFlatten).EdgeDom[0] = NULL;

  return;
}

void
neut_flatten_addface (struct FLATTEN *pFlatten, int *dom)
{
  (*pFlatten).FaceQty++;

  (*pFlatten).FaceDom =
    ut_realloc_2d_int_addline ((*pFlatten).FaceDom, (*pFlatten).FaceQty + 1,
                               2);
  ut_array_1d_int_memcpy (dom, 2, (*pFlatten).FaceDom[(*pFlatten).FaceQty]);

  (*pFlatten).FaceDom[0] = NULL;

  return;
}

void
neut_flatten_addpoly (struct FLATTEN *pFlatten, int *dom)
{
  (*pFlatten).PolyQty++;

  (*pFlatten).PolyDom =
    ut_realloc_2d_int_addline ((*pFlatten).PolyDom, (*pFlatten).PolyQty + 1,
                               2);
  ut_array_1d_int_memcpy (dom, 2, (*pFlatten).PolyDom[(*pFlatten).PolyQty]);

  (*pFlatten).PolyDom[0] = NULL;

  return;
}

void
neut_flatten_init_domstuff_fromver (struct FLATTEN *pFlatten,
                                    struct TESS DTess)
{
  int i, j, k, dver, fver, dedge, dface, dpoly;

  (*pFlatten).DomVerTessVerNb = ut_alloc_1d_int ((*pFlatten).DVerQty + 1);
  (*pFlatten).DomEdgeBTessVerQty = ut_alloc_1d_int ((*pFlatten).DEdgeQty + 1);
  (*pFlatten).DomEdgeBTessVerNb = ut_alloc_1d_pint ((*pFlatten).DEdgeQty + 1);
  (*pFlatten).DomFaceBTessVerQty = ut_alloc_1d_int ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomFaceBTessVerNb = ut_alloc_1d_pint ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomPolyBTessVerQty = ut_alloc_1d_int ((*pFlatten).DPolyQty + 1);
  (*pFlatten).DomPolyBTessVerNb = ut_alloc_1d_pint ((*pFlatten).DPolyQty + 1);

  for (i = 1; i <= (*pFlatten).VerQty; i++)
    if ((*pFlatten).VerDom[i][0] == 0)
    {
      dver = (*pFlatten).VerDom[i][1];
      (*pFlatten).DomVerTessVerNb[dver] = i;
    }
    else if ((*pFlatten).VerDom[i][0] == 1)
    {
      dedge = (*pFlatten).VerDom[i][1];
      (*pFlatten).DomEdgeBTessVerQty[dedge]++;
      (*pFlatten).DomEdgeBTessVerNb[dedge] =
        ut_realloc_1d_int ((*pFlatten).DomEdgeBTessVerNb[dedge],
                           (*pFlatten).DomEdgeBTessVerQty[dedge]);
      (*pFlatten).DomEdgeBTessVerNb[dedge][(*pFlatten).
                                           DomEdgeBTessVerQty[dedge] - 1] = i;
    }
    else if ((*pFlatten).VerDom[i][0] == 2)
    {
      dface = (*pFlatten).VerDom[i][1];
      (*pFlatten).DomFaceBTessVerQty[dface]++;
      (*pFlatten).DomFaceBTessVerNb[dface] =
        ut_realloc_1d_int ((*pFlatten).DomFaceBTessVerNb[dface],
                           (*pFlatten).DomFaceBTessVerQty[dface]);
      (*pFlatten).DomFaceBTessVerNb[dface][(*pFlatten).
                                           DomFaceBTessVerQty[dface] - 1] = i;
    }
    else if ((*pFlatten).VerDom[i][0] == 3)
    {
      dpoly = (*pFlatten).VerDom[i][1];
      (*pFlatten).DomPolyBTessVerQty[dpoly]++;
      (*pFlatten).DomPolyBTessVerNb[dpoly] =
        ut_realloc_1d_int ((*pFlatten).DomPolyBTessVerNb[dpoly],
                           (*pFlatten).DomPolyBTessVerQty[dpoly]);
      (*pFlatten).DomPolyBTessVerNb[dpoly][(*pFlatten).
                                           DomPolyBTessVerQty[dpoly] - 1] = i;
    }
    else
      abort ();

  // Computing DomEdgeTessVerQty / DomEdgeTessVerNb from
  // DomEdgeBTessVerQty / DomEdgeBTessVerNb and
  // DomVerTessVerQty / DomVerTessVerNb.

  (*pFlatten).DomEdgeTessVerQty = ut_alloc_1d_int ((*pFlatten).DEdgeQty + 1);
  (*pFlatten).DomEdgeTessVerNb = ut_alloc_1d_pint ((*pFlatten).DEdgeQty + 1);

  for (i = 1; i <= (*pFlatten).DEdgeQty; i++)
  {
    (*pFlatten).DomEdgeTessVerQty[i] = (*pFlatten).DomEdgeBTessVerQty[i];
    (*pFlatten).DomEdgeTessVerNb[i] =
      ut_alloc_1d_int ((*pFlatten).DomEdgeTessVerQty[i]);

    ut_array_1d_int_memcpy ((*pFlatten).DomEdgeBTessVerNb[i],
                            (*pFlatten).DomEdgeBTessVerQty[i],
                            (*pFlatten).DomEdgeTessVerNb[i]);

    for (j = 0; j < 2; j++)
    {
      dver = DTess.EdgeVerNb[i][j];
      fver = (*pFlatten).DomVerTessVerNb[dver];
      if ((*pFlatten).VerDom[fver][0] != 0)
        abort ();

      (*pFlatten).DomEdgeTessVerNb[i] =
        ut_realloc_1d_int ((*pFlatten).DomEdgeTessVerNb[i],
                           ++(*pFlatten).DomEdgeTessVerQty[i]);
      (*pFlatten).DomEdgeTessVerNb[i][(*pFlatten).DomEdgeTessVerQty[i] - 1] =
        fver;
    }
  }

  // Computing DomFaceTessVerQty / DomFaceTessVerNb from
  // DomFaceBTessVerQty / DomFaceBTessVerNb,
  // DomEdgeBTessVerQty / DomEdgeBTessVerNb and
  // DomVerTessVerQty / DomVerTessVerNb.
  (*pFlatten).DomFaceTessVerQty = ut_alloc_1d_int ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomFaceTessVerNb = ut_alloc_1d_pint ((*pFlatten).DFaceQty + 1);

  for (i = 1; i <= (*pFlatten).DFaceQty; i++)
  {
    (*pFlatten).DomFaceTessVerQty[i] = (*pFlatten).DomFaceBTessVerQty[i];
    (*pFlatten).DomFaceTessVerNb[i] =
      ut_alloc_1d_int ((*pFlatten).DomFaceTessVerQty[i]);

    ut_array_1d_int_memcpy ((*pFlatten).DomFaceBTessVerNb[i],
                            (*pFlatten).DomFaceBTessVerQty[i],
                            (*pFlatten).DomFaceTessVerNb[i]);

    // Adding vers of the edge bodies
    for (j = 1; j <= DTess.FaceVerQty[i]; j++)
    {
      dedge = DTess.FaceEdgeNb[i][j];
      for (k = 0; k < (*pFlatten).DomEdgeBTessVerQty[dedge]; k++)
      {
        fver = (*pFlatten).DomEdgeBTessVerNb[dedge][k];
        if (fver < 1)
        {
          printf ("dedge = %d fver = %d < 1\n", dedge, fver);
          ut_print_neperbug ();
        }
        if ((*pFlatten).VerDom[fver][0] != 1)
          abort ();

        (*pFlatten).DomFaceTessVerNb[i] =
          ut_realloc_1d_int ((*pFlatten).DomFaceTessVerNb[i],
                             ++(*pFlatten).DomFaceTessVerQty[i]);
        (*pFlatten).DomFaceTessVerNb[i][(*pFlatten).DomFaceTessVerQty[i] -
                                        1] = fver;
      }
    }

    // Adding vers of the vers
    for (j = 1; j <= DTess.FaceVerQty[i]; j++)
    {
      dver = DTess.FaceVerNb[i][j];
      fver = (*pFlatten).DomVerTessVerNb[dver];
      if (dver < 1 || fver < 1)
      {
        printf ("dver = %d < 1 || fver = %d < 1\n", dver, fver);
        ut_print_neperbug ();
      }
      if ((*pFlatten).VerDom[fver][0] != 0)
      {
        printf ("(*pFlatten).VerDom[%d][0] == %d != 0\n", fver,
                (*pFlatten).VerDom[fver][0]);
        abort ();
      }

      (*pFlatten).DomFaceTessVerNb[i] =
        ut_realloc_1d_int ((*pFlatten).DomFaceTessVerNb[i],
                           ++(*pFlatten).DomFaceTessVerQty[i]);
      (*pFlatten).DomFaceTessVerNb[i][(*pFlatten).DomFaceTessVerQty[i] - 1] =
        fver;
    }
  }

  return;
}

void
neut_flatten_init_domstuff_fromedge (struct FLATTEN *pFlatten,
                                     struct TESS DTess)
{
  int i, j, k, dedge, dface, fedge, dpoly;

  (*pFlatten).DomEdgeTessEdgeQty = ut_alloc_1d_int ((*pFlatten).DEdgeQty + 1);
  (*pFlatten).DomEdgeTessEdgeNb = ut_alloc_1d_pint ((*pFlatten).DEdgeQty + 1);
  (*pFlatten).DomFaceBTessEdgeQty =
    ut_alloc_1d_int ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomFaceBTessEdgeNb =
    ut_alloc_1d_pint ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomPolyBTessEdgeQty =
    ut_alloc_1d_int ((*pFlatten).DPolyQty + 1);
  (*pFlatten).DomPolyBTessEdgeNb =
    ut_alloc_1d_pint ((*pFlatten).DPolyQty + 1);

  for (i = 1; i <= (*pFlatten).EdgeQty; i++)
    if ((*pFlatten).EdgeDom[i][0] == 1)
    {
      dedge = (*pFlatten).EdgeDom[i][1];
      (*pFlatten).DomEdgeTessEdgeQty[dedge]++;
      (*pFlatten).DomEdgeTessEdgeNb[dedge] =
        ut_realloc_1d_int ((*pFlatten).DomEdgeTessEdgeNb[dedge],
                           (*pFlatten).DomEdgeTessEdgeQty[dedge]);
      (*pFlatten).DomEdgeTessEdgeNb[dedge][(*pFlatten).
                                           DomEdgeTessEdgeQty[dedge] - 1] = i;
    }
    else if ((*pFlatten).EdgeDom[i][0] == 2)
    {
      dface = (*pFlatten).EdgeDom[i][1];
      (*pFlatten).DomFaceBTessEdgeQty[dface]++;
      (*pFlatten).DomFaceBTessEdgeNb[dface] =
        ut_realloc_1d_int ((*pFlatten).DomFaceBTessEdgeNb[dface],
                           (*pFlatten).DomFaceBTessEdgeQty[dface]);
      (*pFlatten).DomFaceBTessEdgeNb[dface][(*pFlatten).
                                            DomFaceBTessEdgeQty[dface] - 1] =
        i;
    }
    else if ((*pFlatten).EdgeDom[i][0] == 3)
    {
      dpoly = (*pFlatten).EdgeDom[i][1];
      (*pFlatten).DomPolyBTessEdgeQty[dpoly]++;
      (*pFlatten).DomPolyBTessEdgeNb[dpoly] =
        ut_realloc_1d_int ((*pFlatten).DomPolyBTessEdgeNb[dpoly],
                           (*pFlatten).DomPolyBTessEdgeQty[dpoly]);
      (*pFlatten).DomPolyBTessEdgeNb[dpoly][(*pFlatten).
                                            DomPolyBTessEdgeQty[dpoly] - 1] =
        i;
    }
    else
      abort ();

  // Computing DomFaceTessEdgeQty / DomFaceTessEdgeNb from
  // DomFaceBTessEdgeQty / DomFaceBTessEdgeNb and
  // DomEdgeTessEdgeQty / DomEdgeTessEdgeNb.

  (*pFlatten).DomFaceTessEdgeQty = ut_alloc_1d_int ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomFaceTessEdgeNb = ut_alloc_1d_pint ((*pFlatten).DFaceQty + 1);

  for (i = 1; i <= DTess.FaceQty; i++)
  {
    (*pFlatten).DomFaceTessEdgeQty[i] = (*pFlatten).DomFaceBTessEdgeQty[i];
    (*pFlatten).DomFaceTessEdgeNb[i] =
      ut_alloc_1d_int ((*pFlatten).DomFaceTessEdgeQty[i]);

    ut_array_1d_int_memcpy ((*pFlatten).DomFaceBTessEdgeNb[i],
                            (*pFlatten).DomFaceBTessEdgeQty[i],
                            (*pFlatten).DomFaceTessEdgeNb[i]);

    for (j = 1; j <= DTess.FaceVerQty[i]; j++)
    {
      dedge = DTess.FaceEdgeNb[i][j];
      for (k = 0; k < (*pFlatten).DomEdgeTessEdgeQty[dedge]; k++)
      {
        fedge = (*pFlatten).DomEdgeTessEdgeNb[dedge][k];

        (*pFlatten).DomFaceTessEdgeNb[i] =
          ut_realloc_1d_int ((*pFlatten).DomFaceTessEdgeNb[i],
                             ++(*pFlatten).DomFaceTessEdgeQty[i]);
        (*pFlatten).DomFaceTessEdgeNb[i][(*pFlatten).DomFaceTessEdgeQty[i] -
                                         1] = fedge;
      }
    }
  }

  return;
}

void
neut_flatten_init_domstuff_fromface (struct FLATTEN *pFlatten)
{
  int i, dface, dpoly;

  (*pFlatten).DomFaceTessFaceQty = ut_alloc_1d_int ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomFaceTessFaceNb = ut_alloc_1d_pint ((*pFlatten).DFaceQty + 1);
  (*pFlatten).DomPolyBTessFaceQty =
    ut_alloc_1d_int ((*pFlatten).DPolyQty + 1);
  (*pFlatten).DomPolyBTessFaceNb =
    ut_alloc_1d_pint ((*pFlatten).DPolyQty + 1);

  for (i = 1; i <= (*pFlatten).FaceQty; i++)
    if ((*pFlatten).FaceDom[i][0] == 2)
    {
      dface = (*pFlatten).FaceDom[i][1];
      (*pFlatten).DomFaceTessFaceQty[dface]++;
      (*pFlatten).DomFaceTessFaceNb[dface] =
        ut_realloc_1d_int ((*pFlatten).DomFaceTessFaceNb[dface],
                           (*pFlatten).DomFaceTessFaceQty[dface]);
      (*pFlatten).DomFaceTessFaceNb[dface][(*pFlatten).
                                           DomFaceTessFaceQty[dface] - 1] = i;
    }
    else if ((*pFlatten).FaceDom[i][0] == 3)
    {
      dpoly = (*pFlatten).FaceDom[i][1];
      (*pFlatten).DomPolyBTessFaceQty[dpoly]++;
      (*pFlatten).DomPolyBTessFaceNb[dpoly] =
        ut_realloc_1d_int ((*pFlatten).DomPolyBTessFaceNb[dpoly],
                           (*pFlatten).DomPolyBTessFaceQty[dpoly]);
      (*pFlatten).DomPolyBTessFaceNb[dpoly][(*pFlatten).
                                            DomPolyBTessFaceQty[dpoly] - 1] =
        i;
    }
    else
      abort ();

  return;
}

void
neut_flatten_free (struct FLATTEN *pFlatten)
{
  int i;

  ut_free_2d_int (&(*pFlatten).VerDom, (*pFlatten).VerQty + 1);
  ut_free_2d_int (&(*pFlatten).EdgeDom, (*pFlatten).EdgeQty + 1);
  ut_free_2d_int (&(*pFlatten).FaceDom, (*pFlatten).FaceQty + 1);
  ut_free_2d_int (&(*pFlatten).PolyDom, (*pFlatten).PolyQty + 1);

  ut_free_1d_int (&(*pFlatten).DomVerTessVerNb);
  ut_free_1d_int (&(*pFlatten).DomEdgeBTessVerQty);
  ut_free_2d_int (&(*pFlatten).DomEdgeBTessVerNb, (*pFlatten).DEdgeQty + 1);
  ut_free_1d_int (&(*pFlatten).DomEdgeTessVerQty);
  ut_free_2d_int (&(*pFlatten).DomEdgeTessVerNb, (*pFlatten).DEdgeQty + 1);
  ut_free_1d_int (&(*pFlatten).DomFaceBTessVerQty);
  ut_free_2d_int (&(*pFlatten).DomFaceBTessVerNb, (*pFlatten).DFaceQty + 1);
  ut_free_1d_int (&(*pFlatten).DomFaceTessVerQty);
  ut_free_2d_int (&(*pFlatten).DomFaceTessVerNb, (*pFlatten).DFaceQty + 1);
  ut_free_1d_int (&(*pFlatten).DomPolyBTessVerQty);
  ut_free_2d_int (&(*pFlatten).DomPolyBTessVerNb, (*pFlatten).DPolyQty + 1);

  ut_free_1d_int (&(*pFlatten).DomEdgeTessEdgeQty);
  ut_free_2d_int (&(*pFlatten).DomEdgeTessEdgeNb, (*pFlatten).DEdgeQty + 1);
  ut_free_1d_int (&(*pFlatten).DomFaceBTessEdgeQty);
  ut_free_2d_int (&(*pFlatten).DomFaceBTessEdgeNb, (*pFlatten).DFaceQty + 1);
  ut_free_1d_int (&(*pFlatten).DomFaceTessEdgeQty);
  ut_free_2d_int (&(*pFlatten).DomFaceTessEdgeNb, (*pFlatten).DFaceQty + 1);
  ut_free_1d_int (&(*pFlatten).DomPolyBTessEdgeQty);
  ut_free_2d_int (&(*pFlatten).DomPolyBTessEdgeNb, (*pFlatten).DPolyQty + 1);

  ut_free_1d_int (&(*pFlatten).DomFaceTessFaceQty);
  ut_free_2d_int (&(*pFlatten).DomFaceTessFaceNb, (*pFlatten).DFaceQty + 1);
  ut_free_1d_int (&(*pFlatten).DomPolyBTessFaceQty);
  ut_free_2d_int (&(*pFlatten).DomPolyBTessFaceNb, (*pFlatten).DPolyQty + 1);

  for (i = 1; i <= (*pFlatten).VerQty; i++)
    ut_free_2d_int (&(*pFlatten).VerTessVerNb[i],
                    (*pFlatten).VerTessVerQty[i]);
  ut_free_1d_ppint (&(*pFlatten).VerTessVerNb);
  ut_free_1d_int (&(*pFlatten).VerTessVerQty);

  for (i = 1; i <= (*pFlatten).EdgeQty; i++)
    ut_free_2d_int (&(*pFlatten).EdgeTessEdgeNb[i],
                    (*pFlatten).EdgeTessEdgeQty[i]);
  free ((*pFlatten).EdgeTessEdgeNb);
  ut_free_1d_int (&(*pFlatten).EdgeTessEdgeQty);

  (*pFlatten).VerQty = 0;
  (*pFlatten).EdgeQty = 0;
  (*pFlatten).FaceQty = 0;
  (*pFlatten).PolyQty = 0;

  return;
}
