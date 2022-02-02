/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_2D_face_mesh_gmsh_.h"

void
nem_meshing_2D_face_mesh_gmsh_boundary (struct TESS Tess, struct NODES Nodes,
                                        struct MESH *Mesh, int face,
                                        int **pbnodes, double ***pbnodecoos,
                                        int *pbnodeqty)
{
  int i;

  neut_mesh_face_boundnodes (Mesh[1], Tess, face, pbnodes, pbnodeqty);

  if (pbnodecoos)
  {
    (*pbnodecoos) = ut_alloc_2d (*pbnodeqty, 3);
    for (i = 0; i < *pbnodeqty; i++)
      ut_array_1d_memcpy (Nodes.NodeCoo[(*pbnodes)[i]], 3, (*pbnodecoos)[i]);
  }

  return;
}

void
nem_meshing_2D_face_mesh_gmsh_proj (struct TESS Tess, struct NODES Nodes,
                                    int face, struct MESHPARA MeshPara,
                                    int *bnodes, double ***pbnodecoos,
                                    double **pbnodecls, int bnodeqty)
{
  int i, j;

  if (!strcmp (MeshPara.face_op[face], "noproj"))
    return;

  else if (!strcmp (MeshPara.face_op[face], "meshproj")
           || !strcmp (MeshPara.face_op[face], "planeproj"))
    for (i = 0; i < bnodeqty; i++)
      ut_space_point_dir_plane_proj ((*pbnodecoos)[i],
                                     MeshPara.face_eq[face] + 1,
                                     MeshPara.face_eq[face],
                                     (*pbnodecoos)[i]);

  else if (!strcmp (MeshPara.face_op[face], "sphereproj"))
  {
    double *uvect = ut_alloc_1d (3);
    double *c = Tess.DomFaceParms[Tess.FaceDom[face][1]];       // centre

    (*pbnodecls) = ut_alloc_1d (bnodeqty);

    for (i = 0; i < bnodeqty; i++)
    {
      ut_space_points_uvect (c, (*pbnodecoos)[i], uvect);
      ut_space_point_dir_plane_proj ((*pbnodecoos)[i], uvect,
                                     MeshPara.face_eq[face],
                                     (*pbnodecoos)[i]);
    }

    // increasing cl if needed to make sure 2D meshing is ok at the boundary
    for (i = 0; i < bnodeqty; i++)
    {
      int bef, aft;
      bef = ut_array_rotpos (0, bnodeqty - 1, i, -1);
      aft = ut_array_rotpos (0, bnodeqty - 1, i, 1);

      (*pbnodecls)[i] = Nodes.NodeCl[bnodes[i]];
      (*pbnodecls)[i] =
        ut_num_max ((*pbnodecls)[i],
                    1.01 * ut_space_dist ((*pbnodecoos)[bef],
                                          (*pbnodecoos)[i]));
      (*pbnodecls)[i] =
        ut_num_max ((*pbnodecls)[i],
                    1.01 * ut_space_dist ((*pbnodecoos)[aft],
                                          (*pbnodecoos)[i]));
    }

    ut_free_1d (&uvect);
  }

  else if (!strcmp (MeshPara.face_op[face], "cylinderproj"))
  {
    int domface = Tess.FaceDom[face][1];
    double d, r = Tess.DomFaceParms[domface][6];
    double *tmp = ut_alloc_1d (3);
    double *plane = ut_alloc_1d (4);
    double *proj = ut_alloc_1d (3);
    double *uvect = ut_alloc_1d (3);

    ut_vector_vectprod (MeshPara.face_eq[face] + 1,
                        Tess.DomFaceParms[domface] + 3, tmp);

    ut_space_point_normal_plane (Tess.DomFaceParms[domface], tmp, plane);

    for (i = 0; i < bnodeqty; i++)
    {
      ut_space_point_dir_plane_proj ((*pbnodecoos)[i],
                                     MeshPara.face_eq[face] + 1,
                                     MeshPara.face_eq[face],
                                     (*pbnodecoos)[i]);

      ut_space_point_plane_proj ((*pbnodecoos)[i], plane, proj);
      ut_space_points_uvect ((*pbnodecoos)[i], proj, uvect);
      d = ut_space_dist ((*pbnodecoos)[i], proj);
      d = r * asin (d / r);

      for (j = 0; j < 3; j++)
        (*pbnodecoos)[i][j] = proj[j] - d * uvect[j];
    }

    ut_free_1d (&proj);
    ut_free_1d (&plane);
    ut_free_1d (&tmp);
    ut_free_1d (&uvect);
  }

  else
    abort ();

  return;
}

void
nem_meshing_2D_face_mesh_gmsh_backproj (struct TESS Tess, struct NODES RNodes,
                                        struct MESH *RMesh, int face,
                                        struct MESHPARA MeshPara,
                                        struct NODES Nodes, int *bnodes,
                                        int *lbnodes, int bnodeqty,
                                        struct NODES *pN, struct MESH M)
{
  int i, domface = (Tess.Dim == 3) ? Tess.FaceDom[face][1] : -1;

  // Below, we proceed in different ways based on the type of the face

  if (!strcmp (MeshPara.face_op[face], "noproj"))
    return;

  else if (!strcmp (MeshPara.face_op[face], "meshproj"))
    neut_nodes_proj_alongontomesh (pN, MeshPara.face_eq[face] + 1, RNodes,
                                   RMesh[2], face);

  else if (!strcmp (MeshPara.face_op[face], "planeproj"))
  {
    // back-projecting
    struct NODES Nint;
    struct MESH Mint;

    neut_nodes_set_zero (&Nint);
    neut_mesh_set_zero (&Mint);

    neut_tess_face_interpolmesh (Tess, face, &Nint, &Mint);
    neut_nodes_proj_alongontomesh (pN, MeshPara.face_eq[face] + 1, Nint, Mint,
                                   1);

    neut_nodes_free (&Nint);
    neut_mesh_free (&Mint);

    if ((*pN).NodeQty > 0)
      nem_meshing_2D_face_mesh_gmsh_backproj_fixboundary (Nodes, bnodes,
                                                          lbnodes, bnodeqty,
                                                          pN);

    // if the face belongs to a curved domface, projecting onto it
    if (neut_tess_face_iscurved (Tess, face))
      for (i = 1; i <= (*pN).NodeQty; i++)
        neut_primparms_point_proj (Tess.DomFaceType[domface],
                                   Tess.DomFaceParms[domface],
                                   (*pN).NodeCoo[i], (*pN).NodeCoo[i]);

    // if the face has curved edges (edges that belong to a curved domedge)
    // laplacian smoothing it
    else if (neut_tess_face_hascurvededge (Tess, face))
      neut_mesh_2d_laplaciansmooth (pN, M, lbnodes, bnodeqty);

    // in other cases (face not flat), we do nothing
  }

  else if (!strcmp (MeshPara.face_op[face], "sphereproj"))
  {
    int domface = Tess.FaceDom[face][1];
    double *c = Tess.DomFaceParms[domface];     // centre
    double r = Tess.DomFaceParms[domface][3];   // radius

    for (i = 1; i <= (*pN).NodeQty; i++)
      ut_space_point_sphere_proj ((*pN).NodeCoo[i], c, r, (*pN).NodeCoo[i]);
  }

  else if (!strcmp (MeshPara.face_op[face], "cylinderproj"))
  {
    int i, j, domface = Tess.FaceDom[face][1];
    double d, s, r = Tess.DomFaceParms[domface][6];
    double *tmp = ut_alloc_1d (3);
    double *plane = ut_alloc_1d (4);
    double *proj = ut_alloc_1d (3);
    double *uvect = ut_alloc_1d (3);
    double sgn, theta;

    ut_vector_vectprod (MeshPara.face_eq[face] + 1,
                        Tess.DomFaceParms[domface] + 3, tmp);

    ut_space_point_normal_plane (Tess.DomFaceParms[domface], tmp, plane);

    for (i = 1; i <= (*pN).NodeQty; i++)
    {
      ut_space_point_plane_proj ((*pN).NodeCoo[i], plane, proj);
      ut_space_points_uvect ((*pN).NodeCoo[i], proj, uvect);
      d = ut_space_dist ((*pN).NodeCoo[i], proj);

      d = sin (d / r) * r;

      for (j = 0; j < 3; j++)
        (*pN).NodeCoo[i][j] = proj[j] - d * uvect[j];

      theta = asin (d / r);

      s = d * tan (theta / 2);

      sgn =
        ut_space_point_plane_side (Tess.DomFaceParms[domface] - 1,
                                   MeshPara.face_eq[face]);
      for (j = 0; j < 3; j++)
        (*pN).NodeCoo[i][j] += sgn * s * MeshPara.face_eq[face][j + 1];
    }

    ut_free_1d (&proj);
    ut_free_1d (&plane);
    ut_free_1d (&tmp);
    ut_free_1d (&uvect);
  }

  else
    abort ();

  // should not be necessary, but just in case
  if ((*pN).NodeQty > 0)
    nem_meshing_2D_face_mesh_gmsh_backproj_fixboundary (Nodes, bnodes,
                                                        lbnodes, bnodeqty,
                                                        pN);

  return;
}

void
nem_meshing_2D_face_mesh_gmsh_writeboundary (struct NODES Nodes, int *bnodes,
                                             double **bnodecoos,
                                             double *bnodecls, int bnodeqty,
                                             FILE * file)
{
  int i, node;

  // printing face nodes
  for (i = 0; i < bnodeqty; i++)
  {
    node = bnodes[i];

    fprintf (file, "Point(%d) = {%.12f, %.12f, %.12f, %.12f};\n", i + 1,
             bnodecoos[i][0], bnodecoos[i][1], bnodecoos[i][2],
             bnodecls ? bnodecls[i] : Nodes.NodeCl[node]);
  }

  for (i = 1; i <= bnodeqty; i++)
    fprintf (file, "Line(%d) = {%d,%d} ;\n", i, i,
             ut_array_rotpos (1, bnodeqty, i, 1));

  fprintf (file, "Line Loop(1) = {");

  for (i = 0; i < bnodeqty; i++)
    fprintf (file, "%d%s", i + 1, i < bnodeqty - 1 ? "," : "};\n");

  fprintf (file, "Plane Surface(1) = {1}; ");
  fprintf (file, "Physical Surface(1) = {1};\n");

  // fprintf (file, "Mesh.CharacteristicLengthMax = %f;\n", cl);
  // above does not work on all architecture.

  return;
}
