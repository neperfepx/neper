/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_test_dom_.h"

int
neut_tess_test_dom_def (struct TESS Tess, int verbosity)
{
  int i, j, ver, edge, face;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain definition...\n");

  // DomVer*
  if (Tess.DomVerQty < 4)
  {
    if (verbosity)
      ut_print_message (2, 4, "number of domvers = %d < 4\n", Tess.DomVerQty);
    return 1;
  }

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    if (Tess.DomVerEdgeQty[i] < Tess.Dim)
    {
      if (verbosity)
        ut_print_message (2, 4, "domver %d has  %d < %d domedges.\n", i,
                          Tess.DomVerEdgeQty[i], Tess.Dim);
      return 2;
    }

    for (j = 0; j < Tess.DomVerEdgeQty[i]; j++)
    {
      edge = Tess.DomVerEdgeNb[i][j];
      if (ut_array_1d_int_eltpos (Tess.DomEdgeVerNb[edge], 2, i) == -1)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "domver %d has domedge %d in its list, but domedge not based on domver.\n",
                            i, edge);

        return 3;
      }
    }
  }

  // DomEdge*
  if (Tess.DomEdgeQty < Tess.Dim + 1)
  {
    if (verbosity)
      ut_print_message (2, 4, "number of domedges = %d < %d\n",
                        Tess.DomEdgeQty, Tess.Dim + 1);
    return 4;
  }

  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    for (j = 0; j < 2; j++)
    {
      ver = Tess.DomEdgeVerNb[i][j];
      if (ut_array_1d_int_eltpos
          (Tess.DomVerEdgeNb[ver], Tess.DomVerEdgeQty[ver], i) == -1)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "domedge %d based on domver %d, but domedge not in domver domedge list.\n",
                            i, ver);
        return 5;
      }
    }

    if (Tess.Dim == 3)
      for (j = 0; j < Tess.Dim - 1; j++)
      {
        face = Tess.DomEdgeFaceNb[i][j];
        if (ut_array_1d_int_eltpos
            (Tess.DomFaceEdgeNb[face] + 1, Tess.DomFaceEdgeQty[face],
             i) == -1)
        {
          if (verbosity)
            ut_print_message (2, 4,
                              "domedge %d has domface %d in its domface list, but domface not based on domedge.\n",
                              i, face);

          return 6;
        }
      }
  }

  // DomFace*
  if (Tess.Dim == 3 && Tess.DomFaceQty < 4)
  {
    if (verbosity)
      ut_print_message (2, 4, "number of domface = %d < 6\n",
                        Tess.DomFaceQty);

    return 7;
  }

  for (i = 1; i <= Tess.DomFaceQty; i++)
  {
    if (Tess.DomFaceVerQty[i] < 0)
    {
      if (verbosity)
        ut_print_message (2, 4, "domface %d has %d < 0 vertices\n", i,
                          Tess.DomFaceVerQty[i]);

      return 8;
    }

    if (Tess.DomFaceEdgeQty[i] < 0)
    {
      if (verbosity)
        ut_print_message (2, 4, "domface %d has %d < 0 edges\n", i,
                          Tess.DomFaceEdgeQty[i]);

      return 8;
    }

    for (j = 1; j <= Tess.DomFaceVerQty[i]; j++)
    {
      ver = Tess.DomFaceVerNb[i][j];
      int qty;
      int *tmp = NULL;
      neut_tess_domver_domface (Tess, ver, &tmp, &qty);
      if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "domface %d based on domver %d, but domface not found in domver domfaces.\n",
                            i, ver);

        ut_free_1d_int (&tmp);
        return 9;
      }
      ut_free_1d_int (&tmp);
    }

    for (j = 1; j <= Tess.DomFaceEdgeQty[i]; j++)
    {
      edge = Tess.DomFaceEdgeNb[i][j];

      if (ut_array_1d_int_eltpos (Tess.DomEdgeFaceNb[edge], 2, i) == -1)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "domface %d based on domedge %d, but domface not found in domedge domface list.\n",
                            i, edge);

        return 10;
      }
    }
  }

  return 0;
}

int
neut_tess_test_dom_tessver (struct TESS Tess, int verbosity)
{
  int i, ver, dver;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain vertices:\n");

  for (dver = 1; dver <= Tess.DomVerQty; dver++)
  {
    if (verbosity)
      ut_print_message (0, 3, "Checking domain vertex %d...\n", dver);

    ver = Tess.DomTessVerNb[dver];

    if (ver < 1 || ver > Tess.VerQty)
    {
      if (verbosity)
        ut_print_message (2, 4, "has ver %d, but ver < 1 or > verqty = %d.\n",
                          ver, Tess.VerQty);

      return 1;
    }

    if (Tess.VerDom[ver][0] != 0)
    {
      if (verbosity)
        ut_print_message (2, 4, "has ver %d, but ver not marked as domver.\n",
                          dver, ver);

      return 2;
    }

    if (Tess.VerState[ver] == -1)
    {
      if (verbosity)
        ut_print_message (2, 4, "has ver %d, but ver has state = -1.\n", dver,
                          ver);

      return 8;
    }

    if (Tess.VerDom[ver][1] != dver)
    {
      if (verbosity)
        ut_print_message (2, 4, "has ver %d, but ver %d has domver %d.\n",
                          dver, ver, ver, Tess.VerDom[ver][1]);

      return 3;
    }
  }

  if (verbosity)
    ut_print_message (0, 3,
                      "Checking vertex to domain vertex relations...\n");

  for (i = 1; i <= Tess.VerQty; i++)
  {

    if (Tess.VerState[i] != -1)
      if (Tess.VerDom[i][0] == 0)
      {
        dver = Tess.VerDom[i][1];

        if (dver < 1 || dver > Tess.DomVerQty)
        {
          if (verbosity)
            ut_print_message (2, 4,
                              "ver %d has domver %d, but domver does not exist (< 1 or > number of domvers = %d).\n",
                              i, dver, Tess.DomVerQty);
          return 4;
        }

        if (Tess.DomTessVerNb[dver] != i)
        {
          if (verbosity)
            ut_print_message (2, 4,
                              "ver %d has domver %d, but domver does not have ver (%d instead).\n",
                              i, dver, Tess.DomTessVerNb[dver]);
          return 5;
        }
      }
  }

  return 0;
}

int
neut_tess_test_dom_tessedge (struct TESS Tess, int verbosity)
{
  int i, j, edge, dedge;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain edges:\n");

  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    if (verbosity)
      ut_print_message (0, 3, "Checking domain edge %d...\n", i);

    if (Tess.DomTessEdgeQty[i] < 1)
    {
      if (verbosity)
        ut_print_message (2, 4, "has %d < 1 tessellation edge.\n",
                          Tess.DomTessEdgeQty[i]);

      return 1;
    }

    for (j = 1; j <= Tess.DomTessEdgeQty[i]; j++)
    {
      edge = Tess.DomTessEdgeNb[i][j];

      if (Tess.EdgeState[edge] == -1)
      {
        if (verbosity)
          ut_print_message (2, 4, "has edge %d, but edge state != -1.\n",
                            edge);

        return 6;
      }

      if (Tess.EdgeDom[edge][0] != 1)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "has edge %d, but edge not marked to be on a domedge.\n",
                            edge);

        return 2;
      }

      if (Tess.EdgeDom[edge][1] != i)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "has edge %d, but edge does not have domedge (%d instead).\n",
                            edge, Tess.EdgeDom[edge][1]);

        return 3;
      }
    }
  }

  if (verbosity)
    ut_print_message (0, 3, "Checking vertex to domain edge relations...\n");

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      if (Tess.VerDom[i][0] == 1)
      {
        dedge = Tess.VerDom[i][1];
        int *tmp = NULL;
        int qty;
        neut_tess_domedge_ver (Tess, dedge, &tmp, &qty);
        if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
        {
          if (verbosity)
            ut_print_message (2, 4,
                              "ver %d is marked to be on domain edge %d, but ver not found in the domedge vers.\n",
                              i, dedge);

          ut_free_1d_int (&tmp);
          return 4;
        }
        ut_free_1d_int (&tmp);
      }

  if (verbosity)
    ut_print_message (0, 3, "Checking edge to domain edge relations...\n");

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] != -1)
      if (Tess.EdgeDom[i][0] == 1)
      {
        dedge = Tess.EdgeDom[i][1];

        if (ut_array_1d_int_eltpos
            (Tess.DomTessEdgeNb[dedge] + 1, Tess.DomTessEdgeQty[dedge],
             i) == -1)
        {
          if (verbosity)
            ut_print_message (2, 4,
                              "edge %d has domedge %d, but domedge does not have edge (%d instead)\n",
                              i, dedge, Tess.DomTessVerNb[dedge]);

          return 5;
        }
      }

  return 0;
}

int
neut_tess_test_dom_tessface (struct TESS Tess, int verbosity)
{
  int i, j, face, dface;

  if (verbosity)
    ut_print_message (0, 2, "Checking domain faces:\n");

  for (i = 1; i <= Tess.DomFaceQty; i++)
  {
    if (verbosity)
      ut_print_message (0, 3, "Checking domain face %d...\n", i);

    if (Tess.DomTessFaceQty[i] < 1)
    {
      if (verbosity)
        ut_print_message (2, 4, "has %d < 1 tessellation face.\n",
                          Tess.DomTessFaceQty[i]);

      return 1;
    }

    for (j = 1; j <= Tess.DomTessFaceQty[i]; j++)
    {
      face = Tess.DomTessFaceNb[i][j];

      if (Tess.FaceState[face] == -1)
      {
        if (verbosity)
          ut_print_message (2, 4, "has face %d, but face has state != -1.\n",
                            Tess.FaceState[face]);

        return 8;
      }

      if (Tess.FaceDom[face][0] != 2)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "has face %d, but face not marked to be on a tessface.\n",
                            face);

        return 2;
      }

      if (Tess.FaceDom[face][1] != i)
      {
        if (verbosity)
          ut_print_message (2, 4,
                            "has face %d, but face does not have domface (%d instead).\n",
                            Tess.FaceDom[face][1]);

        return 3;
      }
    }
  }

  if (Tess.Dim == 3)
  {
    if (verbosity)
      ut_print_message (0, 3,
                        "Checking vertex to domain face relations...\n");

    for (i = 1; i <= Tess.VerQty; i++)
      if (Tess.VerState[i] != -1)
        if (Tess.VerDom[i][0] == 2)
        {
          dface = Tess.VerDom[i][1];
          int *tmp = NULL;
          int qty;
          neut_tess_domface_vers (Tess, dface, &tmp, &qty);
          if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
          {
            ut_free_1d_int (&tmp);

            if (verbosity)
              ut_print_message (2, 4,
                                "ver %d marked to belong to domface %d, but ver not found in domface vers.\n",
                                i, dface);

            return 4;
          }
          ut_free_1d_int (&tmp);
        }

    if (verbosity)
      ut_print_message (0, 3, "Checking edge to domain face relations...\n");

    for (i = 1; i <= Tess.EdgeQty; i++)
      if (Tess.EdgeState[i] != -1)
        if (Tess.EdgeDom[i][0] == 2)
        {
          dface = Tess.EdgeDom[i][1];
          int *tmp = NULL;
          int qty;
          neut_tess_domface_edges (Tess, dface, &tmp, &qty);
          if (ut_array_1d_int_eltpos (tmp, qty, i) == -1)
          {
            ut_free_1d_int (&tmp);

            if (verbosity)
              ut_print_message (2, 4,
                                "edge %d marked to belong to domface %d, but edge not found in domface edges.\n",
                                i, dface);

            return 5;
          }
          ut_free_1d_int (&tmp);
        }

    if (verbosity)
      ut_print_message (0, 3, "Checking face to domain face relations...\n");

    for (i = 1; i <= Tess.FaceQty; i++)
      if (Tess.FaceState[i] != -1)
        if (Tess.FaceDom[i][0] == 2)
        {
          dface = Tess.FaceDom[i][1];
          if (ut_array_1d_int_eltpos
              (Tess.DomTessFaceNb[dface] + 1, Tess.DomTessFaceQty[dface],
               i) == -1)
          {
            if (verbosity)
              ut_print_message (2, 4,
                                "face %d marked to belong to domface %d, but face not found in domface faces.\n",
                                i, dface);

            return 4;
          }
        }
  }

  return 0;
}
