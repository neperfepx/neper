/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_odf_.h"

void
net_tess_opt_comp_objective_fval_gen_odf (struct TOPT *pTOpt, int var)
{
  int i;
  struct OL_SET OSet;
  struct ODF Odf;

  neut_odf_set_zero (&Odf);

  OSet = ol_set_alloc ((*pTOpt).SSet.N, (*pTOpt).SSet.crysym);
  OSet.weight = ut_alloc_1d (OSet.size);
  ut_array_1d_memcpy ((*pTOpt).SSet.SeedOriWeight + 1, OSet.size, OSet.weight);
  if ((*pTOpt).SSet.SeedOriTheta)
  {
    OSet.theta = ut_alloc_1d (OSet.size);
    ut_array_1d_memcpy ((*pTOpt).SSet.SeedOriTheta + 1, OSet.size, OSet.theta);
  }

  ut_string_string ((*pTOpt).Odf.gridtype, &Odf.gridtype);
  ut_string_string ((*pTOpt).Odf.gridunit, &Odf.gridunit);

  neut_nodes_memcpy ((*pTOpt).Odf.Nodes, &Odf.Nodes);
  for (i = 0; i <= 3; i++)
    neut_mesh_memcpy ((*pTOpt).Odf.Mesh[i], Odf.Mesh + i);

  Odf.odfqty = (*pTOpt).Odf.Mesh[3].EltQty;
  Odf.odf = ut_alloc_1d (Odf.odfqty);

  ut_array_2d_memcpy ((*pTOpt).SSet.SeedOri + 1, (*pTOpt).SSet.N, 4, OSet.q);

  // if ((*pTOpt).Odf.sigma != 0)
    Odf.sigma = (*pTOpt).Odf.sigma;
  /*
  else
    neut_odf_setsigma (&Odf, "avthetaeq", (*pTOpt).SSet.N, (*pTOpt).SSet.crysym);
  */

  neut_odf_comp ("m", (OSet.size >= 10000) ? "3" : "all", &OSet, &Odf, 0);

  /*
  if ((*pTOpt).cvlsig[var] > 0)
  {
    if ((*pTOpt).iter == (*pTOpt).loop_start[(*pTOpt).loop - 1]
        || (*pTOpt).iter % 1000 == 1)
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_objective_fval_gen_odf_smoothed (pTOpt, var);
      else
        ut_print_neperbug ();
    }

    else
    {
      if (!strcmp ((*pTOpt).cvlmethod, "numerical"))
        net_tess_opt_comp_objective_fval_gen_odf_smoothed_update (pTOpt, var);
      else
        ut_print_neperbug ();
    }
  }
  */

  net_tess_opt_comp_objective_fval_gen_odf_evaluate (pTOpt, Odf, var);

  ol_set_free (&OSet);

  // neut_odf_free (&Odf);

  return;
}
