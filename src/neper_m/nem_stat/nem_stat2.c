/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_stat_.h"

void
nem_stat_nodes (FILE * file, char *format, struct NODES Nodes,
                struct MESH *Mesh, struct TESS Tess)
{
  int i, j, status, invalqty, valqty;
  double *vals = NULL;
  char **invar = NULL, *valstring = NULL, *type = NULL;
  double **meshp = NULL, *meshd = NULL, **meshv = NULL, **meshn = NULL;

  if (ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshp")
      || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshd")
      || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshv")
      || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshn"))
  {
    meshp = ut_alloc_2d (Nodes.NodeQty + 1, 3);
    meshd = ut_alloc_1d (Nodes.NodeQty + 1);
    meshv = ut_alloc_2d (Nodes.NodeQty + 1, 3);
    meshn = ut_alloc_2d (Nodes.NodeQty + 1, 3);
    neut_mesh_nodes_mesh2ddist (Tess, Nodes, Mesh[2], Mesh[3], meshp, meshd,
                                meshv, meshn);
  }

  ut_list_break (format, NEUT_SEP_NODEP, &invar, &invalqty);

  for (i = 1; i <= Nodes.NodeQty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status =
        neut_mesh_var_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                           Tess, NULL, NULL, NULL, NULL, 0, "node", i,
                           invar[j], &vals, &valqty, &type);

      if (!status)
      {
        if (!strncmp (invar[j], "col_", 4))
          fprintf (file, "%.0f|%.0f|%.0f", vals[0], vals[1], vals[2]);
        else
          ut_array_1d_fprintf_nonl (file, vals, valqty,
                                    !strcmp (type, "%f") ? "%.12f" : type);
      }
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
        ut_print_exprbug (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (&invar, invalqty);
  ut_free_1d_char (&type);
  ut_free_1d_char (&valstring);

  ut_free_1d (&meshd);
  ut_free_2d (&meshp, Nodes.NodeQty + 1);
  ut_free_2d (&meshv, Nodes.NodeQty + 1);
  ut_free_2d (&meshn, Nodes.NodeQty + 1);
  ut_free_1d (&vals);

  return;
}

void
nem_stat_elts (FILE *file, char *entity, int dim, int realdim, char *format, struct NODES Nodes,
               struct MESH *Mesh, struct MESHPARA MeshPara, struct TESS Tess)
{
  int i, j, invalqty, qty, meshx_init, valqty, status;
  double *vals = NULL, cl;
  char **invar = NULL, *type = NULL;
  char *entity2 = ut_alloc_1d_char (10);
  double **meshp = NULL, *meshd = NULL, **meshv = NULL, **meshn = NULL;

  if (!strcmp (entity, "elt"))
    qty = Mesh[realdim].EltQty;
  else if (!strcmp (entity, "elset"))
    qty = Mesh[realdim].ElsetQty;
  else if (!strcmp (entity, "mesh"))
    qty = 1;
  else
    abort ();

  if (dim != 4)
    sprintf (entity2, "%s%dd", entity, realdim);
  else
    sprintf (entity2, "%sco", entity);

  ut_list_break (format, NEUT_SEP_NODEP, &invar, &invalqty);

  meshx_init = 0;
  if (!strcmp (entity, "elt") && realdim == 3
      && (ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshp")
          || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshd")
          || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshv")
          || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshn")))
  {
    meshx_init = 1;
    meshp = ut_alloc_2d (Mesh[3].EltQty + 1, 3);
    meshd = ut_alloc_1d (Mesh[3].EltQty + 1);
    meshv = ut_alloc_2d (Mesh[3].EltQty + 1, 3);
    meshn = ut_alloc_2d (Mesh[3].EltQty + 1, 3);
    neut_mesh_elts_mesh2ddist (Tess, Nodes, Mesh[2], Mesh[3], meshp, meshd,
                               meshv, meshn);
  }

  for (i = 1; i <= qty; i++)
  {
    if (!strcmp (entity, "elset") && realdim == 3)
      cl = (MeshPara.poly_cl) ? MeshPara.poly_cl[i] : DBL_MAX;
    else
      cl = -1;

    for (j = 0; j < invalqty; j++)
    {
      status =
        neut_mesh_var_val (Nodes, Mesh[0], Mesh[1], Mesh[2], Mesh[3], Mesh[4],
                           Tess, NULL, NULL, NULL, NULL, cl, entity2, i,
                           invar[j], &vals, &valqty, &type);
      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty,
                                  !strcmp (type, "%f") ? "%.12f" : type);
      else if (realdim == 3 && !strcmp (invar[j], "2dmeshp"))
        fprintf (file, "%.12f %.12f %.12f", meshp[i][0], meshp[i][1],
                 meshp[i][2]);
      else if (realdim == 3 && !strcmp (invar[j], "2dmeshd"))
        fprintf (file, "%.12f", meshd[i]);
      else if (realdim == 3 && !strcmp (invar[j], "2dmeshv"))
        fprintf (file, "%.12f %.12f %.12f", meshv[i][0], meshv[i][1],
                 meshv[i][2]);
      else if (realdim == 3 && !strcmp (invar[j], "2dmeshn"))
        fprintf (file, "%.12f %.12f %.12f", meshn[i][0], meshn[i][1],
                 meshn[i][2]);
      else
        ut_print_exprbug (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }
  }

  ut_free_2d_char (&invar, invalqty);
  ut_free_1d_char (&entity2);
  ut_free_1d_char (&type);
  ut_free_1d (&vals);

  if (meshx_init)
  {
    ut_free_1d (&meshd);
    ut_free_2d (&meshp, Mesh[3].EltQty + 1);
    ut_free_2d (&meshv, Mesh[3].EltQty + 1);
    ut_free_2d (&meshn, Mesh[3].EltQty + 1);
  }

  return;
}

void
nem_stat_point (FILE * file, char *format, struct NODES Nodes,
                struct MESH *Mesh, struct TESS Tess, struct POINT Point)
{
  int i, j, invalqty, meshx_init = 0, valqty, status;
  double *vals = NULL;
  char **invar = NULL, *valstring = NULL, *type = NULL;
  double **meshp = NULL, *meshd = NULL, **meshv = NULL, **meshn = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  if (dim == 3)
  {
    meshx_init = 0;
    if (ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshp")
        || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshd")
        || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshv")
        || ut_list_testelt (format, NEUT_SEP_NODEP, "2dmeshn"))
    {
      meshx_init = 1;
      meshp = ut_alloc_2d (Point.PointQty + 1, 3);
      meshd = ut_alloc_1d (Point.PointQty + 1);
      meshv = ut_alloc_2d (Point.PointQty + 1, 3);
      meshn = ut_alloc_2d (Point.PointQty + 1, 3);
      neut_mesh_points_mesh2ddist (Tess, Nodes, Mesh[2], Mesh[3],
                                   Point.PointCoo + 1, Point.PointQty,
                                   meshp + 1, meshd + 1, meshv + 1,
                                   meshn + 1);
    }
  }

  ut_list_break (format, NEUT_SEP_NODEP, &invar, &invalqty);

  for (i = 1; i <= Point.PointQty; i++)
    for (j = 0; j < invalqty; j++)
    {
      status =
        neut_point_var_val (Point, i, Tess, Nodes, Mesh[dim], invar[j], &vals,
                            &valqty, &type);

      if (!status)
        ut_array_1d_fprintf_nonl (file, vals, valqty,
                                  !strcmp (type, "%f") ? "%.12f" : type);
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
        ut_print_exprbug (invar[j]);

      fprintf (file, (j < invalqty - 1) ? " " : "\n");
    }

  ut_free_2d_char (&invar, invalqty);
  ut_free_1d_char (&type);
  ut_free_1d_char (&valstring);
  ut_free_1d (&vals);

  if (meshx_init)
  {
    ut_free_1d (&meshd);
    ut_free_2d (&meshp, Point.PointQty + 1);
    ut_free_2d (&meshv, Point.PointQty + 1);
    ut_free_2d (&meshn, Point.PointQty + 1);
  }

  return;
}
