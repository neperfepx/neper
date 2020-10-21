/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_.h"

void
nes_pproc_entity (struct IN_S In, struct SIM *pSim, struct TESS Tess,
                  struct NODES *pNodes, struct MESH *Mesh, char *entity)
{
  int i, j, entityqty, status, resultqty;
  char **results = NULL, *dir = NULL;

  nes_pproc_entity_pre (In, *pSim, entity, &entityqty, &dir, &results,
                        &resultqty);

  if (resultqty)
  {
    ut_print_message (0, 2, "Writing results...\n");
    ut_dir_openmessage (dir, "w");
  }

  for (i = 0; i < resultqty; i++)
  {
    char *result = NULL, *expr = NULL;

    ut_list_break_2 (results[i], NEUT_SEP_DEP, &result, &expr);

    // adding result
    if (result[0] != '!')
    {
      ut_print_message (0, 4, "%s ", result);
      for (j = 0; j < 49 - (int) strlen (result); j++)
        printf (".");
      printf (" ");

      if (result[0] != '!')
        ut_sys_mkdir ("%s/%s", dir, result);

      status = neut_sim_res_exist (*pSim, entity, result, NULL, NULL);

      // result exists
      if (status == 1)
        printf ("\b\b\b exists\n");

      // result does not exist in the sim directory but is a subresult
      else if (status == 2)
        nes_pproc_entity_subres (pSim, entity, entityqty, dir, result);

      else if (expr && !strncmp (expr, "file(", 5))
        nes_pproc_entity_file (pSim, entity, dir, result, expr);

      // result is an expression
      else
        nes_pproc_entity_expr (pSim, Tess, pNodes, Mesh, entity, entityqty, dir, result, expr);
    }

    // removing result
    else
    {
      ut_print_message (0, 4, "%s removal ", result + 1);
      for (j = 0; j < 41 - (int) strlen (result + 1); j++)
        printf (".");
      printf (" ");

      nes_pproc_entity_remove (pSim, entity, dir, result);
    }

    ut_free_1d_char (&result);
    ut_free_1d_char (&expr);
  }

  if (resultqty)
    ut_dir_closemessage (dir, "w");

  ut_free_2d_char (&results, resultqty);
  ut_free_1d_char (&dir);

  return;
}
