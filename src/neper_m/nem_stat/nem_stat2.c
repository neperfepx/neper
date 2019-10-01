/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_stat_.h"

void
nem_stat_nodes (FILE * file, char *format, struct NODES Nodes,
		struct MESH *Mesh, struct PART Part, struct TESS Tess)
{
  int i, j, status, invalqty, var_qty, valqty;
  double *vals = NULL;
  char **invar = NULL, *valstring = NULL, *type = NULL, **vars = NULL;
  double **meshp = NULL, *meshd = NULL, **meshv = NULL, **meshn = NULL;

  if (ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshp")
      || ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshd")
      || ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshv")
      || ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshn"))
  {
    meshp = ut_alloc_2d (Nodes.NodeQty + 1, 3);
    meshd = ut_alloc_1d (Nodes.NodeQty + 1);
    meshv = ut_alloc_2d (Nodes.NodeQty + 1, 3);
    meshn = ut_alloc_2d (Nodes.NodeQty + 1, 3);
    neut_mesh_nodes_mesh2ddist (Tess, Nodes, Mesh[2], Mesh[3], meshp,
				meshd, meshv, meshn);
  }

  ut_string_separate (format, NEUT_SEP_NODEP, &invar, &invalqty);

  neut_mesh_var_list ("node", &vars, &var_qty);

  for (i = 1; i <= Nodes.NodeQty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status = neut_mesh_var_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                                  Part, Tess, NULL, NULL, NULL, NULL, 0,
                                  "node", i, invar[j], &vals, &valqty,
                                  &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty, !strcmp (type, "%f") ? "%.12f" : type);
      else if (!strcmp (invar[j], "2dmeshp"))
	fprintf (file, "%.12f %.12f %.12f", meshp[i][0], meshp[i][1],
		 meshp[i][2]);
      else if (!strcmp (invar[j], "2dmeshd"))
	fprintf (file, "%.12f", meshd[i]);
      else if (!strcmp (invar[j], "2dmeshv"))
	fprintf (file, "%.12f %.12f %.12f", meshv[i][0], meshv[i][1],
		 meshv[i][2]);
      else if (!strcmp (invar[j], "2dmeshn"))
	fprintf (file, "%.12f %.12f %.12f", meshn[i][0], meshn[i][1],
		 meshn[i][2]);
      else
        ut_error_expression (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (invar, invalqty);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d_char (type);
  ut_free_1d_char (valstring);

  ut_free_1d (meshd);
  ut_free_2d (meshp, Nodes.NodeQty + 1);
  ut_free_2d (meshv, Nodes.NodeQty + 1);
  ut_free_2d (meshn, Nodes.NodeQty + 1);
  ut_free_1d (vals);

  return;
}

void
nem_stat_elts (FILE * file, int dim, char *format, struct NODES Nodes,
	       struct MESH *Mesh, struct PART Part, struct TESS Tess)
{
  int i, j, invalqty, var_qty, qty, meshx_init, valqty, status;
  double *vals = NULL;
  char **invar = NULL, *valstring = NULL, *type = NULL, **vars = NULL;
  char *entity = ut_alloc_1d_char (10);
  double **meshp = NULL, *meshd = NULL, **meshv = NULL, **meshn = NULL;

  meshx_init = 0;
  if (dim == 3 &&
      (ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshp")
       || ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshd")
       || ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshv")
       || ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshn")))
  {
    meshx_init = 1;
    meshp = ut_alloc_2d (Mesh[3].EltQty + 1, 3);
    meshd = ut_alloc_1d (Mesh[3].EltQty + 1);
    meshv = ut_alloc_2d (Mesh[3].EltQty + 1, 3);
    meshn = ut_alloc_2d (Mesh[3].EltQty + 1, 3);
    neut_mesh_elts_mesh2ddist (Tess, Nodes, Mesh[2], Mesh[3], meshp,
			       meshd, meshv, meshn);
  }

  sprintf (entity, "elt%dd", dim);

  qty = Mesh[dim].EltQty;

  ut_string_separate (format, NEUT_SEP_NODEP, &invar, &invalqty);

  neut_mesh_var_list (entity, &vars, &var_qty);

  for (i = 1; i <= qty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status = neut_mesh_var_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3],
                                  Part, Tess, NULL, NULL, NULL, NULL, 0,
                                  entity, i, invar[j], &vals, &valqty,
                                  &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty, !strcmp (type, "%f") ? "%.12f" : type);
      else if (dim == 3 && !strcmp (invar[j], "2dmeshp"))
        fprintf (file, "%.12f %.12f %.12f", meshp[i][0], meshp[i][1],
                 meshp[i][2]);
      else if (dim == 3 && !strcmp (invar[j], "2dmeshd"))
        fprintf (file, "%.12f", meshd[i]);
      else if (dim == 3 && !strcmp (invar[j], "2dmeshv"))
        fprintf (file, "%.12f %.12f %.12f", meshv[i][0], meshv[i][1],
                 meshv[i][2]);
      else if (dim == 3 && !strcmp (invar[j], "2dmeshn"))
        fprintf (file, "%.12f %.12f %.12f", meshn[i][0], meshn[i][1],
                 meshn[i][2]);
      else
        ut_error_expression (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (invar, invalqty);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d_char (type);
  ut_free_1d_char (valstring);
  ut_free_1d (vals);

  if (meshx_init)
  {
    ut_free_1d (meshd);
    ut_free_2d (meshp, Mesh[3].EltQty + 1);
    ut_free_2d (meshv, Mesh[3].EltQty + 1);
    ut_free_2d (meshn, Mesh[3].EltQty + 1);
  }

  return;
}

void
nem_stat_elsets (FILE * file, int dim, char *format, struct NODES Nodes,
		 struct MESH *Mesh, struct PART Part,
		 struct MESHPARA MeshPara, struct TESS Tess)
{
  int i, j, status, qty;
  char **vars = NULL;
  char **invar = NULL;
  int invalqty, varqty;
  double val, cl;
  char *type = NULL;
  char *entity = ut_alloc_1d_char (100);

  qty = Mesh[dim].ElsetQty;

  sprintf (entity, "elset%dd", dim);

  ut_string_separate (format, NEUT_SEP_NODEP, &invar, &invalqty);

  neut_mesh_var_list (entity, &vars, &varqty);

  for (i = 1; i <= qty; i++)
  {
    if (dim == 3)
      cl = (MeshPara.poly_cl) ? MeshPara.poly_cl[i] : DBL_MAX;
    else
      cl = -1;

    for (j = 0; j < invalqty; j++)
    {
      status = neut_mesh_var_val_one (Nodes, Mesh[0], Mesh[1], Mesh[2],
                                      Mesh[3], Part, Tess, NULL, NULL, NULL,
                                      NULL, cl, entity, i, invar[j], &val,
                                      &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, &val, 1, !strcmp (type, "%f") ? "%.12f" : type);
      else
        ut_error_expression (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }
  }

  ut_free_1d_char (type);
  ut_free_2d_char (invar, invalqty);
  ut_free_2d_char (vars, varqty);
  ut_free_1d_char (entity);

  return;
}

void
nem_stat_point (FILE * file, char *format,
		struct NODES Nodes,
		struct MESH *Mesh, struct TESS Tess, struct POINT Point)
{
  int i, j, invalqty, var_qty, meshx_init = 0, valqty, status;
  double *vals = NULL;
  char **invar = NULL, *valstring = NULL, *type = NULL, **vars = NULL;
  double **meshp = NULL, *meshd = NULL, **meshv = NULL, **meshn = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  if (dim == 3)
  {
    meshx_init = 0;
    if (ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshp")
	|| ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshd")
	|| ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshv")
	|| ut_string_inlist (format, NEUT_SEP_NODEP, "2dmeshn"))
    {
      meshx_init = 1;
      meshp = ut_alloc_2d (Point.PointQty + 1, 3);
      meshd = ut_alloc_1d (Point.PointQty + 1);
      meshv = ut_alloc_2d (Point.PointQty + 1, 3);
      meshn = ut_alloc_2d (Point.PointQty + 1, 3);
      neut_mesh_points_mesh2ddist (Tess, Nodes, Mesh[2], Mesh[3],
				   Point.PointCoo + 1, Point.PointQty,
				   meshp + 1, meshd + 1, meshv + 1, meshn + 1);
    }
  }

  ut_string_separate (format, NEUT_SEP_NODEP, &invar, &invalqty);

  neut_point_var_list (&vars, &var_qty);

  for (i = 1; i <= Point.PointQty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status = neut_point_var_val (Point, i, Tess, Nodes, Mesh[dim], invar[j],
                                   &vals, &valqty, &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty, !strcmp (type, "%f") ? "%.12f" : type);
      else if (!strcmp (invar[j], "2dmeshp"))
	fprintf (file, "%.12f %.12f %.12f", meshp[i][0], meshp[i][1],
		 meshp[i][2]);
      else if (!strcmp (invar[j], "2dmeshd"))
	fprintf (file, "%.12f", meshd[i]);
      else if (!strcmp (invar[j], "2dmeshv"))
	fprintf (file, "%.12f %.12f %.12f", meshv[i][0], meshv[i][1],
		 meshv[i][2]);
      else if (!strcmp (invar[j], "2dmeshn"))
	fprintf (file, "%.12f %.12f %.12f", meshn[i][0], meshn[i][1],
		 meshn[i][2]);
      else
        ut_error_expression (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (invar, invalqty);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d_char (type);
  ut_free_1d_char (valstring);
  ut_free_1d (vals);

  if (meshx_init)
  {
    ut_free_1d (meshd);
    ut_free_2d (meshp, Point.PointQty + 1);
    ut_free_2d (meshv, Point.PointQty + 1);
    ut_free_2d (meshn, Point.PointQty + 1);
  }

  return;
}
