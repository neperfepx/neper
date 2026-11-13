/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_post_.h"

// printing the result of the optimization
void
net_tess_opt_comp_post (struct TOPT TOpt)
{
  const char *res = TOpt.result ? TOpt.result : "";

  if (TOpt.tarqty == 0)
    return;

  printf ("\n");

  if (!strcmp (res, "stopval"))
    ut_print_message (0, 4, "Reached `val' criterion.");
  else if (!strcmp (res, "ftol"))
    ut_print_message (0, 4, "Reached `nlopt_(r)eps' criterion.");
  else if (!strcmp (res, "xtol"))
    ut_print_message (0, 4, "Reached `x(r)eps' criterion.");
  else if (!strcmp (res, "maxeval"))
    ut_print_message (0, 4, "Reached `itermax' criterion.");
  else if (!strcmp (res, "maxtime"))
    ut_print_message (0, 4, "Reached `time' criterion.");
  else if (!strcmp (res, "eps"))
    ut_print_message (0, 4, "Reached `eps' criterion.");
  else if (!strcmp (res, "reps"))
    ut_print_message (0, 4, "Reached `reps' criterion.");
  else if (!strcmp (res, "ctrlc"))
    ut_print_message (1, 4, "Stopping on Ctrl+C invocation.");
  else if (!strcmp (res, "failure"))
    ut_print_message (1, 4, "Failed optimization.");
  else if (!strcmp (res, "roundoff_limited"))
    ut_print_message (1, 4, "Failed due to roundoff error.");
  else if (!strcmp (res, "forced_stop"))
    ut_print_message (1, 4, "Stopping on unknown forced stop.");
  else if (!strcmp (res, "invalid_args") || !strcmp (res, "out_of_memory"))
    ut_print_message (1, 4, "Failed due to unknown error.");
  else
    ut_print_message (0, 4, "Reached unknown criterion.");

  printf ("\n");

  return;
}
