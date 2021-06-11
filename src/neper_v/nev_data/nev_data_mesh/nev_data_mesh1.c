/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_mesh_.h"

void
nev_data_mesh (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes,
                      struct MESH **pMesh, char *entity_in, char *attribute,
                      char *datastring, struct DATA *MeshData)
{
  int dim, entityqty;
  struct DATA MeshDataSet, *pData = NULL;
  char *entity = NULL, *type = NULL, *value = NULL;;

  neut_data_set_default (&MeshDataSet);

  ut_string_string (entity_in, &entity);

  pData = nev_data_mesh_init (*pMesh, MeshData, &entity, attribute,
                              datastring, &dim, &entityqty, &type,
                              &value, &MeshDataSet);

  // element data: elt0d, elt1d, elt2d or elt3d
  if (strlen (entity) == 5)
    nev_data_mesh_elt (Sim, pTess, pNodes, pMesh, entity_in, dim, entityqty,
                              attribute, type, value, pData);

  // element boundary data
  else
    nev_data_mesh_eltedge (attribute, datastring, pData);

  // copying elset data to elts
  if (!strncmp (entity_in, "elset", 5))
    nev_data_mesh_elset2elt ((*pMesh)[dim], entity_in, attribute, MeshDataSet,
                             MeshData + dim);

  neut_data_free (&MeshDataSet);
  ut_free_1d_char (&entity);
  ut_free_1d_char (&type);
  ut_free_1d_char (&value);

  return;
}
