/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_sim_.h"

void
nes_sim_modify_entity (struct IN_S In, struct SIM *pSim, char *entity)
{
  int i, *presqty = NULL;
  char *inresstring = NULL, **res = NULL;

  if (neut_sim_entityisnode (entity))
  {
    inresstring = In.noderes;
    presqty = &(*pSim).NodeResQty;
    res = (*pSim).NodeRes;
  }
  else if (neut_sim_entityiselt (entity))
  {
    inresstring = In.eltres;
    presqty = &(*pSim).EltResQty;
    res = (*pSim).EltRes;
  }
  else
    abort ();


  // if inputres present, nothing to do
  if (ut_list_testelt (inresstring, NEUT_SEP_NODEP, "inputres"))
    return;

  // if none, removing all
  else if (!strcmp (inresstring, "none"))
  {
    for (i = 0; i < *presqty; i++)
      neut_sim_rmres (pSim, entity, res[i--]);

    return;
  }

  // removing results not listed
  else
    for (i = 0; i < *presqty; i++)
      if (!ut_list_testelt (inresstring, NEUT_SEP_NODEP, res[i]))
        neut_sim_rmres (pSim, entity, res[i--]);

  // do not free res
  // do not free inresstring
  // do not free presqty

  return;
}
