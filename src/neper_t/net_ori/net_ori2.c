/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_.h"

void
net_ori_random (long random, struct OL_SET *pOSet)
{
  unsigned int i;
  double *e = ol_e_alloc ();
  gsl_rng *r = NULL;
  double n1, n2, n3;

  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, random - 1);

  for (i = 0; i < (*pOSet).size; i++)
  {
    // keep the n1 n2 n3 stuff for backward compatibility.
    // Calling the gsl as an argument switches n1 and n3.
    n1 = gsl_rng_uniform (r);
    n2 = gsl_rng_uniform (r);
    n3 = gsl_rng_uniform (r);
    ol_nb_e (n1, n2, n3, e);
    ol_e_q (e, (*pOSet).q[i]);
  }

  gsl_rng_free (r);
  ol_e_free (e);

  return;
}

// Author: L. Renversade.
// fixed rq
void
net_ori_fibre (long random, char *distrib, struct OL_SET *pOSet)
{
  unsigned int i;
  unsigned char c;
  double theta, dirs[3], dirc[3];
  double *q_align = ol_q_alloc ();
  double *q_rand = ol_q_alloc ();

  gsl_rng *rnd = NULL;

  rnd = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rnd, random - 1);

  if (sscanf (distrib, "fibre(%lf,%lf,%lf,%lf,%lf,%lf)", dirs, dirs + 1,
                   dirs + 2, dirc, dirc + 1, dirc + 2) == 6)
  {
    ut_vector_uvect (dirs, dirs);
    ut_vector_uvect (dirc, dirc);
  }

  // legacy
  else if (sscanf (distrib, "fibre(%c,%lf,%lf,%lf)", &c, dirc, dirc + 1, dirc + 2) == 4)
  {
    if (c == 'x' || c == 'y' || c == 'z')
      for (i = 0; i < 3; i++)
        dirs[i] = c == 'x' + i;
    else
      ut_print_message (2, 2, "Failed to parse expression `%s'.\n", distrib);

    ut_print_message (1, 2, "Argument `%s' is deprecated and will not be supported in future versions.  See the documentation.\n", distrib);

    ut_vector_uvect (dirc, dirc);
  }

  else
    ut_print_message (2, 2, "Failed to parse expression `%s'.\n", distrib);

  // 1st rotation, to get in the fibre
  ol_vect_vect_q (dirc, dirs, q_align);

  // 2nd rotation, about the fibre
  for (i = 0; i < (*pOSet).size; i++)
  {
    theta = 2.0 * M_PI * gsl_rng_uniform (rnd);
    ol_rtheta_q_rad (dirs, theta, q_rand);
    ol_q_q_q_ref (q_align, q_rand, (*pOSet).q[i]);
  }

  gsl_rng_free (rnd);
  ol_q_free (q_align);
  ol_q_free (q_rand);

  return;
}

void
net_ori_equal (struct SEEDSET *SSet, int dtess,
               int dcell, struct OL_SET *pOSet)
{
  unsigned int i;

  for (i = 0; i < (*pOSet).size; i++)
    ut_array_1d_memcpy ((*pOSet).q[i], 4, SSet[dtess].q[dcell]);

  return;
}

void
net_ori_spread (char *ori,
                struct SEEDSET *SSet, int dtess,
                int dcell, struct OL_SET *pOSet)
{
  unsigned int i, j;
  double sig, mean;
  double *v = ol_r_alloc ();
  double *q = ol_q_alloc ();
  int varqty;
  char** vars = NULL;
  char** vals = NULL;

  ut_string_function_separate (ori, NULL, &vars, &vals, &varqty);

  if (varqty != 1)
    abort ();

  if (!vars || !vars[0] || !strcmp (vars[0], "mean"))
  {
    sscanf (vals[0], "%lf", &mean);
    sig = mean / (2 * sqrt (2 / M_PI)); // Glez and Driver, J. Appl. Cryst., 2001
  }
  else
    abort ();

  gsl_rng *rand = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (rand, SSet[dtess].Random);

  for (i = 0; i < (*pOSet).size; i++)
  {
    for (j = 0; j < 3; j++)
      v[j] = gsl_ran_gaussian (rand, sig / 2);

    ol_lnq_q (v, q);

    ol_q_q_q (SSet[dtess].q[dcell], q, (*pOSet).q[i]);
  }

  gsl_rng_free (rand);
  ol_q_free (q);
  ut_free_2d_char (vars, varqty);
  ut_free_2d_char (vals, varqty);

  return;
}

int
net_ori_label (char *label, struct OL_SET *pOSet)
{
  unsigned int i;
  int status;
  double **g = ol_g_alloc ();

  status = ol_label_g (label, g);

  if (!status)
  {
    ol_g_q (g, (*pOSet).q[0]);
    for (i = 0; i < (*pOSet).size; i++)
      ut_array_1d_memcpy ((*pOSet).q[i], 4, (*pOSet).q[0]);
  }

  ol_g_free (g);

  return status;
}

void
net_ori_file (char *filename, struct OL_SET *pOSet)
{
  unsigned int i;
  int partqty;
  char **parts = NULL;
  char *des = NULL;
  double *vect = ut_alloc_1d (4);
  double **g = ol_g_alloc ();
  FILE *fp = NULL;

  ut_string_separate (filename, NEUT_SEP_DEP, &parts, &partqty);
  if (partqty == 1)
    ut_string_string ("e", &des);
  else
    ut_string_string (parts[1], &des);

  fp = ut_file_open (parts[0], "r");

  for (i = 0; i < (*pOSet).size; i++)
  {
    if (!strcmp (des, "e"))
    {
      ol_e_fscanf (fp, vect);
      ol_e_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "ek"))
    {
      ol_e_fscanf (fp, vect);
      ol_ek_e (vect, vect);
      ol_e_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "er"))
    {
      ol_e_fscanf (fp, vect);
      ol_er_e (vect, vect);
      ol_e_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "rtheta"))
    {
      ol_rtheta_fscanf (fp, vect, vect + 3);
      ol_rtheta_q (vect, vect[3], (*pOSet).q[i]);
    }
    else if (!strcmp (des, "q"))
    {
      ol_q_fscanf (fp, vect);
      ol_q_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "R"))
    {
      ol_R_fscanf (fp, vect);
      ol_R_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "g"))
    {
      ol_g_fscanf (fp, g);
      ol_g_q (g, (*pOSet).q[i]);
    }
    else
      abort ();
  }

  ut_file_close (fp, filename, "r");

  ut_free_2d_char (parts, partqty);
  ut_free_1d (vect);
  ol_g_free (g);

  return;
}

void
net_ori_crysym (struct OL_SET *pOSet)
{
  unsigned int i;

  for (i = 0; i < (*pOSet).size; i++)
    ol_q_qcrysym ((*pOSet).q[i], (*pOSet).crysym, (*pOSet).q[i]);

  return;
}

int
net_ori_mtess_randseed_rand (int *N, int *id, int *poly, int levelqty)
{
  int i;
  time_t t;
  double tmp;
  int Rand;

  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, 1);

  /* Rand: long integer number calculated from (N,id,level,poly):
   * Rand is 8 numbers long ; N is put at the left side, id at the
   * right side. If N and id numbers cross, they are added.
   * N=123  and id=12     --> Rand=12300012
   * N=3000 and id=65433  --> Rand=30065433
   * N=3333 and id=88888  --> Rand=33418888
   * To this value is added the product level * poly.  It is put at the
   * left hand side, but with a length of 7.
   */

  // levelqty = 1 and = 2 are kept for backward compatibility
  if (levelqty == 1)
  {
    if (id[1] >= 0)
      Rand = N[1] * pow (10, 7 - ut_num_tenlen (N[1])) + id[1];
    else
    {
      time (&t);
      Rand = t;
    }
  }
  /* this version would provide backward compatibility with v2.0, but
   * different inputs can provide same outputs:
   N = 10 poly =  1 level = 1 id = 1 Rand = 11000001
   N = 10 poly = 10 level = 1 id = 1 Rand = 11000001
   Better to avoid it.

   else if (levelqty == 2)
   {
   if (id[2] >= 0)
   {
   Rand = N[1] * pow (10, 7 - ut_num_tenlen (N[1]))
   + poly[2] * 1 * pow (10, 6 - ut_num_tenlen (poly[2] * 1)) +
   id[2];
   printf ("printf N = %d poly = %d level = %d id = %d Rand = %d\n",
   N[1], poly[2], 1, id[2], Rand);
   }
   else
   {
   time (&t);
   Rand = t;
   }
   }
   */
  else
  {
    tmp = 1;
    for (i = 1; i <= levelqty; i++)
    {
      tmp += 100 * gsl_rng_uniform (r) * N[i]
	+ 100 * gsl_rng_uniform (r) * id[i]
	+ 100 * gsl_rng_uniform (r) * poly[i];
    }
    while (tmp > INT_MAX)
      tmp *= 0.5;

    Rand = (int) tmp;
  }

  if (Rand < 0)
    ut_error_reportbug ();

  gsl_rng_free (r);

  return Rand;
}

void
net_ori_mtess_id (struct IN_T In, struct MTESS MTess, struct TESS *Tess,
                  int dtess, int dcell, struct SEEDSET *pSSet)
{
  char *tmp = ut_alloc_1d_char (100);

  neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &tmp);
  net_multiscale_arg_0d_int_fscanf (In.id[Tess[dtess].Level + 1], tmp,
				    0, NULL, NULL, &(*pSSet).Id);

  ut_free_1d_char (tmp);

  return;
}

void
net_ori_mtess_params (struct IN_T In, int level, struct MTESS MTess,
                      struct TESS *Tess, int dtess, int dcell,
                      char **pori, char **pcrysym)
{
  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.ori[level], pori);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.oricrysym[level],
					   pcrysym);

  return;
}

void
net_ori_mtess_randseed (struct MTESS MTess, struct TESS *Tess, int domtess,
                        int dompoly, struct SEEDSET *SSet, int CellQty,
                        struct SEEDSET *pSSet)
{
  int i, tess;
  int levelqty = Tess[domtess].Level + 1;
  int *ns = ut_alloc_1d_int (levelqty + 1);
  int *ids = ut_alloc_1d_int (levelqty + 1);
  int *polys = ut_alloc_1d_int (levelqty + 1);
  int **doms = ut_alloc_2d_int (levelqty + 1, 2);

  neut_mtess_tess_doms (MTess, Tess[domtess], doms);
  ut_array_1d_int_set_2 (doms[levelqty], domtess, dompoly);

  for (i = 1; i < levelqty; i++)
  {
    polys[i] = doms[i][1];
    tess = MTess.DomTess[doms[i - 1][0]][doms[i - 1][1]];
    ids[i] = SSet[tess].Id;
    ns[i] = SSet[tess].N;
  }
  ids[levelqty] = (*pSSet).Id;
  ns[levelqty] = CellQty;
  polys[levelqty] = dompoly;

  (*pSSet).Random = net_ori_mtess_randseed_rand (ns, ids, polys, Tess[domtess].Level + 1);

  ut_free_1d_int (ns);
  ut_free_1d_int (ids);
  ut_free_1d_int (polys);
  ut_free_2d_int (doms, levelqty + 1);

  return;
}

void
net_ori_memcpy (struct SEEDSET *pSSet, struct OL_SET OSet)
{
  (*pSSet).q = ut_alloc_2d ((*pSSet).N + 1, 4);
  ut_array_2d_memcpy ((*pSSet).q + 1, OSet.size, 4, OSet.q);

  return;
}
