/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_real_png_tess_.h"
#include<unistd.h>
#include<fcntl.h>

// Meshes the curved faces and edges of a tessellation (domain faces that are
// not planar, and the faces and edges that they bound), so that they can be
// printed curved instead of flat.  The mesh is a temporary 2D mesh obtained
// as with `neper -M -cl <cl> -pl 100 -dim 2', restricted to the curved faces,
// the faces with a curved edge and their edges, and built silently.  The
// element size, cl, is such that an element spans an angle of
// NEV_CURVEDMESH_ANGLE on the most curved domain face, so that all faces and
// edges render smooth.  Returns 1 if the tessellation has curved faces (and
// the mesh was built), 0 otherwise.
int
nev_print_real_png_tess_curvedmesh (struct TESS Tess, struct NODES *pNodes,
                                    struct MESH *Mesh)
{
  int i, j, curved = 0, stdout_fd, null_fd;
  double rmin;
  struct IN_M In;
  struct MESHPARA MeshPara;
  struct TESS Tess2;
  struct TESR Tesr;
  struct NODES RNodes;
  struct MESH *RMesh = NULL;

  if (Tess.Dim != 3)
    return 0;

  for (i = 1; i <= Tess.FaceQty; i++)
    if (neut_tess_face_iscurved (Tess, i))
    {
      curved = 1;
      break;
    }

  rmin = nev_print_real_png_tess_curvedmesh_rmin (Tess);

  if (!curved || rmin <= 0)
    return 0;

  ut_print_message (0, 3, "Meshing curved faces...\n");

  RMesh = (struct MESH *) calloc (6, sizeof (struct MESH));

  nem_in_set_zero (&In);
  neut_meshpara_set_zero (&MeshPara);
  neut_tess_set_zero (&Tess2);
  neut_tesr_set_zero (&Tesr);
  neut_nodes_set_zero (&RNodes);
  for (i = 0; i < 6; i++)
    neut_mesh_set_zero (RMesh + i);

  // as -M -cl <cl> -pl 100 -dim 2
  nem_input_options_default (&In);
  ut_string_string ("abs", &In.cltype);
  In.clstring = ut_realloc_1d_char (In.clstring, 100);
  sprintf (In.clstring, "%.12g", rmin * NEV_CURVEDMESH_ANGLE * M_PI / 180);
  In.pl = 100;
  ut_string_string ("2", &In.dimstring);
  ut_string_string ("tess", &In.tess);
  nem_input_init_dim_tess (&In, Tess);

  // faces and edges to mesh: the curved faces and the faces with a curved
  // edge, and their edges; for a periodic tessellation, also the master faces
  // and edges (from which the slave ones are meshed)
  int *facemesh = ut_alloc_1d_int (Tess.FaceQty + 1);
  int *edgemesh = ut_alloc_1d_int (Tess.EdgeQty + 1);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (neut_tess_face_iscurved (Tess, i) || neut_tess_face_hascurvededge (Tess, i))
    {
      facemesh[i] = 1;
      if (Tess.PerFaceMaster && Tess.PerFaceMaster[i] > 0)
        facemesh[Tess.PerFaceMaster[i]] = 1;
    }

  for (i = 1; i <= Tess.FaceQty; i++)
    if (facemesh[i])
      for (j = 1; j <= Tess.FaceEdgeQty[i]; j++)
        edgemesh[Tess.FaceEdgeNb[i][j]] = 1;

  if (Tess.PerEdgeMaster)
    for (i = 1; i <= Tess.EdgeQty; i++)
      if (edgemesh[i] && Tess.PerEdgeMaster[i] > 0)
        edgemesh[Tess.PerEdgeMaster[i]] = 1;

  In.meshfacelist = ut_alloc_1d_int (Tess.FaceQty + 1);
  In.meshedgelist = ut_alloc_1d_int (Tess.EdgeQty + 1);
  for (i = 1; i <= Tess.FaceQty; i++)
    if (facemesh[i])
      In.meshfacelist[++In.meshfacelist[0]] = i;
  for (i = 1; i <= Tess.EdgeQty; i++)
    if (edgemesh[i])
      In.meshedgelist[++In.meshedgelist[0]] = i;
  ut_free_1d_int (&facemesh);
  ut_free_1d_int (&edgemesh);

  neut_tess_tess (Tess, &Tess2);

  // meshing, with the standard output disabled
  fflush (stdout);
  stdout_fd = dup (STDOUT_FILENO);
  null_fd = open ("/dev/null", O_WRONLY);
  dup2 (null_fd, STDOUT_FILENO);
  close (null_fd);

  nem_meshing_para (In, &Tess2, &Tesr, &RNodes, RMesh, &MeshPara);
  nem_meshing (In, &MeshPara, &Tess2, RNodes, RMesh, pNodes, Mesh);

  fflush (stdout);
  dup2 (stdout_fd, STDOUT_FILENO);
  close (stdout_fd);

  nem_in_free (In);
  neut_meshpara_free (&MeshPara);
  neut_tess_free (&Tess2);
  neut_tesr_free (&Tesr);
  neut_nodes_free (&RNodes);
  for (i = 0; i < 6; i++)
    neut_mesh_free (RMesh + i);
  free (RMesh);

  return 1;
}

// Tests whether a face must be printed from the curved mesh
int
nev_print_real_png_tess_face_iscurved (struct TESS Tess, struct MESH *Mesh,
                                       int face)
{
  return Mesh && face <= Mesh[2].ElsetQty && Mesh[2].Elsets[face][0] > 0
    && (neut_tess_face_iscurved (Tess, face)
        || neut_tess_face_hascurvededge (Tess, face));
}

// Tests whether an edge must be printed from the curved mesh
int
nev_print_real_png_tess_edge_iscurved (struct TESS Tess, struct MESH *Mesh,
                                       int edge)
{
  return Mesh && edge <= Mesh[1].ElsetQty && Mesh[1].Elsets[edge][0] > 0
    && neut_tess_edge_iscurved (Tess, edge);
}

// Prints the elts of a face as smooth triangles, whose normals at the nodes
// are the area-weighted averages of the normals of the elts of the face, so
// that the face renders smooth (the elts of a face are consistently oriented)
void
nev_print_real_png_tess_face_mesh (FILE * file, struct NODES Nodes,
                                   struct MESH Mesh2D, int face,
                                   char *texture)
{
  int i, j, k, elt, pos, *nodes = NULL, nodeqty = 0;
  int eltqty = Mesh2D.Elsets[face][0];
  int *facenodes = ut_alloc_1d_int (3 * eltqty);
  double **n = ut_alloc_2d (3 * eltqty, 3);
  double *u = ut_alloc_1d (3), *v = ut_alloc_1d (3), *w = ut_alloc_1d (3);

  // node normals
  for (i = 1; i <= eltqty; i++)
  {
    nodes = Mesh2D.EltNodes[Mesh2D.Elsets[face][i]];

    ut_array_1d_sub (Nodes.NodeCoo[nodes[0]], Nodes.NodeCoo[nodes[1]], 3, u);
    ut_array_1d_sub (Nodes.NodeCoo[nodes[0]], Nodes.NodeCoo[nodes[2]], 3, v);
    ut_vector_vectprod (u, v, w);

    for (j = 0; j < 3; j++)
    {
      pos = ut_array_1d_int_eltpos (facenodes, nodeqty, nodes[j]);
      if (pos == -1)
      {
        pos = nodeqty;
        facenodes[nodeqty++] = nodes[j];
      }
      ut_array_1d_add (n[pos], w, 3, n[pos]);
    }
  }

  for (i = 0; i < nodeqty; i++)
    ut_array_1d_normalize (n[i], 3);

  for (i = 1; i <= eltqty; i++)
  {
    elt = Mesh2D.Elsets[face][i];
    nodes = Mesh2D.EltNodes[elt];

    fprintf (file, "smooth_triangle {");
    for (j = 0; j < 3; j++)
    {
      pos = ut_array_1d_int_eltpos (facenodes, nodeqty, nodes[j]);
      fprintf (file, "%s<%.12g,%.12g,%.12g>,", j ? "\n                 " : "",
               Nodes.NodeCoo[nodes[j]][0], Nodes.NodeCoo[nodes[j]][1],
               Nodes.NodeCoo[nodes[j]][2]);
      for (k = 0; k < 3; k++)
        fprintf (file, "%s%.12g", k ? "," : "<", n[pos][k]);
      fprintf (file, ">%s", j < 2 ? "," : "\n");
    }
    fprintf (file, "  texture { %s }\n}\n", texture);
  }

  ut_free_1d_int (&facenodes);
  ut_free_2d (&n, 3 * eltqty);
  ut_free_1d (&u);
  ut_free_1d (&v);
  ut_free_1d (&w);

  return;
}

void
nev_print_real_png_tess_edge_mesh (FILE * file, struct NODES Nodes,
                                   struct MESH Mesh1D, int edge, double rad,
                                   char *texture)
{
  int i, elt;

  for (i = 1; i <= Mesh1D.Elsets[edge][0]; i++)
  {
    elt = Mesh1D.Elsets[edge][i];

    nev_print_real_png_segment_wsph (file,
                                     Nodes.NodeCoo[Mesh1D.EltNodes[elt][0]],
                                     Nodes.NodeCoo[Mesh1D.EltNodes[elt][1]],
                                     rad, texture);
  }

  return;
}
