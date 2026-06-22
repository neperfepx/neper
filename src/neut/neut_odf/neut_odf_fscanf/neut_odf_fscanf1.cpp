/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_fscanf_.h"
#include "neut/neut_oset/neut_oset.hpp"

void
neut_odf_fnscanf (char *filename_in, struct ODF *pOdf, char *mode)
{
  FILE *file = NULL;
  char *filename = NULL;

  if (!filename_in)
    ut_print_message (2, 2, "\nneut_odf_fnscanf: input is NULL\n");

  if (!strncmp (filename_in, "odf(", 4))
  {
    // skipping "odf(" and removing trailing ')'
    ut_string_string (filename_in + 4, &filename);
    filename[strlen (filename) - 1] = '\0';
  }
  else
    ut_string_string (filename_in, &filename);

  file = ut_file_open (filename, mode);
  neut_odf_fscanf (file, pOdf);
  ut_file_close (file, filename, mode);

  ut_free_1d_char (&filename);

  return;
}

void
neut_odf_fscanf (FILE *file, struct ODF *pOdf)
{
  char *version = NULL;
  char *string = ut_alloc_1d_char (1000);

  neut_odf_set_zero (pOdf);

  neut_odf_fscanf_head (file, &version);

  if (ut_file_nextstring_test (file, (char *) "**space"))
    neut_odf_fscanf_space (file, pOdf);

  if (ut_file_nextstring_test (file, (char *) "**eltdata"))
    neut_odf_fscanf_eltdata (file, pOdf);

  if (ut_file_nextstring_test (file, (char *) "**nodedata"))
    neut_odf_fscanf_nodedata (file, pOdf);

  ut_free_1d_char (&version);
  ut_free_1d_char (&string);

  return;
}
