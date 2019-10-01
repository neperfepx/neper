/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_v_.h"

int
neper_v (int fargc, char **fargv, int argc, char **argv)
{
  int i, dim;
  struct PRINT Print;

  // Tessellation
  struct TESS Tess;
  struct TESR Tesr;

  // Mesh
  struct NODES Nodes;
  struct MESH *Mesh = calloc (5, sizeof (MESH));

  // Point
  struct POINT Point;

  // Data
  struct TESSDATA TessData;
  struct TESRDATA TesrData;
  struct NODEDATA NodeData;
  struct MESHDATA *MeshData = calloc (5, sizeof (MESHDATA));
  struct CSYSDATA CsysData;
  struct POINTDATA PointData;

  // Slice: mesh and data
  int SQty = 0;
  struct NODES *SNodes = NULL;
  struct MESH *SMesh = NULL;
  struct NODEDATA *SNodeData = NULL;
  struct MESHDATA **SMeshData = NULL;
  int **SElt2dElt3d = NULL;

  int *expargcs = ut_alloc_1d_int (2);
  char ***expargvs = ut_alloc_1d_ppchar (2);

  ut_arg_expand (fargc, fargv, &(expargcs[0]), &(expargvs[0]));
  ut_arg_expand (argc, argv, &(expargcs[1]), &(expargvs[1]));

  // init general stuff ---
  neut_print_set_default (&Print);
  neut_tess_set_zero (&Tess);
  neut_tesr_set_zero (&Tesr);
  neut_nodes_set_zero (&Nodes);
  neut_point_set_zero (&Point);
  neut_tessdata_set_default (&TessData);
  neut_tesrdata_set_default (&TesrData);
  for (dim = 0; dim <= 4; dim++)
  {
    neut_mesh_set_zero (Mesh + dim);
    neut_meshdata_set_default (MeshData + dim);
  }
  neut_nodedata_set_default (&NodeData);
  neut_csysdata_set_default (&CsysData);
  neut_pointdata_set_default (&PointData);
  // need to default Data and SData here; neut_data_set_default ();

  // Printing module header --------------------------------------------
  ut_print_moduleheader ("-V", fargc, fargv, argc, argv);

  // Checking if povray is present -------------------------------------
  neut_povray_check_error ();

  int type;
  for (type = 0; type < 2; type++)
  {
    int expargc = expargcs[type];
    char **expargv = expargvs[type];

    for (i = 1; i < expargc; i++)
    {
      // tessellation / mesh loading -------------------------------------
      if (expargv[i][0] != '-')
	nev_load (expargv[i], &Tess, &Tesr, &Nodes, Mesh, &Point,
		  &TessData, &TesrData, &NodeData, MeshData, &PointData);

      // data loading and settings ---------------------------------------
      else if (strncmp (expargv[i], "-data", 5) == 0)
	nev_data (expargv, &i, Tess, Tesr, Mesh, &TessData, &TesrData,
		  &NodeData, MeshData, &CsysData, &PointData);

      // show settings ---------------------------------------------------
      else if (strncmp (expargv[i], "-show", 5) == 0)
	nev_show (expargv, &i, Tess, Tesr, Nodes, Mesh, Point, &Print);

      // slicing ---------------------------------------------------------
      else if (strncmp (expargv[i], "-slice", 5) == 0)
	nev_slice (expargv, &i, &Print);

      // camera settings -------------------------------------------------
      else if (strncmp (expargv[i], "-camera", 7) == 0)
	nev_camera (expargv, &i, &Print);

      // image settings --------------------------------------------------
      else if (strncmp (expargv[i], "-image", 6) == 0)
	nev_image (expargv, &i, &Print);

      // include settings ------------------------------------------------
      else if (strncmp (expargv[i], "-include", 8) == 0)
	nev_include (expargv, &i, &Print);

      // povray settings -------------------------------------------------
      else if (strcmp (expargv[i], "-povray") == 0)
	nev_povray (expargv, &i, &Print);

      // image printing --------------------------------------------------
      else if (strcmp (expargv[i], "-print") == 0)
      {
	nev_data_init (Tess, &TessData, Tesr, &TesrData, Nodes, Mesh,
		       Point, &NodeData, MeshData, &CsysData, &PointData);

	if (Print.slice)
	  nev_slice_mesh (Nodes, Mesh[3], NodeData, MeshData, Print.slice,
			  &SQty, &SNodes, &SMesh, &SNodeData,
			  &SMeshData, &SElt2dElt3d);

	int j;
	for (j = 0; j < SQty; j++)
	{

	  nev_nodedata_init (SNodes[j], Mesh[3].ElsetQty, &(SNodeData[j]));
	  nev_meshdata_init (SMesh[j], 1, Mesh[3].ElsetQty, &(SMeshData[j][2]));
	}

	nev_show_init (Tess, Tesr, Nodes, Mesh, SQty, Point, &Print);

	nev_camera_init (Tess, Tesr, Nodes, Mesh, Point, NodeData, &Print);

	nev_print (expargv, &i, Print, Tess, TessData,
		   Tesr, TesrData, Nodes, Mesh,
		   NodeData, MeshData, SQty, SNodes, SMesh, SNodeData,
		   SMeshData, SElt2dElt3d, CsysData, Point, PointData);
      }

      // process image(s) ------------------------------------------------
      else if (strncmp (expargv[i], "-processimage", 13) == 0)
	nev_processimage (expargv, &i);

      else
      {
	ut_print_message (2, 0, "Unknown option %s (i = %d)\n", expargv[i],
			  i);
	abort ();
      }
    }
  }

  neut_print_free (&Print);
  neut_tess_free (&Tess);
  neut_nodes_free (&Nodes);
  for (dim = 0; dim <= 4; dim++)
  {
    neut_mesh_free (Mesh + dim);
    neut_meshdata_free (MeshData + dim);
  }
  neut_tessdata_free (&TessData);
  neut_nodedata_free (&NodeData);

  // ut_free_2d_char (expargv, argc);

  return 0;
}
