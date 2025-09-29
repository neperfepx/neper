/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_del_.h"

void
UpdateFaceState (struct TESS *pTess, int delver, int newver)
{
  int i, face, NewVerFaceQty, DelVerFaceQty;
  int *NewVerFace = NULL;
  int *DelVerFace = NULL;

  neut_tess_ver_faces (*pTess, delver, &DelVerFace, &DelVerFaceQty);
  neut_tess_ver_faces (*pTess, newver, &NewVerFace, &NewVerFaceQty);

  /* every face of the deleted and new vertices will be modified, so
   * their states are set to 1. Maybe some faces will be deleted: in this
   * case, their states will be set to -1 afterwards.
   */
  for (i = 0; i < DelVerFaceQty; i++)
  {
    face = DelVerFace[i];
    (*pTess).FaceState[face] = 1;
  }

  for (i = 0; i < NewVerFaceQty; i++)
  {
    face = NewVerFace[i];
    (*pTess).FaceState[face] = 1;
  }

  ut_free_1d_int (&NewVerFace);
  ut_free_1d_int (&DelVerFace);

  return;
}

/* Delete ver from face; note that this routine DOES NOT modify
 * FaceVerQty (it is also the face edge quantity).
 * FaceVerQty is to be modified either in DeleteVerFromFacep
 * or in net_reg_merge_delFromFacep, if UpdateFaceVer is after UpdateFaceEdgep
 * and if vice versa, respectively.
 */
void
UpdateFaceVer (struct TESS *pTess, int delver, int newver, int verbosity)
{
  int i, j, face, NewVerFaceQty, DelVerFaceQty;
  int *NewVerFace = NULL;
  int *DelVerFace = NULL;

  /* Calculating DelVerFace and NewVerFace */
  neut_tess_ver_faces (*pTess, delver, &DelVerFace, &DelVerFaceQty);
  neut_tess_ver_faces (*pTess, newver, &NewVerFace, &NewVerFaceQty);

  for (i = 0; i < DelVerFaceQty; i++)
  {
    face = DelVerFace[i];

    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("Treating delver face %d (%d/%d) ---\n", face, i,
              DelVerFaceQty);
      ut_print_lineheader (-1);
      printf ("  %d faces: ", (*pTess).FaceVerQty[face]);
      for (j = 1; j <= (*pTess).FaceVerQty[face]; j++)
        printf ("%d ", (*pTess).FaceVerNb[face][j]);
      printf ("\n");
    }

    if (ut_array_1d_int_eltpos (NewVerFace, NewVerFaceQty, face) != -1)
      DeleteVerFromFace (pTess, face, delver, verbosity);
    else
      ReplaceVerInFace (pTess, face, delver, newver);
  }

  ut_free_1d_int (&NewVerFace);
  ut_free_1d_int (&DelVerFace);

  return;
}

/* Delete edge from face; note that this routine MODIFIES
 * FaceVerQty (it is also the face edge quantity).
 * FaceVerQty is to be modified either in DeleteVerFromFacep
 * or in net_reg_merge_delFromFacep, if UpdateFaceVer is after UpdateFaceEdgep
 * and vice versa, respectively.
 */
int
UpdateFaceEdge (struct TESS *pTess, int edge, int verbosity)
{
  int i;
  int face;
  int err = 0;

  for (i = 0; i < (*pTess).EdgeFaceQty[edge]; i++)
  {
    face = (*pTess).EdgeFaceNb[edge][i];
    if (verbosity >= 3)
    {
      ut_print_lineheader (-1);
      printf ("Treating face %d (%d/%d) ---\n", face, i + 1,
              (*pTess).EdgeFaceQty[edge]);
    }

    if ((*pTess).FaceState[face] == -1)
    {
      ut_print_lineheader (2);
      printf ("The face to remove (%d) is already useless!\n", face);
      abort ();
    }

    if (verbosity >= 3)
    {
      ut_print_lineheader (0);
      printf ("  - face %d contains %d edges: ", face,
              (*pTess).FaceVerQty[face]);
    }

    /* if it contains almost 4 edges, the edge is removed;
     * else, the whole face is removed.                     */
    if ((*pTess).FaceVerQty[face] >= 4)
      net_reg_merge_delFromFace (pTess, face, edge, verbosity);
    else
      err = DeleteFace (pTess, face, edge, verbosity);
  }

  return err;
}

void
UpdateVerState (struct TESS *pTess, int delver, int newver, int verbosity)
{
  int wgt1, wgt2;

  if (verbosity >= 3)
  {
    ut_print_lineheader (0);
    printf ("  Updating state of ver %d\n", newver);
    ut_print_lineheader (0);
    printf ("    ver %d: %d and ver %d: %d become ", newver,
            (*pTess).VerState[newver], delver, (*pTess).VerState[delver]);
  }
  /* if state of newver is 0, its wgt is 1, else it is the state */
  wgt1 = ut_num_max_int (1, (*pTess).VerState[newver]);
  /* if state of delver is 0, its wgt is 1, else it is the state */
  wgt2 = ut_num_max_int (1, (*pTess).VerState[delver]);

  /* wgt of newver becomes wgt1+wgt2 */
  (*pTess).VerState[newver] = wgt1 + wgt2;
  /* state of delver is set to -1 --> useless */
  (*pTess).VerState[delver] = -1;

  if (verbosity >= 3)
    printf ("ver %d: %d\n", newver, (*pTess).VerState[newver]);

  return;
}

/* Updating of VerEdgeQty & VerEdgeNb */
void
UpdateVerEdge (struct TESS *pTess, int edge, int delver, int newver,
               int verbosity)
{
  int i;
  int *array;                   /* will be the VerEdgeNb  of newver ver */
  int qty;                      /* will be the VerEdgeQty of newver ver */

  ut_array_1d_int_cat ((*pTess).VerEdgeNb[newver],
                       (*pTess).VerEdgeQty[newver],
                       (*pTess).VerEdgeNb[delver],
                       (*pTess).VerEdgeQty[delver], &array, NULL);

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("  Giving ver %d the edges of ver %d\n", newver, delver);

    ut_print_lineheader (-1);
    printf ("    ver %d: ", newver);
    for (i = 0; i <= (*pTess).VerEdgeQty[newver] - 1; i++)
      printf ("%d ", (*pTess).VerEdgeNb[newver][i]);

    printf ("and ver %d: ", delver);
    for (i = 0; i <= (*pTess).VerEdgeQty[delver] - 1; i++)
      printf ("%d ", (*pTess).VerEdgeNb[delver][i]);
    printf ("become\n");
  }

  qty = (*pTess).VerEdgeQty[delver] + (*pTess).VerEdgeQty[newver];
  qty -= ut_array_1d_int_rmelt (array, qty, edge, 2);

  (*pTess).VerEdgeQty[newver] = qty;
  (*pTess).VerEdgeNb[newver] =
    ut_realloc_1d_int ((*pTess).VerEdgeNb[newver], qty);
  ut_array_1d_int_memcpy (array, qty, (*pTess).VerEdgeNb[newver]);

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    ver %d: ", newver);
    for (i = 0; i <= (*pTess).VerEdgeQty[newver] - 1; i++)
      printf ("%d ", (*pTess).VerEdgeNb[newver][i]);
    printf ("\n");
  }

  ut_free_1d_int (&array);

  return;
}

int
UpdateVerBound (struct TESS *pTess, int delver, int newver)
{
  // vertices are not on the domain: nothing to do
  if ((*pTess).VerDom[delver][0] == -1 && (*pTess).VerDom[newver][0] == -1)
    return 0;

  // one vertex is on a domain vertex
  else if ((*pTess).VerDom[delver][0] == 0 || (*pTess).VerDom[newver][0] == 0)
  {
    // test: both vertices are domain vertices; this should not happen
    if ((*pTess).VerDom[delver][0] == (*pTess).VerDom[newver][0])
      ut_print_neperbug ();

    // delver is the domain ver: copying;
    // otherwise newver is the domain ver: no need to copy
    if ((*pTess).VerDom[delver][0] == 0)
      ut_array_1d_int_memcpy ((*pTess).VerDom[delver], 2,
                              (*pTess).VerDom[newver]);
  }

  // one vertex in on a domain edge
  else if ((*pTess).VerDom[delver][0] == 1 || (*pTess).VerDom[newver][0] == 1)
  {
    // test: both vertices are on a domain edge, but not on the same
    // becoming a domain vertex
    if (((*pTess).VerDom[delver][0] == (*pTess).VerDom[newver][0])
        && ((*pTess).VerDom[delver][1] != (*pTess).VerDom[newver][1]))
      ut_print_neperbug ();     // the edge should not have been marked for deletion in NextEdgeToDel

    // delver is on the domain edge: copying;
    // otherwise newver is on the domain edge: no need to copy
    else if ((*pTess).VerDom[delver][0] == 1)
      ut_array_1d_int_memcpy ((*pTess).VerDom[delver], 2,
                              (*pTess).VerDom[newver]);
  }

  // one vertex in on a domain face
  else if ((*pTess).VerDom[delver][0] == 2 || (*pTess).VerDom[newver][0] == 2)
  {
    // if delver: copying, otherwise: no need to copy
    if ((*pTess).VerDom[delver][0] == 2)
      ut_array_1d_int_memcpy ((*pTess).VerDom[delver], 2,
                              (*pTess).VerDom[newver]);
  }

  return 0;
}

void
UpdateEdgeLength (struct TESS *pTess, int ver)
{
  int i;

  for (i = 0; i < (*pTess).VerEdgeQty[ver]; i++)
    neut_tess_init_edgelength_edge (pTess, (*pTess).VerEdgeNb[ver][i]);

  return;
}

int
UpdateVerCooBary (struct TESS *pTess, int delver, int newver, int verbosity)
{
  int i, domedge, domedgeqty, domver1, domver2, eqqty;
  int *domedges = NULL;
  double **ptcoo = ut_alloc_2d (2, 3);
  double **eqs = NULL;
  int *vers = ut_alloc_1d_int (2);
  double *v = ut_alloc_1d (3);
  double *n = ut_alloc_1d (3);

  vers[0] = delver;
  vers[1] = newver;

  if (verbosity)
    printf ("delver = %d newver = %d\n", delver, newver);

  neut_tess_vers_alldomedges (*pTess, vers, 2, &domedges, &domedgeqty);

  if (verbosity)
  {
    printf ("domedgeqty = %d\n", domedgeqty);
    printf ("domedges = ");
    ut_array_1d_int_fprintf (stdout, domedges, domedgeqty, "%d");
  }

  eqs = ut_alloc_2d (domedgeqty + 1, 4);

  eqqty = 1;
  ut_array_1d_memcpy ((*pTess).FaceEq[1], 4, eqs[0]);

  for (i = 0; i < domedgeqty; i++)
  {
    domedge = domedges[i];

    if (!strcmp ((*pTess).DomEdgeType[domedge], "line"))
    {
      domver1 = (*pTess).DomEdgeVerNb[domedge][0];
      domver2 = (*pTess).DomEdgeVerNb[domedge][1];
      if (verbosity)
        printf ("domedge = %d domver1 = %d domver2 = %d\n", domedge, domver1, domver2);
      if (domver1 > 0 && domver2 > 0 && !strstr ((*pTess).DomEdgeLabel[domedge], "cut"))
      {
        eqqty++;
        ut_space_points_uvect ((*pTess).DomVerCoo[domver1],
                               (*pTess).DomVerCoo[domver2], v);
        ut_vector_vectprod (v, eqs[0] + 1, n);
        ut_space_point_normal_plane ((*pTess).DomVerCoo[domver1], n,
                                     eqs[i + 1]);
      }
    }

    else
      neut_primparms_point_tangentplane ((*pTess).DomEdgeType[domedge],
                                         (*pTess).DomEdgeParms[domedge],
                                         (*pTess).VerCoo[newver],
                                         eqs[i + 1]);
  }

  if (verbosity)
  {
    printf ("eqqty = %d\n", eqqty);
    printf ("eqs = ");
    ut_array_2d_fprintf (stdout, eqs, eqqty, 4, "%f");
  }

  ut_array_1d_memcpy ((*pTess).VerCoo[delver], 3, ptcoo[0]);
  ut_array_1d_memcpy ((*pTess).VerCoo[newver], 3, ptcoo[1]);

  if (verbosity)
  {
    printf ("ptcoo[0] = ");
    ut_array_1d_fprintf (stdout, ptcoo[0], 3, "%f");
    printf ("ptcoo[1] = ");
    ut_array_1d_fprintf (stdout, ptcoo[1], 3, "%f");
  }

  ut_space_points_bary_constrained (ptcoo, NULL, 2, eqs, eqqty,
                                    (*pTess).VerCoo[newver]);

  if (verbosity)
  {
    printf ("(*pTess).VerCoo[newver] = ");
    ut_array_1d_fprintf (stdout, (*pTess).VerCoo[newver], 3, "%f");
  }

  ut_free_2d (&ptcoo, 2);
  ut_free_2d (&eqs, domedgeqty + 1);
  ut_free_1d (&v);
  ut_free_1d (&n);

  return 0;
}

// See documentation file `edgedel.tarpdf' for details on the algorithm.
int
UpdateVerCooMiniFF (struct TESS *pTess, int newver, int verbosity)
{
  int status = 0;
  int i, j, k;
  double **A = NULL;
  double *B = NULL;
  double *X = NULL;
  int faceqty;
  int *face = NULL;
  int N;                        // nb of faces
  int M;                        // nb of constrained faces

  neut_tess_ver_faces (*pTess, newver, &face, &faceqty);

  if (verbosity > 0)
  {
    printf ("newver = %d has bound: %d %d\n", newver,
            (*pTess).VerDom[newver][0], (*pTess).VerDom[newver][1]);

    printf ("newver = %d has %d faces of nbs: ", newver, faceqty);
    ut_array_1d_int_fprintf (stdout, face, faceqty, "%d");
    printf ("and bounds: \n");
    for (i = 0; i < faceqty; i++)
      ut_array_1d_int_fprintf (stdout, (*pTess).FaceDom[face[i]], 2, "%d");
  }

  N = faceqty;
  M = 0;
  int *domainface = ut_alloc_1d_int (N);
  // this is an indirect way, but we need face anyway...
  for (i = 0; i < N; i++)
    if ((*pTess).FaceDom[face[i]][0] == 2)
      domainface[M++] = (*pTess).FaceDom[face[i]][1];

  if (verbosity > 0)
  {
    printf ("domainface = ");
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");
    ut_array_1d_int_sort_uniq (domainface, M, &M);
    printf ("-> ");
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");

    printf ("and %d domain faces of nbs: ", M);
    ut_array_1d_int_fprintf (stdout, domainface, M, "%d");
  }

  int domfaceqty;
  int *tmp = NULL;
  neut_tess_ver_domfaces (*pTess, newver, &tmp, &domfaceqty);

  if (verbosity > 0)
  {
    printf ("neut_tess_ver_domfaces returns qty = %d and ids = ", domfaceqty);
    ut_array_1d_int_fprintf (stdout, tmp, domfaceqty, "%d");
  }

  if (verbosity)
    printf ("domfaceqty = %d\n", domfaceqty);

  int curved, iter;
  double dist, **constraint = ut_alloc_2d (domfaceqty, 4);
  double *eq = ut_alloc_1d (4);

  curved = 0;
  for (i = 0; i < domfaceqty; i++)
    if (strncmp ((*pTess).DomFaceType[tmp[i]], "plane", 5))
    {
      curved = 1;
      break;
    }

  iter = 0;
  do
  {
    iter++;

    M = 0;
    for (i = 0; i < domfaceqty; i++)
    {
      neut_primparms_point_tangentplane ((*pTess).DomFaceType[tmp[i]],
                                         (*pTess).DomFaceParms[tmp[i]],
                                         (*pTess).VerCoo[newver],
                                         eq);

      int constraint_active = 1;
      for (j = 0; j < M; j++)
        if (ut_array_1d_equal (eq, constraint[j], 4, 1e-6))
        {
          constraint_active = 0;
          break;
        }

      if (verbosity)
        printf ("constraint_active = %d\n", constraint_active);

      if (constraint_active)
      {
        ut_array_1d_memcpy (eq, 4, constraint[M]);
        M++;

        if (verbosity > 0)
        {
          printf ("constraint %d (type = %s): ", M - 1,
                  (*pTess).DomFaceType[tmp[i]]);
          fflush (stdout);
          ut_array_1d_fprintf (stdout, constraint[M - 1], 4, "%f");
        }
      }
    }

    if (verbosity > 0)
    {
      printf ("\n\n");
      printf ("newver = %d\n", newver);
      printf ("M = %d: ", M);
      ut_array_1d_int_fprintf (stdout, domainface, M, "%d");
      ut_array_2d_fprintf (stdout, constraint, M, 4, "%9.6f");
      printf ("\n\n");
    }

    A = ut_alloc_2d (3 + M, 3 + M);
    B = ut_alloc_1d (3 + M);
    X = ut_alloc_1d (3 + M);

    // Filling up the 3 x 3 first values of A
    double *n = NULL;
    double d;
    for (i = 0; i < N; i++)
    {
      n = (*pTess).FaceEq[face[i]] + 1;
      for (j = 0; j < 3; j++)
        for (k = 0; k < 3; k++)
          A[j][k] += n[j] * n[k];
    }

    // Filling up the 3 first values of B
    for (i = 0; i < N; i++)
    {
      n = (*pTess).FaceEq[face[i]] + 1;
      d = (*pTess).FaceEq[face[i]][0];
      for (j = 0; j < 3; j++)
        B[j] += d * n[j];
    }

    // Filling up the 3 x M (x 2) constraint values of A and
    // the M constraint values of B.
    double *nprime = NULL;
    double dprime;
    int id = 0;
    for (i = 0; i < M; i++)
    {
      nprime = constraint[i] + 1;
      dprime = constraint[i][0];
      for (j = 0; j < 3; j++)
      {
        A[id + 3][j] = nprime[j];
        A[j][id + 3] = nprime[j];
        B[id + 3] = dprime;
      }
      id++;
    }

    status = ut_linalg_solve_LU (A, B, M + 3, X);

    dist = ut_space_dist ((*pTess).VerCoo[newver], X);

    ut_array_1d_memcpy (X, 3, (*pTess).VerCoo[newver]);

    ut_free_2d (&A, 3 + M);
    ut_free_1d (&B);
    ut_free_1d (&X);
  }
  while (curved && iter < 1000 && dist > 1e-9);

  ut_free_1d (&eq);
  ut_free_1d_int (&face);
  ut_free_2d (&constraint, domfaceqty);
  ut_free_1d_int (&domainface);
  ut_free_1d_int (&tmp);

  if (verbosity)
  {
    printf ("new coo = ");
    ut_array_1d_fprintf (stdout, (*pTess).VerCoo[newver], 3, "%f");
  }

  return status;
}
