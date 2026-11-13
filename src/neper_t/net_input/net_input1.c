/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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

  // checking the formats (an unknown format was ignored, and so nothing
  // written)
  {
    int j, k, qty, valid;
    char **formats = NULL;
    const char *valids[] = { "tess", "geo", "mgeo", "3dec", "fe", "ply", "stl", "stl:bycell",
                             "obj", "ovm", "tesr", "raw", "vtk", "ori", "sim", "debug" };

    ut_list_break ((*pIn).format, NEUT_SEP_NODEP, &formats, &qty);
    for (j = 0; j < qty; j++)
    {
      valid = !strncmp (formats[j], "svg", 3);
      for (k = 0; !valid && k < (int) (sizeof (valids) / sizeof (valids[0])); k++)
        if (!strcmp (formats[j], valids[k]))
          valid = 1;
      if (!valid)
        ut_print_message (2, 0, "Unknown format `%s' (option -format).\n", formats[j]);
    }
    ut_free_2d_char (&formats, qty);
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
    if (!ut_file_exist ((*pIn).load))
      ut_print_message (2, 0, "%s: no such file.\n", (*pIn).load);
    ut_file_format ((*pIn).load, &(*pIn).mode);
  }

  // otherwise, tessellations are built in tess mode
  else if (tess_out || tesr_out || sim_out)
    ut_string_string ("tess", &(*pIn).mode);

  else if (ori_out)
    ut_string_string ("ori", &(*pIn).mode);

  if (!strcmp ((*pIn).mode, "obj") || !strcmp ((*pIn).mode, "ovm"))
    ut_string_string ("tess", &(*pIn).mode);

  net_input_orioptiini_check (*pIn);
  net_input_orisampling_check (*pIn);

  return;
}
