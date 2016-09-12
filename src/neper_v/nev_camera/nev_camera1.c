/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_camera_.h"

void
nev_camera (char **argv, int *pi, struct PRINT *pPrint)
{
  if (!strcmp (argv[*pi], "-cameracoo"))
  {
    (*pi)++;
    (*pPrint).cameracoostring
      = ut_realloc_1d_char ((*pPrint).cameracoostring,
			    strlen (argv[*pi]) + 1);
    strcpy ((*pPrint).cameracoostring, argv[*pi]);
  }

  else if (!strcmp (argv[*pi], "-cameralookat"))
  {
    (*pi)++;
    (*pPrint).cameralookatstring
      = ut_realloc_1d_char ((*pPrint).cameralookatstring,
			    strlen (argv[*pi]) + 1);
    strcpy ((*pPrint).cameralookatstring, argv[*pi]);
  }

  else if (!strcmp (argv[*pi], "-camerasky"))
  {
    (*pi)++;
    (*pPrint).cameraskystring
      = ut_realloc_1d_char ((*pPrint).cameraskystring,
			    strlen (argv[*pi]) + 1);
    strcpy ((*pPrint).cameraskystring, argv[*pi]);
  }

  else if (strcmp (argv[*pi], "-cameraangle") == 0)
    ut_string_real (argv[++(*pi)], &((*pPrint).cameraangle));

  else if (strcmp (argv[*pi], "-cameraprojection") == 0)
    sprintf ((*pPrint).cameraprojection, "%s", argv[++(*pi)]);

  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}

void
nev_camera_init (struct TESS Tess, struct TESR Tesr,
		 struct NODES Nodes, struct MESH *Mesh,
		 struct POINT Point,
		 struct NODEDATA NodeData, struct PRINT *pPrint)
{
  int dim = -1;
  struct NODES Nodes2;

  neut_nodes_set_zero (&Nodes2);

  if (NodeData.CooData != NULL)
  {
    neut_nodes_memcpy (Nodes, &Nodes2);

    if (!strcmp (NodeData.CooDataType, "coo"))
      ut_array_2d_memcpy (Nodes2.NodeCoo + 1, Nodes.NodeQty, 3,
			  NodeData.CooData + 1);
    else if (!strcmp (NodeData.CooDataType, "disp"))
      ut_array_2d_add (Nodes2.NodeCoo + 1,
		       NodeData.CooData + 1,
		       Nodes.NodeQty, 3, Nodes2.NodeCoo + 1);
    else
      ut_error_reportbug ();
  }
  else
    Nodes2 = Nodes;

  if (Tess.VerQty > 0 && Nodes.NodeQty == 0)
  {
    dim = Tess.Dim;

    nev_camera_tess_expr_coo (Tess, (*pPrint).cameracoostring,
			      (*pPrint).cameracoo);
    nev_camera_tess_expr_coo (Tess, (*pPrint).cameralookatstring,
			      (*pPrint).cameralookat);
  }
  else if (Nodes.NodeQty > 0)
  {
    dim = neut_nodes_dim (Nodes);
    if (dim == -1)
      ut_error_reportbug ();

    nev_camera_mesh_expr_coo (Nodes2, Mesh[dim], (*pPrint).cameracoostring,
			      (*pPrint).cameracoo);
    nev_camera_mesh_expr_coo (Nodes2, Mesh[dim], (*pPrint).cameralookatstring,
			      (*pPrint).cameralookat);
  }
  else if (Tesr.CellQty > 0)
  {
    dim = Tesr.Dim;

    nev_camera_tesr_expr_coo (Tesr, (*pPrint).cameracoostring,
			      (*pPrint).cameracoo);
    nev_camera_tesr_expr_coo (Tesr, (*pPrint).cameralookatstring,
			      (*pPrint).cameralookat);
  }
  else if (Point.PointQty > 0)
  {
    dim = Tesr.Dim;

    nev_camera_point_expr_coo (Point, (*pPrint).cameracoostring,
			       (*pPrint).cameracoo);
    nev_camera_point_expr_coo (Point, (*pPrint).cameralookatstring,
			       (*pPrint).cameralookat);
  }

  nev_camera_expr_sky ((*pPrint).cameraskystring, (*pPrint).camerasky);

  if (!strcmp ((*pPrint).cameraprojection, "default"))
  {
    strcpy ((*pPrint).cameraprojection, "perspective");
    if (dim <= 2)
      strcpy ((*pPrint).cameraprojection, "orthographic");
  }

  if (NodeData.CooData != NULL)
    neut_nodes_free (&Nodes2);

  return;
}
