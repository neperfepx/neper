/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_interface_.h"

// types of the interfaces (faces in 3D, edges in 2D) [1...InterQty]:
// NEM_INTERFACE_CONTINUOUS, NEM_INTERFACE_DISCONTINUOUS or
// NEM_INTERFACE_COHESIVE, from their scales.  Domain interfaces are
// continuous.
void
nem_interface_intertypes (struct IN_M In, struct TESS Tess, int **pintertypes)
{
  int i, scale, partqty, interqty, *types = NULL;
  char **parts = NULL;

  ut_list_break (In.interface, NEUT_SEP_FRAC, &parts, &partqty);

  // [0...ScaleQty], 0 being the domain
  types = ut_alloc_1d_int (Tess.ScaleQty + 1);
  for (i = 1; i <= Tess.ScaleQty; i++)
  {
    char *part = parts[ut_num_min_int (i, partqty) - 1];

    if (!strcmp (part, "continuous"))
      types[i] = NEM_INTERFACE_CONTINUOUS;
    else if (!strcmp (part, "discontinuous"))
      types[i] = NEM_INTERFACE_DISCONTINUOUS;
    else if (!strcmp (part, "cohesive"))
      types[i] = NEM_INTERFACE_COHESIVE;
    else
      ut_print_message (2, 3, "Failed to process `-interface %s'.\n", In.interface);
  }

  interqty = (Tess.Dim == 3) ? Tess.FaceQty : Tess.EdgeQty;
  (*pintertypes) = ut_alloc_1d_int (interqty + 1);

  for (i = 1; i <= interqty; i++)
  {
    if (Tess.Dim == 3)
      neut_tess_face_scale (Tess, i, &scale);
    else
      neut_tess_edge_scale (Tess, i, &scale);

    (*pintertypes)[i] = types[scale];
  }

  ut_free_1d_int (&types);
  ut_free_2d_char (&parts, partqty);

  return;
}
