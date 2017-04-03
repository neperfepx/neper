/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_.h"

void
nem_interface (struct IN_M In, struct TESS Tess, struct NODES *pNodes,
               struct MESH *Mesh, struct BOUNDARY *pBound)
{
  if (!strcmp (In.interface, "continuous"))
    return;

  else if (!strcmp (In.interface, "discontinuous")
	|| !strcmp (In.interface, "cohesive"))
  {
    ut_print_message (0, 3, "Making mesh discontinuous...\n");
    nem_interface_dup (Tess, pNodes, Mesh, pBound);
  }

  if (!strcmp (In.interface, "cohesive"))
  {
    ut_print_message (0, 3, "Adding cohesive elements...\n");
    if (Tess.Dim == 3 && (*pNodes).Periodic && ut_array_1d_int_sum ((*pNodes).Periodic, 3) != 0)
    {
      ut_print_messagewnc (2, 72, "Option `-interface cohesive' is not available for periodic meshes.");
      abort ();
    }

    nem_interface_cohesive (Tess, *pNodes, Mesh, *pBound);
  }

  return;
}
