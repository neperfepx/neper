/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_tree_.h"

void
nev_print_tree (struct IN_V In, char *basename, struct SIM Sim)
{
  char *filename = NULL, *outdir = NULL;
  int imagewidth, imageheight;

  ut_print_message (0, 1, "Printing tree...\n");

  if (neut_sim_isvoid (Sim))
  {
    ut_print_message (1, 2, "Simulation directory is void.  Skipping...\n");
    return;
  }

  neut_print_outdir (In.outdir, Sim, "pf", &outdir);
  if (strcmp (outdir, "."))
    ut_sys_mkdir (outdir);

  neut_sim_fprintf_asy (basename, Sim, "w");

  ut_file_dir_basename_extension_filename (outdir, basename, "asy", &filename);

  neut_print_imagesize (In.imagesize, &imagewidth, &imageheight);
  neut_asy_convert (In.asymptote, filename, imagewidth, imageheight,
                    In.imageformat, 2);

  if (!ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "asy"))
    remove (filename);

  ut_free_1d_char (&filename);
  ut_free_1d_char (&outdir);

  return;
}
