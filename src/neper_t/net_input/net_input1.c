/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input_.h"

void
net_input (struct IN_T *pIn, int fargc, char **fargv, int argc, char **argv)
{
  int tess_out, tesr_out, ori_out, sim_out;

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
  sim_out = 0;
  if (ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "tess") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "geo") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "mgeo") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "3dec") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "fe") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "ply") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "stl") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "stl:bycell") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "obj") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "ovm") == 1
      || strstr ((*pIn).format, "svg"))
    tess_out = 1;
  if (ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "tesr") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "raw") == 1
      || ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "vtk") == 1)
    tesr_out = 1;
  if (tess_out == 0 && tesr_out == 0
      && ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "ori") == 1)
    ori_out = 1;
  if (sim_out == 0
      && ut_list_testelt ((*pIn).format, NEUT_SEP_NODEP, "sim") == 1)
    sim_out = 1;

  // if loading from a file, setting mode to its format
  if (strstr ((*pIn).format, "svg") && (*pIn).dim == 3)
    ut_print_message (2, 0, "`-format svg' requires `-dim 2'.\n");

  // Setting mode

  // if loading from a file, setting mode to its format
  if (strcmp ((*pIn).load, "none"))
  {
    int PartQty;
    char **parts = NULL;
    ut_list_break ((*pIn).load, NEUT_SEP_DEP, &parts, &PartQty);
    if (!ut_file_exist (parts[0]))
      ut_print_message (2, 0, "%s: no such file.\n", parts[0]);
    ut_file_format (parts[0], &(*pIn).mode);
  }

  // otherwise, tessellations are built in tess mode
  else if (tess_out || tesr_out || sim_out)
    ut_string_string ("tess", &(*pIn).mode);

  else if (ori_out)
    ut_string_string ("ori", &(*pIn).mode);

  if (!strcmp ((*pIn).mode, "obj") || !strcmp ((*pIn).mode, "ovm"))
    ut_string_string ("tess", &(*pIn).mode);

  return;
}
