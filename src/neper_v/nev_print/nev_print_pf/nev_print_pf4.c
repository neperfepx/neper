/* This id is part of the 'hermes' program. */
/* Copyright (C) 2007-2010, Romain Quey. */
/* See the COPYINPFG id in the top-level directory. */

#include"nev_print_pf_.h"

void
nev_print_pf_file_data_ori_preipf (int input,
                                   struct OL_SET Set, double *uvect,
                                   struct PF *pPf)
{
  int i, j;
  double *qs = ol_q_alloc ();
  double* p = ol_r_alloc ();

  (*pPf).inputcellqty[input] = Set.size;
  (*pPf).inputcellori[input] = ut_alloc_2d ((*pPf).inputcellqty[input] + 1, 4);
  (*pPf).inputcellpole[input] = ut_alloc_1d_int ((*pPf).inputcellqty[input] + 1);

  for (i = 0; i < (int) Set.size; i++)
  {
    ol_q_memcpy (Set.q[i], (*pPf).inputcellori[input][i + 1]);

    for (j = 1; j <= ol_crysym_qty (Set.crysym); j++)
    {
      ol_q_crysym (Set.q[i], Set.crysym, j, qs);

      if (!strcmp ((*pPf).projection, "stereographic"))
        ol_q_ipf_stprojxy (qs, uvect, p);
      else if (!strcmp ((*pPf).projection, "equal-area"))
        ol_q_ipf_eaprojxy (qs, uvect, p);

      if ((*pPf).ipfptqty == 0)
        ut_print_neperbug ();

      if (ut_space_point_indomain_2d (p, (*pPf).domainpts, (*pPf).domainptqty))
      {
        (*pPf).inputcellpole[input][i + 1] = j;
        break;
      }
    }

    if (!(*pPf).inputcellpole[input][i + 1])
      abort ();
  }

  ol_q_free (qs);
  ol_r_free (p);

  return;
}

void
nev_print_pf_file_data_vect_preipf (int input,
                                    double **data, int qty,
                                    struct PF *pPf)
{
  int i, j;
  double **gs = ol_g_alloc ();
  double* v = ol_r_alloc ();
  double* p = ol_r_alloc ();
  double **I = ol_g_alloc ();

  ol_g_set_id (I);

  (*pPf).inputcellqty[input] = qty;
  (*pPf).inputcellori[input] = ut_alloc_2d ((*pPf).inputcellqty[input] + 1, 4);
  (*pPf).inputcellpole[input] = ut_alloc_1d_int ((*pPf).inputcellqty[input] + 1);

  for (i = 0; i < qty; i++)
  {
    for (j = 1; j <= 24; j++)
    {
      ol_g_crysym (I, (*pPf).crysym, j, gs);
      ol_g_vect_vect (gs, data[i], v);
      ut_array_1d_normalize (v, 3);

      if (!strcmp ((*pPf).projection, "stereographic"))
        ol_vect_stprojxy (v, p);
      else if (!strcmp ((*pPf).projection, "equal-area"))
        ol_vect_eaprojxy (v, p);
      else
        abort ();

      if (ut_space_point_indomain_2d (p, (*pPf).domainpts, (*pPf).domainptqty))
      {
        (*pPf).inputcellpole[input][i + 1] = j;
        break;
      }
    }

    if (!(*pPf).inputcellpole[input][i + 1])
      abort ();
  }

  ol_g_free (gs);
  ol_g_free (I);
  ol_r_free (p);

  return;
}
