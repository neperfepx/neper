/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_.h"

void
net_ori_qty (struct SEEDSET *pSSet, char **parts, int partqty, int **pqty)
{
  int i;
  double *fact = NULL;
  fact = ut_alloc_1d (partqty);
  *pqty = ut_alloc_1d_int (partqty);

  for (i = 0; i < partqty; i++)
  {
    int qty2;
    char **parts2 = NULL;

    ut_list_break (parts[i], "*", &parts2, &qty2);

    if (qty2 == 1)
      fact[i] = 1;
    else if (qty2 == 2)
      sscanf (parts2[0], "%lf", fact + i);
    else
      abort ();
    ut_string_string (parts2[qty2 - 1], parts + i);

    ut_free_2d_char (&parts2, qty2);
  }

  ut_array_1d_scale (fact, partqty, (*pSSet).N / ut_array_1d_sum (fact, partqty));
  ut_array_1d_round_keepsum (fact, partqty, fact);
  ut_array_1d_memcpy_toint (fact, partqty, *pqty);
  ut_free_1d (&fact);

  return;
}

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
net_ori_fiber (struct SEEDSET *SSet, int dtess, int dcell, long random,
               char *distrib, struct OL_SET *pOSet)
{
  double theta;
  double *dirc = ut_alloc_1d (3);
  double *dirs = ut_alloc_1d (3);
  char *spread = NULL;

  neut_ori_fiber_sscanf (distrib, dirc, dirs, &theta, &spread);

  neut_ori_fiber (dirc, dirs, random, (*pOSet).size, (*pOSet).q);

  neut_ori_fiber_spread (spread, dirs, SSet[dtess].Random + dcell,
                         (*pOSet).size, (*pOSet).crysym, (*pOSet).q);

  ut_free_1d_char (&spread);
  ut_free_1d (&dirc);
  ut_free_1d (&dirs);

  return;
}

void
net_ori_label (char *label, struct SEEDSET *SSet, int dtess, int dcell, struct OL_SET *pOSet)
{
  unsigned int i;
  int status, partqty;
  double *q = ol_q_alloc ();
  char **parts = NULL;
  char *spread = NULL;

  ut_list_break (label, NEUT_SEP_DEP, &parts, &partqty);

  if (partqty == 2)
    ut_string_string (parts[1], &spread);

  status = 0;
  if (!strcmp (parts[0], "parent"))
    ol_q_memcpy (SSet[dtess].SeedOri[dcell], q);
  else
    status = ol_label_q (parts[0], q);

  if (!status)
  {
    ol_set_misorispread (spread, 3, SSet[dtess].Random + dcell, pOSet);

    for (i = 0; i < (*pOSet).size; i++)
      ol_q_q_q_ref (q, (*pOSet).q[i], (*pOSet).q[i]);
  }

  if (status)
    abort ();

  ol_q_free (q);
  ut_free_2d_char (&parts, partqty);
  ut_free_1d_char (&spread);

  return;
}

void
net_ori_file (char *filename_in, struct OL_SET *pOSet)
{
  unsigned int i;
  char *des = NULL, *conv = NULL, *tmp = NULL;
  double *vect = ut_alloc_1d (4);
  double **g = ol_g_alloc ();
  FILE *fp = NULL;
  char *fct = NULL;
  int varqty = 0;
  char **vars = NULL;
  char **vals = NULL;
  char *filename = NULL;

  if (!strncmp (filename_in, "file(", 5))
  {
    ut_string_function (filename_in, &fct, &vars, &vals, &varqty);
    ut_string_string (vals[0], &filename);
  }
  else
    ut_string_string (filename_in, &filename);

  ut_string_string ("rodrigues", &des);
  ut_string_string ("active", &conv);

  if (varqty == 1)
    ut_string_string (NEUT_DEFAULT_ORIDES, &des);
  else if (varqty == 2)
  {
    if (!ut_string_strcmp (vars[1], "des"))
    {
      ut_list_break_2 (vals[1], NEUT_SEP_DEP, &des, &tmp);
      if (tmp)
        ut_string_string (tmp, &conv);
    }
    else
      abort ();
  }

  if ((*pOSet).size > 0)
    ol_set_free (pOSet);

  (*pOSet) = ol_set_alloc (ut_file_nbwords (filename) / ol_des_size (des), NULL);

  fp = ut_file_open (filename, "r");

  for (i = 0; i < (*pOSet).size; i++)
  {
    if (!strcmp (des, "euler-bunge"))
    {
      ol_e_fscanf (fp, vect);
      ol_e_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "euler-kocks"))
    {
      ol_e_fscanf (fp, vect);
      ol_ek_e (vect, vect);
      ol_e_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "euler-roe"))
    {
      ol_e_fscanf (fp, vect);
      ol_er_e (vect, vect);
      ol_e_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "axis-angle"))
    {
      ol_rtheta_fscanf (fp, vect, vect + 3);
      ol_rtheta_q (vect, vect[3], (*pOSet).q[i]);
    }
    else if (!strcmp (des, "quaternion"))
    {
      ol_q_fscanf (fp, vect);
      ol_q_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "rodrigues"))
    {
      ol_R_fscanf (fp, vect);
      ol_R_q (vect, (*pOSet).q[i]);
    }
    else if (!strcmp (des, "rotmat"))
    {
      ol_g_fscanf (fp, g);
      ol_g_q (g, (*pOSet).q[i]);
    }
    else
      abort ();
  }

  if (!strcmp (conv, "passive"))
    for (i = 0; i < (*pOSet).size; i++)
        ol_q_inverse ((*pOSet).q[i], (*pOSet).q[i]);

  ut_file_close (fp, filename, "r");

  ut_free_1d_char (&des);
  ut_free_1d_char (&conv);
  ut_free_1d_char (&tmp);
  ut_free_1d (&vect);
  ol_g_free (g);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&filename);

  return;
}

void
net_ori_odf (long random, char *odf, struct OL_SET *pOSet)
{
  int i, elt;
  gsl_rng *r = NULL, *r2 = NULL;
  struct ODF Odf;
  char *method = NULL;

  ut_string_string ("inverse_uniform_sampling", &method);

  neut_odf_set_zero (&Odf);

  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, random - 1);
  r2 = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r2, random - 1);

  net_ori_odf_pre (odf, pOSet, &Odf);

  ut_print_message (0, 2, "Sampling odf...\n");

  if (!strcmp (method, "inverse_uniform_sampling"))
  {
    double *x = ut_alloc_1d (Odf.odfqty + 1);

    neut_odf_init_eltweight (&Odf);

    if (!strstr (Odf.gridtype, (*pOSet).crysym))
      ut_print_message (2, 0, "Crystal symmetry (%s) and orientation space (%s) conflict.\n",
                        (*pOSet).crysym, Odf.gridtype);

    // building the probability array
    x = ut_alloc_1d (Odf.odfqty + 1);
    for (i = 1; i <= Odf.odfqty; i++)
      x[i] = Odf.EltWeight[i - 1] * Odf.odf[i - 1] * ol_crysym_qty ((*pOSet).crysym) / pow (M_PI, 2);

    // slight correction, if necessary, to make sure we sum exactly to 1
    ut_array_1d_scale (x, Odf.odfqty + 1, 1. / ut_array_1d_sum (x, Odf.odfqty + 1));

    for (i = 1; i <= Odf.odfqty; i++)
      x[i] += x[i - 1];

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      elt = ut_array_1d_eltpos_interpolate (x, Odf.odfqty + 1, gsl_rng_uniform (r));
      neut_odf_elt_ori (Odf, elt, r2, (*pOSet).q[i]);
    }

    gsl_rng_free (r);
    ut_free_1d (&x);
  }

  else if (!strcmp (method, "rejection")) // for the record, but inefficient
  {
    int status;
    double n1, n2, n3;
    double fmax, t;
    double *e = ol_e_alloc ();
    double *R = ol_R_alloc ();

    fmax = ut_array_1d_max (Odf.odf, Odf.odfqty);

    for (i = 0; i < (int) (*pOSet).size; i++)
    {
      // keep the n1 n2 n3 stuff for backward compatibility.
      // Calling the gsl as an argument switches n1 and n3.
      n1 = gsl_rng_uniform (r);
      n2 = gsl_rng_uniform (r);
      n3 = gsl_rng_uniform (r);
      ol_nb_e (n1, n2, n3, e);
      ol_e_R (e, R);
      ol_R_Rcrysym (R, (*pOSet).crysym, R);

      status = neut_mesh_point_elt (Odf.Nodes, Odf.Mesh[3], R, &elt);
      if (status)
        ut_print_message (2, 0, "Failed to find element for orientation = (%f,%f,%f)\n", R[0], R[1], R[2]);

      t = gsl_rng_uniform (r);

      if (t < Odf.odf[elt - 1] / fmax)
        ol_e_q (e, (*pOSet).q[i]);
      else
        i--;
    }

    gsl_rng_free (r);
    ol_e_free (e);
    ol_R_free (R);
  }

  else
    ut_print_neperbug ();

  return;
}

void
net_ori_oricrysym (struct OL_SET *pOSet)
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
      tmp +=
        100 * gsl_rng_uniform (r) * N[i] + 100 * gsl_rng_uniform (r) * id[i] +
        100 * gsl_rng_uniform (r) * poly[i];
    }
    while (tmp > INT_MAX)
      tmp *= 0.5;

    Rand = (int) tmp;
  }

  if (Rand < 0)
    ut_print_neperbug ();

  gsl_rng_free (r);

  return Rand;
}

void
net_ori_mtess_id (struct IN_T In, struct MTESS MTess, struct TESS *Tess,
                  int dtess, int dcell, struct SEEDSET *pSSet)
{
  char *tmp = ut_alloc_1d_char (100);

  neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &tmp);
  net_multiscale_arg_0d_int_fscanf (In.id[Tess[dtess].Level + 1], tmp, 0,
                                    NULL, NULL, &(*pSSet).Id);

  ut_free_1d_char (&tmp);

  return;
}

void
net_ori_mtess_params (struct IN_T In, int level, struct MTESS MTess,
                      struct TESS *Tess, int dtess, int dcell, char **pori,
                      char **porisampling, char **porispread, char **pcrysym)
{
  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.ori[level], pori);
  ut_string_fnrs (*pori, "fibre", "fiber", 1);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.orisampling[level], porisampling);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.orispread[level], porispread);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.crysym[level], pcrysym);

  if (level == 1 && !strncmp (*pori, "parent", 6))
    ut_print_message (2, 3, "`parent' not available at scale 1.\n");

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

  (*pSSet).Random =
    net_ori_mtess_randseed_rand (ns, ids, polys, Tess[domtess].Level + 1);

  ut_free_1d_int (&ns);
  ut_free_1d_int (&ids);
  ut_free_1d_int (&polys);
  ut_free_2d_int (&doms, levelqty + 1);

  return;
}

void
net_ori_memcpy (struct OL_SET OSet, struct SEEDSET *pSSet)
{
  (*pSSet).SeedOri = ut_alloc_2d ((*pSSet).N + 1, 4);
  ut_array_2d_memcpy (OSet.q, OSet.size, 4, (*pSSet).SeedOri + 1);

  if (OSet.weight)
  {
    (*pSSet).SeedOriWeight = ut_alloc_1d ((*pSSet).N + 1);
    ut_array_1d_memcpy (OSet.weight, OSet.size, (*pSSet).SeedOriWeight + 1);
  }

  if (OSet.theta)
  {
    (*pSSet).SeedOriTheta = ut_alloc_1d ((*pSSet).N + 1);
    ut_array_1d_memcpy (OSet.theta, OSet.size, (*pSSet).SeedOriTheta + 1);
  }

  ut_string_string (OSet.crysym, &(*pSSet).crysym);

  return;
}

void
net_ori_orispread (char *orispread, struct SEEDSET *pSSet)
{
  int i;

  (*pSSet).SeedOriDistrib = ut_alloc_2d_char ((*pSSet).N +  1, 1);

  if (!strncmp (orispread, "file(", 5))
    net_orispread_file (orispread, pSSet);

  else
    for (i = 1; i <= (*pSSet).N; i++)
      ut_string_string (orispread, (*pSSet).SeedOriDistrib + i);

  return;
}
