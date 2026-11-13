/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_simres_.h"

void
neut_sim_simres_parentres (struct SIM Sim, char *entity, char *res, struct SIMRES *pSimRes)
{
  int filecolqty, istensor;
  char *digits = NULL;

  ut_string_removetrailingdigits (res, &(*pSimRes).parentres);
  ut_string_trailingdigits (res, &digits);

  neut_sim_simres_type (Sim, entity, (*pSimRes).parentres, pSimRes);

  if (strlen (digits))
  {
    if (!strlen ((*pSimRes).type))
      ut_print_message (2, 4, "%s: the type of `%s' is undefined (no type in `%s.def').\n",
                        res, (*pSimRes).parentres, (*pSimRes).parentres);

    istensor = strstr ((*pSimRes).type, "tensor") != NULL;

    if (strcmp ((*pSimRes).type, "vector") && !istensor)
      ut_print_message (2, 4, "%s: `%s' is not a vector or a tensor (type: %s).\n",
                        res, (*pSimRes).parentres, (*pSimRes).type);

    filecolqty = ut_file_nbcolumns ((*pSimRes).file);

    // the file may be missing for a given step
    if (filecolqty > 0 && (*pSimRes).colqty != filecolqty)
      ut_print_message (2, 4, "%s: `%s' has %d column(s), while its type (%s) implies %d.\n",
                        res, (*pSimRes).parentres, filecolqty, (*pSimRes).type, (*pSimRes).colqty);

    if (istensor && strlen (digits) != 2)
      ut_print_exprbug (res);

    if (!strcmp ((*pSimRes).type, "vector"))
    {
      (*pSimRes).parentcol = atoi (digits);

      if ((*pSimRes).parentcol < 1
          || ((*pSimRes).colqty > 0 && (*pSimRes).parentcol > (*pSimRes).colqty))
        ut_print_message (2, 4, "%s: `%s' is a vector of %d component(s).\n",
                          res, (*pSimRes).parentres, (*pSimRes).colqty);
    }
    else if (istensor)
    {
      if ((*pSimRes).colqty == 3)
      {
        if (!strcmp (digits, "11"))
          (*pSimRes).parentcol = 0;
        else if (!strcmp (digits, "12"))
          (*pSimRes).parentcol = 1;
        else if (!strcmp (digits, "13"))
          (*pSimRes).parentcol = 2;
        else if (!strcmp (digits, "21"))
          (*pSimRes).parentcol = -1;
        else if (!strcmp (digits, "22"))
          (*pSimRes).parentcol = 0;
        else if (!strcmp (digits, "23"))
          (*pSimRes).parentcol = 3;
        else if (!strcmp (digits, "31"))
          (*pSimRes).parentcol = -2;
        else if (!strcmp (digits, "32"))
          (*pSimRes).parentcol = -3;
        else if (!strcmp (digits, "33"))
          (*pSimRes).parentcol = 0;
        else
          ut_print_exprbug (res);
      }
      else if ((*pSimRes).colqty == 6)
      {
        if (!strcmp (digits, "11"))
          (*pSimRes).parentcol = 1;
        else if (!strcmp (digits, "22"))
          (*pSimRes).parentcol = 2;
        else if (!strcmp (digits, "33"))
          (*pSimRes).parentcol = 3;
        else if (!strcmp (digits, "23") || !strcmp (digits, "32"))
          (*pSimRes).parentcol = 4;
        else if (!strcmp (digits, "31") || !strcmp (digits, "13"))
          (*pSimRes).parentcol = 5;
        else if (!strcmp (digits, "12") || !strcmp (digits, "21"))
          (*pSimRes).parentcol = 6;
        else
          ut_print_exprbug (res);
      }
      else if ((*pSimRes).colqty == 9)
      {
        if (!strcmp (digits, "11"))
          (*pSimRes).parentcol = 1;
        else if (!strcmp (digits, "12"))
          (*pSimRes).parentcol = 2;
        else if (!strcmp (digits, "13"))
          (*pSimRes).parentcol = 3;
        else if (!strcmp (digits, "21"))
          (*pSimRes).parentcol = 4;
        else if (!strcmp (digits, "22"))
          (*pSimRes).parentcol = 5;
        else if (!strcmp (digits, "23"))
          (*pSimRes).parentcol = 6;
        else if (!strcmp (digits, "31"))
          (*pSimRes).parentcol = 7;
        else if (!strcmp (digits, "32"))
          (*pSimRes).parentcol = 8;
        else if (!strcmp (digits, "33"))
          (*pSimRes).parentcol = 9;
        else
          ut_print_exprbug (res);
      }
      else
        ut_print_exprbug (res);
    }
  }

  ut_free_1d_char (&digits);

  return;
}

// the type is that of the definition file, if any, and that of the name otherwise.  It is undefined
// ("") in other cases, in which case the number of columns is still set, from the file.
int
neut_sim_simres_type (struct SIM Sim, char *entity, char *res, struct SIMRES *pSimRes)
{
  char *type = NULL;

  ut_string_string ("", &(*pSimRes).type);
  (*pSimRes).colqty = 0;

  if (strlen (res) == 0)
    return -1;

  neut_sim_simres_type_read (Sim, entity, res, &type);

  if (!type)
    neut_sim_simres_knowntype (res, &type);

  if (type)
  {
    ut_string_string (type, &(*pSimRes).type);
    neut_sim_simres_type_colqty (type, &(*pSimRes).colqty);
  }

  if (!strcmp ((*pSimRes).type, "") || !strcmp ((*pSimRes).type, "vector"))
    (*pSimRes).colqty = ut_file_nbcolumns ((*pSimRes).file);

  ut_free_1d_char (&type);

  return strlen ((*pSimRes).type) > 0 ? 0 : -1;
}
