/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_write_.h"

void
nes_convert_write_inputs (struct IN_S In, struct FEPXSIM *pFSim)
{
  int i, filenameqty = 0;
  char **filename = NULL;
  char *dir = ut_string_paste (In.simdir, "/inputs");

  if ((*pFSim).tess)
  {
    filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
    ut_string_string ((*pFSim).tess, filename + filenameqty - 1);
  }

  if ((*pFSim).msh)
  {
    filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
    ut_string_string ((*pFSim).msh, filename + filenameqty - 1);
  }

  if ((*pFSim).cfg)
  {
    filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
    ut_string_string ((*pFSim).cfg, filename + filenameqty - 1);
  }

  // *.sh
  filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
  ut_string_string ("*.sh", filename + filenameqty - 1);

  if ((*pFSim).bcs)
  {
    filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
    ut_string_string ((*pFSim).bcs, filename + filenameqty - 1);
  }
  if ((*pFSim).ori)
  {
    filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
    ut_string_string ((*pFSim).ori, filename + filenameqty - 1);
  }
  if ((*pFSim).phase)
  {
    filename = ut_realloc_1d_pchar_null (filename, ++filenameqty, 1);
    ut_string_string ((*pFSim).phase, filename + filenameqty - 1);
  }

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir ("%s/inputs", In.simdir);

  for (i = 0; i < filenameqty; i++)
    nes_convert_write_inputs_file (In, *pFSim, filename[i]);

  ut_dir_closemessage (dir, "w");

  ut_free_1d_char (&dir);
  ut_free_2d_char (&filename, filenameqty);

  return;
}

void
nes_convert_write_results (struct IN_S In, struct FEPXSIM *pFSim)
{
  char *dir = ut_string_paste (In.simdir, "/results");

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir (dir);

  nes_convert_write_results_entity (In, pFSim, "nodes");
  nes_convert_write_results_entity (In, pFSim, "elts");

  ut_free_1d_char (&dir);

  return;
}

void
nes_convert_write_restart (char *indir, char *outdir, struct FEPXSIM FSim)
{
  int i;
  char *dir = ut_string_paste (outdir, "/restart");
  char *filename = ut_alloc_1d_char (1000);
  char *infile = ut_alloc_1d_char (1000);
  char *outfile = ut_alloc_1d_char (1000);

  ut_dir_openmessage (dir, "w");
  ut_sys_mkdir (dir);

  sprintf (filename, "rst%d.control", FSim.RestartId);
  sprintf (infile, "%s/%s", indir, filename);
  sprintf (outfile, "%s/restart/%s", outdir, filename);

  ut_print_message (0, 4, "%s...\n", filename);
  ut_file_cp (infile, outfile);

  ut_print_message (0, 4, "rst%d.field.core*...\n", FSim.RestartId);
  for (i = 1; i <= FSim.PartQty; i++)
  {
    sprintf (filename, "rst%d.field.core%d", FSim.RestartId, i);
    sprintf (infile, "%s/%s", indir, filename);
    sprintf (outfile, "%s/restart/%s", outdir, filename);

    ut_file_cp (infile, outfile);
  }

  ut_free_1d_char (&dir);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&infile);
  ut_free_1d_char (&outfile);

  return;
}
