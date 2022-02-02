/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_scaling_.h"

void
nem_scaling_para (char *elttype, struct TESS *pTess, struct TESR *pTesr,
                  struct NODES *pRNodes, struct MESH *RMesh,
                  struct MESHPARA *pMeshPara)
{
  int id, polyqty;
  double *scale = NULL;
  double size;
  char input;
  int dim;

  // Testing type of input: tess or tesr or mesh
  if ((*pTess).VerQty > 0)
  {
    input = 't';
    dim = (*pTess).Dim;
    if ((*pTess).Dim == 3)
      polyqty = (*pTess).PolyQty;
    else if ((*pTess).Dim == 2)
      polyqty = (*pTess).FaceQty;
    else
      abort ();
  }
  else if ((*pTesr).PolyQty > 0)
  {
    input = 'v';
    dim = 3;
    polyqty = (*pTesr).PolyQty;
  }
  else if (RMesh[3].ElsetQty > 0)
  {
    input = 'm';
    dim = 3;
    polyqty = RMesh[3].ElsetQty;
  }
  else
    return;

  polyqty = polyqty;
  dim = dim;

  // If cltype is rcl, data must be 3D
  if (!strcmp ((*pMeshPara).cltype, "rel"))
    if (((input == 't' || input == 'm') && (*pTess).VerQty == 0)
        || ((input == 'v') && (*pTesr).PolyQty == 0))
    {
      ut_print_message (2, 0,
                        "Option -rcl cannot be used since input data is not 3D.\n");
      ut_print_message (2, 0, "Use -cl instead.\n");
      abort ();
    }

  if (!strcmp ((*pMeshPara).cltype, "rel"))
  {
    if (input == 't')
    {
      if ((*pTess).Dim == 3)
        neut_tess_volume (*pTess, &size);
      else if ((*pTess).Dim == 2)
      {
        neut_tess_domface_label_id (*pTess, "z0", &id);
        neut_tess_domface_area (*pTess, id, &size);
      }
    }
    else if (input == 'v')
      neut_tesr_volume (*pTesr, &size);
    else if (input == 'm')
      neut_mesh_volume (*pRNodes, RMesh[3], &size);
    else
      abort ();
  }

  /* Calculation of cl */
  char **cllist = NULL;
  int clqty;
  char **list = NULL;
  int qty;

  // Looking at the master cl (first one)
  ut_list_break ((*pMeshPara).clstring, NEUT_SEP_NODEP, &cllist, &clqty);
  ut_list_break (cllist[0], NEUT_SEP_DEP, &list, &qty);

  if (qty == 1)
  {
    sscanf (list[0], "%lf", &((*pMeshPara).cl[0]));

    if (!strcmp ((*pMeshPara).cltype, "rel"))
      rcl2cl ((*pMeshPara).cl[0], dim, size, polyqty, elttype,
              &((*pMeshPara).cl[0]));
  }

  else if (qty == 3)
  {
    double *val = ut_alloc_1d (3);

    for (i = 0; i < 3; i++)
    {
      sscanf (list[i], "%lf", &(val[i]));

      if (!strcmp ((*pMeshPara).cltype, "rel"))
        rcl2cl (val[i], dim, size, polyqty, elttype, &(val[i]));
    }

    (*pMeshPara).cl[0] = ut_array_1d_gmean (val, 3);

    (*pMeshPara).scale = ut_alloc_1d (3);
    for (i = 0; i < 3; i++)
      (*pMeshPara).scale[i] = (*pMeshPara).cl[0] / val[i];

    ut_free_1d (&val);

    if (input == 't')
      neut_tess_scale (pTess, (*pMeshPara).scale[0], (*pMeshPara).scale[1],
                       (*pMeshPara).scale[2]);

    else if (input == 'v')
      neut_tesr_scale (pTesr, (*pMeshPara).scale[0], (*pMeshPara).scale[1],
                       (*pMeshPara).scale[2]);
    else if (input == 'm')
      neut_nodes_scale (pRNodes, (*pMeshPara).scale[0], (*pMeshPara).scale[1],
                        (*pMeshPara).scale[2]);
    else
      abort ();

    if (input == 'm')
      nem_tess_updatefrommesh_geom (pTess, *pRNodes, RMesh[0], RMesh[1],
                                    RMesh[2], RMesh[3]);
  }
  else
    abort ();

  ut_free_1d_char (&list);

  (*pMeshPara).clpoly = ut_alloc_1d_pchar (clqty);

  for (i = 1; i < clqty; i++)
  {
    // Processing the master cl (first one)
    ut_list_break (cllist[i], NEUT_SEP_DEP, &list, &qty);

    if (qty == 2)
    {
      (*pMeshPara).clpoly[i] = ut_alloc_1d_char (strlen (list[0]) + 1);
      strcpy ((*pMeshPara).clpoly[i], list[0]);
      sscanf (list[1], "%lf", &((*pMeshPara).cl[i]));

      if (!strcmp ((*pMeshPara).cltype, "rel"))
        rcl2cl ((*pMeshPara).cl[i], dim, size, polyqty, elttype,
                &((*pMeshPara).cl[i]));
    }
    else
      abort ();

    ut_free_1d_char (&list);
  }

  ut_free_1d (&scale);

  return;
}

void
nem_scaling_post (struct MESHPARA MeshPara, struct TESS *pTess,
                  struct TESR *pTesr, struct NODES *pNodes)
{
  if (MeshPara.scale)
  {
    if ((*pTess).PolyQty > 0)
      neut_tess_scale (pTess, 1 / MeshPara.scale[0], 1 / MeshPara.scale[1],
                       1 / MeshPara.scale[2]);
    if ((*pTesr).PolyQty > 0)
      neut_tesr_scale (pTesr, 1 / MeshPara.scale[0], 1 / MeshPara.scale[1],
                       1 / MeshPara.scale[2]);

    neut_nodes_scale (pNodes, 1 / MeshPara.scale[0], 1 / MeshPara.scale[1],
                      1 / MeshPara.scale[2]);
  }

  return;
}
