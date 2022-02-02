/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_log_.h"

void
net_tess_opt_comp_objective_log (struct TOPT TOpt)
{
  int i;

  if (strcmp (TOpt.TDyn.logtime, "none"))
    net_tess_opt_comp_objective_log_time (TOpt);

  if (strcmp (TOpt.TDyn.logvar, "none"))
    net_tess_opt_comp_objective_log_var (TOpt);

  if (strcmp (TOpt.TDyn.logdis, "none"))
    for (i = 0; i < TOpt.tarqty; i++)
      net_tess_opt_comp_objective_log_dis (TOpt, i);

  if (strcmp (TOpt.TDyn.logtesr, "none"))
    net_tess_opt_comp_objective_log_tesr (TOpt);

  if (strcmp (TOpt.TDyn.logval, "none"))
    net_tess_opt_comp_objective_log_val (TOpt);

  return;
}
