/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_gen_.h"

void
neut_tess_cell (struct TESS Tess, char **pcell)
{
  if (Tess.Dim == 2)
    ut_string_string ("face", pcell);
  else if (Tess.Dim == 3)
    ut_string_string ("poly", pcell);
  else
    ut_error_reportbug ();

  return;
}

void
neut_tess_var_list (struct TESS Tess, char *entity, char ***pvar,
		    int *pvarqty)
{
  int id = 0;

  if (!strcmp (entity, "seed"))
  {
    (*pvarqty) = 6;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "w");
    strcpy ((*pvar)[id++], "cell_shown");
  }
  else if (!strcmp (entity, "poly")
	   || (!(strcmp (entity, "cell")) && Tess.Dim == 3))
  {
    (*pvarqty) = 25;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "true");
    strcpy ((*pvar)[id++], "body");
    strcpy ((*pvar)[id++], "lamid");
    strcpy ((*pvar)[id++], "modeid");
    strcpy ((*pvar)[id++], "state");
    strcpy ((*pvar)[id++], "vol");
    strcpy ((*pvar)[id++], "size");
    strcpy ((*pvar)[id++], "diameq");
    strcpy ((*pvar)[id++], "radeq");
    strcpy ((*pvar)[id++], "area");
    strcpy ((*pvar)[id++], "sphericity");
    strcpy ((*pvar)[id++], "convexity");
    strcpy ((*pvar)[id++], "dihangleav");
    strcpy ((*pvar)[id++], "dihanglemin");
    strcpy ((*pvar)[id++], "dihanglemax");
    strcpy ((*pvar)[id++], "dihanglelist");
    strcpy ((*pvar)[id++], "vernb");
    strcpy ((*pvar)[id++], "edgenb");
    strcpy ((*pvar)[id++], "facenb");
    strcpy ((*pvar)[id++], "neighnb");
    strcpy ((*pvar)[id++], "scaleid");
  }
  else if (!strcmp (entity, "face")
	   || (!(strcmp (entity, "cell")) && Tess.Dim == 2))
  {
    (*pvarqty) = 31;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "true");
    strcpy ((*pvar)[id++], "body");
    strcpy ((*pvar)[id++], "lamid");
    strcpy ((*pvar)[id++], "modeid");
    strcpy ((*pvar)[id++], "area");
    strcpy ((*pvar)[id++], "size");
    strcpy ((*pvar)[id++], "diameq");
    strcpy ((*pvar)[id++], "radeq");
    strcpy ((*pvar)[id++], "circularity");
    strcpy ((*pvar)[id++], "sphericity");
    strcpy ((*pvar)[id++], "convexity");
    strcpy ((*pvar)[id++], "dihangleav");
    strcpy ((*pvar)[id++], "dihanglemin");
    strcpy ((*pvar)[id++], "dihanglemax");
    strcpy ((*pvar)[id++], "dihanglelist");
    strcpy ((*pvar)[id++], "state");
    strcpy ((*pvar)[id++], "vernb");
    strcpy ((*pvar)[id++], "edgenb");
    strcpy ((*pvar)[id++], "polynb");
    strcpy ((*pvar)[id++], "cellnb");
    strcpy ((*pvar)[id++], "neighnb");
    strcpy ((*pvar)[id++], "ff");
    strcpy ((*pvar)[id++], "domtype");
    strcpy ((*pvar)[id++], "poly_shown");
    strcpy ((*pvar)[id++], "cell_shown");
    strcpy ((*pvar)[id++], "scaleid");
    strcpy ((*pvar)[id++], "theta");
  }
  else if (!strcmp (entity, "edge"))
  {
    (*pvarqty) = 22;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "domtype");
    strcpy ((*pvar)[id++], "true");
    strcpy ((*pvar)[id++], "body");
    strcpy ((*pvar)[id++], "state");
    strcpy ((*pvar)[id++], "length");
    strcpy ((*pvar)[id++], "size");
    strcpy ((*pvar)[id++], "diameq");
    strcpy ((*pvar)[id++], "radeq");
    strcpy ((*pvar)[id++], "vernb");
    strcpy ((*pvar)[id++], "facenb");
    strcpy ((*pvar)[id++], "polynb");
    strcpy ((*pvar)[id++], "cellnb");
    strcpy ((*pvar)[id++], "neighnb");
    strcpy ((*pvar)[id++], "cyl");
    strcpy ((*pvar)[id++], "poly_shown");
    strcpy ((*pvar)[id++], "face_shown");
    strcpy ((*pvar)[id++], "cell_shown");
    strcpy ((*pvar)[id++], "theta");
  }
  else if (!strcmp (entity, "ver"))
  {
    (*pvarqty) = 13;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "edgenb");
    strcpy ((*pvar)[id++], "facenb");
    strcpy ((*pvar)[id++], "polynb");
    strcpy ((*pvar)[id++], "cellnb");
    strcpy ((*pvar)[id++], "domtype");
    strcpy ((*pvar)[id++], "poly_shown");
    strcpy ((*pvar)[id++], "face_shown");
    strcpy ((*pvar)[id++], "edge_shown");
    strcpy ((*pvar)[id++], "cell_shown");
  }
  else
    ut_error_reportbug ();

  return;
}

void
neut_tess_var_dim (int dim, char *entity, char *var, int *pdim)
{
  if (!strcmp (entity, "seed"))
    (*pdim) = 0;
  else if (!strcmp (entity, "poly")
           || (!strcmp (entity, "cell") && dim == 3))
  {
    if (!strcmp (var, "vol") || !strcmp (var, "size"))
      (*pdim) = 3;
    else if (!strcmp (var, "area"))
      (*pdim) = 2;
    else if (!strcmp (var, "diameq"))
      (*pdim) = 1;
    else if (!strcmp (var, "radeq"))
      (*pdim) = 1;
    else
      (*pdim) = 0;
  }
  else if (!strcmp (entity, "face")
           || (!strcmp (entity, "cell") && dim == 2))
  {
    if (!strcmp (var, "area") || !strcmp (var, "size"))
      (*pdim) = 2;
    else if (!strcmp (var, "diameq"))
      (*pdim) = 1;
    else if (!strcmp (var, "radeq"))
      (*pdim) = 1;
    else
      (*pdim) = 0;
  }
  else if (!strcmp (entity, "edge"))
  {
    if (!strcmp (var, "length")
	|| !strcmp (var, "size") || !strcmp (var, "diameq")
	|| !strcmp (var, "radeq"))
      (*pdim) = 1;
    else
      (*pdim) = 0;
  }
  else if (!strcmp (entity, "ver"))
    (*pdim) = 0;
  else if (!strcmp (entity, "point"))
    (*pdim) = 0;
  else
    ut_error_reportbug ();

  return;
}

int
neut_tess_var_val (struct TESS Tess,
		   int *showedge,
		   int *showface, int *showpoly,
		   char *entity, int id, char *var,
		   double *pval, char **ptype)
{
  int i, j, tmp, status, scale;
  double *c = ut_alloc_1d (3);
  char *typetmp = ut_alloc_1d_char (10);
  char *var2 = ut_alloc_1d_char (strlen (var) + 1);

  strcpy (var2, var);
  if (strstr (var2, "cell"))
  {
    if (Tess.Dim == 3)
      ut_string_fnrs (var2, "cell", "poly", 1);
    else if (Tess.Dim == 2)
      ut_string_fnrs (var2, "cell", "face", 1);
  }

  status = -1;
  if (!strcmp (entity, "seed"))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      (*pval) = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      (*pval) = Tess.SeedCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      (*pval) = Tess.SeedCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      (*pval) = Tess.SeedCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "w"))
    {
      (*pval) = Tess.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "poly_shown") && Tess.Dim == 3)
    {
      (*pval) = (showpoly) ? showpoly[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "face_shown") && Tess.Dim == 2)
    {
      (*pval) = (showface) ? showface[id] : 0;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }
  else if (!strcmp (entity, "poly")
	   || (!strcmp (entity, "cell") && Tess.Dim == 3))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      if (!strcmp (entity, "poly"))
	(*pval) = id;
      else
	(*pval) = Tess.CellId ? Tess.CellId[id] : id;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_poly_centroid (Tess, id, c);
      (*pval) = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_poly_centroid (Tess, id, c);
      (*pval) = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_poly_centroid (Tess, id, c);
      (*pval) = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pval) = Tess.CellTrue[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pval) = Tess.CellBody[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "lamid"))
    {
      (*pval) = Tess.CellLamId? Tess.CellLamId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "modeid"))
    {
      (*pval) = Tess.CellModeId? Tess.CellModeId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "state"))
    {
      (*pval) = 0;
      for (i = 1; i <= Tess.PolyFaceQty[id]; i++)
	if (Tess.FaceState[Tess.PolyFaceNb[id][i]] != 0)
	{
	  (*pval) = 1;
	  break;
	}
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vol") || !strcmp (var2, "size"))
    {
      neut_tess_poly_volume (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "diameq"))
    {
      neut_tess_poly_diameq (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "radeq"))
    {
      neut_tess_poly_radeq (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "area"))
    {
      neut_tess_poly_area (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "sphericity"))
    {
      neut_tess_poly_sphericity (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "convexity"))
    {
      neut_tess_poly_convexity (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "dihangleav"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      (*pval) = ut_array_1d_mean (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemin"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      (*pval) = ut_array_1d_min (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemax"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      (*pval) = ut_array_1d_max (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "vernb"))
    {
      neut_tess_poly_vers (Tess, id, NULL, &tmp);
      (*pval) = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "edgenb"))
    {
      neut_tess_poly_edges (Tess, id, NULL, &tmp);
      (*pval) = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pval) = Tess.PolyFaceQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "neighnb"))
    {
      neut_tess_poly_neighpoly (Tess, id, NULL, &tmp);
      (*pval) = tmp;

      strcpy (typetmp, "%d");
    }
    else if (!strncmp (var2, "scaleid(", 8))
    {
      sscanf (var2, "scaleid(%d)", &scale);
      (*pval) = (scale <= Tess.ScaleQty) ? Tess.ScaleCellId[id][scale] : -1;
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }
  else if (!strcmp (entity, "face")
	   || (!strcmp (entity, "cell") && Tess.Dim == 2))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      if (!strcmp (entity, "face"))
	(*pval) = id;
      else
	(*pval) = Tess.CellId ? Tess.CellId[id] : id;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_face_centre_x (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_face_centre_y (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_face_centre_z (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pval) = neut_tess_face_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pval) = neut_tess_face_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "lamid"))
    {
      (*pval) = Tess.CellLamId? Tess.CellLamId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "modeid"))
    {
      (*pval) = Tess.CellModeId? Tess.CellModeId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "area") || !strcmp (var2, "size"))
    {
      neut_tess_face_area (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "diameq"))
    {
      neut_tess_face_diameq (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "radeq"))
    {
      neut_tess_face_radeq (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "circularity") || !strcmp (var2, "sphericity"))
    {
      neut_tess_face_circularity (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "convexity"))
    {
      neut_tess_face_convexity (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "dihangleav"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      (*pval) = ut_array_1d_mean (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemin"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      (*pval) = ut_array_1d_min (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemax"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      (*pval) = ut_array_1d_max (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "state"))
    {
      (*pval) = Tess.FaceState[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vernb"))
    {
      (*pval) = Tess.FaceVerQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pval) = Tess.FaceVerQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "neighnb"))
    {
      neut_tess_face_neighfaces (Tess, id, NULL, &tmp);
      (*pval) = tmp;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_face_polys (Tess, id, NULL, &tmp);
      (*pval) = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "ff"))
    {
      (*pval) = neut_tess_face_ff (Tess, id);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pval) = Tess.FaceDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pval) = (showpoly) ? showpoly[Tess.FaceDom[id][0]] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strncmp (var2, "scaleid(", 8))
    {
      sscanf (var2, "scaleid(%d)", &scale);
      (*pval) = (scale <= Tess.ScaleQty) ? Tess.ScaleCellId[id][scale] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "theta"))
    {
      if (Tess.Dim != 3)
	(*pval) = -1;
      else
      {
	int qty, *tmp = NULL;
	neut_tess_face_cells (Tess, id, &tmp, &qty);

	if (qty == 1)
	  (*pval) = -1;
	else
	  ol_q_q_disori (Tess.CellOri[tmp[0]], Tess.CellOri[tmp[1]],
	      Tess.CellCrySym, pval);
	ut_free_1d_int_ (&tmp);
      }
      strcpy (typetmp, "%f");
    }
    else
      status = -1;
  }
  else if (!strcmp (entity, "edge"))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      if (!strcmp (entity, "edge"))
	(*pval) = id;
      else
	(*pval) = Tess.CellId ? Tess.CellId[id] : id;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_edge_centre_x (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_edge_centre_y (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_edge_centre_z (Tess, id, pval);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pval) = Tess.EdgeDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pval) = neut_tess_edge_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pval) = neut_tess_edge_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "state"))
    {
      (*pval) = Tess.EdgeState[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "length")
	     || !strcmp (var2, "size") || !strcmp (var2, "diameq"))
    {
      (*pval) = Tess.EdgeLength[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "radeq"))
    {
      (*pval) = Tess.EdgeLength[id] * .5;
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "vernb"))
    {
      (*pval) = 2;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pval) = Tess.EdgeFaceQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_edge_polys (Tess, id, NULL, &tmp);
      (*pval) = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "cyl"))
    {
      (*pval) = neut_tess_edge_fake (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pval) = 0;
      if (showpoly)
      {
	int polyqty;
	int *poly = NULL;
	neut_tess_edge_polys (Tess, id, &poly, &polyqty);
	for (j = 0; j < polyqty; j++)
	  if (poly[j] > 0 && showpoly[poly[j]] == 1)
	  {
	    (*pval) = 1;
	    break;
	  }
	ut_free_1d_int (poly);
      }
    }
    else if (!strcmp (var2, "face_shown"))
    {
      (*pval) = 0;
      if (showface)
      {
	int face;
	for (j = 0; j < Tess.EdgeFaceQty[id]; j++)
	{
	  face = Tess.EdgeFaceNb[id][j];
	  if (showface[face])
	  {
	    (*pval) = 1;
	    break;
	  }
	}
      }
    }
    else if (!strcmp (var2, "theta"))
    {
      if (Tess.Dim != 2)
	(*pval) = -1;
      else
      {
	int qty, *tmp = NULL;
	neut_tess_edge_cells (Tess, id, &tmp, &qty);

	if (qty == 1)
	  (*pval) = -1;
	else
	  ol_q_q_disori (Tess.CellOri[tmp[0]], Tess.CellOri[tmp[1]],
	      Tess.CellCrySym, pval);
	ut_free_1d_int_ (&tmp);
      }
      strcpy (typetmp, "%f");
    }
  }
  else if (!strcmp (entity, "ver"))
  {
    status = 0;
    if (!strcmp (var2, "id"))
    {
      (*pval) = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      (*pval) = Tess.VerCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      (*pval) = Tess.VerCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      (*pval) = Tess.VerCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pval) = Tess.VerEdgeQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "facenb"))
    {
      neut_tess_ver_faces (Tess, id, NULL, &tmp);
      (*pval) = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_ver_polys (Tess, id, NULL, &tmp);
      (*pval) = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pval) = neut_tess_ver_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pval) = neut_tess_ver_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "state"))
    {
      (*pval) = Tess.VerState[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pval) = Tess.VerDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pval) = 0;
      if (showpoly)
      {
	int polyqty;
	int *poly = NULL;
	neut_tess_ver_polys (Tess, id, &poly, &polyqty);
	for (j = 0; j < polyqty; j++)
	  if (poly[j] > 0 && showpoly[poly[j]])
	  {
	    (*pval) = 1;
	    break;
	  }
	ut_free_1d_int (poly);
      }
    }
    else if (!strcmp (var2, "face_shown"))
    {
      (*pval) = 0;
      if (showface)
      {
	int faceqty;
	int *face = NULL;
	neut_tess_ver_faces (Tess, id, &face, &faceqty);
	for (j = 0; j < faceqty; j++)
	  if (face[j] > 0 && showface[face[j]])
	  {
	    (*pval) = 1;
	    break;
	  }
	ut_free_1d_int (face);
      }
    }
    else if (!strcmp (var2, "edge_shown"))
    {
      (*pval) = 0;
      if (showedge)
      {
	int edge;
	for (j = 0; j < Tess.VerEdgeQty[id]; j++)
	{
	  edge = Tess.VerEdgeNb[id][j];
	  if (showedge[edge])
	  {
	    (*pval) = 1;
	    break;
	  }
	}
      }
    }
    else
      status = -1;
  }
  else
    ut_error_reportbug ();

  if (ptype)
  {
    ut_free_1d_char (*ptype);
    (*ptype) = ut_alloc_1d_char (strlen (typetmp) + 1);
    strcpy (*ptype, typetmp);
  }

  ut_free_1d (c);
  ut_free_1d_char (typetmp);
  ut_free_1d_char (var2);

  return status;
}

int
neut_tess_var_val_all (struct TESS Tess,
		       int *showedge,
		       int *showface, int *showpoly,
		       char *entity, char *var, double *val, char **ptype)
{
  int i, qty, dim, trueqty, bodyqty;
  double size, factor, tmp, mean;
  char **var2 = NULL;
  int var2qty;

  ut_string_separate (var, NEUT_SEP_DEP, &var2, &var2qty);

  neut_tess_entity_qty (Tess, entity, &qty);

  for (i = 1; i <= qty; i++)
    neut_tess_var_val (Tess, showedge, showface, showpoly,
		       entity, i, var2[0], val + i, ptype);

  if (var2qty == 2)
  {
    if (!strncmp (var2[1], "rel", 3))
    {
      if (!strcmp (var2[1], "rel"))
	mean = ut_array_1d_mean (val + 1, qty);
      else if (!strcmp (var2[1], "relt"))
      {
	mean = 0;
	trueqty = 0;
	for (i = 1; i <= qty; i++)
	  if (neut_tess_entity_true (Tess, entity, i) > 0)
	  {
	    mean += val[i];
	    trueqty++;
	  }

	if (trueqty == 0)
	  abort ();

	mean /= trueqty;
      }
      else if (!strcmp (var2[1], "relb"))
      {
	if (!Tess.CellBody)
	  abort ();

	mean = 0;
	trueqty = 0;
	for (i = 1; i <= qty; i++)
	  if (neut_tess_entity_body (Tess, entity, i) > 0)
	  {
	    mean += val[i];
	    trueqty++;
	  }

	if (trueqty == 0)
	  abort ();

	mean /= trueqty;
      }
      else
	abort ();

      factor = 1. / mean;
    }
    else if (!strncmp (var2[1], "uc", 2))
    {
      if (!strcmp (var2[1], "uc"))
      {
	neut_tess_size (Tess, &size);
	size /= Tess.CellQty;
      }
      else if (!strcmp (var2[1], "uct"))
      {
	if (!Tess.CellTrue)
	  abort ();

	size = 0;
	trueqty = 0;
	for (i = 1; i <= Tess.CellQty; i++)
	  if (Tess.CellTrue[i] > 0)
	  {
	    neut_tess_cell_size (Tess, i, &tmp);
	    size += tmp;
	    trueqty++;
	  }

	if (trueqty == 0)
	  abort ();

	size /= trueqty;
      }
      else if (!strcmp (var2[1], "ucb"))
      {
	if (!Tess.CellBody)
	  abort ();

	size = 0;
	bodyqty = 0;
	for (i = 1; i <= Tess.CellQty; i++)
	  if (Tess.CellBody[i] > 0)
	  {
	    neut_tess_cell_size (Tess, i, &tmp);
	    size += tmp;
	    bodyqty++;
	  }

	if (bodyqty == 0)
	  abort ();

	size /= bodyqty;
      }
      else
	abort ();

      size = pow (size, 1. / Tess.Dim);
      neut_tess_var_dim (Tess.Dim, entity, var2[0], &dim);
      factor = pow (1. / size, dim);
    }
    else
      abort ();

    ut_array_1d_scale (val + 1, qty, factor);
  }

  ut_free_2d_char (var2, var2qty);

  return 0;
}

int
neut_tess_var_val_string (struct TESS Tess, char *entity, int id,
			  char *var, char **pval, char **type)
{
  int k, l, status, face, neighpoly;
  double area;
  double *eq = ut_alloc_1d (4);

  ut_free_1d_char (*pval);
  (*pval) = ut_alloc_1d_char (10000);

  status = 0;

  ut_free_1d_char (*type);
  (*type) = ut_alloc_1d_char (10);
  strcpy (*type, "%s");

  if (!strcmp (entity, "poly") || (!strcmp (entity, "cell") && Tess.Dim == 3))
  {
    if (!strcmp (var, "verlist"))
    {
      int *list = NULL;
      int verqty;
      neut_tess_poly_vers (Tess, id, &list, &verqty);
      for (k = 0; k < verqty; k++)
	sprintf (*pval + strlen (*pval), "%d%s", list[k],
		 (k < verqty - 1) ? " " : "");
      ut_free_1d_int (list);
    }
    else if (!strcmp (var, "vercoolist"))
    {
      int *list = NULL;
      int verqty;
      neut_tess_poly_vers (Tess, id, &list, &verqty);
      for (k = 0; k < verqty; k++)
      {
	for (l = 0; l < Tess.Dim; l++)
	  sprintf (*pval + strlen (*pval), "%.12f%s",
		   Tess.VerCoo[list[k]][l],
		   (k < verqty - 1 || l < Tess.Dim - 1) ? " " : "");
      }
      ut_free_1d_int (list);
    }
    else if (!strcmp (var, "edgelist"))
    {
      int *list = NULL;
      int edgeqty;
      neut_tess_poly_edges (Tess, id, &list, &edgeqty);
      for (k = 0; k < edgeqty; k++)
	sprintf (*pval + strlen (*pval), "%d%s", list[k],
		 (k < edgeqty - 1) ? " " : "");
      ut_free_1d_int (list);
    }
    else if (!strcmp (var, "facelist"))
    {
      for (k = 1; k <= Tess.PolyFaceQty[id]; k++)
	sprintf (*pval + strlen (*pval), "%d%s", Tess.PolyFaceNb[id][k],
		 (k < Tess.PolyFaceQty[id]) ? " " : "");
    }
    else if (!strcmp (var, "npolylist"))
    {
      for (k = 1; k <= Tess.PolyFaceQty[id]; k++)
      {
	face = Tess.PolyFaceNb[id][k];
	neighpoly = (Tess.FacePoly[face][0] != id) ?
	  Tess.FacePoly[face][0] : Tess.FacePoly[face][1];
	sprintf (*pval + strlen (*pval), "%d%s", neighpoly,
		 (k < Tess.PolyFaceQty[id]) ? " " : "");
      }
    }
    else if (!strcmp (var, "facearealist"))
    {
      for (k = 1; k <= Tess.PolyFaceQty[id]; k++)
      {
	face = Tess.PolyFaceNb[id][k];
	neut_tess_face_area (Tess, face, &area);
	sprintf (*pval + strlen (*pval), "%.12f%s", area,
		 (k < Tess.PolyFaceQty[id]) ? " " : "");
      }
    }
    else if (!strcmp (var, "faceeqlist"))
    {
      for (k = 1; k <= Tess.PolyFaceQty[id]; k++)
      {
	face = Tess.PolyFaceNb[id][k];
	ut_array_1d_memcpy (eq, 4, Tess.FaceEq[face]);
	if (Tess.PolyFaceOri[id][k] == -1)
	  ut_array_1d_scale (eq, 4, -1);
	for (l = 0; l < 4; l++)
	  sprintf (*pval + strlen (*pval), "%.12f%s", eq[l],
		   !(l == 3 && k == Tess.PolyFaceQty[id]) ? " " : "");
      }
    }
    else if (!strcmp (var, "dihanglelist"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      for (k = 0; k < qty; k++)
	sprintf (*pval + strlen (*pval), "%.12f%s", tmp[k],
		 (k < qty - 1) ? " " : "");
      ut_free_1d (tmp);
    }
    else
      status = -1;
  }
  else if (!strcmp (entity, "face")
	   || (!strcmp (entity, "cell") && Tess.Dim == 2))
  {
    if (!strcmp (var, "dihanglelist"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      for (k = 0; k < qty; k++)
	sprintf (*pval + strlen (*pval), "%.12f%s", tmp[k],
		 (k < qty - 1) ? " " : "");
      ut_free_1d (tmp);
    }
    else if (!strcmp (var, "verlist"))
    {
      int *list = NULL;
      int verqty;
      neut_tess_face_vers (Tess, id, &list, &verqty);
      for (k = 0; k < verqty; k++)
	sprintf (*pval + strlen (*pval), "%d%s", list[k],
		 (k < verqty - 1) ? " " : "");
      ut_free_1d_int (list);
    }
    else if (!strcmp (var, "edgelist"))
    {
      for (k = 1; k <= Tess.FaceVerQty[id]; k++)
	sprintf (*pval + strlen (*pval), "%d%s", Tess.FaceEdgeNb[id][k],
		 (k < Tess.FaceVerQty[id]) ? " " : "");
    }
    else if (!strcmp (var, "vercoolist"))
    {
      int *list = NULL;
      int verqty;
      neut_tess_face_vers (Tess, id, &list, &verqty);
      for (k = 0; k < verqty; k++)
      {
	for (l = 0; l < Tess.Dim; l++)
	  sprintf (*pval + strlen (*pval), "%.12f%s",
		   Tess.VerCoo[list[k]][l],
		   (k < verqty - 1 || l < Tess.Dim - 1) ? " " : "");
      }
      ut_free_1d_int (list);
    }
    else if (!strcmp (var, "nfacelist")
          || (!strcmp (var, "ncelllist") && Tess.Dim == 2))
    {
      int *list = NULL;
      int qty;
      neut_tess_face_neighfaces (Tess, id, &list, &qty);
      for (k = 0; k < qty; k++)
	sprintf (*pval + strlen (*pval), "%d%s", list[k],
		 (k < qty - 1) ? " " : "");
      ut_free_1d_int (list);
    }
    else
      status = -1;
  }
  else if (!strcmp (entity, "edge"))
  {
    if (!strcmp (var, "facelist"))
    {
      for (k = 0; k < Tess.EdgeFaceQty[id]; k++)
	sprintf (*pval + strlen (*pval), "%d%s", Tess.EdgeFaceNb[id][k],
		 (k < Tess.EdgeFaceQty[id] - 1) ? " " : "");
    }
    else
      status = -1;
  }
  else
    status = -1;

  ut_free_1d (eq);
  (*pval) = ut_realloc_1d_char (*pval, strlen (*pval) + 1);

  return status;
}

void
neut_tess_entity_dim (char *entity, int *pdim)
{
  if (!strcmp (entity, "ver"))
    (*pdim) = 0;
  else if (!strcmp (entity, "edge"))
    (*pdim) = 1;
  else if (!strcmp (entity, "face"))
    (*pdim) = 2;
  else if (!strcmp (entity, "poly"))
    (*pdim) = 3;
  else if (!strcmp (entity, "seed"))
    (*pdim) = 4;
  else
    (*pdim) = -1;

  return;
}

void
neut_tess_dim_entity (int dim, char **pentity)
{
  if (dim == 0)
    ut_string_string ("ver", pentity);
  else if (dim == 1)
    ut_string_string ("edge", pentity);
  else if (dim == 2)
    ut_string_string ("face", pentity);
  else if (dim == 3)
    ut_string_string ("poly", pentity);
  else
    abort ();

  return;
}

void
neut_tess_entity_qty (struct TESS Tess, char *entity, int *pqty)
{
  if (!strcmp (entity, "ver"))
    (*pqty) = Tess.VerQty;
  else if (!strcmp (entity, "edge"))
    (*pqty) = Tess.EdgeQty;
  else if (!strcmp (entity, "face"))
    (*pqty) = Tess.FaceQty;
  else if (!strcmp (entity, "poly"))
    (*pqty) = Tess.PolyQty;
  else if (!strcmp (entity, "cell"))
    (*pqty) = Tess.CellQty;
  else if (!strcmp (entity, "seed"))
    (*pqty) = Tess.CellQty;
  else
    (*pqty) = -1;

  return;
}

int
neut_tess_expr_polylist (struct TESS Tess, char *expr, int **ppoly,
			 int *ppolyqty)
{
  int i, j, status;
  double res;
  FILE *file = NULL;

  if (!expr)
    (*ppolyqty) = 0;

  else if (!strcmp (expr, "all"))
  {
    (*ppolyqty) = Tess.PolyQty;
    (*ppoly) = ut_alloc_1d_int (*ppolyqty + 1);
    for (i = 1; i <= *ppolyqty; i++)
      (*ppoly)[i - 1] = i;
  }

  else if (ut_string_filename (expr))
  {
    (*ppolyqty) = ut_file_nbwords (expr);
    (*ppoly) = ut_alloc_1d_int (*ppolyqty);
    file = ut_file_open (expr, "r");
    ut_array_1d_int_fscanf (file, *ppoly, *ppolyqty);
    ut_file_close (file, expr, "r");
  }

  else
  {
    int var_qty;
    char **vars = NULL;
    double *vals = NULL;

    neut_tess_var_list (Tess, "poly", &vars, &var_qty);
    vals = ut_alloc_1d (var_qty);

    (*ppolyqty) = 0;
    for (i = 1; i <= Tess.PolyQty; i++)
    {
      for (j = 0; j < var_qty; j++)
	if (strstr (expr, vars[j]))
	  neut_tess_var_val (Tess, NULL, NULL, NULL, "poly", i,
			     vars[j], &(vals[j]), NULL);

      status = ut_math_eval (expr, var_qty, vars, vals, &res);
      if (status == -1)
	abort ();
      if (ut_num_equal (res, 1, 1e-6))
      {
	(*ppoly) = ut_realloc_1d_int (*ppoly, ++(*ppolyqty));
	(*ppoly)[(*ppolyqty) - 1] = i;
      }
    }

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  return 0;
}

int
neut_tess_expr_facelist (struct TESS Tess, char *expr, int **pface,
			 int *pfaceqty)
{
  int i, j, status;
  double res;
  FILE *file = NULL;

  if (!expr)
    (*pfaceqty) = 0;

  else if (!strcmp (expr, "all"))
  {
    (*pfaceqty) = Tess.FaceQty;
    (*pface) = ut_alloc_1d_int (*pfaceqty + 1);
    for (i = 1; i <= *pfaceqty; i++)
      (*pface)[i - 1] = i;
  }

  else if (ut_string_filename (expr))
  {
    (*pfaceqty) = ut_file_nbwords (expr);
    (*pface) = ut_alloc_1d_int (*pfaceqty);
    file = ut_file_open (expr, "r");
    ut_array_1d_int_fscanf (file, *pface, *pfaceqty);
    ut_file_close (file, expr, "r");
  }
  else
  {
    int var_qty;
    char **vars = NULL;
    double *vals = NULL;

    neut_tess_var_list (Tess, "face", &vars, &var_qty);
    vals = ut_alloc_1d (var_qty);

    (*pfaceqty) = 0;
    for (i = 1; i <= Tess.FaceQty; i++)
    {
      for (j = 0; j < var_qty; j++)
	if (strstr (expr, vars[j]))
	  neut_tess_var_val (Tess, NULL, NULL, NULL, "face", i,
			     vars[j], &(vals[j]), NULL);

      status = ut_math_eval (expr, var_qty, vars, vals, &res);
      if (status == -1)
	abort ();
      if (ut_num_equal (res, 1, 1e-6))
      {
	(*pface) = ut_realloc_1d_int (*pface, ++(*pfaceqty));
	(*pface)[(*pfaceqty) - 1] = i;
      }
    }

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  return 0;
}

void
neut_tess_entity_expr_val (struct TESS Tess, char *entity, char *expr,
			   double *val)
{
  int i, j, k, status;
  int qty, *qty2 = NULL;
  char ***list = NULL;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;
  int varqty;
  int entityqty;
  double tmp, tmp2;

  neut_tess_entity_qty (Tess, entity, &entityqty);
  ut_string_separate2 (expr, NEUT_SEP_NODEP, NEUT_SEP_DEP, &list, &qty2,
		       &qty);

  // patching list
  for (i = 0; i < qty; i++)
    if (qty2[i] == 2)
      continue;
    else if (qty2[i] == 1)
    {
      list[i] = ut_realloc_1d_pchar (list[i], 2);
      list[i][1] = NULL;
      ut_string_string (list[i][0], &(list[i][1]));
      strcpy (list[i][0], "1");
      qty2[i] = 2;
    }
    else
      abort ();

  neut_tess_var_list (Tess, entity, &vars, &varqty);

  vals = ut_alloc_1d (varqty);

  for (i = 0; i < qty; i++)
  {
    if (ut_string_filename (list[i][1]))
    {
      file = ut_file_open (list[i][1], "R");
      ut_array_1d_fscanf (file, val + 1, entityqty);
      ut_file_close (file, list[i][1], "R");
    }
    else
    {
      ut_string_real (list[i][1], &tmp);

      if (!strcmp (list[i][0], "1"))
	ut_array_1d_set (val + 1, entityqty, tmp);
      else
	for (j = 1; j <= entityqty; j++)
	{
	  for (k = 0; k < varqty; k++)
	    if (strstr (list[i][0], vars[k]))
	      neut_tess_var_val (Tess, NULL, NULL, NULL, entity, j, vars[k],
				 &(vals[k]), NULL);

	  status = ut_math_eval (list[i][0], varqty, vars, vals, &tmp2);

	  if (status == -1)
	    abort ();

	  if (ut_num_equal (tmp2, 1, 1e-6))
	    val[j] = tmp;
	}
    }
  }

  return;
}

int
neut_tess_dim_entityqty (struct TESS Tess, int dim)
{
  if (dim == 0)
    return Tess.VerQty;
  else if (dim == 1)
    return Tess.EdgeQty;
  else if (dim == 2)
    return Tess.FaceQty;
  else if (dim == 3)
    return Tess.PolyQty;
  else
    ut_error_reportbug ();

  return -1;
}

int
neut_tess_val_poly2face (struct TESS Tess, double *polyval, double *faceval)
{
  int i, j, seed, face, master;

  ut_array_1d_set (faceval + 1, Tess.FaceQty, DBL_MAX);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (ut_array_1d_int_max (Tess.FacePoly[i], 2) > 0)
      for (j = 0; j < 2; j++)
      {
	seed = Tess.FacePoly[i][j];
	if (seed > 0 && seed <= Tess.PolyQty)
	{
	  if (seed <= Tess.PolyQty)
	    faceval[i] = ut_num_min (faceval[i], polyval[seed]);
	  else
	  {
	    master = Tess.PerSeedMaster[seed];
	    faceval[i] = ut_num_min (faceval[i], polyval[master]);
	  }
	}
      }
    else
      abort ();

  // This does not look necessary
  if (!strncmp (Tess.Type, "periodic", 8))
    for (i = 1; i <= Tess.PerFaceQty; i++)
    {
      face = Tess.PerFaceNb[i];
      master = Tess.PerFaceMaster[face];
      faceval[face] = faceval[master];
    }

  return 0;
}

int
neut_tess_val_face2edge (struct TESS Tess, double *faceval, double *edgeval)
{
  int i, j, edge, face, master;

  ut_array_1d_set (edgeval + 1, Tess.EdgeQty, DBL_MAX);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeFaceQty[i] > 0)
      for (j = 0; j < Tess.EdgeFaceQty[i]; j++)
      {
	face = Tess.EdgeFaceNb[i][j];
	if (face > 0 && face <= Tess.FaceQty)
	  edgeval[i] = ut_num_min (edgeval[i], faceval[face]);
      }
    else
      abort ();

  // This does not look necessary
  if (!strncmp (Tess.Type, "periodic", 8))
    for (i = 1; i <= Tess.PerEdgeQty; i++)
    {
      edge = Tess.PerEdgeNb[i];
      master = Tess.PerEdgeMaster[edge];
      edgeval[edge] = edgeval[master];
    }

  return 0;
}

int
neut_tess_val_edge2ver (struct TESS Tess, double *edgeval, double *verval)
{
  int i, j, ver, edge, master;

  ut_array_1d_set (verval + 1, Tess.VerQty, DBL_MAX);

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerEdgeQty[i] > 0)
      for (j = 0; j < Tess.VerEdgeQty[i]; j++)
      {
	edge = Tess.VerEdgeNb[i][j];
	if (edge > 0)
	  verval[i] = ut_num_min (verval[i], edgeval[edge]);
      }
    else
      abort ();

  // This does not look necessary
  if (!strncmp (Tess.Type, "periodic", 8))
    for (i = 1; i <= Tess.PerVerQty; i++)
    {
      ver = Tess.PerVerNb[i];
      master = Tess.PerVerMaster[ver];
      verval[ver] = verval[master];
    }

  return 0;
}

int
neut_tess_poly_projection (struct TESS Tess, int cell, double *eq,
			   double ***pcoo, int *pqty)
{
  int i, j, verqty, *vers = NULL;
  int *ids = NULL;
  double **tmp = NULL;
  double *bary = ut_alloc_1d (3);
  double *z = ut_alloc_1d (3);
  double **g = ol_g_alloc ();
  double **ginv = ol_g_alloc ();
  z[2] = 1;

  neut_tess_poly_vers (Tess, cell, &vers, &verqty);
  tmp = ut_alloc_2d (verqty, 3);

  for (i = 0; i < verqty; i++)
    ut_array_1d_memcpy (tmp[i], 3, Tess.VerCoo[vers[i]]);

  for (i = 0; i < verqty; i++)
    ut_space_projpoint_alongonto (tmp[i], eq + 1, eq);

  // merging points
  for (i = 0; i < verqty; i++)
    for (j = i + 1; j < verqty; j++)
      if (ut_space_dist (tmp[i], tmp[j]) < 1e-6)
      {
	ut_array_2d_memcpy (tmp + j, verqty - j - 1, 3, tmp + j + 1);
	j--;
	verqty--;
      }

  // shifting toward origin
  ut_space_bary3d (tmp, verqty, bary);
  for (i = 0; i < verqty; i++)
    ut_array_1d_sub (bary, tmp[i], 3, tmp[i]);

  // aligning normal with z
  ol_vect_vect_g (eq + 1, z, g);
  ol_g_inverse (g, ginv);

  for (i = 0; i < verqty; i++)
    ut_mat_vect_product (ginv, 3, 3, tmp[i], 3, tmp[i]);

  for (i = 0; i < verqty; i++)
    if (fabs (tmp[i][2]) > 1e-6)
      ut_error_reportbug ();

  srand48 (1);
  for (i = 0; i < verqty; i++)
    for (j = 0; j < 2; j++)
      tmp[i][j] += drand48 () * 1e-12;

  ut_space_pts_convexhull_2d (tmp, verqty, &ids, pqty);
  (*pcoo) = ut_alloc_2d (*pqty, 3);
  for (i = 0; i < *pqty; i++)
  {
    ut_mat_vect_product (g, 3, 3, tmp[ids[i]], 3, (*pcoo)[i]);
    ut_array_1d_add ((*pcoo)[i], bary, 3, (*pcoo)[i]);
  }

  ut_free_2d (tmp, verqty);
  ut_free_1d_int (ids);
  ut_free_1d (bary);
  ut_free_1d (z);
  ol_g_free (g);

  return 0;
}

int
neut_tess_poly_projection_ontoline (struct TESS Tess, int cell, double *eq,
				    double ***pcoo, int *pqty)
{
  int i, verqty, *vers = NULL;
  double *proj = NULL;

  neut_tess_poly_vers (Tess, cell, &vers, &verqty);

  proj = ut_alloc_1d (verqty);
  for (i = 0; i < verqty; i++)
    proj[i] = ut_vector_scalprod (Tess.VerCoo[vers[i]], eq);

  (*pqty) = 2;
  (*pcoo) = ut_alloc_2d (2, 3);
  ut_array_1d_memcpy ((*pcoo)[0], 3, eq);
  ut_array_1d_scale ((*pcoo)[0], 3, ut_array_1d_min (proj, verqty));
  ut_array_1d_memcpy ((*pcoo)[1], 3, eq);
  ut_array_1d_scale ((*pcoo)[1], 3, ut_array_1d_max (proj, verqty));

  ut_free_1d (proj);

  return 0;
}

int
neut_tess_delaunay (struct TESS Tess, struct NODES *pNodes,
                    struct MESH *pMesh)
{
  int i, cellqty, *cells = NULL;

  neut_nodes_free (pNodes);
  neut_mesh_free (pMesh);

  (*pMesh) = neut_mesh_alloc (Tess.Dim, "tri", 1, 0, 1);

  (*pNodes).NodeQty = Tess.SeedQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);
  ut_array_2d_memcpy ((*pNodes).NodeCoo + 1, (*pNodes).NodeQty, 3,
                      Tess.SeedCoo + 1);

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerDom[i][0] == -1)
    {
      neut_tess_ver_seeds (Tess, i, &cells, &cellqty);
      if (cellqty != Tess.Dim + 1)
	abort ();

      neut_mesh_addelt (pMesh, cells);
      neut_mesh_elset_addelt (pMesh, 1, i);
    }

  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  ut_array_1d_int_set ((*pMesh).EltElset + 1, (*pMesh).EltQty, 1);

  neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

  ut_free_1d_int (cells);

  return 0;
}

int
neut_tess_dom_pt_randpt (struct TESS Dom, struct POINT Point,
			 gsl_rng *r, double rad, double penetration,
			 double *coo, double *pdist)
{
  int status;

  neut_tess_dom_pt_randpt_pick (Dom, Point, r, coo);
  status = neut_tess_dom_pt_randpt_test (Dom, Point, rad, penetration, coo, pdist);

  return status;
}

int
neut_tess_dom_pt_randpt_pick (struct TESS Dom, struct POINT Point,
			      gsl_rng *r, double *coo)
{
  int i;
  struct POLY Poly;

  neut_poly_set_zero (&Poly);
  net_tess_poly (Dom, 1, &Poly);

  // do not change the below loop to keep 3 gsl_rng_uniform calls /
  // centre for all dimensions.
  for (i = 0; i < 3; i++)
    coo[i] = Point.BBox[i][0] + gsl_rng_uniform (r) * (Point.BBox[i][1] -
	Point.BBox[i][0]);

  for (i = Point.Dim; i < 3; i++)
    coo[i] = ut_array_1d_mean (Point.BBox[i], 2);

  neut_poly_free (&Poly);

  return 0;
}

int
neut_tess_dom_pt_randpt_test (struct TESS Dom, struct POINT Point,
			      double rad,
			      double penetration, double *coo,
			      double *pdist)
{
  int i, status, l, m, n;
  double dist;
  double *ptcoo = ut_alloc_1d (3);

  status = 0;
  if (Dom.FaceQty > 0 && neut_tess_point_inpoly (Dom, coo, 1) == 0)
    status = -2;

  else if (pdist)
  {
    (*pdist) = 0;
    for (i = 1; i <= Point.PointQty; i++)
      for (l = -Point.Periodic[0]; l <= Point.Periodic[0]; l++)
      {
	ptcoo[0] = Point.PointCoo[i][0] + l * (Point.BBox[0][1] - Point.BBox[0][0]);
	for (m = -Point.Periodic[1]; m <= Point.Periodic[1]; m++)
	{
	  ptcoo[1] = Point.PointCoo[i][1] + m * (Point.BBox[1][1] - Point.BBox[1][0]);
	  for (n = -Point.Periodic[2]; n <= Point.Periodic[2]; n++)
	  {
	    ptcoo[2] = Point.PointCoo[i][2] + n * (Point.BBox[2][1] - Point.BBox[2][0]);
	    dist = ut_space_dist (ptcoo, coo) - Point.PointRad[i] - rad;
	    (*pdist) = ut_num_min (dist, (*pdist));
	  }
	}
      }

    if (*pdist < - penetration * rad)
      status = -1;
  }

  ut_free_1d (ptcoo);

  return status;
}

int
neut_tess_dom_pt_randpt_cluster (struct TESS Dom, struct POINT Point,
				 gsl_rng *r, struct POINT Point2,
				 double penetration,
				 double *coo, double *pdist)
{
  int i, status = 0;
  double rad2, *coo2 = ut_alloc_1d (3);
  double *val = ut_alloc_1d (Point2.PointQty + 1);

  neut_tess_dom_pt_randpt_pick (Dom, Point, r, coo);

  for (i = 1; i <= Point2.PointQty; i++)
  {
    ut_array_1d_add (coo, Point2.PointCoo[i], 3, coo2);
    rad2 = Point2.PointRad[i];
    status = neut_tess_dom_pt_randpt_test (Dom, Point, rad2, penetration,
					   coo2, val + i);
  }

  (*pdist) = ut_array_1d_sum (val + 1, Point2.PointQty);

  ut_free_1d (val);
  ut_free_1d (coo2);

  return status;
}
