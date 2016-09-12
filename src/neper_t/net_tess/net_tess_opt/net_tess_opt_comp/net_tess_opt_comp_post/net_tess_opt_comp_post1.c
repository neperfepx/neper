/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_post_.h"

#ifdef HAVE_NLOPT

void
net_tess_opt_comp_post (struct TOPT TOpt, int res)
{
  if (TOpt.tarqty == 0)
    return;

  printf ("\n");

  if (res >= 0)
  {
    if (res == NLOPT_STOPVAL_REACHED)
      ut_print_message (0, 3, "Reached `val' criterion.");
    else if (res == NLOPT_FTOL_REACHED)
      ut_print_message (0, 3, "Reached `(r)eps' criterion.");
    else if (res == NLOPT_XTOL_REACHED)
      ut_print_message (0, 3, "Reached `x(r)eps' criterion.");
    else if (res == NLOPT_MAXEVAL_REACHED)
      ut_print_message (0, 3, "Reached `itermax' criterion.");
    else if (res == NLOPT_MAXTIME_REACHED)
      ut_print_message (0, 3, "Reached `time' criterion.");
    else
      ut_print_message (0, 3, "Reached unknown criterion.");

  }
  else
  {
    if (res == NLOPT_FAILURE)
      ut_print_message (1, 3, "Failed optimization.");
    else if (res == NLOPT_ROUNDOFF_LIMITED)
      ut_print_message (1, 3, "Failed due to roundoff error.");
    else if (res == NLOPT_FORCED_STOP)
    {
      if (nlopt_get_force_stop (TOpt.opt) == -5)
	ut_print_message (1, 3, "Stopping on Ctrl+C invocation.");
      else if (nlopt_get_force_stop (TOpt.opt) == -6)
	ut_print_message (0, 3, "Reached `dvalditer' criterion.");
      else
	ut_print_message (1, 3, "Stopping on unknown forced stop (%d).",
			  nlopt_get_force_stop (TOpt.opt));
    }
    else
      ut_print_message (1, 3, "Failed due to unknown error.");
  }

  return;
}

#endif
