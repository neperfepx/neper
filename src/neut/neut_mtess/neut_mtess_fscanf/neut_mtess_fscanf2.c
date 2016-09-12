/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fscanf_.h"

int
neut_mtess_fscanf_head (struct MTESS *pMTess, FILE * file)
{
  int i, id, status;
  char *string = ut_alloc_1d_char (100);

  if (fscanf (file, "%s", string) != 1)
  {
    ut_print_message (2, 0,
		      "Input file is not a valid multilevel tessellation file.\n");
    abort ();
  }

  if (!strcmp (string, "***mtess"))
  {
    if (ut_file_string_scanncomp (file, "**format") != 0
	|| ut_file_string_scanncomp (file, "2.0") != 0
	|| ut_file_string_scanncomp (file, "**general") != 0)
    {
      ut_print_message (2, 0,
			"Input file is not a valid multilevel tessellation file.\n");
      abort ();
    }

    if (fscanf (file, "%d", &((*pMTess).LevelQty)) != 1)
    {
      ut_print_message (2, 0,
			"Input file is not a valid multilevel tessellation file.\n");
      abort ();
    }

    (*pMTess).LevelTessQty = ut_alloc_1d_int ((*pMTess).LevelQty + 1);
    (*pMTess).LevelTessQty[0] = 1;

    (*pMTess).LevelTess = ut_alloc_1d_pint ((*pMTess).LevelQty + 1);
    for (i = 1; i <= (*pMTess).LevelQty; i++)
    {
      if (fscanf (file, "%d%d", &id, &((*pMTess).LevelTessQty[i])) != 2
	  || id != i)
	abort ();

      (*pMTess).LevelTess[i] =
	ut_alloc_1d_int ((*pMTess).LevelTessQty[i] + 1);
      ut_array_1d_int_fscanf (file, (*pMTess).LevelTess[i] + 1,
			      (*pMTess).LevelTessQty[i]);
    }

    if (fscanf (file, "%d", &((*pMTess).TessQty)) != 1)
      abort ();

    (*pMTess).TessDom = ut_alloc_2d_int ((*pMTess).TessQty + 1, 2);
    for (i = 1; i <= (*pMTess).TessQty; i++)
    {
      if (fscanf (file, "%d", &id) != 1 || id != i)
	abort ();

      ut_array_1d_int_fscanf (file, (*pMTess).TessDom[i], 2);
    }

    if (fscanf (file, "%d", &((*pMTess).TessDQty)) != 1)
      abort ();

    (*pMTess).DomTess = ut_alloc_1d_pint ((*pMTess).TessDQty + 1);
    for (i = 1; i <= (*pMTess).TessDQty; i++)
    {
      int qty;
      if (fscanf (file, "%d%d", &id, &qty) != 2 || id != i)
	abort ();

      (*pMTess).DomTess[i] = ut_alloc_1d_int (qty + 1);
      ut_array_1d_int_fscanf (file, (*pMTess).DomTess[i] + 1, qty);
    }

    if (ut_file_string_scanncomp (file, "***end") != 0)
      abort ();

    status = 0;
  }

  else if (!strcmp (string, "***tess"))
  {
    fseek (file, 0, 0);
    (*pMTess).LevelQty = 1;

    (*pMTess).LevelTessQty = ut_alloc_1d_int ((*pMTess).LevelQty + 1);
    (*pMTess).LevelTessQty[0] = 1;

    (*pMTess).LevelTess = ut_alloc_1d_pint ((*pMTess).LevelQty + 1);

    (*pMTess).LevelTessQty[1] = 1;
    (*pMTess).LevelTess[1] = ut_alloc_1d_int ((*pMTess).LevelTessQty[1] + 1);
    (*pMTess).LevelTess[1][1] = 1;

    (*pMTess).TessQty = 1;
    (*pMTess).TessDom = ut_alloc_2d_int ((*pMTess).TessQty + 1, 2);
    (*pMTess).TessDom[1][1] = 1;

    status = 1;
  }

  else
  {
    ut_print_message (2, 0,
		      "Input file is not a valid multilevel tessellation file.\n");
    abort ();
  }

  ut_free_1d_char (string);

  return status;
}
