/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_mesh_1d_.h"

void
nev_print_mesh_1d (FILE * file, struct PRINT Print, struct TESS Tess,
		   struct MESH *Mesh, struct NODEDATA NodeData, struct
		   MESHDATA *MeshData)
{
  int i, j, elset, elt1d_qty, elt3dqty, printelt1d_qty, texture_unique;
  double ambient = (Print.showshadow == 1) ? 0.6 : 1;
  char *texture = NULL;
  int *hidden = NULL;
  int *elts3d = NULL;
  int *Col = ut_alloc_1d_int (3);

  elt1d_qty = ut_array_1d_int_sum (Print.showelt1d + 1, Mesh[1].EltQty);
  if (elt1d_qty == 0)
    return;

  texture = ut_alloc_1d_char (100);
  hidden = ut_alloc_1d_int (Mesh[1].EltQty + 1);

  // if 3D mesh is here, checking if it is hidden. If so, won't print it.
  if (Mesh[3].EltQty > 0)
  {
    ut_array_1d_int_set (hidden + 1, Mesh[1].EltQty, 1);
    for (i = 1; i <= Mesh[1].EltQty; i++)
      if (Print.showelt1d[i])
      {
        elset = Mesh[1].EltElset[i];

	if (Tess.EdgeQty == 0)
	  hidden[i] = 0;
        else if (Tess.EdgeQty > 0 && Tess.EdgeDom[elset][0] >= 1)
	  hidden[i] = 0;
        else if (!neut_mesh_elt1d_isembedded (Mesh[3], Mesh[1], i))
          hidden[i] = 0;
	else
	{
	  neut_mesh_elt1d_elts3d (Mesh[1], i, Mesh[2], Mesh[3], &elts3d,
				  &elt3dqty);

	  for (j = 0; j < elt3dqty; j++)
	    if (Print.showelt3d[elts3d[j]] == 0)
	    {
	      hidden[i] = 0;
	      break;
	    }
	}

	ut_free_1d_int (elts3d);
	elts3d = NULL;
      }
    hidden[0] = ut_array_1d_int_sum (hidden + 1, Mesh[1].EltQty);
  }

  texture_unique = 1;
  for (i = 1; i <= Mesh[1].EltQty; i++)
    if (Print.showelt1d[i] == 1 && hidden[i] == 0)
    {
      if (Col == NULL)
      {
	Col = ut_alloc_1d_int (3);
	ut_array_1d_int_memcpy (Col, 3, MeshData[1].Col[i]);
      }
      else if (ut_array_1d_int_diff (MeshData[1].Col[i], 3, Col, 3))
      {
	texture_unique = 0;
	break;
      }
    }

  if (texture_unique)
  {
    fprintf (file,
	     "#declare elt1d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
	     Col[0] / 255., Col[1] / 255., Col[2] / 255., ambient);

    strcpy (texture, "elt1d");
  }

  printelt1d_qty = 0;
  for (i = 1; i <= Mesh[1].EltQty; i++)
    if (Print.showelt1d[i] == 1 && hidden[i] == 0)
    {
      if (!texture_unique)
      {
	fprintf (file,
		 "#declare elt1d%d =\n  texture { pigment { rgb <%f,%f,%f> } finish {ambient %f} }\n",
		 i,
		 MeshData[1].Col[i][0] / 255.,
		 MeshData[1].Col[i][1] / 255.,
		 MeshData[1].Col[i][2] / 255., ambient);

	sprintf (texture, "elt1d%d", i);
      }

      char *string = ut_alloc_1d_char (100);
      sprintf (string, "%.12f", MeshData[1].Rad[i]);
      nev_print_segment_wsph (file,
			      NodeData.Coo[Mesh[1].EltNodes[i][0]],
			      NodeData.Coo[Mesh[1].EltNodes[i][1]],
			      string, texture);

      printelt1d_qty++;
      ut_free_1d_char (string);
    }

  ut_print_message (0, 4,
		    "Number of 1D elts    reduced by %3.0f\%% (to %d).\n",
		    100 * (double) hidden[0] / (double) elt1d_qty,
		    elt1d_qty - hidden[0]);

  ut_free_1d_int (hidden);

  return;
}
