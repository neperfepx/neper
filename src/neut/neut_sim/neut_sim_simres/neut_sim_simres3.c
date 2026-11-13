/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_simres_.h"

void
neut_sim_simres_type_read (struct SIM Sim, char *entity, char *res, char **ptype)
{
  int i, pos;

  neut_sim_entity_pos (Sim, entity, &pos);

  if (pos != -1)
    for (i = 0; i < Sim.EntityResQty[pos]; i++)
      if (!strcmp (Sim.EntityRes[pos][i], res))
      {
        ut_string_string (Sim.EntityResType[pos][i], ptype);
        break;
      }

  return;
}
