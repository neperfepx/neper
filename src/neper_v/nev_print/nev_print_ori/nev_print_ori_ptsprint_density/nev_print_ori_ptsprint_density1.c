/* This file is part of the 'hermes' program. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYINORIG file in the top-level directory. */

#include "nev_print_ori_ptsprint_density_.h"

void
nev_print_ori_ptsprint_density (struct IN_V In, char *layout, char *filename, FILE *file, struct PRINT Print,
                                struct DATA Data, struct ORI *pOri, struct OL_SET *pOSet)
{
  int i, j;
  struct DATA NodeData;
  struct DATA **MeshData;
  struct PRINT OdfPrint;
  struct ODF Odf;
  int *rgb = ut_alloc_1d_int (3);

  neut_odf_set_zero (&Odf);

  // we could copy (neut_ospace_memcpy) or work by pointers(?) instead of reading again
  neut_ospace_fnscanf ((*pOri).Sp.crysym, (*pOri).Sp.space, (*pOri).Sp.fr, &Odf.Sp, "R");

  // Copying to DATAs for printing

  neut_data_set_default (&NodeData);

  MeshData = calloc (5, sizeof (DATA*));
  for (i = 0; i < 5; i++)
  {
    MeshData[i] = calloc (3, sizeof (DATA));
    for (j = 0; j < 3; j++)
      neut_data_set_default (MeshData[i] + j);
  }

  // NodeData
  NodeData.Qty = (*pOri).Sp.Nodes.NodeQty;
  NodeData.Col = ut_alloc_2d_int (NodeData.Qty + 1, 3);
  NodeData.ColData = ut_alloc_2d (NodeData.Qty + 1, 1);
  NodeData.Coo = ut_alloc_2d ((*pOri).Sp.Nodes.NodeQty + 1, 3);

  ut_array_2d_memcpy ((*pOri).Sp.Nodes.NodeCoo + 1, (*pOri).Sp.Nodes.NodeQty, 3,
                      NodeData.Coo + 1);

  ut_string_string ("real", &(NodeData.ColDataType));
  ut_string_string (Data.ColScheme, &(NodeData.ColScheme));

  // MeshData
  for (i = 0; i <= 3; i++)
  {
    MeshData[i][0].Qty = (*pOri).Sp.Mesh[i].EltQty;
    MeshData[i][0].BCol = ut_alloc_1d_int (3);
    MeshData[i][0].Col = ut_alloc_2d_int (MeshData[i][0].Qty + 1, 3);
    MeshData[i][0].ColData = ut_alloc_2d (MeshData[i][0].Qty + 1, 1);
    MeshData[1][0].Rad = ut_alloc_1d (MeshData[1][0].Qty + 1);
  }

  if (Data.Scale)
  {
    ut_string_string (Data.Scale, &NodeData.Scale);
    ut_string_string (Data.Scale, &(MeshData[3][0].Scale));
  }

  if (Data.ScaleTitle)
  {
    ut_string_string (Data.ScaleTitle, &NodeData.ScaleTitle);
    ut_string_string (Data.ScaleTitle, &(MeshData[3][0].ScaleTitle));
  }
  else
  {
    ut_string_string ("ODF [MRD]", &NodeData.ScaleTitle);
    ut_string_string ("ODF [MRD]", &(MeshData[3][0].ScaleTitle));
  }

  // Computing ODF
  if (!(*pOri).field)
  {
    neut_odf_setsigma (&Odf, (char *) "avthetaeq", (*pOSet).size, (*pOSet).crysym);

    neut_odf_comp ("node", (char *) "5", pOSet, &Odf, 0);
    double index;
    neut_odfn_index (&Odf, &index);
    ut_print_message (0, 3, "ODF index = %f\n", index);

    for (i = 1; i <= (*pOri).Sp.Nodes.NodeQty; i++)
      NodeData.ColData[i][0] = Odf.odfn[i - 1];

    neut_data_init_color_real (&NodeData);
    ut_string_string ("from_nodes", &(MeshData[3][0].ColDataType));
  }

  else
  {
    double *tmp = NULL;
    struct SIMRES SimRes;

    neut_simres_set_zero (&SimRes);

    neut_sim_simres (*((*pOri).pSim), "tess", (*pOri).field, &SimRes);

    if (!ut_file_exist (SimRes.file))
      ut_print_exprbug ((*pOri).field);

    if (ut_file_nbwords (SimRes.file) == (*pOri).Sp.Nodes.NodeQty)
    {
      tmp = ut_alloc_1d ((*pOri).Sp.Nodes.NodeQty + 1);
      ut_array_1d_fnscanf (SimRes.file, tmp + 1, (*pOri).Sp.Nodes.NodeQty, "r");

      for (i = 1; i <= (*pOri).Sp.Nodes.NodeQty; i++)
        NodeData.ColData[i][0] = tmp[i];

      neut_data_init_color_real (&NodeData);
      ut_string_string ("from_nodes", &(MeshData[3][0].ColDataType));
    }

    else if (ut_file_nbwords (SimRes.file) == (*pOri).Sp.Mesh[3].EltQty)
    {
      tmp = ut_alloc_1d ((*pOri).Sp.Mesh[3].EltQty + 1);
      ut_array_1d_fnscanf (SimRes.file, tmp + 1, (*pOri).Sp.Mesh[3].EltQty, "r");

      for (i = 1; i <= (*pOri).Sp.Mesh[3].EltQty; i++)
        MeshData[3][0].ColData[i][0] = tmp[i];

      neut_data_init_color_real (MeshData[3]);
    }

    neut_simres_free (&SimRes);
    ut_free_1d (&tmp);
  }

  // OdfPrint
  neut_print_set_default (&OdfPrint);

  OdfPrint.lightsourceqty = Print.lightsourceqty;
  OdfPrint.lightsources = ut_alloc_1d_pchar (Print.lightsourceqty);
  for (i = 0; i < Print.lightsourceqty; i++)
    ut_string_string (Print.lightsources[i], OdfPrint.lightsources + i);
  OdfPrint.lightambient = Print.lightambient;
  OdfPrint.lightdiffuse = Print.lightdiffuse;
  OdfPrint.lightreflection = Print.lightreflection;

  neut_mesh_init_nodeelts ((*pOri).Sp.Mesh + 2, (*pOri).Sp.Nodes.NodeQty);

  OdfPrint.shownode = ut_alloc_1d_int ((*pOri).Sp.Nodes.NodeQty + 1);
  OdfPrint.showelt0d = ut_alloc_1d_int ((*pOri).Sp.Mesh[0].EltQty + 1);
  OdfPrint.showelt1d = ut_alloc_1d_int ((*pOri).Sp.Mesh[1].EltQty + 1);
  OdfPrint.showelt2d = ut_alloc_1d_int ((*pOri).Sp.Mesh[2].EltQty + 1);
  OdfPrint.showelt3d = ut_alloc_1d_int ((*pOri).Sp.Mesh[3].EltQty + 1);
  ut_array_1d_int_set (OdfPrint.showelt1d + 1, (*pOri).Sp.Mesh[1].EltQty, 1);
  ut_array_1d_int_set (OdfPrint.showelt3d + 1, (*pOri).Sp.Mesh[3].EltQty, 1);
  // OdfPrint.showsliceelt1d = 0;

  if (!strcmp (layout, "surface"))
  {
    // Printing mesh
    nev_print_real_png_mesh (file, OdfPrint, (*pOri).Sp.Tess, (*pOri).Sp.Nodes,
                             (*pOri).Sp.Mesh, NodeData, MeshData);
  }

  else if (!strcmp (layout, "slices"))
  {
    int SQty;
    struct NODES *SNodes = NULL;
    struct MESH **SMesh = NULL;
    struct DATA *SNodeData = NULL;
    struct DATA ***SMeshData = NULL;
    int **SElt2dElt3d = NULL;

    ut_array_1d_int_set (OdfPrint.showelt1d + 1, (*pOri).Sp.Mesh[1].EltQty, 1);
    ut_array_1d_int_set (OdfPrint.showelt3d + 1, (*pOri).Sp.Mesh[3].EltQty, 1);

    neut_mesh_slice ((*pOri).Sp.Nodes, (*pOri).Sp.Mesh[3], NodeData,
                     MeshData[3][0], (char *) "x=0,y=0,z=0", &SQty, &SNodes, &SMesh,
                     &SNodeData, &SMeshData, &SElt2dElt3d);

    for (i = 0; i < SQty; i++)
    {
      ut_string_string ("real", &(SNodeData[i].ColDataType));
      neut_data_init_color_real (SNodeData + i);
    }

    // Printing mesh
    nev_print_real_png_meshslices (file, OdfPrint, MeshData, SQty, SNodes, SMesh,
                                   SNodeData, SMeshData, SElt2dElt3d);

    for (i = 0; i < SQty; i++)
    {
      neut_nodes_free (SNodes + i);
      for (j = 0; j < 3; j++)
        neut_mesh_free (SMesh[i] + j);
      neut_data_free (SNodeData + i);
      for (j = 0; j < 3; j++)
        neut_data_free (SMeshData[i][j]);
      free (SMeshData[i]);
    }
    free (SMesh);
    free (SNodes);
    free (SNodeData);
    free (SMeshData);
    ut_free_2d_int (&SElt2dElt3d, SQty);
  }

  else
  {
    printf ("layout = %s\n", layout);
    abort ();
  }

  // Printing scale
  int imageheight;
  char *basename = NULL;
  char *filename2 = ut_alloc_1d_char (1000);

  neut_print_imagesize (In.imagesize, NULL, &imageheight);

  ut_string_body (filename, ".", &basename);
  sprintf (filename2, "%s.scale.pov", basename);

  FILE *file2 = ut_file_open (filename2, "W");
  nev_print_png_scale (In, file2, Print, NodeData);
  ut_file_close (file2, filename2, "W");

  neut_data_free (&NodeData);
  for (i = 0; i < 5; i++)
  {
    for (j = 0; j < 3; j++)
      neut_data_free (MeshData[i] + j);
    free (MeshData[i]);
  }
  free (MeshData);

  ut_free_1d_char (&basename);
  ut_free_1d_char (&filename2);
  neut_odf_free (&Odf);
  neut_print_free (&OdfPrint);
  ut_free_1d_int (&rgb);

  return;
}
