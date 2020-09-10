/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_povray_.h"

void
neut_povray_check (char *povray)
{
  int status, major, minor;
  char *tmp = ut_alloc_1d_char (1000);
  char *tmp2 = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  sprintf (tmp, "tmp%d", getpid ());
  sprintf (tmp2, "%s --version 2>&1 | head -1 > %s", povray, tmp);
  status = system (tmp2);

  if (status)
    ut_print_message (2, 2, "Povray not found (%s).\n", povray);

  else
  {
    file = ut_file_open (tmp, "R");

    if (fscanf (file, "%*s%d.%d", &major, &minor) != 2)
      ut_print_message (2, 2, "Failed to determine Povray version (%s).\n", povray);

    else if (major < 3 || minor < 7)
      ut_print_message (2, 2, "Povray version >= 3.7 required (%d.%d found for %s).\n",
                        major, minor, povray);

    ut_file_close (file, tmp, "R");
  }

  remove (tmp);

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);

  return;
}
