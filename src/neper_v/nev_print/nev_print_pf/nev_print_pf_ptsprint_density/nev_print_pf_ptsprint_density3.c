/* This id is part of the 'hermes' program. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYINPFG id in the top-level directory. */

#include "nev_print_pf_ptsprint_density_.h"

void
nev_print_pf_ptsprint_density_write_data (struct PF Pf, char *filename,
                                          double **ldensity)
{
  int i, j;
  FILE *file = ut_file_open (filename, "W");

  for (i = 0; i <= Pf.gridsize[0] + 1; i++)
    for (j = 0; j <= Pf.gridsize[1] + 1; j++)
      fprintf (file, "%f %f %f\n",
               Pf.lgrid[i][j][0], Pf.lgrid[i][j][1], ldensity[i][j]);

  ut_file_close (file, filename, "W");

  return;
}

void
nev_print_pf_ptsprint_density_write_text (struct PF Pf, FILE *file, char *filename2)
{
  fprintf (file, "int n0 = %d;\n", Pf.gridsize[0] + 2);
  fprintf (file, "int n1 = %d;\n", Pf.gridsize[1] + 2);
  fprintf (file, "int n = n0*n1;\n");
  fprintf (file, "pair[]points = new pair[n];\n");
  fprintf (file, "real[]values = new real[n];\n");
  fprintf (file, "\n");
  fprintf (file, "file in = (input (\"%s\")).line();\n", filename2);
  fprintf (file, "real[][]tmp = in.dimension(0, 0);\n");
  fprintf (file, "for (int i = 0; i < n; ++i)\n");
  fprintf (file, "{\n");
  if (strcmp (Pf.space, "ipf"))
    fprintf (file, "  points[i] = scale*(tmp[i][0], tmp[i][1]);\n");
  else
    fprintf (file, "  points[i] = scale*(tmp[i][0], tmp[i][1]);\n");
  fprintf (file, "  values[i] = tmp[i][2];\n");
  fprintf (file, "}\n");
  fprintf (file, "\n");

  fprintf (file, "real[][] v=new real[n0][n1];\n");
  fprintf (file, "int id = 0;\n");
  fprintf (file, "for (int i = 0; i < n0; ++i)\n");
  fprintf (file, "  for (int j = 0; j < n1; ++j)\n");
  fprintf (file, "  {\n");
  fprintf (file, "    v[i][j] = values[id];\n");
  fprintf (file, "    ++id;\n");
  fprintf (file, "  }\n");
  fprintf (file, "\n");

  return;
}
