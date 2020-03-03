/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

/* Delete edge from face; note that this routine DOES modify */
/* FaceVerQty (it is also the face edge quantity).		 */
void
net_reg_merge_delFromFace (struct TESS *pTess, int face, int edge,
                           int verbosity)
{
  int i;
  int pos;

  if (verbosity >= 3)
  {
    printf ("Deleting edge %d from face %d.\n", edge, face);

    ut_print_lineheader (0);
    printf ("    ");
    for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
      printf ("%d ", (*pTess).FaceEdgeNb[face][i]);
  }

  pos =
    oneDIntEltPos ((*pTess).FaceEdgeNb[face], 1, (*pTess).FaceVerQty[face],
                   edge);

  ut_array_1d_int_rmelt ((*pTess).FaceEdgeNb[face] + 1,
                         (*pTess).FaceVerQty[face], edge, 1);

  ut_array_1d_int_memcpy ((*pTess).FaceEdgeOri[face] + pos + 1,
                          (*pTess).FaceVerQty[face] - pos,
                          (*pTess).FaceEdgeOri[face] + pos);

  (*pTess).FaceVerQty[face]--;

  if (verbosity >= 3)
  {
    printf (" become  ");
    for (i = 1; i <= (*pTess).FaceVerQty[face]; i++)
      printf ("%d ", (*pTess).FaceEdgeNb[face][i]);
    printf ("\n");
  }

  return;
}

/* Delete ver from face; note that this routine DOES NOT modify
 * FaceVerQty.
 */
void
DeleteVerFromFace (struct TESS *pTess, int face, int delver, int verbosity)
{
  int qty;                      /* for the test */

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("Deleting ver %d from face %d\n", delver, face);
  }

  qty =
    ut_array_1d_int_rmelt ((*pTess).FaceVerNb[face] + 1,
                           (*pTess).FaceVerQty[face], delver, 1);

  /* test */
  if (qty != 1)
  {
    printf ("qty!=1 =%d\n", qty);
    abort ();
  }

  return;
}

/* Delete ver from face; note that this routine modifies
 * FaceVerQty.
 */
void
ReplaceVerInFace (struct TESS *pTess, int face, int delver, int newver)
{
  int pos;                      /* for the test */

  /* ut_print_lineheader(-1);
   * printf("Replacing ver %d by ver %d in face %d\n",delver,newver,face);
   */

  pos =
    oneDIntEltPos ((*pTess).FaceVerNb[face], 1, (*pTess).FaceVerQty[face],
                   delver);

  /* test */
  if (pos == -1)
  {
    printf ("pos==-1 =%d\n", pos);
    abort ();
  }

  (*pTess).FaceVerNb[face][pos] = newver;

  return;
}

int
DeleteFace (struct TESS *pTess, int face, int edge, int verbosity)
{
  int i, j, k;
  int tmpface;
  int tmpver;
  int nnew;
  int *pnew = &nnew;
  int old;
  int *pold = &old;
  int ori;
  int pos;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("Deleting face %d\n", face);
  }

  DeleteFaceFromItsPoly (pTess, face);

  /* We search out the two remaining edges of the face; they
   * become the same: we keep the one with the lower id.
   */

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Deleting one of its two remaining edges: ");
  }
  /* we determine which are the numbers of the edge we keep (nnew) and the
   * edge we delete (old) -- They are now the same, we keep the lower nb.
   */
  SearchNewNOldEdges (pTess, face, edge, pnew, pold);
  if (verbosity >= 3)
    printf ("%d (keeping %d)\n", old, nnew);

  if ((*pTess).EdgeVerNb[nnew][0] == (*pTess).EdgeVerNb[old][0]
      || (*pTess).EdgeVerNb[nnew][1] == (*pTess).EdgeVerNb[old][1])
    ori = 1;
  else
    ori = -1;

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("      Giving edge %d the faces of edge %d\n", nnew, old);

    ut_print_lineheader (-1);
    printf ("        edge %d: ", nnew);
    for (i = 0; i <= (*pTess).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[nnew][i]);
    printf (" and  ");
    printf ("edge %d: ", old);
    for (i = 0; i <= (*pTess).EdgeFaceQty[old] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[old][i]);
    printf ("become:\n");
  }

  /* for the nnew edge: determination of the face properties: qty & nbs
   */
  (*pTess).EdgeFaceNb[nnew] =
    ut_realloc_1d_int ((*pTess).EdgeFaceNb[nnew],
                       (*pTess).EdgeFaceQty[nnew] +
                       (*pTess).EdgeFaceQty[old]);
  ut_array_1d_int_memcpy ((*pTess).EdgeFaceNb[old], (*pTess).EdgeFaceQty[old],
                          (*pTess).EdgeFaceNb[nnew] +
                          (*pTess).EdgeFaceQty[nnew]);

  (*pTess).EdgeFaceQty[nnew] += (*pTess).EdgeFaceQty[old];

  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pTess).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pTess).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[nnew][i]);
    printf ("\n");

    printf ("sorting\n");
  }
  ut_array_1d_int_sort ((*pTess).EdgeFaceNb[nnew],
                        (*pTess).EdgeFaceQty[nnew]);
  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pTess).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pTess).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[nnew][i]);
    printf ("\n");

    printf ("compressing\n");
  }

  // Replacing this by ut_array_1d_int_uniq leads to a bug for
  // neper -FM n2-id1.tesl -fmax 20
  (*pTess).EdgeFaceQty[nnew] =
    1 + oneDIntCompress ((*pTess).EdgeFaceNb[nnew], 0,
                         (*pTess).EdgeFaceQty[nnew] - 1);
  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pTess).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pTess).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[nnew][i]);
    printf ("\n");

    printf ("deleting and compressing\n");
  }

  (*pTess).EdgeFaceQty[nnew] -=
    oneDIntDeleteNCompress ((*pTess).EdgeFaceNb[nnew], 0,
                            (*pTess).EdgeFaceQty[nnew], face, 2);
  if (verbosity >= 3)
  {
    printf ("EdgeFaceNb: %d: ", (*pTess).EdgeFaceQty[nnew]);
    for (i = 0; i <= (*pTess).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[nnew][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf ("        edge %d: ", nnew);
    for (i = 0; i <= (*pTess).EdgeFaceQty[nnew] - 1; i++)
      printf ("%d ", (*pTess).EdgeFaceNb[nnew][i]);
    printf ("\n");

    ut_print_lineheader (-1);
    printf
      ("      Changing edge %d to edge %d in the parent faces of edge %d\n",
       old, nnew, old);
  }

  /* for the faces of the old edge: edge nb=old <-- nnew
   */
  for (i = 0; i < (*pTess).EdgeFaceQty[old]; i++)
  {
    tmpface = (*pTess).EdgeFaceNb[old][i];
    if (tmpface == face)        /* this is the deleted face */
      continue;

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("        face %d: ", tmpface);
      for (j = 1; j <= (*pTess).FaceVerQty[tmpface]; j++)
        printf ("%d ", (*pTess).FaceEdgeNb[tmpface][j]);
      printf ("\n");
    }

    pos =
      oneDIntEltPos ((*pTess).FaceEdgeNb[tmpface], 1,
                     (*pTess).FaceVerQty[tmpface], old);
    (*pTess).FaceEdgeNb[tmpface][pos] = nnew;
    (*pTess).FaceEdgeOri[tmpface][pos] *= ori;

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("          become: ");
      for (j = 1; j <= (*pTess).FaceVerQty[tmpface]; j++)
        printf ("%d ", (*pTess).FaceEdgeNb[tmpface][j]);
      printf ("\nvertices:\n");
      for (j = 1; j <= (*pTess).FaceVerQty[tmpface]; j++)
        printf ("%d ", (*pTess).FaceVerNb[tmpface][j]);
      printf ("\n");
    }

    for (j = 1; j <= (*pTess).FaceVerQty[tmpface] - 1; j++)
      if ((*pTess).FaceEdgeNb[tmpface][j] ==
          (*pTess).FaceEdgeNb[tmpface][j + 1])
      {
        ut_array_1d_int_rmelt ((*pTess).FaceEdgeNb[tmpface] + 1,
                               (*pTess).FaceVerQty[tmpface],
                               (*pTess).FaceEdgeNb[tmpface][j], 2);
        for (k = (*pTess).FaceVerQty[tmpface]; k >= j + 2; k--)
          (*pTess).FaceEdgeOri[tmpface][k - 2] =
            (*pTess).FaceEdgeOri[tmpface][k];
        for (k = (*pTess).FaceVerQty[tmpface]; k >= j + 2; k--)
          (*pTess).FaceVerNb[tmpface][k - 2] = (*pTess).FaceVerNb[tmpface][k];

        (*pTess).FaceVerQty[tmpface] -= 2;
        if (verbosity >= 2)
        {
          ut_print_message (1, 3, "critical case detected\n");

          for (j = 1; j <= (*pTess).FaceVerQty[tmpface]; j++)
            printf ("%d ", (*pTess).FaceEdgeNb[tmpface][j]);
          printf ("\n");
        }

        break;
      }
    if ((*pTess).FaceEdgeNb[tmpface][1] ==
        (*pTess).FaceEdgeNb[tmpface][(*pTess).FaceVerQty[tmpface]])
    {
      for (k = 1; k <= (*pTess).FaceVerQty[tmpface] - 1; k++)
        (*pTess).FaceEdgeNb[tmpface][k] = (*pTess).FaceEdgeNb[tmpface][k + 1];
      for (k = 1; k <= (*pTess).FaceVerQty[tmpface] - 1; k++)
        (*pTess).FaceEdgeOri[tmpface][k] =
          (*pTess).FaceEdgeOri[tmpface][k + 1];
      for (k = 1; k <= (*pTess).FaceVerQty[tmpface] - 1; k++)
        (*pTess).FaceVerNb[tmpface][k] = (*pTess).FaceVerNb[tmpface][k + 1];

      (*pTess).FaceVerQty[tmpface] -= 2;
      if (verbosity >= 2)
        ut_print_message (1, 3, "critical case 2 detected\n");
      return 1;
    }
  }

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("      Deleting edge %d in its 2 vertices\n", old);
  }
  /* for the vertices of the old edge */
  for (i = 0; i <= 1; i++)
  {
    tmpver = (*pTess).EdgeVerNb[old][i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("        ver %d: ", tmpver);
      for (j = 0; j <= (*pTess).VerEdgeQty[tmpver] - 1; j++)
        printf ("%d ", (*pTess).VerEdgeNb[tmpver][j]);
      printf (" become  ");
    }

    (*pTess).VerEdgeQty[tmpver] -=
      oneDIntDeleteNCompress ((*pTess).VerEdgeNb[tmpver], 0,
                              (*pTess).VerEdgeQty[tmpver] - 1, old, 1);

    if (verbosity >= 3)
    {
      for (j = 0; j <= (*pTess).VerEdgeQty[tmpver] - 1; j++)
        printf ("%d ", (*pTess).VerEdgeNb[tmpver][j]);
      printf ("\n");
    }
  }

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Setting edge %d as useless\n", old);
  }
  (*pTess).EdgeState[old] = -1;

  if ((*pTess).EdgeDom[old][0] == 1)    // means old is tronger than nnew since nnew
    // should not have a domain edge
  {
    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("    Changing bound data of edge %d to the one of edge %d\n",
              nnew, old);
      ut_print_lineheader (-1);
      printf ("    %d %d becomes %d %d\n", (*pTess).EdgeDom[nnew][0],
              (*pTess).EdgeDom[nnew][1], (*pTess).EdgeDom[old][0],
              (*pTess).EdgeDom[old][1]);
    }

    ut_array_1d_int_memcpy ((*pTess).EdgeDom[old], 2, (*pTess).EdgeDom[nnew]);
  }

  /* ut_print_lineheader(-1); printf("      Setting face %d ver qty to 0\n",face); */
  /* (*pTess).FaceVerQty[face]=0;      */

  /* the face is set as useless       */
  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    Setting face %d as useless\n", face);
  }
  (*pTess).FaceState[face] = -1;

  return 0;
}
