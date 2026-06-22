/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_odf_fscanf_.h"
#include "neut/neut_oset/neut_oset.hpp"

void
neut_odf_fscanf_head (FILE *file, char **pversion)
{
  if (!ut_file_string_scanandtest (file, "***odf")
      || !ut_file_string_scanandtest (file, "**format"))
    ut_print_message (2, 0, "Input file is not a valid odf file.\n");

  (*pversion) = ut_alloc_1d_char (100);
  if (fscanf (file, "%s", *pversion) != 1)
    ut_print_message (2, 0, "Input file is not a valid odf file.\n");

  if (strcmp (*pversion, "1.0"))
    ut_print_message (2, 0, "Input file is not a valid odf file.\n");

  return;
}

void
neut_odf_fscanf_space (FILE *file, struct ODF *pOdf)
{
  int level;
  char *string = ut_alloc_1d_char (1000);
  char *string2 = ut_alloc_1d_char (1000);
  char *crysym = NULL, *space = ut_alloc_1d_char (100), *fr = NULL;

  ut_file_skip (file, 1);
  if (fscanf (file, "%s", space) != 1)
    abort ();

  ut_file_nextstring_sectionlevel (file, &level);
  while (!ut_file_nextstring_sectionlevel (file, &level) && level == 1)
  {
    ut_file_nextstring (file, string);

    if (!strcmp (string, "*crysym"))
    {
      if (fscanf (file, "%*s%s", string2) != 1)
        abort ();
      ut_string_string (string2, &crysym);
    }

    else if (!strcmp (string, "*fr"))
    {
      if (fscanf (file, "%*s%s", string2) != 1)
        abort ();
      ut_string_string (string2, &fr);
    }

    else if (!strcmp (string, "*theta"))
    {
      if (fscanf (file, "%*s%lf", &((*pOdf).sigma)) != 1)
        abort ();
      (*pOdf).sigma *= M_PI / 180;
    }

    else
      ut_print_message (2, 2, "Could not read field `%s'.\n", string);
  }

  neut_ospace_fnscanf (crysym, space, fr, &(*pOdf).Sp, (char *) "R");

  ut_free_1d_char (&string);
  ut_free_1d_char (&string2);
  ut_free_1d_char (&crysym);
  ut_free_1d_char (&space);
  ut_free_1d_char (&fr);

  return;
}

void
neut_odf_fscanf_eltdata (FILE *file, struct ODF *pOdf)
{
  int i;
  FILE *file2 = NULL;
  char *filename = NULL;
  char *dataformat = ut_alloc_1d_char (10);

  ut_file_skip (file, 1);

  if (fscanf (file, "%s", dataformat) != 1
      || (!strcmp (dataformat, "ascii") && !strncmp (dataformat, "bin", 3)))
    abort ();

  ut_file_scan_file (file, NULL, &file2, &filename);

  (*pOdf).odfqty = (*pOdf).Sp.Mesh[3].EltQty;
  (*pOdf).odf = ut_alloc_1d ((*pOdf).odfqty);

  for (i = 0; i < (*pOdf).odfqty; i++)
  {
    if (!strcmp (dataformat, "ascii"))
    {
      if (fscanf (file2, "%lf", (*pOdf).odf + i) != 1)
        abort ();
    }
    else
      abort ();
  }

  if (filename && !ut_file_isendoffile (file2))
    ut_print_message (2, 4, "File contains more data than needed.\n");

  if (filename)
    ut_file_close (file2, filename, "r");

  ut_free_1d_char (&dataformat);
  ut_free_1d_char (&filename);

  return;
}

void
neut_odf_fscanf_nodedata (FILE *file, struct ODF *pOdf)
{
  int i;
  FILE *file2 = NULL;
  char *filename = NULL;
  char *dataformat = ut_alloc_1d_char (10);

  ut_file_skip (file, 1);

  if (fscanf (file, "%s", dataformat) != 1
      || (!strcmp (dataformat, "ascii") && !strncmp (dataformat, "bin", 3)))
    abort ();

  ut_file_scan_file (file, NULL, &file2, &filename);

  (*pOdf).odfnqty = (*pOdf).Sp.Nodes.NodeQty;
  (*pOdf).odfn = ut_alloc_1d ((*pOdf).odfnqty);

  for (i = 0; i < (*pOdf).odfnqty; i++)
  {
    if (!strcmp (dataformat, "ascii"))
    {
      if (fscanf (file2, "%lf", (*pOdf).odfn + i) != 1)
        abort ();
    }
    else
      abort ();
  }

  if (!filename && !ut_file_isendoffile (file2))
    ut_print_message (2, 4, "File contains more data than needed.\n");

  if (filename)
    ut_file_close (file2, filename, "r");

  ut_free_1d_char (&dataformat);
  ut_free_1d_char (&filename);

  return;
}
