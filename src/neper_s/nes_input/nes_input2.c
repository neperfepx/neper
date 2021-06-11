/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
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
  if (!(*pIn).fepxdir && !(*pIn).mergedir && !(*pIn).simdir)
    ut_print_message (2, 2,
		      "Missing input data.\n");

  /* fepxdir */
  if ((*pIn).fepxdir)
    if (ut_sys_isdir ((*pIn).fepxdir))
      if (!strcmp ((*pIn).fepxdir, "."))
        (*pIn).fepxdir = ut_sys_realpath ((*pIn).fepxdir);

  // removing trailing '/'
  if ((*pIn).fepxdir)
    while ((*pIn).fepxdir[strlen ((*pIn).fepxdir) - 1] == '/')
      (*pIn).fepxdir[strlen ((*pIn).fepxdir) - 1] = '\0';

 if ((*pIn).mergedir)
 {
    while ((*pIn).mergedir[strlen ((*pIn).mergedir) - 1] == '/')
      (*pIn).mergedir[strlen ((*pIn).mergedir) - 1] = '\0';
    ut_string_fnrs ((*pIn).mergedir, "/,", ",", INT_MAX);
 }

  if ((*pIn).simdir)
    while ((*pIn).simdir[strlen ((*pIn).simdir) - 1] == '/')
      (*pIn).simdir[strlen ((*pIn).simdir) - 1] = '\0';

  if (!(*pIn).simdir)
  {
    if ((*pIn).fepxdir)
      (*pIn).simdir = ut_string_addextension ((*pIn).fepxdir, ".sim");
    else if ((*pIn).mergedir)
    {
      ut_string_string ((*pIn).mergedir, &(*pIn).simdir);
      ut_string_fnrs ((*pIn).simdir, NEUT_SEP_NODEP, "-", INT_MAX);
    }
  }

  return;
}
