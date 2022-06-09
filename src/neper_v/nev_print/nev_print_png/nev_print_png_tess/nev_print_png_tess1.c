/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_tess_.h"

void
nev_print_png_tess (FILE * file, struct PRINT Print, struct TESS Tess,
                struct DATA *TessData)
{
  int i, j, status, polyfaceqty = 0;
  double **coo = NULL;
  int *polylist = NULL;
  int *polyfacelist = NULL;
  int *facelist = NULL;
  int *edgelist = NULL;
  int *verlist = NULL;
  int *seedlist = NULL;
  int *crystallist = NULL;
  int neighqty;
  int *neigh = NULL;
  int poly, allneigh, face, edge, ver, seed, crystal;
  char *texture = ut_alloc_1d_char (100);
  double Trs;

  // Writing pov file --------------------------------------------------
  // nev_print_png_tess_header (file, TessData);

  seedlist = ut_alloc_1d_int (Tess.CellQty + 1);

  for (i = 1; i <= Tess.CellQty; i++)
    if (Print.showseed[i] == 1)
      seedlist[++seedlist[0]] = i;

  crystallist = ut_alloc_1d_int (Tess.CellQty + 1);

  for (i = 1; i <= Tess.CellQty; i++)
    if (Print.showcrystal[i] == 1)
      crystallist[++crystallist[0]] = i;

  verlist = ut_alloc_1d_int (Tess.VerQty + 1);

  for (i = 1; i <= Tess.VerQty; i++)
    if (Print.showver[i] == 1)
      verlist[++verlist[0]] = i;

  edgelist = ut_alloc_1d_int (Tess.EdgeQty + 1);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Print.showedge[i] == 1)
      edgelist[++edgelist[0]] = i;

  facelist = ut_alloc_1d_int (Tess.FaceQty + 1);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Print.showface[i] == 1)
      facelist[++facelist[0]] = i;

  // looking for the poly faces to print
  if (Tess.Dim == 3)
  {
    polylist = ut_alloc_1d_int (Tess.PolyQty + 1);
    Trs = ut_array_1d_max (TessData[3].Trs + 1, TessData[3].Qty);

    if (Print.showpoly[0] > 0)
      for (i = 1; i <= Tess.PolyQty; i++)
        if (Print.showpoly[i] == 1)
        {
          neut_tess_poly_neighpolys (Tess, i, &neigh, &neighqty);

          if (neighqty < Tess.PolyFaceQty[i])
            polylist[++polylist[0]] = i;
          else
          {
            allneigh = 1;
            for (j = 0; j < neighqty; j++)
              if (neut_tess_seed_iscell (Tess, neigh[j])
                  && Print.showpoly[neigh[j]] == 0)
              {
                allneigh = 0;
                break;
              }
            if (allneigh == 0 || Trs > 1e-6)
              polylist[++polylist[0]] = i;
          }
        }

    // from the polylist, determining the face list
    polyfaceqty = 0;
    polyfacelist = ut_alloc_1d_int (Tess.FaceQty + 1);

    for (i = 1; i <= Tess.FaceQty; i++)
    {
      status = 0;
      for (j = 0; j < 2; j++)
        if (neut_tess_seed_iscell (Tess, Tess.FacePoly[i][j])
            && Print.showpoly[Tess.FacePoly[i][j]])
          status = 1;

      if (status && !Print.showface[i])
        polyfacelist[polyfaceqty++] = i;
    }
  }

  double *p = ut_alloc_1d (3);
  double *p2 = NULL;
  double ambient = (Print.sceneshadow == 1) ? 0.6 : 1;

  // Writing poly faces
  if (Tess.Dim == 3)
    for (i = 0; i < polyfaceqty; i++)
    {
      face = polyfacelist[i];

      int *Col = ut_alloc_1d_int (3);
      double Trs = 0;
      int qty = 0;
      for (j = 0; j < 2; j++)
      {
        poly = Tess.FacePoly[face][j];
        if (neut_tess_seed_iscell (Tess, poly) && Print.showpoly[poly])
        {
          qty++;
          ut_array_1d_int_add (Col, TessData[3].Col[poly], 3, Col);
          Trs += TessData[3].Trs[poly];
        }
      }
      ut_array_1d_int_scale (Col, 3, 1. / qty);
      Trs /= qty;

      fprintf (file,
               "#declare grainface%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
               i, Col[0] / 255., Col[1] / 255., Col[2] / 255., Trs, ambient);

      ut_free_1d_int (&Col);

      sprintf (texture, "grainface%d", i);

      // vertex coordinates
      coo = ut_alloc_2d (Tess.FaceVerQty[face], 3);

      for (j = 1; j <= Tess.FaceVerQty[face]; j++)
        ut_array_1d_memcpy (Tess.VerCoo[Tess.FaceVerNb[face][j]], 3,
                            coo[j - 1]);

      fprintf (file, "// poly face %d\n", face);

      nev_print_png_polygon (file, Tess.FaceEq[face], Tess.FaceVerQty[face], coo,
                         texture, NULL, NULL, Tess.FacePt[face] - 1, p2, NULL,
                         NULL);

      ut_free_2d (&coo, Tess.FaceVerQty[face]);
    }

  // Writing faces
  for (i = 1; i <= facelist[0]; i++)
  {
    face = facelist[i];

    fprintf (file,
             "#declare face%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
             face, TessData[2].Col[face][0] / 255.,
             TessData[2].Col[face][1] / 255., TessData[2].Col[face][2] / 255.,
             TessData[2].Trs[face], ambient);

    sprintf (texture, "face%d", face);

    // vertex coordinates
    coo = ut_alloc_2d (Tess.FaceVerQty[face], 3);

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      ut_array_1d_memcpy (Tess.VerCoo[Tess.FaceVerNb[face][j]], 3,
                          coo[j - 1]);

    // interpolation point
    if (Print.showfaceinter == 1 && Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
        neut_tess_face_centre (Tess, face, p);
      else if (Tess.FacePt[face] > 0)
        ut_array_1d_memcpy (Tess.
                            VerCoo[Tess.FaceVerNb[face][Tess.FacePt[face]]],
                            3, p);
      p2 = p;
    }
    else
      p2 = NULL;

    fprintf (file, "// face %d\n", face);

    nev_print_png_polygon (file, Tess.FaceEq[face], Tess.FaceVerQty[face], coo,
                       texture, NULL, NULL, Tess.FacePt[face] - 1, p2, NULL,
                       NULL);

    if (Print.showfaceinter == 1 && neut_tess_face_ff (Tess, face) > 0)
    {
      // defining faceinter_rad as the average of the radii of the face
      // edges
      double faceinter_rad = 0;
      double faceinter_trs = 0;
      for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      {
        faceinter_rad += TessData[1].Rad[Tess.FaceEdgeNb[face][j]];
        faceinter_trs += TessData[1].Trs[Tess.FaceEdgeNb[face][j]];
      }
      faceinter_rad /= Tess.FaceVerQty[face];
      faceinter_trs /= Tess.FaceVerQty[face];

      fprintf (file, "#declare faceinter_rad = %.12f;\n", faceinter_rad);
      fprintf (file,
               "#declare faceinter_texture = texture { pigment { rgbt <0.5,0.5,0.5,%f> } finish {ambient %f} }\n",
               faceinter_trs, ambient);

      // not printing the sphere at both extremities because they would
      // overlap with the regular edge spheres, which leads to bad
      // rendering
      for (j = 0; j < Tess.FaceVerQty[face]; j++)
      {
        if (Tess.FacePt[face] <= 0)
          nev_print_png_segment (file, p, coo[j], "faceinter_rad",
                             "faceinter_texture");
        else if (Tess.FacePt[face] != j + 1
                 && Tess.FacePt[face] != ut_array_rotpos (1,
                                                          Tess.
                                                          FaceVerQty[face],
                                                          j + 1, 1)
                 && Tess.FacePt[face] != ut_array_rotpos (1,
                                                          Tess.
                                                          FaceVerQty[face],
                                                          j + 1, -1))
          nev_print_png_segment (file, p, coo[j], "faceinter_rad",
                             "faceinter_texture");
      }
    }

    ut_free_2d (&coo, Tess.FaceVerQty[face]);
  }

  ut_free_1d (&p);

  // Writing edges
  for (i = 1; i <= edgelist[0]; i++)
  {
    edge = edgelist[i];

    fprintf (file,
             "#declare edge%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
             edge, TessData[1].Col[edge][0] / 255.,
             TessData[1].Col[edge][1] / 255., TessData[1].Col[edge][2] / 255.,
             TessData[1].Trs[edge], ambient);

    sprintf (texture, "edge%d", edge);

    coo = ut_alloc_2d (2, 3);

    ut_array_1d_memcpy (Tess.VerCoo[Tess.EdgeVerNb[edge][0]], 3, coo[0]);
    ut_array_1d_memcpy (Tess.VerCoo[Tess.EdgeVerNb[edge][1]], 3, coo[1]);

    char *string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", TessData[1].Rad[edge]);
    nev_print_png_segment_wsph (file, coo[0], coo[1], string, texture);
    ut_free_1d_char (&string);

    ut_free_2d (&coo, 2);
  }

  // Writing vers
  for (i = 1; i <= verlist[0]; i++)
  {
    ver = verlist[i];

    fprintf (file,
             "#declare ver%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
             ver, TessData[0].Col[ver][0] / 255.,
             TessData[0].Col[ver][1] / 255., TessData[0].Col[ver][2] / 255.,
             TessData[0].Trs[ver], ambient);

    sprintf (texture, "ver%d", ver);

    char *string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", TessData[0].Rad[ver]);
    nev_print_png_sphere (file, Tess.VerCoo[ver], string, texture);
    ut_free_1d_char (&string);
  }

  // Writing seeds
  for (i = 1; i <= seedlist[0]; i++)
  {
    seed = seedlist[i];

    fprintf (file,
             "#declare seed%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
             seed, TessData[4].Col[seed][0] / 255.,
             TessData[4].Col[seed][1] / 255., TessData[4].Col[seed][2] / 255.,
             TessData[4].Trs[seed], ambient);

    sprintf (texture, "seed%d", seed);

    char *string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", TessData[4].Rad[seed]);
    nev_print_png_sphere (file, Tess.SeedCoo[seed], string, texture);
    ut_free_1d_char (&string);
  }

  // Writing crystals
  for (i = 1; i <= crystallist[0]; i++)
  {
    char *bordertexture = ut_alloc_1d_char (100);
    crystal = crystallist[i];

    fprintf (file,
             "#declare crystal%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
             crystal, TessData[5].Col[crystal][0] / 255.,
             TessData[5].Col[crystal][1] / 255., TessData[5].Col[crystal][2] / 255.,
             TessData[5].Trs[crystal], ambient);

    sprintf (texture, "crystal%d", crystal);

    if (TessData[5].BRad > 0)
      fprintf (file,
               "#declare crystal%dedge =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
               crystal, TessData[5].BCol[0] / 255.,
               TessData[5].BCol[1] / 255., TessData[5].BCol[2] / 255.,
               TessData[5].BRad, ambient);

    sprintf (bordertexture, "crystal%dedge", crystal);

    char *string = ut_alloc_1d_char (100);
    double *coo = ut_alloc_1d (3);
    double *rad = ut_alloc_1d (10);
    double **g = ol_g_alloc ();

    neut_tess_cell_centroid (Tess, crystal, coo);
    ol_q_g (Tess.CellOri[crystal], g);
    for (j = 0; j < 3; j++)
      ut_array_1d_memcpy (g[j], 3, rad + 1 + 3 * j);

    if (!strcmp (Tess.CellCrySym, "cubic"))
    {
      rad[0] = .80599597700823431925 * TessData[5].Rad[crystal];
      nev_print_png_cube (file, coo, rad, texture, TessData[5].BRad, bordertexture);
    }
    else if (!strcmp (Tess.CellCrySym, "hexagonal"))
    {
      // .99575107876767219668 = pow (4 * M_PI / (9 * sqrt (2)), 1. / 3)
      rad[0] = 0.99575107876767219668 * TessData[5].Rad[crystal]; // inner radius
      nev_print_png_hcp (file, coo, rad, texture, TessData[5].BRad, bordertexture);
    }
    else
    {
      rad[0] = TessData[5].Rad[crystal];
      char *string = ut_alloc_1d_char (100);
      sprintf (string, "%.12f", TessData[5].Rad[crystal]);
      nev_print_png_sphere (file, coo, string, texture);
      ut_free_1d_char (&string);
    }
    ut_free_1d_char (&string);
    ut_free_1d (&coo);
    ut_free_1d (&rad);
    ol_g_free (g);
    ut_free_1d_char (&bordertexture);
  }

  ut_free_1d_int (&polylist);
  ut_free_1d_int (&polyfacelist);
  ut_free_1d_int (&facelist);
  ut_free_1d_int (&edgelist);
  ut_free_1d_int (&verlist);
  ut_free_1d_int (&seedlist);
  ut_free_1d_int (&crystallist);
  ut_free_1d_int (&neigh);
  ut_free_1d_char (&texture);

  return;
}
