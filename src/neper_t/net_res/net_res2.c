/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_res_.h"

void
net_res_sim (struct IN_T In, struct TESS Tess, struct TESR Tesr)
{
  FILE *file = NULL;
  char *body = NULL, *simtess = NULL, *simtesr = NULL;

  ut_file_removepath (In.body, &body);

  ut_dir_openmessage (In.sim, "w");

  if (!neut_tess_isvoid (Tess))
  {
    ut_string_sprintf (&simtess, "%s/inputs/%s.tess", In.sim, body);

    file = ut_file_open (simtess, "W");
    neut_tess_fprintf (file, Tess);
    ut_file_close (file, simtess, "W");
  }

  if (!neut_tesr_isvoid (Tesr))
  {
    ut_string_sprintf (&simtesr, "%s/inputs/%s.tesr", In.sim, body);

    file = ut_file_open (simtesr, "W");
    neut_tesr_fprintf (file, NULL, Tesr);
    ut_file_close (file, simtesr, "W");
  }

  ut_dir_closemessage (In.sim, "w");

  ut_free_1d_char (&body);
  ut_free_1d_char (&simtess);
  ut_free_1d_char (&simtesr);

  return;
}
