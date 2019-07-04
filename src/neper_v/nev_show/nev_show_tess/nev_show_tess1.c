/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_show_tess_.h"

void
nev_show_tess (char **argv, int *pi, struct PRINT *pPrint)
{
  (*pi)++;
  ut_string_int (argv[(*pi)], &((*pPrint).showtess));
  if ((*pPrint).showtess < 0 || (*pPrint).showtess > 1)
  {
    ut_print_message (2, 0, "-showtess: bad value.\n");
    abort ();
  }

  return;
}

void
nev_show_tess_seed (char **argv, int *pi, struct TESS Tess,
		    struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = ut_alloc_1d (1000);
  int SeedQty = Tess.CellQty;

  (*pi)++;
  (*pPrint).showseed = ut_realloc_1d_int ((*pPrint).showseed, SeedQty + 1);
  ut_array_1d_int_zero ((*pPrint).showseed, SeedQty + 1);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], (*pPrint).showseed, SeedQty);

  if (status != 0)
  {
    neut_tess_var_list (Tess, "seed", &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= SeedQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_tess_var_val_one (Tess, (*pPrint).showedge,
			         (*pPrint).showface, (*pPrint).showpoly, "seed",
			         i, vars[j], vals + j, NULL);

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	(*pPrint).showseed[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pPrint).showseed[0] =
    ut_array_1d_int_sum ((*pPrint).showseed + 1, SeedQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
nev_show_tess_ver (char **argv, int *pi, struct TESS Tess,
		   struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = NULL;

  (*pi)++;
  (*pPrint).showver = ut_realloc_1d_int ((*pPrint).showver, Tess.VerQty + 1);
  ut_array_1d_int_zero ((*pPrint).showver, Tess.VerQty + 1);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], (*pPrint).showver, Tess.VerQty);

  if (status != 0)
  {
    neut_tess_var_list (Tess, "ver", &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tess.VerQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_tess_var_val_one (Tess, (*pPrint).showedge,
                                 (*pPrint).showface, (*pPrint).showpoly, "ver", i,
                                 vars[j], vals + j, NULL);

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	(*pPrint).showver[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pPrint).showver[0] = ut_array_1d_int_sum ((*pPrint).showver + 1,
					      Tess.VerQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
nev_show_tess_edge (char **argv, int *pi, struct TESS Tess,
		    struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = NULL;

  (*pi)++;
  (*pPrint).showedge =
    ut_realloc_1d_int ((*pPrint).showedge, Tess.EdgeQty + 1);
  ut_array_1d_int_zero ((*pPrint).showedge, Tess.EdgeQty + 1);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], (*pPrint).showedge, Tess.EdgeQty);

  if (status != 0)
  {
    neut_tess_var_list (Tess, "edge", &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tess.EdgeQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_tess_var_val_one (Tess,
			         (*pPrint).showedge,
			         (*pPrint).showface, (*pPrint).showpoly,
			         "edge", i, vars[j], vals + j, NULL);

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	(*pPrint).showedge[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pPrint).showedge[0] = ut_array_1d_int_sum ((*pPrint).showedge + 1,
					       Tess.EdgeQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
nev_show_tess_face (char **argv, int *pi, struct TESS Tess,
		    struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = NULL;

  (*pi)++;
  (*pPrint).showface =
    ut_realloc_1d_int ((*pPrint).showface, Tess.FaceQty + 1);
  ut_array_1d_int_zero ((*pPrint).showface + 1, Tess.FaceQty);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], (*pPrint).showface, Tess.FaceQty);

  if (status != 0)
  {
    neut_tess_var_list (Tess, "face", &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tess.FaceQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_tess_var_val_one (Tess, NULL, NULL,
			         (*pPrint).showface, "face", i, vars[j],
			         vals + j, NULL);

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	(*pPrint).showface[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pPrint).showface[0] = ut_array_1d_int_sum ((*pPrint).showface + 1,
					       Tess.FaceQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
nev_show_tess_poly (char **argv, int *pi, struct TESS Tess,
		    struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = NULL;

  (*pi)++;
  (*pPrint).showpoly =
    ut_realloc_1d_int ((*pPrint).showpoly, Tess.PolyQty + 1);
  ut_array_1d_int_zero ((*pPrint).showpoly + 1, Tess.PolyQty);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], (*pPrint).showpoly, Tess.PolyQty);

  if (status != 0)
  {
    neut_tess_var_list (Tess, "poly", &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tess.PolyQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	  neut_tess_var_val_one (Tess, NULL, NULL, NULL, "poly", i, vars[j],
			         vals + j, NULL);

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	(*pPrint).showpoly[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  (*pPrint).showpoly[0] = ut_array_1d_int_sum ((*pPrint).showpoly + 1,
					       Tess.PolyQty);

  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);

  return;
}

void
nev_show_tess_cell (char **argv, int *pi, struct TESS Tess,
		    struct PRINT *pPrint)
{
  int i, j, status, varqty;
  double res;
  char **vars = NULL;
  double *vals = NULL;
  int *showcell = ut_alloc_1d_int (Tess.CellQty + 1);
  char *cell = NULL;

  neut_tess_cell (Tess, &cell);

  (*pi)++;

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], showcell, Tess.CellQty);

  if (status != 0)
  {
    neut_tess_var_list (Tess, cell, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= Tess.CellQty; i++)
    {
      for (j = 0; j < varqty; j++)
	if (strstr (argv[(*pi)], vars[j]))
	{
	  if (!strcmp (vars[j], "id"))
	    vals[j] = Tess.CellId ? Tess.CellId[i] : i;
	  else
	    neut_tess_var_val_one (Tess, NULL, NULL, NULL, cell, i, vars[j],
			           vals + j, NULL);
	}

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (status == 0)
	showcell[i] = res;
      else
	ut_print_message (2, 0, "Expression `%s' could not be processed.\n",
			  argv[(*pi)]);
    }
  }

  showcell[0] = ut_array_1d_int_sum (showcell + 1, Tess.CellQty);

  if (Tess.Dim == 3)
  {
    (*pPrint).showpoly =
      ut_realloc_1d_int ((*pPrint).showpoly, Tess.PolyQty + 1);
    ut_array_1d_int_memcpy ((*pPrint).showpoly, Tess.PolyQty + 1, showcell);
  }
  else if (Tess.Dim == 2)
  {
    (*pPrint).showface =
      ut_realloc_1d_int ((*pPrint).showface, Tess.FaceQty + 1);
    ut_array_1d_int_memcpy ((*pPrint).showface, Tess.FaceQty + 1, showcell);
  }

  ut_free_1d_int (showcell);
  ut_free_2d_char (vars, varqty);
  ut_free_1d (vals);
  ut_free_1d_char (cell);

  return;
}

void
nev_show_tess_faceinter (char **argv, int *pi, struct PRINT *pPrint)
{
  (*pi)++;
  ut_string_int (argv[(*pi)], &((*pPrint).showfaceinter));

  return;
}
