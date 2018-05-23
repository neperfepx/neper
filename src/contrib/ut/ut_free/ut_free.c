/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

void
ut_free_1d_int (int *res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_ushort (unsigned short *res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_uchar (unsigned char *res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_uint (unsigned int *res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_sizet (size_t * res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_float (float *res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_double (double *res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_pint (int **res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_pchar (char **res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_ppint (int ***res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_ppint_ (int ****pres)
{
  if (!(*pres))
    return;

  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_ppchar (char ***res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_pdouble (double **res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d_ppdouble (double ***res)
{
  if (!res)
    return;

  free (res);
  res = NULL;

  return;
}

void
ut_free_1d (double *res)
{
  if (!res)
    return;

  ut_free_1d_double (res);

  return;
}

void
ut_free_2d_int (int **array, unsigned int line)
{
  int i;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
  {
    if (array[i] != NULL)
      free (array[i]);
  }

  free (array);
  array = NULL;

  return;
}

void
ut_free_2d_int_ (int ***parray, unsigned int line)
{
  int i;

  if (!(*parray))
    return;

  for (i = line - 1; i >= 0; i--)
  {
    if ((*parray)[i] != NULL)
      free ((*parray)[i]);
  }

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_2d_ (double ***parray, unsigned int line)
{
  int i;

  if (!(*parray))
    return;

  for (i = line - 1; i >= 0; i--)
  {
    if ((*parray)[i] != NULL)
      free ((*parray)[i]);
  }

  free ((*parray));
  (*parray) = NULL;

  return;
}

void
ut_free_2d_ushort (unsigned short **array, unsigned int line)
{
  int i;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
  {
    if (array[i] != NULL)
      free (array[i]);
  }

  free (array);
  array = NULL;

  return;
}

void
ut_free_2d_uint (unsigned int **array, unsigned int line)
{
  int i;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_2d_float (float **array, unsigned int line)
{
  int i;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_2d_double (double **array, unsigned int line)
{
  int i;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_2d (double **array, unsigned int line)
{
  if (!array)
    return;

  ut_free_2d_double (array, line);
  array = NULL;

  return;
}

void
ut_free_3d_int (int ***array, unsigned int line, unsigned int column)
{
  int i, j;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_3d_ushort (unsigned short ***array, unsigned int line, unsigned int column)
{
  int i, j;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_3d_uchar (unsigned char ***array, unsigned int line, unsigned int column)
{
  int i, j;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_3d_char (char ***array, unsigned int line, unsigned int column)
{
  int i, j;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_3d (double ***array, unsigned int line, unsigned int column)
{
  int i, j;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_4d (double ****array, unsigned int line, unsigned int column, unsigned int depth)
{
  int i, j, k;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      for (k = depth - 1; k >= 0; k--)
	free (array[i][j][k]);

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_4d_int (int ****array, unsigned int line, unsigned int column, unsigned int depth)
{
  int i, j, k;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      for (k = depth - 1; k >= 0; k--)
	free (array[i][j][k]);

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_3d_float (float ***array, unsigned int line, unsigned int column)
{
  int i, j;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

void
ut_free_1d_char (char *string)
{
  if (!string)
    return;

  free (string);
  string = NULL;

  return;
}

void
ut_free_2d_char (char **array, unsigned int line)
{
  int i;

  if (!array)
    return;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);
  array = NULL;

  return;
}

extern void
ut_free_1d_pfile (FILE ** files)
{
  if (!files)
    return;

  free (files);
  files = NULL;

  return;
}

void
ut_free_1d_int_ (int **pres)
{
  if (!(*pres))
    return;

  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_ (double **pres)
{
  if (!(*pres))
    return;

  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_1d_char_ (char **pres)
{
  if (!(*pres))
    return;

  free (*pres);
  (*pres) = NULL;

  return;
}

void
ut_free_2d_contig (double **p)
{
  if (!p)
    return;

  free (p);
  p = NULL;
}

void
ut_free_2d_contig_long (long **p)
{
  if (!p)
    return;

  free (p);
  p = NULL;
}
