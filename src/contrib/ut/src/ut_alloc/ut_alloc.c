/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

void ut_alloc_check (void *ptr, int size);

double *
ut_alloc_1d_double (unsigned int size)
{
  double *array = calloc (size, sizeof (double));
  ut_alloc_check (array, size);

  return array;
}

double *
ut_alloc_1d (unsigned int size)
{
  return ut_alloc_1d_double (size);
}

float *
ut_alloc_1d_float (unsigned int size)
{
  float *array = calloc (size, sizeof (float));
  ut_alloc_check (array, size);

  return array;
}

int *
ut_alloc_1d_int (unsigned int size)
{
  int *array = calloc (size, sizeof (int));
  ut_alloc_check (array, size);

  return array;
}

unsigned int *
ut_alloc_1d_uint (unsigned int size)
{
  unsigned int *array = calloc (size, sizeof (int));
  ut_alloc_check (array, size);

  return array;
}

unsigned short *
ut_alloc_1d_ushort (unsigned int size)
{
  unsigned short *array = calloc (size, sizeof (int));
  ut_alloc_check (array, size);

  return array;
}

char *
ut_alloc_1d_char (unsigned int size)
{
  char *array = calloc (size, sizeof (char));
  ut_alloc_check (array, size);

  return array;
}

unsigned char *
ut_alloc_1d_uchar (unsigned int size)
{
  unsigned char *array = calloc (size, sizeof (int));
  ut_alloc_check (array, size);

  return array;
}

double **
ut_alloc_1d_pdouble (unsigned int size)
{
  unsigned int i;

  double **array = calloc (size, sizeof (double *));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

int **
ut_alloc_1d_pint (unsigned int size)
{
  unsigned int i;

  int **array = calloc (size, sizeof (int *));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

unsigned short **
ut_alloc_1d_pushort (unsigned int size)
{
  unsigned int i;

  unsigned short **array = calloc (size, sizeof (unsigned short *));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

unsigned char **
ut_alloc_1d_puchar (unsigned int size)
{
  unsigned int i;

  unsigned char **array = calloc (size, sizeof (unsigned char *));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

char **
ut_alloc_1d_pchar (unsigned int size)
{
  unsigned int i;

  char **array = calloc (size, sizeof (char *));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

int ***
ut_alloc_1d_ppint (unsigned int size)
{
  unsigned int i;

  int ***array = calloc (size, sizeof (int **));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

unsigned short ***
ut_alloc_1d_ppushort (unsigned int size)
{
  unsigned int i;

  unsigned short ***array = calloc (size, sizeof (int **));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

unsigned char ***
ut_alloc_1d_ppuchar (unsigned int size)
{
  unsigned int i;

  unsigned char ***array = calloc (size, sizeof (int **));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

double ***
ut_alloc_1d_ppdouble (unsigned int size)
{
  unsigned int i;

  double ***array = calloc (size, sizeof (double **));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

double ****
ut_alloc_1d_pppdouble (unsigned int size)
{
  unsigned int i;

  double ****array = calloc (size, sizeof (double ***));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

char ***
ut_alloc_1d_ppchar (unsigned int size)
{
  unsigned int i;

  char ***array = calloc (size, sizeof (char **));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

FILE **
ut_alloc_1d_pfile (unsigned int size)
{
  unsigned int i;

  FILE **array = calloc (size, sizeof (FILE *));
  ut_alloc_check (array, size);
  for (i = 0; i < size; i++)
    array[i] = NULL;

  return array;
}

int **
ut_alloc_2d_int (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  int **array = calloc (size1, sizeof (int *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (int));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

unsigned short **
ut_alloc_2d_ushort (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  unsigned short **array = calloc (size1, sizeof (unsigned short *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (unsigned short));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

unsigned char **
ut_alloc_2d_uchar (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  unsigned char **array = calloc (size1, sizeof (unsigned char *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (unsigned char));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

unsigned int **
ut_alloc_2d_uint (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  unsigned int **array = calloc (size1, sizeof (int *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (int));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

float **
ut_alloc_2d_float (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  float **array = calloc (size1, sizeof (float *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (float));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

double **
ut_alloc_2d_double (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  double **array = calloc (size1, sizeof (double *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (double));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

double **
ut_alloc_2d (unsigned int size1, unsigned int size2)
{
  return ut_alloc_2d_double (size1, size2);
}

char **
ut_alloc_2d_char (unsigned int size1, unsigned int size2)
{
  unsigned int i;
  char **array = calloc (size1, sizeof (char *));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (char));
    ut_alloc_check (array[i], size2);
  }

  return array;
}

int ***
ut_alloc_2d_pint (unsigned int size1, unsigned int size2)
{
  unsigned int i, j;
  int ***array = calloc (size1, sizeof (int **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (int *));
    ut_alloc_check (array[i], size2);
    for (j = 0; j < size2; j++)
      array[i][j] = NULL;
  }

  return array;
}

char ***
ut_alloc_2d_pchar (unsigned int size1, unsigned int size2)
{
  unsigned int i, j;
  char ***array = calloc (size1, sizeof (char **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (char *));
    ut_alloc_check (array[i], size2);
    for (j = 0; j < size2; j++)
      array[i][j] = NULL;
  }

  return array;
}

int ***
ut_alloc_3d_int (unsigned int size1, unsigned int size2, unsigned int size3)
{
  unsigned int i, j;
  int ***array = calloc (size1, sizeof (int **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (int *));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (int));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

unsigned short ***
ut_alloc_3d_ushort (unsigned int size1, unsigned int size2,
                    unsigned int size3)
{
  unsigned int i, j;
  unsigned short ***array = calloc (size1, sizeof (unsigned short **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (unsigned short *));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (unsigned short));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

unsigned char ***
ut_alloc_3d_uchar (unsigned int size1, unsigned int size2, unsigned int size3)
{
  unsigned int i, j;
  unsigned char ***array = calloc (size1, sizeof (unsigned char **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (unsigned char *));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (unsigned char));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

unsigned int ***
ut_alloc_3d_uint (unsigned int size1, unsigned int size2, unsigned int size3)
{
  unsigned int i, j;
  unsigned int ***array = calloc (size1, sizeof (unsigned int **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (unsigned int *));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (unsigned int));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

double ***
ut_alloc_3d_double (unsigned int size1, unsigned int size2,
                    unsigned int size3)
{
  unsigned int i, j;
  double ***array = calloc (size1, sizeof (double **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (double *));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (double));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

float ***
ut_alloc_3d_float (unsigned int size1, unsigned int size2, unsigned int size3)
{
  unsigned int i, j;
  float ***array = calloc (size1, sizeof (float **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (float *));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (float));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

double ***
ut_alloc_3d (unsigned int size1, unsigned int size2, unsigned int size3)
{
  return ut_alloc_3d_double (size1, size2, size3);
}

char ***
ut_alloc_3d_char (unsigned int size1, unsigned int size2, unsigned int size3)
{
  unsigned int i, j;
  char ***array = calloc (size1, sizeof (char **));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (char *));
    ut_alloc_check (array[i], size2);
    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (char));
      ut_alloc_check (array[i][j], size3);
    }
  }

  return array;
}

double ****
ut_alloc_4d_double (unsigned int size1, unsigned int size2,
                    unsigned int size3, unsigned int size4)
{
  unsigned int i, j, k;
  double ****array = calloc (size1, sizeof (double ***));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (double **));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (double *));
      ut_alloc_check (array[i][j], size3);

      for (k = 0; k < size3; k++)
      {
        array[i][j][k] = calloc (size4, sizeof (double));
        ut_alloc_check (array[i][j][k], size4);
      }
    }
  }

  return array;
}

int ****
ut_alloc_4d_int (unsigned int size1, unsigned int size2, unsigned int size3,
                 unsigned int size4)
{
  unsigned int i, j, k;
  int ****array = calloc (size1, sizeof (int ***));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (int **));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (int *));
      ut_alloc_check (array[i][j], size3);

      for (k = 0; k < size3; k++)
      {
        array[i][j][k] = calloc (size4, sizeof (int));
        ut_alloc_check (array[i][j][k], size4);
      }
    }
  }

  return array;
}

int ****
ut_alloc_3d_pint (unsigned int size1, unsigned int size2, unsigned int size3)
{
  unsigned int i, j, k;
  int ****array = calloc (size1, sizeof (int ***));
  ut_alloc_check (array, size1);

  for (i = 0; i < size1; i++)
  {
    array[i] = calloc (size2, sizeof (int **));
    ut_alloc_check (array[i], size2);

    for (j = 0; j < size2; j++)
    {
      array[i][j] = calloc (size3, sizeof (int *));
      ut_alloc_check (array[i][j], size3);

      for (k = 0; k < size3; k++)
        array[i][j][k] = NULL;
    }
  }

  return array;
}

double ****
ut_alloc_4d (unsigned int size1, unsigned int size2, unsigned int size3,
             unsigned int size4)
{
  return ut_alloc_4d_double (size1, size2, size3, size4);
}

int *
ut_realloc_1d_int (int *array, unsigned int size)
{
  array = realloc (array, size * sizeof (int));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

int *
ut_realloc_1d_int_zero (int *array, unsigned int size, unsigned int zero)
{
  unsigned int i;

  if (zero > size)
    abort ();

  array = ut_realloc_1d_int (array, size);
  if (size == 0)
    free (array);

  for (i = 0; i < zero; i++)
    array[size - 1 - i] = 0;

  return size ? array : NULL;
}

unsigned short *
ut_realloc_1d_ushort (unsigned short *array, unsigned int size)
{
  array = realloc (array, size * sizeof (unsigned short));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

unsigned char *
ut_realloc_1d_uchar (unsigned char *array, unsigned int size)
{
  array = realloc (array, size * sizeof (unsigned char));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

double *
ut_realloc_1d (double *array, unsigned int size)
{
  return ut_realloc_1d_double (array, size);
}

double *
ut_realloc_1d_double (double *array, unsigned int size)
{
  array = realloc (array, size * sizeof (double));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

char *
ut_realloc_1d_char (char *array, unsigned int size)
{
  array = realloc (array, size * sizeof (char));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

int **
ut_realloc_1d_pint (int **array, unsigned int size)
{
  array = realloc (array, size * sizeof (int *));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

unsigned short **
ut_realloc_1d_pushort (unsigned short **array, unsigned int size)
{
  array = realloc (array, size * sizeof (unsigned short *));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

unsigned char **
ut_realloc_1d_puchar (unsigned char **array, unsigned int size)
{
  array = realloc (array, size * sizeof (unsigned char *));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

char **
ut_realloc_1d_pchar (char **array, unsigned int size)
{
  array = realloc (array, size * sizeof (char *));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

char **
ut_realloc_1d_pchar_null (char **array, unsigned int size, unsigned int null)
{
  unsigned int i;

  if (null > size)
    abort ();

  array = ut_realloc_1d_pchar (array, size);

  if (array)
    for (i = 0; i < null; i++)
      array[size - 1 - i] = NULL;

  return size ? array : NULL;
}

char ***
ut_realloc_1d_ppchar_null (char ***array, unsigned int size, unsigned int null)
{
  unsigned int i;

  if (null > size)
    abort ();

  array = ut_realloc_1d_ppchar (array, size);

  if (array)
    for (i = 0; i < null; i++)
      array[size - 1 - i] = NULL;

  return size ? array : NULL;
}

int **
ut_realloc_1d_pint_null (int **array, unsigned int size, unsigned int null)
{
  unsigned int i;

  if (null > size)
    abort ();

  array = ut_realloc_1d_pint (array, size);

  if (array)
    for (i = 0; i < null; i++)
      array[size - 1 - i] = NULL;

  return size ? array : NULL;
}

int ***
ut_realloc_1d_ppint_null (int ***array, unsigned int size, unsigned int null)
{
  unsigned int i;

  if (null > size)
    abort ();

  array = ut_realloc_1d_ppint (array, size);

  if (array)
    for (i = 0; i < null; i++)
      array[size - 1 - i] = NULL;

  return size ? array : NULL;
}

double **
ut_realloc_1d_pdouble (double **array, unsigned int size)
{
  array = realloc (array, size * sizeof (double *));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

double **
ut_realloc_1d_pdouble_null (double **array, unsigned int size, unsigned int null)
{
  unsigned int i;

  if (null > size)
    abort ();

  array = ut_realloc_1d_pdouble (array, size);

  if (array)
    for (i = 0; i < null; i++)
      array[size - 1 - i] = NULL;

  return size ? array : NULL;
}

double ***
ut_realloc_1d_ppdouble (double ***array, unsigned int size)
{
  array = realloc (array, size * sizeof (double **));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

int ***
ut_realloc_1d_ppint (int ***array, unsigned int size)
{
  array = realloc (array, size * sizeof (int **));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

char ***
ut_realloc_1d_ppchar (char ***array, unsigned int size)
{
  array = realloc (array, size * sizeof (char **));
  if (size == 0)
    free (array);

  return size ? array : NULL;
}

int **
ut_realloc_2d_int_addline (int **array, int size1, int size2)
{
  array = ut_realloc_1d_pint (array, size1);
  array[size1 - 1] = ut_alloc_1d_int (size2);

  return size1 ? array : NULL;
}

int **
ut_realloc_2d_int_size2 (int **array, int size1, int size2_before,
                         int size2_after)
{
  int i, j;

  for (i = 0; i < size1; i++)
  {
    array[i] = ut_realloc_1d_int (array[i], size2_after);
    for (j = size2_before; j < size2_after; j++)
      array[i][j] = 0;
  }

  return array;
}

unsigned short **
ut_realloc_2d_ushort_addline (unsigned short **array, int size1, int size2)
{
  array = ut_realloc_1d_pushort (array, size1);
  array[size1 - 1] = ut_alloc_1d_ushort (size2);

  return array;
}

unsigned char **
ut_realloc_2d_uchar_addline (unsigned char **array, int size1, int size2)
{
  array = ut_realloc_1d_puchar (array, size1);
  array[size1 - 1] = ut_alloc_1d_uchar (size2);

  return array;
}

double **
ut_realloc_2d_addline (double **array, int size1, int size2)
{
  return ut_realloc_2d_double_addline (array, size1, size2);
}

double **
ut_realloc_2d_double_addline (double **array, int size1, int size2)
{
  array = ut_realloc_1d_pdouble (array, size1);
  array[size1 - 1] = ut_alloc_1d (size2);

  return array;
}

char **
ut_realloc_2d_char_addline (char **array, int size1, int size2)
{
  array = ut_realloc_1d_pchar (array, size1);
  array[size1 - 1] = ut_alloc_1d_char (size2);

  return array;
}

int **
ut_realloc_2d_int_delline (int **array, int size_before, int size_after)
{
  int i;
  for (i = size_before - 1; i >= size_after; i--)
    ut_free_1d_int (&array[i]);
  array = ut_realloc_1d_pint (array, size_after);

  return array;
}

unsigned short **
ut_realloc_2d_ushort_delline (unsigned short **array, int size_before,
                              int size_after)
{
  int i;
  for (i = size_before - 1; i >= size_after; i--)
    ut_free_1d_ushort (array + i);
  array = ut_realloc_1d_pushort (array, size_after);

  return array;
}

unsigned char **
ut_realloc_2d_uchar_delline (unsigned char **array, int size_before,
                             int size_after)
{
  int i;
  for (i = size_before - 1; i >= size_after; i--)
    ut_free_1d_uchar (&array[i]);
  array = ut_realloc_1d_puchar (array, size_after);

  return array;
}

double **
ut_realloc_2d_delline (double **array, int size_before, int size_after)
{
  return ut_realloc_2d_double_delline (array, size_before, size_after);
}

double **
ut_realloc_2d_double_delline (double **array, int size_before, int size_after)
{
  int i;
  for (i = size_before - 1; i >= size_after; i--)
    ut_free_1d (&array[i]);
  array = ut_realloc_1d_pdouble (array, size_after);

  return array;
}

double ***
ut_realloc_3d_addarray (double ***array, int size1, int size2, int size3)
{
  return ut_realloc_3d_double_addarray (array, size1, size2, size3);
}

double ***
ut_realloc_3d_double_addarray (double ***array, int size1, int size2,
                               int size3)
{
  array = ut_realloc_1d_ppdouble (array, size1);
  array[size1 - 1] = ut_alloc_2d (size2, size3);

  return array;
}

int ***
ut_realloc_3d_int_addarray (int ***array, int size1, int size2, int size3)
{
  array = ut_realloc_1d_ppint (array, size1);
  array[size1 - 1] = ut_alloc_2d_int (size2, size3);

  return array;
}

double **
ut_alloc_2d_contig (unsigned int size1, unsigned int size2)
{
  unsigned int r;

  /* allocate chunk: size1 times the pointer, size1 * size2 times the value */
  double **array =
    malloc (size1 * sizeof (double *) + size1 * size2 * sizeof (double));

  /* calculate pointer to first row: point directly behind the pointers,
   * then cast */
  double *row = (double *) (array + size1);

  /* set all row pointers */
  for (r = 0; r < size1; ++r)
  {
    array[r] = row;
    row += size2;
  }
  unsigned int i, j;
  for (i = 0; i < size1; ++i)
    for (j = 0; j < size2; ++j)
      array[i][j] = 0;

  return array;
}

long **
ut_alloc_2d_contig_long (unsigned int size1, unsigned int size2)
{
  unsigned int r;

  /* allocate chunk: size1 times the pointer, size1 * size2 times the value */
  long **array =
    malloc (size1 * sizeof (long *) + size1 * size2 * sizeof (long));

  /* calculate pointer to first row: point directly behind the pointers,
   * then cast */
  long *row = (long *) (array + size1);

  /* set all row pointers */
  for (r = 0; r < size1; ++r)
  {
    array[r] = row;
    row += size2;
  }
  size_t i, j;
  for (i = 0; i < size1; ++i)
    for (j = 0; j < size2; ++j)
      array[i][j] = 0;

  return array;
}

void
ut_alloc_check (void *ptr, int size)
{
  if (size > 0 && !ptr)
    ut_print_message (2, 0, "memory allocation failed! (%d)\n");
}
