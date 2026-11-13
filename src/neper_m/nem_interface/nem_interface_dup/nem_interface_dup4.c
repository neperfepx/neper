/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_dup_.h"

// Tests whether 2 seeds of an entity (vertex or edge) are joined by a
// continuous interface (i.e. not in inters) around the entity.  A
// periodic seed is not a cell, so the test is done after translating the
// entity so that the first seed becomes a cell: the interface is then
// between its master cell and the seed of the master cell of the second
// seed at the relative periodicity shift.
int
nem_interface_dup_seedpair_continuous (struct TESS Tess, int *inters, int interqty, int dim,
                                       int id, int seed1, int seed2)
{
  int i, j, status, inter, entid, master[2], shift[2][3], tmp[3];
  int entinterqty, *entinters = NULL, seedqty, *seeds = NULL;

  for (i = 0; i < 2; i++)
  {
    neut_tess_seed_masterseed (Tess, i == 0 ? seed1 : seed2, master + i);
    ut_array_1d_int_zero (shift[i], 3);
    if ((i == 0 ? seed1 : seed2) > Tess.CellQty)
      ut_array_1d_int_memcpy (Tess.PerSeedShift[i == 0 ? seed1 : seed2], 3, shift[i]);
  }

  // translating the entity and the shifts by -shift[0]
  ut_array_1d_int_memcpy (shift[0], 3, tmp);
  ut_array_1d_int_scale (tmp, 3, -1);
  if (nem_interface_dup_entity_shift (Tess, dim, id, tmp, &entid) != 0)
    return 0;
  ut_array_1d_int_sub (shift[0], shift[1], 3, shift[1]);
  ut_array_1d_int_zero (shift[0], 3);

  if (dim == 0)
    neut_tess_ver_inters (Tess, entid, &entinters, &entinterqty);
  else
    neut_tess_edge_faces (Tess, entid, &entinters, &entinterqty);

  status = 0;
  for (i = 0; i < entinterqty && !status; i++)
  {
    inter = entinters[i];

    if (ut_array_1d_int_eltpos (inters, interqty, inter) != -1)
      continue;

    neut_tess_inter_seeds_positive (Tess, inter, &seeds, &seedqty);

    if (seedqty == 2)
      for (j = 0; j < 2 && !status; j++)
      {
        int m0, m1, t1[3];

        neut_tess_seed_masterseed (Tess, seeds[j], &m0);
        neut_tess_seed_masterseed (Tess, seeds[1 - j], &m1);
        ut_array_1d_int_zero (t1, 3);
        if (seeds[1 - j] > Tess.CellQty)
          ut_array_1d_int_memcpy (Tess.PerSeedShift[seeds[1 - j]], 3, t1);

        if (seeds[j] <= Tess.CellQty && m0 == master[0] && m1 == master[1]
            && ut_array_1d_int_equal (t1, 3, shift[1], 3))
          status = 1;
      }

    ut_free_1d_int (&seeds);
  }

  ut_free_1d_int (&entinters);

  return status;
}

// periodic image of an entity (vertex or edge) at a periodicity shift
// from it; returns -1 if none
int
nem_interface_dup_entity_shift (struct TESS Tess, int dim, int id, int *shift, int *pid)
{
  int i, master, slave, slaveqty, *slaves = NULL, **pershift = NULL, target[3];

  (*pid) = -1;

  if (!shift[0] && !shift[1] && !shift[2])
  {
    (*pid) = id;
    return 0;
  }

  if (!strcmp (Tess.Type, "standard"))
    return -1;

  if (dim == 0)
  {
    master = Tess.PerVerMaster[id] > 0 ? Tess.PerVerMaster[id] : id;
    slaveqty = Tess.PerVerSlaveQty[master];
    slaves = Tess.PerVerSlaveNb[master];
    pershift = Tess.PerVerShift;
  }
  else
  {
    master = Tess.PerEdgeMaster[id] > 0 ? Tess.PerEdgeMaster[id] : id;
    slaveqty = Tess.PerEdgeSlaveQty[master];
    slaves = Tess.PerEdgeSlaveNb[master];
    pershift = Tess.PerEdgeShift;
  }

  // shift of the target wrt the master
  ut_array_1d_int_memcpy (shift, 3, target);
  if (master != id)
    ut_array_1d_int_add (target, pershift[id], 3, target);

  if (!target[0] && !target[1] && !target[2])
    (*pid) = master;
  else
    for (i = 1; i <= slaveqty; i++)
    {
      slave = slaves[i];
      if (ut_array_1d_int_equal (pershift[slave], 3, target, 3))
      {
        (*pid) = slave;
        break;
      }
    }

  return (*pid) == -1 ? -1 : 0;
}
