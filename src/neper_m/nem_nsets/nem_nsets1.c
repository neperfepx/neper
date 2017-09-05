/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets_.h"

void
nem_nsets (struct IN_M In, struct TESS Tess, struct TESR Tesr,
           struct NODES Nodes, struct MESH *Mesh, struct NSET *NSet)
{
  int i;

  if (In.nset == NULL || strlen (In.nset) == 0 || !strcmp (In.nset, "none"))
    return;

  if (!strcmp (In.elttype, "tri"))
  {
    if (Tess.Dim == 3 || Tesr.Dim == 3)
      nem_nsets_2d_tess (Tess, Mesh, NSet);

    if (Tess.Dim >= 2 || Tesr.Dim >= 2)
      nem_nsets_1d_tess (Tess, Mesh, NSet);

    if (Tess.Dim >= 1 || Tesr.Dim >= 1)
      nem_nsets_0d_tess (Tess, Mesh, NSet);

    if (ut_string_finds (In.nset, "all") != -1
	|| ut_string_finds (In.nset, "body") != -1
	|| ut_string_finds (In.nset, "bodies") != -1)
    {
      if (Tess.Dim == 3 || Tesr.Dim == 3)
	nem_nsets_2dbody_tess (Tess, NSet);

      if (Tess.Dim >= 2 || Tesr.Dim >= 2)
	nem_nsets_1dbody_tess (Tess, NSet);
    }
  }

  else if (!strcmp (In.elttype, "quad"))
  {
    if (Tess.PolyQty > 0 && !strcmp (Tess.DomType, "cube"))
    {
      nem_nsets_1d_tess_str (Tess, NSet);
      nem_nsets_0d_tess_str (Tess, NSet);

      if (ut_string_finds (In.nset, "all") != -1
	  || ut_string_finds (In.nset, "body") != -1
	  || ut_string_finds (In.nset, "bodies") != -1)
      {
	nem_nsets_2dbody_tess (Tess, NSet);
	nem_nsets_1dbody_tess (Tess, NSet);
      }
    }

    else
    {
      if (NSet[2].qty > 0)
	nem_nsets_1d_str (NSet);

      if (NSet[1].qty > 0)
	nem_nsets_0d_str (NSet);

      if (ut_string_finds (In.nset, "all") != -1
	  || ut_string_finds (In.nset, "body") != -1
	  || ut_string_finds (In.nset, "bodies") != -1)
      {
	nem_nsets_2dbody_str (NSet);
	nem_nsets_1dbody_str (NSet);
      }
    }
  }

  // Adding duplicate nodes
  for (i = 0; i < Tess.Dim; i++)
    nem_nset_addduplicates (NSet + i, Nodes);

  return;
}
