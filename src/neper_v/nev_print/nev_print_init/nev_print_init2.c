/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_init_.h"

void
nev_print_init_data (struct IN_V In, struct PRINT Print,
               struct TESS Tess, struct DATA *TessData, struct TESR Tesr,
               struct DATA *TesrData, struct NODES Nodes,
               struct MESH *Mesh, struct POINT *Points, int PointQty,
               struct DATA *pData, struct DATA **MeshData,
               struct DATA *pCsysData, struct DATA *PointData, char **pcrysym)
{
  int i, j, meshdim;
  double size;
  char **entity = ut_alloc_1d_pchar (3);
  ut_string_string ("elt", entity);
  ut_string_string ("elset", entity + 1);
  ut_string_string ("mesh", entity + 2);

  /*
  if (Tess.Dim == 0 && Tess.CellQty < 1)
    Tess.CellQty = 1;
  */

  if (Print.showtess && !neut_tess_isvoid (Tess))
  {
    for (i = 0; i <= 5; i++)
      nev_print_init_data_tess (In, Tess, TessData + i);
    ut_string_string (Tess.CellCrySym, pcrysym);
  }

  if (Print.showtesr && !neut_tesr_isvoid (Tesr))
  {
    for (i = 0; i <= 1; i++)
      nev_print_init_data_tesr (In, Tesr, TesrData + i);
    ut_string_string (Tesr.CellCrySym, pcrysym);
  }

  if ((Print.shownode || Print.showmesh || Print.showslice) && !neut_nodes_isvoid (Nodes))
    nev_print_init_data_nodes (In, Nodes, *pcrysym, Nodes.NodeQty, pData);

  meshdim = neut_mesh_array_dim (Mesh);
  if ((Print.showmesh || Print.showslice) && meshdim > 0)
  {
    neut_mesh_size (Nodes, Mesh[meshdim], &size);
    ut_string_string (Mesh[meshdim].ElsetCrySym, pcrysym);

    for (i = 0; i <= meshdim; i++)
      if (!neut_mesh_isvoid (Mesh[i]))
        for (j = 0; j < 3; j++)
          nev_print_init_data_mesh (In, Nodes, Mesh[i], *pcrysym, size, Mesh[i].ElsetQty,
                                    entity[j], meshdim, MeshData[i] + j);
  }

  if (Print.showcsys)
    nev_print_init_data_csys (In, pCsysData);

  if (PointQty != Print.inputqty)
    abort ();

  for (i = 0; i < Print.inputqty; i++)
    if (!neut_point_isvoid (Points[i]))
      nev_print_init_data_points (In, Points[i], *pcrysym, PointData + i);

  ut_free_2d_char (&entity, 3);

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
    if ((*pPrint).showsliceelt1d == -1)
      (*pPrint).showsliceelt1d = 0;
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

    if ((*pPrint).showelset3d[0] == -1)
    {
      (*pPrint).showelset3d =
        ut_realloc_1d_int ((*pPrint).showelset3d, Mesh[3].ElsetQty + 1);

      (*pPrint).showelset3d[0] = (dim == 3) ? Mesh[3].ElsetQty : 0;
      ut_array_1d_int_set ((*pPrint).showelset3d + 1, Mesh[3].ElsetQty, dim == 3);
    }

    if ((*pPrint).showelset2d[0] == -1)
    {
      (*pPrint).showelset2d =
        ut_realloc_1d_int ((*pPrint).showelset2d, Mesh[2].ElsetQty + 1);

      (*pPrint).showelset2d[0] = (dim == 2) ? Mesh[2].ElsetQty : 0;
      ut_array_1d_int_set ((*pPrint).showelset2d + 1, Mesh[2].ElsetQty, dim == 2);
    }

    if ((*pPrint).showelset1d[0] == -1)
    {
      (*pPrint).showelset1d =
        ut_realloc_1d_int ((*pPrint).showelset1d, Mesh[1].ElsetQty + 1);

      (*pPrint).showelset1d[0] = (dim == 1) ? Mesh[1].ElsetQty : 0;
      ut_array_1d_int_set ((*pPrint).showelset1d + 1, Mesh[1].ElsetQty, dim == 1);
    }

    if ((*pPrint).showelset0d[0] == -1)
    {
      (*pPrint).showelset0d =
        ut_realloc_1d_int ((*pPrint).showelset0d, Mesh[0].ElsetQty + 1);

      (*pPrint).showelset0d[0] = (dim == 0) ? Mesh[0].ElsetQty : 0;
      ut_array_1d_int_set ((*pPrint).showelset0d + 1, Mesh[0].ElsetQty, dim == 0);
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
nev_print_init_camera (struct IN_V In, struct TESS Tess, struct TESR Tesr, struct NODES Nodes,
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

    nev_print_init_camera_coo_tess (Tess, In.cameracoo,
                              (*pPrint).cameracoo);
    nev_print_init_camera_coo_tess (Tess, In.cameralookat,
                              (*pPrint).cameralookat);
  }

  else if (!neut_nodes_isvoid (Nodes))
  {
    dim = neut_nodes_dim (Nodes);
    if (dim == -1)
      ut_print_neperbug ();

    nev_print_init_camera_coo_mesh (Nodes2, Mesh[dim], In.cameracoo,
                                    (*pPrint).cameracoo);
    nev_print_init_camera_coo_mesh (Nodes2, Mesh[dim], In.cameralookat,
                                    (*pPrint).cameralookat);
  }

  else if (!neut_tesr_isvoid (Tesr))
  {
    dim = Tesr.Dim;

    nev_print_init_camera_coo_tesr (Tesr, In.cameracoo,
                                    (*pPrint).cameracoo);
    nev_print_init_camera_coo_tesr (Tesr, In.cameralookat,
                                    (*pPrint).cameralookat);
  }

  else if (PointQty > 0)
  {
    dim = Points[0].Dim;

    nev_print_init_camera_coo_points (Points[0], In.cameracoo,
                               (*pPrint).cameracoo);
    nev_print_init_camera_coo_points (Points[0], In.cameralookat,
                               (*pPrint).cameralookat);
  }

  nev_print_init_camera_sky (In.camerasky, dim, (*pPrint).camerasky);

  (*pPrint).cameraangle = atof (In.cameraangle);

  ut_string_string (In.cameraprojection, &(*pPrint).cameraprojection);
  if (!strcmp (In.cameraprojection, "default"))
  {
    ut_string_string ("perspective", &(*pPrint).cameraprojection);
    if (dim <= 2)
      ut_string_string ("orthographic", &(*pPrint).cameraprojection);
  }

  if (NodeData.CooData)
    neut_nodes_free (&Nodes2);

  return;
}

void
nev_print_init_light (struct IN_V In, struct TESS Tess, struct TESR Tesr, struct MESH *Mesh, struct PRINT *pPrint)
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

  if (!strcmp (In.lightsource, "default"))
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

  else if (!(strcmp (In.lightsource, "none")))
    (*pPrint).lightsourceqty = 0;

  else
    ut_list_break (In.lightsource, NEUT_SEP_NODEP,
                   &(*pPrint).lightsources, &(*pPrint).lightsourceqty);

  // lightambient

  if (!strcmp (In.lightambient, "default"))
  {
    if (dim == 3)
      (*pPrint).lightambient = 0.6;
    else
      (*pPrint).lightambient = 1;
  }
  else
    (*pPrint).lightambient = atof (In.lightambient);

  if (!strcmp (In.lightdiffuse, "default"))
    (*pPrint).lightdiffuse = 0.6;
  else
    (*pPrint).lightdiffuse = atof (In.lightdiffuse);

  if (!strcmp (In.lightreflection, "default"))
    (*pPrint).lightreflection = 0;
  else
    (*pPrint).lightreflection = atof (In.lightreflection);

  return;
}

void
nev_print_init_pf (struct IN_V In, struct TESS Tess, struct TESR Tesr,
                   struct POINT *Points, int PointQty, struct PF *pPf)
{
  int i, varqty;
  double *polef = ut_alloc_1d (3);
  char *fct = NULL;
  char **vars = NULL, **vals = NULL;
  char *inputcrysym = NULL;

  ut_string_string (In.space, &(*pPf).space);
  ut_string_string (In.pfshape, &(*pPf).shape);
  ut_string_string (In.pfprojection, &(*pPf).projection);
  ut_string_string (In.pfsym, &(*pPf).refsym);
  ut_string_string (In.pffont, &(*pPf).font);
  ut_string_string (In.pfmode, &(*pPf).mode);
  ut_string_string (In.pfpolelabel, &(*pPf).pfpolelabel);
  ut_string_string (In.pfprojlabel, &(*pPf).pfprojlabel);
  if (!strcmp ((*pPf).pfprojlabel, "default"))
  {
    if (!strcmp ((*pPf).projection, "stereographic"))
      ut_string_string ("stereo. proj.", &(*pPf).pfprojlabel);
    else if (!strcmp ((*pPf).projection, "equal-area"))
      ut_string_string ("equal-area proj.", &(*pPf).pfprojlabel);
  }
  (*pPf).clustering = In.pfclustering;

  if (!neut_tess_isvoid (Tess))
    ut_string_string (Tess.CellCrySym, &inputcrysym);
  else if (!neut_tesr_isvoid (Tesr))
    ut_string_string (Tesr.CellCrySym, &inputcrysym);
  for (i = 0; i < PointQty; i++)
    if (Points[i].crysym)
    {
      ut_string_string (Points[i].crysym, &inputcrysym);
      break;
    }

  int inputcrysymdef = 0;
  if (inputcrysym && strcmp (inputcrysym, "triclinic"))
    inputcrysymdef = 1;

  if (inputcrysymdef)
  {
    if (In.crysym)
    {
      if (strcmp (inputcrysym, In.crysym))
        ut_print_message (1, 2, "Enforcing %s crystal symmetry over input value (%s)\n",
                          In.crysym, inputcrysym);

      ut_string_string (In.crysym, &(*pPf).crysym);
    }
    else
      ut_string_string (inputcrysym, &(*pPf).crysym);
  }

  else
  {
    if (In.crysym)
      ut_string_string (In.crysym, &(*pPf).crysym);
    else
      ut_string_string ("cubic", &(*pPf).crysym);
  }

  if (!strcmp (In.pfpolestring, "default"))
  {
    if (!strcmp (In.space, "pf"))
    {
      if (!strcmp ((*pPf).crysym, "cubic"))
      {
        ut_string_string ("111", &(*pPf).pfpolestring);
        ut_array_1d_set_3 (polef, 1, 1, 1);
      }
      else if (!strcmp ((*pPf).crysym, "hexagonal"))
      {
        ut_string_string ("0001", &(*pPf).pfpolestring);
        ut_array_1d_set_3 (polef, 0, 0, 1);
      }
    }
    else if (!strcmp (In.space, "ipf"))
      ut_array_1d_set_3 (polef, 0, 0, 1);
    else // real space
    {}
  }
  else
  {
    int qty, *tmp = NULL;
    ut_list_break_int (In.pfpolestring, NEUT_SEP_DEP, &tmp, &qty);
    ut_string_string (In.pfpolestring, &(*pPf).pfpolestring);
    ut_string_fnrs ((*pPf).pfpolestring, NEUT_SEP_DEP, "", INT_MAX);
    if (qty == 3)
      ut_array_1d_memcpy_fromint (tmp, 3, polef);
    else if (qty == 4)
      ol_hpole_vect (tmp, polef);
    ut_free_1d_int (&tmp);
  }

  if (!strcmp (In.space, "pf"))
    ol_vect_crysym_all (polef, (*pPf).crysym, &(*pPf).pfpoles, &(*pPf).pfpoleqty);

  else if (!strcmp (In.space, "ipf"))
  {
    (*pPf).pfpoleqty = 1;
    (*pPf).pfpoles = ut_alloc_2d ((*pPf).pfpoleqty + 1, 3);
    ut_array_1d_memcpy (polef, 3, (*pPf).pfpoles[1]);
  }

  else // real space
  {}

  for (i = 1; i <= (*pPf).pfpoleqty; i++)
    ut_array_1d_normalize ((*pPf).pfpoles[i], 3);

  ut_string_function (In.pfkernel, &fct, &vars, &vals, &varqty);

  if (varqty == 1)
    (*pPf).pfkernelsig = atof (vals[0]);

  int dirqty;
  char **dirs = NULL;

  (*pPf).pfdir = ut_alloc_2d (3, 3);

  ut_list_break (In.pfdirstring, NEUT_SEP_DEP, &dirs, &dirqty);
  if (dirqty != 2)
    ut_print_exprbug (In.pfdirstring);

  for (i = 0; i < 2; i++)
  {
    if (!strcmp (dirs[i], "x") || !strcmp (dirs[i], "+x") || !strcmp (dirs[i], "-x"))
      (*pPf).pfdir[i][0] = 1.;
    else if (!strcmp (dirs[i], "y") || !strcmp (dirs[i], "+y") || !strcmp (dirs[i], "-y"))
      (*pPf).pfdir[i][1] = 1.;
    else if (!strcmp (dirs[i], "z") || !strcmp (dirs[i], "+z") || !strcmp (dirs[i], "-z"))
      (*pPf).pfdir[i][2] = 1.;
    else
      ut_print_exprbug (In.pfdirstring);

    if (strstr (dirs[i], "-"))
      ut_array_1d_scale ((*pPf).pfdir[i], 3, -1);
  }

  ut_vector_vectprod ((*pPf).pfdir[0], (*pPf).pfdir[1], (*pPf).pfdir[2]);

  if (ut_num_equal (ut_array_1d_min ((*pPf).pfdir[2], 3), -1, 1e-6))
    ut_print_message (1, 2, "%s: normal vector pointing downwards, which is non-standard.\n", In.pfdirstring);

  if (!strcmp (In.space, "ipf"))
    neut_pf_init_ipfborder (pPf);

  (*pPf).gridsize = ut_alloc_1d_int (2);
  ut_array_1d_int_set ((*pPf).gridsize, 2, atoi (In.pfgridsize));
  if (!strcmp (In.space, "ipf") && !strcmp ((*pPf).crysym, "hexagonal"))
    (*pPf).gridsize[1] *= 0.5;

  neut_pf_init_grid (pPf);

  ut_free_2d_char (&dirs, dirqty);
  ut_free_1d (&polef);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&inputcrysym);

  return;
}
