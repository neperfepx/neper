/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input_.h"

void
net_input (struct IN_T *pIn, int fargc, char **fargv, int argc, char **argv)
{
  int tess_out, tesr_out, ori_out;

  net_input_treatargs (fargc, fargv, argc, argv, pIn);

  if (!strcmp ((*pIn).format, "none"))
  {
    if (strcmp ((*pIn).load, "none"))
      ut_string_string ((*pIn).input, &(*pIn).format);
    else
      ut_string_string ("tess", &(*pIn).format);
  }

  tess_out = 0;
  tesr_out = 0;
  ori_out = 0;
  if (ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "tess") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "mtess") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "geo") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "mgeo") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "3dec") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "fe") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "ply") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "stl") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "obj") == 1)
    tess_out = 1;
  if (ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "tesr") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "raw") == 1
      || ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "vtk") == 1)
    tesr_out = 1;
  if (tess_out == 0 && tesr_out == 0
      && ut_string_inlist ((*pIn).format, NEUT_SEP_NODEP, "ori") == 1)
    ori_out = 1;

  // Setting mode

  // if loading from a file, setting mode to its format
  if (strcmp ((*pIn).load, "none"))
  {
    if (!ut_file_exist ((*pIn).load))
      ut_print_message (2, 0, "%s: no such file.\n", (*pIn).load);
    ut_file_format ((*pIn).load, &(*pIn).mode);
  }

  // otherwise, tessellations are built in tess mode
  else
    ut_string_string ("tess", &(*pIn).mode);

  (void) ori_out;

  return;
}
