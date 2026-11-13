/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nes_input_.h"

void
nes_input_treatargs (int fargc, char **fargv, int argc, char **argv,
                     struct IN_S *pIn)
{
  nes_input_options_default (pIn);

  // READING options
  nes_input_options_set (pIn, fargc, fargv);
  nes_input_options_set (pIn, argc, argv);

  /* input */
  if (!(*pIn).mergedir && !(*pIn).simdir)
    ut_print_message (2, 2,
		      "Missing input data.\n");

 if ((*pIn).mergedir)
 {
    while ((*pIn).mergedir[strlen ((*pIn).mergedir) - 1] == '/')
      (*pIn).mergedir[strlen ((*pIn).mergedir) - 1] = '\0';
    ut_string_fnrs ((*pIn).mergedir, "/,", ",", INT_MAX);
 }

  if ((*pIn).simdir)
    while ((*pIn).simdir[strlen ((*pIn).simdir) - 1] == '/')
      (*pIn).simdir[strlen ((*pIn).simdir) - 1] = '\0';

  // merging: output directory <directory_1>-<directory_2>-... by default
  // (without the .sim extensions), with the .sim extension added if missing
  if ((*pIn).mergedir)
  {
    if (!(*pIn).simdir)
    {
      ut_string_string ((*pIn).mergedir, &(*pIn).simdir);
      ut_string_fnrs ((*pIn).simdir, ".sim" NEUT_SEP_NODEP, NEUT_SEP_NODEP, INT_MAX);
      ut_string_fnrs ((*pIn).simdir, NEUT_SEP_NODEP, "-", INT_MAX);
    }

    if (ut_string_strcmp (strrchr ((*pIn).simdir, '.'), ".sim"))
    {
      char *tmp = ut_string_addextension ((*pIn).simdir, ".sim");
      ut_string_string (tmp, &(*pIn).simdir);
      ut_free_1d_char (&tmp);
    }
  }

  if (!(*pIn).orispace || !strcmp ((*pIn).orispace, "none"))
    ut_string_string ("rodrigues(fr20)", &(*pIn).orispace);

  return;
}
