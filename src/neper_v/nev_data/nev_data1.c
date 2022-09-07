/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_.h"

void
nev_data (char **argv, int *pi, struct SIM Sim, struct TESS *pTess,
          struct TESR *pTesr, struct NODES *pNodes,
          struct MESH **pMesh, struct POINT **pPoints, int PointQty,
          struct DATA *TessData,
          struct DATA *TesrData, struct DATA *pNodeData,
          struct DATA *MeshData, struct DATA *pCsysData,
          struct DATA **pPointData)
{
  int i;
  char *entity = ut_alloc_1d_char (100);
  char *nameedge = NULL;
  char *attribute = ut_alloc_1d_char (100);
  char **PointNames = ut_alloc_1d_pchar (PointQty);

  for (i = 0; i < PointQty; i++)
    ut_string_string ((*pPoints)[i].Name, PointNames + i);

  neut_data_string_entity_attribute (argv[(*pi)], entity, PointNames, PointQty, attribute);

  if ((!strcmp (entity, "poly") || !strcmp (entity, "edge")
    || !strcmp (entity, "face") || !strcmp (entity, "ver")
    || !strcmp (entity, "cell") || !strcmp (entity, "seed")
    || !strncmp (entity, "crystal", 7) || !strncmp (entity, "celledge", 8))
    && !neut_tess_isvoid (*pTess))
      nev_data_tess (Sim, pTess, entity, attribute, argv[++(*pi)], TessData);

  else if ((!strcmp (entity, "cell") || !strncmp (entity, "vox", 3)
         || !strcmp (entity, "edge") || !strncmp (entity, "voidvox", 7))
         && !neut_tesr_isvoid (*pTesr))
    nev_data_tesr (Sim, pTesr, entity, attribute, argv[++(*pi)], TesrData);

  else if (!strncmp (entity, "node", 4))
    nev_data_nodes (Sim, pTess, pNodes, pMesh, attribute, argv[++(*pi)], pNodeData);

  else if (!strncmp (entity, "elt", 3) || !strncmp (entity, "elset", 5)
        || !strncmp (entity, "mesh", 4))
    nev_data_mesh (Sim, pTess, pNodes, pMesh, entity, attribute, argv[++(*pi)],
                   MeshData);

  else if (!strcmp (entity, "csys"))
    nev_data_csys (Sim, attribute, argv[++(*pi)], pCsysData);

  else
  {
    int status = -1;

    for (i = 0; i < PointQty; i++)
    {
      nameedge = ut_string_paste ((*pPoints)[i].Name, "edge");

      if (strlen ((*pPoints)[i].Name) == 0
       || !strcmp (entity, (*pPoints)[i].Name) || !strcmp (entity, nameedge))
      {
        nev_data_points (Sim, (*pPoints) + i, pTess, pNodes, pMesh, entity, attribute,
                         argv[(*pi) + 1], (*pPointData) + i);
        status = 0;
        break;
      }

      ut_free_1d_char (&nameedge);
    }

    if (status)
      ut_print_message (1, 0, "Unknown entity `%s'.  Skipping...\n", entity);

    (*pi)++;
  }

  ut_free_1d_char (&entity);
  ut_free_1d_char (&attribute);
  ut_free_2d_char (&PointNames, PointQty);

  return;
}
