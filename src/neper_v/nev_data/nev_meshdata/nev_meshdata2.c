/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_meshdata_.h"

void
nev_meshdata_fscanf_elt (struct MESH Mesh, struct MESHDATA *pMD,
			 char *type, char *argument, int elset)
{
  int i;
  char *value = NULL;
  char **args = NULL;
  int argqty;

  if (!strcmp (type, "col"))
  {
    if (!strcmp (argument, "from_nodes"))
    {
      ut_string_string (argument, &(*pMD).ColDataType);
      return;
    }

    ut_string_separate (argument, NEUT_SEP_DEP, &args, &argqty);

    nev_data_typearg_args ("col", argument, &(*pMD).ColDataType, &value, NULL);

    if (!strcmp ((*pMD).ColDataType, "id"))
    {
      (*pMD).ColData = ut_alloc_2d ((*pMD).EltQty + 1, 3);

      if (elset == 0)
	for (i = 1; i <= (*pMD).EltQty; i++)
	  (*pMD).ColData[i][0] = i;
      else
      {
	if (argqty == 1)
	  for (i = 1; i <= (*pMD).EltQty; i++)
	    (*pMD).ColData[i][0] = Mesh.ElsetId ? Mesh.ElsetId[i] : i;
	else
	  ut_array_2d_fscanfn (args[1], (*pMD).ColData + 1,
			       (*pMD).EltQty, 1);
      }
    }
    else if (!strcmp ((*pMD).ColDataType, "col"))
    {
      (*pMD).ColData = ut_alloc_2d ((*pMD).EltQty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pMD).ColData + 1,
				 (*pMD).EltQty, 3, "colour,size");
    }
    else if (!strncmp ((*pMD).ColDataType, "ori", 3))
      nev_data_fscanf_ori (value, (*pMD).EltQty, NULL,
                           &(*pMD).ColData, &(*pMD).ColDataType);
    else if (!strcmp ((*pMD).ColDataType, "scal"))
    {
      (*pMD).ColData = ut_alloc_2d ((*pMD).EltQty + 1, 1);
      ut_array_2d_fscanfn_wcard (value, (*pMD).ColData + 1,
				 (*pMD).EltQty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "rad"))
  {
    (*pMD).RadDataType = ut_alloc_1d_char (100);
    strcpy ((*pMD).RadDataType, type);

    (*pMD).RadData = ut_alloc_2d ((*pMD).EltQty + 1, 1);
    ut_array_2d_fscanfn_wcard (argument, (*pMD).RadData + 1,
			       (*pMD).EltQty, 1, "numeral,size");
  }
  else if (!strcmp (type, "colscheme"))
  {
    (*pMD).ColScheme = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pMD).ColScheme, argument);
  }
  else if (!strcmp (type, "scale"))
  {
    (*pMD).Scale = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pMD).Scale, argument);
  }
  else if (!strcmp (type, "scaletitle"))
  {
    (*pMD).ScaleTitle = ut_alloc_1d_char (strlen (argument) + 1);
    strcpy ((*pMD).ScaleTitle, argument);
  }
  else
    ut_error_reportbug ();

  ut_free_1d_char (value);
  ut_free_2d_char (args, argqty);

  return;
}

void
nev_meshdata_fscanf_eltb (struct MESHDATA *pMD, char *type, char *argument)
{
  if (!strcmp (type, "col"))
  {
    (*pMD).BCol = ut_alloc_1d_int (3);
    ut_array_1d_int_fscanfn_wcard (argument, (*pMD).BCol, 3, "colour");
  }
  else if (!strcmp (type, "rad"))
    ut_array_1d_fscanfn_wcard (argument, &((*pMD).BRad), 1, "numeral");
  else
    ut_error_reportbug ();

  return;
}

void
nev_meshdata_elset2elt (struct MESH Mesh,
			char *entity, char *type,
			struct MESHDATA MeshDataSet, struct MESHDATA *pMD)
{
  int i, j, size;

  if (entity[strlen (entity) - 1] != 'b')
  {
    if (!strcmp (type, "col"))
    {
      (*pMD).ColDataType
	= ut_alloc_1d_char (strlen (MeshDataSet.ColDataType) + 1);
      strcpy ((*pMD).ColDataType, MeshDataSet.ColDataType);

      size = -1;
      if (!strcmp ((*pMD).ColDataType, "id"))
	size = 1;
      else if (!strcmp ((*pMD).ColDataType, "col")
	       || !strncmp ((*pMD).ColDataType, "ori", 3))
	size = 4;
      else if (!strcmp ((*pMD).ColDataType, "rad"))
	size = 1;
      else if (!strcmp ((*pMD).ColDataType, "scal"))
	size = 1;
      else
	abort ();

      (*pMD).ColData = ut_alloc_2d ((*pMD).EltQty + 1, size);
      for (i = 1; i <= Mesh.ElsetQty; i++)
	for (j = 1; j <= Mesh.Elsets[i][0]; j++)
	  ut_array_1d_memcpy ((*pMD).ColData[Mesh.Elsets[i][j]],
			      size, MeshDataSet.ColData[i]);
    }
    else if (!strcmp (type, "rad"))
    {
      (*pMD).RadDataType
	= ut_alloc_1d_char (strlen (MeshDataSet.RadDataType + 1));
      strcpy ((*pMD).RadDataType, MeshDataSet.RadDataType);

      (*pMD).RadData = ut_alloc_2d ((*pMD).EltQty + 1, 1);
      for (i = 1; i <= Mesh.ElsetQty; i++)
	for (j = 1; j <= Mesh.Elsets[i][0]; j++)
	  (*pMD).RadData[Mesh.Elsets[i][j]] = MeshDataSet.ColData[i];
    }
    else if (!strcmp (type, "colscheme"))
    {
      (*pMD).ColScheme =
	ut_alloc_1d_char (strlen (MeshDataSet.ColScheme) + 1);
      strcpy ((*pMD).ColScheme, MeshDataSet.ColScheme);
    }
    else if (!strcmp (type, "scale"))
    {
      (*pMD).Scale = ut_alloc_1d_char (strlen (MeshDataSet.Scale) + 1);
      strcpy ((*pMD).Scale, MeshDataSet.Scale);
    }
    else if (!strcmp (type, "scaletitle"))
    {
      (*pMD).ScaleTitle
	= ut_alloc_1d_char (strlen (MeshDataSet.ScaleTitle) + 1);
      strcpy ((*pMD).ScaleTitle, MeshDataSet.ScaleTitle);
    }
    else
      ut_error_reportbug ();
  }
  else
  {
    if (!strcmp (type, "col"))
    {
      (*pMD).BCol = ut_alloc_1d_int (3);
      ut_array_1d_int_memcpy ((*pMD).BCol, 3, MeshDataSet.BCol);
    }
    else if (!strcmp (type, "rad"))
      (*pMD).BRad = MeshDataSet.BRad;
    else
      ut_error_reportbug ();
  }

  return;
}
