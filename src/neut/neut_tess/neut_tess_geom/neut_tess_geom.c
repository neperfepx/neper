/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_geom_.h"

int
neut_tess_cell_size (struct TESS Tess, int cell, double *psize)
{
  if (Tess.Dim == 3)
  {
    neut_tess_poly_volume (Tess, cell, psize);
    if (Tess.PseudoDim == 2)
      (*psize) /= Tess.PseudoSize;
  }
  else if (Tess.Dim == 2)
    neut_tess_face_area (Tess, cell, psize);
  else
    ut_error_reportbug ();

  return 0;
}

int
neut_tess_edge_eq (struct TESS Tess, int edge, double *eq)
{
  if (edge < 1 || edge > Tess.EdgeQty || Tess.Dim != 2)
    ut_error_reportbug ();

  ut_space_points_line (Tess.VerCoo[Tess.EdgeVerNb[edge][0]],
			Tess.VerCoo[Tess.EdgeVerNb[edge][1]], eq);

  return 0;
}

int
neut_tess_edge_dir (struct TESS Tess, int edge, double *dir)
{
  if (edge < 1 || edge > Tess.EdgeQty)
    ut_error_reportbug ();

  ut_array_1d_sub (Tess.VerCoo[Tess.EdgeVerNb[edge][0]],
                   Tess.VerCoo[Tess.EdgeVerNb[edge][1]], 3, dir);

  ut_array_1d_normalize (dir, 3);

  return 0;
}

int
neut_tess_domedge_eq (struct TESS Tess, int domedge, double *eq)
{
  if (domedge < 1 || domedge > Tess.DomEdgeQty || Tess.Dim != 2)
    ut_error_reportbug ();

  ut_space_points_line (Tess.DomVerCoo[Tess.DomEdgeVerNb[domedge][0]],
			Tess.DomVerCoo[Tess.DomEdgeVerNb[domedge][1]], eq);

  return 0;
}

int
neut_tess_domedge_length (struct TESS Tess, int dedge, double *pval)
{
  (*pval) = ut_space_dist (Tess.DomVerCoo[Tess.DomEdgeVerNb[dedge][0]],
			   Tess.DomVerCoo[Tess.DomEdgeVerNb[dedge][1]]);

  return 0;
}

int
neut_tess_face_area (struct TESS Tess, int face, double *parea)
{
  int i, ver;
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;

  if (face < 1 || face > Tess.FaceQty)
    ut_error_reportbug ();

  if (Tess.FaceVerQty[face] == 0)
  {
    (*parea) = 0;
    return 1;
  }

  p0 = ut_alloc_1d (3);

  if (Tess.FaceState[face] == -1)
  {
    (*parea) = 0;
    return -1;
  }

  if (Tess.FaceState[face] > 0)
  {
    if (Tess.FacePt[face] == 0)
      ut_array_1d_memcpy (p0, 3, Tess.FacePtCoo[face]);
    else if (Tess.FacePt[face] > 0)
    {
      ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
      ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
    }
    else
      neut_tess_face_centre (Tess, face, p0);
  }
  else if (Tess.FaceState[face] == 0)
    neut_tess_face_centre (Tess, face, p0);

  (*parea) = 0;
  for (i = 1; i < Tess.FaceVerQty[face]; i++)
  {
    p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = Tess.VerCoo[Tess.FaceVerNb[face][Tess.FaceVerQty[face]]];
  p2 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (p0);

  return 0;
}

int
neut_tess_faces_area (struct TESS Tess, int *faces, int faceqty,
		      double *parea)
{
  int i;
  double facearea;

  (*parea) = 0;
  for (i = 0; i < faceqty; i++)
  {
    neut_tess_face_area (Tess, faces[i], &facearea);
    (*parea) += facearea;
  }

  return 0;
}

int
neut_tess_size (struct TESS Tess, double *psize)
{
  if (Tess.Dim == 3)
  {
    neut_tess_volume (Tess, psize);
    if (Tess.PseudoDim == 2)
      (*psize) /= Tess.PseudoSize;
  }
  else if (Tess.Dim == 2)
    neut_tess_area (Tess, psize);
  else
    abort ();

  return 0;
}

int
neut_tess_diameq (struct TESS Tess, double *pval)
{
  neut_tess_size (Tess, pval);

  if (Tess.Dim == 3)
    (*pval) = sqrt ((4 / M_PI) * (*pval));
  else if (Tess.Dim == 2)
    (*pval) = pow ((6 / M_PI) * (*pval), 0.3333333333333333333333333);

  return 0;
}

void
neut_tess_cellavdiameq (struct TESS Tess, int CellQty, double *pdiameq)
{
  double size;

  neut_tess_size (Tess, &size);
  size /= CellQty;

  if (Tess.Dim == 3 && Tess.PseudoDim == -1)
    *pdiameq = pow ((6 / M_PI) * size, 1 / 3.);
  else if (Tess.Dim == 2 || (Tess.Dim == 3 && Tess.PseudoDim == 2))
    *pdiameq = sqrt ((4 / M_PI) * size);
  else
    abort ();

  return;
}

void
neut_tess_cellavdiameq_cellqty (struct TESS Tess, double avdiameq, int *pCellQty, double *pfact)
{
  double tesssize, size;

  neut_tess_size (Tess, &tesssize);

  if (Tess.Dim == 3 && Tess.PseudoDim == -1)
    size = pow (avdiameq, 3) * (M_PI / 6);
  else if (Tess.Dim == 2 || (Tess.Dim == 3 && Tess.PseudoDim == 2))
    size = pow (avdiameq, 2) * (M_PI / 4);
  else
    abort ();

  (*pCellQty) = ut_num_d2ri (tesssize / size);
  if (pfact)
    (*pfact) = (tesssize / size) / (*pCellQty);

  return;
}

void
neut_tess_cellavsize_cellqty (struct TESS Tess, double avsize, int *pCellQty, double *pfact)
{
  double tesssize;

  neut_tess_size (Tess, &tesssize);

  (*pCellQty) = ut_num_d2ri (tesssize / avsize);
  if (pfact)
    (*pfact) = (tesssize / avsize) / (*pCellQty);

  return;
}

void
neut_tess_cellavradeq (struct TESS Tess, int CellQty, double *pradeq)
{
  neut_tess_cellavdiameq (Tess, CellQty, pradeq);
  (*pradeq) *= 0.5;

  return;
}

void
neut_tess_cellavsize (struct TESS Tess, int CellQty, double *psize)
{
  neut_tess_size (Tess, psize);
  *psize /= CellQty;

  return;
}

/*
int
neut_tess_face_area2 (struct TESS *Tess, int face, double *parea)
{
  int i, ver;
  double *p0 = NULL;
  double *p1 = NULL;
  double *p2 = NULL;

  if (face < 1 || face > (*Tess).FaceQty)
    ut_error_reportbug ();

  p0 = ut_alloc_1d (3);

  if ((*Tess).FaceState[face] == -1)
  {
    (*parea) = 0;
    return -1;
  }

  if ((*Tess).FaceState[face] > 0)
  {
    if ((*Tess).FacePt[face] == 0)
      ut_array_1d_memcpy (p0, 3, (*Tess).FacePtCoo[face]);
    else if ((*Tess).FacePt[face] > 0)
    {
      ver = (*Tess).FaceVerNb[face][(*Tess).FacePt[face]];
      ut_array_1d_memcpy (p0, 3, (*Tess).VerCoo[ver]);
    }
    else
      neut_tess_face_centre2 (Tess, face, p0);
  }
  else if ((*Tess).FaceState[face] == 0)
  {
    ver = (*Tess).FaceVerNb[face][1];
    ut_array_1d_memcpy (p0, 3, (*Tess).VerCoo[ver]);
  }

  (*parea) = 0;
  for (i = 1; i < (*Tess).FaceVerQty[face]; i++)
  {
    p1 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][i]];
    p2 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][i + 1]];
    (*parea) += ut_space_triangle_area (p0, p1, p2);
  }

  p1 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][(*Tess).FaceVerQty[face]]];
  p2 = (*Tess).VerCoo[(*Tess).FaceVerNb[face][1]];
  (*parea) += ut_space_triangle_area (p0, p1, p2);

  ut_free_1d (p0);

  return 0;
}
*/

int
neut_tess_volume (struct TESS Tess, double *pvol)
{
  int i;
  double tmp;

  (*pvol) = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    neut_tess_poly_volume (Tess, i, &tmp);
    (*pvol) += tmp;
  }

  return 0;
}

int
neut_tess_poly_volume (struct TESS Tess, int poly, double *pvol)
{
  int i, ver, f, face;
  double area, h;
  double *p1 = NULL;
  double *p2 = NULL;
  double *p0 = NULL;
  double *proj = NULL;
  double *eq = NULL;
  double *centre = NULL;

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (poly < 1 || poly > Tess.PolyQty || Tess.PolyFaceQty[poly] == 0)
  {
    (*pvol) = 0;
    return -1;
  }

  if (Tess.PolyFaceQty[poly] == 0)
  {
    (*pvol) = 0;
    return 1;
  }

  p0 = ut_alloc_1d (3);
  proj = ut_alloc_1d (3);
  eq = ut_alloc_1d (4);
  centre = ut_alloc_1d (3);

  neut_tess_poly_vercentroid (Tess, poly, centre);

  (*pvol) = 0;
  for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
  {
    face = Tess.PolyFaceNb[poly][f];

    if (Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
	ut_array_1d_memcpy (p0, 3, Tess.FacePtCoo[face]);
      else if (Tess.FacePt[face] == -1)
      {
	ver = Tess.FaceVerNb[face][1];
	neut_tess_face_centre (Tess, face, p0);
      }
      else
      {
	ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
	ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
      }
    }
    else
    {
      ver = Tess.FaceVerNb[face][1];
      ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
    }

    for (i = 1; i < Tess.FaceVerQty[face]; i++)
    {
      p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
      p2 = Tess.VerCoo[Tess.FaceVerNb[face][i + 1]];
      area = ut_space_triangle_area (p0, p1, p2);
      if (!isnan (area))
      {
	ut_array_1d_memcpy (proj, 3, centre);
	ut_space_points_plane (p0, p1, p2, eq);
	ut_space_projpoint_alongonto (proj, eq + 1, eq);
	h = ut_space_dist (centre, proj);
	if (!isnan (h))
	  (*pvol) += area * h * 0.3333333333333333333333;
      }
    }

    p1 = Tess.VerCoo[Tess.FaceVerNb[face][Tess.FaceVerQty[face]]];
    p2 = Tess.VerCoo[Tess.FaceVerNb[face][1]];
    area = ut_space_triangle_area (p0, p1, p2);
    if (!isnan (area))
    {
      ut_array_1d_memcpy (proj, 3, centre);
      ut_space_points_plane (p0, p1, p2, eq);
      ut_space_projpoint_alongonto (proj, eq + 1, eq);
      h = ut_space_dist (centre, proj);
      if (!isnan (h))
	(*pvol) += area * h * 0.3333333333333333333333;
    }
  }

  // don't free p1, p2 (shortcuts)
  ut_free_1d (proj);
  ut_free_1d (eq);
  ut_free_1d (p0);
  ut_free_1d (centre);

  return 0;
}

int
neut_tess_area (struct TESS Tess, double *parea)
{
  int i;
  double tmp;

  (*parea) = 0;
  for (i = 1; i <= Tess.FaceQty; i++)
  {
    neut_tess_face_area (Tess, i, &tmp);
    (*parea) += tmp;
  }

  return 0;
}

int
neut_tess_domface_area (struct TESS Tess, int domface, double *parea)
{
  int i;
  double tmp;

  if (domface < 0 || domface > Tess.DomFaceQty)
  {
    printf ("domface = %d < 0 || > Tess.DomFaceQty = %d\n", domface,
	    Tess.DomFaceQty);
    ut_error_reportbug ();
  }

  (*parea) = 0;
  for (i = 1; i <= Tess.DomTessFaceQty[domface]; i++)
  {
    neut_tess_face_area (Tess, Tess.DomTessFaceNb[domface][i], &tmp);
    (*parea) += tmp;
  }

  return 0;
}

int
neut_tess_domface_label_area (struct TESS Tess, char *domface, double *parea)
{
  int id;

  neut_tess_domface_label_id (Tess, domface, &id);
  neut_tess_domface_area (Tess, id, parea);

  return 0;
}

int
neut_tess_poly_vercentroid (struct TESS Tess, int poly, double *coo)
{
  int i, verqty;
  int *vers = NULL;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  ut_array_1d_zero (coo, 3);

  for (i = 0; i < verqty; i++)
    ut_array_1d_add (coo, Tess.VerCoo[vers[i]], 3, coo);
  ut_array_1d_scale (coo, 3, 1. / verqty);

  ut_free_1d_int (vers);

  return 0;
}

int
neut_tess_poly_centroid (struct TESS Tess, int poly, double *coo)
{
  int i, ver, f, face, status;
  double area;
  double *p0 = ut_alloc_1d (3);
  double *p1 = NULL;
  double *p2 = NULL;
  double vol, totvol;
  double *tmp = ut_alloc_1d (3);
  double *verbary = ut_alloc_1d (3);
  int *vers = NULL;
  int verqty;

  // Principle: sum the volumes of the pyramids based on the faces and
  // the poly centre. For the non-plane faces, we consider the
  // individual triangular parts (no approximation)

  if (poly < 1 || poly > Tess.PolyQty || Tess.PolyFaceQty[poly] == 0)
    return -1;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  if (verqty > 0)
  {
    for (i = 0; i < verqty; i++)
      ut_array_1d_add (verbary, Tess.VerCoo[vers[i]], 3, verbary);
    ut_array_1d_scale (verbary, 3, 1. / verqty);

    totvol = 0;
    ut_array_1d_set (coo, 3, 0);

    for (f = 1; f <= Tess.PolyFaceQty[poly]; f++)
    {
      face = Tess.PolyFaceNb[poly][f];

      if (Tess.FaceState[face] > 0)
      {
	if (Tess.FacePt[face] == 0)
	  ut_array_1d_memcpy (p0, 3, Tess.FacePtCoo[face]);
	else if (Tess.FacePt[face] == -1)
	{
	  ver = Tess.FaceVerNb[face][1];
	  neut_tess_face_centre (Tess, face, p0);
	}
	else
	{
	  ver = Tess.FaceVerNb[face][Tess.FacePt[face]];
	  ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
	}
      }
      else
      {
	ver = Tess.FaceVerNb[face][1];
	ut_array_1d_memcpy (p0, 3, Tess.VerCoo[ver]);
      }

      for (i = 1; i <= Tess.FaceVerQty[face]; i++)
      {
	p1 = Tess.VerCoo[Tess.FaceVerNb[face][i]];
	p2 = Tess.VerCoo[Tess.FaceVerNb[face]
			 [ut_num_rotpos (1, Tess.FaceVerQty[face], i, 1)]];

	area = ut_space_triangle_area (p0, p1, p2);

	if (area > 1e-20)
	{
	  ut_space_tet_centre (verbary, p0, p1, p2, tmp);
	  vol = ut_space_tet_volume (verbary, p0, p1, p2);
	  ut_array_1d_scale (tmp, 3, vol);
	  ut_array_1d_add (coo, tmp, 3, coo);
	  totvol += vol;
	}
      }
    }

    ut_array_1d_scale (coo, 3, 1. / totvol);
    status = 0;
  }
  else
  {
    ut_array_1d_zero (coo, 3);
    status = -1;
  }

  // don't free p1, p2 (shortcuts)
  ut_free_1d (p0);
  ut_free_1d (tmp);
  ut_free_1d (verbary);
  ut_free_1d_int (vers);

  return status;
}

int
neut_tess_cell_centroid (struct TESS Tess, int cell, double *coo)
{
  int status = 0;

  if (Tess.Dim == 3)
    status = neut_tess_poly_centroid (Tess, cell, coo);
  else if (Tess.Dim == 2)
    neut_tess_face_centre (Tess, cell, coo);
  else
    abort ();

  return status;
}

void
neut_tess_bbox (struct TESS Tess, double **bbox)
{
  int i, j;

  /* Searching the tessellation bounding box */
  bbox[0][0] = bbox[1][0] = bbox[2][0] = DBL_MAX;
  bbox[0][1] = bbox[1][1] = bbox[2][1] = -DBL_MAX;

  if (Tess.DomVerQty == 0)
    for (i = 1; i <= Tess.VerQty; i++)
      for (j = 0; j < 3; j++)
      {
	bbox[j][0] = ut_num_min (bbox[j][0], Tess.VerCoo[i][j]);
	bbox[j][1] = ut_num_max (bbox[j][1], Tess.VerCoo[i][j]);
      }
  else
    for (i = 1; i <= Tess.DomVerQty; i++)
      for (j = 0; j < 3; j++)
      {
	bbox[j][0] = ut_num_min (bbox[j][0], Tess.DomVerCoo[i][j]);
	bbox[j][1] = ut_num_max (bbox[j][1], Tess.DomVerCoo[i][j]);
      }

  return;
}

void
neut_tess_bboxsize (struct TESS Tess, double *bboxsize)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (Tess, bbox);
  for (i = 0; i < 3; i++)
    bboxsize[i] = bbox[i][1] - bbox[i][0];

  ut_free_2d (bbox, 3);

  return;
}

void
neut_tess_vers_bbox (struct TESS Tess, int *vers, int verqty, double **bbox)
{
  int i, j;

  for (i = 0; i < Tess.Dim; i++)
  {
    bbox[i][0] = DBL_MAX;
    bbox[i][1] = -DBL_MAX;
  }

  for (i = 0; i < verqty; i++)
    for (j = 0; j < Tess.Dim; j++)
    {
      bbox[j][0] = ut_num_min (bbox[j][0], Tess.VerCoo[vers[i]][j]);
      bbox[j][1] = ut_num_max (bbox[j][1], Tess.VerCoo[vers[i]][j]);
    }

  return;
}

int
neut_tess_cell_bbox (struct TESS Tess, int cell, double **bbox)
{
  if (neut_tess_cell_isvoid (Tess, cell))
  {
    ut_array_2d_zero (bbox, 3, 2);

    return -1;
  }

  else
  {
    int verqty, *vers = NULL;

    neut_tess_cell_vers (Tess, cell, &vers, &verqty);

    neut_tess_vers_bbox (Tess, vers, verqty, bbox);

    ut_free_1d_int (vers);

    return 0;
  }
}

void
neut_tess_poly_bbox (struct TESS Tess, int poly, double **bbox)
{
  int verqty, *vers = NULL;

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  neut_tess_vers_bbox (Tess, vers, verqty, bbox);

  ut_free_1d_int (vers);

  return;
}

void
neut_tess_face_bbox (struct TESS Tess, int face, double **bbox)
{
  int verqty, *vers = NULL;

  neut_tess_face_vers (Tess, face, &vers, &verqty);

  neut_tess_vers_bbox (Tess, vers, verqty, bbox);

  ut_free_1d_int (vers);

  return;
}

void
neut_tess_edge_bbox (struct TESS Tess, int edge, double **bbox)
{
  int verqty, *vers = NULL;

  neut_tess_edge_vers (Tess, edge, &vers, &verqty);

  neut_tess_vers_bbox (Tess, vers, verqty, bbox);

  ut_free_1d_int (vers);

  return;
}

void
neut_tess_face_centre_x (struct TESS Tess, int face, double *px)
{
  double *centre = ut_alloc_1d (3);

  neut_tess_face_centre (Tess, face, centre);
  (*px) = centre[0];

  ut_free_1d (centre);

  return;
}

void
neut_tess_face_centre_y (struct TESS Tess, int face, double *px)
{
  double *centre = ut_alloc_1d (3);

  neut_tess_face_centre (Tess, face, centre);
  (*px) = centre[1];

  ut_free_1d (centre);

  return;
}

void
neut_tess_face_centre_z (struct TESS Tess, int face, double *px)
{
  double *centre = ut_alloc_1d (3);

  neut_tess_face_centre (Tess, face, centre);
  (*px) = centre[2];

  ut_free_1d (centre);

  return;
}

void
neut_tess_face_centre (struct TESS Tess, int face, double *centre)
{
  int i, ver, ver1, ver2, pos;
  double *coo = NULL;
  double *tmp = NULL;
  double val, totval;

  if (Tess.FaceVerQty[face] == 0)
  {
    ut_array_1d_zero (centre, 3);

    return;
  }

  coo = ut_alloc_1d (3);
  tmp = ut_alloc_1d (3);

  pos = (Tess.Dim == 3 && Tess.FacePt[face] > 0) ? Tess.FacePt[face] : 1;
  ver = Tess.FaceVerNb[face][pos];
  ut_array_1d_memcpy (coo, 3, Tess.VerCoo[ver]);

  ut_array_1d_set (centre, 3, 0);
  totval = 0;

  if (Tess.FaceVerQty[face] > 0)
  {
    for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    {
      ver1 = Tess.FaceVerNb[face][i];
      ver2 =
	Tess.FaceVerNb[face][ut_num_rotpos (1, Tess.FaceVerQty[face], i, 1)];
      ut_space_triangle_centre (Tess.VerCoo[ver1], Tess.VerCoo[ver2], coo,
				tmp);
      val =
	ut_space_triangle_area (Tess.VerCoo[ver1], Tess.VerCoo[ver2], coo);

      ut_array_1d_scale (tmp, 3, val);
      ut_array_1d_add (centre, tmp, 3, centre);
      totval += val;
    }
    ut_array_1d_scale (centre, 3, 1 / totval);
  }

  ut_free_1d (coo);
  ut_free_1d (tmp);

  return;
}

int
neut_tess_edge_centre_x (struct TESS Tess, int edge, double *px)
{
  (*px) = (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][0]
	   + Tess.VerCoo[Tess.EdgeVerNb[edge][1]][0]) / 2.;

  return 0;
}

int
neut_tess_edge_centre_y (struct TESS Tess, int edge, double *py)
{
  (*py) = (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][1]
	   + Tess.VerCoo[Tess.EdgeVerNb[edge][1]][1]) / 2.;

  return 0;
}

int
neut_tess_edge_centre_z (struct TESS Tess, int edge, double *pz)
{
  (*pz) = (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][2]
	   + Tess.VerCoo[Tess.EdgeVerNb[edge][1]][2]) / 2.;

  return 0;
}

int
neut_tess_edge_centre (struct TESS Tess, int edge, double *pos)
{
  int i;

  for (i = 0; i < 3; i++)
    pos[i] = (Tess.VerCoo[Tess.EdgeVerNb[edge][0]][i]
	      + Tess.VerCoo[Tess.EdgeVerNb[edge][1]][i]) / 2.;

  return 0;
}

void
neut_tess_centre (struct TESS Tess, double *centre)
{
  int i;
  double totval, val;
  double *coo = ut_alloc_1d (3);

  ut_array_1d_zero (centre, 3);

  if (Tess.Dim == 3)
  {
    totval = 0;
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      neut_tess_poly_centroid (Tess, i, coo);
      neut_tess_poly_volume (Tess, i, &val);
      ut_array_1d_scale (coo, 3, val);
      ut_array_1d_add (centre, coo, 3, centre);
      totval += val;
    }
    ut_array_1d_scale (centre, 3, 1. / totval);
  }
  else if (Tess.Dim == 2)
  {
    totval = 0;
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      neut_tess_face_centre (Tess, i, coo);
      neut_tess_face_area (Tess, i, &val);
      ut_array_1d_scale (coo, 3, val);
      ut_array_1d_add (centre, coo, 3, centre);
      totval += val;
    }
    ut_array_1d_scale (centre, 3, 1. / totval);
  }

  ut_free_1d (coo);

  return;
}

void
neut_tess_bboxcentre (struct TESS Tess, double *centre)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tess_bbox (Tess, bbox);

  for (i = 0; i < Tess.Dim; i++)
    centre[i] = ut_array_1d_mean (bbox[i], 2);

  ut_free_2d (bbox, 3);

  return;
}

int
neut_tess_point_inpoly_std (struct TESS Tess, double *coo, int nb)
{
  int i, j, percooqty, status, status2;
  double **percoos = NULL;

  neut_tess_coo_percoos (Tess, coo, &percoos, &percooqty);

  status = 0;
  for (i = 0; i < percooqty; i++)
  {
    status2 = 1;

    for (j = 1; j <= Tess.PolyFaceQty[nb]; j++)
    {
      if (Tess.PolyFaceOri[nb][j] *
          ut_space_planeside_tol (Tess.FaceEq[Tess.PolyFaceNb[nb][j]], percoos[i] - 1, 1e-12) > 0)
      {
        status2 = 0;
        break;
      }
    }

    if (status2)
    {
      status = 1;
      break;
    }
  }

  ut_free_2d (percoos, percooqty);

  return status;
}

int
neut_tess_point_inpoly_reg (struct TESS Tess, double *coo, int nb)
{
  int i, elt, status, percooqty;
  double **percoos = NULL;
  struct MESH Mesh;
  struct NODES Nodes;

  neut_mesh_set_zero (&Mesh);
  neut_nodes_set_zero (&Nodes);

  neut_tess_poly_mesh (Tess, nb, &Nodes, &Mesh);

  neut_tess_coo_percoos (Tess, coo, &percoos, &percooqty);

  status = 0;
  for (i = 0; i < percooqty; i++)
    if (neut_mesh_elset_point_elt (Mesh, Nodes, 1, percoos[i], &elt) == 0)
    {
      status = 1;
      break;
    }

  ut_free_2d (percoos, percooqty);
  neut_mesh_free (&Mesh);
  neut_nodes_free (&Nodes);

  return status;
}

int
neut_tess_point_incell (struct TESS Tess, double *coo, int nb)
{
  if (Tess.Dim == 3)
    return neut_tess_point_inpoly (Tess, coo, nb);
  if (Tess.Dim == 2)
    return neut_tess_point_inface (Tess, coo, nb);
  else
    ut_error_reportbug ();

  return 0;
}

int
neut_tess_point_inpoly (struct TESS Tess, double *coo, int nb)
{
  if (Tess.Dim != 3)
    ut_error_reportbug ();

  if (Tess.PolyFaceQty[nb] == 0)
    return 0;
  else if (neut_tess_poly_std (Tess, nb) == 1)
    return neut_tess_point_inpoly_std (Tess, coo, nb);
  else
    return neut_tess_point_inpoly_reg (Tess, coo, nb);
}

int
neut_tess_point_inface (struct TESS Tess, double *coo, int face)
{
  int i, j, ver1, ver2, status;
  double *centre = ut_alloc_1d (3);
  int *per = ut_alloc_1d_int (3);
  double *coob = ut_alloc_1d (3);
  int *Periodic = ut_alloc_1d_int (3);
  double *PeriodicDist = ut_alloc_1d (3);

  if (Tess.Periodic)
    ut_array_1d_int_memcpy (Periodic, 3, Tess.Periodic);
  if (Tess.PeriodicDist)
    ut_array_1d_memcpy (PeriodicDist, 3, Tess.PeriodicDist);

  neut_tess_face_centre (Tess, face, centre);

  status = 0;
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    ver1 = Tess.FaceVerNb[face][i];
    ver2 =
      Tess.FaceVerNb[face][ut_num_rotpos (1, Tess.FaceVerQty[face], i, 1)];

    for (per[0] = -Periodic[0]; per[0] <= Periodic[0]; per[0]++)
    {
      for (per[1] = -Periodic[1]; per[1] <= Periodic[1]; per[1]++)
      {
	for (j = 0; j < Tess.Dim; j++)
	  coob[j] = coo[j] + per[j] * PeriodicDist[j];

	if (ut_space_triangle_point_in (centre, Tess.VerCoo[ver1],
					Tess.VerCoo[ver2], coob,
					1e-6, 0))
	{
	  status = 1;
	  break;
	}
      }

      if (status)
	break;
    }

    if (status)
      break;
  }

  ut_free_1d (centre);
  ut_free_1d (coob);
  ut_free_1d_int (per);
  ut_free_1d_int (Periodic);
  ut_free_1d (PeriodicDist);

  return status;
}

int
neut_tess_point_inedge (struct TESS Tess, double *coo, int edge)
{
  int ver1, ver2;

  ver1 = Tess.EdgeVerNb[edge][0];
  ver2 = Tess.EdgeVerNb[edge][1];

  return ut_space_segment_point_in (Tess.VerCoo[ver1],
				    Tess.VerCoo[ver2], coo);
}

/* rsel2sel sets sel from rsel */
void
rsel2sel (double rsel, int dim, double vol, int CellQty, double *psel)
{
  if (dim == 3)
    (*psel) =
      0.5 * rsel * 0.5 * pow (vol / CellQty, 0.3333333333333333333333333333);
  else if (dim == 2)
  {
    (*psel) = 0.5 * rsel * 0.5 * pow (vol / CellQty, 0.5);
    (*psel) *= 0.5;
  }

  return;
}

/* rcl2cl sets cl from rcl */
void
rcl2cl (double rcl, int dim, double size, int CellQty, char *elttype,
	double *pl)
{
  if (dim == 3)
  {
    if (elttype == NULL || !strcmp (elttype, "tri"))
      (*pl) =
	rcl * 0.5 * pow (size / CellQty, 0.3333333333333333333333333333);
    else if (!strcmp (elttype, "quad"))
    {
      (*pl) =
	rcl * 0.5 * pow (size / CellQty, 0.3333333333333333333333333333);
      (*pl) *= .43084877208099956915;
    }
  }
  else if (dim == 2)
  {
    if (elttype == NULL || !strcmp (elttype, "tri"))
    {
      (*pl) = rcl * 0.5 * pow (size / CellQty, 0.5);
      (*pl) *= 0.5;
    }
    else if (!strcmp (elttype, "quad"))
    {
      (*pl) =
	rcl * 0.5 * pow (size / CellQty, 0.3333333333333333333333333333);
      (*pl) *= .43084877208099956915;
    }
    else
      ut_error_reportbug ();
  }
  else if (dim == 1)
  {
    (*pl) = rcl * 0.2 * size / CellQty;
  }

  return;
}

void
neut_tess_face_normal_fromver (struct TESS Tess, int face, double *n)
{
  int i, cooqty;
  double **coo = NULL;
  double *bary = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);

  cooqty = Tess.FaceVerQty[face];
  coo = ut_alloc_2d (cooqty, 3);

  for (i = 0; i < Tess.FaceVerQty[face]; i++)
  {
    ut_array_1d_memcpy (coo[i], 3, Tess.VerCoo[Tess.FaceVerNb[face][i + 1]]);
    ut_array_1d_add (bary, coo[i], 3, bary);
  }
  ut_array_1d_scale (bary, 3, 1. / cooqty);

  ut_array_1d_zero (n, 3);
  for (i = 0; i < Tess.FaceVerQty[face]; i++)
  {
    ut_space_trianglenormal (bary, coo[i],
			     coo[ut_num_rotpos
				 (0, Tess.FaceVerQty[face] - 1, i, 1)], n0);
    ut_array_1d_add (n, n0, 3, n);
  }
  ut_array_1d_scale (n, 3, 1. / ut_array_1d_norm (n, 3));

  ut_free_2d (coo, cooqty);
  ut_free_1d (bary);
  ut_free_1d (n0);

  return;
}

int
neut_tess_length (struct TESS Tess, double *plength)
{
  int i;

  (*plength) = 0;
  for (i = 1; i <= Tess.EdgeQty; i++)
    (*plength) += Tess.EdgeLength[i];

  return 0;
}

void
neut_tess_face_dihangle (struct TESS Tess, int face, int id, double *pangle)
{
  int id_prev, id_next;
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);

  id_prev = ut_num_rotpos (1, Tess.FaceVerQty[face], id, -1);
  id_next = ut_num_rotpos (1, Tess.FaceVerQty[face], id, 1);

  ut_array_1d_sub (Tess.VerCoo[Tess.FaceVerNb[face][id_prev]],
		   Tess.VerCoo[Tess.FaceVerNb[face][id]], 3, v1);
  ut_array_1d_sub (Tess.VerCoo[Tess.FaceVerNb[face][id_next]],
		   Tess.VerCoo[Tess.FaceVerNb[face][id]], 3, v2);

  (*pangle) = ut_vector_angle (v1, v2);

  ut_free_1d (v1);
  ut_free_1d (v2);

  return;
}

void
neut_tess_face_dihangles (struct TESS Tess, int face,
			  double **pangle, int *pqty)
{
  int i;

  (*pangle) = ut_alloc_1d (Tess.FaceVerQty[face]);
  (*pqty) = Tess.FaceVerQty[face];

  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
    neut_tess_face_dihangle (Tess, face, i, &((*pangle)[i - 1]));

  return;
}

void
neut_tess_poly_dihangles (struct TESS Tess, int poly,
			  double **pangle, int *pqty)
{
  int *edges = NULL;
  int i, j, edgeqty, ori;
  int *faces = NULL;
  double **n = ut_alloc_2d (2, 3);

  neut_tess_poly_edges (Tess, poly, &edges, &edgeqty);

  (*pangle) = ut_alloc_1d (edgeqty);
  (*pqty) = edgeqty;

  for (i = 0; i < edgeqty; i++)
  {
    neut_tess_poly_edge_faces (Tess, poly, edges[i], &faces);

    for (j = 0; j < 2; j++)
    {
      ut_array_1d_memcpy (n[j], 3, Tess.FaceEq[faces[j]] + 1);
      neut_tess_poly_face_ori (Tess, poly, faces[j], &ori);
      ut_array_1d_scale (n[j], 3, ori);
    }

    (*pangle)[i] = 180 - ut_vector_angle (n[0], n[1]);

    ut_free_1d_int (faces);
    faces = NULL;
  }

  ut_free_1d_int (faces);
  ut_free_2d (n, 2);

  return;
}

void
neut_tess_face_radeq (struct TESS Tess, int face, double *pval)
{
  neut_tess_face_diameq (Tess, face, pval);
  (*pval) *= .5;

  return;
}

void
neut_tess_face_diameq (struct TESS Tess, int face, double *pval)
{
  neut_tess_face_area (Tess, face, pval);
  (*pval) = sqrt ((4 / M_PI) * (*pval));

  return;
}

void
neut_tess_poly_radeq (struct TESS Tess, int poly, double *pval)
{
  neut_tess_poly_diameq (Tess, poly, pval);
  (*pval) *= .5;

  return;
}

void
neut_tess_poly_diameq (struct TESS Tess, int poly, double *pval)
{
  neut_tess_poly_volume (Tess, poly, pval);
  (*pval) = pow ((6 / M_PI) * (*pval), 0.3333333333333333333333333);

  return;
}

void
neut_tess_cell_radeq (struct TESS Tess, int cell, double *pval)
{
  neut_tess_cell_diameq (Tess, cell, pval);
  (*pval) *= .5;

  return;
}

void
neut_tess_cell_diameq (struct TESS Tess, int cell, double *pval)
{
  if (Tess.Dim == 2)
    neut_tess_face_diameq (Tess, cell, pval);

  else if (Tess.Dim == 3)
    neut_tess_poly_diameq (Tess, cell, pval);

  else
    abort ();

  return;
}

void
neut_tess_face_perimeter (struct TESS Tess, int face, double *pval)
{
  int i, edge;

  (*pval) = 0;
  for (i = 1; i <= Tess.FaceVerQty[face]; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];
    (*pval) += Tess.EdgeLength[edge];
  }

  return;
}

void
neut_tess_poly_area (struct TESS Tess, int poly, double *pval)
{
  int i, face;
  double tmp;

  (*pval) = 0;
  for (i = 1; i <= Tess.PolyFaceQty[poly]; i++)
  {
    face = Tess.PolyFaceNb[poly][i];
    neut_tess_face_area (Tess, face, &tmp);
    (*pval) += tmp;
  }

  return;
}

int
neut_tess_face_circularity (struct TESS Tess, int face, double *pval)
{
  double diameq, tmp;

  if (Tess.FaceVerQty[face] == 0)
  {
    (*pval) = 0;
    return -1;
  }

  neut_tess_face_diameq (Tess, face, &diameq);
  neut_tess_face_perimeter (Tess, face, &tmp);
  (*pval) = M_PI * diameq / tmp;

  return 0;
}

int
neut_tess_poly_sphericity (struct TESS Tess, int poly, double *pval)
{
  double diameq, tmp;

  if (Tess.PolyFaceQty[poly] == 0)
  {
    (*pval) = 0;
    return -1;
  }

  neut_tess_poly_diameq (Tess, poly, &diameq);
  neut_tess_poly_area (Tess, poly, &tmp);
  (*pval) = M_PI * diameq * diameq / tmp;

  return 0;
}

int
neut_tess_cell_sphericity (struct TESS Tess, int cell, double *pval)
{
  int status;

  if (Tess.Dim == 2)
    status = neut_tess_face_circularity (Tess, cell, pval);
  else if (Tess.Dim == 3)
    status = neut_tess_poly_sphericity (Tess, cell, pval);
  else
    abort ();

  return status;
}

int
neut_tess_cell_planeside (struct TESS Tess, int cell, double *plane,
			  int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_planeside (plane, Tess.VerCoo[vers[i]] - 1);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (vers);
  ut_free_1d_int (side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_sphereside (struct TESS Tess, int cell, double *C, double rad,
			   int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_sphereside (C, rad, Tess.VerCoo[vers[i]]);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (vers);
  ut_free_1d_int (side);

  return 0;
}

int
neut_tess_cell_primside (struct TESS Tess, int cell, struct PRIM Prim, int *pside)
{
  if (!strcmp (Prim.Type, "hspace"))
    return -neut_tess_cell_planeside (Tess, cell, Prim.Eq, pside);
  else if (!strcmp (Prim.Type, "hspacei"))
    return neut_tess_cell_planeside (Tess, cell, Prim.Eq, pside);
  else if (!strcmp (Prim.Type, "sphere"))
    return neut_tess_cell_sphereside (Tess, cell, Prim.Base, Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "spherei"))
    return -neut_tess_cell_sphereside (Tess, cell, Prim.Base, Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "cylinder"))
    return neut_tess_cell_cylside (Tess, cell, Prim.Base, Prim.Dir, Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "cylinderi"))
    return -neut_tess_cell_cylside (Tess, cell, Prim.Base, Prim.Dir, Prim.Rad[0], pside);
  else if (!strcmp (Prim.Type, "ecylinder"))
    return neut_tess_cell_cyl2side (Tess, cell, Prim.Base, Prim.Dir,
                                    Prim.Dir + 3, Prim.Dir + 6, Prim.Rad[0], Prim.Rad[1], pside);
  else if (!strcmp (Prim.Type, "ecylinderi"))
    return -neut_tess_cell_cyl2side (Tess, cell, Prim.Base, Prim.Dir,
                                     Prim.Dir + 3, Prim.Dir + 6, Prim.Rad[0], Prim.Rad[1], pside);
  else if (!strcmp (Prim.Type, "torus"))
    return neut_tess_cell_torusside (Tess, cell, Prim.Base, Prim.Dir, Prim.Rad[0], Prim.Rad[1], pside);
  else if (!strcmp (Prim.Type, "torusi"))
    return -neut_tess_cell_torusside (Tess, cell, Prim.Base, Prim.Dir, Prim.Rad[0], Prim.Rad[1], pside);
  else
    abort ();
}

void
neut_tess_face_convexhull (struct TESS Tess, int face,
                           struct NODES *pN, struct MESH *pM)
{
  int i, j, verqty, *vers = NULL;
  double eps = 1e-9;
  double **vercoos = NULL;
  double distmin, dist;
  int distminid;
  int *pt = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  srand48 (1);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  neut_tess_face_vers (Tess, face, &vers, &verqty);

  vercoos = ut_alloc_2d (verqty, 3);
  for (i = 0; i < verqty; i++)
  {
    ut_array_1d_memcpy (vercoos[i], 3, Tess.VerCoo[vers[i]]);
    if (Tess.VerCoo[vers[i]][2] != 0)
    {
      printf ("FIXME (works only if the face is normal to z)\n");
      abort ();
    }
  }

  for (i = 0; i < verqty; i++)
    for (j = 0; j < Tess.Dim; j++)
      vercoos[i][j] += eps * drand48 ();

  net_pts_convexhull (vercoos, verqty, 2, pN, pM);

  for (i = 0; i < verqty; i++)
  {
    distmin = DBL_MAX;
    distminid = -1;
    for (j = 1; j <= (*pN).NodeQty; j++)
    {
      dist = ut_space_dist (vercoos[i], (*pN).NodeCoo[j]);
      if (dist < distmin)
      {
	distmin = dist;
	distminid = j;
      }
    }

    if (distmin < eps)
      ut_array_1d_memcpy ((*pN).NodeCoo[distminid], 3, Tess.VerCoo[vers[i]]);
  }

  ut_free_1d (coo);
  ut_free_1d_int (pt);
  ut_free_1d_int (vers);
  ut_free_2d (vercoos, verqty);

  return;
}

void
neut_tess_poly_convexhull (struct TESS Tess, int poly,
                           struct NODES *pN, struct MESH *pM)
{
  int i, j, verqty, *vers = NULL;
  double eps = 1e-9;
  double **vercoos = NULL;
  double distmin, dist;
  int distminid;
  int *pt = ut_alloc_1d_int (3);
  double *coo = ut_alloc_1d (3);
  srand48 (1);

  neut_nodes_set_zero (pN);
  neut_mesh_set_zero (pM);

  neut_tess_poly_vers (Tess, poly, &vers, &verqty);

  vercoos = ut_alloc_2d (verqty, 3);
  for (i = 0; i < verqty; i++)
    ut_array_1d_memcpy (vercoos[i], 3, Tess.VerCoo[vers[i]]);

  for (i = 0; i < verqty; i++)
    for (j = 0; j < Tess.Dim; j++)
      vercoos[i][j] += eps * drand48 ();

  net_pts_convexhull (vercoos, verqty, 3, pN, pM);

  for (i = 0; i < verqty; i++)
  {
    distmin = DBL_MAX;
    distminid = -1;
    for (j = 1; j <= (*pN).NodeQty; j++)
    {
      dist = ut_space_dist (vercoos[i], (*pN).NodeCoo[j]);
      if (dist < distmin)
      {
	distmin = dist;
	distminid = j;
      }
    }

    if (distmin < eps)
      ut_array_1d_memcpy ((*pN).NodeCoo[distminid], 3, Tess.VerCoo[vers[i]]);
  }

  ut_free_1d (coo);
  ut_free_1d_int (pt);

  ut_free_1d_int (vers);
  ut_free_2d (vercoos, verqty);

  return;
}

void
neut_tess_poly_convexity (struct TESS Tess, int poly, double *pval)
{
  double vol, vol2;
  struct NODES N;
  struct MESH M;
  struct MESH M3;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (&M3);

  neut_tess_poly_volume (Tess, poly, &vol2);

  neut_tess_poly_convexhull (Tess, poly, &N, &M);
  neut_mesh2d_mesh3d (&N, M, &M3);
  neut_mesh_volume (N, M3, &vol);

  (*pval) = vol2 / vol;
  if (*pval > 1 + 1e-6 || *pval < 0)
  {
    printf ("vol  = %.15f\n", vol);
    printf ("vol2 = %.15f\n", vol2);
    printf ("convexity = %.15f\n", *pval);
    ut_error_reportbug ();
  }

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  neut_mesh_free (&M3);

  return;
}

void
neut_tess_face_convexity (struct TESS Tess, int face, double *pval)
{
  double area, area2;
  struct NODES N;
  struct MESH M;
  struct MESH M2;

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  neut_mesh_set_zero (&M2);

  neut_tess_face_area (Tess, face, &area2);

  neut_tess_face_convexhull (Tess, face, &N, &M);
  neut_mesh1d_mesh2d (&N, M, &M2);
  neut_mesh_area (N, M2, &area);
  (*pval) = area2 / area;

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  neut_mesh_free (&M2);

  return;
}

void
neut_tess_cell_convexity (struct TESS Tess, int cell, double *pval)
{
  if (Tess.Dim == 2)
    neut_tess_face_convexity (Tess, cell, pval);
  else if (Tess.Dim == 3)
    neut_tess_poly_convexity (Tess, cell, pval);
  else
    abort ();

  return;
}

void
neut_tess_edgepair_angle (struct TESS Tess, int edge1, int edge2, double *pangle)
{
  int comver;
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);

  neut_tess_edge_dir (Tess, edge1, v1);
  neut_tess_edge_dir (Tess, edge2, v2);

  *pangle = ut_vector_angle (v1, v2);

  if (!neut_tess_edgepair_comver (Tess, edge1, edge2, &comver))
  {
    if (ut_array_1d_int_eltpos (Tess.EdgeVerNb[edge1], 2, comver)
     == ut_array_1d_int_eltpos (Tess.EdgeVerNb[edge2], 2, comver))
      (*pangle) -= 180;
  }

  ut_free_1d (v1);
  ut_free_1d (v2);

  return;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_cylside (struct TESS Tess, int cell, double *C, double *v,
                        double rad, int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_cylside (C, v, rad, Tess.VerCoo[vers[i]]);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (vers);
  ut_free_1d_int (side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_cyl2side (struct TESS Tess, int cell, double *C, double *v,
                         double *ell1, double *ell2, double rad1, double rad2,
                         int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
    side[i] = ut_space_cyl2side (C, v, ell1, ell2, rad1, rad2, Tess.VerCoo[vers[i]]);

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (vers);
  ut_free_1d_int (side);

  return 0;
}

// -1: inside, 1: outside, 0: cut
int
neut_tess_cell_torusside (struct TESS Tess, int cell, double *C, double *v,
                          double rad, double rad2, int *pside)
{
  int i, verqty, min, max;
  int *vers = NULL;
  int *side = NULL;
  double *P = NULL;
  double *K = ut_alloc_1d (3);
  double dist;

  neut_tess_cell_vers (Tess, cell, &vers, &verqty);

  if (verqty == 0)
    return -1;

  side = ut_alloc_1d_int (verqty);

  for (i = 0; i < verqty; i++)
  {
    P = Tess.VerCoo[vers[i]];
    ut_space_point_circle_dist (P, C, v, rad, &dist, K);
    side[i] = (dist < rad2) ? -1 : 1;
  }

  min = ut_array_1d_int_min (side, verqty);
  max = ut_array_1d_int_max (side, verqty);

  if (min == max)
    (*pside) = min;
  else
    (*pside) = 0;

  ut_free_1d_int (vers);
  ut_free_1d_int (side);
  ut_free_1d (K);

  return 0;
}

void
neut_tess_face_eq_fromvers (struct TESS Tess, int face, double *eq)
{
  int verqty;
  double **vercoos = NULL;

  neut_tess_face_vercoos (Tess, face, &vercoos, &verqty);

  ut_space_pointset_plane (vercoos, verqty, eq);

  ut_free_2d (vercoos, verqty);

  return;
}

void
neut_tess_faceinter_fromdomface (struct TESS *pTess, int face)
{
  int domface = (*pTess).FaceDom[face][1];

  if (domface <= 0)
    abort ();

  neut_tess_face_centre (*pTess, face, (*pTess).FacePtCoo[face]);

  neut_primparms_point_proj ((*pTess).DomFaceType[domface],
                             (*pTess).DomFaceParms[domface],
                             (*pTess).FacePtCoo[face],
                             (*pTess).FacePtCoo[face]);

  neut_primparms_point_tangentplane ((*pTess).DomFaceType[domface],
                             (*pTess).DomFaceParms[domface],
                             (*pTess).FacePtCoo[face],
                             (*pTess).FaceEq[face]);

  (*pTess).FaceState[face] = 0;
  (*pTess).FacePt[face] = 0;

  return;
}

int
neut_tess_domfaces_point_proj (struct TESS Tess, int *domfaces, int domfaceqty,
                               double *point, double *proj)
{
  int i, iter = 0, domface, itermax = 100;
  double dist, totdist, eps;
  double *point_cpy = ut_alloc_1d (3);

  eps = 1e-6;

  ut_array_1d_memcpy (point_cpy, 3, point);

  ut_array_1d_memcpy (proj, 3, point);

  do
  {
    iter++;
    totdist = 0;

    for (i = 0; i < domfaceqty; i++)
    {
      domface = domfaces[i];
      dist = neut_primparms_point_proj (Tess.DomFaceType[domface],
                                        Tess.DomFaceParms[domface],
                                        proj, proj);
      totdist += dist;
    }
  }
  while (iter <= itermax && totdist > eps);

#ifdef DEVEL_DEBUGGING_TEST
  if (iter > itermax)
  {
    neut_tess_name_fprintf_gmsh ("tmp.geo", Tess);
    printf ("point = ");
    ut_array_1d_fprintf (stdout, point_cpy, 3, "%f");
    printf ("fails to project onto domfaces\n");
    for (i = 0; i < domfaceqty; i++)
    {
      domface = domfaces[i];
      printf ("  %d: %s ", domface, Tess.DomFaceType[domface]);
      ut_array_1d_fprintf (stdout, Tess.DomFaceParms[domface], Tess.DomFaceParmQty[domface], "%f");
    }
    printf ("totdist = %f eps = %f\n", totdist, eps);

    abort ();
  }
#endif

  ut_free_1d (point_cpy);

  return (iter <= itermax) ? 0 : -1;
}

void
neut_tess_poly_faces_aveq (struct TESS Tess, int poly, int *faces, int faceqty,
                           double *eq)
{
  int i, ori, face;
  double area, *tmp = ut_alloc_1d (4);

  ut_array_1d_zero (eq, 4);

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    ut_array_1d_memcpy (tmp, 4, Tess.FaceEq[face]);

    neut_tess_poly_face_ori (Tess, poly, face, &ori);

    neut_tess_face_area (Tess, face, &area);

    ut_array_1d_scale (tmp, 4, ori * area);

    ut_array_1d_add (eq, tmp, 4, eq);
  }

  ut_array_1d_scale (eq, 4, 1 / ut_array_1d_norm (eq + 1, 3));

  ut_free_1d (tmp);

  return;
}

void
neut_tess_coo_percoos (struct TESS Tess, double *coo, double ***ppercoos, int *ppercooqty)
{
  int i, j, k, l, id, qty = ut_array_1d_int_sum (Tess.Periodic, 3);
  int *per = ut_alloc_1d_int (3);

  (*ppercooqty) = pow (3, qty);

  (*ppercoos) = ut_alloc_2d (*ppercooqty, 3);

  ut_array_1d_memcpy (*ppercoos[0], 3, coo);
  for (i = 0; i < 3; i++)
  {
    while (Tess.Periodic[i] && (*ppercoos)[0][i] < 0)
      (*ppercoos)[0][i] += Tess.PeriodicDist[i];
    while (Tess.Periodic[i] && (*ppercoos)[0][i] > Tess.PeriodicDist[i])
      (*ppercoos)[0][i] -= Tess.PeriodicDist[i];
  }

  id = 0;
  for (k = -Tess.Periodic[2]; k <= Tess.Periodic[2]; k++)
    for (j = -Tess.Periodic[1]; j <= Tess.Periodic[1]; j++)
      for (i = -Tess.Periodic[0]; i <= Tess.Periodic[0]; i++)
        if (i != 0 || j != 0 || k != 0)
        {
          ut_array_1d_int_set_3 (per, i, j, k);

          id++;
          for (l = 0; l < 3; l++)
            (*ppercoos)[id][l] = (*ppercoos)[0][l] + per[l] * Tess.PeriodicDist[l];
        }

  ut_free_1d_int (per);

  return;
}
