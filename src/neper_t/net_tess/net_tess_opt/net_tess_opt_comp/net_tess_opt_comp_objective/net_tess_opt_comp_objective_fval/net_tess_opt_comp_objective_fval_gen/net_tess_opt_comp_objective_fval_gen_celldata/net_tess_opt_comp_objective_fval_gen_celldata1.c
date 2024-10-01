/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_celldata_.h"

void
net_tess_opt_comp_objective_fval_gen_celldata (struct TOPT *pTOpt, int id)
{
  if (!strncmp ((*pTOpt).tarvar[id], "size", 4)
      || !strcmp ((*pTOpt).tarvar[id], "diameq")
      || !strcmp ((*pTOpt).tarvar[id], "sphericity"))
    net_tess_opt_comp_objective_fval_gen_celldata_scalar (pTOpt, id);

  else if (!strcmp ((*pTOpt).tarvar[id], "centroid"))
    net_tess_opt_comp_objective_fval_gen_celldata_centroid (pTOpt, id);

  else if (!strcmp ((*pTOpt).tarvar[id], "centroidtol"))
    net_tess_opt_comp_objective_fval_gen_celldata_centroidtol (pTOpt, id);

  else if (!strcmp ((*pTOpt).tarvar[id], "centroidsize")
           || !strcmp ((*pTOpt).tarvar[id], "centroiddiameq"))
    net_tess_opt_comp_objective_fval_gen_celldata_centroiddiameq (pTOpt, id);

  else
    abort ();

  return;
}
