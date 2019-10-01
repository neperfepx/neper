/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the tgeom-level directory. */

#include "neut_tesr_geom_.h"

int
neut_tesr_vox_pos (struct TESR Tesr, int vox, int *pos)
{
  int qty, unitqty;

  qty = vox;
  unitqty = Tesr.size[0] * Tesr.size[1];
  pos[2] = 1 + (qty - 1) / unitqty;

  qty -= (pos[2] - 1) * unitqty;
  unitqty = Tesr.size[0];
  pos[1] = 1 + (qty - 1) / unitqty;

  qty -= (pos[1] - 1) * unitqty;
  pos[0] = qty;

  return 0;
}

int
neut_tesr_pos3_vox (struct TESR Tesr, int x, int y, int z, int *pvox)
{
  (*pvox) = (z - 1) * Tesr.size[1] * Tesr.size[0]
          + (y - 1) * Tesr.size[0]
          + x;

  return 0;
}

int
neut_tesr_pos_vox (struct TESR Tesr, int *pos, int *pvox)
{
  return neut_tesr_pos3_vox (Tesr, pos[0], pos[1], pos[2], pvox);
}

int
neut_tesr_vox_coo (struct TESR Tesr, int vox, double *coo)
{
  int pos[3];

  neut_tesr_vox_pos (Tesr, vox, pos);
  neut_tesr_pos_coo (Tesr, pos, coo);

  return 0;
}

int
neut_tesr_vox_cell (struct TESR Tesr, int vox, int *pcell)
{
  int voxpos[3];

  neut_tesr_vox_pos (Tesr, vox, voxpos);
  (*pcell) = Tesr.VoxCell[voxpos[0]][voxpos[1]][voxpos[2]];

  return 0;
}

int
neut_tesr_coo_pos (struct TESR Tesr, double *coo, int mode, int *voxpos)
{
  int i;

  for (i = 0; i < Tesr.Dim; i++)
  {
    voxpos[i] = ceil ((coo[i] - Tesr.Origin[i]) / Tesr.vsize[i] + ut_num_sgn_int (mode) * 1e-6);
    voxpos[i] = ut_num_min_int (voxpos[i], Tesr.size[i]);
  }
  for (i = Tesr.Dim; i < 3; i++)
    voxpos[i] = 1;

  return 0;
}

int
neut_tesr_pos_coo (struct TESR Tesr, int *voxpos, double *coo)
{
  int i;

  for (i = 0; i < Tesr.Dim; i++)
    coo[i] = Tesr.Origin[i] + (voxpos[i] - 0.5) * Tesr.vsize[i];

  return 0;
}

int
neut_tesr_cell_voxqty (struct TESR Tesr, int cell)
{
  int i, j, k, vqty;

  vqty = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
	if (Tesr.VoxCell[i][j][k] == cell)
	  vqty++;

  return vqty;
}

int
neut_tesr_voxqty (struct TESR Tesr)
{
  int i, j, k, vqty;

  vqty = 0;
  for (k = 1; k <= Tesr.size[2]; k++)
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	if (Tesr.VoxCell[i][j][k] != 0)
	  vqty++;

  return vqty;
}

double
neut_tesr_avcellvoxqty (struct TESR Tesr)
{
  return (double) neut_tesr_voxqty (Tesr) / Tesr.CellQty;
}

int
neut_tesr_cell_volume (struct TESR Tesr, int cell, double *pvol)
{
  int vqty;
  double voxvol;

  if (Tesr.CellVol)
    (*pvol) = Tesr.CellVol[cell];

  else
  {
    vqty = neut_tesr_cell_voxqty (Tesr, cell);
    neut_tesr_voxsize (Tesr, &voxvol);
    (*pvol) = voxvol * vqty;
  }

  return (*pvol > 0) ? 0 : -1;
}

int
neut_tesr_cell_area (struct TESR Tesr, int cell, double *parea)
{
  int i, j;

  if (Tesr.Dim != 2)
    abort ();

  if (Tesr.CellVol)
    (*parea) = Tesr.CellVol[cell];
  else
  {
    (*parea) = 0;
    for (j = 1; j <= Tesr.size[1]; j++)
      for (i = 1; i <= Tesr.size[0]; i++)
	if (Tesr.VoxCell[i][j][1] == cell)
	  (*parea)++;

    (*parea) *= (Tesr.vsize[0] * Tesr.vsize[1]);
  }

  return 0;
}

int
neut_tesr_cell_size (struct TESR Tesr, int cell, double *psize)
{
  if (Tesr.Dim == 2)
    neut_tesr_cell_area (Tesr, cell, psize);
  else if (Tesr.Dim == 3)
    neut_tesr_cell_volume (Tesr, cell, psize);
  else
    abort ();

  return 0;
}

int
neut_tesr_cell_centre (struct TESR Tesr, int cell, double *coo)
{
  int i, j, k, qty;
  int *voxpos = NULL;
  double *voxcoo = NULL;

  if (Tesr.CellCoo)
  {
    ut_array_1d_memcpy (coo, 3, Tesr.CellCoo[cell]);
    return 0;
  }

  voxpos = ut_alloc_1d_int (3);
  voxcoo = ut_alloc_1d (3);

  qty = 0;
  ut_array_1d_zero (coo, 3);
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
	if (Tesr.VoxCell[i][j][k] == cell)
	{
	  ut_array_1d_int_set_3 (voxpos, i, j, k);
	  neut_tesr_pos_coo (Tesr, voxpos, voxcoo);
	  ut_array_1d_add (coo, voxcoo, 3, coo);
	  qty++;
	}

  if (qty > 0)
    ut_array_1d_scale (coo, 3, 1. / qty);

  ut_free_1d_int (voxpos);
  ut_free_1d (voxcoo);

  return (qty > 0) ? 0 : -1;
}

void
neut_tesr_rastercentre (struct TESR Tesr, double *coo)
{
  int i;

  for (i = 0; i < 3; i++)
    coo[i] = Tesr.Origin[i] + 0.5 * Tesr.vsize[i] * Tesr.size[i];

  return;
}

void
neut_tesr_centre (struct TESR Tesr, double *coo)
{
  int i, j;
  double *vol = NULL, **cellcoo = NULL;

  if (!Tesr.hasvoid)
  {
    neut_tesr_rastercentre (Tesr, coo);
    return;
  }

  vol = ut_alloc_1d (Tesr.CellQty);
  cellcoo = ut_alloc_2d (Tesr.CellQty, 3);

  for (i = 0; i < Tesr.CellQty; i++)
  {
    neut_tesr_cell_volume (Tesr, i + 1, vol + i);
    neut_tesr_cell_centre (Tesr, i + 1, cellcoo[i]);
  }

  ut_array_1d_zero (coo, 3);
  for (i = 0; i < Tesr.CellQty; i++)
    for (j = 0; j < 3; j++)
      coo[j] += vol[i] * cellcoo[i][j];
  ut_array_1d_scale (coo, 3, 1. / ut_array_1d_sum (vol, Tesr.CellQty));
  ut_array_1d_add (coo, Tesr.Origin, 3, coo);

  ut_free_1d (vol);
  ut_free_2d (cellcoo, Tesr.CellQty);

  return;
}

void
neut_tesr_cell_diameq (struct TESR Tesr, int cell, double *pval)
{
  if (Tesr.Dim == 3)
  {
    if (Tesr.CellVol)
      (*pval) = Tesr.CellVol[cell];
    else
      neut_tesr_cell_volume (Tesr, cell, pval);
    (*pval) = pow ((6 / M_PI) * (*pval), 0.3333333333333333333333333);
  }
  else if (Tesr.Dim == 2)
  {
    if (Tesr.CellVol)
      (*pval) = Tesr.CellVol[cell];
    else
      neut_tesr_cell_area (Tesr, cell, pval);
    (*pval) = sqrt ((4 / M_PI) * (*pval));
  }

  else
    abort ();

  return;
}

void
neut_tesr_cell_radeq (struct TESR Tesr, int cell, double *pval)
{
  neut_tesr_cell_diameq (Tesr, cell, pval);
  (*pval) *= 0.5;

  return;
}

void
neut_tesr_cell_points (struct TESR Tesr, int cell, int ***ppts, int *pptqty)
{
  int i, j, k;

  if (!Tesr.CellBBox)
    ut_error_reportbug ();

  (*pptqty) = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
	if (Tesr.VoxCell[i][j][k] == cell)
	{
	  (*pptqty)++;
	  (*ppts) = ut_realloc_2d_int_addline (*ppts, *pptqty, 3);
	  ut_array_1d_int_set_3 ((*ppts)[(*pptqty) - 1], i, j, k);
	}

  return;
}

void
neut_tesr_cell_coos (struct TESR Tesr, int cell, double ***pcoos,
		     int *pcooqty)
{
  int i, **pts = NULL;

  neut_tesr_cell_points (Tesr, cell, &pts, pcooqty);
  // ut_array_2d_int_fprintf (stdout, pts, *pcooqty, 3, "%d");

  (*pcoos) = ut_alloc_2d (*pcooqty, Tesr.Dim);
  for (i = 0; i < *pcooqty; i++)
    neut_tesr_pos_coo (Tesr, pts[i], (*pcoos)[i]);

  ut_free_2d_int (pts, *pcooqty);

  return;
}

void
neut_tesr_cell_boundpoints (struct TESR Tesr, int cell, int ***ppts,
			    int *pptqty, int connec, int interior)
{
  int i, j, k;

  if (!Tesr.CellBBox)
    ut_error_reportbug ();

  (*pptqty) = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
	if (Tesr.VoxCell[i][j][k] == cell)
	  if (neut_tesr_cell_boundpoints_test (Tesr, cell, i, j, k, connec, interior))
	  {
	    (*pptqty)++;
	    (*ppts) = ut_realloc_2d_int_addline (*ppts, *pptqty, 3);
	    ut_array_1d_int_set_3 ((*ppts)[(*pptqty) - 1], i, j, k);
	  }

  return;
}

void
neut_tesr_cell_cornerpoints (struct TESR Tesr, int cell, int ***ppts,
			     int *pptqty)
{
  int i, j, k;

  if (!Tesr.CellBBox)
    ut_error_reportbug ();

  (*pptqty) = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
	if (Tesr.VoxCell[i][j][k] == cell)
	{
	  // neighbours along x
	  if (Tesr.VoxCell[i - 1][j][k] == cell
	      && Tesr.VoxCell[i + 1][j][k] == cell)
	    continue;
	  // neighbours along y
	  if (Tesr.VoxCell[i][j - 1][k] == cell
	      && Tesr.VoxCell[i][j + 1][k] == cell)
	    continue;
	  // neighbours along z
	  if (Tesr.VoxCell[i][j][k - 1] == cell
	      && Tesr.VoxCell[i][j][k + 1] == cell)
	    continue;
	  // neighbours along x y
	  if (Tesr.VoxCell[i - 1][j - 1][k] == cell
	      && Tesr.VoxCell[i + 1][j + 1][k] == cell)
	    continue;
	  // neighbours along x -y
	  if (Tesr.VoxCell[i - 1][j + 1][k] == cell
	      && Tesr.VoxCell[i + 1][j - 1][k] == cell)
	    continue;
	  // neighbours along y z
	  if (Tesr.VoxCell[i][j - 1][k - 1] == cell
	      && Tesr.VoxCell[i][j + 1][k + 1] == cell)
	    continue;
	  // neighbours along y -z
	  if (Tesr.VoxCell[i][j - 1][k + 1] == cell
	      && Tesr.VoxCell[i][j + 1][k - 1] == cell)
	    continue;
	  // neighbours along z x
	  if (Tesr.VoxCell[i - 1][j][k - 1] == cell
	      && Tesr.VoxCell[i + 1][j][k + 1] == cell)
	    continue;
	  // neighbours along z -x
	  if (Tesr.VoxCell[i + 1][j][k - 1] == cell
	      && Tesr.VoxCell[i - 1][j][k + 1] == cell)
	    continue;

	  (*pptqty)++;
	  (*ppts) = ut_realloc_2d_int_addline (*ppts, *pptqty, 3);
	  ut_array_1d_int_set_3 ((*ppts)[(*pptqty) - 1], i, j, k);
	}

  return;
}

void
neut_tesr_cell_convexity (struct TESR Tesr, int cell, double *pval)
{
  int i, j, k, qty, **pts = NULL;
  double voxvol, dist, vol, vol2, **coos = NULL;
  struct NODES N;
  struct MESH M;
  int *pt = NULL;
  double *coo = NULL;
  double eps = 1e-4 * ut_array_1d_min (Tesr.vsize, Tesr.Dim);
  double eps3 = 1e-2 * ut_array_1d_gmean (Tesr.vsize, Tesr.Dim);
  srand48 (1);

  if (Tesr.CellConvexity)
  {
    (*pval) = Tesr.CellConvexity[cell];
    return;
  }

  pt = ut_alloc_1d_int (3);
  coo = ut_alloc_1d (3);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);

  neut_tesr_cell_cornerpoints (Tesr, cell, &pts, &qty);

  coos = ut_alloc_2d (qty, 3);
  for (i = 0; i < qty; i++)
  {
    neut_tesr_pos_coo (Tesr, pts[i], coos[i]);
    for (j = 0; j < Tesr.Dim; j++)
      coos[i][j] += eps * drand48 ();
  }

  net_pts_convexhull (coos, qty, 3, &N, &M);
  vol = 0;
  for (k = Tesr.CellBBox[cell][2][0]; k <= Tesr.CellBBox[cell][2][1]; k++)
    for (j = Tesr.CellBBox[cell][1][0]; j <= Tesr.CellBBox[cell][1][1]; j++)
      for (i = Tesr.CellBBox[cell][0][0]; i <= Tesr.CellBBox[cell][0][1]; i++)
      {
	if (Tesr.VoxCell[i][j][k] == cell)
	  vol++;
	else
	{
	  ut_array_1d_int_set_3 (pt, i, j, k);
	  neut_tesr_pos_coo (Tesr, pt, coo);
	  ut_space_polypts_point_dist (coos, qty, coo, &dist);
	  if (dist < eps3)
	    vol++;
	}
      }
  neut_tesr_voxsize (Tesr, &voxvol);
  vol *= voxvol;
  neut_tesr_cell_volume (Tesr, cell, &vol2);

  (*pval) = vol2 / vol;
  if (*pval > 1 + 1e-9 || *pval < 0)
    ut_error_reportbug ();

  ut_free_2d_int (pts, qty);
  ut_free_2d (coos, qty);
  ut_free_1d (coo);
  ut_free_1d_int (pt);

  neut_nodes_free (&N);
  neut_mesh_free (&M);

  return;
}

void
neut_tesr_cell_aniso (struct TESR Tesr, int cell,
		      double **evect, double *eval)
{
  int i, cooqty = 0;
  double **coos = NULL;
  double *centre = ut_alloc_1d (Tesr.Dim);
  double **S = ut_alloc_2d (Tesr.Dim, Tesr.Dim);

  neut_tesr_cell_centre (Tesr, cell, centre);
  neut_tesr_cell_coos (Tesr, cell, &coos, &cooqty);
  for (i = 0; i < cooqty; i++)
    ut_array_1d_sub (coos[i], centre, Tesr.Dim, coos[i]);
  ut_vector_set_covar (coos, Tesr.Dim, cooqty, S);
  ut_mat_eigen (Tesr.Dim, S, eval, evect);

  ut_free_1d (centre);
  ut_free_2d (coos, cooqty);

  ut_free_2d (S, Tesr.Dim);

  return;
}

void
neut_tesr_cell_anisoxyz (struct TESR Tesr, int cell, double *fact)
{
  int i, j, cooqty = 0;
  double **coos = NULL;
  double *centre = ut_alloc_1d (Tesr.Dim);
  double **S = ut_alloc_2d (Tesr.Dim, Tesr.Dim);

  neut_tesr_cell_coos (Tesr, cell, &coos, &cooqty);
  neut_tesr_cell_centre (Tesr, cell, centre);
  for (i = 0; i < cooqty; i++)
    ut_array_1d_sub (coos[i], centre, Tesr.Dim, coos[i]);

  for (i = 0; i < Tesr.Dim; i++)
  {
    fact[i] = 0;
    for (j = 0; j < cooqty; j++)
      fact[i] += pow (coos[j][i], 2);
    fact[i] = sqrt (fact[i]);
  }
  ut_array_1d_scale (fact, Tesr.Dim,
		     1. / pow (ut_array_1d_prod (fact, Tesr.Dim),
			       1. / Tesr.Dim));

  ut_free_1d (centre);
  ut_free_2d (coos, cooqty);
  ut_free_2d (S, Tesr.Dim);

  return;
}

void
neut_tesr_cells_anisoxyz (struct TESR Tesr, double *fact)
{
  int i;
  double vol;
  double **facts = ut_alloc_2d (Tesr.CellQty + 1, Tesr.Dim);
  double *vols = ut_alloc_1d (Tesr.CellQty + 1);

  vol = 0;
  ut_array_1d_zero (fact, 3);
  for (i = 1; i <= Tesr.CellQty; i++)
  {
    neut_tesr_cell_anisoxyz (Tesr, i, facts[i]);
    neut_tesr_cell_volume (Tesr, i, vols + i);
    ut_array_1d_scale (facts[i], Tesr.Dim, vols[i]);
    ut_array_1d_add (facts[i], fact, Tesr.Dim, fact);
    vol += vols[i];
  }
  ut_array_1d_scale (fact, Tesr.Dim, 1. / vol);

  ut_array_1d_scale (fact, Tesr.Dim,
		     1. / pow (ut_array_1d_prod (fact, Tesr.Dim),
			       1. / Tesr.Dim));

  ut_free_2d (facts, Tesr.CellQty);
  ut_free_1d (vols);

  return;
}

int
neut_tesr_perpos_pos (struct TESR Tesr, int *per, int *pos, int *pos2)
{
  int i;

  ut_array_1d_int_memcpy (pos2, Tesr.Dim, pos);

  for (i = 0; i < Tesr.Dim; i++)
    if (pos[i] < 1 || pos[i] > Tesr.size[i])
      if (!per || per[i])
      {
        while (pos2[i] < 1)
          pos2[i] += Tesr.size[i];
        while (pos2[i] > Tesr.size[i])
          pos2[i] -= Tesr.size[i];
      }

  return 0;
}

void
neut_tesr_bbox (struct TESR Tesr, double **bbox)
{
  int i;

  ut_array_2d_set (bbox, 3, 1, 0);

  for (i = 0; i < 3; i++)
  {
    bbox[i][0] = Tesr.Origin[i];
    bbox[i][1] = Tesr.Origin[i] + Tesr.size[i] * Tesr.vsize[i];
  }

  return;
}

void
neut_tesr_bboxsize (struct TESR Tesr, double *bboxsize)
{
  int i;
  double **bbox = ut_alloc_2d (3, 2);

  neut_tesr_bbox (Tesr, bbox);
  for (i = 0; i < 3; i++)
    bboxsize[i] = bbox[i][1] - bbox[i][0];

  ut_free_2d (bbox, 3);

  return;
}
