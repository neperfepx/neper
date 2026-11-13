/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_show_.h"

// nev_print_show_dim returns the dimension of the entity of a show option
// (3 to 0), or 4 for a general option (csys, scale, etc.)
int
nev_print_show_dim (char *entity, struct TESS Tess, struct TESR Tesr,
                    struct MESH *Mesh)
{
  if (!strcmp (entity, "poly") || !strcmp (entity, "vox")
      || !strcmp (entity, "voidvox") || !strcmp (entity, "faceinter"))
    return 3;
  else if (!strcmp (entity, "face"))
    return 2;
  else if (!strcmp (entity, "edge"))
    return 1;
  else if (!strcmp (entity, "ver") || !strcmp (entity, "seed")
           || !strcmp (entity, "node"))
    return 0;
  else if (!strcmp (entity, "cell"))
    return Tess.Dim > 0 ? Tess.Dim : Tesr.Dim;
  else if (!strcmp (entity, "elt") || !strcmp (entity, "elset")
           || !strcmp (entity, "eltco") || !strcmp (entity, "elsetco"))
    return neut_mesh_array_dim (Mesh);
  else if ((!strncmp (entity, "elset", 5) && strlen (entity) == 7)
           || (!strncmp (entity, "elt", 3) && strlen (entity) == 5))
    return entity[strlen (entity) - 2] - '0';
  else if (!strcmp (entity, "csys") || !strcmp (entity, "domain") || !strcmp (entity, "scale")
           || !strncmp (entity, "scalem", 6) || !strcmp (entity, "tess")
           || !strcmp (entity, "tesr") || !strcmp (entity, "mesh")
           || !strncmp (entity, "meshslice", 9))
    return 4;
  else                          // points
    return 0;
}
