/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tesr_fscanf_.h"

void
neut_tesr_fnscanf (char *name, struct TESR *pTesr)
{
  FILE *file = NULL;
  char *name2 = NULL;

  ut_string_string (name, &name2);

  file = ut_file_open (name, "r");
  neut_tesr_fscanf (file, dirname (name2), pTesr);
  ut_file_close (file, name, "r");

  ut_free_1d_char (&name2);

  return;
}

void
neut_tesr_fscanf (FILE * file, char *dirname, struct TESR *pTesr)
{
  char *version = NULL, *dataformat = NULL, *oridataformat = NULL, string[1000];

  neut_tesr_free (pTesr);

  neut_tesr_fscanf_head (pTesr, &version, &dataformat, &oridataformat, file);

  neut_tesr_fscanf_cell (pTesr, version, file);

  do
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "**data"))
      neut_tesr_fscanf_data (pTesr, dirname, version, &dataformat, file);

    else if (!strcmp (string, "**oridata"))
      neut_tesr_fscanf_oridata (pTesr, dirname, version, &oridataformat, file);

    else if (!strcmp (string, "**oridef"))
      neut_tesr_fscanf_oridef (pTesr, dirname, file);
  }
  while (strcmp (string, "***end"));

  neut_tesr_fscanf_foot (file);

  neut_tesr_init_cellbbox (pTesr);
  neut_tesr_init_hasvoid (pTesr);

  ut_free_1d_char (&version);
  ut_free_1d_char (&dataformat);
  ut_free_1d_char (&oridataformat);

  return;
}
