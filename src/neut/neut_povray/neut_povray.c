/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_povray_.h"

int
neut_povray_check (char *povray)
{
  int status;

  status = ut_sys_which (povray);

  return status;
}

int
neut_povray_check_error (char *povray)
{
  if (neut_povray_check (povray) != 0)
    ut_print_message (2, 0, "POV-Ray (command `%s') not found.  Install it or specify it using option `-povray'.\n", povray);

  return 0;
}
