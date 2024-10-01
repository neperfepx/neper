/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nes_entity_.h"

void
nes_entity (struct IN_S In, struct SIM *pSim)
{
  char *name = NULL;

  if (!strstr (In.entity, ":"))
    ut_print_message (2, 2, "Entity must be defined as `<name>:<expression>'.\n");

  neut_sim_entity_name (In.entity, &name);

  if (!neut_sim_entity_exist (*pSim, In.entity))
  {
    neut_sim_addentity (pSim, In.entity);
    if (!strcmp ((*pSim).EntityType[(*pSim).EntityQty - 1], "unknown"))
      ut_string_string ("elset", (*pSim).EntityType + (*pSim).EntityQty - 1);
  }

  else
    ut_print_message (2, 2, "Entity `%s' already exists.\n", name);

  // updating report file
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&name);

  return;
}
