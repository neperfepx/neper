/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_seedset_.h"
#include "neut_structs/neut_nfcloud_struct.hpp"

extern void neut_seedset_kdtree_cloud (struct SEEDSET SSet,
                                       NFCLOUD * pnf_cloud,
                                       int **pptid_seedid,
                                       int **pseedid_ptid);
extern void neut_seedset_kdtree_tree (NFCLOUD * pnf_cloud,
                                      NFTREE ** pnf_tree);
extern void neut_seedset_kdtree_update (NFCLOUD * pnf_cloud,
                                        int *seedmovedqty, int seedmoved,
                                        NFTREE ** pnf_tree);
extern void neut_seedset_kdtree_update_seed (struct SEEDSET SSet, int seed,
                                             NFCLOUD * pnf_cloud,
                                             NFTREE ** pnf_tree,
                                             int **pptid_seedid,
                                             int **pseedid_ptid);

void
neut_seedset_set_zero (struct SEEDSET *pSSet)
{
  (*pSSet).N = 0;
  (*pSSet).Nall = 0;
  (*pSSet).Id = 0;
  (*pSSet).Dim = 3;
  (*pSSet).weight = NULL;
  (*pSSet).Random = 0;
  (*pSSet).SeedCoo0 = NULL;
  (*pSSet).SeedCoo = NULL;
  (*pSSet).SeedWeight = NULL;
  (*pSSet).SeedOriTheta = NULL;
  (*pSSet).SeedOriWeight = NULL;
  (*pSSet).LamEq = NULL;
  (*pSSet).LamWidth = NULL;
  (*pSSet).LamWidthId = NULL;

  (*pSSet).SeedOri = NULL;
  (*pSSet).SeedOriR = NULL;
  (*pSSet).SeedOriDistrib = NULL;
  (*pSSet).crysym = NULL;

  (*pSSet).Size = NULL;

  (*pSSet).Type = NULL;
  (*pSSet).Periodic = NULL;
  (*pSSet).PeriodicDist = NULL;
  (*pSSet).PerSeedMaster = NULL;
  (*pSSet).PerSeedShift = NULL;
  (*pSSet).PerSeedSlaveQty = NULL;
  (*pSSet).PerSeedSlaveNb = NULL;

  return;
}

void
neut_seedset_free (struct SEEDSET *pSSet)
{
  if ((*pSSet).Nall < (*pSSet).N)
    abort ();

  ut_free_2d (&(*pSSet).Size, 3);
  ut_free_1d_char (&(*pSSet).weight);
  ut_free_2d (&(*pSSet).SeedCoo0, (*pSSet).N + 1);
  ut_free_2d (&(*pSSet).SeedCoo, (*pSSet).Nall + 1);
  ut_free_1d (&(*pSSet).SeedWeight);
  ut_free_1d (&(*pSSet).SeedOriTheta);
  ut_free_1d (&(*pSSet).SeedOriWeight);
  // UNCOMMENT and DEBUG with
  // neper -T -n from_morpho -morpho "lamellar(w=0.1)" -o new
  // ut_free_2d (&(*pSSet).LamEq, (*pSSet).N + 1);
  ut_free_1d (&(*pSSet).LamWidth);
  ut_free_1d_int (&(*pSSet).LamWidthId);
  ut_free_2d (&(*pSSet).SeedOri, (*pSSet).N + 1);
  ut_free_2d (&(*pSSet).SeedOriR, (*pSSet).N + 1);
  ut_free_2d_char (&(*pSSet).SeedOriDistrib, (*pSSet).N + 1);
  ut_free_1d_char (&(*pSSet).crysym);
  ut_free_1d_char (&(*pSSet).Type);
  ut_free_1d_int (&(*pSSet).Periodic);
  ut_free_1d (&(*pSSet).PeriodicDist);
  if ((*pSSet).PerSeedMaster)
    ut_free_1d_int (&(*pSSet).PerSeedMaster);
  if ((*pSSet).PerSeedShift)
    ut_free_2d_int (&(*pSSet).PerSeedShift, (*pSSet).Nall + 1);
  ut_free_1d_int (&(*pSSet).PerSeedSlaveQty);
  ut_free_2d_int (&(*pSSet).PerSeedSlaveNb, (*pSSet).N + 1);

  neut_seedset_set_zero (pSSet);

  return;
}

int
neut_seedset_seed_master (struct SEEDSET SSet, int seed)
{
  (void) SSet;
  (void) seed;

  printf ("FIXME\n");
  abort ();

  return 0;
}

int
neut_seedset_addseed (struct SEEDSET *pSSet, double *coo, double w)
{
  int i;

  if ((*pSSet).N == 0)
  {
    (*pSSet).SeedCoo0 = ut_alloc_2d (1, 3);
    (*pSSet).SeedCoo = ut_alloc_2d (1, 3);
    if (!(*pSSet).Periodic)
      (*pSSet).Periodic = ut_alloc_1d_int (3);
    if (!(*pSSet).PerSeedMaster)
      (*pSSet).PerSeedMaster = ut_alloc_1d_int (1);
    if (!(*pSSet).PerSeedShift)
      (*pSSet).PerSeedShift = ut_alloc_2d_int (1, 3);
  }

  (*pSSet).N++;

  (*pSSet).SeedCoo0 =
    ut_realloc_2d_addline ((*pSSet).SeedCoo0, (*pSSet).N + 1, 3);
  (*pSSet).SeedCoo =
    ut_realloc_2d_addline ((*pSSet).SeedCoo, (*pSSet).N + 1, 3);
  ut_array_1d_memcpy (coo, 3, (*pSSet).SeedCoo0[(*pSSet).N]);

  if (!(*pSSet).Type || !strcmp ((*pSSet).Type, "standard"))
    ut_array_1d_memcpy (coo, 3, (*pSSet).SeedCoo[(*pSSet).N]);
  else
    for (i = 0; i < (*pSSet).Dim; i++)
      (*pSSet).SeedCoo[(*pSSet).N][i] =
        (*pSSet).SeedCoo0[(*pSSet).N][i] -
        (*pSSet).Periodic[i] * floor ((*pSSet).SeedCoo0[(*pSSet).N][i] /
                                      (*pSSet).Periodic[i]);

  (*pSSet).SeedWeight = ut_realloc_1d ((*pSSet).SeedWeight, (*pSSet).N + 1);
  (*pSSet).SeedWeight[(*pSSet).N] = w;

  (*pSSet).Nall = (*pSSet).N;
  (*pSSet).PerSeedMaster =
    ut_realloc_1d_int ((*pSSet).PerSeedMaster, (*pSSet).Nall + 1);
  (*pSSet).PerSeedShift =
    ut_realloc_2d_int_addline ((*pSSet).PerSeedShift, (*pSSet).Nall + 1, 3);

  return 0;
}

int
neut_seedset_slave_add (struct SEEDSET *pSSet, int master, int *shift)
{
  int id;

  if ((*pSSet).Nall == 0)
  {
    (*pSSet).Nall = (*pSSet).N;
    (*pSSet).PerSeedMaster = ut_alloc_1d_int ((*pSSet).N + 1);
    (*pSSet).PerSeedShift = ut_alloc_1d_pint ((*pSSet).N + 1);
  }

  id = ++(*pSSet).Nall;

  (*pSSet).PerSeedMaster =
    ut_realloc_1d_int ((*pSSet).PerSeedMaster, (*pSSet).Nall + 1);
  (*pSSet).PerSeedMaster[id] = master;
  (*pSSet).PerSeedShift =
    ut_realloc_2d_int_addline ((*pSSet).PerSeedShift, (*pSSet).Nall + 1, 3);
  ut_array_1d_int_memcpy (shift, 3, (*pSSet).PerSeedShift[id]);

  (*pSSet).SeedCoo =
    ut_realloc_2d_addline ((*pSSet).SeedCoo, (*pSSet).Nall + 1, 3);
  (*pSSet).SeedWeight =
    ut_realloc_1d ((*pSSet).SeedWeight, (*pSSet).Nall + 1);

  neut_seedset_slave_setprop (pSSet, id);

  return 0;
}

int
neut_seedset_slave_setprop (struct SEEDSET *pSSet, int id)
{
  int i, master;

  master = (*pSSet).PerSeedMaster[id];

  ut_array_1d_memcpy ((*pSSet).SeedCoo[master], 3, (*pSSet).SeedCoo[id]);
  for (i = 0; i < 3; i++)
    (*pSSet).SeedCoo[id][i] +=
      (*pSSet).PerSeedShift[id][i] * (*pSSet).PeriodicDist[i];

  (*pSSet).SeedWeight[id] = (*pSSet).SeedWeight[master];

  return 0;
}

int
neut_seedset_update_slaves (struct SEEDSET *pSSet)
{
  int i;

  for (i = (*pSSet).N + 1; i <= (*pSSet).Nall; i++)
    neut_seedset_slave_setprop (pSSet, i);

  return 0;
}

void
neut_seedset_init_seedslave (struct SEEDSET *pSSet)
{
  int master, slave;

  (*pSSet).PerSeedSlaveQty = ut_alloc_1d_int ((*pSSet).N + 1);
  (*pSSet).PerSeedSlaveNb = ut_alloc_1d_pint ((*pSSet).N + 1);

  for (slave = (*pSSet).N + 1; slave <= (*pSSet).Nall; slave++)
  {
    master = (*pSSet).PerSeedMaster[slave];

    (*pSSet).PerSeedSlaveQty[master]++;
    (*pSSet).PerSeedSlaveNb[master] =
      ut_realloc_1d_int ((*pSSet).PerSeedSlaveNb[master],
                         (*pSSet).PerSeedSlaveQty[master] + 1);
    (*pSSet).PerSeedSlaveNb[master][(*pSSet).PerSeedSlaveQty[master]] = slave;
  }

  return;
}

int
neut_seedset_seedcoo0toseedcoo (struct SEEDSET *pSSet)
{
  int i;

  if (!(*pSSet).SeedCoo)
    (*pSSet).SeedCoo = ut_alloc_2d ((*pSSet).N + 1, 3);

  for (i = 1; i <= (*pSSet).N; i++)
    neut_seedset_seed_update_fromseedcoo0 (pSSet, i);

  return 0;
}

int
neut_seedset_seedcootoseedcoo0 (struct SEEDSET *pSSet)
{
  int i;

  if (!(*pSSet).SeedCoo0)
    (*pSSet).SeedCoo0 = ut_alloc_2d ((*pSSet).N + 1, 3);

  for (i = 1; i <= (*pSSet).N; i++)
    ut_array_1d_memcpy ((*pSSet).SeedCoo[i], 3, (*pSSet).SeedCoo0[i]);

  return 0;
}

int
neut_seedset_seed_update_fromseedcoo0 (struct SEEDSET *pSSet, int id)
{
  int i;
  double alpha;

  if (!strcmp ((*pSSet).Type, "standard"))
    ut_array_1d_memcpy ((*pSSet).SeedCoo0[id], 3, (*pSSet).SeedCoo[id]);
  else
    for (i = 0; i < (*pSSet).Dim; i++)
    {
      if ((*pSSet).Periodic[i])
      {
        alpha =
          floor (((*pSSet).SeedCoo0[id][i] -
                  (*pSSet).Size[i][0]) / (*pSSet).PeriodicDist[i]);
        (*pSSet).SeedCoo[id][i] =
          (*pSSet).SeedCoo0[id][i] - alpha * (*pSSet).PeriodicDist[i];

        if ((*pSSet).SeedCoo[id][i] < 0
            || (*pSSet).SeedCoo[id][i] > (*pSSet).PeriodicDist[i])
          abort ();
      }
      else
        (*pSSet).SeedCoo[id][i] = (*pSSet).SeedCoo0[id][i];
    }

  if (!strncmp ((*pSSet).Type, "periodic", 8))
    neut_seedset_seed_updateslaves (pSSet, id);

  return 0;
}

int
neut_seedset_seed_update_fromseedorir (struct SEEDSET *pSSet, int id)
{
  ol_R_q ((*pSSet).SeedOriR[id], (*pSSet).SeedOri[id]);

  return 0;
}

void
neut_seedset_seed_updateslaves (struct SEEDSET *pSSet, int id)
{
  int i;

  for (i = 1; i <= (*pSSet).PerSeedSlaveQty[id]; i++)
    neut_seedset_slave_setprop (pSSet, (*pSSet).PerSeedSlaveNb[id][i]);

  return;
}

double
neut_seedset_voidcell_distance (struct SEEDSET SSet, int seed)
{
  int i;
  double dist, res, *plane = ut_alloc_1d (4);

  res = 0;
  for (i = 1; i <= SSet.Nall; i++)
  {
    if (i == seed)
      continue;

    ut_space_points_bisplane_power (SSet.SeedCoo[seed], SSet.SeedCoo[i],
                                    SSet.SeedWeight[seed], SSet.SeedWeight[i],
                                    plane);
    dist = ut_space_point_plane_dist_signed (SSet.SeedCoo[seed], plane);

    if (dist > 0)
      res += dist;
  }

  ut_free_1d (&plane);

  return res;
}

void
neut_seedset_memcpy (struct SEEDSET SSetA, struct SEEDSET *pSSetB)
{
  neut_seedset_free (pSSetB);
  neut_seedset_memcpy_general (SSetA, pSSetB);
  neut_seedset_memcpy_seeds (SSetA, pSSetB);
  neut_seedset_memcpy_periodic (SSetA, pSSetB);

  return;
}

void
neut_seedset_memcpy_seed_addseed (struct SEEDSET SSetA, int seed,
                                  struct SEEDSET *pSSetB)
{
  int newseed = ++(*pSSetB).N;

  if (newseed == 1)
  {
    (*pSSetB).SeedOri = ut_alloc_2d (1, 1);
    (*pSSetB).SeedOriDistrib = ut_alloc_2d_char (1, 1);
    (*pSSetB).SeedCoo0 = ut_alloc_2d (1, 3);
    (*pSSetB).SeedCoo = ut_alloc_2d (1, 3);
    if (SSetA.LamEq)
      (*pSSetB).LamEq = ut_alloc_2d (1, 4);
  }

  if (SSetA.LamEq)
  {
    (*pSSetB).LamEq =
      ut_realloc_2d_addline ((*pSSetB).LamEq, (*pSSetB).N + 1, 4);
    ut_array_1d_memcpy (SSetA.LamEq[seed], 4, (*pSSetB).LamEq[newseed]);
  }

  if (SSetA.LamWidth)
  {
    (*pSSetB).LamWidth = ut_realloc_1d ((*pSSetB).LamWidth, (*pSSetB).N + 1);
    (*pSSetB).LamWidth[newseed] = SSetA.LamWidth[seed];
  }

  if (SSetA.LamWidthId)
  {
    (*pSSetB).LamWidthId =
      ut_realloc_1d_int ((*pSSetB).LamWidthId, (*pSSetB).N + 1);
    (*pSSetB).LamWidthId[newseed] = SSetA.LamWidthId[seed];
  }

  (*pSSetB).SeedCoo0 =
    ut_realloc_2d_addline ((*pSSetB).SeedCoo0, (*pSSetB).N + 1, 3);
  ut_array_1d_memcpy (SSetA.SeedCoo0[seed], 3, (*pSSetB).SeedCoo0[newseed]);

  (*pSSetB).SeedCoo =
    ut_realloc_2d_addline ((*pSSetB).SeedCoo, (*pSSetB).N + 1, 3);
  ut_array_1d_memcpy (SSetA.SeedCoo[seed], 3, (*pSSetB).SeedCoo[newseed]);

  (*pSSetB).SeedWeight =
    ut_realloc_1d ((*pSSetB).SeedWeight, (*pSSetB).N + 1);
  (*pSSetB).SeedWeight[newseed] = SSetA.SeedWeight[seed];

  (*pSSetB).SeedOri = ut_realloc_2d_addline ((*pSSetB).SeedOri, (*pSSetB).N + 1, 4);
  ut_array_1d_memcpy (SSetA.SeedOri[seed], 4, (*pSSetB).SeedOri[newseed]);

  ut_string_string (SSetA.SeedOriDistrib[seed], &(*pSSetB).SeedOriDistrib[newseed]);

  return;
}

void
neut_seedset_activedim (struct SEEDSET SSet, char *morpho, int **pdim,
                        int *pdimqty)
{
  (*pdim) = ut_alloc_1d_int (3);
  (*pdimqty) = 0;

  if (!strcmp (morpho, "columnar(x)"))
  {
    (*pdim)[(*pdimqty)++] = 1;
    (*pdim)[(*pdimqty)++] = 2;
  }
  else if (!strcmp (morpho, "columnar(y)"))
  {
    (*pdim)[(*pdimqty)++] = 0;
    (*pdim)[(*pdimqty)++] = 2;
  }
  else if (!strcmp (morpho, "columnar(z)"))
  {
    (*pdim)[(*pdimqty)++] = 0;
    (*pdim)[(*pdimqty)++] = 1;
  }
  else if (!strcmp (morpho, "bamboo(x)"))
    (*pdim)[(*pdimqty)++] = 0;
  else if (!strcmp (morpho, "bamboo(y)"))
    (*pdim)[(*pdimqty)++] = 1;
  else if (!strcmp (morpho, "bamboo(z)"))
    (*pdim)[(*pdimqty)++] = 2;
  else
  {
    (*pdim)[(*pdimqty)++] = 0;
    (*pdim)[(*pdimqty)++] = 1;
    (*pdim)[(*pdimqty)++] = 2;
  }

  if (SSet.Dim == 2)
    ut_array_1d_int_list_rmval (pdim, pdimqty, 2);

  return;
}

void
neut_seedset_seed_randomize (struct SEEDSET *pSSet, int seed, gsl_rng * r,
                             int *dims, int dimqty, double mindist,
                             double maxdist)
{
  int i;
  double *v = ut_alloc_1d (3);

  ut_space_random (r, dims, dimqty, mindist, maxdist, v);

  for (i = 0; i < dimqty; i++)
    (*pSSet).SeedCoo[seed][dims[i]] += v[dims[i]];

  ut_free_1d (&v);

  return;
}

void
neut_seedset_bbox_size (struct SEEDSET SSet, double *psize)
{
  if (SSet.Dim == 3)
    (*psize) =
      (SSet.Size[2][1] - SSet.Size[2][0]) * (SSet.Size[1][1] -
                                             SSet.Size[1][0]) *
      (SSet.Size[0][1] - SSet.Size[0][0]);
  else if (SSet.Dim == 2)
    (*psize) =
      (SSet.Size[1][1] - SSet.Size[1][0]) * (SSet.Size[0][1] -
                                             SSet.Size[0][0]);
  else
    abort ();

  return;
}

void
neut_seedset_kdtree (struct SEEDSET SSet, NFCLOUD * pnf_cloud,
                     NFTREE ** pnf_tree, int **pptid_seedid,
                     int **pseedid_ptid)
{
  neut_seedset_kdtree_cloud (SSet, pnf_cloud, pptid_seedid, pseedid_ptid);
  neut_seedset_kdtree_tree (pnf_cloud, pnf_tree);

  return;
}

void
neut_seedset_kdtree_update (struct SEEDSET SSet, int *seedmoved,
                            int seedmovedqty, NFCLOUD * pnf_cloud,
                            NFTREE ** pnf_tree, int **pptid_seedid,
                            int **pseedid_ptid)
{
  int i, j;

  for (i = 0; i < seedmovedqty; i++)
    neut_seedset_kdtree_update_seed (SSet, seedmoved[i], pnf_cloud, pnf_tree,
                                     pptid_seedid, pseedid_ptid);

  if (SSet.Type[0] == 'p')
    for (i = 0; i < seedmovedqty; i++)
      for (j = 1; j <= SSet.PerSeedSlaveQty[seedmoved[i]]; j++)
        neut_seedset_kdtree_update_seed (SSet,
                                         SSet.PerSeedSlaveNb[seedmoved[i]][j],
                                         pnf_cloud, pnf_tree, pptid_seedid,
                                         pseedid_ptid);

  return;
}

void
neut_seedset_bcc (struct TESS Dom, int n, struct SEEDSET *pSSet)
{
  int i, x, y, z, nb, N = ut_num_d2ri (pow (n, 3) + pow (n + 1, 3));
  double step[3], **bbox = ut_alloc_2d (3, 2);

  (*pSSet).SeedCoo = ut_alloc_2d (N + 1, 3);

  neut_tess_bbox (Dom, bbox);
  for (i = 0; i < 3; i++)
    step[i] = (bbox[i][1] - bbox[i][0]) / n;

  nb = 0;
  for (x = 1; x <= n; x++)
    for (y = 1; y <= n; y++)
      for (z = 1; z <= n; z++)
      {
        nb++;
        (*pSSet).SeedCoo[nb][0] = bbox[0][0] + (x - 0.5) * step[0];
        (*pSSet).SeedCoo[nb][1] = bbox[1][0] + (y - 0.5) * step[1];
        (*pSSet).SeedCoo[nb][2] = bbox[2][0] + (z - 0.5) * step[2];
      }

  for (x = 0; x <= n; x++)
    for (y = 0; y <= n; y++)
      for (z = 0; z <= n; z++)
      {
        nb++;
        (*pSSet).SeedCoo[nb][0] = bbox[0][0] + x * step[0];
        (*pSSet).SeedCoo[nb][1] = bbox[1][0] + y * step[1];
        (*pSSet).SeedCoo[nb][2] = bbox[2][0] + z * step[2];
      }

  ut_free_2d (&bbox, 3);
  (*pSSet).N = nb;

  return;
}

void
neut_seedset_bcc_expr (struct TESS Dom, char *cooexpr, struct SEEDSET *pSSet)
{
  int n;

  sscanf (cooexpr, "bcc(%d)", &n);

  neut_seedset_bcc (Dom, n, pSSet);

  return;
}
