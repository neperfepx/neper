/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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
    (*pvarqty) = 8;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "default");
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "w");
    strcpy ((*pvar)[id++], "cell_shown");
    strcpy ((*pvar)[id++], "e");
  }
  else if (!strcmp (entity, "poly")
	   || (!(strcmp (entity, "cell")) && Tess.Dim == 3))
  {
    (*pvarqty) = 45;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "default");
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "w");
    strcpy ((*pvar)[id++], "xmin");
    strcpy ((*pvar)[id++], "xmax");
    strcpy ((*pvar)[id++], "ymin");
    strcpy ((*pvar)[id++], "ymax");
    strcpy ((*pvar)[id++], "zmin");
    strcpy ((*pvar)[id++], "zmax");
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
    strcpy ((*pvar)[id++], "dihangles");
    strcpy ((*pvar)[id++], "vernb");
    strcpy ((*pvar)[id++], "edgenb");
    strcpy ((*pvar)[id++], "facenb");
    strcpy ((*pvar)[id++], "neighnb");
    strcpy ((*pvar)[id++], "scaleid");
    strcpy ((*pvar)[id++], "faces");
    strcpy ((*pvar)[id++], "edges");
    strcpy ((*pvar)[id++], "vers");
    strcpy ((*pvar)[id++], "npolys");
    strcpy ((*pvar)[id++], "ncells");
    strcpy ((*pvar)[id++], "npolynb");
    strcpy ((*pvar)[id++], "nseeds");
    strcpy ((*pvar)[id++], "nseednb");
    strcpy ((*pvar)[id++], "faceareas");
    strcpy ((*pvar)[id++], "faceeqs");
    strcpy ((*pvar)[id++], "vercoos");
    strcpy ((*pvar)[id++], "e");
  }
  else if (!strcmp (entity, "face")
	   || (!(strcmp (entity, "cell")) && Tess.Dim == 2))
  {
    (*pvarqty) = 47;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "default");
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "w");
    strcpy ((*pvar)[id++], "xmin");
    strcpy ((*pvar)[id++], "xmax");
    strcpy ((*pvar)[id++], "ymin");
    strcpy ((*pvar)[id++], "ymax");
    strcpy ((*pvar)[id++], "zmin");
    strcpy ((*pvar)[id++], "zmax");
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
    strcpy ((*pvar)[id++], "dihangles");
    strcpy ((*pvar)[id++], "state");
    strcpy ((*pvar)[id++], "vernb");
    strcpy ((*pvar)[id++], "edgenb");
    strcpy ((*pvar)[id++], "polynb");
    strcpy ((*pvar)[id++], "cellnb");
    strcpy ((*pvar)[id++], "neighnb");
    strcpy ((*pvar)[id++], "ff");
    strcpy ((*pvar)[id++], "domtype");
    strcpy ((*pvar)[id++], "domface");
    strcpy ((*pvar)[id++], "poly_shown");
    strcpy ((*pvar)[id++], "cell_shown");
    strcpy ((*pvar)[id++], "scaleid");
    strcpy ((*pvar)[id++], "theta");
    strcpy ((*pvar)[id++], "polys");
    strcpy ((*pvar)[id++], "edges");
    strcpy ((*pvar)[id++], "vers");
    strcpy ((*pvar)[id++], "nfaces");
    strcpy ((*pvar)[id++], "nfacenb");
    strcpy ((*pvar)[id++], "vercoos");
    strcpy ((*pvar)[id++], "e");
  }
  else if (!strcmp (entity, "edge"))
  {
    (*pvarqty) = 33;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "default");
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "xmin");
    strcpy ((*pvar)[id++], "xmax");
    strcpy ((*pvar)[id++], "ymin");
    strcpy ((*pvar)[id++], "ymax");
    strcpy ((*pvar)[id++], "zmin");
    strcpy ((*pvar)[id++], "zmax");
    strcpy ((*pvar)[id++], "domtype");
    strcpy ((*pvar)[id++], "domedge");
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
    strcpy ((*pvar)[id++], "polys");
    strcpy ((*pvar)[id++], "faces");
    strcpy ((*pvar)[id++], "vers");
  }
  else if (!strcmp (entity, "ver"))
  {
    (*pvarqty) = 18;
    (*pvar) = ut_alloc_2d_char (*pvarqty, 20);
    strcpy ((*pvar)[id++], "default");
    strcpy ((*pvar)[id++], "id");
    strcpy ((*pvar)[id++], "x");
    strcpy ((*pvar)[id++], "y");
    strcpy ((*pvar)[id++], "z");
    strcpy ((*pvar)[id++], "edgenb");
    strcpy ((*pvar)[id++], "facenb");
    strcpy ((*pvar)[id++], "polynb");
    strcpy ((*pvar)[id++], "cellnb");
    strcpy ((*pvar)[id++], "domtype");
    strcpy ((*pvar)[id++], "domver");
    strcpy ((*pvar)[id++], "poly_shown");
    strcpy ((*pvar)[id++], "face_shown");
    strcpy ((*pvar)[id++], "edge_shown");
    strcpy ((*pvar)[id++], "cell_shown");
    strcpy ((*pvar)[id++], "polys");
    strcpy ((*pvar)[id++], "faces");
    strcpy ((*pvar)[id++], "edges");
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
		   double **pvals, int *pvalqty, char **ptype)
{
  if (!strcmp (var, "default"))
    return 0;

  (*pvals) = ut_realloc_1d (*pvals, 1);

  if (pvalqty)
    (*pvalqty) = 1;

  int i, j, tmp, status, scale;
  double *c = ut_alloc_1d (3);
  char *typetmp = ut_alloc_1d_char (10);
  char *var2 = ut_alloc_1d_char (strlen (var) + 1);
  double **bbox = ut_alloc_2d (3, 2);

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
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      (*pvals)[0] = Tess.SeedCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      (*pvals)[0] = Tess.SeedCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      (*pvals)[0] = Tess.SeedCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "w"))
    {
      (*pvals)[0] = Tess.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "poly_shown") && Tess.Dim == 3)
    {
      (*pvals)[0] = (showpoly) ? showpoly[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "face_shown") && Tess.Dim == 2)
    {
      (*pvals)[0] = (showface) ? showface[id] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "e"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ol_q_e (Tess.CellOri[id], *pvals);
      strcpy (typetmp, "%f");
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
	(*pvals)[0] = id;
      else
	(*pvals)[0] = Tess.CellId ? Tess.CellId[id] : id;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_poly_centroid (Tess, id, c);
      (*pvals)[0] = c[0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_poly_centroid (Tess, id, c);
      (*pvals)[0] = c[1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_poly_centroid (Tess, id, c);
      (*pvals)[0] = c[2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "w"))
    {
      (*pvals)[0] = Tess.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "xmin"))
    {
      neut_tess_poly_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[0][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "xmax"))
    {
      neut_tess_poly_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[0][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "ymin"))
    {
      neut_tess_poly_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[1][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "ymax"))
    {
      neut_tess_poly_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[1][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "zmin"))
    {
      neut_tess_poly_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[2][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "zmax"))
    {
      neut_tess_poly_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[2][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pvals)[0] = Tess.CellTrue[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pvals)[0] = Tess.CellBody[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "lamid"))
    {
      (*pvals)[0] = Tess.CellLamId? Tess.CellLamId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "modeid"))
    {
      (*pvals)[0] = Tess.CellModeId? Tess.CellModeId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = 0;
      for (i = 1; i <= Tess.PolyFaceQty[id]; i++)
	if (Tess.FaceState[Tess.PolyFaceNb[id][i]] != 0)
	{
	  (*pvals)[0] = 1;
	  break;
	}
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vol") || !strcmp (var2, "size"))
    {
      neut_tess_poly_volume (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "diameq"))
    {
      neut_tess_poly_diameq (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "radeq"))
    {
      neut_tess_poly_radeq (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "area"))
    {
      neut_tess_poly_area (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "sphericity"))
    {
      neut_tess_poly_sphericity (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "convexity"))
    {
      neut_tess_poly_convexity (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "dihangles"))
    {
      neut_tess_poly_dihangles (Tess, id, pvals, pvalqty);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "dihangleav"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_mean (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemin"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_min (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemax"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles (Tess, id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_max (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "vernb"))
    {
      neut_tess_poly_vers (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "edgenb"))
    {
      neut_tess_poly_edges (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pvals)[0] = Tess.PolyFaceQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strncmp (var2, "scaleid(", 8))
    {
      sscanf (var2, "scaleid(%d)", &scale);
      (*pvals)[0] = (scale <= Tess.ScaleQty && Tess.ScaleCellId) ? Tess.ScaleCellId[id][scale] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "faces") || !strcmp (var2, "facelist"))
    {
      (*pvalqty) = Tess.PolyFaceQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, Tess.PolyFaceNb[id] + 1);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "faceareas") || !strcmp (var2, "facearealist"))
    {
      (*pvalqty) = Tess.PolyFaceQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < *pvalqty; i++)
        neut_tess_face_area (Tess, Tess.PolyFaceNb[id][i + 1], (*pvals) + i);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "faceeqs") || !strcmp (var2, "faceeqlist"))
    {
      (*pvalqty) = 4 * Tess.PolyFaceQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < Tess.PolyFaceQty[id]; i++)
        ut_array_1d_memcpy ((*pvals) + 4 * i, 4, Tess.FaceEq[Tess.PolyFaceNb[id][i + 1]]);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "edges") || !strcmp (var2, "edgelist"))
    {
      int *edges = NULL;
      neut_tess_poly_edges (Tess, id, &edges, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, edges);
      strcpy (typetmp, "%d");
      ut_free_1d_int (edges);
    }
    else if (!strcmp (var2, "vers") || !strcmp (var2, "verlist"))
    {
      int *vers = NULL;
      neut_tess_poly_vers (Tess, id, &vers, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, vers);
      strcpy (typetmp, "%d");
      ut_free_1d_int (vers);
    }
    else if (!strcmp (var2, "npolys") || !strcmp (var2, "ncells"))
    {
      if (Tess.Dim == 3)
      {
        int *npolys = NULL;
        neut_tess_poly_neighpoly (Tess, id, &npolys, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
        ut_array_1d_memcpy_fromint (*pvals, *pvalqty, npolys);
        strcpy (typetmp, "%d");
        ut_free_1d_int (npolys);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "npolynb") || !strcmp (var2, "neighnb"))
    {
      int tmp;
      neut_tess_poly_neighpoly (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "nseeds") || !strcmp (var2, "npolylist"))
    {
      int *nseeds = NULL;
      neut_tess_poly_neighseeds (Tess, id, &nseeds, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, nseeds);
      strcpy (typetmp, "%d");
      ut_free_1d_int (nseeds);
    }
    else if (!strcmp (var2, "nseednb"))
    {
      int tmp;
      neut_tess_poly_neighseeds (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vercoos") || !strcmp (var2, "vercoolist"))
    {
      int *vers = NULL, verqty;
      neut_tess_poly_vers (Tess, id, &vers, &verqty);
      (*pvalqty) = verqty * 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < verqty; i++)
        ut_array_1d_memcpy ((*pvals) + 3 * i, 3, Tess.VerCoo[vers[i]]);
      strcpy (typetmp, "%f");
      ut_free_1d_int (vers);
    }
    else if (!strcmp (var2, "e"))
    {
      (*pvalqty) = 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ol_q_e (Tess.CellOri[id], *pvals);
      strcpy (typetmp, "%f");
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
	(*pvals)[0] = id;
      else
	(*pvals)[0] = Tess.CellId ? Tess.CellId[id] : id;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_face_centre_x (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_face_centre_y (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_face_centre_z (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "w"))
    {
      (*pvals)[0] = Tess.SeedWeight[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "xmin"))
    {
      neut_tess_face_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[0][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "xmax"))
    {
      neut_tess_face_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[0][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "ymin"))
    {
      neut_tess_face_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[1][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "ymax"))
    {
      neut_tess_face_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[1][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "zmin"))
    {
      neut_tess_face_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[2][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "zmax"))
    {
      neut_tess_face_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[2][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pvals)[0] = neut_tess_face_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pvals)[0] = neut_tess_face_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "lamid"))
    {
      (*pvals)[0] = Tess.CellLamId? Tess.CellLamId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "modeid"))
    {
      (*pvals)[0] = Tess.CellModeId? Tess.CellModeId[id] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "area") || !strcmp (var2, "size"))
    {
      neut_tess_face_area (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "diameq"))
    {
      neut_tess_face_diameq (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "radeq"))
    {
      neut_tess_face_radeq (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "circularity") || !strcmp (var2, "sphericity"))
    {
      neut_tess_face_circularity (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "convexity"))
    {
      neut_tess_face_convexity (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "dihangles"))
    {
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, pvals, pvalqty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihangleav"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_mean (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemin"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_min (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "dihanglemax"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles (Tess, id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_max (tmp, qty);
      strcpy (typetmp, "%f");

      ut_free_1d (tmp);
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = Tess.FaceState[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vernb"))
    {
      (*pvals)[0] = Tess.FaceVerQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pvals)[0] = Tess.FaceVerQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_face_polys (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "ff"))
    {
      (*pvals)[0] = neut_tess_face_ff (Tess, id);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pvals)[0] = Tess.FaceDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "domface"))
    {
      (*pvals)[0] = (Tess.FaceDom[id][0] == 2) ? Tess.FaceDom[id][1] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pvals)[0] = (showpoly) ? showpoly[Tess.FaceDom[id][0]] : 0;
      strcpy (typetmp, "%d");
    }
    else if (!strncmp (var2, "scaleid(", 8))
    {
      sscanf (var2, "scaleid(%d)", &scale);
      (*pvals)[0] = (scale <= Tess.ScaleQty) ? Tess.ScaleCellId[id][scale] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "scale"))
    {
      neut_tess_face_scale (Tess, id, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "theta"))
    {
      if (Tess.Dim != 3)
	(*pvals)[0] = -1;
      else
      {
	int qty, *tmp = NULL;
	neut_tess_face_cells (Tess, id, &tmp, &qty);

	if (qty == 1)
	  (*pvals)[0] = -1;
	else
	  ol_q_q_disori (Tess.CellOri[tmp[0]], Tess.CellOri[tmp[1]],
	      Tess.CellCrySym, *pvals);
	ut_free_1d_int_ (&tmp);
      }
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "polys") || !strcmp (var2, "polylist"))
    {
      int *polys = NULL;
      neut_tess_face_polys (Tess, id, &polys, pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, polys);
      strcpy (typetmp, "%d");
      ut_free_1d_int (polys);
    }
    else if (!strcmp (var2, "edges") || !strcmp (var2, "edgelist"))
    {
      (*pvalqty) = Tess.FaceVerQty[id];
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, Tess.FaceEdgeNb[id] + 1);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "nfaces") || !strcmp (var2, "nfacelist"))
    {
      int *nfaces = NULL;
      neut_tess_face_neighfaces (Tess, id, &nfaces, pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, nfaces);
      strcpy (typetmp, "%d");
      ut_free_1d_int (nfaces);
    }
    else if (!strcmp (var2, "nfacenb") || !strcmp (var2, "neighnb"))
    {
      int qty;
      neut_tess_face_neighfaces (Tess, id, NULL, &qty);
      (*pvals)[0] = qty;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vers") || !strcmp (var2, "verlist"))
    {
      (*pvalqty) = Tess.FaceVerQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, Tess.FaceVerNb[id] + 1);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "vercoos"))
    {
      int *vers = NULL, verqty;
      neut_tess_face_vers (Tess, id, &vers, &verqty);
      (*pvalqty) = verqty * 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < verqty; i++)
        ut_array_1d_memcpy ((*pvals) + 3 * i, 3, Tess.VerCoo[vers[i]]);
      strcpy (typetmp, "%f");
      ut_free_1d_int (vers);
    }
    else if (!strcmp (var2, "e"))
    {
      if (Tess.Dim == 2)
      {
        (*pvalqty) = 3;
        (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
        ol_q_e (Tess.CellOri[id], *pvals);
      }
      else
        (*pvalqty) = 0;

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
	(*pvals)[0] = id;
      else
	(*pvals)[0] = Tess.CellId ? Tess.CellId[id] : id;

      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_edge_centre_x (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_edge_centre_y (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_edge_centre_z (Tess, id, *pvals);
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "xmin"))
    {
      neut_tess_edge_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[0][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "xmax"))
    {
      neut_tess_edge_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[0][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "ymin"))
    {
      neut_tess_edge_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[1][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "ymax"))
    {
      neut_tess_edge_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[1][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "zmin"))
    {
      neut_tess_edge_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[2][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "zmax"))
    {
      neut_tess_edge_bbox (Tess, id, bbox);
      (*pvals)[0] = bbox[2][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pvals)[0] = Tess.EdgeDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "domedge"))
    {
      (*pvals)[0] = (Tess.EdgeDom[id][0] == 1) ? Tess.EdgeDom[id][1] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pvals)[0] = neut_tess_edge_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pvals)[0] = neut_tess_edge_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = Tess.EdgeState[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "length")
	     || !strcmp (var2, "size") || !strcmp (var2, "diameq"))
    {
      (*pvals)[0] = Tess.EdgeLength[id];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "radeq"))
    {
      (*pvals)[0] = Tess.EdgeLength[id] * .5;
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "vernb"))
    {
      (*pvals)[0] = 2;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pvals)[0] = Tess.EdgeFaceQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_edge_polys (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "cyl"))
    {
      (*pvals)[0] = neut_tess_edge_fake (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pvals)[0] = 0;
      if (showpoly)
      {
	int polyqty;
	int *poly = NULL;
	neut_tess_edge_polys (Tess, id, &poly, &polyqty);
	for (j = 0; j < polyqty; j++)
	  if (poly[j] > 0 && showpoly[poly[j]] == 1)
	  {
	    (*pvals)[0] = 1;
	    break;
	  }
	ut_free_1d_int (poly);
      }
    }
    else if (!strcmp (var2, "face_shown"))
    {
      (*pvals)[0] = 0;
      if (showface)
      {
	int face;
	for (j = 0; j < Tess.EdgeFaceQty[id]; j++)
	{
	  face = Tess.EdgeFaceNb[id][j];
	  if (showface[face])
	  {
	    (*pvals)[0] = 1;
	    break;
	  }
	}
      }
    }
    else if (!strcmp (var2, "theta"))
    {
      if (Tess.Dim != 2)
	(*pvals)[0] = -1;
      else
      {
	int qty, *tmp = NULL;
	neut_tess_edge_cells (Tess, id, &tmp, &qty);

	if (qty == 1)
	  (*pvals)[0] = -1;
	else
	  ol_q_q_disori (Tess.CellOri[tmp[0]], Tess.CellOri[tmp[1]],
	      Tess.CellCrySym, *pvals);
	ut_free_1d_int_ (&tmp);
      }
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "domedge"))
    {
      (*pvals)[0] = (Tess.EdgeDom[id][0] == 1) ? Tess.EdgeDom[id][1] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polys") || !strcmp (var2, "polylist"))
    {
      int *polys = NULL;
      neut_tess_edge_polys (Tess, id, &polys, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, polys);
      strcpy (typetmp, "%d");
      ut_free_1d_int (polys);
    }
    else if (!strcmp (var2, "faces") || !strcmp (var2, "facelist"))
    {
      int *faces = NULL;
      neut_tess_edge_faces (Tess, id, &faces, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, faces);
      strcpy (typetmp, "%d");
      ut_free_1d_int (faces);
    }
    else if (!strcmp (var2, "vers") || !strcmp (var2, "verlist"))
    {
      (*pvalqty) = 2;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, Tess.EdgeVerNb[id]);
      strcpy (typetmp, "%d");
    }
    else
      status = -1;
  }
  else if (!strcmp (entity, "ver"))
  {
    status = 0;
    if (!strcmp (var2, "id"))
    {
      (*pvals)[0] = id;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "x"))
    {
      (*pvals)[0] = Tess.VerCoo[id][0];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "y"))
    {
      (*pvals)[0] = Tess.VerCoo[id][1];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "z"))
    {
      (*pvals)[0] = Tess.VerCoo[id][2];
      strcpy (typetmp, "%f");
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pvals)[0] = Tess.VerEdgeQty[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "facenb"))
    {
      neut_tess_ver_faces (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_ver_polys (Tess, id, NULL, &tmp);
      (*pvals)[0] = tmp;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "true"))
    {
      (*pvals)[0] = neut_tess_ver_true (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "body"))
    {
      (*pvals)[0] = neut_tess_ver_body (Tess, id);
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = Tess.VerState[id];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pvals)[0] = Tess.VerDom[id][0];
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "domver"))
    {
      (*pvals)[0] = (Tess.VerDom[id][0] == 0) ? Tess.VerDom[id][1] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pvals)[0] = 0;
      if (showpoly)
      {
	int polyqty;
	int *poly = NULL;
	neut_tess_ver_polys (Tess, id, &poly, &polyqty);
	for (j = 0; j < polyqty; j++)
	  if (poly[j] > 0 && showpoly[poly[j]])
	  {
	    (*pvals)[0] = 1;
	    break;
	  }
	ut_free_1d_int (poly);
      }
    }
    else if (!strcmp (var2, "face_shown"))
    {
      (*pvals)[0] = 0;
      if (showface)
      {
	int faceqty;
	int *face = NULL;
	neut_tess_ver_faces (Tess, id, &face, &faceqty);
	for (j = 0; j < faceqty; j++)
	  if (face[j] > 0 && showface[face[j]])
	  {
	    (*pvals)[0] = 1;
	    break;
	  }
	ut_free_1d_int (face);
      }
    }
    else if (!strcmp (var2, "edge_shown"))
    {
      (*pvals)[0] = 0;
      if (showedge)
      {
	int edge;
	for (j = 0; j < Tess.VerEdgeQty[id]; j++)
	{
	  edge = Tess.VerEdgeNb[id][j];
	  if (showedge[edge])
	  {
	    (*pvals)[0] = 1;
	    break;
	  }
	}
      }
    }
    else if (!strcmp (var2, "domver"))
    {
      (*pvals)[0] = (Tess.VerDom[id][0] == 0) ? Tess.VerDom[id][1] : -1;
      strcpy (typetmp, "%d");
    }
    else if (!strcmp (var2, "polys") || !strcmp (var2, "polylist"))
    {
      int *polys = NULL;
      neut_tess_ver_polys (Tess, id, &polys, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, polys);
      strcpy (typetmp, "%d");
      ut_free_1d_int (polys);
    }
    else if (!strcmp (var2, "faces") || !strcmp (var2, "facelist"))
    {
      int *faces = NULL;
      neut_tess_ver_faces (Tess, id, &faces, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, faces);
      strcpy (typetmp, "%d");
      ut_free_1d_int (faces);
    }
    else if (!strcmp (var2, "edges") || !strcmp (var2, "edgelist"))
    {
      int *edges = NULL;
      neut_tess_ver_edges (Tess, id, &edges, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (*pvals, *pvalqty, edges);
      strcpy (typetmp, "%d");
      ut_free_1d_int (edges);
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
  ut_free_2d (bbox, 3);
  ut_free_1d_char (typetmp);
  ut_free_1d_char (var2);

  return status;
}

int
neut_tess_var_val_one (struct TESS Tess,
		       int *showedge,
		       int *showface, int *showpoly,
		       char *entity, int id, char *var,
		       double *pvals, char **ptype)
{
  double *tmp = NULL;
  int qty;

  neut_tess_var_val (Tess, showedge, showface, showpoly, entity, id, var, &tmp,
                     &qty, ptype);

  (*pvals) = tmp[0];

  ut_free_1d (tmp);

  return qty == 1 ? 0 : -1;
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
neut_tess_expr_celllist (struct TESS Tess, char *expr, int **pcells,
			 int *pcellqty)
{
  int status = -1;

  if (Tess.Dim == 3)
    status = neut_tess_expr_polylist (Tess, expr, pcells, pcellqty);
  else if (Tess.Dim == 2)
    status = neut_tess_expr_facelist (Tess, expr, pcells, pcellqty);
  else
    abort ();

  return status;
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
	  neut_tess_var_val_one (Tess, NULL, NULL, NULL, "poly", i,
			         vars[j], vals + j, NULL);

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

  // for consistency with neut_tess_expr_facelist
  ut_array_1d_int_sort (*ppoly, *ppolyqty);

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
	  neut_tess_var_val_one (Tess, NULL, NULL, NULL, "face", i,
			         vars[j], vals + j, NULL);

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

  // this is needed e.g. in nem_meshing_2D
  ut_array_1d_int_sort (*pface, *pfaceqty);

  return 0;
}

void
neut_tess_entity_expr_val (struct TESS Tess, char *entity, char *expr,
			   double *val)
{
  int j, k, status, entityqty, varqty;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;

  neut_tess_entity_qty (Tess, entity, &entityqty);
  neut_tess_var_list (Tess, entity, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  if (ut_string_filename (expr))
  {
    file = ut_file_open (expr, "R");
    ut_array_1d_fscanf (file, val + 1, entityqty);
    ut_file_close (file, expr, "R");
  }
  else
  {
    for (j = 1; j <= entityqty; j++)
    {
      for (k = 0; k < varqty; k++)
        if (strstr (expr, vars[k]))
        {
          if (!strcmp (vars[k], "default"))
            vals[k] = val[j];

          neut_tess_var_val_one (Tess, NULL, NULL, NULL, entity, j, vars[k],
                                 vals + k, NULL);
        }

      status = ut_math_eval (expr, varqty, vars, vals, val + j);
      if (status == -1)
        abort ();
    }
  }

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

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
neut_tess_expr_vars_vals (struct TESS Tess, char* expr, int *showedge,
                          int *showface, int *showpoly, char *entity,
			  int id, char ***pvars, double **pvals, char ***ptypes,
			  int *pvarqty)
{
  int i;

  ut_math_vars (expr, pvars, pvarqty);

  (*pvals) = ut_alloc_1d (*pvarqty);

  if (ptypes)
    (*ptypes) = ut_alloc_1d_pchar (*pvarqty);

  for (i = 0; i < *pvarqty; i++)
    neut_tess_var_val_one (Tess, showedge, showface, showpoly, entity, id,
	                   (*pvars)[i], (*pvals) + i, ptypes? (*ptypes) + i : NULL);

  return 0;
}

int
neut_tess_isvoid (struct TESS Tess)
{
  return (Tess.CellQty == 0);
}

int
neut_tess_cell_isvoid (struct TESS Tess, int cell)
{
  return (Tess.Dim == 3 && Tess.PolyFaceQty[cell] == 0) || (Tess.Dim == 2 && Tess.FaceVerQty[cell] == 0);
}
