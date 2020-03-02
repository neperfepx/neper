/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

void
ut_free_1d_pint (int ***pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_pdouble (double ***pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_ppint (int ****pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_ppdouble (double ****pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_ppchar (char ****pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_2d_int (int ***parray, unsigned int line)
{
  int i;

  if (!(*parray))
    return;

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));

  return;
}

void
ut_free_2d_uint (unsigned int ***parray, unsigned int line)
{
  int i;

  if (!(*parray))
    return;

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));

  return;
}

void
ut_free_2d (double ***parray, unsigned int line)
{
  int i;

  if (!(*parray))
    return;

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));

  return;
}

void
ut_free_2d_char (char ***parray, unsigned int line)
{
  int i;

  if (!(*parray))
    return;

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));

  return;
}

void
ut_free_3d_int (int ****parray, unsigned int line, unsigned int column)
{
  int i, j;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_3d_ushort (unsigned short ****parray, unsigned int line,
                   unsigned int column)
{
  int i, j;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_3d_uchar (unsigned char ****parray, unsigned int line,
                  unsigned int column)
{
  int i, j;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_3d_char (char ****parray, unsigned int line, unsigned int column)
{
  int i, j;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_3d (double ****parray, unsigned int line, unsigned int column)
{
  int i, j;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_4d (double *****parray, unsigned int line, unsigned int column,
            unsigned int depth)
{
  int i, j, k;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      for (k = depth - 1; k >= 0; k--)
        free ((*parray)[i][j][k]);

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_4d_int (int *****parray, unsigned int line, unsigned int column,
                unsigned int depth)
{
  int i, j, k;

  if (!*parray)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      for (k = depth - 1; k >= 0; k--)
        free ((*parray)[i][j][k]);

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free ((*parray)[i][j]);

  for (i = line - 1; i >= 0; i--)
    free ((*parray)[i]);

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_1d_int (int **pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_ushort (unsigned short **pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_uchar (unsigned char **pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d (double **pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_char (char **pres)
{
  free (*pres);
  (*pres) = NULL;

  return;
}
