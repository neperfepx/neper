/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_extrude_.h"

void
neut_tess_extrude_gen (struct TESS Tess, struct TESS *pT)
{
  neut_tess_tess_gen (Tess, pT);
  (*pT).Dim = 3;

  return;
}

void
neut_tess_extrude_cell (struct TESS Tess, double d, struct TESS *pT)
{
  int i;

  neut_tess_tess_cell (Tess, pT);
  for (i = 1; i <= (*pT).CellQty; i++)
    (*pT).SeedCoo[i][2] = .5 * d;

  return;
}

void
neut_tess_extrude_ver (struct TESS Tess, double d, struct TESS *pT)
{
  int i, ver;

  neut_tess_tess_ver (Tess, pT);
  for (i = 1; i <= Tess.VerQty; i++)
  {
    ver = neut_tess_addver (pT, (*pT).VerCoo[i]);
    (*pT).VerCoo[ver][2] = d;
    (*pT).VerState[ver] = (*pT).VerState[i];
  }

  return;
}

void
neut_tess_extrude_edge (struct TESS Tess, struct TESS *pT)
{
  int i, edge;

  neut_tess_tess_edge (Tess, pT);
  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    edge = neut_tess_addedge (pT, (*pT).EdgeVerNb[i][0] + Tess.VerQty,
			      (*pT).EdgeVerNb[i][1] + Tess.VerQty);

    (*pT).EdgeLength[edge] = (*pT).EdgeLength[i];
    (*pT).EdgeState[edge] = (*pT).EdgeState[i];
    (*pT).EdgeDel[edge] = (*pT).EdgeDel[i];
  }

  for (i = 1; i <= Tess.VerQty; i++)
  {
    edge = neut_tess_addedge (pT, i, Tess.VerQty + i);
    (*pT).EdgeLength[edge] = (*pT).EdgeLength[i];
  }

  return;
}

void
neut_tess_extrude_face (struct TESS Tess, double d, struct TESS *pT)
{
  int i, j, face;

  neut_tess_tess_face (Tess, pT);
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    face = neut_tess_addface_alloc (pT);
    ut_array_1d_memcpy ((*pT).FaceEq[face], 4, (*pT).FaceEq[i]);
    (*pT).FaceEq[face][0] += d;

    for (j = 1; j <= (*pT).FaceVerQty[i]; j++)
    {
      neut_tess_face_addedge (pT, face,
			      (*pT).FaceEdgeNb[i][j] + Tess.EdgeQty,
			      (*pT).FaceEdgeOri[i][j]);
    }

    (*pT).FaceState[face] = (*pT).FaceState[i];
    (*pT).FacePt[face] = (*pT).FacePt[i];
    ut_array_1d_memcpy ((*pT).FacePtCoo[face], 3, (*pT).FacePtCoo[i]);
  }

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    face = neut_tess_addface_alloc (pT);
    neut_tess_face_addedge (pT, face, i, 1);
    neut_tess_face_addedge (pT, face,
			    Tess.EdgeQty * 2 + (*pT).EdgeVerNb[i][0], -1);
    neut_tess_face_addedge (pT, face, Tess.EdgeQty + i, -1);
    neut_tess_face_addedge (pT, face,
			    Tess.EdgeQty * 2 + (*pT).EdgeVerNb[i][1], 1);

    ut_space_points_plane ((*pT).VerCoo[(*pT).FaceVerNb[face][1]],
			   (*pT).VerCoo[(*pT).FaceVerNb[face][2]],
			   (*pT).VerCoo[(*pT).FaceVerNb[face][3]],
			   (*pT).FaceEq[face]);
  }

  return;
}

void
neut_tess_extrude_poly (struct TESS Tess, struct TESS *pT)
{
  int i, j, poly, ef, efori;

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    poly = neut_tess_addpoly_alloc (pT);

    neut_tess_poly_addface (pT, poly, i, -ut_num_sgn ((*pT).FaceEq[i][3]));
    neut_tess_poly_addface (pT, poly, Tess.FaceQty + i,
			    -ut_num_sgn ((*pT).FaceEq[Tess.FaceQty + i][3]));
    for (j = 1; j <= (*pT).FaceVerQty[i]; j++)
    {
      ef = Tess.FaceQty * 2 + Tess.FaceEdgeNb[i][j];
      efori = Tess.FaceEdgeOri[i][j];
      neut_tess_poly_addface (pT, poly, ef, efori);
    }
  }

  return;
}

void
neut_tess_extrude_domain (struct TESS Tess, double d, struct TESS *pT)
{
  int i, j, edge, face, domver, domedge, domface, dver1, dver2;

  neut_tess_tess_domain (Tess, pT);

  // domver
  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    domver = neut_tess_adddomver (pT, (*pT).DomVerCoo[i]);
    (*pT).DomVerCoo[domver][2] = d;

    ut_string_string ((*pT).DomVerLabel[i], &((*pT).DomVerLabel[domver]));

    (*pT).DomVerLabel[i]
      = ut_realloc_1d_char ((*pT).DomVerLabel[i],
			    strlen ((*pT).DomVerLabel[i]) + 3);
    (*pT).DomVerLabel[i] = strcat ((*pT).DomVerLabel[i], "z0");
    (*pT).DomVerLabel[domver]
      = ut_realloc_1d_char ((*pT).DomVerLabel[domver],
			    strlen ((*pT).DomVerLabel[domver]) + 3);
    (*pT).DomVerLabel[domver] = strcat ((*pT).DomVerLabel[domver], "z1");
  }

  // domedge
  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    domedge =
      neut_tess_adddomedge (pT, (*pT).DomEdgeVerNb[i][0] + Tess.DomVerQty,
			    (*pT).DomEdgeVerNb[i][1] + Tess.DomVerQty);

    ut_string_string ((*pT).DomEdgeLabel[i], &((*pT).DomEdgeLabel[domedge]));

    (*pT).DomEdgeLabel[i]
      = ut_realloc_1d_char ((*pT).DomEdgeLabel[i],
			    strlen ((*pT).DomEdgeLabel[i]) + 3);
    (*pT).DomEdgeLabel[i] = strcat ((*pT).DomEdgeLabel[i], "z0");
    (*pT).DomEdgeLabel[domedge]
      = ut_realloc_1d_char ((*pT).DomEdgeLabel[domedge],
			    strlen ((*pT).DomEdgeLabel[domedge]) + 3);
    (*pT).DomEdgeLabel[domedge] = strcat ((*pT).DomEdgeLabel[domedge], "z1");
  }

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    domedge = neut_tess_adddomedge (pT, i, Tess.DomVerQty + i);
    ut_string_string (Tess.DomVerLabel[i], &((*pT).DomEdgeLabel[domedge]));
  }

  (*pT).DomVerEdgeQty = ut_alloc_1d_int ((*pT).DomVerQty + 1);
  (*pT).DomVerEdgeNb = ut_alloc_2d_int ((*pT).DomVerQty + 1, 3);
  ut_array_2d_int_zero ((*pT).DomVerEdgeNb + 1, (*pT).DomVerQty, 3);
  for (i = 1; i <= (*pT).DomEdgeQty; i++)
    for (j = 0; j < 2; j++)
    {
      domver = (*pT).DomEdgeVerNb[i][j];

      (*pT).DomVerEdgeQty[domver]++;

      if ((*pT).DomVerEdgeNb[domver][0] == 0)
	(*pT).DomVerEdgeNb[domver][0] = i;
      else if ((*pT).DomVerEdgeNb[domver][1] == 0)
	(*pT).DomVerEdgeNb[domver][1] = i;
      else if ((*pT).DomVerEdgeNb[domver][2] == 0)
	(*pT).DomVerEdgeNb[domver][2] = i;
      else
	ut_error_reportbug ();
    }

  domface = neut_tess_adddomface_alloc (pT);
  ut_string_string ("z0", &((*pT).DomFaceLabel[domface]));
  (*pT).DomFaceEq[domface][3] = -1;
  (*pT).DomFaceVerQty[domface] = Tess.DomVerQty;
  (*pT).DomFaceVerNb[domface]
    = ut_alloc_1d_int ((*pT).DomFaceVerQty[domface] + 1);
  ut_array_1d_int_set_id ((*pT).DomFaceVerNb[domface], Tess.DomVerQty + 1);
  (*pT).DomFaceEdgeNb[domface]
    = ut_alloc_1d_int ((*pT).DomFaceVerQty[domface] + 1);
  ut_array_1d_int_set_id ((*pT).DomFaceEdgeNb[domface], Tess.DomVerQty + 1);

  domface = neut_tess_adddomface_alloc (pT);
  ut_string_string ("z1", &((*pT).DomFaceLabel[domface]));
  (*pT).DomFaceEq[domface][0] = -d;
  (*pT).DomFaceEq[domface][3] = -1;
  (*pT).DomFaceVerQty[domface] = Tess.DomVerQty;
  (*pT).DomFaceVerNb[domface]
    = ut_alloc_1d_int ((*pT).DomFaceVerQty[domface] + 1);
  ut_array_1d_int_set_id ((*pT).DomFaceVerNb[domface], Tess.DomVerQty + 1);
  ut_array_1d_int_addval ((*pT).DomFaceVerNb[domface] + 1,
			  (*pT).DomFaceVerQty[domface], Tess.DomVerQty,
			  (*pT).DomFaceVerNb[domface] + 1);
  (*pT).DomFaceEdgeNb[domface]
    = ut_alloc_1d_int ((*pT).DomFaceVerQty[domface] + 1);
  ut_array_1d_int_set_id ((*pT).DomFaceEdgeNb[domface], Tess.DomVerQty + 1);
  ut_array_1d_int_addval ((*pT).DomFaceEdgeNb[domface] + 1,
			  (*pT).DomFaceVerQty[domface], Tess.DomVerQty,
			  (*pT).DomFaceEdgeNb[domface] + 1);

  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    domface = neut_tess_adddomface_alloc (pT);
    ut_string_string (Tess.DomEdgeLabel[i], &((*pT).DomFaceLabel[domface]));
    neut_tess_domedge_eq (Tess, i, (*pT).DomFaceEq[domface]);

    (*pT).DomFaceVerQty[domface] = 4;
    (*pT).DomFaceVerNb[domface]
      = ut_alloc_1d_int ((*pT).DomFaceVerQty[domface] + 1);
    (*pT).DomFaceEdgeNb[domface]
      = ut_alloc_1d_int ((*pT).DomFaceVerQty[domface] + 1);

    dver1 = Tess.DomEdgeVerNb[i][0];
    dver2 = Tess.DomEdgeVerNb[i][1];

    (*pT).DomFaceVerNb[domface][1] = dver1;
    (*pT).DomFaceVerNb[domface][2] = dver2;
    (*pT).DomFaceVerNb[domface][3] = Tess.DomVerQty + dver2;
    (*pT).DomFaceVerNb[domface][4] = Tess.DomVerQty + dver1;

    (*pT).DomFaceEdgeNb[domface][1] = i;
    (*pT).DomFaceEdgeNb[domface][3] = 2 * Tess.DomEdgeQty + dver2;
    (*pT).DomFaceEdgeNb[domface][2] = i + Tess.DomEdgeQty;
    (*pT).DomFaceEdgeNb[domface][4] = 2 * Tess.DomEdgeQty + dver1;
  }

  ut_array_2d_int_zero ((*pT).DomEdgeFaceNb + 1, (*pT).DomEdgeQty, 2);
  for (i = 1; i <= (*pT).DomFaceQty; i++)
    for (j = 1; j <= (*pT).DomFaceVerQty[i]; j++)
    {
      domedge = (*pT).DomFaceEdgeNb[i][j];

      if ((*pT).DomEdgeFaceNb[domedge][0] == 0)
	(*pT).DomEdgeFaceNb[domedge][0] = i;
      else if ((*pT).DomEdgeFaceNb[domedge][1] == 0)
	(*pT).DomEdgeFaceNb[domedge][1] = i;
      else
	ut_error_reportbug ();
    }

  for (i = 1; i <= Tess.DomEdgeQty; i++)
    for (j = 1; j <= Tess.DomTessEdgeQty[i]; j++)
    {
      edge = Tess.DomTessEdgeNb[i][j];
      face = 2 * Tess.FaceQty + edge;
      (*pT).FacePoly[face][1] = -i - 2;
    }

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    (*pT).FacePoly[i][1] = -1;
    (*pT).FacePoly[Tess.FaceQty + i][1] = -2;
  }

  neut_tess_init_facedom_fromfacepoly (pT);
  neut_tess_init_edgedom_fromfacedom (pT);
  neut_tess_init_verdom_fromedgedom (pT);
  neut_tess_init_domtessface (pT);
  neut_tess_init_domtessedge (pT);
  neut_tess_init_domtessver (pT);

  return;
}
