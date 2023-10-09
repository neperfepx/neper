/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
    ut_print_neperbug ();

  return;
}

void
neut_tess_var_dim (int dim, char *entity, char *var, int *pdim)
{
  if (!strcmp (entity, "seed"))
    (*pdim) = 0;
  else if (!strcmp (entity, "poly") || (!strcmp (entity, "cell") && dim == 3)
           || (!strcmp (entity, "crystal") && dim == 3))
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
  else if (!strcmp (entity, "face") || (!strcmp (entity, "cell") && dim == 2))
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
    if (!strcmp (var, "length") || !strcmp (var, "size")
        || !strcmp (var, "diameq") || !strcmp (var, "radeq"))
      (*pdim) = 1;
    else
      (*pdim) = 0;
  }
  else if (!strcmp (entity, "ver"))
    (*pdim) = 0;
  else if (!strcmp (entity, "point"))
    (*pdim) = 0;
  else
    ut_print_neperbug ();

  return;
}

int
neut_tess_var_val (struct TESS *pTess,
                   int *showedge, int *showface,
                   int *showpoly, char *entity, int id, char *var,
                   double **pvals, int *pvalqty, char **ptype)
{
  if (!strcmp (var, "default"))
    return 0;

  (*pvals) = ut_realloc_1d (*pvals, 1);

  if (pvalqty)
    (*pvalqty) = 1;

  int i, j, tmp, status, scale;
  double *c = ut_alloc_1d (3);
  char *typetmp = NULL;
  char *var2 = ut_alloc_1d_char (strlen (var) + 1);
  double **bbox = ut_alloc_2d (3, 2);

  ut_string_string ("%f", &typetmp);

  strcpy (var2, var);
  if (strstr (var2, "cell"))
  {
    if ((*pTess).Dim == 3)
      ut_string_fnrs (var2, "cell", "poly", 1);
    else if ((*pTess).Dim == 2)
      ut_string_fnrs (var2, "cell", "face", 1);
  }

  status = -1;
  if (!strcmp (entity, "tess"))
  {
    status = 0;

    if (!strcmp (var2, "dim"))
    {
      (*pvals)[0] = (*pTess).Dim;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "vernb"))
    {
      (*pvals)[0] = (*pTess).VerQty;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pvals)[0] = (*pTess).EdgeQty;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pvals)[0] = (*pTess).FaceQty;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "polynb"))
    {
      (*pvals)[0] = (*pTess).PolyQty;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "seednb"))
    {
      (*pvals)[0] = (*pTess).SeedQty;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_centre ((*pTess), c);
      (*pvals)[0] = c[0];
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_centre ((*pTess), c);
      (*pvals)[0] = c[1];
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_centre ((*pTess), c);
      (*pvals)[0] = c[2];
    }
    else if (!strcmp (var2, "coo"))
    {
      neut_tess_centre ((*pTess), *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var2, "size"))
      neut_tess_size ((*pTess), *pvals);
    else if (!strcmp (var2, "area"))
      neut_tess_area ((*pTess), *pvals);
    else if (!strcmp (var2, "volume"))
      neut_tess_volume ((*pTess), *pvals);
    else if (!strcmp (var2, "diameq"))
      neut_tess_diameq ((*pTess), *pvals);
    else if (!strcmp (var2, "radeq"))
      neut_tess_radeq ((*pTess), *pvals);
    else
      status = -1;
  }

  else if (!strcmp (entity, "seed"))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      (*pvals)[0] = id;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "x"))
      (*pvals)[0] = (*pTess).SeedCoo[id][0];
    else if (!strcmp (var2, "y"))
      (*pvals)[0] = (*pTess).SeedCoo[id][1];
    else if (!strcmp (var2, "z"))
      (*pvals)[0] = (*pTess).SeedCoo[id][2];
    else if (!strcmp (var2, "coo"))
    {
      ut_array_1d_memcpy ((*pTess).SeedCoo[id], 3, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var2, "w"))
      (*pvals)[0] = (*pTess).SeedWeight[id];
    else if (!strcmp (var2, "poly_shown") && (*pTess).Dim == 3)
    {
      (*pvals)[0] = (showpoly) ? showpoly[id] : 0;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "face_shown") && (*pTess).Dim == 2)
    {
      (*pvals)[0] = (showface) ? showface[id] : 0;
      ut_string_string ("%d", &typetmp);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "poly")
           || (!strcmp (entity, "cell") && (*pTess).Dim == 3)
           || (!strcmp (entity, "crystal") && (*pTess).Dim == 3))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      if (!strcmp (entity, "poly"))
        (*pvals)[0] = id;
      else
        (*pvals)[0] = neut_tess_cell_id ((*pTess), id);

      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "x"))
    {
      neut_tess_poly_centroid ((*pTess), id, c);
      (*pvals)[0] = c[0];
    }
    else if (!strcmp (var2, "y"))
    {
      neut_tess_poly_centroid ((*pTess), id, c);
      (*pvals)[0] = c[1];
    }
    else if (!strcmp (var2, "z"))
    {
      neut_tess_poly_centroid ((*pTess), id, c);
      (*pvals)[0] = c[2];
    }
    else if (!strcmp (var2, "coo"))
    {
      neut_tess_poly_centroid ((*pTess), id, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var2, "w"))
      (*pvals)[0] = (*pTess).SeedWeight[id];
    else if (!strcmp (var2, "xmin"))
    {
      neut_tess_poly_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[0][0];
    }
    else if (!strcmp (var2, "xmax"))
    {
      neut_tess_poly_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[0][1];
    }
    else if (!strcmp (var2, "ymin"))
    {
      neut_tess_poly_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[1][0];
    }
    else if (!strcmp (var2, "ymax"))
    {
      neut_tess_poly_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[1][1];
    }
    else if (!strcmp (var2, "zmin"))
    {
      neut_tess_poly_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[2][0];
    }
    else if (!strcmp (var2, "zmax"))
    {
      neut_tess_poly_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[2][1];
    }
    else if (!strncmp (var2, "body", 4))
    {
      (*pvals)[0] = neut_tess_cell_body (pTess, var2, id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "lam"))
    {
      (*pvals)[0] = (*pTess).CellLamId ? (*pTess).CellLamId[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "group"))
    {
      (*pvals)[0] = (*pTess).CellGroup ? (*pTess).CellGroup[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "mode"))
    {
      (*pvals)[0] = (*pTess).CellModeId ? (*pTess).CellModeId[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = 0;
      for (i = 1; i <= (*pTess).PolyFaceQty[id]; i++)
        if ((*pTess).FaceState[(*pTess).PolyFaceNb[id][i]] != 0)
        {
          (*pvals)[0] = 1;
          break;
        }
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "vol") || !strcmp (var2, "size"))
      neut_tess_poly_volume ((*pTess), id, *pvals);
    else if (!strcmp (var2, "diameq"))
      neut_tess_cell_diameq ((*pTess), id, *pvals);
    else if (!strcmp (var2, "avdiameq"))
      neut_tess_cellavdiameq ((*pTess), (*pTess).CellQty, *pvals);
    else if (!strcmp (var2, "radeq"))
      neut_tess_poly_radeq ((*pTess), id, *pvals);
    else if (!strcmp (var2, "avradeq"))
      neut_tess_cellavradeq ((*pTess), (*pTess).CellQty, *pvals);
    else if (!strcmp (var2, "area"))
      neut_tess_poly_area ((*pTess), id, *pvals);
    else if (!strcmp (var2, "sphericity"))
      neut_tess_poly_sphericity ((*pTess), id, *pvals);
    else if (!strcmp (var2, "convexity"))
      neut_tess_poly_convexity ((*pTess), id, *pvals);
    else if (!strcmp (var2, "dihangles"))
      neut_tess_poly_dihangles ((*pTess), id, pvals, pvalqty);
    else if (!strcmp (var2, "dihangleav"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles ((*pTess), id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_mean (tmp, qty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "dihanglemin"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles ((*pTess), id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_min (tmp, qty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "dihanglemax"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_poly_dihangles ((*pTess), id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_max (tmp, qty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "vernb"))
    {
      neut_tess_poly_vers ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "edgenb"))
    {
      neut_tess_poly_edges ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pvals)[0] = (*pTess).PolyFaceQty[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strncmp (var2, "scaleid(", 8))
    {
      sscanf (var2, "scaleid(%d)", &scale);
      (*pvals)[0] = (scale <= (*pTess).ScaleQty
                     && (*pTess).ScaleCellId) ? (*pTess).ScaleCellId[id][scale] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "scale"))
    {
      neut_tess_cell_scale (*pTess, id, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "faces") || !strcmp (var2, "facelist"))
    {
      (*pvalqty) = (*pTess).PolyFaceQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint ((*pTess).PolyFaceNb[id] + 1, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "faceareas") || !strcmp (var2, "facearealist"))
    {
      (*pvalqty) = (*pTess).PolyFaceQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < *pvalqty; i++)
        neut_tess_face_area ((*pTess), (*pTess).PolyFaceNb[id][i + 1], (*pvals) + i);
    }
    else if (!strcmp (var2, "faceeqs") || !strcmp (var2, "faceeqlist"))
    {
      (*pvalqty) = 4 * (*pTess).PolyFaceQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < (*pTess).PolyFaceQty[id]; i++)
        ut_array_1d_memcpy ((*pTess).FaceEq[(*pTess).PolyFaceNb[id][i + 1]], 4,
                            (*pvals) + 4 * i);
    }
    else if (!strcmp (var2, "edges") || !strcmp (var2, "edgelist"))
    {
      int *edges = NULL;
      neut_tess_poly_edges ((*pTess), id, &edges, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (edges, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&edges);
    }
    else if (!strcmp (var2, "vers") || !strcmp (var2, "verlist"))
    {
      int *vers = NULL;
      neut_tess_poly_vers ((*pTess), id, &vers, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (vers, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&vers);
    }
    else if (!strcmp (var2, "npolys_unsort"))
    {
      if ((*pTess).Dim == 3)
      {
        int *npolys = NULL;
        neut_tess_poly_neighpolys_unsort ((*pTess), id, &npolys, pvalqty);
        (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
        ut_array_1d_memcpy_fromint (npolys, *pvalqty, *pvals);
        ut_string_string ("%d", &typetmp);
        ut_free_1d_int (&npolys);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "npolys"))
    {
      if ((*pTess).Dim == 3)
      {
        int *npolys = NULL;
        neut_tess_poly_neighpolys ((*pTess), id, &npolys, pvalqty);
        (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
        ut_array_1d_memcpy_fromint (npolys, *pvalqty, *pvals);
        ut_string_string ("%d", &typetmp);
        ut_free_1d_int (&npolys);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "npolys_samedomain"))
    {
      if ((*pTess).Dim == 3)
      {
        int *npolys = NULL;
        neut_tess_poly_neighpolys_samedomain ((*pTess), id, &npolys, pvalqty);
        (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
        ut_array_1d_memcpy_fromint (npolys, *pvalqty, *pvals);
        ut_string_string ("%d", &typetmp);
        ut_free_1d_int (&npolys);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "npolynb_samedomain"))
    {
      if ((*pTess).Dim == 3)
      {
        int tmp;
        neut_tess_poly_neighpolys_samedomain ((*pTess), id, NULL, &tmp);
        (*pvals)[0] = tmp;
        ut_string_string ("%d", &typetmp);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "npolynb") || !strcmp (var2, "neighnb"))
    {
      int tmp;
      neut_tess_poly_neighpolys ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "nseeds"))
    {
      int *nseeds = NULL;
      neut_tess_poly_neighseeds ((*pTess), id, &nseeds, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (nseeds, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&nseeds);
    }
    else if (!strcmp (var2, "nseednb"))
    {
      int tmp;
      neut_tess_poly_neighseeds ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "vercoos") || !strcmp (var2, "vercoolist"))
    {
      int *vers = NULL, verqty;
      neut_tess_poly_vers ((*pTess), id, &vers, &verqty);
      (*pvalqty) = verqty * 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < verqty; i++)
        ut_array_1d_memcpy ((*pTess).VerCoo[vers[i]], 3, (*pvals) + 3 * i);
      ut_free_1d_int (&vers);
    }
    else if (!strncmp (var, "fiber", 5))
    {
      double theta;
      double *dirc = ut_alloc_1d (3);
      double *dirs = ut_alloc_1d (3);

      neut_ori_fiber_sscanf (var, dirc, dirs, &theta, NULL);
      (*pvals)[0] = neut_ori_fiber_in ((*pTess).CellOri[id], (*pTess).CellCrySym, dirc, dirs, theta);
      strcpy (typetmp, "%d");

      ut_free_1d (&dirc);
      ut_free_1d (&dirs);
    }
    else if (neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_ori_des_ori ((*pTess).CellOri[id], var, *pvals);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "face")
           || (!strcmp (entity, "cell") && (*pTess).Dim == 2)
           || (!strcmp (entity, "crystal") && (*pTess).Dim == 2))
  {
    status = 0;

    if (!strcmp (var2, "id"))
    {
      if (!strcmp (entity, "face"))
        (*pvals)[0] = id;
      else
        (*pvals)[0] = neut_tess_cell_id ((*pTess), id);

      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "x"))
      neut_tess_face_centre_x ((*pTess), id, *pvals);
    else if (!strcmp (var2, "y"))
      neut_tess_face_centre_y ((*pTess), id, *pvals);
    else if (!strcmp (var2, "z"))
      neut_tess_face_centre_z ((*pTess), id, *pvals);
    else if (!strcmp (var2, "coo"))
    {
      neut_tess_face_centre ((*pTess), id, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var2, "w"))
      (*pvals)[0] = (*pTess).SeedWeight[id];
    else if (!strcmp (var2, "xmin"))
    {
      neut_tess_face_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[0][0];
    }
    else if (!strcmp (var2, "xmax"))
    {
      neut_tess_face_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[0][1];
    }
    else if (!strcmp (var2, "ymin"))
    {
      neut_tess_face_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[1][0];
    }
    else if (!strcmp (var2, "ymax"))
    {
      neut_tess_face_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[1][1];
    }
    else if (!strcmp (var2, "zmin"))
    {
      neut_tess_face_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[2][0];
    }
    else if (!strcmp (var2, "zmax"))
    {
      neut_tess_face_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[2][1];
    }
    else if (!strncmp (var2, "body", 4))
    {
      (*pvals)[0] = neut_tess_face_body (pTess, var2, id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "lam"))
    {
      (*pvals)[0] = (*pTess).CellLamId ? (*pTess).CellLamId[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "mode"))
    {
      (*pvals)[0] = (*pTess).CellModeId ? (*pTess).CellModeId[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "group"))
    {
      (*pvals)[0] = (*pTess).CellGroup ? (*pTess).CellGroup[id] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "area") || !strcmp (var2, "size"))
      neut_tess_face_area ((*pTess), id, *pvals);
    else if (!strcmp (var2, "diameq"))
      neut_tess_face_diameq ((*pTess), id, *pvals);
    else if (!strcmp (var2, "radeq"))
      neut_tess_face_radeq ((*pTess), id, *pvals);
    else if (!strcmp (var2, "circularity") || !strcmp (var2, "sphericity"))
      neut_tess_face_circularity ((*pTess), id, *pvals);
    else if (!strcmp (var2, "convexity"))
      neut_tess_face_convexity ((*pTess), id, *pvals);
    else if (!strcmp (var2, "dihangles"))
    {
      double *tmp = NULL;
      neut_tess_face_dihangles ((*pTess), id, pvals, pvalqty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "dihangleav"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles ((*pTess), id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_mean (tmp, qty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "dihanglemin"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles ((*pTess), id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_min (tmp, qty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "dihanglemax"))
    {
      int qty;
      double *tmp = NULL;
      neut_tess_face_dihangles ((*pTess), id, &tmp, &qty);

      (*pvals)[0] = ut_array_1d_max (tmp, qty);

      ut_free_1d (&tmp);
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = (*pTess).FaceState[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "vernb"))
    {
      (*pvals)[0] = (*pTess).FaceVerQty[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pvals)[0] = (*pTess).FaceVerQty[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_face_polys ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "ff"))
    {
      (*pvals)[0] = neut_tess_face_ff ((*pTess), id);
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pvals)[0] = (*pTess).FaceDom[id][0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "domface"))
    {
      (*pvals)[0] = ((*pTess).FaceDom[id][0] == 2) ? (*pTess).FaceDom[id][1] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pvals)[0] = (showpoly) ? showpoly[(*pTess).FaceDom[id][0]] : 0;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strncmp (var2, "scaleid(", 8))
    {
      sscanf (var2, "scaleid(%d)", &scale);
      (*pvals)[0] =
        (scale <= (*pTess).ScaleQty) ? (*pTess).ScaleCellId[id][scale] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "scale"))
    {
      neut_tess_cell_scale (*pTess, id, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "scale"))
    {
      neut_tess_face_scale ((*pTess), id, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "theta"))
    {
      int qty, *tmp = NULL;
      neut_tess_face_cells ((*pTess), id, &tmp, &qty);

      if (qty == 1)
        (*pvals)[0] = -1;
      else
        ol_q_q_disori ((*pTess).CellOri[tmp[0]], (*pTess).CellOri[tmp[1]],
                       (*pTess).CellCrySym, *pvals);
      ut_free_1d_int (&tmp);
    }
    else if (!strcmp (var2, "polys") || !strcmp (var2, "polylist"))
    {
      int *polys = NULL;
      neut_tess_face_polys ((*pTess), id, &polys, pvalqty);
      ut_array_1d_memcpy_fromint (polys, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&polys);
    }
    else if (!strcmp (var2, "edges") || !strcmp (var2, "edgelist"))
    {
      (*pvalqty) = (*pTess).FaceVerQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint ((*pTess).FaceEdgeNb[id] + 1, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "nfaces") || !strcmp (var2, "nfacelist"))
    {
      int *nfaces = NULL;
      neut_tess_face_neighfaces ((*pTess), id, &nfaces, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (nfaces, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&nfaces);
    }
    else if (!strcmp (var2, "nfacenb") || !strcmp (var2, "neighnb"))
    {
      int qty;
      neut_tess_face_neighfaces ((*pTess), id, NULL, &qty);
      (*pvals)[0] = qty;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "nfaces_samedomain"))
    {
      if ((*pTess).Dim == 2)
      {
        int *nfaces = NULL;
        neut_tess_face_neighfaces_samedomain ((*pTess), id, &nfaces, pvalqty);
        (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
        ut_array_1d_memcpy_fromint (nfaces, *pvalqty, *pvals);
        ut_string_string ("%d", &typetmp);
        ut_free_1d_int (&nfaces);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "nfacenb_samedomain"))
    {
      if ((*pTess).Dim == 2)
      {
        int tmp;
        neut_tess_face_neighfaces_samedomain ((*pTess), id, NULL, &tmp);
        (*pvals)[0] = tmp;
        ut_string_string ("%d", &typetmp);
      }
      else
        (*pvalqty) = 0;
    }
    else if (!strcmp (var2, "vers") || !strcmp (var2, "verlist"))
    {
      (*pvalqty) = (*pTess).FaceVerQty[id];
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint ((*pTess).FaceVerNb[id] + 1, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "vercoos"))
    {
      int *vers = NULL, verqty;
      neut_tess_face_vers ((*pTess), id, &vers, &verqty);
      (*pvalqty) = verqty * 3;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      for (i = 0; i < verqty; i++)
        ut_array_1d_memcpy ((*pTess).VerCoo[vers[i]], 3, (*pvals) + 3 * i);
      ut_free_1d_int (&vers);
    }
    else if (!strcmp (var2, "per"))
    {
      (*pvals)[0] = neut_tess_face_isper ((*pTess), id);
      ut_string_string ("%d", &typetmp);
    }
    else if (neut_ori_des_isvalid (var)) // orientation descriptor
    {
      (*pvalqty) = ol_des_size (var);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      neut_ori_des_ori ((*pTess).CellOri[id], var, *pvals);
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
        (*pvals)[0] = neut_tess_cell_id ((*pTess), id);

      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "x"))
      neut_tess_edge_centre_x ((*pTess), id, *pvals);
    else if (!strcmp (var2, "y"))
      neut_tess_edge_centre_y ((*pTess), id, *pvals);
    else if (!strcmp (var2, "z"))
      neut_tess_edge_centre_z ((*pTess), id, *pvals);
    else if (!strcmp (var2, "coo"))
    {
      neut_tess_edge_centre ((*pTess), id, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var2, "xmin"))
    {
      neut_tess_edge_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[0][0];
    }
    else if (!strcmp (var2, "xmax"))
    {
      neut_tess_edge_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[0][1];
    }
    else if (!strcmp (var2, "ymin"))
    {
      neut_tess_edge_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[1][0];
    }
    else if (!strcmp (var2, "ymax"))
    {
      neut_tess_edge_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[1][1];
    }
    else if (!strcmp (var2, "zmin"))
    {
      neut_tess_edge_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[2][0];
    }
    else if (!strcmp (var2, "zmax"))
    {
      neut_tess_edge_bbox ((*pTess), id, bbox);
      (*pvals)[0] = bbox[2][1];
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pvals)[0] = (*pTess).EdgeDom[id][0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "domedge"))
    {
      (*pvals)[0] = ((*pTess).EdgeDom[id][0] == 1) ? (*pTess).EdgeDom[id][1] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strncmp (var2, "body", 4))
    {
      (*pvals)[0] = neut_tess_edge_body (pTess, var2, id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = (*pTess).EdgeState[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "length") || !strcmp (var2, "size")
             || !strcmp (var2, "diameq"))
      (*pvals)[0] = (*pTess).EdgeLength[id];
    else if (!strcmp (var2, "radeq"))
      (*pvals)[0] = (*pTess).EdgeLength[id] * .5;
    else if (!strcmp (var2, "vernb"))
    {
      (*pvals)[0] = 2;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "facenb"))
    {
      (*pvals)[0] = (*pTess).EdgeFaceQty[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_edge_polys ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "cyl"))
    {
      (*pvals)[0] = neut_tess_edge_fake ((*pTess), id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pvals)[0] = 0;
      if (showpoly)
      {
        int polyqty;
        int *poly = NULL;
        neut_tess_edge_polys ((*pTess), id, &poly, &polyqty);
        for (j = 0; j < polyqty; j++)
          if (poly[j] > 0 && showpoly[poly[j]] == 1)
          {
            (*pvals)[0] = 1;
            break;
          }
        ut_free_1d_int (&poly);
      }
    }
    else if (!strcmp (var2, "face_shown"))
    {
      (*pvals)[0] = 0;
      if (showface)
      {
        int face;
        for (j = 0; j < (*pTess).EdgeFaceQty[id]; j++)
        {
          face = (*pTess).EdgeFaceNb[id][j];
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
      if ((*pTess).Dim != 2)
        (*pvals)[0] = -1;
      else
      {
        int qty, *tmp = NULL;
        neut_tess_edge_cells ((*pTess), id, &tmp, &qty);

        if (qty == 1)
          (*pvals)[0] = -1;
        else
          ol_q_q_disori ((*pTess).CellOri[tmp[0]], (*pTess).CellOri[tmp[1]],
                         (*pTess).CellCrySym, *pvals);
        ut_free_1d_int (&tmp);
      }
    }
    else if (!strcmp (var2, "domedge"))
    {
      (*pvals)[0] = ((*pTess).EdgeDom[id][0] == 1) ? (*pTess).EdgeDom[id][1] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "polys") || !strcmp (var2, "polylist"))
    {
      int *polys = NULL;
      neut_tess_edge_polys ((*pTess), id, &polys, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (polys, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&polys);
    }
    else if (!strcmp (var2, "faces") || !strcmp (var2, "facelist"))
    {
      int *faces = NULL;
      neut_tess_edge_faces ((*pTess), id, &faces, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (faces, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&faces);
    }
    else if (!strcmp (var2, "vers") || !strcmp (var2, "verlist"))
    {
      (*pvalqty) = 2;
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint ((*pTess).EdgeVerNb[id], *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "per"))
    {
      (*pvals)[0] = neut_tess_edge_isper ((*pTess), id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "scale"))
    {
      neut_tess_edge_scale ((*pTess), id, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
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
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "x"))
      (*pvals)[0] = (*pTess).VerCoo[id][0];
    else if (!strcmp (var2, "y"))
      (*pvals)[0] = (*pTess).VerCoo[id][1];
    else if (!strcmp (var2, "z"))
      (*pvals)[0] = (*pTess).VerCoo[id][2];
    else if (!strcmp (var2, "coo"))
    {
      ut_array_1d_memcpy ((*pTess).VerCoo[id], 3, *pvals);
      (*pvalqty) = 3;
    }
    else if (!strcmp (var2, "edgenb"))
    {
      (*pvals)[0] = (*pTess).VerEdgeQty[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "facenb"))
    {
      neut_tess_ver_faces ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "polynb"))
    {
      neut_tess_ver_polys ((*pTess), id, NULL, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strncmp (var2, "body", 4))
    {
      (*pvals)[0] = neut_tess_ver_body (pTess, var2, id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "state"))
    {
      (*pvals)[0] = (*pTess).VerState[id];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "domtype"))
    {
      (*pvals)[0] = (*pTess).VerDom[id][0];
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "domver"))
    {
      (*pvals)[0] = ((*pTess).VerDom[id][0] == 0) ? (*pTess).VerDom[id][1] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "poly_shown"))
    {
      (*pvals)[0] = 0;
      if (showpoly)
      {
        int polyqty;
        int *poly = NULL;
        neut_tess_ver_polys ((*pTess), id, &poly, &polyqty);
        for (j = 0; j < polyqty; j++)
          if (poly[j] > 0 && showpoly[poly[j]])
          {
            (*pvals)[0] = 1;
            break;
          }
        ut_free_1d_int (&poly);
      }
    }
    else if (!strcmp (var2, "face_shown"))
    {
      (*pvals)[0] = 0;
      if (showface)
      {
        int faceqty;
        int *face = NULL;
        neut_tess_ver_faces ((*pTess), id, &face, &faceqty);
        for (j = 0; j < faceqty; j++)
          if (face[j] > 0 && showface[face[j]])
          {
            (*pvals)[0] = 1;
            break;
          }
        ut_free_1d_int (&face);
      }
    }
    else if (!strcmp (var2, "edge_shown"))
    {
      (*pvals)[0] = 0;
      if (showedge)
      {
        int edge;
        for (j = 0; j < (*pTess).VerEdgeQty[id]; j++)
        {
          edge = (*pTess).VerEdgeNb[id][j];
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
      (*pvals)[0] = ((*pTess).VerDom[id][0] == 0) ? (*pTess).VerDom[id][1] : -1;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "polys") || !strcmp (var2, "polylist"))
    {
      int *polys = NULL;
      neut_tess_ver_polys ((*pTess), id, &polys, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (polys, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&polys);
    }
    else if (!strcmp (var2, "faces") || !strcmp (var2, "facelist"))
    {
      int *faces = NULL;
      neut_tess_ver_faces ((*pTess), id, &faces, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (faces, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&faces);
    }
    else if (!strcmp (var2, "edges") || !strcmp (var2, "edgelist"))
    {
      int *edges = NULL;
      neut_tess_ver_edges ((*pTess), id, &edges, pvalqty);
      (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
      ut_array_1d_memcpy_fromint (edges, *pvalqty, *pvals);
      ut_string_string ("%d", &typetmp);
      ut_free_1d_int (&edges);
    }
    else if (!strcmp (var2, "per"))
    {
      (*pvals)[0] = neut_tess_ver_isper ((*pTess), id);
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var2, "scale"))
    {
      neut_tess_ver_scale ((*pTess), id, &tmp);
      (*pvals)[0] = tmp;
      ut_string_string ("%d", &typetmp);
    }
    else
      status = -1;
  }

  else if (!strcmp (entity, "group"))
  {
    status = 0;
    if (!strcmp (var, "id"))
    {
      (*pvals)[0] = id;
      ut_string_string ("%d", &typetmp);
    }
    else if (!strcmp (var, "vol"))
      neut_tess_group_vol ((*pTess), id, *pvals);
    else if (!strcmp (var, "area"))
      neut_tess_group_area ((*pTess), id, *pvals);
    else if (!strcmp (var, "size"))
      neut_tess_group_size ((*pTess), id, *pvals);
    else
      status = -1;
  }

  else
    ut_print_message (2, 0, "Failed to process expression `%s'.\n", entity);

  if (status == -1 && (*pTess).pSim)
    status = neut_sim_entity_id_res_val (*((*pTess).pSim), entity, id, var, *pvals);

  if (ptype)
    ut_string_string (typetmp, ptype);

  ut_free_1d (&c);
  ut_free_2d (&bbox, 3);
  ut_free_1d_char (&typetmp);
  ut_free_1d_char (&var2);

  if (status)
    (*pvalqty) = 0;

  return status;
}

int
neut_tess_var_val_one (struct TESS *pTess, int *showedge, int *showface,
                       int *showpoly, char *entity, int id, char *var,
                       double *pvals, char **ptype)
{
  double *tmp = NULL;
  int status, qty;

  status = neut_tess_var_val (pTess, showedge, showface, showpoly, entity, id,
                              var, &tmp, &qty, ptype);

  (*pvals) = tmp[0];

  ut_free_1d (&tmp);

  return (!status && qty == 1) ? 0 : -1;
}

void
neut_tess_entity_dim (struct TESS Tess, char *entity, int *pdim)
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
  else if (!strncmp (entity, "cell", 4))
    (*pdim) = Tess.Dim;
  else if (!strncmp (entity, "crystal", 7))
    (*pdim) = 5;
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
  else if (dim == 4)
    ut_string_string ("seed", pentity);
  else if (dim == 5)
    ut_string_string ("crystal", pentity);
  else
    abort ();

  return;
}

void
neut_tess_entity_qty (struct TESS Tess, char *entity, int *pqty)
{
  if (!strcmp (entity, "tess"))
    (*pqty) = 1;
  else if (!strcmp (entity, "ver"))
    (*pqty) = Tess.VerQty;
  else if (!strcmp (entity, "edge"))
    (*pqty) = Tess.EdgeQty;
  else if (!strcmp (entity, "face"))
    (*pqty) = Tess.FaceQty;
  else if (!strcmp (entity, "poly"))
    (*pqty) = Tess.PolyQty;
  else if (!strcmp (entity, "cell"))
    (*pqty) = Tess.CellQty;
  else if (!strcmp (entity, "crystal"))
    (*pqty) = Tess.CellQty;
  else if (!strcmp (entity, "crystaledge"))
    (*pqty) = 1;
  else if (!strcmp (entity, "celledge"))
    (*pqty) = 1;
  else if (!strcmp (entity, "seed"))
    (*pqty) = Tess.CellQty;
  else if (!strcmp (entity, "group"))
    (*pqty) = Tess.CellGroup ? ut_array_1d_int_max (Tess.CellGroup + 1, Tess.CellQty) : 0;
  else
    (*pqty) = -1;

  return;
}

int
neut_tess_expr_cells (struct TESS *pTess, char *expr,
                      int **pcells, int *pcellqty)
{
  int status = -1;

  if ((*pTess).Dim == 3)
    status = neut_tess_expr_polys (pTess, expr, pcells, pcellqty);
  else if ((*pTess).Dim == 2)
    status = neut_tess_expr_faces (pTess, expr, pcells, pcellqty);
  else
    abort ();

  return status;
}

int
neut_tess_expr_polys (struct TESS *pTess, char *expr, int **ppoly,
                         int *ppolyqty)
{
  int i, j, status;
  double res;
  FILE *file = NULL;

  if (!expr)
    (*ppolyqty) = 0;

  else if (!strcmp (expr, "all"))
  {
    (*ppolyqty) = (*pTess).PolyQty;
    (*ppoly) = ut_alloc_1d_int (*ppolyqty + 1);
    for (i = 1; i <= *ppolyqty; i++)
      (*ppoly)[i - 1] = i;
  }

  else if (ut_string_isfilename (expr))
  {
    (*ppolyqty) = ut_file_nbwords (expr);
    (*ppoly) = ut_alloc_1d_int (*ppolyqty);
    file = ut_file_open (expr, "r");
    ut_array_1d_int_fscanf (file, *ppoly, *ppolyqty);
    ut_file_close (file, expr, "r");
  }

  else
  {
    int varqty;
    char **vars = NULL;
    double *vals = NULL;

    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    (*ppolyqty) = 0;
    for (i = 1; i <= (*pTess).PolyQty; i++)
    {
      for (j = 0; j < varqty; j++)
        neut_tess_var_val_one (pTess, NULL, NULL, NULL, "poly", i, vars[j],
                               vals + j, NULL);

      status = ut_math_eval (expr, varqty, vars, vals, &res);
      if (status == -1)
        abort ();
      if (ut_num_equal (res, 1, 1e-6))
      {
        (*ppoly) = ut_realloc_1d_int (*ppoly, ++(*ppolyqty));
        (*ppoly)[(*ppolyqty) - 1] = i;
      }
    }

    ut_free_2d_char (&vars, varqty);
    ut_free_1d (&vals);
  }

  // for consistency with neut_tess_expr_faces
  ut_array_1d_int_sort (*ppoly, *ppolyqty);

  return 0;
}

int
neut_tess_expr_faces (struct TESS *pTess, char *expr, int **pface,
                         int *pfaceqty)
{
  int i, j, status;
  double res;
  FILE *file = NULL;

  if (!expr)
    (*pfaceqty) = 0;

  else if (!strcmp (expr, "all"))
  {
    (*pfaceqty) = (*pTess).FaceQty;
    (*pface) = ut_alloc_1d_int (*pfaceqty + 1);
    for (i = 1; i <= *pfaceqty; i++)
      (*pface)[i - 1] = i;
  }

  else if (ut_string_isfilename (expr))
  {
    (*pfaceqty) = ut_file_nbwords (expr);
    (*pface) = ut_alloc_1d_int (*pfaceqty);
    file = ut_file_open (expr, "r");
    ut_array_1d_int_fscanf (file, *pface, *pfaceqty);
    ut_file_close (file, expr, "r");
  }
  else
  {
    int varqty;
    char **vars = NULL;
    double *vals = NULL;

    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    (*pfaceqty) = 0;
    for (i = 1; i <= (*pTess).FaceQty; i++)
    {
      for (j = 0; j < varqty; j++)
        neut_tess_var_val_one (pTess, NULL, NULL, NULL, "face", i, vars[j],
                               vals + j, NULL);

      status = ut_math_eval (expr, varqty, vars, vals, &res);
      if (status == -1)
        abort ();
      if (ut_num_equal (res, 1, 1e-6))
      {
        (*pface) = ut_realloc_1d_int (*pface, ++(*pfaceqty));
        (*pface)[(*pfaceqty) - 1] = i;
      }
    }

    ut_free_2d_char (&vars, varqty);
    ut_free_1d (&vals);
  }

  // this is needed e.g. in nem_meshing_2D
  ut_array_1d_int_sort (*pface, *pfaceqty);

  return 0;
}

void
neut_tess_entity_expr_val_int (struct TESS *pTess, char *entity, char *expr,
                               int *val)
{
  int qty;
  double *tmp = NULL;

  neut_tess_entity_qty (*pTess, entity, &qty);
  tmp = ut_alloc_1d (qty + 1);

  neut_tess_entity_expr_val (pTess, entity, expr, tmp, NULL);
  ut_array_1d_round (tmp + 1, qty, val + 1);

  ut_free_1d (&tmp);

  return;
}

void
neut_tess_entity_expr_val (struct TESS *pTess, char *entity,
                           char *expr, double *val, char **ptype)
{
  int j, k, status, entityqty, varqty;
  char **vars = NULL;
  double *vals = NULL;
  FILE *file = NULL;
  char *type = NULL;

  if (ptype)
    ut_string_string ("%d", ptype);

  neut_tess_entity_qty (*pTess, entity, &entityqty);
  ut_math_vars (expr, &vars, &varqty);
  vals = ut_alloc_1d (varqty);

  if (ut_string_isfilename (expr))
  {
    file = ut_file_open (expr, "R");
    ut_array_1d_fscanf (file, val + 1, entityqty);
    ut_file_close (file, expr, "R");
  }
  else
  {
    for (j = 1; j <= entityqty; j++)
    {
      if (varqty == 1 && !strcmp (expr, vars[0]))
      {
        status = neut_tess_var_val_one (pTess, NULL, NULL, NULL, entity, j, expr,
                                        val + j, &type);
        if (ptype && !strcmp (type, "%f"))
          ut_string_string ("%f", ptype);
      }
      else
      {
        for (k = 0; k < varqty; k++)
        {
          if (!strcmp (vars[k], "default"))
            vals[k] = val[j];

          status = neut_tess_var_val_one (pTess, NULL, NULL, NULL, entity, j, vars[k],
                                 vals + k, &type);
          if (ptype && !strcmp (type, "%f"))
            ut_string_string ("%f", ptype);
        }

        status = ut_math_eval (expr, varqty, vars, vals, val + j);
        if (status == -1)
          ut_print_exprbug (expr);
      }
    }
  }

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);
  ut_free_1d_char (&type);

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
    ut_print_neperbug ();

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
  if (!strcmp (Tess.Type, "periodic"))
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
  if (!strcmp (Tess.Type, "periodic"))
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
  if (!strcmp (Tess.Type, "periodic"))
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
    ut_array_1d_memcpy (Tess.VerCoo[vers[i]], 3, tmp[i]);

  for (i = 0; i < verqty; i++)
    ut_space_point_dir_plane_proj (tmp[i], eq + 1, eq, tmp[i]);

  // merging points
  for (i = 0; i < verqty; i++)
    for (j = i + 1; j < verqty; j++)
      if (ut_space_dist (tmp[i], tmp[j]) < 1e-6)
      {
        ut_array_2d_memcpy (tmp + j + 1, verqty - j - 1, 3, tmp + j);
        j--;
        verqty--;
      }

  // shifting toward origin
  ut_space_bary (tmp, verqty, bary);
  for (i = 0; i < verqty; i++)
    ut_array_1d_sub (bary, tmp[i], 3, tmp[i]);

  // aligning normal with z
  ol_vect_vect_g (eq + 1, z, g);
  ol_g_inverse (g, ginv);

  for (i = 0; i < verqty; i++)
    ut_mat_vect_product (ginv, 3, 3, tmp[i], 3, tmp[i]);

  for (i = 0; i < verqty; i++)
    if (fabs (tmp[i][2]) > 1e-6)
      ut_print_neperbug ();

  srand48 (1);
  for (i = 0; i < verqty; i++)
    for (j = 0; j < 2; j++)
      tmp[i][j] += drand48 () * 1e-12;

  ut_space_points_convexhull_2d (tmp, verqty, &ids, pqty);
  (*pcoo) = ut_alloc_2d (*pqty, 3);
  for (i = 0; i < *pqty; i++)
  {
    ut_mat_vect_product (g, 3, 3, tmp[ids[i]], 3, (*pcoo)[i]);
    ut_array_1d_add ((*pcoo)[i], bary, 3, (*pcoo)[i]);
  }

  ut_free_2d (&tmp, verqty);
  ut_free_1d_int (&ids);
  ut_free_1d (&bary);
  ut_free_1d (&z);
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
  ut_array_1d_memcpy (eq, 3, (*pcoo)[0]);
  ut_array_1d_scale ((*pcoo)[0], 3, ut_array_1d_min (proj, verqty));
  ut_array_1d_memcpy (eq, 3, (*pcoo)[1]);
  ut_array_1d_scale ((*pcoo)[1], 3, ut_array_1d_max (proj, verqty));

  ut_free_1d (&proj);

  return 0;
}

int
neut_tess_delaunay (struct TESS Tess, struct NODES *pNodes,
                    struct MESH *pMesh)
{
  int i, cellqty, *cells = NULL;

  neut_nodes_reset (pNodes);
  neut_mesh_free (pMesh);

  (*pMesh) = neut_mesh_alloc (Tess.Dim, "tri", 1, 0, 1);

  (*pNodes).NodeQty = Tess.SeedQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);
  ut_array_2d_memcpy (Tess.SeedCoo + 1, (*pNodes).NodeQty, 3,
                      (*pNodes).NodeCoo + 1);

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

  ut_free_1d_int (&cells);

  return 0;
}

int
neut_tess_expr_vars_vals (struct TESS *pTess, char *expr, int *showedge,
                          int *showface, int *showpoly, char *entity, int id,
                          char ***pvars, double **pvals, char ***ptypes,
                          int *pvarqty)
{
  int i;

  ut_math_vars (expr, pvars, pvarqty);

  (*pvals) = ut_alloc_1d (*pvarqty);

  if (ptypes)
    (*ptypes) = ut_alloc_1d_pchar (*pvarqty);

  for (i = 0; i < *pvarqty; i++)
    neut_tess_var_val_one (pTess, showedge, showface, showpoly, entity, id,
                           (*pvars)[i], (*pvals) + i,
                           ptypes ? (*ptypes) + i : NULL);

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
  return (Tess.Dim == 3 && Tess.PolyFaceQty[cell] == 0) || (Tess.Dim == 2
                                                            && Tess.
                                                            FaceVerQty[cell]
                                                            == 0);
}

int
neut_tess_hascellorispread (struct TESS Tess)
{
  int i;

  if (Tess.CellOriDistrib)
    for (i = 1; i <= Tess.CellQty; i++)
      if (strcmp (Tess.CellOriDistrib[i], "none"))
        return 1;

  return 0;
}

int
neut_tess_isreg (struct TESS Tess)
{
  return ut_array_1d_int_max (Tess.VerState + 1, Tess.VerQty) > 0;
}

int
neut_tess_cell_id (struct TESS Tess, int cell)
{
  return Tess.CellId ? Tess.CellId[cell] : cell;
}

void
neut_tess_olset (struct TESS Tess, struct OL_SET *pOSet)
{
  int i;

  (*pOSet) = ol_set_alloc (Tess.CellQty, Tess.CellCrySym ? Tess.CellCrySym : "triclinic");

  for (i = 1; i <= Tess.CellQty; i++)
  {
    ol_q_memcpy (Tess.CellOri[i], (*pOSet).q[i -1]);
    neut_tess_cell_size (Tess, i, (*pOSet).weight + i - 1);
  }

  return;
}

int
neut_tess_cellori (struct TESS Tess, double **cellori)
{
  int status = -1;
  struct SIM *pSim = Tess.pSim;

  // if sim, trying to read from sim (even at initial step)
  if (pSim && !neut_sim_isvoid (*pSim))
  {
    struct SIMRES SimRes;

    neut_simres_set_zero (&SimRes);

    neut_sim_simres (*pSim, "cell", "ori", &SimRes);

    if (ut_file_exist (SimRes.file))
    {
      neut_ori_fnscanf (SimRes.file, (*pSim).OriDes, "ascii", cellori + 1, NULL, Tess.CellQty, NULL, "R");
      status = 0;
    }
    else if ((*pSim).step == 0 && Tess.CellOri)
    {
      ut_array_2d_memcpy (Tess.CellOri + 1, Tess.CellQty, 4, cellori + 1);
      status = 0;
    }

    neut_simres_free (&SimRes);
  }

  // otherwise, reading internal
  else if (Tess.CellOri)
  {
    ut_array_2d_memcpy (Tess.CellOri + 1, Tess.CellQty, 4, cellori + 1);
    status = 0;
  }

  return status;
}

void
neut_tess_cells_olset (struct TESS Tess, double **CellOri, char *crysym,
                       int *cells, int cellqty, struct OL_SET *pOSet)
{
  int i, cell;

  *pOSet = ol_set_alloc (cellqty, crysym);

  for (i = 0; i < cellqty; i++)
  {
    cell = cells ? cells[i] : i + 1;
    neut_tess_cell_size (Tess, cell, (*pOSet).weight + i);
    ol_q_memcpy (CellOri ? CellOri[cell] : Tess.CellOri[cell], (*pOSet).q[i]);
  }

  if (crysym)
    ut_string_string (crysym, &(*pOSet).crysym);

  return;
}

int
neut_tess_expr_val (struct TESS *pTess, int *showedge, int *showface, int *showpoly,
                    char *entity, int id, char *expr,
                    double **pvals, int *pvalqty, char **ptype)
{
  int i, status, varqty;
  char **vars = NULL;
  double *vals = NULL;

  ut_math_vars (expr, &vars, &varqty);

  vals = ut_alloc_1d (varqty);

  if (varqty == 1 && !strcmp (expr, vars[0]))
    status = neut_tess_var_val (pTess, showedge, showface, showpoly, entity, id, expr, pvals, pvalqty, ptype);

  else
  {
    for (i = 0; i < varqty; i++)
      neut_tess_var_val_one (pTess, showedge, showface, showpoly, entity, id, vars[i], vals + i, NULL);

    (*pvalqty) = 1;
    (*pvals) = ut_realloc_1d (*pvals, *pvalqty);
    status = ut_math_eval (expr, varqty, vars, vals, *pvals);

    if (ptype)
    {
      if (ut_math_eval_exprislogical (expr))
        ut_string_string ("%d", ptype);
      else
        ut_string_string ("%f", ptype);
    }
  }

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return status;
}

int
neut_tess_expr_val_one (struct TESS *pTess, int *showedge, int *showface, int *showpoly,
                        char *entity, int id, char *expr, double *pval, char **ptype)
{
  int qty;
  double *tmp = NULL;

  neut_tess_expr_val (pTess, showedge, showface, showpoly, entity, id, expr, &tmp, &qty, ptype);
  if (qty != 1)
    abort ();

  *pval = tmp[0];

  ut_free_1d (&tmp);

  return 0;
}
