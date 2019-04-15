/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include"ut.h"

double *
ut_alloc_1d_double (unsigned int line)
{
  double *res = (double *) calloc (line, sizeof (double));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

double *
ut_alloc_1d (unsigned int line)
{
  return ut_alloc_1d_double (line);
}

float *
ut_alloc_1d_float (unsigned int line)
{
  float *res = (float *) calloc (line, sizeof (float));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

int *
ut_alloc_1d_int (unsigned int line)
{
  int *res = (int *) calloc (line, sizeof (int));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

size_t *
ut_alloc_1d_sizet (unsigned int line)
{
  size_t *res = (size_t *) calloc (line, sizeof (size_t));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned int *
ut_alloc_1d_uint (unsigned int line)
{
  unsigned int *res = (unsigned int *) calloc (line, sizeof (int));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned short *
ut_alloc_1d_ushort (unsigned int line)
{
  unsigned short *res = (unsigned short *) calloc (line, sizeof (int));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

char *
ut_alloc_1d_char (unsigned int line)
{
  char *res = (char *) calloc (line, sizeof (char));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned char *
ut_alloc_1d_uchar (unsigned int line)
{
  unsigned char *res = (unsigned char *) calloc (line, sizeof (int));
  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

double **
ut_alloc_1d_pdouble (unsigned int line)
{
  unsigned int i;

  double **res = (double **) calloc (line, sizeof (double *));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

int **
ut_alloc_1d_pint (unsigned int line)
{
  unsigned int i;

  int **res = (int **) calloc (line, sizeof (int *));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned short **
ut_alloc_1d_pushort (unsigned int line)
{
  unsigned int i;

  unsigned short **res =
    (unsigned short **) calloc (line, sizeof (unsigned short *));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned char **
ut_alloc_1d_puchar (unsigned int line)
{
  unsigned int i;

  unsigned char **res =
    (unsigned char **) calloc (line, sizeof (unsigned char *));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

char **
ut_alloc_1d_pchar (unsigned int line)
{
  unsigned int i;

  char **res = (char **) calloc (line, sizeof (char *));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

int ***
ut_alloc_1d_ppint (unsigned int line)
{
  unsigned int i;

  int ***res = (int ***) calloc (line, sizeof (int **));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned short ***
ut_alloc_1d_ppushort (unsigned int line)
{
  unsigned int i;

  unsigned short ***res = (unsigned short ***) calloc (line, sizeof (int **));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned char ***
ut_alloc_1d_ppuchar (unsigned int line)
{
  unsigned int i;

  unsigned char ***res = (unsigned char ***) calloc (line, sizeof (int **));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

double ***
ut_alloc_1d_ppdouble (unsigned int line)
{
  unsigned int i;

  double ***res = (double ***) calloc (line, sizeof (double **));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

char ***
ut_alloc_1d_ppchar (unsigned int line)
{
  unsigned int i;

  char ***res = (char ***) calloc (line, sizeof (char **));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

FILE **
ut_alloc_1d_pfile (unsigned int line)
{
  unsigned int i;

  FILE **res = (FILE **) calloc (line, sizeof (FILE *));
  for (i = 0; i < line; i++)
    res[i] = NULL;

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

int **
ut_alloc_2d_int (unsigned int line, unsigned int column)
{
  unsigned int i;
  int **res = (int **) calloc (line, sizeof (int *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (int *) calloc (column, sizeof (int));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

unsigned short **
ut_alloc_2d_ushort (unsigned int line, unsigned int column)
{
  unsigned int i;
  unsigned short **res =
    (unsigned short **) calloc (line, sizeof (unsigned short *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (unsigned short *) calloc (column, sizeof (unsigned short));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

unsigned char **
ut_alloc_2d_uchar (unsigned int line, unsigned int column)
{
  unsigned int i;
  unsigned char **res =
    (unsigned char **) calloc (line, sizeof (unsigned char *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (unsigned char *) calloc (column, sizeof (unsigned char));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

unsigned int **
ut_alloc_2d_uint (unsigned int line, unsigned int column)
{
  unsigned int i;
  unsigned int **res = (unsigned int **) calloc (line, sizeof (int *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (unsigned int *) calloc (column, sizeof (int));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

float **
ut_alloc_2d_float (unsigned int line, unsigned int column)
{
  unsigned int i;
  float **res = (float **) calloc (line, sizeof (float *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (float *) calloc (column, sizeof (float));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

double **
ut_alloc_2d_double (unsigned int line, unsigned int column)
{
  unsigned int i;
  double **res = (double **) calloc (line, sizeof (double *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (double *) calloc (column, sizeof (double));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

double **
ut_alloc_2d (unsigned int line, unsigned int column)
{
  return ut_alloc_2d_double (line, column);
}

char **
ut_alloc_2d_char (unsigned int line, unsigned int column)
{
  unsigned int i;
  char **res = (char **) calloc (line, sizeof (char *));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (char *) calloc (column, sizeof (char));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

int ***
ut_alloc_2d_pint (unsigned int line, unsigned int column)
{
  unsigned int i, j;
  int ***res = (int ***) calloc (line, sizeof (int **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (int **) calloc (column, sizeof (int *));
    for (j = 0; j < column; j++)
      res[i][j] = NULL;

    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

char ***
ut_alloc_2d_pchar (unsigned int line, unsigned int column)
{
  unsigned int i, j;
  char ***res = (char ***) calloc (line, sizeof (char **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%d)\n", line, column);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (char **) calloc (column, sizeof (char *));
    for (j = 0; j < column; j++)
      res[i][j] = NULL;

    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%d)\n", line, column);
      abort ();
    }
  }

  return res;
}

int ***
ut_alloc_3d_int (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  int ***res = (int ***) calloc (line, sizeof (int **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (int **) calloc (column, sizeof (int *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }

    for (j = 0; j < column; j++)
    {
      res[i][j] = (int *) calloc (depth, sizeof (int));
      if (!res[i][j])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

unsigned short ***
ut_alloc_3d_ushort (unsigned int line, unsigned int column,
		    unsigned int depth)
{
  unsigned int i, j;
  unsigned short ***res =
    (unsigned short ***) calloc (line, sizeof (unsigned short **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (unsigned short **) calloc (column, sizeof (unsigned short *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }

    for (j = 0; j < column; j++)
    {
      res[i][j] = (unsigned short *) calloc (depth, sizeof (unsigned short));
      if (!res[i][j])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

unsigned char ***
ut_alloc_3d_uchar (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  unsigned char ***res =
    (unsigned char ***) calloc (line, sizeof (unsigned char **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (unsigned char **) calloc (column, sizeof (unsigned char *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }

    for (j = 0; j < column; j++)
    {
      res[i][j] = (unsigned char *) calloc (depth, sizeof (unsigned char));
      if (!res[i][j])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

unsigned int ***
ut_alloc_3d_uint (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  unsigned int ***res = (unsigned int ***) calloc (line, sizeof
						   (unsigned int **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (unsigned int **) calloc (column, sizeof (unsigned int *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }

    for (j = 0; j < column; j++)
    {
      res[i][j] = (unsigned int *) calloc (depth, sizeof (unsigned int));
      if (!res[i][j])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

double ***
ut_alloc_3d_double (unsigned int line, unsigned int column,
		    unsigned int depth)
{
  unsigned int i, j;
  double ***res = (double ***) calloc (line, sizeof (double **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (double **) calloc (column, sizeof (double *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }

    for (j = 0; j < column; j++)
    {
      res[i][j] = (double *) calloc (depth, sizeof (double));
      if (!res[i][j])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

float ***
ut_alloc_3d_float (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  float ***res = (float ***) calloc (line, sizeof (float **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (float **) calloc (column, sizeof (float *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }

    for (j = 0; j < column; j++)
    {
      res[i][j] = (float *) calloc (depth, sizeof (float));
      if (!res[i][j])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

double ***
ut_alloc_3d (unsigned int line, unsigned int column, unsigned int depth)
{
  return ut_alloc_3d_double (line, column, depth);
}

char ***
ut_alloc_3d_char (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  char ***res = (char ***) calloc (line, sizeof (char **));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
    abort ();
  }

  for (i = 0; i < line; i++)
  {
    res[i] = (char **) calloc (column, sizeof (char *));
    if (!res[i])
    {
      printf ("memory allocation failed! (%dx%dx%d)\n", line, column, depth);
      abort ();
    }
    for (j = 0; j < column; j++)
    {
      res[i][j] = (char *) calloc (depth, sizeof (char));
      if (!res[i])
      {
	printf ("memory allocation failed! (%dx%dx%d)\n", line, column,
		depth);
	abort ();
      }
    }
  }

  return res;
}

double ****
ut_alloc_4d_double (unsigned int dim1, unsigned int dim2,
		    unsigned int dim3, unsigned int dim4)
{
  unsigned int i, j, k;
  double ****res = (double ****) calloc (dim1, sizeof (double ***));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%dx%d)\n", dim1, dim2, dim3,
	    dim4);
    abort ();
  }

  for (i = 0; i < dim1; i++)
  {
    res[i] = (double ***) calloc (dim2, sizeof (double **));
    if (!res[i])
    {
      printf ("memory allocation failed!\n");
      abort ();
    }

    for (j = 0; j < dim2; j++)
    {
      res[i][j] = (double **) calloc (dim3, sizeof (double *));
      if (!res[i][j])
      {
	printf ("memory allocation failed!\n");
	abort ();
      }

      for (k = 0; k < dim3; k++)
      {
	res[i][j][k] = (double *) calloc (dim4, sizeof (double));
	if (!res[i][j][k])
	{
	  printf ("memory allocation failed!\n");
	  abort ();
	}
      }
    }
  }

  return res;
}

int ****
ut_alloc_4d_int (unsigned int dim1, unsigned int dim2,
		 unsigned int dim3, unsigned int dim4)
{
  unsigned int i, j, k;
  int ****res = (int ****) calloc (dim1, sizeof (int ***));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%dx%d)\n", dim1, dim2, dim3,
	    dim4);
    abort ();
  }

  for (i = 0; i < dim1; i++)
  {
    res[i] = (int ***) calloc (dim2, sizeof (int **));
    if (!res[i])
    {
      printf ("memory allocation failed!\n");
      abort ();
    }

    for (j = 0; j < dim2; j++)
    {
      res[i][j] = (int **) calloc (dim3, sizeof (int *));
      if (!res[i][j])
      {
	printf ("memory allocation failed!\n");
	abort ();
      }

      for (k = 0; k < dim3; k++)
      {
	res[i][j][k] = (int *) calloc (dim4, sizeof (int));
	if (!res[i][j][k])
	{
	  printf ("memory allocation failed!\n");
	  abort ();
	}
      }
    }
  }

  return res;
}

int ****
ut_alloc_3d_pint (unsigned int dim1, unsigned int dim2,
		  unsigned int dim3)
{
  unsigned int i, j, k;
  int ****res = (int ****) calloc (dim1, sizeof (int ***));
  if (!res)
  {
    printf ("memory allocation failed! (%dx%dx%d)\n", dim1, dim2, dim3);
    abort ();
  }

  for (i = 0; i < dim1; i++)
  {
    res[i] = (int ***) calloc (dim2, sizeof (int **));
    if (!res[i])
    {
      printf ("memory allocation failed!\n");
      abort ();
    }

    for (j = 0; j < dim2; j++)
    {
      res[i][j] = (int **) calloc (dim3, sizeof (int *));
      if (!res[i][j])
      {
	printf ("memory allocation failed!\n");
	abort ();
      }

      for (k = 0; k < dim3; k++)
	res[i][j][k] = NULL;
    }
  }

  return res;
}

double ****
ut_alloc_4d (unsigned int dim1, unsigned int dim2,
	     unsigned int dim3, unsigned int dim4)
{
  return ut_alloc_4d_double (dim1, dim2, dim3, dim4);
}

int *
ut_realloc_1d_int (int *res, unsigned int line)
{
  if (line == 0)
  {
    ut_free_1d_int (res);
    return NULL;
  }
  else
  {
    res = (int *) realloc (res, line * sizeof (int));
    if (!res)
    {
      printf ("memory allocation failed! (%d)\n", line);
      abort ();
    }
  }

  return res;
}

unsigned short *
ut_realloc_1d_ushort (unsigned short *res, unsigned int line)
{
  if (line == 0)
  {
    ut_free_1d_ushort (res);
    return NULL;
  }
  else
  {
    res = (unsigned short *) realloc (res, line * sizeof (unsigned short));
    if (!res)
    {
      printf ("memory allocation failed! (%d)\n", line);
      abort ();
    }
  }

  return res;
}

unsigned char *
ut_realloc_1d_uchar (unsigned char *res, unsigned int line)
{
  if (line == 0)
  {
    ut_free_1d_uchar (res);
    return NULL;
  }
  else
  {
    res = (unsigned char *) realloc (res, line * sizeof (unsigned char));
    if (!res)
    {
      printf ("memory allocation failed! (%d)\n", line);
      abort ();
    }
  }

  return res;
}

double *
ut_realloc_1d (double *res, unsigned int line)
{
  if (line == 0)
  {
    ut_free_1d (res);
    return NULL;
  }
  else
  {
    res = (double *) realloc (res, line * sizeof (double));
    if (!res)
    {
      printf ("memory allocation failed! (%d)\n", line);
      abort ();
    }
  }

  return res;
}

char *
ut_realloc_1d_char (char *res, unsigned int line)
{
  if (line == 0)
  {
    ut_free_1d_char (res);
    return NULL;
  }
  else
  {
    res = (char *) realloc (res, line * sizeof (char));
    if (!res)
    {
      printf ("memory allocation failed! (%d)\n", line);
      abort ();
    }
  }

  return res;
}
int **
ut_realloc_1d_pint (int **res, unsigned int line)
{
  res = realloc (res, line * sizeof (int *));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned short **
ut_realloc_1d_pushort (unsigned short **res, unsigned int line)
{
  res = realloc (res, line * sizeof (unsigned short *));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

unsigned char **
ut_realloc_1d_puchar (unsigned char **res, unsigned int line)
{
  res = realloc (res, line * sizeof (unsigned char *));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

char **
ut_realloc_1d_pchar (char **res, unsigned int line)
{
  res = realloc (res, line * sizeof (char *));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

double **
ut_realloc_1d_pdouble (double **res, unsigned int line)
{
  res = realloc (res, line * sizeof (double *));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

double ***
ut_realloc_1d_ppdouble (double ***res, unsigned int line)
{
  res = realloc (res, line * sizeof (double **));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

int ***
ut_realloc_1d_ppint (int ***res, unsigned int line)
{
  res = realloc (res, line * sizeof (int **));

  if (!res)
  {
    printf ("memory allocation failed! (%d)\n", line);
    abort ();
  }

  return res;
}

int **
ut_realloc_2d_int_addline (int **array, int line, int col)
{
  array = ut_realloc_1d_pint (array, line);
  array[line - 1] = ut_alloc_1d_int (col);

  return array;
}

int **
ut_realloc_2d_int_size2 (int **array, int line, int col1, int col2)
{
  int i, j;

  for (i = 0; i < line; i++)
  {
    array[i] = ut_realloc_1d_int (array[i], col2);
    for (j = col1; j < col2; j++)
      array[i][j] = 0;
  }

  return array;
}

unsigned short **
ut_realloc_2d_ushort_addline (unsigned short **array, int line, int col)
{
  array = ut_realloc_1d_pushort (array, line);
  array[line - 1] = ut_alloc_1d_ushort (col);

  return array;
}

unsigned char **
ut_realloc_2d_uchar_addline (unsigned char **array, int line, int col)
{
  array = ut_realloc_1d_puchar (array, line);
  array[line - 1] = ut_alloc_1d_uchar (col);

  return array;
}

double **
ut_realloc_2d_addline (double **array, int line, int col)
{
  array = ut_realloc_1d_pdouble (array, line);
  array[line - 1] = ut_alloc_1d (col);

  return array;
}

char **
ut_realloc_2d_char_addline (char **array, int line, int col)
{
  array = ut_realloc_1d_pchar (array, line);
  array[line - 1] = ut_alloc_1d_char (col);

  return array;
}

int **
ut_realloc_2d_int_delline (int **array, int oldline, int line)
{
  int i;
  for (i = oldline - 1; i >= line; i--)
    ut_free_1d_int (array[i]);
  array = ut_realloc_1d_pint (array, line);

  return array;
}

unsigned short **
ut_realloc_2d_ushort_delline (unsigned short **array, int oldline, int line)
{
  int i;
  for (i = oldline - 1; i >= line; i--)
    ut_free_1d_ushort (array[i]);
  array = ut_realloc_1d_pushort (array, line);

  return array;
}

unsigned char **
ut_realloc_2d_uchar_delline (unsigned char **array, int oldline, int line)
{
  int i;
  for (i = oldline - 1; i >= line; i--)
    ut_free_1d_uchar (array[i]);
  array = ut_realloc_1d_puchar (array, line);

  return array;
}

double **
ut_realloc_2d_delline (double **array, int oldline, int line)
{
  int i;
  for (i = oldline - 1; i >= line; i--)
    ut_free_1d (array[i]);
  array = ut_realloc_1d_pdouble (array, line);

  return array;
}

double ***
ut_realloc_3d_addarray (double ***array, int line, int col, int depth)
{
  array = ut_realloc_1d_ppdouble (array, line);
  array[line - 1] = ut_alloc_2d (col, depth);

  return array;
}

int ***
ut_realloc_3d_int_addarray (int ***array, int line, int col, int depth)
{
  array = ut_realloc_1d_ppint (array, line);
  array[line - 1] = ut_alloc_2d_int (col, depth);

  return array;
}

/**
 * \file alloc_2d_contig.c
 * \brief
 *
 *
 * \author VILLANI Aurelien
 *
 */
double **
ut_alloc_2d_contig (size_t rows, size_t cols)
{
  // https://stackoverflow.com/questions/31641509/assigning-memory-for-contiguous-2d-array
  size_t r;

  /* allocate chunk: rows times the pointer, rows * cols times the value */
  double **array =
    malloc (rows * sizeof (double *) + rows * cols * sizeof (double));

  /* calculate pointer to first row: point directly behind the pointers,
   * then cast */
  double *row = (double *) (array + rows);

  /* set all row pointers */
  for (r = 0; r < rows; ++r)
  {
    array[r] = row;
    row += cols;
  }
  size_t i, j;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      array[i][j] = 0;
    }
  }
  return array;
}

long **
ut_alloc_2d_contig_long (size_t rows, size_t cols)
{
  // https://stackoverflow.com/questions/31641509/assigning-memory-for-contiguous-2d-array
  size_t r;

  /* allocate chunk: rows times the pointer, rows * cols times the value */
  long **array =
    malloc (rows * sizeof (long *) + rows * cols * sizeof (long));

  /* calculate pointer to first row: point directly behind the pointers,
   * then cast */
  long *row = (long *) (array + rows);

  /* set all row pointers */
  for (r = 0; r < rows; ++r)
  {
    array[r] = row;
    row += cols;
  }
  size_t i, j;
  for (i = 0; i < rows; ++i)
  {
    for (j = 0; j < cols; ++j)
    {
      array[i][j] = 0;
    }
  }
  return array;
}
