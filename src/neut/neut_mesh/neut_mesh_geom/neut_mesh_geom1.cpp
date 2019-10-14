/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_geom_.h"
#include"neut/neut_structs/neut_nanoflann_struct.hpp"

/* neut_mesh_elt_area computes the area of a 2D element */
int
neut_mesh_elt_area (struct NODES Nodes, struct MESH Mesh, int elt,
		    double *parea)
{
  if (Mesh.Dimension != 2)
    ut_error_reportbug ();

  (*parea) = ut_space_triangle_area (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
				     Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
				     Nodes.NodeCoo[Mesh.EltNodes[elt][2]]);

  return 0;
}

/* neut_mesh_elt_volume computes the volume of an element */
int
neut_mesh_elt_volume (struct NODES Nodes, struct MESH Mesh, int elt,
		      double *pvol)
{
  if (Mesh.Dimension != 3)
    return -1;

  if (strcmp (Mesh.EltType, "tri") == 0)
    (*pvol) = ut_space_tet_volume (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
				   Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
				   Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
				   Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);
  else if (strcmp (Mesh.EltType, "quad") == 0)
    (*pvol) = ut_space_hexa_volume (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][4]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][5]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][6]],
				    Nodes.NodeCoo[Mesh.EltNodes[elt][7]]);
  else
    abort ();

  return 0;
}

int
neut_mesh_volume (struct NODES Nodes, struct MESH Mesh, double *pvol)
{
  int i;
  double eltvol;

  if (Mesh.Dimension != 3)
    return -1;

  (*pvol) = 0;
  for (i = 1; i <= Mesh.EltQty; i++)
  {
    neut_mesh_elt_volume (Nodes, Mesh, i, &eltvol);
    (*pvol) += eltvol;
  }

  return 0;
}

int
neut_mesh_elset_volume (struct NODES Nodes, struct MESH Mesh, int elset,
			double *pvol)
{
  int i;
  double tmp;

  if (Mesh.Dimension != 3)
  {
    (*pvol) = 0;
    return -1;
  }

  (*pvol) = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    neut_mesh_elt_volume (Nodes, Mesh, Mesh.Elsets[elset][i], &tmp);
    (*pvol) += tmp;
  }

  return 0;
}

int
neut_mesh_elset_area (struct NODES Nodes, struct MESH Mesh, int elset,
		      double *parea)
{
  int i;
  double tmp;

  if (Mesh.Dimension != 2)
  {
    (*parea) = 0;
    return -1;
  }

  (*parea) = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    neut_mesh_elt_area (Nodes, Mesh, Mesh.Elsets[elset][i], &tmp);
    (*parea) += tmp;
  }

  return 0;
}

int
neut_mesh_area (struct NODES Nodes, struct MESH Mesh, double *parea)
{
  int i;
  double tmp;

  if (Mesh.Dimension != 2)
    return -1;

  (*parea) = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    neut_mesh_elset_area (Nodes, Mesh, i, &tmp);
    (*parea) += tmp;
  }

  return 0;
}

int
neut_mesh_elset_length (struct NODES Nodes, struct MESH Mesh, int elset,
			double *plength)
{
  int i;
  double tmp;

  if (Mesh.Dimension != 1)
  {
    (*plength) = 0;
    return -1;
  }

  (*plength) = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    neut_mesh_elt_length (Nodes, Mesh, Mesh.Elsets[elset][i], &tmp);
    (*plength) += tmp;
  }

  return 0;
}

int
neut_mesh_length (struct NODES Nodes, struct MESH Mesh, double *plength)
{
  int i;
  double tmp;

  if (Mesh.Dimension != 1)
    return -1;

  (*plength) = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    neut_mesh_elset_length (Nodes, Mesh, i, &tmp);
    (*plength) += tmp;
  }

  return 0;
}

int
neut_mesh_size (struct NODES Nodes, struct MESH Mesh, double *psize)
{
  int i;
  double *bbox = ut_alloc_1d (6);

  if (Mesh.Dimension == 0)
  {
    neut_nodes_bbox_vect (Nodes, bbox);
    (*psize) = 0;
    for (i = 0; i < 3; i++)
      (*psize) = ut_num_max (*psize, bbox[2 * i + 1] - bbox[2 * i]);
  }
  else if (Mesh.Dimension == 1)
    neut_mesh_length (Nodes, Mesh, psize);
  else if (Mesh.Dimension == 2)
    neut_mesh_area (Nodes, Mesh, psize);
  else if (Mesh.Dimension == 3)
    neut_mesh_volume (Nodes, Mesh, psize);
  else
    abort ();

  ut_free_1d (bbox);

  return 0;
}

int
neut_mesh_elset_centre (struct NODES Nodes, struct MESH Mesh, int elset,
			double *centre)
{
  int i, elt;
  double *coo = ut_alloc_1d (3);
  double val_tot, val;

  val_tot = 0;
  val = 0;
  ut_array_1d_set (centre, 3, 0);

  if (Mesh.Elsets[elset][0] > 0)
  {
    for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
    {
      elt = Mesh.Elsets[elset][i];
      neut_mesh_elt_centre (Nodes, Mesh, elt, coo);
      if (Mesh.Dimension == 3)
	neut_mesh_elt_volume (Nodes, Mesh, elt, &val);
      else if (Mesh.Dimension == 2)
	neut_mesh_elt_area (Nodes, Mesh, elt, &val);
      else if (Mesh.Dimension == 1)
	neut_mesh_elt_length (Nodes, Mesh, elt, &val);
      else if (Mesh.Dimension == 0)
	val = 1;

      ut_array_1d_scale (coo, 3, val);
      ut_array_1d_add (centre, coo, 3, centre);

      val_tot += val;
    }
    ut_array_1d_scale (centre, 3, 1. / val_tot);
  }

  ut_free_1d (coo);

  return 0;
}

int
neut_mesh_elset_centre_x (struct NODES Nodes, struct MESH Mesh, int elset,
			  double *px)
{
  double *tmp = ut_alloc_1d (3);

  neut_mesh_elset_centre (Nodes, Mesh, elset, tmp);
  (*px) = tmp[0];

  ut_free_1d (tmp);

  return 0;
}

int
neut_mesh_elset_centre_y (struct NODES Nodes, struct MESH Mesh, int elset,
			  double *py)
{
  double *tmp = ut_alloc_1d (3);

  neut_mesh_elset_centre (Nodes, Mesh, elset, tmp);
  (*py) = tmp[1];

  ut_free_1d (tmp);

  return 0;
}

int
neut_mesh_elset_centre_z (struct NODES Nodes, struct MESH Mesh, int elset,
			  double *pz)
{
  double *tmp = ut_alloc_1d (3);

  neut_mesh_elset_centre (Nodes, Mesh, elset, tmp);
  (*pz) = tmp[2];

  ut_free_1d (tmp);

  return 0;
}

int
neut_mesh_elsets_centre (struct NODES Nodes, struct MESH Mesh,
			 int *elsets, int elsetqty, double *centre)
{
  int i, elset;
  double area, totarea;
  double *tmp = ut_alloc_1d (3);

  ut_array_1d_zero (centre, 3);

  totarea = 0;
  for (i = 0; i < elsetqty; i++)
  {
    elset = elsets[i];
    neut_mesh_elset_centre (Nodes, Mesh, elset, tmp);
    neut_mesh_elset_area (Nodes, Mesh, elset, &area);
    ut_array_1d_scale (tmp, 3, area);
    ut_array_1d_add (centre, tmp, 3, centre);
    totarea += area;
  }
  ut_array_1d_scale (centre, 3, 1. / totarea);

  ut_free_1d (tmp);

  return 0;
}

void
neut_mesh_elt_centre (struct NODES Nodes, struct MESH Mesh,
		      int elt, double *coo)
{
  int i, j, node;
  int nodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  /* averaging the coo of the vertex nodes */
  ut_array_1d_zero (coo, 3);
  for (i = 0; i < nodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i];
    for (j = 0; j < 3; j++)
      coo[j] += Nodes.NodeCoo[node][j];
  }

  ut_array_1d_scale (coo, 3, 1. / nodeqty);

  return;
}

void
neut_mesh_elt_centre_x (struct NODES Nodes, struct MESH Mesh,
			int elt, double *px)
{
  int i, node;
  int nodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  (*px) = 0;
  for (i = 0; i < nodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i];
    (*px) += Nodes.NodeCoo[node][0];
  }

  (*px) /= nodeqty;

  return;
}

void
neut_mesh_elt_centre_y (struct NODES Nodes, struct MESH Mesh,
			int elt, double *py)
{
  int i, node;
  int nodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  (*py) = 0;
  for (i = 0; i < nodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i];
    (*py) += Nodes.NodeCoo[node][1];
  }

  (*py) /= nodeqty;

  return;
}

void
neut_mesh_elt_centre_z (struct NODES Nodes, struct MESH Mesh,
			int elt, double *pz)
{
  int i, node;
  int nodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  (*pz) = 0;
  for (i = 0; i < nodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i];
    (*pz) += Nodes.NodeCoo[node][2];
  }

  (*pz) /= nodeqty;

  return;
}

void
neut_mesh_elt_normal (struct MESH Mesh, struct NODES Nodes, int elt,
		      double *n)
{
  ut_space_trianglenormal (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]], n);

  return;
}

void
neut_mesh_node_normal (struct MESH Mesh, struct NODES Nodes, int node,
		       double *n)
{
  int i, elt;
  double **eltn = NULL;
  double *coo = ut_alloc_1d (3);
  double *weight = NULL;

  if (!Mesh.NodeElts)
    ut_error_reportbug ();

  eltn = ut_alloc_2d (Mesh.NodeElts[node][0] + 1, 3);
  weight = ut_alloc_1d (Mesh.NodeElts[node][0] + 1);

  for (i = 1; i <= Mesh.NodeElts[node][0]; i++)
  {
    elt = Mesh.NodeElts[node][i];
    neut_mesh_elt_normal (Mesh, Nodes, elt, eltn[i]);
    neut_mesh_elt_centre (Nodes, Mesh, elt, coo);
    weight[i] = ut_space_dist (Nodes.NodeCoo[node], coo);
  }

  ut_array_1d_zero (n, 3);
  for (i = 1; i <= Mesh.NodeElts[node][0]; i++)
  {
    ut_array_1d_scale (eltn[i], 3, weight[i]);
    ut_array_1d_add (n, eltn[i], 3, n);
  }
  ut_array_1d_scale (n, 3, 1. / ut_array_1d_sum (weight + 1,
						 Mesh.NodeElts[node][0]));

  ut_free_2d (eltn, Mesh.NodeElts[node][0] + 1);
  ut_free_1d (coo);
  ut_free_1d (weight);

  return;
}

void
neut_mesh_elt_eq (struct MESH Mesh, struct NODES Nodes, int elt, double *eq)
{
  if (Mesh.Dimension == 2)
  {
    if (Mesh.EltType == NULL || strcmp (Mesh.EltType, "tri") == 0)
    {
      ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][2]], eq);
    }
    else if (strcmp (Mesh.EltType, "quad") == 0)
    {
      double **eqs = ut_alloc_2d (4, 4);
      int i;

      ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][2]], eqs[0]);
      ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][3]], eqs[1]);
      ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][0]], eqs[2]);
      ut_space_points_plane (Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			     Nodes.NodeCoo[Mesh.EltNodes[elt][1]], eqs[3]);

      ut_array_1d_zero (eq, 4);
      for (i = 0; i < 4; i++)
	ut_array_1d_add (eq, eqs[i], 4, eq);
      ut_array_1d_scale (eq, 4, 1. / ut_vector_norm (eq + 1));

      ut_free_2d (eqs, 4);
    }
  }
  if (Mesh.Dimension == 1)
  {
    ut_space_points_line (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			  Nodes.NodeCoo[Mesh.EltNodes[elt][1]], eq);
  }

  return;
}

void
neut_mesh_elset_eq (struct MESH Mesh, struct NODES Nodes, int elset,
		    double *eq)
{
  int i, elt;
  double norm, area;
  double *eqe = ut_alloc_1d (4);

  if (Mesh.Dimension != 2)
    ut_error_reportbug ();

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_elt_eq (Mesh, Nodes, elt, eqe);
    neut_mesh_elt_area (Nodes, Mesh, elt, &area);
    ut_array_1d_scale (eqe, 4, area);
    ut_array_1d_add (eqe, eq, 4, eq);
  }

  norm = ut_vector_norm (eq + 1);
  ut_array_1d_scale (eq, 4, 1. / norm);

  ut_free_1d (eqe);

  return;
}

int
neut_mesh_elt_length (struct NODES Nodes, struct MESH Mesh, int elt,
		      double *plength)
{
  int node1, node2;

  if (Mesh.Dimension != 1)
    return -1;

  node1 = Mesh.EltNodes[elt][0];
  node2 = Mesh.EltNodes[elt][1];

  (*plength) = ut_space_dist (Nodes.NodeCoo[node1], Nodes.NodeCoo[node2]);

  return 0;
}

int
neut_mesh_elset_rr (struct NODES Nodes, struct MESH Mesh, int elset,
		    double *prrmean, double *prrmin, double *prrmax)
{
  int i, elt;
  double rr;

  if (Mesh.Dimension != 3 && strcmp (Mesh.EltType, "tri") != 0)
    ut_error_reportbug ();

  (*prrmin) = DBL_MAX;
  (*prrmax) = -DBL_MAX;
  (*prrmean) = 0;

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];

    rr = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			 Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			 Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			 Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);

    (*prrmin) = ut_num_min ((*prrmin), rr);
    (*prrmax) = ut_num_max ((*prrmax), rr);
    (*prrmean) += rr;
  }
  (*prrmean) /= Mesh.Elsets[elset][0];

  return 0;
}

int
neut_mesh_elt_rr (struct NODES Nodes, struct MESH Mesh, int elt, double *prr)
{
  if (Mesh.Dimension != 3)
    ut_error_reportbug ();

  (*prr) = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);

  return 0;
}

int
neut_mesh_rr (struct NODES Nodes, struct MESH Mesh, double *prrav,
	      double *prrmin, double *prrmax)
{
  int i;
  double rr, min, max, av;

  min = DBL_MAX;
  max = -DBL_MAX;
  av = 0;

  if (Mesh.Dimension == 2)
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      rr = neut_elt_rr_2d (Nodes.NodeCoo[Mesh.EltNodes[i][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][2]]);

      min = ut_num_min (min, rr);
      max = ut_num_max (max, rr);
      av += rr;
    }

  else if (Mesh.Dimension == 3)
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      rr = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[i][0]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][1]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][2]],
			   Nodes.NodeCoo[Mesh.EltNodes[i][3]]);

      min = ut_num_min (min, rr);
      max = ut_num_max (max, rr);
      av += rr;
    }

  av /= Mesh.EltQty;

  if (prrmin != NULL)
    (*prrmin) = min;

  if (prrmax != NULL)
    (*prrmax) = max;

  if (prrav != NULL)
    (*prrav) = av;

  return 0;
}

int
neut_mesh_Odis (struct NODES Nodes, struct MESH Mesh, char *Odisexpr,
		double *pOdis)
{
  int i;
  double tmp = 1;
  double *rr = ut_alloc_1d (Mesh.EltQty + 1);

  if (Mesh.Dimension == 3)
    for (i = 1; i <= Mesh.EltQty; i++)
      rr[i] = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[i][0]],
			      Nodes.NodeCoo[Mesh.EltNodes[i][1]],
			      Nodes.NodeCoo[Mesh.EltNodes[i][2]],
			      Nodes.NodeCoo[Mesh.EltNodes[i][3]]);
  else if (Mesh.Dimension == 2)
    for (i = 1; i <= Mesh.EltQty; i++)
      rr[i] = neut_elt_rr_2d (Nodes.NodeCoo[Mesh.EltNodes[i][0]],
			      Nodes.NodeCoo[Mesh.EltNodes[i][1]],
			      Nodes.NodeCoo[Mesh.EltNodes[i][2]]);

  int var_qty = 4;
  char **vars = ut_alloc_2d_char (var_qty, 10);
  double *vals = ut_alloc_1d (var_qty);

  strcpy (vars[0], "dis");
  strcpy (vars[1], "dismin");
  strcpy (vars[2], "dismax");
  strcpy (vars[3], "dismean");
  vals[1] = ut_array_1d_min (rr + 1, Mesh.EltQty);
  vals[2] = ut_array_1d_max (rr + 1, Mesh.EltQty);
  vals[3] = ut_array_1d_mean (rr + 1, Mesh.EltQty);

  (*pOdis) = 1;
  for (i = 1; i <= Mesh.EltQty; i++)
  {
    vals[0] = rr[i];

    if (!strcmp (Odisexpr, "dis^(exp((dis^0.1)/(dis^0.1-1)))"))
    {
      // a tolerancy is required as values very close to 1
      // (machine precision) would return tmp = 0.
      if (rr[i] < 1 - 1e-6)
	tmp = pow (rr[i], exp (pow (rr[i], 0.1) / (pow (rr[i], 0.1) - 1)));
      else
	tmp = 1;
    }
    else if (!strcmp (Odisexpr, "dis"))
      tmp = rr[i];
    else
    {
      // a tolerancy is required, as explained above.
      if (rr[i] < 1 - 1e6)
	ut_math_eval (Odisexpr, var_qty, vars, vals, &tmp);
      else
	tmp = 1;
    }
    (*pOdis) *= tmp;
  }

  ut_free_1d (rr);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  return 0;
}

int
neut_mesh_elset_Odis (struct NODES Nodes, struct MESH Mesh, int elset,
		      char *Odisexpr, double *pOdis)
{
  int i, elt;
  double tmp = 1;
  double *rr = ut_alloc_1d (Mesh.Elsets[elset][0] + 1);

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    rr[elt] = neut_elt_rr_3d (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
			      Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
			      Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
			      Nodes.NodeCoo[Mesh.EltNodes[elt][3]]);
  }

  int var_qty = 4;
  char **vars = ut_alloc_2d_char (var_qty, 10);
  double *vals = ut_alloc_1d (var_qty);

  strcpy (vars[0], "dis");
  strcpy (vars[1], "dismin");
  strcpy (vars[2], "dismax");
  strcpy (vars[3], "dismean");
  vals[1] = ut_array_1d_min (rr + 1, Mesh.Elsets[elset][0]);
  vals[2] = ut_array_1d_max (rr + 1, Mesh.Elsets[elset][0]);
  vals[3] = ut_array_1d_mean (rr + 1, Mesh.Elsets[elset][0]);

  (*pOdis) = 1;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    vals[0] = rr[elt];

    if (!strcmp (Odisexpr, "dis^(exp((dis^0.1)/(dis^0.1-1)))"))
    {
      if (rr[elt] < 1 - 1e-6)
	tmp =
	  pow (rr[elt], exp (pow (rr[elt], 0.1) / (pow (rr[elt], 0.1) - 1)));
      else
	tmp = 1;
    }
    else
    {
      if (rr[elt] < 1 - 1e-6)
	ut_math_eval (Odisexpr, var_qty, vars, vals, &tmp);
      else
	tmp = 1;
    }
    (*pOdis) *= tmp;
  }

  ut_free_1d (rr);
  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  return 0;
}

void
neut_mesh_centre (struct NODES Nodes, struct MESH Mesh, double *centre)
{
  int i;
  double *elsetpos = ut_alloc_1d (3);
  double val, val_tot;

  ut_array_1d_zero (centre, 3);

  val_tot = 0;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    neut_mesh_elset_centre (Nodes, Mesh, i, elsetpos);
    if (Mesh.Dimension == 3)
      neut_mesh_elset_volume (Nodes, Mesh, i, &val);
    else if (Mesh.Dimension == 2)
      neut_mesh_elset_area (Nodes, Mesh, i, &val);
    else if (Mesh.Dimension == 1)
      neut_mesh_elset_length (Nodes, Mesh, i, &val);
    else
      val = 1;

    ut_array_1d_scale (elsetpos, 3, val);
    val_tot += val;

    ut_array_1d_add (centre, elsetpos, 3, centre);
  }

  ut_array_1d_scale (centre, 3, 1. / val_tot);

  ut_free_1d (elsetpos);

  return;
}

void
neut_mesh_elt_lengths (struct NODES Nodes, struct MESH Mesh, int elt,
		       double *pavlength, double *pminlength,
		       double *pmaxlength)
{
  int node1, node2, node3, node4;
  double *length = ut_alloc_1d (4);

  if (Mesh.Dimension == 2)
  {
    node1 = Mesh.EltNodes[elt][0];
    node2 = Mesh.EltNodes[elt][1];
    node3 = Mesh.EltNodes[elt][2];

    length[0] = ut_space_dist (Nodes.NodeCoo[node1], Nodes.NodeCoo[node2]);
    length[1] = ut_space_dist (Nodes.NodeCoo[node2], Nodes.NodeCoo[node3]);
    length[2] = ut_space_dist (Nodes.NodeCoo[node3], Nodes.NodeCoo[node1]);

    if (pavlength != NULL)
      (*pavlength) = ut_array_1d_mean (length, 3);
    if (pminlength != NULL)
      (*pminlength) = ut_array_1d_min (length, 3);
    if (pmaxlength != NULL)
      (*pmaxlength) = ut_array_1d_max (length, 3);
  }
  else if (Mesh.Dimension == 3)
  {
    node1 = Mesh.EltNodes[elt][0];
    node2 = Mesh.EltNodes[elt][1];
    node3 = Mesh.EltNodes[elt][2];
    node4 = Mesh.EltNodes[elt][3];

    length[0] = ut_space_dist (Nodes.NodeCoo[node1], Nodes.NodeCoo[node2]);
    length[1] = ut_space_dist (Nodes.NodeCoo[node2], Nodes.NodeCoo[node3]);
    length[2] = ut_space_dist (Nodes.NodeCoo[node3], Nodes.NodeCoo[node4]);
    length[3] = ut_space_dist (Nodes.NodeCoo[node4], Nodes.NodeCoo[node1]);

    if (pavlength != NULL)
      (*pavlength) = ut_array_1d_mean (length, 4);
    if (pminlength != NULL)
      (*pminlength) = ut_array_1d_min (length, 4);
    if (pmaxlength != NULL)
      (*pmaxlength) = ut_array_1d_max (length, 4);
  }
  else
    ut_error_reportbug ();

  ut_free_1d (length);

  return;
}

int
neut_mesh_elset_Osize (struct NODES Nodes, struct MESH Mesh, int elset,
		       double cl, double *pOsize)
{
  int i, elt;
  double *meanlength = ut_alloc_1d (Mesh.Elsets[elset][0]);

  if (Mesh.EltQty == 0)
  {
    printf ("neut_mesh_Osize: Mesh.EltQty == 0\n");
    ut_error_reportbug ();
  }

  if (elset > Mesh.ElsetQty)
  {
    printf ("elset > Mesh.ElsetQty\n");
    ut_error_reportbug ();
  }

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_elt_lengths (Nodes, Mesh, elt, &(meanlength[i - 1]),
			   NULL, NULL);
  }

  ut_array_1d_scale (meanlength, Mesh.Elsets[elset][0], 1 / cl);

  for (i = 0; i < Mesh.Elsets[elset][0]; i++)
    if (meanlength[i] > 1)
      meanlength[i] = 1 / meanlength[i];
  (*pOsize) = ut_array_1d_mean (meanlength, Mesh.Elsets[elset][0]);

  (*pOsize) = pow (*pOsize, 3);

  ut_free_1d (meanlength);

  return 0;
}

int
neut_mesh_Osize (struct NODES Nodes, struct MESH Mesh, double cl,
		 double *pOsize)
{
  int i;
  double *meanlength = ut_alloc_1d (Mesh.EltQty);

  if (Mesh.EltQty == 0)
  {
    printf ("neut_mesh_Osize: Mesh.EltQty == 0\n");
    abort ();
  }

  for (i = 1; i <= Mesh.EltQty; i++)
    neut_mesh_elt_lengths (Nodes, Mesh, i, &(meanlength[i - 1]), NULL, NULL);

  ut_array_1d_scale (meanlength, Mesh.EltQty, 1 / cl);

  for (i = 0; i < Mesh.EltQty; i++)
    if (meanlength[i] > 1)
      meanlength[i] = 1 / meanlength[i];
  (*pOsize) = ut_array_1d_mean (meanlength, Mesh.EltQty);

  (*pOsize) = pow (*pOsize, 3);

  ut_free_1d (meanlength);

  return 0;
}

int
neut_mesh_eltlength (struct NODES Nodes, struct MESH Mesh, double *pavlength)
{
  int i;
  double *meanlength = ut_alloc_1d (Mesh.EltQty);

  for (i = 1; i <= Mesh.EltQty; i++)
    neut_mesh_elt_lengths (Nodes, Mesh, i, &(meanlength[i - 1]), NULL, NULL);

  (*pavlength) = ut_array_1d_mean (meanlength, Mesh.EltQty);

  ut_free_1d (meanlength);

  return 0;
}

void
neut_mesh_elt_node_angle (struct NODES Nodes, struct MESH Mesh, int elt,
			  int node, double *pangle)
{
  int pos1, pos2, pos3, before, after, eltnodeqty;
  double *v1 = ut_alloc_1d (3);
  double *v2 = ut_alloc_1d (3);

  if (Mesh.Dimension != 2)
    ut_error_reportbug ();

  eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, 1);

  pos2 = ut_array_1d_int_eltpos (Mesh.EltNodes[elt], eltnodeqty, node);
  pos1 = ut_num_rotpos (0, eltnodeqty - 1, pos2, -1);
  pos3 = ut_num_rotpos (0, eltnodeqty - 1, pos2, 1);

  before = Mesh.EltNodes[elt][pos1];
  after = Mesh.EltNodes[elt][pos3];

  ut_array_1d_sub (Nodes.NodeCoo[node], Nodes.NodeCoo[before], 3, v1);
  ut_array_1d_sub (Nodes.NodeCoo[node], Nodes.NodeCoo[after], 3, v2);

  (*pangle) = ut_vector_angle (v1, v2);

  ut_free_1d (v1);
  ut_free_1d (v2);

  return;
}

int
neut_mesh_point_elt (struct MESH Mesh, struct NODES Nodes,
		     double *coo, int *pelt)
{
  int i, status;

  (*pelt) = -1;
  status = 1;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    status = neut_mesh_elset_point_elt (Mesh, Nodes, i, coo, pelt);

    if (status == 0)
      break;
  }

  return status;
}

int
neut_mesh_elset_point_elt (struct MESH Mesh, struct NODES Nodes, int elset,
			   double *coo, int *pelt)
{
  int i, inelt;

  (*pelt) = -1;

  if (elset < 1 || elset > Mesh.ElsetQty)
    return -1;

  inelt = 0;

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    inelt = neut_mesh_point_elt_in (Mesh, Nodes, coo, Mesh.Elsets[elset][i]);

    if (inelt == 1)
    {
      (*pelt) = Mesh.Elsets[elset][i];
      break;
    }
  }

  return (inelt == 1) ? 0 : -1;
}

int
neut_mesh_point_elset (struct MESH Mesh, struct NODES Nodes,
		       double *coo, int *elsetlist, int elsetqty, int *pelset)
{
  int i, elt = -1, status;

  if (elsetlist && elsetqty > 0)
  {
    for (i = 0; i < elsetqty; i++)
    {
      status =
	neut_mesh_elset_point_elt (Mesh, Nodes, elsetlist[i], coo, &elt);
      if (!status)
	break;
    }
  }
  else
    neut_mesh_point_elt (Mesh, Nodes, coo, &elt);

  (*pelset) = (elt != -1) ? Mesh.EltElset[elt] : -1;

  return (*pelset > 0) ? 0 : -1;
}

int
neut_mesh_elset_point_closestelt (struct MESH Mesh, struct NODES Nodes,
				  int elset, double *coo, int *pelt)
{
  int i, elt, id;
  double *dist = ut_alloc_1d (Mesh.Elsets[elset][0]);
  double *eltcoo = ut_alloc_1d (3);

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_elt_centre (Nodes, Mesh, elt, eltcoo);
    dist[i - 1] = ut_space_dist (coo, eltcoo);
  }
  id = 1 + ut_array_1d_min_index (dist, Mesh.Elsets[elset][0]);

  (*pelt) = Mesh.Elsets[elset][id];

  ut_free_1d (dist);
  ut_free_1d (eltcoo);

  return 0;
}

int
neut_mesh_elset_points_closestelts (struct MESH Mesh, struct NODES Nodes,
                                    int elset, double **coos, int qty,
                                    char *method, int *elts)
{
  int i, j, *testqty = ut_alloc_1d_int (4);
  NFTREE *nf_tree = nullptr;
  NFCLOUD nf_cloud;

  if (Mesh.Elsets[elset][0] <= 3)
    ut_array_1d_int_set_4 (testqty, Mesh.Elsets[elset][0], -1, -1, -1);
  else if (Mesh.Elsets[elset][0] <= 10)
    ut_array_1d_int_set_4 (testqty, 3, Mesh.Elsets[elset][0], -1, -1);
  else if (Mesh.Elsets[elset][0] <= 30)
    ut_array_1d_int_set_4 (testqty, 3, 10, Mesh.Elsets[elset][0], -1);
  else
    ut_array_1d_int_set_4 (testqty, 3, 10, 30, Mesh.Elsets[elset][0]);

  // filling cloud

  nf_cloud.pts.resize (Mesh.Elsets[elset][0]);

  for (i = 0; i < Mesh.Elsets[elset][0]; i++)
    neut_mesh_elt_centre (Nodes, Mesh, Mesh.Elsets[elset][i + 1], nf_cloud.pts[i].p);

  // building tree

  nf_tree = new NFTREE (3, nf_cloud);

  // finding neighbour

  for (i = 0; i < qty; i++)
  {
    if (!strcmp (method, "distance"))
      neut_mesh_elset_points_closestelts_search (Mesh, elset, coos[i], nf_tree,
                                                 1, elts + i);

    else if (!strcmp (method, "location"))
      for (j = 0; j < 4 && testqty[j] > 0; j++)
      {
        neut_mesh_elset_points_closestelts_search (Mesh, elset, coos[i],
                                                   nf_tree, testqty[j], elts + i);

        if (neut_mesh_point_elt_in (Mesh, Nodes, coos[i], elts[i]))
          break;
      }

    else
      abort ();
  }

  delete nf_tree;
  ut_free_1d_int (testqty);

  return 0;
}

int
neut_mesh_point_closestpoint (struct MESH Mesh, struct NODES Nodes,
			      double *coo, double *pdist, double *ptcoo,
			      double *ptv, double *ptn)
{
  int j, elt, node;
  double d, eltdist, nodedist;
  double *proj = ut_alloc_1d (3);
  double *eq = ut_alloc_1d (4);
  double *eltcoo = ut_alloc_1d (3);
  eltdist = DBL_MAX;

  elt = -1;

  if (Mesh.Dimension != 2)
    ut_error_reportbug ();

  // testing out elt bodies
  for (j = 1; j <= Mesh.EltQty; j++)
  {
    if (ut_space_triangle_point_in
	(Nodes.NodeCoo[Mesh.EltNodes[j][0]],
	 Nodes.NodeCoo[Mesh.EltNodes[j][1]],
	 Nodes.NodeCoo[Mesh.EltNodes[j][2]], coo, 1e-6, 1e-6))
    {
      ut_array_1d_memcpy (proj, 3, coo);
      neut_mesh_elt_eq (Mesh, Nodes, j, eq);
      ut_space_projpoint_alongonto (proj, eq + 1, eq);
      d = ut_space_dist (coo, proj);
      if (d < eltdist)
      {
	eltdist = d;
	elt = j;
	ut_array_1d_memcpy (eltcoo, 3, proj);
      }
    }
  }

  // testing out nodes
  neut_mesh_point_closestnode (Mesh, Nodes, coo, &node);
  nodedist = ut_space_dist (coo, Nodes.NodeCoo[node]);

  if (nodedist < eltdist)
  {
    if (pdist)
      (*pdist) = nodedist;
    if (ptcoo)
      ut_array_1d_memcpy (ptcoo, 3, Nodes.NodeCoo[node]);
    if (ptv)
      ut_array_1d_sub (coo, Nodes.NodeCoo[node], 3, ptv);
    if (ptn)
      neut_mesh_node_normal (Mesh, Nodes, node, ptn);
  }
  else
  {
    if (pdist)
      (*pdist) = eltdist;
    if (ptcoo)
      ut_array_1d_memcpy (ptcoo, 3, eltcoo);
    if (ptv)
      ut_array_1d_sub (coo, eltcoo, 3, ptv);
    if (ptn)
      neut_mesh_elt_normal (Mesh, Nodes, elt, ptn);
  }

  ut_free_1d (proj);
  ut_free_1d (eq);

  return (elt > 0) ? 0 : -1;
}

int
neut_mesh_point_closestnode (struct MESH Mesh, struct NODES Nodes,
			     double *coo, int *pnode)
{
  int i, n, nodeqty;
  int *nodes = NULL;
  double dist, distmin;

  neut_mesh_nodes (Mesh, &nodes, &nodeqty);

  (*pnode) = -1;
  distmin = DBL_MAX;
  for (i = 0; i < nodeqty; i++)
  {
    n = nodes[i];
    dist = ut_space_dist (coo, Nodes.NodeCoo[n]);
    if (dist < distmin)
    {
      distmin = dist;
      (*pnode) = n;
    }
  }

  ut_free_1d_int (nodes);

  return (*pnode > 0) ? 0 : -1;
}

int
neut_mesh_point_proj_alongonto (double *Coo, double *n, struct NODES N,
				struct MESH M, int elset)
{
  int j, elt, status;
  double *coo = ut_alloc_1d (3);
  double *eq = ut_alloc_1d (4);

  // FILE* file;
  // struct PART PGarbage;
  // sprintf (message, "face-%d-bef.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");

  status = -1;

  for (j = 1; j <= M.Elsets[elset][0]; j++)
  {
    elt = M.Elsets[elset][j];
    ut_array_1d_memcpy (coo, 3, Coo);
    neut_mesh_elt_eq (M, N, elt, eq);
    ut_space_projpoint_alongonto (coo, n, eq);

    if (ut_space_triangle_point_in (N.NodeCoo[M.EltNodes[elt][0]],
				    N.NodeCoo[M.EltNodes[elt][1]],
				    N.NodeCoo[M.EltNodes[elt][2]], coo,
				    1e-6, 1e-6) == 1)
    {
      ut_array_1d_memcpy (Coo, 3, coo);
      status = elt;
      break;
    }
  }

  ut_free_1d (coo);
  ut_free_1d (eq);

  // sprintf (message, "face-%d-aft.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");

  return status;
}

int
neut_mesh_point_elt_in (struct MESH Mesh, struct NODES Nodes,
			double *coo, int elt)
{
  if (Mesh.Dimension == 3)
      return ut_space_tet_point_in (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
                                    Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
                                    Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
                                    Nodes.NodeCoo[Mesh.EltNodes[elt][3]],
                                    coo);

  else if (Mesh.Dimension == 2)
      return ut_space_triangle_point_in (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
                                         Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
                                         Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
                                         coo, 1e-6, 1e-6);
  else
    abort ();
}

int
neut_mesh_elt2dpair_angle (struct NODES Nodes, struct MESH Mesh,
                           int elt1, int elt2, double *pangle)
{
  double *n1 = ut_alloc_1d (3);
  double *n2 = ut_alloc_1d (3);
  int nodeqty, *nodes = NULL;
  int status;
  int order1, order2;

  neut_mesh_eltpair_comnodes (Mesh, elt1, elt2, &nodes, &nodeqty);

  if (nodeqty == 3)
  {
    status = 2;
    (*pangle) = 180;
  }

  else
  {
    status = 0;

    order1 = neut_mesh_elt2d_nodes_ordering (Mesh, elt1, nodes, nodeqty);
    order2 = neut_mesh_elt2d_nodes_ordering (Mesh, elt2, nodes, nodeqty);

    neut_mesh_elt_normal (Mesh, Nodes, elt1, n1);
    neut_mesh_elt_normal (Mesh, Nodes, elt2, n2);
    if (order1 == order2)
      ut_array_1d_scale (n2, 3, -1);

    (*pangle) = ut_vector_angle (n1, n2);
  }

  ut_free_1d (n1);
  ut_free_1d (n2);
  ut_free_1d_int (nodes);

  return status;
}
