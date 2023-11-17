/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_.h"
#ifdef HAVE_OPENMP
#include<omp.h>
#endif /* HAVE_OPENMP */

void
net_tess_opt_comp_objective_fval_gen (struct TOPT *pTOpt, int var)
{
  int i, cell;
  struct timeval t1, t2, t3;

  gettimeofday (&t1, NULL);

#pragma omp parallel for private(cell) schedule(dynamic)
  for (i = 0; i < (*pTOpt).cellchangedqty; i++)
  {
    cell = (*pTOpt).cellchanged[i];

    if ((*pTOpt).curcellpenalty[cell] == 0)
    {
      if (!strcmp ((*pTOpt).tarvar[var], "size"))
        net_tess_opt_comp_objective_fval_gen_size (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "diameq"))
        net_tess_opt_comp_objective_fval_gen_diameq (pTOpt, var, cell);

      else if (strstr ((*pTOpt).tarvar[var], "sphericity")
               || strstr ((*pTOpt).tarvar[var], "1-sphericity"))
        net_tess_opt_comp_objective_fval_gen_sphericity (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "convexity"))
        net_tess_opt_comp_objective_fval_gen_convexity (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "centroid")
            || !strcmp ((*pTOpt).tarvar[var], "centroidtol"))
        net_tess_opt_comp_objective_fval_gen_centroid (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "centroidsize")
               || !strcmp ((*pTOpt).tarvar[var], "centroiddiameq"))
        net_tess_opt_comp_objective_fval_gen_centroidsize (pTOpt, var, cell);

      else if (!strcmp ((*pTOpt).tarvar[var], "ori"))
        net_tess_opt_comp_objective_fval_gen_ori (pTOpt, var, cell);

      else
      {
#ifdef HAVE_OPENMP
        if (omp_get_thread_num() == 0)
          ut_print_message (2, 3, "Failed to process `%s'...\n", (*pTOpt).tarvar[var]);
#else
        ut_print_message (2, 3, "Failed to process `%s'...\n", (*pTOpt).tarvar[var]);
#endif /* HAVE_OPENMP */
      }
    }
  }

  gettimeofday (&t2, NULL);

  if (!strcmp ((*pTOpt).tartype[var], "stat"))
    net_tess_opt_comp_objective_fval_gen_stat (pTOpt, var);
  else if (!strcmp ((*pTOpt).tartype[var], "odf"))
    net_tess_opt_comp_objective_fval_gen_odf (pTOpt, var);
  else
    net_tess_opt_comp_objective_fval_gen_celldata (pTOpt, var);

  if (isnan ((*pTOpt).curval[var]))
  {
    printf ("(*pTOpt).tartype[%d] = %s\n", var, (*pTOpt).tartype[var]);
    printf ("(*pTOpt).curval[%d] is not-a-number.\n", var);
    ut_print_neperbug ();
  }

  gettimeofday (&t3, NULL);

  (*pTOpt).TDyn.val_val_cellval_dur += ut_time_subtract (&t1, &t2);
  (*pTOpt).TDyn.val_val_comp_dur += ut_time_subtract (&t2, &t3);

  return;
}
