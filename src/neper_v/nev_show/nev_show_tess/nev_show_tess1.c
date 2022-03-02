/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
nev_show_tess_entity (char **argv, int *pi, struct TESS Tess, char *entity,
                      struct PRINT *pPrint)
{
  int i, j, status, entityqty, varqty = 0;
  double res;
  char **vars = NULL;
  double *vals = NULL;

  int **pshow = NULL;

  if (!strcmp (entity, "ver"))
    pshow = &(*pPrint).showver;
  else if (!strcmp (entity, "edge"))
    pshow = &(*pPrint).showedge;
  else if (!strcmp (entity, "face"))
    pshow = &(*pPrint).showface;
  else if (!strcmp (entity, "poly"))
    pshow = &(*pPrint).showpoly;
  else if (!strcmp (entity, "cell"))
    pshow = Tess.Dim == 2 ? &(*pPrint).showface : &(*pPrint).showpoly;
  else if (!strcmp (entity, "seed"))
    pshow = &(*pPrint).showseed;
  else if (!strcmp (entity, "crystal"))
    pshow = &(*pPrint).showcrystal;

  neut_tess_entity_qty (Tess, entity, &entityqty);

  (*pi)++;
  (*pshow) = ut_realloc_1d_int (*pshow, entityqty + 1);
  ut_array_1d_int_zero (*pshow, entityqty + 1);

  // checking for a general expression
  status = nev_show_genexpr (argv[(*pi)], *pshow, entityqty);

  if (status != 0)
  {
    ut_math_vars (argv[(*pi)], &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= entityqty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        status = neut_tess_var_val_one (Tess, (*pPrint).showedge,
                                        (*pPrint).showface,
                                        (*pPrint).showpoly, entity, i, vars[j],
                                        vals + j, NULL);
        if (status)
          ut_print_exprbug (vars[j]);
      }

      status = ut_math_eval (argv[(*pi)], varqty, vars, vals, &res);
      if (!status)
        (*pshow)[i] = res;
      else
        ut_print_exprbug (argv[(*pi)]);
    }
  }

  (*pshow)[0] =
    ut_array_1d_int_sum ((*pshow) + 1, entityqty);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}

void
nev_show_tess_faceinter (char **argv, int *pi, struct PRINT *pPrint)
{
  ut_string_int (argv[++(*pi)], &((*pPrint).showfaceinter));

  return;
}
