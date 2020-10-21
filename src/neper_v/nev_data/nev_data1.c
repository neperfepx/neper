/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_.h"

void
nev_data (char **argv, int *pi, struct SIM Sim, struct TESS *pTess,
          struct TESR *pTesr, struct NODES *pNodes,
          struct MESH **pMesh, struct POINT *pPoint,
          struct DATA *TessData,
          struct DATA *pTesrData, struct DATA *pNodeData,
          struct DATA *MeshData, struct DATA *pCsysData,
          struct DATA *pPointData)
{
  int dim = neut_mesh_array_dim (*pMesh);
  char *entity = ut_alloc_1d_char (100);
  char *attribute = ut_alloc_1d_char (100);

  neut_data_string_entity_attribute (argv[(*pi)], entity, attribute);

  if (!strcmp (entity, "cell") && !neut_tess_isvoid (*pTess))
    neut_tess_cell (*pTess, &entity);
  else if (!strcmp (entity, "elt"))
    sprintf (entity, "elt%dd", dim);
  else if (!strcmp (entity, "elset"))
    sprintf (entity, "elset%dd", dim);

  if ((!strcmp (entity, "poly") || !strcmp (entity, "edge")
    || !strcmp (entity, "face") || !strcmp (entity, "ver")
    || !strcmp (entity, "seed")) && !neut_tess_isvoid (*pTess))
      nev_data_tess (Sim, pTess, entity, attribute, argv[++(*pi)], TessData);

  else if ((!strcmp (entity, "cell") || !strncmp (entity, "vox", 3)
         || !strcmp (entity, "edge") || !strncmp (entity, "voidvox", 7))
         && !neut_tesr_isvoid (*pTesr))
    nev_data_tesr (Sim, pTesr, entity, attribute, argv[++(*pi)], pTesrData);

  else if (!strcmp (entity, "node"))
    nev_data_nodes (Sim, pTess, pNodes, pMesh, attribute, argv[++(*pi)], pNodeData);

  else if (!strncmp (entity, "elt", 3) || !strncmp (entity, "elset", 5))
    nev_data_mesh (Sim, pTess, pNodes, pMesh, entity, attribute,
                          argv[++(*pi)], MeshData);

  else if (!strcmp (entity, "csys"))
    nev_data_csys (Sim, attribute, argv[++(*pi)], pCsysData);

  else if (!strncmp (entity, "point", 5))
    nev_data_points (Sim, pPoint, pTess, pNodes, pMesh, entity, attribute, argv[++(*pi)], pPointData);

  else
  {
    ut_print_message (1, 0, "Unknown entity `%s'.  Skipping...\n", entity);
    ++(*pi);
  }

  ut_free_1d_char (&entity);
  ut_free_1d_char (&attribute);

  return;
}
