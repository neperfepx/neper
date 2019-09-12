/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_smoothing_.h"

void
nem_smoothing (struct IN_M In, struct TESS *pTess, struct NODES *pNodes,
	       struct MESH *Mesh)
{
  int i;

  ut_print_message (0, 3, "Smoothing with algorithm `%s'...\n",
		    In.tesrsmooth);

  if (!strcmp (In.tesrsmooth, "laplacian"))
    for (i = 1; i <= neut_mesh_array_dim (Mesh); i++)
      nem_smoothing_laplacian (*pTess, pNodes, Mesh, i, In.tesrsmoothfact,
                               In.tesrsmoothitermax, "all");

  else if (!strcmp (In.tesrsmooth, "none"))
  {
  }
  else
    abort ();

  // Updating tess geometrical properties from the smoothed mesh
  nem_tess_updatefrommesh_geom (pTess, *pNodes, Mesh);

  return;
}
