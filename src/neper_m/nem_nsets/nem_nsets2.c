/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets_.h"

void
nem_nsets_2d_tess (struct TESS Tess, struct MESH *Mesh, struct NSET *NSet)
{
  int i;

  NSet[2].qty = Tess.DomFaceQty;
  NSet[2].names = ut_alloc_1d_pchar (NSet[2].qty + 1);

  // setting up face nset labels
  for (i = 1; i <= NSet[2].qty; i++)
    ut_string_string (Tess.DomFaceLabel[i], NSet[2].names + i);

  // Computing nsets
  NSet[2].nodeqty = ut_alloc_1d_int (Tess.DomFaceQty + 1);
  NSet[2].nodes = ut_alloc_1d_pint (Tess.DomFaceQty + 1);

  for (i = 1; i <= Tess.DomFaceQty; i++)
    neut_mesh_elsets_nodes (Mesh[2], Tess.DomTessFaceNb[i] + 1,
			    Tess.DomTessFaceQty[i], NSet[2].nodes + i,
			    NSet[2].nodeqty + i);

  return;
}

void
nem_nsets_1d_tess (struct TESS Tess, struct MESH *Mesh, struct NSET *NSet)
{
  int i;

  // Edges
  NSet[1].qty = Tess.DomEdgeQty;
  NSet[1].names = ut_alloc_1d_pchar (Tess.DomEdgeQty + 1);
  NSet[1].nodeqty = ut_alloc_1d_int (Tess.DomEdgeQty + 1);
  NSet[1].nodes = ut_alloc_1d_pint (Tess.DomEdgeQty + 1);

  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    ut_string_string (Tess.DomEdgeLabel[i], NSet[1].names + i);

    neut_mesh_elsets_nodes (Mesh[1], Tess.DomTessEdgeNb[i] + 1,
			    Tess.DomTessEdgeQty[i], NSet[1].nodes + i,
			    NSet[1].nodeqty + i);
  }

  return;
}

void
nem_nsets_0d_tess (struct TESS Tess, struct MESH *Mesh, struct NSET *NSet)
{
  int i;

  // Vertices
  NSet[0].qty = Tess.DomVerQty;
  NSet[0].names = ut_alloc_1d_pchar (Tess.DomVerQty + 1);
  NSet[0].nodeqty = ut_alloc_1d_int (Tess.DomVerQty + 1);
  NSet[0].nodes = ut_alloc_1d_pint (Tess.DomVerQty + 1);

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    ut_string_string (Tess.DomVerLabel[i], NSet[0].names + i);

    if (Tess.DomTessVerNb[i] > 0)
      neut_mesh_elset_nodes (Mesh[0], Tess.DomTessVerNb[i],
			     NSet[0].nodes + i, NSet[0].nodeqty + i);
  }

  return;
}

void
nem_nsets_2dbody_tess (struct TESS Tess, struct NSET *NSet)
{
  int i, j, k, edge;
  int ref, prevqty = NSet[2].qty;

  NSet[2].qty *= 2;
  NSet[2].names = ut_realloc_1d_pchar (NSet[2].names, NSet[2].qty + 1);
  NSet[2].nodeqty = ut_realloc_1d_int (NSet[2].nodeqty, NSet[2].qty + 1);
  NSet[2].nodes = ut_realloc_1d_pint (NSet[2].nodes, NSet[2].qty + 1);

  for (i = prevqty + 1; i <= NSet[2].qty; i++)
  {
    ref = i - prevqty;

    NSet[2].names[i] = ut_alloc_1d_char (strlen (NSet[2].names[ref]) + 5);
    sprintf (NSet[2].names[i], "%sbody", NSet[2].names[ref]);

    NSet[2].nodeqty[i] = NSet[2].nodeqty[ref];
    NSet[2].nodes[i] = ut_alloc_1d_int (NSet[2].nodeqty[i]);
    ut_array_1d_int_memcpy (NSet[2].nodes[i], NSet[2].nodeqty[i],
			    NSet[2].nodes[ref]);

    for (j = 1; j <= Tess.DomFaceEdgeQty[ref]; j++)
    {
      edge = Tess.DomFaceEdgeNb[ref][j];

      for (k = 0; k < NSet[1].nodeqty[edge]; k++)
	NSet[2].nodeqty[i] -=
	  ut_array_1d_int_deletencompress (NSet[2].nodes[i],
					   NSet[2].nodeqty[i],
					   NSet[1].nodes[edge][k], 1);
    }

    NSet[2].nodes[i] =
      ut_realloc_1d_int (NSet[2].nodes[i], NSet[2].nodeqty[i]);
  }

  return;
}

void
nem_nsets_1dbody_tess (struct TESS Tess, struct NSET *NSet)
{
  int i, j, k, ver;
  int ref, prevqty = NSet[1].qty;

  NSet[1].qty *= 2;
  NSet[1].names = ut_realloc_1d_pchar (NSet[1].names, NSet[1].qty + 1);
  NSet[1].nodeqty = ut_realloc_1d_int (NSet[1].nodeqty, NSet[1].qty + 1);
  NSet[1].nodes = ut_realloc_1d_pint (NSet[1].nodes, NSet[1].qty + 1);

  for (i = prevqty + 1; i <= NSet[1].qty; i++)
  {
    ref = i - prevqty;

    NSet[1].names[i] = ut_alloc_1d_char (strlen (NSet[1].names[ref]) + 5);
    sprintf (NSet[1].names[i], "%sbody", NSet[1].names[ref]);

    NSet[1].nodeqty[i] = NSet[1].nodeqty[ref];
    NSet[1].nodes[i] = ut_alloc_1d_int (NSet[1].nodeqty[i]);
    ut_array_1d_int_memcpy (NSet[1].nodes[i], NSet[1].nodeqty[i],
			    NSet[1].nodes[ref]);

    for (j = 0; j < 2; j++)
    {
      ver = Tess.DomEdgeVerNb[ref][j];

      for (k = 0; k < NSet[0].nodeqty[ver]; k++)
	NSet[1].nodeqty[i] -=
	  ut_array_1d_int_deletencompress (NSet[1].nodes[i],
					   NSet[1].nodeqty[i],
					   NSet[0].nodes[ver][k], 1);
    }

    NSet[1].nodes[i] =
      ut_realloc_1d_int (NSet[1].nodes[i], NSet[1].nodeqty[i]);
  }

  return;
}

void
nem_nsets_1d_tess_str (struct TESS Tess, struct NSET *NSet)
{
  int i;

  NSet[1].qty = Tess.DomEdgeQty;
  NSet[1].names = ut_alloc_1d_pchar (NSet[1].qty + 1);
  NSet[1].nodeqty = ut_alloc_1d_int (NSet[1].qty + 1);
  NSet[1].nodes = ut_alloc_1d_pint (NSet[1].qty + 1);

  for (i = 1; i <= Tess.DomEdgeQty; i++)
    neut_nsets_inter (NSet[2], Tess.DomEdgeFaceNb[i][0],
		      Tess.DomEdgeFaceNb[i][1], &(NSet[1].names[i]),
		      &(NSet[1].nodes[i]), &(NSet[1].nodeqty[i]));

  return;
}

void
nem_nsets_0d_tess_str (struct TESS Tess, struct NSET *NSet)
{
  int i;

  NSet[0].qty = Tess.DomVerQty;
  NSet[0].names = ut_alloc_1d_pchar (NSet[0].qty + 1);
  NSet[0].nodeqty = ut_alloc_1d_int (NSet[0].qty + 1);
  NSet[0].nodes = ut_alloc_1d_pint (NSet[0].qty + 1);

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    ut_string_string (Tess.DomEdgeLabel[i], NSet[0].names + i);

    neut_nsets_inter (NSet[1], Tess.DomVerEdgeNb[i][0],
		      Tess.DomVerEdgeNb[i][1], NULL, &(NSet[0].nodes[i]),
		      &(NSet[0].nodeqty[i]));
  }

  return;
}

void
nem_nsets_1d_str (struct NSET *NSet)
{
  int i;
  char **edgelabel = NULL;
  int **edgeface = NULL;

  if (NSet[2].qty == 0)
  {
    NSet[1].qty = 0;
    return;
  }

  edgelabel = ut_alloc_2d_char (13, 7);
  edgeface = ut_alloc_2d_int (13, 2);

  strcpy (edgelabel[1], "x0y0");
  edgeface[1][0] = 1;
  edgeface[1][1] = 3;
  strcpy (edgelabel[2], "x1y0");
  edgeface[2][0] = 2;
  edgeface[2][1] = 3;
  strcpy (edgelabel[3], "x0y1");
  edgeface[3][0] = 1;
  edgeface[3][1] = 4;
  strcpy (edgelabel[4], "x1y1");
  edgeface[4][0] = 2;
  edgeface[4][1] = 4;
  strcpy (edgelabel[5], "y0z0");
  edgeface[5][0] = 3;
  edgeface[5][1] = 5;
  strcpy (edgelabel[6], "y1z0");
  edgeface[6][0] = 4;
  edgeface[6][1] = 5;
  strcpy (edgelabel[7], "y0z1");
  edgeface[7][0] = 3;
  edgeface[7][1] = 6;
  strcpy (edgelabel[8], "y1z1");
  edgeface[8][0] = 4;
  edgeface[8][1] = 6;
  strcpy (edgelabel[9], "x0z0");
  edgeface[9][0] = 1;
  edgeface[9][1] = 5;
  strcpy (edgelabel[10], "x1z0");
  edgeface[10][0] = 2;
  edgeface[10][1] = 5;
  strcpy (edgelabel[11], "x0z1");
  edgeface[11][0] = 1;
  edgeface[11][1] = 6;
  strcpy (edgelabel[12], "x1z1");
  edgeface[12][0] = 2;
  edgeface[12][1] = 6;

  NSet[1].qty = 12;
  NSet[1].names = ut_alloc_1d_pchar (13);
  NSet[1].nodeqty = ut_alloc_1d_int (13);
  NSet[1].nodes = ut_alloc_1d_pint (13);

  for (i = 1; i <= 12; i++)
  {
    neut_nsets_inter (NSet[2], edgeface[i][0], edgeface[i][1],
		      &(NSet[1].names[i]),
		      &(NSet[1].nodes[i]), &(NSet[1].nodeqty[i]));
  }

  ut_free_2d_char (edgelabel, 13);
  ut_free_2d_int (edgeface, 13);

  return;
}

void
nem_nsets_0d_str (struct NSET *NSet)
{
  int i;
  char **verlabel = NULL;
  int **veredge = NULL;

  if (NSet[1].qty == 0)
  {
    NSet[0].qty = 0;
    return;
  }

  if (NSet[1].qty == 12)
  {
    verlabel = ut_alloc_2d_char (9, 7);
    veredge = ut_alloc_2d_int (9, 2);

    strcpy (verlabel[1], "x0y0z0");
    veredge[1][0] = 1;
    veredge[1][1] = 5;
    strcpy (verlabel[2], "x1y0z0");
    veredge[2][0] = 2;
    veredge[2][1] = 5;
    strcpy (verlabel[3], "x0y1z0");
    veredge[3][0] = 3;
    veredge[3][1] = 6;
    strcpy (verlabel[4], "x1y1z0");
    veredge[4][0] = 4;
    veredge[4][1] = 6;
    strcpy (verlabel[5], "x0y0z1");
    veredge[5][0] = 1;
    veredge[5][1] = 7;
    strcpy (verlabel[6], "x1y0z1");
    veredge[6][0] = 2;
    veredge[6][1] = 7;
    strcpy (verlabel[7], "x0y1z1");
    veredge[7][0] = 3;
    veredge[7][1] = 8;
    strcpy (verlabel[8], "x1y1z1");
    veredge[8][0] = 4;
    veredge[8][1] = 8;

    NSet[0].qty = 8;
    NSet[0].names = ut_alloc_2d_char (9, 7);
    NSet[0].nodeqty = ut_alloc_1d_int (9);
    NSet[0].nodes = ut_alloc_1d_pint (9);

    for (i = 1; i <= 8; i++)
    {
      neut_nsets_inter (NSet[1], veredge[i][0], veredge[i][1], NULL,
			&(NSet[0].nodes[i]), &(NSet[0].nodeqty[i]));
      strcpy (NSet[0].names[i], verlabel[i]);
    }

    ut_free_2d_char (verlabel, 9);
    ut_free_2d_int (veredge, 9);
  }

  else if (NSet[1].qty == 4)
  {
    verlabel = ut_alloc_2d_char (5, 7);
    veredge = ut_alloc_2d_int (5, 2);

    strcpy (verlabel[1], "x0y0");
    veredge[1][0] = 1;
    veredge[1][1] = 3;
    strcpy (verlabel[2], "x1y0");
    veredge[2][0] = 2;
    veredge[2][1] = 3;
    strcpy (verlabel[3], "x0y1");
    veredge[3][0] = 1;
    veredge[3][1] = 4;
    strcpy (verlabel[4], "x1y1");
    veredge[4][0] = 2;
    veredge[4][1] = 4;

    NSet[0].qty = 4;
    NSet[0].names = ut_alloc_2d_char (NSet[0].qty + 1, 7);
    NSet[0].nodeqty = ut_alloc_1d_int (NSet[0].qty + 1);
    NSet[0].nodes = ut_alloc_1d_pint (NSet[0].qty + 1);

    for (i = 1; i <= NSet[0].qty; i++)
    {
      neut_nsets_inter (NSet[1], veredge[i][0], veredge[i][1], NULL,
			&(NSet[0].nodes[i]), &(NSet[0].nodeqty[i]));
      strcpy (NSet[0].names[i], verlabel[i]);
    }

    ut_free_2d_char (verlabel, NSet[0].qty + 1);
    ut_free_2d_int (veredge, NSet[0].qty + 1);
  }

  return;
}

void
nem_nsets_2dbody_str (struct NSET *NSet)
{
  int i, j, k, edge;
  int ref, prevqty = NSet[2].qty;
  int **faceedge = ut_alloc_2d_int (7, 4);

  faceedge[1][0] = 1;
  faceedge[1][1] = 3;
  faceedge[1][2] = 9;
  faceedge[1][3] = 11;
  faceedge[2][0] = 2;
  faceedge[2][1] = 4;
  faceedge[2][2] = 10;
  faceedge[2][3] = 12;
  faceedge[3][0] = 1;
  faceedge[3][1] = 2;
  faceedge[3][2] = 5;
  faceedge[3][3] = 7;
  faceedge[4][0] = 3;
  faceedge[4][1] = 4;
  faceedge[4][2] = 6;
  faceedge[4][3] = 8;
  faceedge[5][0] = 5;
  faceedge[5][1] = 6;
  faceedge[5][2] = 9;
  faceedge[5][3] = 10;
  faceedge[6][0] = 7;
  faceedge[6][1] = 8;
  faceedge[6][2] = 11;
  faceedge[6][3] = 12;

  NSet[2].qty *= 2;
  NSet[2].names = ut_realloc_1d_pchar (NSet[2].names, NSet[2].qty + 1);
  NSet[2].nodeqty = ut_realloc_1d_int (NSet[2].nodeqty, NSet[2].qty + 1);
  NSet[2].nodes = ut_realloc_1d_pint (NSet[2].nodes, NSet[2].qty + 1);

  for (i = prevqty + 1; i <= NSet[2].qty; i++)
  {
    ref = i - prevqty;

    NSet[2].names[i] = ut_alloc_1d_char (strlen (NSet[2].names[ref]) + 5);
    sprintf (NSet[2].names[i], "%sbody", NSet[2].names[ref]);

    NSet[2].nodeqty[i] = NSet[2].nodeqty[ref];
    NSet[2].nodes[i] = ut_alloc_1d_int (NSet[2].nodeqty[i]);
    ut_array_1d_int_memcpy (NSet[2].nodes[i], NSet[2].nodeqty[i],
			    NSet[2].nodes[ref]);

    for (j = 0; j < 4; j++)
    {
      edge = faceedge[ref][j];

      for (k = 0; k < NSet[1].nodeqty[edge]; k++)
	NSet[2].nodeqty[i] -=
	  ut_array_1d_int_deletencompress (NSet[2].nodes[i],
					   NSet[2].nodeqty[i],
					   NSet[1].nodes[edge][k], 1);
    }

    NSet[2].nodes[i] =
      ut_realloc_1d_int (NSet[2].nodes[i], NSet[2].nodeqty[i]);
  }

  ut_free_2d_int (faceedge, 7);

  return;
}

void
nem_nsets_1dbody_str (struct NSET *NSet)
{
  int i, j, k, ver;
  int ref, prevqty = NSet[1].qty;
  int **edgever = NULL;

  if (NSet[1].qty == 12)
  {
    edgever = ut_alloc_2d_int (13, 2);

    edgever[1][0] = 1;
    edgever[1][1] = 5;
    edgever[2][0] = 2;
    edgever[2][1] = 6;
    edgever[3][0] = 3;
    edgever[3][1] = 7;
    edgever[4][0] = 4;
    edgever[4][1] = 8;
    edgever[5][0] = 1;
    edgever[5][1] = 2;
    edgever[6][0] = 3;
    edgever[6][1] = 4;
    edgever[7][0] = 5;
    edgever[7][1] = 6;
    edgever[8][0] = 7;
    edgever[8][1] = 8;
    edgever[9][0] = 1;
    edgever[9][1] = 3;
    edgever[10][0] = 2;
    edgever[10][1] = 4;
    edgever[11][0] = 5;
    edgever[11][1] = 7;
    edgever[12][0] = 6;
    edgever[12][1] = 8;
  }

  else if (NSet[1].qty == 4)
  {
    edgever = ut_alloc_2d_int (5, 2);

    edgever[1][0] = 1;
    edgever[1][1] = 3;
    edgever[2][0] = 2;
    edgever[2][1] = 4;
    edgever[3][0] = 1;
    edgever[3][1] = 2;
    edgever[4][0] = 3;
    edgever[4][1] = 4;
  }
  else
    ut_error_reportbug ();

  NSet[1].qty *= 2;
  NSet[1].names = ut_realloc_1d_pchar (NSet[1].names, NSet[1].qty + 1);
  NSet[1].nodeqty = ut_realloc_1d_int (NSet[1].nodeqty, NSet[1].qty + 1);
  NSet[1].nodes = ut_realloc_1d_pint (NSet[1].nodes, NSet[1].qty + 1);

  for (i = prevqty + 1; i <= NSet[1].qty; i++)
  {
    ref = i - prevqty;

    NSet[1].names[i] = ut_alloc_1d_char (strlen (NSet[1].names[ref]) + 5);
    sprintf (NSet[1].names[i], "%sbody", NSet[1].names[ref]);

    NSet[1].nodeqty[i] = NSet[1].nodeqty[ref];
    NSet[1].nodes[i] = ut_alloc_1d_int (NSet[1].nodeqty[i]);
    ut_array_1d_int_memcpy (NSet[1].nodes[i], NSet[1].nodeqty[i],
			    NSet[1].nodes[ref]);

    for (j = 0; j < 2; j++)
    {
      ver = edgever[ref][j];

      for (k = 0; k < NSet[0].nodeqty[ver]; k++)
	NSet[1].nodeqty[i] -=
	  ut_array_1d_int_deletencompress (NSet[1].nodes[i],
					   NSet[1].nodeqty[i],
					   NSet[0].nodes[ver][k], 1);
    }

    NSet[1].nodes[i] =
      ut_realloc_1d_int (NSet[1].nodes[i], NSet[1].nodeqty[i]);
  }

  ut_free_2d_int (edgever, NSet[2].qty + 1);

  return;
}
