/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include "ol_gnd_rho.h"

int
ol_gnd_nye_rho (struct OL_CRYS Crys, double** nye, double* rho)
{
  int status = 0;

  // simple cube decomposition (Nye) - no crystallography
  if (! strcmp (Crys.rho_min, "sc"))
  {
    ol_gnd_nye_rho_sc (Crys, nye, rho);
    return status;
  }

  // Setting up the problem under the form: A . rho = nye_v
 
  int id, i, j, k, line;
  double*   nyev  = ut_alloc_1d (9); // Vector form of Nye's tensor
  double**  A     = ut_alloc_2d (9, 18);

  id = 0;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      nyev[id++] = nye[i][j];

  double*** d     = ut_alloc_3d (18, 3, 3);
  for (i = 0; i < 18; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
	d[i][j][k] = Crys.gnd_s[i][j] * Crys.gnd_t[i][k];

  for (i = 0; i < 18; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
      {
	line = j * 3 + k;
	A[line][i] = d[i][j][k];
      }
  ut_free_3d (d, 18, 3);

  if (! strcmp (Crys.rho_min, "L2"))
    ol_gnd_nye_rho_l2 (Crys, A, nyev, rho);
  else if (! strcmp (Crys.rho_min, "L1"))
  {
#ifdef HAVE_GLPK
    ol_gnd_nye_rho_l1  (Crys, A, nyev, rho);
#else
    printf ("L1 norm not available (no GLPK support)\n");
#endif
  }
  else if (! strcmp (Crys.rho_min, "energy"))
  {
#ifdef HAVE_GLPK
    ol_gnd_nye_rho_energy  (Crys, A, nyev, rho);
#else
    printf ("L1 norm not available (no GLPK support)\n");
#endif
  }
  else if (! strcmp (Crys.rho_min, "L3"))
  {
#ifdef HAVE_NLOPT
    ol_gnd_nye_rho_norm  (Crys, A, nyev, rho);
#else
    printf ("Arbitrary norm not available (no NLOPT support)\n");
#endif
  }

  ut_free_1d (nyev);
  ut_free_2d (A, 9);

  return status;
}

int
ol_gnd_nye_rho_log (struct OL_CRYS Crys, double** nye, double* rho)
{
  int status;
  size_t k;

  status = ol_gnd_nye_rho (Crys, nye, rho);
  for (k = 0; k < Crys.gnd_size; k++)
    rho[k] = log (ut_num_max (1, rho[k]) / log (10));

  return status;
}


void
ol_gnd_map_nye_rho (struct OL_CRYS Crys, struct OL_GNDMAP* pGnd)
{
  size_t i, j;

  for (i = 1; i < (*pGnd).xsize - 1; i++)
    for (j = 1; j < (*pGnd).ysize - 1; j++)
      if ((*pGnd).id[i][j] == 1)
      {
	ol_gnd_nye_rho (Crys, (*pGnd).nye[i][j], (*pGnd).rho[i][j]);
	(*pGnd).rhosum[i][j] = ut_array_1d_abssum ((*pGnd).rho[i][j], Crys.gnd_size);
      }

  return;
}

void
ol_gnd_map_nye_rho_log (struct OL_CRYS Crys, struct OL_GNDMAP* pGnd)
{
  unsigned int i, j, k;

  ol_gnd_map_nye_rho (Crys, pGnd);

  for (k = 0; k < Crys.gnd_size; k++)
    for (i = 0; i < (*pGnd).xsize; i++)
      for (j = 0; j < (*pGnd).ysize; j++)
	if ((*pGnd).id[i][j] == 1)
	  (*pGnd).rho[i][j][k] = log (ut_num_max (1, (*pGnd).rho[i][j][k])) / log (10);

  for (i = 0; i < (*pGnd).xsize; i++)
    for (j = 0; j < (*pGnd).ysize; j++)
      if ((*pGnd).id[i][j] == 1)
	  (*pGnd).rhosum[i][j] = log (ut_num_max (1, (*pGnd).rhosum[i][j])) / log (10);

  return;
}

void
ol_gnd_map_rho (struct OL_MAP Map, struct OL_CRYS Crys, double thetamin, double thetamax, struct OL_GNDMAP* pGnd)
{
  ol_gnd_map_nye (Map, Crys, thetamin, thetamax, pGnd);
  ol_gnd_map_nye_rho (Crys, pGnd);

  return;
}

void
ol_gnd_map_rho_log (struct OL_MAP Map, struct OL_CRYS Crys, double
    thetamin, double thetamax, struct OL_GNDMAP* pGnd)
{
  ol_gnd_map_nye (Map, Crys, thetamin, thetamax, pGnd);
  ol_gnd_map_nye_rho_log (Crys, pGnd);
  
  return;
}
