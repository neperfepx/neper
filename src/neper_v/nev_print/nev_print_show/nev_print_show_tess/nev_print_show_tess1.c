/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_show_tess_.h"

void
nev_print_show_tess_entity (char *entity, char *expr, struct TESS *pTess,
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
    pshow = (*pTess).Dim == 2 ? &(*pPrint).showface : &(*pPrint).showpoly;
  else if (!strcmp (entity, "seed"))
    pshow = &(*pPrint).showseed;
  else if (!strcmp (entity, "crystal"))
    pshow = &(*pPrint).showcrystal;

  neut_tess_entity_qty (*pTess, entity, &entityqty);

  (*pshow) = ut_realloc_1d_int (*pshow, entityqty + 1);
  ut_array_1d_int_zero (*pshow, entityqty + 1);

  // checking for a general expression
  status = nev_print_show_genexpr (expr, *pshow, entityqty);

  if (status != 0)
  {
    ut_math_vars (expr, &vars, &varqty);
    vals = ut_alloc_1d (varqty);

    for (i = 1; i <= entityqty; i++)
    {
      for (j = 0; j < varqty; j++)
      {
        status = neut_tess_var_val_one (pTess, (*pPrint).showedge,
                                        (*pPrint).showface,
                                        (*pPrint).showpoly, entity, i, vars[j],
                                        vals + j, NULL);
        if (status)
          ut_print_exprbug (vars[j]);
      }

      status = ut_math_eval (expr, varqty, vars, vals, &res);
      if (!status)
        (*pshow)[i] = res;
      else
        ut_print_exprbug (expr);
    }
  }

  (*pshow)[0] =
    ut_array_1d_int_sum ((*pshow) + 1, entityqty);

  ut_free_2d_char (&vars, varqty);
  ut_free_1d (&vals);

  return;
}
