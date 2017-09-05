/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"
#ifdef HAVE_LIBSCOTCH

void
nem_partition (struct IN_M In, struct NODES *pNodes, struct MESH *Mesh,
	       struct PART *pPart)
{
  int dim = neut_mesh_array_dim (Mesh);

  SCOTCH_Mesh SCMesh;
  SCOTCH_Arch Arch;
  SCOTCH_archInit (&Arch);

  ut_print_message (0, 2, "Initializing...\n");
  nem_partition_init (In, &Arch, pPart);

  ut_print_message (0, 2, "Preparing data...\n");
  nem_partition_prep (pNodes, Mesh, pPart, &SCMesh);

  ut_print_message (0, 2, "Partitioning nodes...\n");
  nem_partition_nodes (In, &SCMesh, &Arch, pNodes, Mesh, pPart);

  ut_print_message (0, 2, "Partitioning elements...\n");
  nem_partition_elts (In, Mesh, pPart);

  ut_print_message (0, 2, "Partitioning properties:\n");
  nem_partition_stats ((*pPart).qty, Mesh[dim], *pPart);

  return;
}

#endif
