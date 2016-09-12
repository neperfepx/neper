/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_foot_.h"

void
nev_print_foot (FILE * file, struct PRINT Print)
{
  int i;
  char **parts;
  int partqty;
  FILE *filein = NULL;

  if (Print.includepov != NULL)
  {
    char *line = ut_alloc_1d_char (1000);

    ut_string_separate (Print.includepov, NEUT_SEP_NODEP, &parts, &partqty);

    for (i = 0; i < partqty; i++)
    {
      filein = ut_file_open (parts[i], "R");

      fprintf (file, "\n");
      fprintf (file, "// Including file %s ---------\n", parts[i]);
      while (fgets (line, 1000, filein) != NULL)
	fprintf (file, "%s", line);
      fprintf (file, "// Included file %s ---------\n", parts[i]);
      fprintf (file, "\n");

      ut_file_close (filein, parts[i], "R");
    }

    ut_free_1d_char (line);
  }

  fprintf (file, "rotate<-90,  0,  0>\n");
  fprintf (file, "scale <  1,  1, -1>\n");
  fprintf (file, "}\n");

  return;
}
