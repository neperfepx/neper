/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_tess_.h"

void
nev_print_tess (FILE * file, struct PRINT Print, struct TESS Tess,
		struct TESSDATA TessData)
{
  int i, j, status, polyfaceqty = 0;
  double **coo = NULL;
  int *polylist = NULL;
  int *polyfacelist = NULL;
  int *facelist = NULL;
  int *edgelist = NULL;
  int *verlist = NULL;
  int *seedlist = NULL;
  int neighqty;
  int *neigh = NULL;
  int poly, allneigh, face, edge, ver, seed;
  char *texture = ut_alloc_1d_char (100);
  double trs;

  // Writing pov file --------------------------------------------------
  // nev_print_tess_header (file, TessData);

  seedlist = ut_alloc_1d_int (Tess.CellQty + 1);

  for (i = 1; i <= Tess.CellQty; i++)
    if (Print.showseed[i] == 1)
      seedlist[++seedlist[0]] = i;

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
    trs = ut_array_1d_max (TessData.trs[3] + 1, TessData.PolyQty);

    if (Print.showpoly[0] > 0)
      for (i = 1; i <= Tess.PolyQty; i++)
	if (Print.showpoly[i] == 1)
	{
	  neut_tess_poly_neighpoly (Tess, i, &neigh, &neighqty);

	  if (neighqty < Tess.PolyFaceQty[i])
	    polylist[++polylist[0]] = i;
	  else
	  {
	    allneigh = 1;
	    for (j = 0; j < neighqty; j++)
	      if (neut_tess_seed_iscell (Tess, neigh[j]) && Print.showpoly[neigh[j]] == 0)
	      {
		allneigh = 0;
		break;
	      }
	    if (allneigh == 0 || trs > 1e-6)
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
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;

  // Writing poly faces
  if (Tess.Dim == 3)
    for (i = 0; i < polyfaceqty; i++)
    {
      face = polyfacelist[i];

      int *Col = ut_alloc_1d_int (3);
      double trs = 0;
      int qty = 0;
      for (j = 0; j < 2; j++)
      {
	poly = Tess.FacePoly[face][j];
	if (neut_tess_seed_iscell (Tess, poly) && Print.showpoly[poly])
	{
	  qty++;
	  ut_array_1d_int_add (Col, TessData.Col[3][poly], 3, Col);
	  trs += TessData.trs[3][poly];
	}
      }
      ut_array_1d_int_scale (Col, 3, 1. / qty);
      trs /= qty;

      fprintf (file,
	       "#declare grainface%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
	       i, Col[0] / 255., Col[1] / 255., Col[2] / 255., trs, ambient);

      ut_free_1d_int (Col);

      sprintf (texture, "grainface%d", i);

      // vertex coordinates
      coo = ut_alloc_2d (Tess.FaceVerQty[face], 3);

      for (j = 1; j <= Tess.FaceVerQty[face]; j++)
	ut_array_1d_memcpy (coo[j - 1], 3,
			    Tess.VerCoo[Tess.FaceVerNb[face][j]]);

      fprintf (file, "// poly face %d\n", face);

      nev_print_polygon (file, Tess.FaceVerQty[face], coo, texture,
			 NULL, NULL, Tess.FacePt[face] - 1, p2, NULL, NULL);

      ut_free_2d (coo, Tess.FaceVerQty[face]);
    }

  // Writing faces
  for (i = 1; i <= facelist[0]; i++)
  {
    face = facelist[i];

    fprintf (file,
	     "#declare face%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
	     face,
	     TessData.Col[2][face][0] / 255.,
	     TessData.Col[2][face][1] / 255.,
	     TessData.Col[2][face][2] / 255., TessData.trs[2][face], ambient);

    sprintf (texture, "face%d", face);

    // vertex coordinates
    coo = ut_alloc_2d (Tess.FaceVerQty[face], 3);

    for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      ut_array_1d_memcpy (coo[j - 1], 3,
			  Tess.VerCoo[Tess.FaceVerNb[face][j]]);

    // interpolation point
    if (Print.showfaceinter == 1 && Tess.FaceState[face] > 0)
    {
      if (Tess.FacePt[face] == 0)
	neut_tess_face_centre (Tess, face, p);
      else if (Tess.FacePt[face] > 0)
	ut_array_1d_memcpy (p, 3,
			    Tess.VerCoo[Tess.
					FaceVerNb[face][Tess.FacePt[face]]]);
      p2 = p;
    }
    else
      p2 = NULL;

    fprintf (file, "// face %d\n", face);

    nev_print_polygon (file, Tess.FaceVerQty[face], coo, texture,
		       NULL, NULL, Tess.FacePt[face] - 1, p2, NULL, NULL);

    if (Print.showfaceinter == 1 && neut_tess_face_ff (Tess, face) > 0)
    {
      // defining faceinter_rad as the average of the radii of the face
      // edges
      double faceinter_rad = 0;
      double faceinter_trs = 0;
      for (j = 1; j <= Tess.FaceVerQty[face]; j++)
      {
	faceinter_rad += TessData.Rad[1][Tess.FaceEdgeNb[face][j]];
	faceinter_trs += TessData.trs[1][Tess.FaceEdgeNb[face][j]];
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
	  nev_print_segment (file, p, coo[j], "faceinter_rad",
			     "faceinter_texture");
	else if (Tess.FacePt[face] != j + 1
		 && Tess.FacePt[face] != ut_num_rotpos (1,
							Tess.FaceVerQty[face],
							j + 1, 1)
		 && Tess.FacePt[face] != ut_num_rotpos (1,
							Tess.FaceVerQty[face],
							j + 1, -1))
	  nev_print_segment (file, p, coo[j], "faceinter_rad",
			     "faceinter_texture");
      }
    }

    ut_free_2d (coo, Tess.FaceVerQty[face]);
  }

  ut_free_1d (p);

  // Writing edges
  for (i = 1; i <= edgelist[0]; i++)
  {
    edge = edgelist[i];

    fprintf (file,
	     "#declare edge%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
	     edge,
	     TessData.Col[1][edge][0] / 255.,
	     TessData.Col[1][edge][1] / 255.,
	     TessData.Col[1][edge][2] / 255., TessData.trs[1][edge], ambient);

    sprintf (texture, "edge%d", edge);

    coo = ut_alloc_2d (2, 3);

    ut_array_1d_memcpy (coo[0], 3, Tess.VerCoo[Tess.EdgeVerNb[edge][0]]);
    ut_array_1d_memcpy (coo[1], 3, Tess.VerCoo[Tess.EdgeVerNb[edge][1]]);

    char *string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", TessData.Rad[1][edge]);
    nev_print_segment_wsph (file, coo[0], coo[1], string, texture);
    ut_free_1d_char (string);

    ut_free_2d (coo, 2);
  }

  // Writing vers
  for (i = 1; i <= verlist[0]; i++)
  {
    ver = verlist[i];

    fprintf (file,
	     "#declare ver%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
	     ver,
	     TessData.Col[0][ver][0] / 255.,
	     TessData.Col[0][ver][1] / 255.,
	     TessData.Col[0][ver][2] / 255., TessData.trs[0][ver], ambient);

    sprintf (texture, "ver%d", ver);

    char *string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", TessData.Rad[0][ver]);
    nev_print_sphere (file, Tess.VerCoo[ver], string, texture);
    ut_free_1d_char (string);
  }

  // Writing seeds
  for (i = 1; i <= seedlist[0]; i++)
  {
    seed = seedlist[i];

    fprintf (file,
	     "#declare seed%d =\n  texture { pigment { rgbt <%f,%f,%f,%f> } finish {ambient %f} }\n",
	     seed,
	     TessData.Col[4][seed][0] / 255.,
	     TessData.Col[4][seed][1] / 255.,
	     TessData.Col[4][seed][2] / 255., TessData.trs[4][seed], ambient);

    sprintf (texture, "seed%d", seed);

    char *string = ut_alloc_1d_char (100);
    sprintf (string, "%.12f", TessData.Rad[4][seed]);
    nev_print_sphere (file, Tess.SeedCoo[seed], string, texture);
    ut_free_1d_char (string);
  }

  ut_free_1d_int (polylist);
  ut_free_1d_int (polyfacelist);
  ut_free_1d_int (facelist);
  ut_free_1d_int (edgelist);
  ut_free_1d_int (verlist);
  ut_free_1d_int (seedlist);
  ut_free_1d_int (neigh);
  ut_free_1d_char (texture);

  return;
}
