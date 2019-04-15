/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_csysdata_.h"

void
nev_csysdata_init (struct CSYSDATA *pCsysData)
{
  int i;

  (*pCsysData).Col = ut_alloc_1d_int (3);
  ut_array_1d_int_set ((*pCsysData).Col, 3, 32);
  (*pCsysData).Coo = ut_alloc_1d (3);
  (*pCsysData).Rad = 0.01;
  (*pCsysData).Length = 0.2;

  if ((*pCsysData).ColData)
  {
    if ((*pCsysData).ColDataType && !strcmp ((*pCsysData).ColDataType, "col"))
      for (i = 0; i < 3; i++)
	(*pCsysData).Col[i] = ut_num_d2ri ((*pCsysData).ColData[i]);
    else
    {
      printf ("(*pCsysData).ColDataType = %s\n", (*pCsysData).ColDataType);
      ut_error_reportbug ();
    }
  }

  if ((*pCsysData).RadData)
    (*pCsysData).Rad = (*pCsysData).RadData;

  if ((*pCsysData).LengthData)
    (*pCsysData).Length = (*pCsysData).LengthData;

  if ((*pCsysData).CooDataType)
  {
    if (!strcmp ((*pCsysData).CooDataType, "coo"))
      for (i = 0; i < 3; i++)
	(*pCsysData).Coo[i] = (*pCsysData).CooData[i];
    else
      abort ();
  }

  if (!(*pCsysData).Label)
  {
    (*pCsysData).Label = ut_alloc_2d_char (3, 3);
    strcpy ((*pCsysData).Label[0], "X1");
    strcpy ((*pCsysData).Label[1], "X2");
    strcpy ((*pCsysData).Label[2], "X3");
  }

  if ((*pCsysData).FontSize == -1)
    (*pCsysData).FontSize = 1;

  return;
}

void
nev_csysdata_fscanf (char *type, char *argument, struct CSYSDATA *pCsysData)
{
  char **args = NULL;
  int i, argqty;
  ut_string_separate (argument, NEUT_SEP_DEP, &args, &argqty);
  char *value = NULL;

  if (!strcmp (type, "col"))
  {
    nev_data_typearg_args ("col", argument, &(*pCsysData).ColDataType,
			   &value, NULL);

    if (!strcmp ((*pCsysData).ColDataType, "col"))
    {
      (*pCsysData).ColData = ut_alloc_1d (3);
      ut_array_1d_fscanfn_wcard (value, (*pCsysData).ColData,
				 3, "colour,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "rad"))
  {
    (*pCsysData).RadDataType = ut_alloc_1d_char (100);

    if (argqty == 1)
      strcpy ((*pCsysData).RadDataType, type);
    else
      strcpy ((*pCsysData).RadDataType, args[0]);

    if (!strcmp ((*pCsysData).RadDataType, "rad"))
    {
      ut_array_1d_fscanfn_wcard (args[0], &(*pCsysData).RadData,
				 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "length"))
  {
    (*pCsysData).LengthDataType = ut_alloc_1d_char (100);

    if (argqty == 1)
      strcpy ((*pCsysData).LengthDataType, type);
    else
      strcpy ((*pCsysData).LengthDataType, args[0]);

    if (!strcmp ((*pCsysData).LengthDataType, "length"))
    {
      ut_array_1d_fscanfn_wcard (args[0], &(*pCsysData).LengthData,
				 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (!strcmp (type, "coo"))
  {
    if (argqty == 3)
    {
      ut_string_string ("coo", &(*pCsysData).CooDataType);
      if ((*pCsysData).CooData == NULL)
	(*pCsysData).CooData = ut_alloc_1d (3);
      for (i = 0; i < 3; i++)
	sscanf (args[i], "%lf", (*pCsysData).CooData + i);
    }
    else
      abort ();
  }
  else if (!strcmp (type, "label"))
  {
    if (argqty == 3)
    {
      if ((*pCsysData).Label == NULL)
	(*pCsysData).Label = ut_alloc_1d_pchar (3);

      for (i = 0; i < 3; i++)
	ut_string_string (args[i], (*pCsysData).Label + i);
    }
    else
      abort ();
  }
  else if (!strcmp (type, "fontsize"))
  {
    if (argqty == 1)
      sscanf (args[0], "%lf", &((*pCsysData).FontSize));
    else
      abort ();
  }
  else
    ut_error_reportbug ();

  ut_free_1d_char (value);

  return;
}
