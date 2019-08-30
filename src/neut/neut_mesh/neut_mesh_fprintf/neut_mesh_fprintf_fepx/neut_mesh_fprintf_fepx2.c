/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_fepx_.h"

void
neut_mesh_fprintf_fepx_parms (FILE * file, struct NODES Nodes,
			      struct MESH Mesh)
{
  /* fepx1 = nb of elts and nodes */
  fprintf (file, "%d %d 9\n", Mesh.EltQty, Nodes.NodeQty);
}

void
neut_mesh_fprintf_fepx_nodes (FILE * file, struct NODES Nodes)
{
  int i, j;

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i - 1);
    for (j = 0; j < 3; j++)
      fprintf (file, "  %.12f",
	       (fabs (Nodes.NodeCoo[i][j]) <
		1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }

  return;
}

void
neut_mesh_fprintf_fepx_elts (FILE * file, struct TESS Tess,
			     struct NSET NSet2D, struct MESH Mesh)
{
  int i, j, fod;

  if (Mesh.Dimension == 2 && Mesh.EltOrder == 1)
  {
    fprintf (file, "1\n      %d\n", Mesh.EltQty);
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      for (j = 0; j < 3; j++)
	fprintf (file, " %d", Mesh.EltNodes[i][j] - 1);

      fod = -ut_array_1d_int_min (Tess.FacePoly[Mesh.EltElset[i]], 2);

      fprintf (file, " %s\n", NSet2D.names[fod]);
    }
  }
  else if (Mesh.Dimension == 2 && Mesh.EltOrder == 2)
  {
    fprintf (file, "1\n%8d\n", Mesh.EltQty);
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][0] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][3] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][1] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][4] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][2] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][5] - 1);

      fod = -ut_array_1d_int_min (Tess.FacePoly[Mesh.EltElset[i]], 2);

      fprintf (file, " %s\n", NSet2D.names[fod]);
    }
  }
  else if (Mesh.Dimension == 3 && Mesh.EltOrder == 1)
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      for (j = 0; j < 4; j++)
	fprintf (file, " %d", Mesh.EltNodes[i][j] - 1);
      fprintf (file, "\n");
    }
  else if (Mesh.Dimension == 3 && Mesh.EltOrder == 2)
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%d ", i - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][0] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][4] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][1] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][5] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][2] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][6] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][7] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][9] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][8] - 1);
      fprintf (file, " %d", Mesh.EltNodes[i][3] - 1);
      fprintf (file, "\n");
    }

  return;
}

void
neut_mesh_fprintf_fepx_skinelts (FILE * file, struct TESS Tess,
				 struct MESH Mesh2D, struct MESH Mesh3D,
				 struct NODES Nodes, char *surflist)
{
  double res;
  int i, j, k, l, eltnb, eltqty, elt3dqty, id, status;
  int *elt3d = NULL;
  double *n = ut_alloc_1d (3);
  double *n0 = ut_alloc_1d (3);
  int seqo1[3] = { 0, 1, 2 };
  int seqo2[6] = { 0, 3, 1, 4, 2, 5 };
  char **name = NULL;
  int qty;

  ut_string_separate (surflist, NEUT_SEP_NODEP, &name, &qty);

  fprintf (file, "%d\n", qty);

  for (i = 0; i < qty; i++)
  {
    status = neut_tess_domface_label_id (Tess, name[i], &id);

    if (status != 0)
      continue;

    // calculating number of elements
    eltqty = 0;
    for (j = 1; j <= Tess.DomTessFaceQty[id]; j++)
      eltqty += Mesh2D.Elsets[Tess.DomTessFaceNb[id][j]][0];

    fprintf (file, "%d\n", eltqty);

    ut_array_1d_memcpy (n0, 3, Tess.DomFaceEq[id] + 1);
    ut_array_1d_scale (n0, 3, -1);

    for (j = 1; j <= Tess.DomTessFaceQty[id]; j++)
    {
      for (k = 1; k <= Mesh2D.Elsets[Tess.DomTessFaceNb[id][j]][0]; k++)
      {
	eltnb = Mesh2D.Elsets[Tess.DomTessFaceNb[id][j]][k];
	neut_mesh_elt_normal (Mesh2D, Nodes, eltnb, n);

	neut_mesh_elt2d_elts3d (Mesh2D, eltnb, Mesh3D, &elt3d, &elt3dqty);

	if (elt3dqty != 1)
	{
	  printf ("Nodes:\n");
	  neut_debug_nodes (stdout, Nodes);
	  printf ("Mesh2D:\n");
	  neut_debug_mesh (stdout, Mesh2D);
	  printf ("Mesh3D:\n");
	  neut_debug_mesh (stdout, Mesh3D);

	  printf ("elt = %d\n", eltnb);
	  printf ("elt3dqty != 1 ( = %d)\n", elt3dqty);
	  ut_array_1d_int_fprintf (stdout, elt3d, elt3dqty, "%d");
	  abort ();
	}

	fprintf (file, "%d", elt3d[0] - 1);

	res = ut_vector_scalprod (n0, n);
	if (Mesh2D.EltOrder == 1)
	{
	  if (res > 0)
	    for (l = 0; l <= 2; l++)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo1[l]] - 1);
	  else
	    for (l = 2; l >= 0; l--)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo1[l]] - 1);
	}
	else if (Mesh2D.EltOrder == 2)
	{
	  if (res > 0)
	  {
	    for (l = 0; l < 6; l++)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo2[l]] - 1);
	  }
	  else
	  {
	    /* reverse order from 0 (must be from a 1st order node) */
	    fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo2[0]] - 1);
	    for (l = 5; l >= 1; l--)
	      fprintf (file, " %d", Mesh2D.EltNodes[eltnb][seqo2[l]] - 1);
	  }
	}

	fprintf (file, " \n");
      }
    }
  }

  ut_free_1d (n);
  ut_free_1d (n0);
  ut_free_1d_int (elt3d);
  ut_free_2d_char (name, qty);

  return;
}

void
neut_mesh_fprintf_fepx_elsets (FILE * file, struct MESH Mesh3D, char *version)
{
  int i;

  if (version && !strcmp (version, "legacy"))
    fprintf (file, "grain-input\n");

  fprintf (file, "%d %d\n", Mesh3D.EltQty, Mesh3D.ElsetQty);

  // 1 if for the phase
  for (i = 1; i <= Mesh3D.EltQty; i++)
    fprintf (file, "%d 1\n",
	     (!Mesh3D.ElsetId) ? Mesh3D.EltElset[i]
	     : Mesh3D.ElsetId[Mesh3D.EltElset[i]]);

  if (version && !strcmp (version, "legacy"))
    fprintf (file, "end-options\n");

  return;
}

void
neut_mesh_fprintf_fepx_nsets (FILE * file, struct NSET NSet0D,
			      struct NSET NSet1D, struct NSET NSet2D,
			      char *nset, char *version)
{
  int i, j, status;
  char **name;
  int qty;

  ut_string_separate (nset, NEUT_SEP_NODEP, &name, &qty);

  /* first pass, for calculating total number of bcs */
  int bcqty = 0;
  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
	bcqty += NSet0D.nodeqty[j];
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
	bcqty += NSet1D.nodeqty[j];
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
	bcqty += NSet2D.nodeqty[j];
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  /* header */
  if (version && !strcmp(version,"legacy"))
    fprintf (file, "0\n");

  fprintf(file, "%d\n", bcqty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (!strcmp (name[i], NSet0D.names[j]))
      {
	neut_mesh_fprintf_fepx_nset (file, NSet0D.names[j], NSet0D.nodeqty[j],
				     NSet0D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (!strcmp (name[i], NSet1D.names[j]))
      {
	neut_mesh_fprintf_fepx_nset (file, NSet1D.names[j], NSet1D.nodeqty[j],
				     NSet1D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (!strcmp (name[i], NSet2D.names[j]))
      {
	neut_mesh_fprintf_fepx_nset (file, NSet2D.names[j], NSet2D.nodeqty[j],
				     NSet2D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  /* foot */
  fprintf (file, "0\n");

  return;
}

void
neut_mesh_fprintf_fepx_kocks (FILE *file, struct TESS Tess, char *version)
{
  int i;
  double *ek = ol_e_alloc ();

  if (version && strcmp (version, "legacy"))
    abort ();

  fprintf (file, "grain-orientations\n");
  fprintf (file, "%d\n", Tess.CellQty);
  for (i = 1; i <= Tess.CellQty; i++)
  {
    ol_q_e (Tess.CellOri[i], ek);
    ol_e_ek (ek, ek);
    fprintf (file, "%17.12f %17.12f %17.12f %d\n", ek[0], ek[1], ek[2], i);
  }
  fprintf (file, "EOF\n");

  ol_e_free (ek);

  return;
}

/*
void
neut_mesh_bcsfile_update (FILE* filein, FILE* file, struct PART Part)
{
  int i, j, id, newid, tmp, qty, colqty;
  char* string = ut_alloc_1d_char (1000);

  colqty = ut_file_nbwords_pointer (filein) - 3;
  colqty /= tmp;
  colqty--;

  fscanf (filein, "%d\n", &tmp);
  fprintf (file, "%d\n", tmp);
  fscanf (filein, "%d\n", &qty);
  fprintf (file, "%d\n", qty);

  for (i = 0; i < qty; i++)
  {
    fscanf (filein, "%d", &id);
    id++;
    if (Part.qty > 0)
      newid = Part.node_old_new[id];
    else
      newid = id;

    fprintf (file, "%d", newid - 1);

    for (j = 0; j < colqty; j++)
    {
      fscanf (filein, "%s", string);
      fprintf (file, " %s", string);
    }

    fprintf (file, "\n");
  }

  fscanf (filein, "%d\n", &tmp);
  fprintf (file, "%d\n", tmp);

  ut_free_1d_char (string);

  return;
}
*/

/*
void
neut_mesh_surffile_update (FILE* filein, FILE* file, struct PART Part)
{
  int i, j, st, eltnodeqty, id, newid, tmp, qty;
  char* string = ut_alloc_1d_char (1000);

  eltnodeqty = ut_file_nbwords_pointer (filein) - 2;

  fscanf (filein, "%d", &tmp);
  fprintf (file, "%d\n", tmp);

  fscanf (filein, "%d", &qty);
  fprintf (file, "%d\n", qty);

  eltnodeqty /= qty;

  st = 0;
  switch (eltnodeqty)
  {
  case 8:
    st = 1;
    eltnodeqty = 6;
    break;
  case 7:
    st = 0;
    eltnodeqty = 6;
    break;
  case 5:
    st = 1;
    eltnodeqty = 3;
    break;
  case 4:
    st = 0;
    eltnodeqty = 3;
    break;
  default:
    {
      ut_print_message (2, 0, "surf file has a bad format.\n");
      abort ();
    }
  }

  for (i = 0; i < qty; i++)
  {
    fscanf (filein, "%d", &id);
    id++;
    if (Part.qty > 0)
      newid = Part.elts_old_new[id];
    else
      newid = id;

    fprintf (file, "%d", newid - 1);

    for (j = 0; j < eltnodeqty; j++)
    {
      fscanf (filein, "%d", &id);
      id++;
      if (Part.qty > 0)
	newid = Part.nodes_old_new[id];
      else
	newid = id;
      fprintf (file, " %d", newid - 1);
    }

    if (st == 1)
    {
      fscanf (filein, "%s", string);
      fprintf (file, " %s", string);
    }
    fprintf (file, "\n");
  }

  ut_free_1d_char (string);

  return;
}
*/
