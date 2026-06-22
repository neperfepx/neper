/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_ori_.h"

int
neut_ori_fscanf_id (FILE *file, char *prefix, int i, int *pid)
{
  if (!ut_string_strcmp (prefix, "id"))
  {
    if (fscanf (file, "%d", pid) != 1)
      return -1;
  }

  else
    (*pid) = i + 1;

  return 1;
}

void
neut_ori_fprintf_aslabel (FILE *file, char *format, char *label)
{
  if (!strcmp (format, "ascii"))
    fprintf (file, "   %s\n", label);
  else
    abort ();
}
