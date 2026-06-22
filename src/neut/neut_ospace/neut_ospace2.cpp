/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_ospace_.h"

void
neut_ospace_fnscanf_tess (char *crysym, char *space, struct OSPACE *pOSp, char *mode)
{
  char *filename = ut_alloc_1d_char (1000);
  char *data = NULL;

  (void) space;

  if (neut_sys_data (&data, 0))
    abort ();

  sprintf (filename, "%s/orispace/%s/fr.tess", data, crysym);

  neut_tess_fnscanf (filename, &((*pOSp).Tess), mode);

  ut_free_1d_char (&filename);
  ut_free_1d_char (&data);

  return;
}

void
neut_ospace_fnscanf_mesh (char *crysym, char *space, char *fr, struct OSPACE *pOSp, char *mode)
{
  int i, dim;
  char *filename = ut_alloc_1d_char (1000);
  char *data = NULL;

  (void) space;

  if (neut_sys_data (&data, 0))
    abort ();

  if (!strncmp (fr, "fr", 2))
    sprintf (filename, "%s/orispace/%s/%s.msh", data, crysym, fr);
  else
    ut_string_string (fr, &filename);

  neut_mesh_fnscanf_msh (filename, &((*pOSp).Nodes),
                         (*pOSp).Mesh, (*pOSp).Mesh + 1,
                         (*pOSp).Mesh + 2, (*pOSp).Mesh + 3, NULL, NULL, mode);

  dim = neut_mesh_array_dim ((*pOSp).Mesh);

  ut_string_string ((*pOSp).Mesh[dim].Domain, &((*pOSp).Nodes.Domain));
  if (!(*pOSp).Mesh[dim].Domain)
    ut_print_message (2, 2, "Mesh domain not defined\n");

  ut_string_string (space, &((*pOSp).space));

  if (!strncmp ((*pOSp).Mesh[dim].Domain, "euler-bunge", 11))
  {
    ut_string_string ("radian", &(*pOSp).spaceunit);
    if (!strcmp ((*pOSp).space, "euler-bunge"))
    {
      double **bbox = ut_alloc_2d (3, 2);
      neut_nodes_bbox ((*pOSp).Nodes, bbox);
      for (i = 0; i < 3; i++)
        if (bbox[i][1] > 10) // meaning we are in degrees and not radians
        {
          ut_string_string ("degree", &(*pOSp).spaceunit);
          break;
        }
      ut_free_2d (&bbox, 3);
    }
  }

  neut_mesh_init_eltori ((*pOSp).Nodes, (*pOSp).Mesh + 3);
  neut_nodes_init_nodeori (&(*pOSp).Nodes);
  neut_mesh_init_eltweight ((*pOSp).Nodes, (*pOSp).Mesh + 3);
  neut_nodes_init_nodeweight (&(*pOSp).Nodes, (*pOSp).Mesh + 3);

  ut_free_1d_char (&filename);

  return;
}
