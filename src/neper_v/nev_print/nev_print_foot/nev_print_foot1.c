/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_foot_.h"

void
nev_print_foot (FILE * file, struct PRINT Print)
{
  int i, j, k, povqty, *povqty2 = NULL, argqty;
  double tmp;
  char ***povs = NULL;
  FILE *filein = NULL;
  char *line = ut_alloc_1d_char (1000);
  char *fct = NULL, **vars = NULL, **vals = NULL;

  if (Print.includepov)
  {
    ut_string_separate2 (Print.includepov, NEUT_SEP_NODEP, NEUT_SEP_DEP, &povs, &povqty2, &povqty);

    for (i = 0; i < povqty; i++)
    {
      filein = ut_file_open (povs[i][0], "R");

      fprintf (file, "\n");
      fprintf (file, "// Including file %s ---------\n", povs[i][0]);

      for (j = 2; j <= povqty2[i]; j++)
        fprintf (file, "union {\n\n");

      while (fgets (line, 1000, filein) != NULL)
	fprintf (file, "%s", line);
      fprintf (file, "// Included file %s ---------\n", povs[i][0]);

      for (j = 2; j <= povqty2[i]; j++)
      {
        ut_string_function_separate (povs[i][j - 1], &fct, &vars, &vals, &argqty);

        fprintf (file, "\n%s<", fct);
        for (k = 0; k < argqty; k++)
        {
          ut_math_eval (vals[k], 0, NULL, NULL, &tmp);
          fprintf (file, "%.12f%s", tmp, k < argqty - 1 ? "," : ">\n");
        }
        fprintf (file, "}\n");

        ut_free_2d_char (vars, argqty);
        ut_free_2d_char (vals, argqty);
      }
      fprintf (file, "\n");

      ut_file_close (filein, povs[i][0], "R");
    }
  }

  if (!ut_string_inlist (Print.format, NEUT_SEP_NODEP, "pov:objects"))
  {
    fprintf (file, "rotate<-90,  0,  0>\n");
    fprintf (file, "scale <  1,  1, -1>\n");
  }
  fprintf (file, "}\n");

  // ut_free_3d_char (povs, povqty);
  ut_free_1d_int (povqty2);
  ut_free_1d_char (line);

  return;
}
