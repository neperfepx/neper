/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_mesh_.h"

void
nev_print_data_mesh (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes,
                      struct MESH **pMesh, char *entity_in, char *attribute,
                      char *datastring, struct DATA **MeshData)
{
  int dim, entityqty;
  struct DATA *pData = NULL;
  char *entity = NULL, *type = NULL, *value = NULL;;

  ut_string_string (entity_in, &entity);

  nev_print_data_mesh_init (*pMesh, MeshData, &entity, attribute, datastring,
                            &dim, &entityqty, &type, &value, &pData);

  // element data: elt0d, elt1d, elt2d or elt3d
  if (strstr (entity, "elt") && !strstr (entity, "edge"))
    nev_print_data_mesh_elt (Sim, pTess, pNodes, pMesh, entity_in, dim, entityqty,
                       attribute, type, datastring, value, pData);

  // element boundary data
  else if (strstr (entity, "edge"))
    nev_print_data_mesh_eltedge (attribute, datastring, pData);

  // copying elset data to elts
  if (!strncmp (entity_in, "elset", 5) || !strncmp (entity_in, "mesh", 4))
    nev_print_data_mesh_elset2elt ((*pMesh)[dim], entity_in, attribute,
                             MeshData[dim][(!strncmp (entity_in, "elset", 5)) ? 1 : 2],
                             MeshData[dim]);

  ut_free_1d_char (&entity);
  ut_free_1d_char (&type);
  ut_free_1d_char (&value);

  return;
}
