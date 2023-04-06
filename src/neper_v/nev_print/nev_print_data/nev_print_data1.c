/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_.h"

void
nev_print_data (struct IN_V In, struct SIM Sim, struct TESS *pTess,
                struct TESR *pTesr, struct NODES *pNodes, struct MESH **pMesh,
                struct POINT **pPoints, int PointQty, struct DATA *TessData,
                struct DATA *TesrData, struct DATA *pNodeData,
                struct DATA **MeshData, struct DATA *pCsysData,
                struct DATA **pPointData)
{
  int i, j;
  char *entity = ut_alloc_1d_char (100);
  char *nameedge = NULL;
  char *attribute = ut_alloc_1d_char (100);
  char **PointNames = ut_alloc_1d_pchar (PointQty);

  for (j = 0; j < PointQty; j++)
    ut_string_string ((*pPoints)[j].Name, PointNames + j);

  for (i = 0; i < In.dataqty; i++)
  {
    neut_data_string_entity_attribute (In.data[i][0], entity, PointNames, PointQty, attribute);

    if ((!strcmp (entity, "poly") || !strcmp (entity, "edge")
      || !strcmp (entity, "face") || !strcmp (entity, "ver")
      || !strcmp (entity, "cell") || !strcmp (entity, "seed")
      || !strncmp (entity, "crystal", 7) || !strncmp (entity, "celledge", 8))
      && !neut_tess_isvoid (*pTess))
      nev_print_data_tess (Sim, pTess, entity, attribute, In.data[i][1], TessData);

    else if ((!strcmp (entity, "cell") || !strncmp (entity, "vox", 3)
           || !strcmp (entity, "edge") || !strncmp (entity, "voidvox", 7))
           && !neut_tesr_isvoid (*pTesr))
      nev_print_data_tesr (Sim, pTesr, entity, attribute, In.data[i][1], TesrData);

    else if (!strncmp (entity, "node", 4))
      nev_print_data_nodes (Sim, pTess, pNodes, pMesh, attribute, In.data[i][1], pNodeData);

    else if (!strncmp (entity, "elt", 3) || !strncmp (entity, "elset", 5)
          || !strncmp (entity, "mesh", 4))
      nev_print_data_mesh (Sim, pTess, pNodes, pMesh, entity, attribute,
                           In.data[i][1], MeshData);

    else if (!strcmp (entity, "csys"))
      nev_print_data_csys (Sim, attribute, In.data[i][1], pCsysData);

    else
    {
      int status = -1;

      for (j = 0; j < PointQty; j++)
      {
        nameedge = ut_string_paste ((*pPoints)[j].Name, "edge");

        if (strlen ((*pPoints)[j].Name) == 0
         || !strcmp (entity, (*pPoints)[j].Name) || !strcmp (entity, nameedge))
        {
          nev_print_data_points (Sim, (*pPoints) + j, pTess, pNodes, pMesh,
                                 entity, attribute, In.data[i][1], (*pPointData) + j);
          status = 0;
          break;
        }

        ut_free_1d_char (&nameedge);
      }

      if (status)
        ut_print_message (1, 0, "Unknown entity `%s'.  Skipping...\n", entity);
    }
  }

  ut_free_1d_char (&entity);
  ut_free_1d_char (&attribute);
  ut_free_2d_char (&PointNames, PointQty);

  return;
}
