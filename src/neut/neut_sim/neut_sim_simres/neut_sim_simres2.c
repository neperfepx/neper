/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_simres_.h"

void
neut_sim_simres_parentres (char *res, struct SIMRES *pSimRes)
{
  char *digits = NULL;

  ut_string_removetrailingdigits (res, &(*pSimRes).parentres);
  ut_string_trailingdigits (res, &digits);

  neut_sim_simres_type ((*pSimRes).parentres, pSimRes);

  if (strlen (digits))
  {
    if (!strcmp ((*pSimRes).type, "tensor") && strlen (digits) != 2)
      ut_print_exprbug (res);

    if (!strcmp ((*pSimRes).type, "vector"))
      (*pSimRes).parentcol = atoi (digits);
    else if (!strcmp ((*pSimRes).type, "tensor"))
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

int
neut_sim_simres_type (char *res, struct SIMRES *pSimRes)
{
  ut_string_string ("", &(*pSimRes).type);
  (*pSimRes).colqty = 0;

  if (strlen (res) == 0)
    return -1;

  if (!strcmp (res, "coo")
   || !strcmp (res, "disp")
   || !strcmp (res, "vel"))
  {
    ut_string_string ("vector", &(*pSimRes).type);
    (*pSimRes).colqty = 3;
  }

  else if (!strcmp (res, "spinrate"))
  {
    ut_string_string ("tensor", &(*pSimRes).type);
    (*pSimRes).colqty = 3;
  }

  else if (!strcmp (res, "velgrad"))
  {
    ut_string_string ("tensor", &(*pSimRes).type);
    (*pSimRes).colqty = 9;
  }

  else if (!strcmp (res, "strain")
        || !strcmp (res, "strain-el")
        || !strcmp (res, "strain-pl")
        || !strcmp (res, "stress"))
  {
    ut_string_string ("tensor", &(*pSimRes).type);
    (*pSimRes).colqty = 6;
  }

  else if (!strcmp (res, "ori"))
  {
    ut_string_string ("ori", &(*pSimRes).type);
    (*pSimRes).colqty = 0;
  }

  else if (!strncmp (res, "work", 4)
        || strstr (res, "energy")
        || isdigit (res[strlen (res) - 1])
        || strstr (res, "-eq"))
  {
    ut_string_string ("real", &(*pSimRes).type);
    (*pSimRes).colqty = 1;
  }

  else
  {
    char *tmp = ut_alloc_1d_char (1000);
    FILE *fp = NULL;

    (*pSimRes).colqty = ut_file_nbcolumns ((*pSimRes).file);

    if ((*pSimRes).colqty == 1)
    {
      fp = ut_file_open ((*pSimRes).file, "R");
      ut_file_nextstring (fp, tmp);
      ut_string_string (strstr (tmp, ".") ? "real" : "int", &(*pSimRes).type);
      ut_file_close (fp, (*pSimRes).file, "R");
    }
    else if ((*pSimRes).colqty == 6 || (*pSimRes).colqty == 9)
      ut_string_string ("tensor", &(*pSimRes).type);
    else
      ut_string_string ("vector", &(*pSimRes).type);

    ut_free_1d_char (&tmp);
  }

  return strlen ((*pSimRes).type) > 0 ? 0 : -1;
}
