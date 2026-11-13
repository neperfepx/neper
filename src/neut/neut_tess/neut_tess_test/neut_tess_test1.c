/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_.h"

int
neut_tess_test (struct TESS Tess, int verbosity)
{
  int i;
  int res = 0;

  if (!strcmp (Tess.Type, "periodic"))
    return res;

  if (verbosity)
    ut_print_message (0, 1, "Checking vertices:\n");

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      if (neut_tess_test_ver (Tess, i, verbosity) != 0)
        return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking edges:\n");

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] == 0)
      if (neut_tess_test_edge (Tess, i, verbosity) != 0)
        return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking faces:\n");

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceState[i] >= 0)
      if (neut_tess_test_face (Tess, i, verbosity) != 0)
        return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking polyhedra:\n");

  for (i = 1; i <= Tess.PolyQty; i++)
    if (Tess.PolyState[i] >= 0)
      if (neut_tess_test_poly (Tess, i, verbosity) != 0)
        return -1;

  if (verbosity)
    ut_print_message (0, 1, "Checking domain:\n");

  if (Tess.DomVerQty > 0)
    if (neut_tess_test_dom (Tess, verbosity) != 0)
      return -1;

  return res;
}

int
neut_tess_testAroundVer (struct TESS Tess, int ver, int verbosity)
{
  int i, edge;
  int res = 0;
  int faceqty, polyqty;
  int *face = NULL;
  int *poly = NULL;

  if (verbosity >= 4)
    printf ("checking TESS AROUND VER %d\n", ver);

  /* Checking of the vertex */
  if (verbosity >= 4)
    printf ("Checking vertex: ");

  if (Tess.VerState[ver] != -1)
  {
    if (verbosity >= 4)
      printf ("%d:", ver);
    res = neut_tess_test_ver (Tess, ver, verbosity);
    if (res != 0)
    {
      if (verbosity >= 4)
        printf ("ver %d: nok\n", ver);
      return res;
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Checking of the edges */
  if (verbosity >= 4)
    printf ("\nChecking edges: ");

  for (i = 0; i <= Tess.VerEdgeQty[ver] - 1; i++)
  {
    edge = Tess.VerEdgeNb[ver][i];

    if (Tess.EdgeState[edge] == 0)
    {
      if (verbosity >= 4)
        printf ("%d:", edge);
      res = neut_tess_test_edge (Tess, edge, 0);
      if (res != 0)
      {
        if (verbosity >= 4)
          printf ("edge %d: nok\n", edge);
        return res;
      }
    }
    else
    {
      res = 1;
      abort ();
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  neut_tess_ver_faces (Tess, ver, &face, &faceqty);

  if (verbosity >= 4)
    printf ("\nChecking faces: ");

  for (i = 0; i < faceqty; i++)
  {
    if (Tess.FaceState[face[i]] >= 0)
    {
      if (verbosity >= 4)
        printf ("%d:", face[i]);

      res = neut_tess_test_face (Tess, face[i], 0);
      if (res != 0)
      {
        if (verbosity >= 4)
          printf ("face[i] %d: nok\n", face[i]);
        return res;
      }
    }
    else
    {
      abort ();
      return 1;
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Checking of the polys */
  neut_tess_ver_polys (Tess, ver, &poly, &polyqty);

  if (verbosity >= 4)
    printf ("\nChecking polys: ");
  for (i = 0; i < polyqty; i++)
  {
    if (poly[i] < 0)
      continue;

    if (verbosity >= 4)
      printf ("%d:", i);
    res = neut_tess_test_poly (Tess, poly[i], 0);
    if (res != 0)
    {
      if (verbosity >= 4)
        printf ("poly %d: nok\n", poly[i]);
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /* Checking domain */
  if (verbosity >= 4)
    printf ("\nChecking domain: ");
  res = neut_tess_test_dom (Tess, 0);

  ut_free_1d_int (&face);
  ut_free_1d_int (&poly);

  return res;
}

/* Checking the tessellation around a vertex: all the polys around the ver
 * and all entities of the polys: faces, edges & vertices.
 */
int
neut_tess_testAroundVer2 (struct TESS Tess, int ver, int verbosity)
{
  int i, j, k;
  int edge, face;
  int res = 0;
  int verqty, *vers = NULL;
  int edgeqty, *edges = NULL;
  int faceqty, *faces = NULL;
  int polyqty, *polys = NULL;

  if (verbosity >= 4)
    printf ("checking TESS (2) AROUND VER %d (/%d)\n", ver, Tess.VerQty);

  /* Searching polyhedra */
  neut_tess_ver_polys (Tess, ver, &polys, &polyqty);

  /* searching faces and edges of all the polyhedra (made unique below) */
  faceqty = 0;
  edgeqty = 0;
  verqty = 0;
  for (i = 0; i < polyqty; i++)
    if (polys[i] > 0)
    {
      faceqty += Tess.PolyFaceQty[polys[i]];
      for (j = 1; j <= Tess.PolyFaceQty[polys[i]]; j++)
      {
        edgeqty += Tess.FaceEdgeQty[Tess.PolyFaceNb[polys[i]][j]];
        verqty += Tess.FaceVerQty[Tess.PolyFaceNb[polys[i]][j]];
      }
    }
  faces = ut_alloc_1d_int (faceqty);
  edges = ut_alloc_1d_int (edgeqty);
  vers = ut_alloc_1d_int (verqty);

  faceqty = 0;
  edgeqty = 0;
  verqty = 0;
  for (i = 0; i < polyqty; i++)
  {
    if (polys[i] <= 0)
      continue;

    for (j = 1; j <= Tess.PolyFaceQty[polys[i]]; j++)
    {
      face = Tess.PolyFaceNb[polys[i]][j];

      faces[faceqty++] = face;

      for (k = 1; k <= Tess.FaceEdgeQty[face]; k++)
        edges[edgeqty++] = Tess.FaceEdgeNb[face][k];

      for (k = 1; k <= Tess.FaceVerQty[face]; k++)
        vers[verqty++] = Tess.FaceVerNb[face][k];

    }
  }

  ut_array_1d_int_sort_uniq (faces, faceqty, &faceqty);
  ut_array_1d_int_sort_uniq (edges, edgeqty, &edgeqty);
  ut_array_1d_int_sort_uniq (vers, verqty, &verqty);

  /* Checking of the vertex */
  if (verbosity >= 4)
    printf ("Checking vertex: ");

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];

    if (Tess.VerState[ver] != -1)
    {
      if (verbosity >= 4)
        printf ("%d:", ver);
      res = neut_tess_test_ver (Tess, ver, verbosity);
      if (res != 0)
      {
        if (verbosity >= 4)
          printf ("ver %d: nok\n", ver);
        ut_free_1d_int (&vers);
        ut_free_1d_int (&edges);
        ut_free_1d_int (&faces);
        ut_free_1d_int (&polys);
        return res;
      }
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Checking of the edges */
  if (verbosity >= 4)
    printf ("\nChecking edges: ");

  for (i = 0; i < edgeqty; i++)
  {
    edge = edges[i];

    if (Tess.EdgeState[edge] == 0)
    {
      if (verbosity >= 4)
        printf ("%d:", edge);
      res = neut_tess_test_edge (Tess, edge, verbosity);
      if (res != 0)
      {
        if (verbosity >= 4)
          printf ("edge %d: nok\n", edge);
        ut_free_1d_int (&vers);
        ut_free_1d_int (&edges);
        ut_free_1d_int (&faces);
        ut_free_1d_int (&polys);
        return res;
      }
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /* Checking of the faces */

  if (verbosity >= 4)
    printf ("\nChecking faces: ");

  for (i = 0; i < faceqty; i++)
  {
    face = faces[i];

    if (Tess.FaceState[face] >= 0)
    {
      if (verbosity >= 4)
        printf ("%d:", face);
      res = neut_tess_test_face (Tess, face, verbosity);
      if (res != 0)
      {
        if (verbosity >= 4)
          printf ("face %d: nok\n", face);
        ut_free_1d_int (&vers);
        ut_free_1d_int (&edges);
        ut_free_1d_int (&faces);
        ut_free_1d_int (&polys);
        return res;
      }
    }
  }

  if (verbosity >= 4)
    printf ("\n");

  /* Checking of the polys */

  if (verbosity >= 4)
    printf ("\nChecking polys: ");

  for (i = 0; i < polyqty; i++)
  {
    if (polys[i] < 0)
      continue;

    if (verbosity >= 4)
      printf ("%d:", i);
    res = neut_tess_test_poly (Tess, polys[i], verbosity);
    if (res != 0)
    {
      if (verbosity >= 4)
        printf ("polys %d: nok\n", polys[i]);
      ut_free_1d_int (&vers);
      ut_free_1d_int (&edges);
      ut_free_1d_int (&faces);
      ut_free_1d_int (&polys);
      return res;
    }
  }
  if (verbosity >= 4)
    printf ("\n");

  /*
     // Checking domain
     if (verbosity >= 4)
     printf ("\nChecking domain: ");
     res = neut_tess_test_dom (Tess, 0);
   */

  ut_free_1d_int (&vers);
  ut_free_1d_int (&edges);
  ut_free_1d_int (&faces);
  ut_free_1d_int (&polys);

  return res;
}

// Testing the periodicity of a tessellation: each slave vertex, edge and
// face is a copy of its master, i.e. the vertex is located at the master
// vertex shifted by the periodicity distance, and the vertices of the edge
// or face are copies of the vertices of the master edge or face, of the
// same offset (and in the same order, accounting for orientation).
int
neut_tess_test_per (struct TESS Tess, int verbosity)
{
  int i, j, k, ver, master, qty, pos, ori;
  int *shift = ut_alloc_1d_int (3), *vers = NULL;
  double size, *coo = ut_alloc_1d (3);
  int res = 0;

  neut_tess_bboxsize (Tess, coo);
  size = ut_array_1d_max (coo, 3);

  // vertices
  for (i = 1; i <= Tess.VerQty && !res; i++)
  {
    master = Tess.PerVerMaster[i];

    if (Tess.VerState[i] == -1 || master <= 0)
      continue;

    if (master > Tess.VerQty || Tess.VerState[master] == -1
        || Tess.PerVerMaster[master] != 0
        || ut_array_1d_int_eltpos (Tess.PerVerSlaveNb[master] + 1,
                                   Tess.PerVerSlaveQty[master], i) == -1)
      res = -1;

    for (j = 0; j < 3; j++)
      coo[j] = Tess.VerCoo[master][j]
        + Tess.PerVerShift[i][j] * Tess.PeriodicDist[j];
    if (ut_space_dist (coo, Tess.VerCoo[i]) > 1e-9 * size)
      res = -1;

    if (res && verbosity)
      printf ("periodicity of vertex %d (master %d) is not valid\n", i,
              master);
  }

  // edges
  for (i = 1; i <= Tess.PerEdgeQty && !res; i++)
  {
    master = Tess.PerEdgeMaster[Tess.PerEdgeNb[i]];
    ori = Tess.PerEdgeOri[Tess.PerEdgeNb[i]];

    for (j = 0; j < 2 && !res; j++)
    {
      ver = Tess.EdgeVerNb[Tess.PerEdgeNb[i]][(ori == 1) ? j : 1 - j];
      res = neut_tess_ver_iscopy (Tess, ver, Tess.EdgeVerNb[master][j],
                                  Tess.PerEdgeShift[Tess.PerEdgeNb[i]]) ?
        0 : -1;
    }

    if (res && verbosity)
      printf ("periodicity of edge %d (master %d) is not valid\n",
              Tess.PerEdgeNb[i], master);
  }

  // faces: a face of a periodic seed (slave seed) is periodic
  if (Tess.Dim == 3)
    for (i = 1; i <= Tess.FaceQty && !res; i++)
      if (Tess.FaceState[i] != -1
          && ut_array_1d_int_max (Tess.FacePoly[i], 2) > Tess.CellQty
          && Tess.PerFaceMaster[i] <= 0 && Tess.PerFaceSlaveNb[i] <= 0)
      {
        res = -1;
        if (verbosity)
          printf ("face %d is not periodic\n", i);
      }

  // faces
  for (i = 1; i <= Tess.PerFaceQty && !res; i++)
  {
    master = Tess.PerFaceMaster[Tess.PerFaceNb[i]];
    qty = Tess.FaceVerQty[master];

    if (Tess.FaceVerQty[Tess.PerFaceNb[i]] != qty)
      res = -1;

    vers = Tess.FaceVerNb[Tess.PerFaceNb[i]] + 1;

    // position, in the slave face, of the copy of the first vertex of the
    // master face
    pos = -1;
    for (j = 0; j < qty && pos == -1 && !res; j++)
      if (neut_tess_ver_iscopy (Tess, vers[j], Tess.FaceVerNb[master][1],
                                Tess.PerFaceShift[Tess.PerFaceNb[i]]))
        pos = j;

    if (pos == -1)
      res = -1;

    // following vertices, in the same or opposite order
    for (ori = 1; ori >= -1 && !res; ori -= 2)
    {
      for (j = 1; j < qty; j++)
      {
        k = (pos + ori * j + qty) % qty;
        if (!neut_tess_ver_iscopy (Tess, vers[k],
                                   Tess.FaceVerNb[master][j + 1],
                                   Tess.PerFaceShift[Tess.PerFaceNb[i]]))
          break;
      }

      if (j == qty)
        break;
    }

    if (ori < -1)
      res = -1;

    if (res && verbosity)
      printf ("periodicity of face %d (master %d) is not valid\n",
              Tess.PerFaceNb[i], master);
  }

  ut_free_1d_int (&shift);
  ut_free_1d (&coo);

  return res;
}
