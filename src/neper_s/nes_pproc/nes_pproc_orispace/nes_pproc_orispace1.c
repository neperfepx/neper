/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nes_pproc_orispace_.h"

void
nes_pproc_orispace (struct IN_S In, char *crysym, struct SIM *pSim)
{
  int varqty;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (In.orispace, &fct, &vars, &vals, &varqty);

  if (strcmp (fct, "rodrigues") || varqty != 1)
    ut_print_message (2, 2, "Cannot process `%s'.\n", In.orispace);

  neut_ospace_fnscanf (crysym, fct, vals[0], &(*pSim).Sp, "r");

  if (strncmp ((*pSim).Sp.fr, "fr", 2))
  {
    ut_sys_mkdir ("%s/orispace", (*pSim).simdir);

    /*
    for (i = 0; i < partqty; i++)
    {
      ut_file_format (parts[i], &format);

      if (!strcmp (format, "tess"))
      {
        ut_file_squashname (parts[i], &(*pSim).OriSpaceTess);
        ut_file_removepath ((*pSim).OriSpaceTess, &(*pSim).OriSpaceTess);
        filename = ut_string_paste3 ((*pSim).simdir, "/orispace/", (*pSim).OriSpaceTess);
      }
      else if (!strcmp (format, "gmsh:msh"))
      {
        ut_file_squashname (parts[i], &(*pSim).OriSpaceMesh);
        ut_file_removepath ((*pSim).OriSpaceMesh, &(*pSim).OriSpaceMesh);
        filename = ut_string_paste3 ((*pSim).simdir, "/orispace/", (*pSim).OriSpaceMesh);
      }
      else
        abort ();

      ut_file_cp (parts[i], filename);
    }
    */
  }

  neut_sim_fprintf (In.simdir, *pSim, "W");

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return;
}
