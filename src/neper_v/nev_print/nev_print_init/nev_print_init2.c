/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_init_.h"

void
nev_print_init_data (struct PRINT Print,
               struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
               struct DATA *TesrData, struct NODES Nodes,
               struct MESH *Mesh, struct POINT *Points, int PointQty,
               struct DATA *pData, struct DATA *MeshData,
               struct DATA *pCsysData, struct DATA *PointData)
{
  int i, meshdim;
  double size;

  /*
  if (Tess.Dim == 0 && Tess.CellQty < 1)
    Tess.CellQty = 1;
  */

  if (Print.showtess && !neut_tess_isvoid (Tess))
    for (i = 0; i <= 5; i++)
      nev_print_init_data_tess (Print, Tess, TessData + i);

  if (Print.showtesr && !neut_tesr_isvoid (Tesr))
    for (i = 0; i <= 1; i++)
      nev_print_init_data_tesr (Print, Tesr, TesrData + i);

  if ((Print.shownode || Print.showmesh || Print.showslice) && !neut_nodes_isvoid (Nodes))
    nev_print_init_data_nodes (Print, Nodes, Nodes.NodeQty, pData);

  meshdim = neut_mesh_array_dim (Mesh);
  if ((Print.showmesh || Print.showslice) && meshdim > 0)
  {
    neut_mesh_size (Nodes, Mesh[meshdim], &size);

    for (i = 0; i <= meshdim; i++)
      if (!neut_mesh_isvoid (Mesh[i]))
        nev_print_init_data_mesh (Print, Mesh[i], size, Mesh[i].ElsetQty, meshdim, MeshData + i);
  }

  if (Print.showcsys)
    nev_print_init_data_csys (Print, pCsysData);

  if (PointQty != Print.inputqty)
    abort ();

  for (i = 0; i < Print.inputqty; i++)
    if (!neut_point_isvoid (Points[i]))
      nev_print_init_data_points (Print, Points[i], PointData + i);

  return;
}

void
nev_print_init_show (struct TESS Tess, struct TESR Tesr, struct NODES Nodes,
               struct MESH *Mesh, int SQty, struct POINT *Points, int PointQty,
               struct PRINT *pPrint)
{
  int dim = neut_mesh_array_dim (Mesh);
  int i, j, k, cellqty;
  int *cells = NULL;

  if (SQty > 0)
  {
    if ((*pPrint).showslice == -1)
      (*pPrint).showslice = 1;
    if ((*pPrint).showmesh == -1)
      (*pPrint).showmesh = 0;
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showtesr == -1)
      (*pPrint).showtesr = 0;
  }

  if (dim >= 0 && (*pPrint).showmesh == -1)
  {
    (*pPrint).showmesh = 1;
    if ((*pPrint).showtess == -1)
      (*pPrint).showtess = 0;
    if ((*pPrint).showtesr == -1)
      (*pPrint).showtesr = 0;
  }

  if (!neut_tess_isvoid (Tess) && (*pPrint).showtess == -1)
    (*pPrint).showtess = 1;

  if (!neut_tesr_isvoid (Tesr) && (*pPrint).showtesr == -1)
    (*pPrint).showtesr = 1;

  if (PointQty > 0)
  {
    if (!(*pPrint).showpoint)
      (*pPrint).showpoint = ut_alloc_1d_pint (PointQty);

    for (i = 0; i < PointQty; i++)
      if (!(*pPrint).showpoint[i])
      {
        (*pPrint).showpoint[i] = ut_alloc_1d_int (Points[i].Qty + 1);
        ut_array_1d_int_set ((*pPrint).showpoint[i] + 1, Points[i].Qty, 1);
      }
  }

  if ((*pPrint).showmesh == 1)
  {
    if ((*pPrint).showelt3d[0] == -1)
    {
      (*pPrint).showelt3d =
        ut_realloc_1d_int ((*pPrint).showelt3d, Mesh[3].EltQty + 1);

      (*pPrint).showelt3d[0] = (dim == 3) ? Mesh[3].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt3d + 1, Mesh[3].EltQty, dim == 3);
    }

    if ((*pPrint).showelt2d[0] == -1)
    {
      (*pPrint).showelt2d =
        ut_realloc_1d_int ((*pPrint).showelt2d, Mesh[2].EltQty + 1);

      (*pPrint).showelt2d[0] = (dim == 2) ? Mesh[2].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt2d + 1, Mesh[2].EltQty, dim == 2);
    }

    if ((*pPrint).showelt1d[0] == -1)
    {
      (*pPrint).showelt1d =
        ut_realloc_1d_int ((*pPrint).showelt1d, Mesh[1].EltQty + 1);

      (*pPrint).showelt1d[0] = (dim == 1) ? Mesh[1].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt1d + 1, Mesh[1].EltQty, dim == 1);
    }

    if ((*pPrint).showelt0d[0] == -1)
    {
      (*pPrint).showelt0d =
        ut_realloc_1d_int ((*pPrint).showelt0d, Mesh[0].EltQty + 1);

      (*pPrint).showelt0d[0] = (dim == 0) ? Mesh[0].EltQty : 0;
      ut_array_1d_int_set ((*pPrint).showelt0d + 1, Mesh[0].EltQty, dim == 0);
    }

    if ((*pPrint).shownode[0] == -1)
    {
      (*pPrint).shownode =
        ut_realloc_1d_int ((*pPrint).shownode, Nodes.NodeQty + 1);

      (*pPrint).shownode[0] = 0;
      ut_array_1d_int_set ((*pPrint).shownode + 1, Nodes.NodeQty, 0);
    }
  }

  if ((*pPrint).showtess == 1)
  {
    if (Tess.Dim == 3 && (*pPrint).showpoly[0] == -1)
    {
      (*pPrint).showpoly =
        ut_realloc_1d_int ((*pPrint).showpoly, Tess.PolyQty + 1);
      (*pPrint).showpoly[0] = Tess.PolyQty;
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Tess.PolyQty, 1);
    }

    if ((*pPrint).showface[0] == -1)
    {
      (*pPrint).showface =
        ut_realloc_1d_int ((*pPrint).showface, Tess.FaceQty + 1);
      if (Tess.Dim == 2)
      {
        (*pPrint).showface[0] = Tess.FaceQty;
        ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 1);
      }
      else
      {
        (*pPrint).showface[0] = 0;
        ut_array_1d_int_set ((*pPrint).showface + 1, Tess.FaceQty, 0);
      }
    }

    if ((*pPrint).showedge[0] == -1)
    {
      (*pPrint).showedge =
        ut_realloc_1d_int ((*pPrint).showedge, Tess.EdgeQty + 1);
      ut_array_1d_int_set ((*pPrint).showedge, Tess.EdgeQty + 1, 0);

      for (i = 1; i <= Tess.EdgeQty; i++)
      {
        neut_tess_edge_cells (Tess, i, &cells, &cellqty);
        for (j = 0; j < cellqty; j++)
          if ((Tess.Dim == 3 && (*pPrint).showpoly[cells[j]] == 1)
              || (Tess.Dim == 2 && (*pPrint).showface[cells[j]] == 1))
          {
            (*pPrint).showedge[i] = 1;
            break;
          }
        ut_free_1d_int (&cells);
      }
    }

    if ((*pPrint).showver[0] == -1)
    {
      (*pPrint).showver =
        ut_realloc_1d_int ((*pPrint).showver, Tess.VerQty + 1);
      ut_array_1d_int_set ((*pPrint).showver, Tess.VerQty + 1, 0);
    }

    if ((*pPrint).showseed[0] == -1)
    {
      (*pPrint).showseed =
        ut_realloc_1d_int ((*pPrint).showseed, Tess.SeedQty + 1);
      ut_array_1d_int_set ((*pPrint).showseed, Tess.SeedQty + 1, 0);
    }

    if ((*pPrint).showcrystal[0] == -1)
    {
      (*pPrint).showcrystal =
        ut_realloc_1d_int ((*pPrint).showcrystal, Tess.CellQty + 1);
      ut_array_1d_int_set ((*pPrint).showcrystal, Tess.CellQty + 1, 0);
    }
  }

  else if ((*pPrint).showtesr == 1)
  {
    if (Tesr.Dim == 3 && (*pPrint).showpoly[0] == -1)
    {
      (*pPrint).showpoly =
        ut_realloc_1d_int ((*pPrint).showpoly, Tesr.CellQty + 1);
      (*pPrint).showpoly[0] = Tesr.CellQty;
      ut_array_1d_int_set ((*pPrint).showpoly + 1, Tesr.CellQty, 1);
    }
    else if (Tesr.Dim == 2 && (*pPrint).showface[0] == -1)
    {
      (*pPrint).showface =
        ut_realloc_1d_int ((*pPrint).showface, Tesr.CellQty + 1);
      (*pPrint).showface[0] = Tesr.CellQty;
      ut_array_1d_int_set ((*pPrint).showface + 1, Tesr.CellQty, 1);
    }

    if (!(*pPrint).showvox)
    {
      (*pPrint).showvox = ut_alloc_3d_int (Tesr.size[0] + 1, Tesr.size[1] + 1, Tesr.size[2] + 1);
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
            if (!Tesr.VoxCell || Tesr.VoxCell[i][j][k])
              (*pPrint).showvox[i][j][k] = 1;
    }
  }

  return;
}

void
nev_print_init_camera (struct TESS Tess, struct TESR Tesr, struct NODES Nodes,
                 struct MESH *Mesh, struct POINT *Points, int PointQty,
                 struct DATA NodeData, struct PRINT *pPrint)
{
  int dim = -1;
  struct NODES Nodes2;

  neut_nodes_set_zero (&Nodes2);

  if (NodeData.CooData)
  {
    neut_nodes_memcpy (Nodes, &Nodes2);

    if (!strcmp (NodeData.CooDataType, "coo"))
      ut_array_2d_memcpy (NodeData.CooData + 1, Nodes.NodeQty, 3,
                          Nodes2.NodeCoo + 1);
    else if (!strcmp (NodeData.CooDataType, "disp"))
      ut_array_2d_add (Nodes2.NodeCoo + 1, NodeData.CooData + 1,
                       Nodes.NodeQty, 3, Nodes2.NodeCoo + 1);
    else
      ut_print_neperbug ();
  }
  else
    Nodes2 = Nodes;

  if (!neut_tess_isvoid (Tess) && neut_nodes_isvoid (Nodes))
  {
    dim = Tess.Dim;

    nev_print_init_camera_coo_tess (Tess, (*pPrint).cameracoostring,
                              (*pPrint).cameracoo);
    nev_print_init_camera_coo_tess (Tess, (*pPrint).cameralookatstring,
                              (*pPrint).cameralookat);
  }

  else if (!neut_nodes_isvoid (Nodes))
  {
    dim = neut_nodes_dim (Nodes);
    if (dim == -1)
      ut_print_neperbug ();

    nev_print_init_camera_coo_mesh (Nodes2, Mesh[dim], (*pPrint).cameracoostring,
                              (*pPrint).cameracoo);
    nev_print_init_camera_coo_mesh (Nodes2, Mesh[dim], (*pPrint).cameralookatstring,
                              (*pPrint).cameralookat);
  }

  else if (!neut_tesr_isvoid (Tesr))
  {
    dim = Tesr.Dim;

    nev_print_init_camera_coo_tesr (Tesr, (*pPrint).cameracoostring,
                              (*pPrint).cameracoo);
    nev_print_init_camera_coo_tesr (Tesr, (*pPrint).cameralookatstring,
                              (*pPrint).cameralookat);
  }

  else if (PointQty > 0)
  {
    dim = Points[0].Dim;

    nev_print_init_camera_coo_points (Points[0], (*pPrint).cameracoostring,
                               (*pPrint).cameracoo);
    nev_print_init_camera_coo_points (Points[0], (*pPrint).cameralookatstring,
                               (*pPrint).cameralookat);
  }

  nev_print_init_camera_sky ((*pPrint).cameraskystring, dim, (*pPrint).camerasky);

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

void
nev_print_init_light (struct TESS Tess, struct TESR Tesr, struct MESH *Mesh, struct PRINT *pPrint)
{
  int dim;

  if ((*pPrint).showmesh == 1)
    dim = neut_mesh_array_dim (Mesh);
  else if ((*pPrint).showtess == 1)
    dim = Tess.Dim;
  else if ((*pPrint).showtesr == 1)
    dim = Tesr.Dim;
  else
    dim = 3;

  // lightsourcesource

  if (!strcmp ((*pPrint).lightsourcestring, "default"))
  {
    if (dim == 3)
    {
      (*pPrint).lightsources = ut_alloc_1d_pchar (1);
      ut_string_string ("point(coo=camera,shadow=0)", (*pPrint).lightsources);
      (*pPrint).lightsourceqty = 1;
    }
    else
      (*pPrint).lightsourceqty = 0;
  }

  else if (!(strcmp ((*pPrint).lightsourcestring, "none")))
    (*pPrint).lightsourceqty = 0;

  else
    ut_list_break ((*pPrint).lightsourcestring, NEUT_SEP_NODEP,
                   &(*pPrint).lightsources, &(*pPrint).lightsourceqty);

  // lightambient

  if (!strcmp ((*pPrint).lightambientstring, "default"))
  {
    if (dim == 3)
      (*pPrint).lightambient = 0.6;
    else
      (*pPrint).lightambient = 1;
  }
  else
    (*pPrint).lightambient = atof ((*pPrint).lightambientstring);

  if (!strcmp ((*pPrint).lightdiffusestring, "default"))
    (*pPrint).lightdiffuse = 0.6;
  else
    (*pPrint).lightdiffuse = atof ((*pPrint).lightdiffusestring);

  if (!strcmp ((*pPrint).lightreflectionstring, "default"))
    (*pPrint).lightreflection = 0;
  else
    (*pPrint).lightreflection = atof ((*pPrint).lightreflectionstring);

  return;
}

void
nev_print_init_pf (struct PRINT *pPrint)
{
  int i, varqty;
  int *polef = ut_alloc_1d_int (3);
  char *fct = NULL;
  char **vars = NULL, **vals = NULL;

  sscanf ((*pPrint).pfpolestring, "%d:%d:%d\n", polef, polef + 1, polef + 2);
  ol_polef_polecrysym (polef, "cubic", &(*pPrint).pfpoles, &(*pPrint).pfpoleqty);

  ut_string_function ((*pPrint).pfkernel, &fct, &vars, &vals, &varqty);

  if (varqty == 1)
    (*pPrint).pfkernelsig = atof (vals[0]);

  int dirqty;
  char **dirs = NULL;

  (*pPrint).pfdir = ut_alloc_2d (3, 3);

  ut_list_break ((*pPrint).pfdirstring, NEUT_SEP_DEP, &dirs, &dirqty);
  if (dirqty != 2)
    ut_print_exprbug ((*pPrint).pfdirstring);

  for (i = 0; i < 2; i++)
  {
    if (!strcmp (dirs[i], "x") || !strcmp (dirs[i], "+x") || !strcmp (dirs[i], "-x"))
      (*pPrint).pfdir[i][0] = 1.;
    else if (!strcmp (dirs[i], "y") || !strcmp (dirs[i], "+y") || !strcmp (dirs[i], "-y"))
      (*pPrint).pfdir[i][1] = 1.;
    else if (!strcmp (dirs[i], "z") || !strcmp (dirs[i], "+z") || !strcmp (dirs[i], "-z"))
      (*pPrint).pfdir[i][2] = 1.;
    else
      ut_print_exprbug ((*pPrint).pfdirstring);

    if (strstr (dirs[i], "-"))
      ut_array_1d_scale ((*pPrint).pfdir[i], 3, -1);
  }

  ut_vector_vectprod ((*pPrint).pfdir[0], (*pPrint).pfdir[1], (*pPrint).pfdir[2]);

  if (ut_num_equal (ut_array_1d_min ((*pPrint).pfdir[2], 3), -1, 1e-6))
    ut_print_message (1, 2, "%s: normal vector pointing downwards, which is non-standard.\n", (*pPrint).pfdirstring);

  ut_free_2d_char (&dirs, dirqty);
  ut_free_1d_int (&polef);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);

  return;
}
