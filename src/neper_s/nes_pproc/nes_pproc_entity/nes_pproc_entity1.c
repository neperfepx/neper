/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_.h"

void
nes_pproc_entity (struct SIM *pSim, struct TESS *pTess, struct TESR Tesr,
                  struct NODES *pNodes,
                  struct MESH *Mesh, char *entity, char *entityresexpr)
{
  int i, j, memberqty, status, resultqty;
  char **results = NULL, *dir = NULL, *result = NULL;
  char *message = ut_alloc_1d_char (1000);
  char *parent = NULL;
  struct SIMRES SimRes;

  neut_simres_set_zero (&SimRes);

  nes_pproc_entity_pre (pSim, entity, entityresexpr, &memberqty, &dir,
                        &results, &resultqty);

  if (resultqty)
  {
    ut_print_message (0, 2, "Writing results...\n");
    ut_dir_openmessage (dir, "w");
  }

  status = neut_sim_entity_parent (*pSim, entity, &parent);

  if (status)
    ut_print_message (2, 3, "Entity `%s' has no parent.\n", entity);

  // The option initializes the entity members only if needed
  neut_sim_entity_init_members (pSim, pTess, *pNodes, Mesh, entity);

  for (i = 0; i < resultqty; i++)
  {
    // special case: removing result
    if (results[i][0] == '!' || results[i][0] == '\\')
      nes_pproc_entity_remove (pSim, entity, results[i] + 1);

    // general case: adding result (includes the case of \result)
    if (results[i][0] != '!')
    {
      result = (results[i][0] == '\\') ? results[i] + 1 : results[i];

      neut_sim_simres (*pSim, entity, result, &SimRes);

      if (strcmp (SimRes.res, "ori") || SimRes.expr)
        sprintf (message, "%s", SimRes.res);
      else if (!neut_tess_isvoid (*pTess))
        sprintf (message, "%s (%s crystal symmetry)", SimRes.res,
                           (*pTess).CellCrySym ? (*pTess).CellCrySym : "undefined");
      else if (!neut_tesr_isvoid (Tesr))
        sprintf (message, "%s (%s crystal symmetry)", SimRes.res,
                           Tesr.CellCrySym ? Tesr.CellCrySym : "undefined");

      ut_print_message (0, 4, "%s ", message);
      for (j = 0; j < 49 - (int) strlen (message); j++)
        printf (".");
      printf (" ");

      ut_sys_mkdir (SimRes.dir);

      // result exists
      if (!strcmp (SimRes.status, "result"))
        printf ("\b\b\b exists\n");

      // result is a subresult (some specificities, do not merge with res_expr)
      else if (!strcmp (SimRes.status, "subresult"))
        nes_pproc_entity_subres (pSim, entity, &SimRes);

      // result is a file result
      else if (!strcmp (SimRes.status, "file"))
        nes_pproc_entity_file (pSim, entity, dir, SimRes.res, SimRes.expr);

      // treating result as an expression and then as a built-in result
      // (an expression is necessarily 1D while a built-in result is not).
      else if (!strcmp (SimRes.status, "unknown"))
      {
        status = nes_pproc_entity_expr (pSim, pTess, Tesr, pNodes, Mesh, entity, &SimRes);

        if (status)
          status = nes_pproc_entity_builtin (pSim, pTess, Tesr, pNodes, Mesh, entity, &SimRes);
      }

      else
        abort ();
    }
  }

  if (resultqty)
    ut_dir_closemessage (dir, "w");

  ut_free_2d_char (&results, resultqty);
  ut_free_1d_char (&dir);
  ut_free_1d_char (&message);
  neut_simres_free (&SimRes);
  ut_free_1d_char (&parent);

  return;
}
