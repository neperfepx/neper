/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_ori_uniform_.h"

void
net_ori_uniform_init (struct IN_T In, int level, struct MTESS MTess,
                      struct TESS *Tess, int dtess, int dcell, long random,
                      struct OL_SET *pOSet, struct OOPT *pOOpt,
                      int verbositylevel)
{
  int varqty = 3;
  double *vals = ut_alloc_1d (varqty);
  char **vars = ut_alloc_1d_pchar (varqty);
  ut_string_string ("dr", vars);
  ut_string_string ("Nstar", vars + 1);
  ut_string_string ("pi", vars + 2);

  (*pOOpt).n = (*pOSet).size;
  (*pOOpt).id = 0;
  ut_string_string ((*pOSet).crysym, &(*pOOpt).crysym);
  ut_string_string (In.optineigh[1][level], &(*pOOpt).orioptineigh);

  net_orioptistop (In, level, MTess, Tess, dtess, dcell, (char *) "thomson", &(*pOOpt).orioptistop);

  neut_ori_n_avradeq (NULL, (*pOSet).size, (*pOSet).crysym, vals);
  vals[1] = (*pOSet).size * (*pOSet).nc;
  vals[2] = M_PI;

  ut_math_eval (In.optineigh[1][level], varqty, vars, vals,
                &((*pOOpt).neighd));

  (*pOOpt).neighd = ut_num_min ((*pOOpt).neighd, M_PI - OL_EPS_RAD);

  (*pOOpt).neighdE = 2 * sin (0.5 * (*pOOpt).neighd);
  (*pOOpt).neighdE = pow ((*pOOpt).neighdE, 2);
  if ((*pOOpt).neighdE > 4 - OL_EPS)
    (*pOOpt).neighdE = 4 + OL_EPS;

  // optiini

  if (!strcmp (In.optiini[1][level], "random"))
    net_ori_random (random, pOSet);
  else if (ut_string_isfilename (In.optiini[1][level]))
  {
    ut_print_message (0, verbositylevel,
                      "Loading orientations from file...\n");
    net_ori_file (In.optiini[1][level], pOSet);
  }
  else
    abort ();

  // optioptifix

  if (strcmp (In.optifix[1][level], "none"))
  {
    (*pOOpt).fixori = ut_alloc_1d_int ((*pOOpt).n);

    char *mid = NULL;
    neut_mtess_tess_poly_mid (MTess, Tess[dtess], dcell, &mid);
    net_multiscale_arg_1d_int_fscanf (In.optifix[1][level], mid,
                                      (*pOOpt).fixori, (*pOOpt).n);
    ut_free_1d_char (&mid);

    FILE *fp = ut_file_open (In.optifix[1][level], "r");
    ut_array_1d_int_fscanf (fp, (*pOOpt).fixori, (*pOOpt).n);
    ut_file_close (fp, In.optifix[1][level], "r");

    (*pOOpt).fixoriqty = ut_array_1d_int_sum ((*pOOpt).fixori, (*pOOpt).n);
    ut_print_message (0, verbositylevel, "%d fixed orientation%s.\n",
                      (*pOOpt).fixoriqty, (*pOOpt).fixoriqty > 1 ? "s" : "");
  }

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.optilogvar[1][level],
                                           &(*pOOpt).logvar);

  net_multiscale_mtess_arg_0d_char_fscanf (level, MTess, Tess, dtess, dcell,
                                           In.optilogval[1][level],
                                           &(*pOOpt).logval);

  // terminal output
  ut_print_message (0, verbositylevel, "Crystal symmetry: %s",
                    (*pOSet).crysym);
  if ((*pOOpt).neighd < M_PI - OL_EPS_RAD)
    printf (", neighbor cut-off: %.6f", (*pOOpt).neighd);
  else
    printf (", no cut-off");
  printf ("\n");

  ut_free_1d (&vals);
  ut_free_2d_char (&vars, varqty);

  return;
}

int
net_ori_uniform_opt (struct IN_T In, struct OOPT *pOOpt, struct OL_SET *pOSet,
                     int verbositylevel)
{
  int stop_iter = 0;
  double alpha = DBL_MAX, **fc = NULL, **qc = NULL;
  char *message = ut_alloc_1d_char (1000);
  char *prevmessage = ut_alloc_1d_char (1000);
  double *Etot = NULL;
  double **f = ut_alloc_2d ((*pOSet).size, 3);
  double *E = ut_alloc_1d ((*pOSet).size);
  my_kd_tree_t *qtree = nullptr;
  struct QCLOUD qcloud;
  double **dq = NULL;

  fc = ut_alloc_2d ((*pOSet).size, 3);
  qc = ut_alloc_2d ((*pOSet).size, 4);
  dq = ut_alloc_2d ((*pOSet).size, 3);

  (*pOOpt).nc = ol_crysym_qty ((*pOSet).crysym);

  (*pOOpt).iter = 0;

  if (!strcmp ((*pOOpt).orioptistop, "iter==-1"))
    return 0;

  if ((*pOOpt).neighd < M_PI)
    neut_oset_kdtree (pOSet, &qcloud, &qtree);

  // computing forces
  net_ori_uniform_opt_forces (pOSet, f, E, pOOpt, &qcloud, qtree);

  // computing energy
  net_ori_uniform_opt_energy (pOSet, E, &Etot, pOOpt);

  neut_oopt_finalize (pOOpt);

  // printing results
  // net_ori_uniform_opt_verbosity (*pOOpt, iter, alpha, prevmessage, message);

  net_ori_uniform_log (In, *pOSet, *pOOpt);

  neut_oopt_time_set_zero (pOOpt);

  stop_iter = net_ori_uniform_opt_stop (pOOpt);
  while (!stop_iter)
  {
    (*pOOpt).iter++;

    // determining alpha
    net_ori_uniform_opt_alpha (pOOpt, *pOSet, dq, f, fc, &alpha);

    ut_array_2d_memcpy ((*pOSet).q, (*pOSet).size, 4, qc);
    ut_array_2d_memcpy (f, (*pOSet).size, 3, fc);

    // evolving orientations
    net_ori_uniform_opt_rot (pOOpt, f, alpha, pOSet, dq);

    if (strcmp ((*pOOpt).orioptineigh, "all"))
      neut_oset_kdtree (pOSet, &qcloud, &qtree);

    // computing forces
    net_ori_uniform_opt_forces (pOSet, f, E, pOOpt, &qcloud, qtree);

    net_ori_uniform_opt_energy (pOSet, E, &Etot, pOOpt);

    net_ori_uniform_opt_verbosity (*pOOpt, prevmessage, message,
                                   verbositylevel);

    net_ori_uniform_log (In, *pOSet, *pOOpt);

    neut_oopt_finalize (pOOpt);

    neut_oopt_time_set_zero (pOOpt);

    stop_iter = net_ori_uniform_opt_stop (pOOpt);
  }

  printf ("\n");
  ut_print_message (0, verbositylevel, "Final solution  : f   =%.9f (%d iterations)\n",
                    (*pOOpt).fmin, (*pOOpt).iter);

  ut_free_1d_char (&message);
  ut_free_1d_char (&prevmessage);
  neut_oopt_free (pOOpt);
  ut_free_2d (&f, (*pOSet).size);
  ut_free_1d (&E);
  ut_free_2d (&fc, (*pOSet).size);
  ut_free_2d (&qc, (*pOSet).size);

  delete qtree;

  return EXIT_SUCCESS;
}
