/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_parse_.h"

void
nes_sim_parse_fix_entity (struct IN_S In, struct SIM *pSim, char *entity)
{
  int i, inresqty, *presqty = NULL;
  char *inresstring = NULL, **inres = NULL, **res = NULL;

  if (neut_sim_entityisnode (entity))
  {
    inresstring = In.noderes;
    presqty = &(*pSim).NodeResQty;
    res = (*pSim).NodeRes;
  }
  else if (neut_sim_entityiselt (entity))
  {
    inresstring = In.eltres;
    ut_list_break (In.noderes, NEUT_SEP_NODEP, &inres, &inresqty);
    presqty = &(*pSim).EltResQty;
    res = (*pSim).EltRes;
  }
  else
    abort ();

  ut_list_break (inresstring, NEUT_SEP_NODEP, &inres, &inresqty);

  for (i = 0; i < inresqty; i++)
    if (strcmp (inres[i], "inputres") && !neut_sim_testres (*pSim, entity, inres[i]))
    {
      if (neut_sim_entityisnode (entity))
        neut_sim_addnoderes (pSim, inres[i], 1);
      else if (neut_sim_entityiselt (entity))
        neut_sim_addeltres (pSim, inres[i], 1);
      else
        abort ();
    }

  if (!ut_list_testelt (inresstring, NEUT_SEP_NODEP, "inputres"))
    for (i = 0; i < *presqty; i++)
    {
      if (!ut_list_testelt (inresstring, NEUT_SEP_NODEP, res[i]))
      {
        if (neut_sim_entityisnode (entity))
          neut_sim_rmnoderes (pSim, res[i--]);
        else if (neut_sim_entityiselt (entity))
          neut_sim_rmeltres (pSim, res[i--]);
        else
          abort ();
      }
    }

  ut_free_2d_char (&inres, inresqty);
  // do not free res
  // do not free inresstring

  return;
}
