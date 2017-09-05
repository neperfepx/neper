/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
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

  ut_free_1d_int (NewVerFace);
  ut_free_1d_int (DelVerFace);

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

  ut_free_1d_int (NewVerFace);
  ut_free_1d_int (DelVerFace);

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
  int *array;			/* will be the VerEdgeNb  of newver ver */
  int qty;			/* will be the VerEdgeQty of newver ver */

  array =
    ut_array_1d_int_cat ((*pTess).VerEdgeNb[newver],
			 (*pTess).VerEdgeQty[newver],
			 (*pTess).VerEdgeNb[delver],
			 (*pTess).VerEdgeQty[delver]);

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
  qty -= ut_array_1d_int_deletencompress (array, qty, edge, 2);

  (*pTess).VerEdgeQty[newver] = qty;
  (*pTess).VerEdgeNb[newver] =
    ut_realloc_1d_int ((*pTess).VerEdgeNb[newver], qty);
  ut_array_1d_int_memcpy ((*pTess).VerEdgeNb[newver], qty, array);

  if (verbosity >= 3)
  {
    ut_print_lineheader (-1);
    printf ("    ver %d: ", newver);
    for (i = 0; i <= (*pTess).VerEdgeQty[newver] - 1; i++)
      printf ("%d ", (*pTess).VerEdgeNb[newver][i]);
    printf ("\n");
  }

  ut_free_1d_int (array);

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
      ut_error_reportbug ();

    // delver is the domain ver: copying;
    // otherwise newver is the domain ver: no need to copy
    if ((*pTess).VerDom[delver][0] == 0)
      ut_array_1d_int_memcpy ((*pTess).VerDom[newver], 2,
			      (*pTess).VerDom[delver]);
  }

  // one vertex in on a domain edge
  else if ((*pTess).VerDom[delver][0] == 1 || (*pTess).VerDom[newver][0] == 1)
  {
    // test: both vertices are on a domain edge, but not on the same
    // becoming a domain vertex
    if (((*pTess).VerDom[delver][0] == (*pTess).VerDom[newver][0])
	&& ((*pTess).VerDom[delver][1] != (*pTess).VerDom[newver][1]))
      ut_error_reportbug ();	// the edge should not have been marked for deletion in NextEdgeToDel

    // delver is on the domain edge: copying;
    // otherwise newver is on the domain edge: no need to copy
    else if ((*pTess).VerDom[delver][0] == 1)
      ut_array_1d_int_memcpy ((*pTess).VerDom[newver], 2,
			      (*pTess).VerDom[delver]);
  }

  // one vertex in on a domain face
  else if ((*pTess).VerDom[delver][0] == 2 || (*pTess).VerDom[newver][0] == 2)
  {
    // if delver: copying, otherwise: no need to copy
    if ((*pTess).VerDom[delver][0] == 2)
      ut_array_1d_int_memcpy ((*pTess).VerDom[newver], 2,
			      (*pTess).VerDom[delver]);
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
UpdateVerCooBary (struct TESS *pTess, int delver, int newver)
{
  int i, domedge, domedgeqty, domver1, domver2, eqqty;
  int *domedges = NULL;
  double **ptcoo = ut_alloc_2d (2, 3);
  double **eqs = NULL;
  int *vers = ut_alloc_1d_int (2);

  vers[0] = delver;
  vers[1] = newver;

  neut_tess_vers_alldomedges (*pTess, vers, 2, &domedges, &domedgeqty);

  eqqty = domedgeqty;

  if (domedgeqty > 0)
  {
    eqs = ut_alloc_2d (eqqty, 3);
    for (i = 0; i < domedgeqty; i++)
    {
      domedge = domedges[i];
      domver1 = (*pTess).DomEdgeVerNb[domedge][0];
      domver2 = (*pTess).DomEdgeVerNb[domedge][1];
      ut_space_points_line ((*pTess).DomVerCoo[domver1],
			    (*pTess).DomVerCoo[domver2], eqs[i]);
    }
  }

  ut_array_1d_memcpy (ptcoo[0], 3, (*pTess).VerCoo[delver]);
  ut_array_1d_memcpy (ptcoo[1], 3, (*pTess).VerCoo[newver]);

  ut_space_bary2d_constrained (ptcoo, NULL, 2, eqs, eqqty,
			       (*pTess).VerCoo[newver]);

  ut_free_2d (ptcoo, 2);
  ut_free_2d (eqs, eqqty);

  return 0;
}

// See documentation file `edgedel.tarpdf' for details on the algorithm.
int
UpdateVerCooMiniFF (struct TESS *pTess, int newver, int verbosity)
{
  int i, j, k;
  double **A = NULL;
  double *B = NULL;
  double *X = NULL;
  int faceqty;
  int *face = NULL;
  int N;			// nb of faces
  int M;			// nb of constrained faces

  verbosity = 0;

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

  int qty;
  int *tmp = NULL;
  neut_tess_ver_domfaces (*pTess, newver, &tmp, &qty);

  if (verbosity > 0)
  {
    printf ("neut_tess_ver_domfaces returns qty = %d and ids = ", qty);
    ut_array_1d_int_fprintf (stdout, tmp, qty, "%d");
  }

  M = qty;

  double **constraint = ut_alloc_2d (M, 4);
  // for (i = 0; i < M; i++)
  // ut_array_1d_memcpy (constraint[i], 4, (*pTess).DomFaceEq[domainface[i]]);
  for (i = 0; i < M; i++)
    ut_array_1d_memcpy (constraint[i], 4, (*pTess).DomFaceEq[tmp[i]]);

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
  for (i = 0; i < M; i++)
  {
    nprime = constraint[i] + 1;
    dprime = constraint[i][0];
    for (j = 0; j < 3; j++)
    {
      A[i + 3][j] = nprime[j];
      A[j][i + 3] = nprime[j];
      B[i + 3] = dprime;
    }
  }

  int status = ut_linalg_solve_LU (A, B, M + 3, X);

  ut_array_1d_memcpy ((*pTess).VerCoo[newver], 3, X);

  ut_free_2d (A, 3 + M);
  ut_free_1d (B);
  ut_free_1d (X);
  ut_free_1d_int (face);
  ut_free_2d (constraint, M);
  ut_free_1d_int (domainface);
  ut_free_1d_int (tmp);

  return status;
}
