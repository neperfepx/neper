/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<float.h>
#include<math.h>
#include<assert.h>
#include"ut.h"
#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#endif // HAVE_GSL

int ut_array_1d_int_sort_comp (const void *, const void *);
int ut_array_1d_pchar_sort_comp (const void *, const void *);
int ut_array_1d_sort_comp (const void *, const void *);
int ut_array_1d_int_sort_index_comp (const void *, const void *);
int ut_array_1d_sort_index_comp (const void *, const void *);

void
ut_array_1d_scale (double *array, int size, double factor)
{
  int i;

  if (factor == 1)
    return;

  for (i = 0; i < size; i++)
    array[i] *= factor;

  return;
}

void
ut_array_1d_normalize (double *array, int size)
{
  ut_array_1d_scale (array, size, 1. / ut_array_1d_norm (array, size));

  return;
}


void
ut_array_1d_round (double *array, int qty, int *array2)
{
  int i;

  for (i = 0; i < qty; i++)
    array2[i] = ut_num_d2ri (array[i]);

  return;
}

void
ut_array_1d_int_scale (int *array, int size, double factor)
{
  int i;

  if (factor == 1)
    return;

  for (i = 0; i < size; i++)
    array[i] *= factor;

  return;
}

void
ut_array_1d_add (double *array1, double *array2, int size, double *array3)
{
  int i;

  for (i = 0; i < size; i++)
    array3[i] = array1[i] + array2[i];

  return;
}

void
ut_array_1d_int_add (int *array1, int *array2, int size, int *array3)
{
  int i;

  for (i = 0; i < size; i++)
    array3[i] = array1[i] + array2[i];

  return;
}

void
ut_array_2d_add (double **array1, double **array2, int size1, int size2,
                 double **array3)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      array3[i][j] = array1[i][j] + array2[i][j];

  return;
}

void
ut_array_3d_add (double ***array1, double ***array2, int size1, int size2,
                 int size3, double ***array3)
{
  int i, j, k;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        array3[i][j][k] = array1[i][j][k] + array2[i][j][k];

  return;
}

void
ut_array_1d_addval (double *array1, int size, double val, double *array2)
{
  int i;

  for (i = 0; i < size; i++)
    array2[i] = array1[i] + val;

  return;
}

void
ut_array_1d_int_addval (int *array1, int size, int val, int *array2)
{
  int i;

  for (i = 0; i < size; i++)
    array2[i] = array1[i] + val;

  return;
}

void
ut_array_2d_int_addval (int **array1, int size1, int size2, int val,
                        int **array2)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      array2[i][j] = array1[i][j] + val;

  return;
}

void
ut_array_2d_addval (double **array1, int size1, int size2, double val,
                    double **array2)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      array2[i][j] = array1[i][j] + val;

  return;
}

double
ut_array_1d_norm (double *array, int size)
{
  int i;
  double norm;

  norm = 0;
  for (i = 0; i < size; i++)
    norm += array[i] * array[i];

  return sqrt (norm);
}

double
ut_array_1d_int_norm (int *array, int size)
{
  int i;
  double norm;

  norm = 0;
  for (i = 0; i < size; i++)
    norm += (double) array[i] * (double) array[i];

  return sqrt ((double) norm);
}

void
ut_array_1d_zero (double *array, int size)
{
  int i;

  for (i = 0; i < size; i++)
    array[i] = 0;

  return;
}

void
ut_array_1d_int_zero (int *array, int size)
{
  int i;

  for (i = 0; i < size; i++)
    array[i] = 0;

  return;
}

void
ut_array_2d_zero (double **array, int size1, int size2)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      array[i][j] = 0;

  return;
}

void
ut_array_3d_zero (double ***array, int size1, int size2, int size3)
{
  int i, j, k;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        array[i][j][k] = 0;

  return;
}

void
ut_array_2d_int_zero (int **array, int size1, int size2)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      array[i][j] = 0;

  return;
}

void
ut_array_3d_int_zero (int ***array, int size1, int size2, int size3)
{
  int i, j, k;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        array[i][j][k] = 0;

  return;
}

int
ut_array_1d_fscanf (FILE * file, double *array, int size)
{
  int i, res = 0;

  for (i = 0; i < size; i++)
    res = fscanf (file, "%lf", &array[i]);

  return res;
}

int
ut_array_1d_int_fscanf (FILE * file, int *array, int size)
{
  int i, res = 0;

  for (i = 0; i < size; i++)
    res = fscanf (file, "%d", &array[i]);

  return res;
}

int
ut_array_1d_string_fscanf (FILE * file, char **array, int size)
{
  int i, res = 0;

  for (i = 0; i < size; i++)
    res = fscanf (file, "%s", array[i]);

  return res;
}

int
ut_array_1d_fprintf (FILE * file, double *array, int size, const char *format)
{
  ut_array_1d_fprintf_nonl (file, array, size, format);
  return fprintf (file, "\n");
}

int
ut_array_1d_fnprintf (char * filename, double *array, int size, const char *format, const char *mode)
{
  int status;

  FILE *file = ut_file_open (filename, mode);

  ut_array_1d_fprintf_nonl (file, array, size, format);

  status = fprintf (file, "\n");

  ut_file_close (file, filename, mode);

  return status;
}

int
ut_array_1d_fprintf_column (FILE * file, double *array, int size, const char *format)
{
  int i, status = -1;

  for (i = 0; i < size; i++)
  {
    status = fprintf (file, format, array[i]);
    status = fprintf (file, "\n");
  }

  return status;
}

int
ut_array_1d_fnprintf_column (char *filename, double *array, int size, const char *format, char *mode)
{
  int status;
  FILE *file = ut_file_open (filename, mode);

  status = ut_array_1d_fprintf_column (file, array, size, format);

  ut_file_close (file, filename, mode);

  return status;
}

int
ut_array_2d_fscanf (FILE * file, double **array, int d1, int d2)
{
  int i, j, res = 0;

  for (i = 0; i < d1; i++)
    for (j = 0; j < d2; j++)
      res = fscanf (file, "%lf", &array[i][j]);

  return res;
}

int
ut_array_1d_fnscanf (char *filename, double *array, int d1, char *mode)
{
  FILE *file = NULL;
  int status = 0, varqty;
  char *fct = NULL, **vals = NULL, **vars = NULL;
  char *filename2 = NULL;

  if (!strncmp (filename, "file(", 5))
    ut_string_string (filename, &filename2);
  else
  {
    filename2 = ut_alloc_1d_char (strlen (filename) + 7);
    sprintf (filename2, "file(%s)", filename);
  }

  ut_string_function (filename2, &fct, &vars, &vals, &varqty);

  if (varqty == 1)
  {
    file = ut_file_open (filename2, mode ? mode : "r");
    status = ut_array_1d_fscanf (file, array, d1);
    ut_file_close (file, filename2, mode ? mode : "r");
  }

  else if (varqty == 2 && !strcmp (vars[1], "col"))
  {
    int i, j, col, totcolqty;
    double *tmp = NULL;

    col = atoi (vals[1]);

    file = ut_file_open (vals[0], mode ? mode : "r");

    for (i = 0; i < d1; i++)
    {
      ut_file_nextlinenbwords (file, &totcolqty);

      tmp = ut_alloc_1d (totcolqty);
      for (j = 0; j < totcolqty; j++)
        if (fscanf (file, "%lf", tmp + j) != 1)
          abort ();

      ut_file_goto_nextline (file);

      array[i] = col ? tmp[abs (col) - 1] : 0;

      if (col < 0)
        array[i] *= -1;

      ut_free_1d (&tmp);
    }

    ut_file_close (file, vals[0], mode ? mode : "r");
    status = 1;
  }
  else
    abort ();

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);
  ut_free_1d_char (&filename2);

  return status;
}

int
ut_array_1d_int_fnscanf (char *filename, int *array, int d1, char *mode)
{
  FILE *file = NULL;
  int status = 0, varqty;
  char *fct = NULL, **vals = NULL, **vars = NULL;

  ut_string_function (filename, &fct, &vars, &vals, &varqty);

  if (varqty == 1)
  {
    file = ut_file_open (filename, mode ? mode : "r");
    status = ut_array_1d_int_fscanf (file, array, d1);
    ut_file_close (file, filename, mode ? mode : "r");
  }

  else if (varqty == 2 && !strcmp (vars[1], "col"))
  {
    int i, j, col, totcolqty;
    int *tmp = NULL;

    col = atoi (vals[1]);

    file = ut_file_open (vals[0], mode ? mode : "r");

    for (i = 0; i < d1; i++)
    {
      ut_file_nextlinenbwords (file, &totcolqty);

      tmp = ut_alloc_1d_int (totcolqty);
      for (j = 0; j < totcolqty; j++)
        if (fscanf (file, "%d", tmp + j) != 1)
          abort ();

      ut_file_goto_nextline (file);

      array[i] = tmp[col - 1];

      ut_free_1d_int (&tmp);
    }

    ut_file_close (file, vals[0], mode ? mode : "r");
  }
  else
    abort ();

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return status;
}

int
ut_array_2d_fnscanf (char *filename, double **array, int d1, int d2, char *mode)
{
  int i, j, varqty;
  FILE *file = NULL;
  int status = 0;
  char *fct = NULL, **vals = NULL, **vars = NULL;
  char *filename2 = ut_alloc_1d_char (strlen (filename) + 10);

  if (!strncmp (filename, "file(", 5))
    ut_string_function (filename, &fct, &vars, &vals, &varqty);
  else
  {
    sprintf (filename2, "file(%s)", filename);
    ut_string_function (filename2, &fct, &vars, &vals, &varqty);
  }

  ut_free_1d_char (&filename2);

  if (varqty == 1)
  {
    file = ut_file_open (filename, mode ? mode : "r");
    status = ut_array_2d_fscanf (file, array, d1, d2);
    ut_file_close (file, filename, mode ? mode : "r");
  }

  else if (varqty == 2 && !strcmp (vars[1], "col"))
  {
    int colqty, totcolqty;
    char **parts = NULL;
    int *cols = NULL;
    double *tmp = NULL;

    ut_list_break (vals[1], ":", &parts, &colqty);

    if (colqty != d2)
      return -1;

    cols = ut_alloc_1d_int (colqty);

    for (i = 0; i < colqty; i++)
      cols[i] = atoi (parts[i]);

    totcolqty = ut_file_nbcolumns (vals[0]);

    file = ut_file_open (vals[0], mode ? mode : "r");

    for (i = 0; i < d1; i++)
    {
      ut_file_nextlinenbwords (file, &totcolqty);

      tmp = ut_alloc_1d (totcolqty);
      for (j = 0; j < totcolqty; j++)
        if (fscanf (file, "%lf", tmp + j) != 1)
          abort ();

      ut_file_goto_nextline (file);

      for (j = 0; j < colqty; j++)
      {
        array[i][j] = cols[j] ? tmp[abs (cols[j]) - 1] : 0;
        if (cols[j] < 0)
          array[i][j] *= -1;
      }

      ut_free_1d (&tmp);
    }
    ut_file_close (file, vals[0], mode ? mode : "r");

    ut_free_2d_char (&parts, colqty);
  }

  else
    abort ();

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return status;
}

int
ut_array_2d_int_fnscanf (char *filename, int **array, int d1, int d2, char *mode)
{
  int i, j, varqty;
  FILE *file = NULL;
  int status = 0;
  char *fct = NULL, **vals = NULL, **vars = NULL;
  char *filename2 = ut_alloc_1d_char (strlen (filename) + 10);

  if (!strncmp (filename, "file(", 5))
    ut_string_function (filename, &fct, &vars, &vals, &varqty);
  else
  {
    sprintf (filename2, "file(%s)", filename);
    ut_string_function (filename2, &fct, &vars, &vals, &varqty);
  }

  ut_free_1d_char (&filename2);

  if (varqty == 1)
  {
    file = ut_file_open (filename, mode ? mode : "r");
    status = ut_array_2d_int_fscanf (file, array, d1, d2);
    ut_file_close (file, filename, mode ? mode : "r");
  }

  else if (varqty == 2 && !strcmp (vars[1], "col"))
  {
    int colqty, totcolqty;
    char **parts = NULL;
    int *cols = NULL;
    int *tmp = NULL;

    ut_list_break (vals[1], ":", &parts, &colqty);

    if (colqty != d2)
      return -1;

    cols = ut_alloc_1d_int (colqty);

    for (i = 0; i < colqty; i++)
      cols[i] = atoi (parts[i]);

    totcolqty = ut_file_nbcolumns (vals[0]);

    file = ut_file_open (vals[0], mode ? mode : "r");

    for (i = 0; i < d1; i++)
    {
      ut_file_nextlinenbwords (file, &totcolqty);

      tmp = ut_alloc_1d_int (totcolqty);
      for (j = 0; j < totcolqty; j++)
        if (fscanf (file, "%d", tmp + j) != 1)
          abort ();

      ut_file_goto_nextline (file);

      for (j = 0; j < colqty; j++)
        array[i][j] = tmp[cols[j] - 1];

      ut_free_1d_int (&tmp);
    }
    ut_file_close (file, vals[0], mode ? mode : "r");

    ut_free_2d_char (&parts, colqty);
  }

  else
    abort ();

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, varqty);
  ut_free_2d_char (&vars, varqty);

  return status;
}

int
ut_array_2d_fnscanf_wcard (char *filename, double **array, int d1, int d2,
                           char *wcard, char *mode)
{
  int i;

  // if file not found and filename is a number, fill the array with
  // this number.
  if (!ut_file_exist (filename) && ut_list_testelt (wcard, ",", "numeral")
      && !ut_string_format_test (filename, "%s"))
  {
    double val;
    sscanf (filename, "%lf", &val);
    ut_array_2d_set (array, d1, d2, val);
  }

  // if file not found and filename is a color, fill the array with
  // this color.
  else if (!ut_file_exist (filename) && ut_list_testelt (wcard, ",", "color")
           && ut_color_name_isvalid (filename))
  {
    int *val = ut_alloc_1d_int (3);
    ut_color_name_rgb (filename, val);

    int i, j;
    for (i = 0; i < d1; i++)
      for (j = 0; j < 3; j++)
        array[i][j] = val[j];

    ut_free_1d_int (&val);
  }

  // if file not found and filename is a `palette', fill the array with
  // the color palette.
  else if (!ut_file_exist (filename) && ut_list_testelt (wcard, ",", "color")
           && !strcmp (filename, "palette"))
  {
    int i, j;
    int **tmp = ut_alloc_2d_int (d1, 3);
    ut_color_palette (d1, 0.2, 0.8, tmp);

    for (i = 0; i < d1; i++)
      for (j = 0; j < 3; j++)
        array[i][j] = tmp[i][j];

    ut_free_2d_int (&tmp, d1);
  }

  else if (ut_file_exist (filename) && ut_list_testelt (wcard, ",", "color"))
  {
    char *name = ut_alloc_1d_char (100);
    int *val = ut_alloc_1d_int (3);

    // check that the file does not have more data than goes in the
    // array.
    if (ut_list_testelt (wcard, ",", "size") == 1)
      ut_file_nbwords_testwmessage (filename, d1);

    FILE *file = NULL;
    file = ut_file_open (filename, mode ? mode : "r");
    for (i = 0; i < d1; i++)
    {
      if (fscanf (file, "%s", name) != 1)
        abort ();
      ut_color_name_rgb (name, val);
      ut_array_1d_memcpy_fromint (val, 3, array[i]);
    }
    ut_file_close (file, filename, mode ? mode : "r");

    ut_free_1d_int (&val);
    ut_free_1d_char (&name);
  }

  else
  {
    // check that the file does not have more data than goes in the
    // array.
    if (ut_list_testelt (wcard, ",", "size") == 1)
      ut_file_nbwords_testwmessage (filename, d1 * d2);

    ut_array_2d_fnscanf (filename, array, d1, d2, mode);
  }

  return 0;
}

int
ut_array_1d_fnscanf_wcard (char *filename, double *array, int d1, char *wcard, char *mode)
{
  double **tmp = ut_alloc_2d (1, d1);

  ut_array_2d_fnscanf_wcard (filename, tmp, 1, d1, wcard, mode);

  ut_array_1d_memcpy (tmp[0], d1, array);

  ut_free_2d (&tmp, 1);

  return 0;
}

int
ut_array_1d_int_fnscanf_wcard (char *filename, int *array, int d1,
                               char *wcard, char *mode)
{
  int i;
  double *tmp = ut_alloc_1d (d1);

  ut_array_1d_fnscanf_wcard (filename, tmp, d1, wcard, mode);
  for (i = 0; i < d1; i++)
    array[i] = ut_num_d2ri (tmp[i]);

  ut_free_1d (&tmp);

  return 0;
}

int
ut_array_1d_int_fprintf (FILE * file, int *array, int size,
                         const char *format)
{
  int i, res;

  for (i = 0; i < size - 1; i++)
  {
    fprintf (file, format, array[i]);
    fprintf (file, " ");
  }
  if (size > 0)
    fprintf (file, format, array[size - 1]);
  res = fprintf (file, "\n");

  return res;
}

int
ut_array_1d_int_fprintf_reverse (FILE * file, int *array, int size,
                                 const char *format)
{
  int i, res;

  for (i = size - 1; i > 0; i--)
  {
    fprintf (file, format, array[i]);
    fprintf (file, " ");
  }
  if (size > 0)
    fprintf (file, format, array[0]);
  res = fprintf (file, "\n");

  return res;
}

int
ut_array_1d_int_fprintf_nonl (FILE * file, int *array, int size,
                              const char *format)
{
  int i, res = 0;

  for (i = 0; i < size - 1; i++)
  {
    fprintf (file, format, array[i]);
    res = fprintf (file, " ");
  }
  if (size > 0)
    res = fprintf (file, format, array[size - 1]);

  return res;
}

int
ut_array_1d_fprintf_nonl (FILE * file, double *array, int size,
                          const char *format)
{
  int i, res = 0;

  if (strstr (format, "f"))
  {
    for (i = 0; i < size - 1; i++)
    {
      fprintf (file, format, array[i]);
      res = fprintf (file, " ");
    }
    if (size > 0)
      res = fprintf (file, format, array[size - 1]);
  }

  else if (strstr (format, "d"))
  {
    int *a2 = ut_alloc_1d_int (size);
    ut_array_1d_round (array, size, a2);

    ut_array_1d_int_fprintf_nonl (file, a2, size, format);
    ut_free_1d_int (&a2);
  }

  return res;
}

int
ut_array_1d_uint_fprintf (FILE * file, unsigned int *array, int size,
                          const char *format)
{
  int i, res;

  for (i = 0; i < size - 1; i++)
  {
    fprintf (file, format, array[i]);
    fprintf (file, " ");
  }
  if (size > 0)
    fprintf (file, format, array[size - 1]);
  res = fprintf (file, "\n");

  return res;
}

int
ut_array_2d_fprintf (FILE * file, double **array, int size1, int size2,
                     const char *format)
{
  int i, j, res;

  res = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size2 - 1; j++)
    {
      res = fprintf (file, format, array[i][j]);
      res = fprintf (file, " ");
    }
    if (size2 > 0)
      res = fprintf (file, format, array[i][size2 - 1]);
    res = fprintf (file, "\n");
  }

  return res;
}

int
ut_array_2d_fprintf_oneline (FILE * fileid, double **array, int size1,
                             int size2, const char *format)
{
  int res, i, j, k;
  int numel = size1 * size2;

  for (i = 0, k = 0; (k < numel - 1) && (i < size1); i++)
  {
    for (j = 0; (k < numel - 1) && (j < size2); j++, k++)
    {
      fprintf (fileid, format, array[i][j]);
      fprintf (fileid, " ");
    }
  }
  fprintf (fileid, format, array[size1 - 1][size2 - 1]);
  res = fprintf (fileid, "\n");

  return res;
}

int
ut_array_2d_fprintf_col (FILE * file, double **array, int size1, int size2,
                         int *cols, int colqty, const char *format)
{
  int i, j, res;

  for (j = 0; j < colqty; ++j)
    if (cols[j] < 0 || cols[j] > size2 - 1)
      abort ();

  res = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < colqty; ++j)
    {
      res = fprintf (file, format, array[i][cols[j]]);
      res = fprintf (file, (j < colqty - 1) ? " " : "\n");
    }

  return res;
}

int
ut_array_2d_int_fprintf (FILE * file, int **array, int size1, int size2,
                         const char *format)
{
  int i, j, res;

  res = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size2 - 1; j++)
    {
      res = fprintf (file, format, array[i][j]);
      res = fprintf (file, " ");
    }
    if (size2 > 0)
      res = fprintf (file, format, array[i][size2 - 1]);
    res = fprintf (file, "\n");
  }

  return res;
}

int
ut_array_3d_int_fprintf (FILE * file, int ***array, int size1, int size2,
                         int size3, const char *format)
{
  int i, res = 0;

  for (i = 0; i < size1; i++)
  {
    res = ut_array_2d_int_fprintf (file, array[i], size2, size3, format);
    fprintf (file, "\n");
  }

  return res;
}

int
ut_array_2dns_int_fprintf (FILE * file, int **array, int size1, int *size2,
                           const char *format)
{
  int i, j, res;

  res = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size2[i] - 1; j++)
    {
      res = fprintf (file, format, array[i][j]);
      res = fprintf (file, " ");
    }
    if (size2[i] > 0)
      res = fprintf (file, format, array[i][size2[i] - 1]);
    res = fprintf (file, "\n");
  }

  return res;
}

int
ut_array_2d_int_fscanf (FILE * file, int **array, int d1, int d2)
{
  int i, j, res = 0;

  for (i = 0; i < d1; i++)
    for (j = 0; j < d2; j++)
      res = fscanf (file, "%d", &array[i][j]);

  return res;
}

void
ut_array_2d_scale (double **array, int size1, int size2, double factor)
{
  int i, j;

  if (factor == 1)
    return;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      array[i][j] *= factor;

  return;
}

double
ut_array_1d_min (double *array, int size)
{
  int i;
  double min;

  if (size <= 0)
    abort ();

  min = array[0];
  for (i = 0; i < size; i++)
    if (array[i] < min)
      min = array[i];

  return min;
}

double
ut_array_1d_mean (double *array, int size)
{
  int i;
  double mean = 0;

  if (size <= 0)
    abort ();

  for (i = 0; i < size; i++)
    mean += array[i];
  mean /= size;

  return mean;
}

double
ut_array_1d_wmean (double *array, double *weight, int size)
{
  int i;
  double sum, mean = 0;

  if (size <= 0)
    abort ();

  sum = 0;
  for (i = 0; i < size; i++)
  {
    mean += array[i] * weight[i];
    sum += weight[i];
  }

  mean /= sum;

  return mean;
}

double
ut_array_1d_lmean (double *array, int size, double p)
{
  int i;
  double val = 0;

  if (size <= 0)
    abort ();

  if (p == DBL_MAX)
    val = ut_array_1d_max (array, size) / size;

  else
  {
    for (i = 0; i < size; i++)
      val += pow (array[i], p);
    val = pow (1. / size * val, 1. / p);
  }

  return val;
}

double
ut_array_1d_lnorm (double *array, int size, double p)
{
  int i;
  double val = 0;

  if (size <= 0)
    abort ();

  if (p == DBL_MAX)
    val = ut_array_1d_max (array, size);

  else
  {
    for (i = 0; i < size; i++)
      val += pow (array[i], p);
    val = pow (val, 1. / p);
  }

  return val;
}

double
ut_array_1d_lmean_expr (double *array, int size, char *expr)
{
  int status;
  double p;

  if (!strcmp (expr, "Linf"))
    p = DBL_MAX;
  else if (!expr || !strcmp (expr, "default"))
    p = 2;
  else
  {
    status = sscanf (expr, "L%lf", &p);
    if (status != 1)
      abort ();
  }

  return ut_array_1d_lmean (array, size, p);
}

double
ut_array_1d_gmean (double *array, int size)
{
  int i;
  double mean = 1;

  if (size <= 0)
    abort ();

  for (i = 0; i < size; i++)
    mean *= array[i];
  mean = pow (mean, 1. / size);

  return mean;
}

double
ut_array_1d_int_mean (int *array, int size)
{
  int i;
  double mean = 0;

  if (size <= 0)
    abort ();

  for (i = 0; i < size; i++)
    mean += array[i];
  mean /= size;

  return mean;
}

double
ut_array_1d_int_gmean (int *array, int size)
{
  int i;
  double mean = 1;

  if (size <= 0)
    abort ();

  for (i = 0; i < size; i++)
    mean *= array[i];
  mean = pow (mean, 1. / size);

  return mean;
}

double
ut_array_1d_stddev (double *array, double mean, int size)
{
  int i;
  double stddev;

  if (size <= 0)
    abort ();

  stddev = 0;
  for (i = 0; i < size; i++)
    stddev += (array[i] - mean) * (array[i] - mean);
  stddev /= size;
  stddev = sqrt (stddev);

  return stddev;
}

double
ut_array_1d_wstddev (double *array, double *warray, double mean, int size)
{
  int i, M;
  double tmp0, tmp1, stddev;

  M = 0;
  for (i = 0; i < size; i++)
    M += (warray[i] != 0);

  if (M <= 0)
    abort ();

  tmp0 = 0;
  tmp1 = 0;
  for (i = 0; i < size; i++)
  {
    tmp0 += warray[i] * pow (array[i] - mean, 2);
    tmp1 += warray[i];
  }
  tmp1 *= (double) (M - 1) / M;
  stddev = sqrt (tmp0 / tmp1);

  return stddev;
}

double
ut_array_2d_col_min (double **array, int col, int size)
{
  int i;
  double min;

  if (size <= 0)
    abort ();

  min = array[0][col];
  for (i = 0; i < size; i++)
    if (array[i][col] < min)
      min = array[i][col];

  return min;
}

double
ut_array_2d_col_max (double **array, int col, int size)
{
  int i;
  double max;

  if (size <= 0)
    abort ();

  max = array[0][col];
  for (i = 0; i < size; i++)
    if (array[i][col] > max)
      max = array[i][col];

  return max;
}

double
ut_array_2d_stddev (double **array, double mean, int size1, int size2)
{
  int i, j;
  double stddev;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  stddev = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      stddev += (array[i][j] - mean) * (array[i][j] - mean);
  stddev /= (size1 * size2);
  stddev = sqrt (stddev);

  return stddev;
}

double
ut_array_3d_stddev (double ***array, double mean, int size1, int size2,
                    int size3)
{
  int i, j, k;
  double stddev;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  stddev = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        stddev += (array[i][j][k] - mean) * (array[i][j][k] - mean);
  stddev /= (size1 * size2 * size3);
  stddev = sqrt (stddev);

  return stddev;
}

double
ut_array_3d_int_stddev (int ***array, double mean, int size1, int size2,
                        int size3)
{
  int i, j, k;
  double stddev;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  stddev = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        stddev += (array[i][j][k] - mean) * (array[i][j][k] - mean);
  stddev /= (size1 * size2 * size3);
  stddev = sqrt (stddev);

  return stddev;
}

double
ut_array_3d_ushort_stddev (unsigned short ***array, double mean, int size1,
                           int size2, int size3)
{
  int i, j, k;
  double stddev;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  stddev = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        stddev += (array[i][j][k] - mean) * (array[i][j][k] - mean);
  stddev /= (size1 * size2 * size3);
  stddev = sqrt (stddev);

  return stddev;
}

double
ut_array_3d_uchar_stddev (unsigned char ***array, double mean, int size1,
                          int size2, int size3)
{
  int i, j, k;
  double stddev;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  stddev = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        stddev += (array[i][j][k] - mean) * (array[i][j][k] - mean);
  stddev /= (size1 * size2 * size3);
  stddev = sqrt (stddev);

  return stddev;
}

int
ut_array_1d_min_index (double *array, int size)
{
  int i, pos;
  double min;

  if (size <= 0)
    abort ();

  pos = 0;
  min = array[0];
  for (i = 0; i < size; i++)
    if (array[i] < min)
    {
      min = array[i];
      pos = i;
    }

  return pos;
}

int
ut_array_1d_int_min_index (int *array, int size)
{
  int i, pos;
  int min;

  if (size <= 0)
    abort ();

  pos = 0;
  min = array[0];
  for (i = 0; i < size; i++)
    if (array[i] < min)
    {
      min = array[i];
      pos = i;
    }

  return pos;
}

double
ut_array_1d_max (double *array, int size)
{
  int i;
  double max;

  if (size <= 0)
    abort ();

  max = array[0];
  for (i = 0; i < size; i++)
    if (array[i] > max)
      max = array[i];

  return max;
}

double
ut_array_1d_absmax (double *array, int size)
{
  int i;
  double absmax;

  if (size <= 0)
    abort ();

  absmax = fabs (array[0]);
  for (i = 0; i < size; i++)
    if (fabs (array[i]) > absmax)
      absmax = fabs (array[i]);

  return absmax;
}

int
ut_array_1d_absmax_index (double *array, int size)
{
  int i, pos;
  double absmax;

  if (size <= 0)
    abort ();

  pos = 0;
  absmax = fabs (array[0]);
  for (i = 0; i < size; i++)
    if (fabs (array[i]) > absmax)
    {
      absmax = fabs (array[i]);
      pos = i;
    }

  return pos;
}

double
ut_array_1d_absmin (double *array, int size)
{
  int i;
  double absmin;

  if (size <= 0)
    abort ();

  absmin = fabs (array[0]);
  for (i = 0; i < size; i++)
    if (fabs (array[i]) < absmin)
      absmin = fabs (array[i]);

  return absmin;
}

int
ut_array_1d_absmin_index (double *array, int size)
{
  int i, pos;
  double absmin;

  if (size <= 0)
    abort ();

  pos = 0;
  absmin = fabs (array[0]);
  for (i = 0; i < size; i++)
    if (fabs (array[i]) < absmin)
    {
      absmin = fabs (array[i]);
      pos = i;
    }

  return pos;
}

int
ut_array_1d_int_max_index (int *array, int size)
{
  int i, pos;
  int max;

  if (size <= 0)
    abort ();

  pos = 0;
  max = array[0];
  for (i = 0; i < size; i++)
    if (array[i] > max)
    {
      max = array[i];
      pos = i;
    }

  return pos;
}

int
ut_array_1d_max_index (double *array, int size)
{
  int i, pos;
  double max;

  if (size <= 0)
    abort ();

  pos = 0;
  max = array[0];
  for (i = 0; i < size; i++)
    if (array[i] > max)
    {
      max = array[i];
      pos = i;
    }

  return pos;
}

int
ut_array_1d_int_min (int *array, int size)
{
  int i, min;

  if (size <= 0)
    abort ();

  min = array[0];
  for (i = 0; i < size; i++)
    if (array[i] < min)
      min = array[i];

  return min;
}

int
ut_array_2d_int_min (int **array, int size1, int size2)
{
  int i, j, min;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  min = array[0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      if (array[i][j] < min)
        min = array[i][j];

  return min;
}

double
ut_array_2d_min (double **array, int size1, int size2)
{
  int i, j;
  double min;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  min = array[0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      if (array[i][j] < min)
        min = array[i][j];

  return min;
}

double
ut_array_3d_min (double ***array, int size1, int size2, int size3)
{
  int i, j, k;
  double min;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  min = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] < min)
          min = array[i][j][k];

  return min;
}

int
ut_array_3d_int_min (int ***array, int size1, int size2, int size3)
{
  int i, j, k;
  int min;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  min = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] < min)
          min = array[i][j][k];

  return min;
}

unsigned short
ut_array_3d_ushort_min (unsigned short ***array, int size1, int size2,
                        int size3)
{
  int i, j, k;
  int min;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  min = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] < min)
          min = array[i][j][k];

  return min;
}

unsigned char
ut_array_3d_uchar_min (unsigned char ***array, int size1, int size2,
                       int size3)
{
  int i, j, k;
  int min;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  min = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] < min)
          min = array[i][j][k];

  return min;
}

double
ut_array_2d_max (double **array, int size1, int size2)
{
  int i, j;
  double max;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  max = array[0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      if (array[i][j] > max)
        max = array[i][j];

  return max;
}

int
ut_array_2d_int_max (int **array, int size1, int size2)
{
  int i, j;
  int max;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  max = array[0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      if (array[i][j] > max)
        max = array[i][j];

  return max;
}

double
ut_array_2d_absmax (double **array, int size1, int size2)
{
  int i, j;
  double max;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  max = fabs (array[0][0]);
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      if (array[i][j] > max)
        max = fabs (array[i][j]);

  return max;
}

double
ut_array_3d_max (double ***array, int size1, int size2, int size3)
{
  int i, j, k;
  double max;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  max = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] > max)
          max = array[i][j][k];

  return max;
}

int
ut_array_3d_int_max (int ***array, int size1, int size2, int size3)
{
  int i, j, k;
  int max;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  max = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] > max)
          max = array[i][j][k];

  return max;
}

unsigned short
ut_array_3d_ushort_max (unsigned short ***array, int size1, int size2,
                        int size3)
{
  int i, j, k;
  int max;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  max = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] > max)
          max = array[i][j][k];

  return max;
}

unsigned char
ut_array_3d_uchar_max (unsigned char ***array, int size1, int size2,
                       int size3)
{
  int i, j, k;
  int max;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  max = array[0][0][0];
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        if (array[i][j][k] > max)
          max = array[i][j][k];

  return max;
}

double
ut_array_2d_sum (double **array, int size1, int size2)
{
  int i, j;
  double sum;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  sum = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      sum += array[i][j];

  return sum;
}

double
ut_array_2d_mean (double **array, int size1, int size2)
{
  double mean;

  mean = ut_array_2d_sum (array, size1, size2);
  mean /= (size1 * size2);

  return mean;
}

double
ut_array_2d_col_mean (double **array, int size1, int col)
{
  int i;
  double mean;

  if (size1 <= 0 || col <= 0)
    abort ();

  mean = 0;
  for (i = 0; i < size1; i++)
    mean += array[i][col];
  mean /= size1;

  return mean;
}

double
ut_array_2d_col_wmean (double **array, double *weight, int size1, int col)
{
  int i;
  double sum, sumweight;

  if (size1 <= 0 || col < 0)
    abort ();

  sum = 0;
  sumweight = 0;
  for (i = 0; i < size1; i++)
  {
    sum += weight[i] * array[i][col];
    sumweight += weight[i];
  }

  return sum / sumweight;
}

double
ut_array_2d_gmean (double **array, int size1, int size2)
{
  int i, j;
  double mean;

  if (size1 <= 0 || size2 <= 0)
    abort ();

  mean = 1;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      mean *= array[i][j];
  mean = pow (mean, 1. / (size1 * size2));

  return mean;
}

double
ut_array_3d_mean (double ***array, int size1, int size2, int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean += array[i][j][k];
  mean /= (size1 * size2 * size3);

  return mean;
}

double
ut_array_3d_gmean (double ***array, int size1, int size2, int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 1;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean *= array[i][j][k];
  mean = pow (mean, 1. / (size1 * size2 * size3));

  return mean;
}

double
ut_array_3d_int_mean (int ***array, int size1, int size2, int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean += array[i][j][k];
  mean /= (size1 * size2 * size3);

  return mean;
}

double
ut_array_3d_int_gmean (int ***array, int size1, int size2, int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 1;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean *= array[i][j][k];
  mean = pow (mean, 1. / (size1 * size2 * size3));

  return mean;
}

double
ut_array_3d_ushort_mean (unsigned short ***array, int size1, int size2,
                         int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean += array[i][j][k];
  mean /= (size1 * size2 * size3);

  return mean;
}

double
ut_array_3d_ushort_gmean (unsigned short ***array, int size1, int size2,
                          int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 1;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean *= array[i][j][k];
  mean = pow (mean, 1. / (size1 * size2 * size3));

  return mean;
}

double
ut_array_3d_uchar_mean (unsigned char ***array, int size1, int size2,
                        int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 0;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean += array[i][j][k];
  mean /= (size1 * size2 * size3);

  return mean;
}

double
ut_array_3d_uchar_gmean (unsigned char ***array, int size1, int size2,
                         int size3)
{
  int i, j, k;
  double mean;

  if (size1 <= 0 || size2 <= 0 || size3 <= 0)
    abort ();

  mean = 1;
  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        mean *= array[i][j][k];
  mean = pow (mean, 1. / (size1 * size2 * size3));

  return mean;
}

int
ut_array_1d_int_max (int *array, int size)
{
  int i, max;

  if (size <= 0)
    abort ();

  max = array[0];
  for (i = 0; i < size; i++)
    if (array[i] > max)
      max = array[i];

  return max;
}

void
ut_array_1d_memcpy (double *src, int size, double *dest)
{
  int i;

  for (i = 0; i < size; i++)
    dest[i] = src[i];

  return;
}

void
ut_array_1d_memcpy_fromint (int *src, int size, double *dest)
{
  int i;

  for (i = 0; i < size; i++)
    dest[i] = src[i];

  return;
}

void
ut_array_1d_memcpy_toint (double *src, int size, int *dest)
{
  int i;

  for (i = 0; i < size; i++)
    dest[i] = ut_num_d2ri (src[i]);

  return;
}

void
ut_array_1d_int_memcpy (int *src, int size, int *dest)
{
  int i;

  for (i = 0; i < size; i++)
    dest[i] = src[i];

  return;
}

void
ut_array_1d_uint_memcpy (unsigned int *src, int size, unsigned int *dest)
{
  int i;

  for (i = 0; i < size; i++)
    dest[i] = src[i];

  return;
}

void
ut_array_2d_memcpy (double **src, int size1, int size2, double **dest)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      dest[i][j] = src[i][j];

  return;
}

void
ut_array_3d_memcpy (double ***src, int size1, int size2, int size3,
                    double ***dest)
{
  int i, j, k;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        dest[i][j][k] = src[i][j][k];

  return;
}

void
ut_array_4d_memcpy (double ****src, int size1, int size2, int size3,
                    int size4, double ****dest)
{
  int i, j, k, l;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        for (l = 0; l < size4; l++)
          dest[i][j][k][l] = src[i][j][k][l];

  return;
}

void
ut_array_2d_int_memcpy (int **src, int size1, int size2, int **dest)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      dest[i][j] = src[i][j];

  return;
}

void
ut_array_1d_pchar_memcpy (char **src, int size1, char **dest)
{
  int i;

  for (i = 0; i < size1; i++)
  {
    dest[i] = ut_alloc_1d_char (strlen (src[i]) + 1);
    strcpy (dest[i], src[i]);
  }

  return;
}

void
ut_array_3d_int_memcpy (int ***src, int size1, int size2, int size3,
                        int ***dest)
{
  int i, j, k;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        dest[i][j][k] = src[i][j][k];

  return;
}

void
ut_array_3d_uchar_memcpy (unsigned char ***src, int size1, int size2,
                          int size3, unsigned char ***dest)
{
  int i, j, k;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      for (k = 0; k < size3; k++)
        dest[i][j][k] = src[i][j][k];

  return;
}

void
ut_array_1d_sort (double *array, int size)
{
  qsort (array, size, sizeof (double), ut_array_1d_sort_comp);

  return;
}

void
ut_array_1d_sort_des (double *array, int size)
{
  ut_array_1d_sort (array, size);
  ut_array_1d_reverseelts (array, size);

  return;
}

void
ut_array_1d_int_sort_des (int *array, int size)
{
  ut_array_1d_int_sort (array, size);
  ut_array_1d_int_reverseelts (array, size);

  return;
}

void
ut_array_1d_sort_des_index (double *array, int size, int *pos)
{
  ut_array_1d_sort_index (array, size, pos);
  ut_array_1d_int_reverseelts (pos, size);

  return;
}

void
ut_array_1d_int_sort_des_index (int *array, int size, int *pos)
{
  ut_array_1d_int_sort_index (array, size, pos);
  ut_array_1d_int_reverseelts (pos, size);

  return;
}

void
ut_array_1d_int_sort (int *array, int size)
{
  qsort (array, size, sizeof (int), ut_array_1d_int_sort_comp);

  return;
}

int
ut_array_1d_int_sort_comp (const void *i, const void *j)
{
  return *(int *) i - *(int *) j;
}

void
ut_array_1d_pchar_sort (char **array, int size)
{
  qsort (array, size, sizeof (char *), ut_array_1d_pchar_sort_comp);

  return;
}

int
ut_array_1d_pchar_sort_comp (const void *i, const void *j)
{
  return strcmp (*(char **) i, *(char **) j);
}

int
ut_array_1d_sort_comp (const void *i, const void *j)
{
  if ((*(double *) i - *(double *) j) < 0)
    return -1;
  else if ((*(double *) i - *(double *) j) > 0)
    return 1;
  else
    return 0;
}

int
ut_array_1d_int_sort_index_comp (const void *a, const void *b)
{
  return **(const int **) a - **(const int **) b;
}

int
ut_array_1d_sort_index_comp (const void *a, const void *b)
{
  if (**(const double **) a - **(const double **) b < 0)
    return -1;
  else if (**(const double **) a - **(const double **) b > 0)
    return 1;
  else
    return 0;
}

void
ut_array_1d_int_sort_index (int *array, int size, int *pos)
{
  int i;
  int **x = ut_alloc_1d_pint (size);

  for (i = 0; i < size; ++i)
    x[i] = array + i;

  qsort (x, size, sizeof (*x), ut_array_1d_int_sort_index_comp);

  for (i = 0; i < size; ++i)
    pos[i] = (x[i] - array);

  ut_free_1d_pint (&x);

  return;
}

void
ut_array_1d_sort_permarray_int (double *array, int size, int *permarray)
{
  int i;
  int *id = ut_alloc_1d_int (size);
  int *tmp = ut_alloc_1d_int (size);

  ut_array_1d_sort_index (array, size, id);

  ut_array_1d_int_memcpy (permarray, size, tmp);
  for (i = 0; i < size; i++)
    permarray[i] = tmp[id[i]];

  ut_free_1d_int (&id);
  ut_free_1d_int (&tmp);

  return;
}

void
ut_array_1d_sort_index (double *array, int size, int *pos)
{
  int i;
  double **x = ut_alloc_1d_pdouble (size);

  for (i = 0; i < size; ++i)
    x[i] = array + i;

  qsort (x, size, sizeof (*x), ut_array_1d_sort_index_comp);

  for (i = 0; i < size; ++i)
    pos[i] = (x[i] - array);

  ut_free_1d_pdouble (&x);

  return;
}

/* The subroutine sorts the array from element beg */
/* to element end, using the bubble sorting algorithm. */
void
ut_array_1d_abssort (double *array, int size)
{
  int i, j;

  for (i = 0; i < size; i++)
    for (j = size - 1; j > i; j--)
      if (fabs (array[j - 1]) > fabs (array[j]))
        ut_array_1d_switch (array, j - 1, j);

  return;
}

/* The subroutine indirectly sorts the array from element beg */
/* to element end, using the bubble sorting algorithm. */
void
ut_array_1d_abssort_index (double *array, int size, int *pos)
{
  int i, j;
  double *arraycpy = ut_alloc_1d (size);

  ut_array_1d_memcpy (array, size, arraycpy);

  for (i = 0; i < size; i++)
    pos[i] = i;

  for (i = 0; i < size; i++)
    for (j = size - 1; j > i; j--)
      if (fabs (arraycpy[j - 1]) > fabs (arraycpy[j]))
      {
        ut_array_1d_switch (arraycpy, j - 1, j);
        ut_array_1d_int_switch (pos, j - 1, j);
      }

  ut_free_1d (&arraycpy);

  return;
}

void
ut_array_1d_int_abssort_index (int *array, int size, int *pos)
{
  int i, j;
  int *arraycpy = ut_alloc_1d_int (size);

  ut_array_1d_int_memcpy (array, size, arraycpy);

  for (i = 0; i < size; i++)
    pos[i] = i;

  for (i = 0; i < size; i++)
    for (j = size - 1; j > i; j--)
      if (abs (arraycpy[j - 1]) > abs (arraycpy[j]))
      {
        ut_array_1d_int_switch (arraycpy, j - 1, j);
        ut_array_1d_int_switch (pos, j - 1, j);
      }

  ut_free_1d_int (&arraycpy);

  return;
}

void
ut_array_1d_switch (double *array, int n1, int n2)
{
  double tmp;

  tmp = array[n1];
  array[n1] = array[n2];
  array[n2] = tmp;

  return;
}

void
ut_array_1d_int_switch (int *array, int n1, int n2)
{
  int tmp;

  tmp = array[n1];
  array[n1] = array[n2];
  array[n2] = tmp;

  return;
}

void
ut_array_2d_int_switchlines (int **array, int colnb, int line1, int line2)
{
  int i, tmp;

  for (i = 0; i < colnb; i++)
  {
    tmp = array[line1][i];
    array[line1][i] = array[line2][i];
    array[line2][i] = tmp;
  }

  return;
}

void
ut_array_2d_int_switchlines_length (int **array, int line1, int col1,
                                    int line2, int col2)
{
  int i, colmin, colmax;

  // handling special, simple case where col1 = col2
  if (col1 == col2)
  {
    ut_array_2d_int_switchlines (array, col1, line1, line2);
    return;
  }

  colmin = ut_num_min (col1, col2);
  colmax = ut_num_max (col1, col2);

  // first part of the lines
  for (i = 0; i < colmin; i++)
    ut_num_switch_int (array[line1] + i, array[line2] + i);

  // second part of the lines (+ realloc'ing)
  // first, copying long line to small line, then realloc long line.
  if (col1 == colmax)
  {
    array[line2] = ut_realloc_1d_int (array[line2], colmax);
    for (i = colmin; i < colmax; i++)
      array[line2][i] = array[line1][i];
    array[line1] = ut_realloc_1d_int (array[line1], colmin);
  }
  else
  {
    array[line1] = ut_realloc_1d_int (array[line1], colmax);
    for (i = colmin; i < colmax; i++)
      array[line1][i] = array[line2][i];
    array[line2] = ut_realloc_1d_int (array[line2], colmin);
  }

  return;
}

void
ut_array_2d_switchlines (double **array, int colnb, int line1, int line2)
{
  int i;
  double tmp = 0;

  for (i = 0; i < colnb; i++)
  {
    tmp = array[line1][i];
    array[line1][i] = array[line2][i];
    array[line2][i] = tmp;
  }

  return;
}

void
ut_array_1d_int_rotate (int *array, int n1, int num)
{
  int i;
  int *copy;

  copy = (int *) ut_alloc_1d_int (n1);

  for (i = 0; i < n1; i++)
    copy[i] = array[i];

  for (i = 0; i < n1; i++)
    array[i] = copy[ut_array_rotpos (0, n1 - 1, i, num)];

  ut_free_1d_int (&copy);

  return;
}

void
ut_array_1d_rotate (double *array, int n1, int num)
{
  int i;
  double *copy;

  copy = (double *) ut_alloc_1d (n1);

  for (i = 0; i < n1; i++)
    copy[i] = array[i];

  for (i = 0; i < n1; i++)
    array[i] = copy[ut_array_rotpos (0, n1 - 1, i, num)];

  ut_free_1d (&copy);

  return;
}

int
ut_array_1d_int_find (int *array, int n1, int val)
{
  int i, res = -1;

  for (i = 0; i < n1; i++)
    if (array[i] == val)
    {
      res = i;
      break;
    }

  return res;
}

void
ut_array_1d_int_findnreplace (int *array, int n1, int val, int val2)
{
  int pos;

  if (val == val2)
    return;

  pos = ut_array_1d_int_find (array, n1, val);
  while (pos != -1)
  {
    array[pos] = val2;
    pos = ut_array_1d_int_find (array, n1, val);
  }

  return;
}

void
ut_array_1d_int_reverseelts (int *array, int n1)
{
  int i, tmp;

  for (i = 0; i < n1 / 2; i++)
  {
    tmp = array[i];
    array[i] = array[n1 - 1 - i];
    array[n1 - 1 - i] = tmp;
  }

  return;
}

void
ut_array_1d_reverseelts (double *array, int n1)
{
  int i;
  double tmp;

  for (i = 0; i < n1 / 2; i++)
  {
    tmp = array[i];
    array[i] = array[n1 - 1 - i];
    array[n1 - 1 - i] = tmp;
  }

  return;
}

void
ut_array_1d_permute (double *array, int qty, int *permutation)
{
  int i;
  double *tmp = ut_alloc_1d (qty);

  for (i = 0; i < qty; i++)
    tmp[i] = array[i];

  for (i = 0; i < qty; i++)
    array[i] = tmp[permutation[i]];

  ut_free_1d (&tmp);

  return;
}

void
ut_array_1d_int_permute (int *array, int qty, int *permutation)
{
  int i;
  int *tmp = ut_alloc_1d_int (qty);

  for (i = 0; i < qty; i++)
    tmp[i] = array[i];

  for (i = 0; i < qty; i++)
    array[i] = tmp[permutation[i]];

  ut_free_1d_int (&tmp);

  return;
}

void
ut_array_2d_int_permute (int **array, int line, int col, int *permutation)
{
  int i, j;
  int **tmp = ut_alloc_2d_int (line, col);

  for (i = 0; i < line; i++)
    for (j = 0; j < col; j++)
      tmp[i][j] = array[i][j];

  for (i = 0; i < line; i++)
    for (j = 0; j < col; j++)
      array[i][j] = tmp[permutation[i]][j];

  ut_free_2d_int (&tmp, line);

  return;
}

double
ut_array_1d_sum (double *data, int n)
{
  int i;
  double res = 0;

  for (i = 0; i < n; i++)
    res += data[i];

  return res;
}

double
ut_array_1d_prod (double *data, int n)
{
  int i;
  double res = 1;

  for (i = 0; i < n; i++)
    res *= data[i];

  return res;
}

double
ut_array_1d_scalprod (double *data1, double *data2, int n)
{
  int i;
  double res = 0;

  for (i = 0; i < n; i++)
    res += data1[i] * data2[i];

  return res;
}

int
ut_array_1d_int_scalprod (int *data1, int *data2, int n)
{
  int i;
  int res = 0;

  for (i = 0; i < n; i++)
    res += data1[i] * data2[i];

  return res;
}

int
ut_array_1d_int_sum (int *data, int n)
{
  int i;
  int res = 0;

  for (i = 0; i < n; i++)
    res += data[i];
  return res;
}

int
ut_array_1d_int_prod (int *data, int n)
{
  int i;
  int res = 1;

  for (i = 0; i < n; i++)
    res *= data[i];
  return res;
}

unsigned int
ut_array_1d_uint_sum (unsigned int *data, int n)
{
  int i;
  unsigned int res = 0;

  for (i = 0; i < n; i++)
    res += data[i];
  return res;
}

double
ut_array_1d_abssum (double *data, int n)
{
  int i;
  double res = 0;

  for (i = 0; i < n; i++)
    res += fabs (data[i]);

  return res;
}

int
ut_array_1d_int_abssum (int *data, int n)
{
  int i;
  int res = 0;

  for (i = 0; i < n; i++)
    res += abs (data[i]);
  return res;
}

void
ut_array_1d_abs (double *data, int n)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = fabs (data[i]);

  return;
}

void
ut_array_1d_int_abs (int *data, int n)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = ut_num_fabs_int (data[i]);

  return;
}

void
ut_array_1d_int_sgn (int *data, int n, int *sgn)
{
  int i;

  for (i = 0; i < n; i++)
    sgn[i] = (data[i] < 0) ? -1 : 1;

  return;
}

void
ut_array_1d_set (double *data, int n, double val)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = val;

  return;
}

void
ut_array_1d_int_set (int *data, int n, int val)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = val;

  return;
}

void
ut_array_1d_uint_set (unsigned int *data, int n, unsigned int val)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = val;

  return;
}

void
ut_array_2d_set (double **data, int n, int n2, double val)
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n2; j++)
      data[i][j] = val;

  return;
}

void
ut_array_2d_int_set (int **data, int n, int n2, int val)
{
  int i, j;

  for (i = 0; i < n; i++)
    for (j = 0; j < n2; j++)
      data[i][j] = val;

  return;
}

void
ut_array_2d_int_col_set (int **data, int n, int col, int val)
{
  int i;

  for (i = 0; i < n; i++)
    data[i][col] = val;

  return;
}

void
ut_array_2d_col_set (double **data, int n, int col, double val)
{
  int i;

  for (i = 0; i < n; i++)
    data[i][col] = val;

  return;
}

void
ut_array_3d_set (double ***data, int n, int n2, int n3, double val)
{
  int i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n2; j++)
      for (k = 0; k < n3; k++)
        data[i][j][k] = val;

  return;
}

void
ut_array_3d_int_set (int ***data, int n, int n2, int n3, int val)
{
  int i, j, k;

  for (i = 0; i < n; i++)
    for (j = 0; j < n2; j++)
      for (k = 0; k < n3; k++)
        data[i][j][k] = val;

  return;
}

int
ut_array_1d_rotpos (int beg, int end, int pos, int near)
{
  pos += near;

  while (pos > end)
    pos -= (end - beg + 1);

  while (pos < beg)
    pos += (end - beg + 1);

  return pos;
}

int
ut_array_1d_int_eltpos (int *array, int size, int val)
{
  int i;

  if (array == NULL)
    return -1;

  for (i = 0; i < size; i++)
    if (array[i] == val)
      return i;

  return -1;
}

int
ut_array_1d_uint_eltpos (unsigned int *array, int size, unsigned int val)
{
  int i;

  if (array == NULL)
    return -1;

  for (i = 0; i < size; i++)
    if (array[i] == val)
      return i;

  return -1;
}

int
ut_array_1d_int_eltpos_all (int *array, int size, int val, int *pos)
{
  int i, qty;

  if (array == NULL)
    return -1;

  qty = 0;
  for (i = 0; i < size; i++)
    if (array[i] == val)
      pos[qty++] = i;

  return qty;
}

int
ut_array_1d_string_eltpos (char **string, int size, char *val)
{
  int i;

  for (i = 0; i < size; i++)
    if (!strcmp (string[i], val))
      return i;

  return -1;
}

int
ut_array_2d_int_eltpos (int **array, int size1, int size2, int val, int *px,
                        int *py)
{
  int i, j, x, y;

  x = -1;
  y = -1;

  for (j = 0; j < size2; j++)
    for (i = 0; i < size1; i++)
      if (array[i][j] == val)
      {
        x = i;
        y = j;
        return 1;
      }

  if (px)
    (*px) = x;

  if (py)
    (*py) = y;

  return -1;
}

double
ut_array_1d_eltpos_interpolate (double *array, int size, double val)
{
  int i;
  double pos = -1;

  for (i = 0; i < size - 1; i++)
    if ((val >= array[i] && val <= array[i + 1])
        || (val <= array[i] && val >= array[i + 1]))
    {
      if (array[i + 1] - array[i] != 0)
        pos = i + (val - array[i]) / (array[i + 1] - array[i]);
      else
        pos = i + 0.5;

      break;
    }

  return pos;
}

void
ut_array_1d_int_uniq (int *array, int oldsize, int *psize)
{
  int i, size2;

  if (oldsize == 0)
    return;

  size2 = 0;
  for (i = 0; i < oldsize - 1; i++)
    if (array[i] != array[i + 1])
      array[++size2] = array[i + 1];

  *psize = size2 + 1;

  return;
}

void
ut_array_1d_int_sort_uniq (int *array, int oldsize, int *psize)
{
  ut_array_1d_int_sort (array, oldsize);
  ut_array_1d_int_uniq (array, oldsize, psize);

  return;
}

void
ut_array_1d_sub (double *data1, double *data2, int size, double *data3)
{
  int i;

  for (i = 0; i < size; i++)
    data3[i] = data2[i] - data1[i];

  return;
}

void
ut_array_2d_sub (double **data1, double **data2, int size1, int size2,
                 double **data3)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
      data3[i][j] = data2[i][j] - data1[i][j];

  return;
}

void
ut_array_1d_sample (double *b1, int nbl, double factor, double **pb2,
                    int *pnbl2)
{
  int id;
  double factorinv = 1 / factor;
  double rid;

  (*pnbl2) = (unsigned int) ((double) nbl * factor) + 10;

  (*pb2) = ut_alloc_1d (*pnbl2);

  (*pnbl2) = 0;
  rid = 0;
  id = 0;
  while (id < nbl)
  {
    id = ut_num_d2ri (rid);

    (*pb2)[(*pnbl2)++] = b1[id];
    rid += factorinv;
  }

  return;
}

void
ut_array_2d_sample (double **b1, int nbl, int nbc, double factor,
                    double ***pb2, int *pnbl2)
{
  int id;
  double factorinv = 1 / factor;
  int j;
  double rid;

  (*pnbl2) = (int) (((double) nbl * factor) + 10);

  (*pb2) = ut_alloc_2d (*pnbl2, nbc);

  (*pnbl2) = 0;
  rid = 0;
  while (rid < nbl)
  {
    id = ut_num_d2ri (rid);

    for (j = 0; j < nbc; j++)
      (*pb2)[*pnbl2][j] = b1[id][j];

    (*pnbl2)++;

    rid += factorinv;
  }

  return;
}

int
ut_array_1d_fscanf_sample (FILE * file, int tsize, double factor, double **pb,
                           int *psize)
{
  int res;
  double *data = ut_alloc_1d (tsize);

  res = ut_array_1d_fscanf (file, data, tsize);
  ut_array_1d_sample (data, tsize, factor, pb, psize);

  ut_free_1d (&data);

  return res;
}

int
ut_array_2d_fscanf_sample (FILE * file, int tsize, int size2, double factor,
                           double ***pb, int *psize)
{
  int res;
  double **data = ut_alloc_2d (tsize, size2);

  res = ut_array_2d_fscanf (file, data, tsize, size2);
  ut_array_2d_sample (data, tsize, size2, factor, pb, psize);

  ut_free_2d (&data, tsize);

  return res;
}

int
ut_array_1d_int_valnb (int *array, int size, int val)
{
  int qty = 0;
  int i;

  for (i = 0; i < size; i++)
    if (array[i] == val)
      qty++;

  return qty;
}

int
ut_array_1d_uint_valnb (unsigned int *array, int size, unsigned int val)
{
  int qty = 0;
  int i;

  for (i = 0; i < size; i++)
    if (array[i] == val)
      qty++;

  return qty;
}

void
ut_array_1d_int_cat (int *array, int sizea, int *array2, int sizeb,
                     int **parray3, int *psize3)
{
  int i, elt;

  (*parray3) = ut_alloc_1d_int (sizea + sizeb);

  elt = 0;
  for (i = 0; i < sizea; i++)
  {
    (*parray3)[elt] = array[i];
    elt++;
  }
  for (i = 0; i < sizeb; i++)
  {
    (*parray3)[elt] = array2[i];
    elt++;
  }

  if (psize3)
    (*psize3) = sizea + sizeb;

  return;
}

int
ut_array_1d_int_rmelt (int *array, int size, int value, int loop)
{
  int l;
  int i;
  int qty = 0;
  int pos;

  for (l = 1; l <= loop; l++)
  {
    pos = ut_array_1d_int_eltpos (array, size, value);
    if (pos != -1)
    {
      for (i = pos; i < size - 1; i++)
        array[i] = array[i + 1];
      qty++;
      size--;
    }
    else
      break;
  }

  return qty;
}

void
ut_array_1d_int_inter (int *b1, int b1qty, int *b2, int b2qty, int *b3,
                       int *pb3qty)
{
  int i;
  int *inter = ut_alloc_1d_int (b1qty + b2qty);
  int qty = 0;
  int *b2cpy = NULL;

  if (b2 == b3)
  {
    b2cpy = ut_alloc_1d_int (b2qty);
    ut_array_1d_int_memcpy (b2, b2qty, b2cpy);
  }
  else
    b2cpy = b2;

  for (i = 0; i < b2qty; i++)
    if (ut_array_1d_int_eltpos (b1, b1qty, b2cpy[i]) != -1)
      inter[qty++] = b2cpy[i];

  (*pb3qty) = qty;
  if (b3)
    ut_array_1d_int_memcpy (inter, qty, b3);

  if (b2cpy != b2)
    ut_free_1d_int (&b2cpy);

  ut_free_1d_int (&inter);

  return;
}

void
ut_array_2d_int_sortbycol (int **array, int nbl, int nbc, int col)
{
  int i, j;
  int *cpy = ut_alloc_1d_int (nbl);
  int *pos = ut_alloc_1d_int (nbl);

  assert (col < nbc && col >= 0);

  for (i = 0; i < nbl; i++)
    cpy[i] = array[i][col];

  ut_array_1d_int_sort_index (cpy, nbl, pos);

  for (i = 0; i < nbc; i++)
  {
    /* copying data */
    for (j = 0; j < nbl; j++)
      cpy[j] = array[j][i];

    /* recording data */
    for (j = 0; j < nbl; j++)
      array[j][i] = cpy[pos[j]];
  }

  ut_free_1d_int (&cpy);
  ut_free_1d_int (&pos);

  return;
}

void
ut_array_2d_int_sortbycol_des (int **array, int nbl, int nbc, int col)
{
  int i, j;
  int *cpy = ut_alloc_1d_int (nbl);
  int *pos = ut_alloc_1d_int (nbl);

  assert (col < nbc && col >= 0);

  for (i = 0; i < nbl; i++)
    cpy[i] = array[i][col];

  ut_array_1d_int_sort_index (cpy, nbl, pos);
  ut_array_1d_int_reverseelts (pos, nbl);

  for (i = 0; i < nbc; i++)
  {
    /* copying data */
    for (j = 0; j < nbl; j++)
      cpy[j] = array[j][i];

    /* recording data */
    for (j = 0; j < nbl; j++)
      array[j][i] = cpy[pos[j]];
  }

  ut_free_1d_int (&cpy);
  ut_free_1d_int (&pos);

  return;
}

/*
void
ut_array_2d_int_sortbycol_des (int** array, int nbl, int nbc, int col)
{
  int i, j, k, tmp;

  assert (col < nbc && col >= 0);

  for (i = 0; i < nbl; i++)
    for (j = nbl - 1; j > i; j--)
      if (array[j - 1][col] < array[j][col])
	for (k = 0; k < nbc; k++)
	{
	  tmp = array[j - 1][k];
	  array[j - 1][k] = array[j][k];
	  array[j][k] = tmp;
	}

  return;
}
*/

int
ut_array_2d_int_linepos (int **array, int line, int col, int *array2)
{
  int i, j, status;

  if (!array || !array2)
    return -1;

  for (i = 0; i < line; i++)
  {
    status = 1;
    for (j = 0; j < col; j++)
      if (array[i][j] != array2[j])
      {
        status = 0;
        break;
      }
    if (status == 1)
      return i;
  }

  return -1;
}

int
ut_array_2dns_int_linepos (int **array, int line, int *col, int *array2,
                           int col2)
{
  int i, j, status;

  if (array == NULL || array2 == NULL)
    return -1;

  for (i = 0; i < line; i++)
    if (col2 == col[i])
    {
      status = 1;
      for (j = 0; j < col2; j++)
        if (array[i][j] != array2[j])
        {
          status = 0;
          break;
        }
      if (status == 1)
        return i;
    }

  return -1;
}

void
ut_array_1d_int_inv (int *array, int qty, int **pbinv, int *pbinqty)
{
  int i, max;

  max = ut_array_1d_int_max (array, qty) + 1;

  ut_free_1d_int (pbinv);
  (*pbinv) = ut_alloc_1d_int (max);
  ut_array_1d_int_set (*pbinv, max, -1);

  for (i = 0; i < qty; i++)
    (*pbinv)[array[i]] = i;

  if (pbinqty)
    *pbinqty = max;

  return;
}

int
ut_array_1d_int_equal (int *array, int qtya, int *array2, int qtyb)
{
  int i;

  if (qtya != qtyb)
    return 0;

  for (i = 0; i < qtya; i++)
    if (array[i] != array2[i])
      return 0;

  return 1;
}

int
ut_array_1d_equal (double *array, double *array2, int qty, double eps)
{
  int i;

  for (i = 0; i < qty; i++)
    if (ut_num_equal (array[i], array2[i], eps) != 1)
      return 0;

  return 1;
}

int
ut_array_1d_requal (double *array, double *array2, int qty, double eps)
{
  int i;

  for (i = 0; i < qty; i++)
    if (ut_num_requal (array[i], array2[i], eps) != 1)
      return 0;

  return 1;
}

int
ut_array_2d_equal (double **array, double **array2, int size1, int size2,
                   double eps)
{
  int i, j;

  for (i = 0; i < size1; i++)
    for (j = 0; i < size2; i++)
      if (ut_num_equal (array[i][j], array2[i][j], eps) != 1)
        return 0;

  return 1;
}

void
ut_array_2d_transpose (double **A, int s1, int s2, double ***pT)
{
  int i, j;

  (*pT) = ut_alloc_2d (s2, s1);
  for (i = 0; i < s2; i++)
    for (j = 0; j < s1; j++)
      (*pT)[i][j] = A[j][i];

  return;
}

void
ut_array_2d_int_transpose (int **A, int s1, int s2, int ***pT)
{
  int i, j;

  (*pT) = ut_alloc_2d_int (s2, s1);
  for (i = 0; i < s2; i++)
    for (j = 0; j < s1; j++)
      (*pT)[i][j] = A[j][i];

  return;
}

int
ut_array_1d_int_list_addval (int **parray, int *psize, int val)
{
  int status = -1;

  if (ut_array_1d_int_eltpos (*parray, *psize, val) == -1)
  {
    ut_array_1d_int_list_addval_nocheck (parray, psize, val);
    status = 0;
  }

  return status;
}

int
ut_array_1d_int_list1_addval (int **parray, int *psize, int val)
{
  int status = -1;

  if (ut_array_1d_int_eltpos (*parray + 1, *psize, val) == -1)
  {
    ut_array_1d_int_list1_addval_nocheck (parray, psize, val);
    status = 0;
  }

  return status;
}

void
ut_array_1d_int_list_addvals (int **parray, int *psize, int *elts, int eltqty)
{
  int i;

  for (i = 0; i < eltqty; i++)
    ut_array_1d_int_list_addval (parray, psize, elts[i]);

  return;
}

int
ut_array_1d_int_list_addval_nocheck (int **parray, int *psize, int val)
{
  (*psize)++;
  (*parray) = ut_realloc_1d_int (*parray, *psize);
  (*parray)[(*psize) - 1] = val;

  return 0;
}

int
ut_array_1d_int_list1_addval_nocheck (int **parray, int *psize, int val)
{
  (*psize)++;
  (*parray) = ut_realloc_1d_int (*parray, *psize + 1);
  (*parray)[*psize] = val;

  return 0;
}

void
ut_array_1d_int_list_rmval (int **parray, int *psize, int val)
{
  (*psize) -= ut_array_1d_int_rmelt (*parray, *psize, val, 1);

  return;
}

void
ut_array_1d_int_list_rmvals (int **parray, int *psize, int *vals, int valqty)
{
  int i;

  for (i = 0; i < valqty; i++)
    ut_array_1d_int_list_rmval (parray, psize, vals[i]);

  return;
}

void
ut_array_1d_int_valnbs (int *array, int size, int **pvals, int **pvalnbs,
                        int *pvalqty)
{
  int i, qty;

  (*pvals) = ut_alloc_1d_int (size);
  (*pvalnbs) = ut_alloc_1d_int (size);

  ut_array_1d_int_memcpy (array, size, *pvals);
  ut_array_1d_int_sort_uniq (*pvals, size, pvalqty);

  for (i = 0; i < (*pvalqty); i++)
    (*pvalnbs)[i] = ut_array_1d_int_valnb (array, size, (*pvals)[i]);

  (*pvals) = ut_realloc_1d_int (*pvals, *pvalqty);
  (*pvalnbs) = ut_realloc_1d_int (*pvalnbs, *pvalqty);

  int *perm = ut_alloc_1d_int (*pvalqty);
  ut_array_1d_int_sort_index (*pvalnbs, *pvalqty, perm);
  ut_array_1d_int_reverseelts (perm, *pvalqty);
  ut_array_1d_int_permute (*pvals, *pvalqty, perm);
  ut_array_1d_int_permute (*pvalnbs, *pvalqty, perm);
  ut_free_1d_int (&perm);

  qty = 0;
  for (i = 0; i < *pvalqty; i += qty)
  {
    qty = ut_array_1d_int_valnb ((*pvalnbs) + i, *pvalqty - i, (*pvalnbs)[i]);
    ut_array_1d_int_sort ((*pvals) + i, qty);
  }

  return;
}

void
ut_array_1d_int_set_3 (int *array, int v1, int v2, int v3)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;

  return;
}

void
ut_array_1d_int_set_4 (int *array, int v1, int v2, int v3, int v4)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;
  array[3] = v4;

  return;
}

void
ut_array_1d_int_set_5 (int *array, int v1, int v2, int v3, int v4, int v5)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;
  array[3] = v4;
  array[4] = v5;

  return;
}

void
ut_array_1d_set_2 (double *array, double v1, double v2)
{
  array[0] = v1;
  array[1] = v2;

  return;
}

void
ut_array_1d_set_3 (double *array, double v1, double v2, double v3)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;

  return;
}

void
ut_array_1d_set_4 (double *array, double v1, double v2, double v3, double v4)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;
  array[3] = v4;

  return;
}

void
ut_array_1d_set_5 (double *array, double v1, double v2, double v3, double v4,
                   double v5)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;
  array[3] = v4;
  array[4] = v5;

  return;
}

void
ut_array_1d_set_6 (double *array, double v1, double v2, double v3, double v4,
                   double v5, double v6)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;
  array[3] = v4;
  array[4] = v5;
  array[5] = v6;

  return;
}

void
ut_array_1d_set_7 (double *array, double v1, double v2, double v3, double v4,
                   double v5, double v6, double v7)
{
  array[0] = v1;
  array[1] = v2;
  array[2] = v3;
  array[3] = v4;
  array[4] = v5;
  array[5] = v6;
  array[6] = v7;

  return;
}

void
ut_array_1d_int_set_2 (int *array, int v1, int v2)
{
  array[0] = v1;
  array[1] = v2;

  return;
}

void
ut_array_1d_int_set_id (int *array, int size)
{
  int i;

  for (i = 0; i < size; i++)
    array[i] = i;

  return;
}

int
ut_array_0d_fscanf_prefix (FILE * file, double *pval, char *flag)
{
  int status;
  char *tmp = ut_alloc_1d_char (1000);

  status = ut_array_0d_string_fscanf_prefix (file, tmp, flag);
  sscanf (tmp, "%lf", pval);

  ut_free_1d_char (&tmp);

  return status;
}

int
ut_array_0d_int_fscanf_prefix (FILE * file, int *pval, char *flag)
{
  int status;
  char *tmp = ut_alloc_1d_char (1000);

  status = ut_array_0d_string_fscanf_prefix (file, tmp, flag);
  sscanf (tmp, "%d", pval);

  ut_free_1d_char (&tmp);

  return status;
}

int
ut_array_0d_string_fscanf_prefix (FILE * file, char *val, char *flag)
{
  int status, qty;
  char *tmp = ut_alloc_1d_char (1000);
  char *lineflag = ut_alloc_1d_char (10000);

  if (!flag || !strcmp (flag, "") || !strcmp (flag, "none"))
  {
    do
    {
      status = -1;
      if (fgets (tmp, 10000, file))
      {
        tmp[strlen (tmp) - 1] = '\0';
        status = ut_string_nbwords (tmp);
      }
    }
    while (status > 1);

    if (status == 1)
      status = 1 + sscanf (tmp, "%s", val);
    else
      status = -1;
  }

  else
  {
    do
    {
      status = -1;
      qty = -1;
      if (fgets (tmp, 10000, file))
      {
        tmp[strlen (tmp) - 1] = '\0';
        status = sscanf (tmp, "%s", lineflag);
        qty = ut_string_nbwords (tmp);
      }
      if (qty > -1)
        status = 1;
    }
    while (status == 1 && strcmp (lineflag, flag));

    if (status == 1 && !strcmp (lineflag, flag))
      status = sscanf (tmp, "%s%s", lineflag, val);
    else
      status = -1;
  }

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&lineflag);

  return (status != 2) ? -1 : 1;
}

int
ut_array_1d_fscanf_prefix (FILE * file, double *array, int size, char *prefix)
{
  if (!strcmp (prefix, "none"))
    return ut_array_1d_fscanf (file, array, size);

  else
  {
    int i, tmpqty, status;
    char *tmp = ut_alloc_1d_char (1000);
    char *lineflag = ut_alloc_1d_char (10000);
    char **parts = NULL;
    int partqty = 0;

    tmpqty = 0;
    do
    {
      status = -1;
      if (fgets (tmp, 10000, file))
      {
        tmp[strlen (tmp) - 1] = '\0';
        status = sscanf (tmp, "%s", lineflag);
        if (!strcmp (lineflag, prefix))
        {
          ut_string_substrings (tmp + strlen (lineflag), &parts, &partqty);

          if (tmpqty + partqty > size)
            status = -1;
          else
          {
            for (i = 0; i < partqty; i++)
              sscanf (parts[i], "%lf", array + tmpqty++);
          }
        }
      }
    }
    while (status == 1 && tmpqty < size);

    ut_free_1d_char (&tmp);
    ut_free_1d_char (&lineflag);
    ut_free_2d_char (&parts, partqty);

    return (tmpqty < size) ? -1 : 1;
  }
}

int
ut_array_1d_int_fscanf_prefix (FILE * file, int *array, int size, char *flag)
{
  if (!strcmp (flag, "none"))
    return ut_array_1d_int_fscanf (file, array, size);

  else
  {
    int i, tmpqty, status;
    char *tmp = NULL;
    char *lineflag = NULL;
    char **parts = NULL;
    int partqty = 0;

    tmp = ut_alloc_1d_char (1000);
    lineflag = ut_alloc_1d_char (10000);

    tmpqty = 0;
    do
    {
      status = -1;
      if (fgets (tmp, 10000, file))
      {
        tmp[strlen (tmp) - 1] = '\0';
        status = sscanf (tmp, "%s", lineflag);
        if (!strcmp (lineflag, flag))
        {
          ut_string_substrings (tmp + strlen (lineflag), &parts, &partqty);

          if (tmpqty + partqty > size)
            status = -1;
          else
          {
            for (i = 0; i < partqty; i++)
              sscanf (parts[i], "%d", array + tmpqty++);
          }
        }
      }
    }
    while (status == 1 && tmpqty < size);

    ut_free_1d_char (&tmp);
    ut_free_1d_char (&lineflag);
    ut_free_2d_char (&parts, partqty);

    return (tmpqty < size) ? -1 : 1;
  }
}

int
ut_array_1d_string_fscanf_prefix (FILE * file, char **array, int size,
                                  char *flag)
{
  int i, tmpqty, status;
  char *tmp = NULL;
  char *lineflag = NULL;
  char **parts = NULL;
  int partqty = 0;

  if (!strcmp (flag, "none"))
  {
    status = ut_array_1d_string_fscanf (file, array, size);
    tmpqty = (status == 1) ? size : -1;
  }

  else
  {
    tmp = ut_alloc_1d_char (1000);
    lineflag = ut_alloc_1d_char (10000);

    tmpqty = 0;
    do
    {
      status = -1;
      if (fgets (tmp, 10000, file))
      {
        tmp[strlen (tmp) - 1] = '\0';
        status = sscanf (tmp, "%s", lineflag);
        if (!strcmp (lineflag, flag))
        {
          ut_string_substrings (tmp + strlen (lineflag), &parts, &partqty);

          if (tmpqty + partqty > size)
            status = -1;
          else
          {
            for (i = 0; i < partqty; i++)
              sscanf (parts[i], "%s", array[tmpqty++]);
          }
        }
      }
    }
    while (status == 1 && tmpqty < size);

    ut_free_1d_char (&tmp);
    ut_free_1d_char (&lineflag);
    ut_free_2d_char (&parts, partqty);
  }

  return (tmpqty < size) ? -1 : 1;
}

int
ut_array_2d_fscanf_prefix (FILE * file, double **array, int size1, int size2,
                           char *flag)
{
  int qty, status;
  char *tmp = NULL;

  if (!strcmp (flag, "none"))
  {
    status = ut_array_2d_fscanf (file, array, size1, size2);
    qty = size1;

    if (status == 1)
      status++;
  }

  else
  {
    tmp = ut_alloc_1d_char (1000);

    qty = 0;
    do
    {
      status = fscanf (file, "%s", tmp);
      status += ut_array_1d_fscanf (file, array[qty], size2);
      if (!strcmp (tmp, flag))
        qty++;
    }
    while (qty != size1 && status == 2);

    if (status != 2)
    {
      printf ("qty == %d vs size1 = %d\n", qty, size1);
      abort ();
    }

    ut_free_1d_char (&tmp);
  }

  return (status != 2 || qty != size1) ? -1 : 1;
}

int
ut_array_2d_int_fscanf_prefix (FILE * file, int **array, int size1, int size2,
                               char *flag)
{
  int qty, status;
  char *tmp = NULL;

  if (!strcmp (flag, "none"))
  {
    status = ut_array_2d_int_fscanf (file, array, size1, size2);
    qty = size1;

    if (status == 1)
      status++;
  }

  else
  {
    tmp = ut_alloc_1d_char (1000);

    qty = 0;
    do
    {
      status = fscanf (file, "%s", tmp);
      status += ut_array_1d_int_fscanf (file, array[qty], size2);
      if (!strcmp (tmp, flag))
        qty++;
    }
    while (qty != size1);

    ut_free_1d_char (&tmp);
  }

  return (status != 2 || qty != size1) ? -1 : 1;
}

int
ut_array_1d_int_opposite (int *array, int *array2, int qty)
{
  int i;

  for (i = 0; i < qty; i++)
    if (array[i] != -array2[i])
      return 0;

  return 1;
}

void
ut_array_3d_int_switcharrays (int ***array, int size1, int size2, int line1,
                              int line2)
{
  int i, j;
  double tmp = 0;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
    {
      tmp = array[line1][i][j];
      array[line1][i][j] = array[line2][i][j];
      array[line2][i][j] = tmp;
    }

  return;
}

void
ut_array_3d_switcharrays (double ***array, int size1, int size2, int line1,
                          int line2)
{
  int i, j;
  double tmp = 0;

  for (i = 0; i < size1; i++)
    for (j = 0; j < size2; j++)
    {
      tmp = array[line1][i][j];
      array[line1][i][j] = array[line2][i][j];
      array[line2][i][j] = tmp;
    }

  return;
}

int
ut_array_1d_int_listpair_merge (int *list1, int qty1, int *list2, int qty2,
                                int **plist, int *pqty)
{
  int qty, *tmp = NULL;

  qty = qty1 + qty2;
  tmp = ut_alloc_1d_int (qty);
  ut_array_1d_int_memcpy (list1, qty1, tmp);
  ut_array_1d_int_memcpy (list2, qty2, tmp + qty1);
  ut_array_1d_int_sort_uniq (tmp, qty, &qty);

  if (pqty)
    (*pqty) = qty;

  if (plist)
  {
    (*plist) = ut_realloc_1d_int (*plist, qty);
    ut_array_1d_int_memcpy (tmp, qty, *plist);
  }

  ut_free_1d_int (&tmp);

  return 0;
}

int
ut_array_1d_int_listpair_equal (int *list1, int qty1, int *list2, int qty2)
{
  if (qty1 != qty2)
    return 0;

  int status;
  int *list1b = ut_alloc_1d_int (qty1);
  int *list2b = ut_alloc_1d_int (qty2);

  ut_array_1d_int_memcpy (list1, qty1, list1b);
  ut_array_1d_int_memcpy (list2, qty2, list2b);

  ut_array_1d_int_sort (list1b, qty1);
  ut_array_1d_int_sort (list2b, qty2);

  status = ut_array_1d_int_equal (list1b, qty1, list2b, qty2);

  ut_free_1d_int (&list1b);
  ut_free_1d_int (&list2b);

  return status;
}

void
ut_array_1d_int_lists_merge (int **listvals, int *listvalqty, int listqty,
                             int **pvals, int *pvalqty)
{
  int i, pos;

  (*pvalqty) = ut_array_1d_int_sum (listvalqty, listqty);

  (*pvals) = ut_alloc_1d_int (*pvalqty);

  pos = 0;
  for (i = 0; i < listqty; i++)
  {
    ut_array_1d_int_memcpy (listvals[i], listvalqty[i], *pvals + pos);
    pos += listvalqty[i];
  }

  ut_array_1d_int_sort_uniq (*pvals, *pvalqty, pvalqty);

  return;
}

int
ut_array_1d_int_duplicates (int *array, int qty)
{
  int qty2 = 0, *tmp = ut_alloc_1d_int (qty);

  ut_array_1d_int_memcpy (array, qty, tmp);
  ut_array_1d_int_sort_uniq (tmp, qty, &qty2);

  ut_free_1d_int (&tmp);

  return (qty != qty2);
}

int
ut_array_1d_int_duplicates_nonzero (int *array, int qty)
{
  int qty2 = 0, *tmp = ut_alloc_1d_int (qty);
  int qty3;

  ut_array_1d_int_memcpy (array, qty, tmp);
  ut_array_1d_int_sort_uniq (tmp, qty, &qty2);
  qty3 = ut_array_1d_int_valnb (array, qty, 0);

  qty -= qty3;
  qty2 -= qty3 ? 1 : 0;

  ut_free_1d_int (&tmp);

  return (qty != qty2);
}

int
ut_array_1d_eltpos (double *array, int size, double val)
{
  int i, minid;
  double min;

  if (array == NULL)
    return -1;

  minid = -1;
  min = DBL_MAX;
  for (i = 0; i < size; i++)
    if (fabs (array[i] - val) < min)
    {
      min = fabs (array[i] - val);
      minid = i;
    }

  return minid;
}

#ifdef HAVE_GSL
void
ut_array_1d_set_rand (double *data, int n, double min, double max,
                      gsl_rng * r)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = min + (max - min) * gsl_rng_uniform (r);

  return;
}

void
ut_array_1d_int_set_rand (int *data, int n, int min, int max, gsl_rng * r)
{
  int i;

  for (i = 0; i < n; i++)
    data[i] = min + (max - min + 1) * gsl_rng_uniform (r);

  return;
}

void
ut_array_1d_int_choose (int *src, int srcsize, int *dest, int destsize,
                        gsl_rng * r)
{
  gsl_ran_choose (r, dest, destsize, src, srcsize, sizeof (int));

  return;
}

void
ut_array_1d_round_keepsum (double *array, int size, double *array2)
{
  int i, id, remaining, *status = ut_alloc_1d_int (size);
  double correc, *diff = ut_alloc_1d (size);

  ut_array_1d_memcpy (array, size, array2);

  remaining = size;
  while (remaining > 1)
  {
    // computing the difference to the nearest integer (excluding those already rounded)
    for (i = 0; i < size; i++)
      if (status[i] == 0)
        diff[i] = ut_num_d2ri (array2[i]) - array2[i];
      else
        diff[i] = DBL_MAX;

    // picking the element with the smallest difference
    id = ut_array_1d_absmin_index (diff, size);

    // distributing the difference to other (non-rounded) elements
    correc = diff[id] / --remaining;
    status[id] = 1;
    array2[id] = ut_num_d2ri (array2[id]);
    for (i = 0; i < size; i++)
      if (!status[i])
        array2[i] -= correc;

    if (remaining == 1)
      for (i = 0; i < size; i++)
        if (!status[i])
        {
          array2[i] = ut_num_d2ri (array2[i]);
          break;
        }
  }

  ut_free_1d_int (&status);
  ut_free_1d (&diff);

  return;
}

#endif // HAVE_GSL

int
ut_array_2d_pair (double **array, int sizea1, int sizea2, double **array2,
                  int sizeb1, int sizeb2, int *pair, double *pdist)
{
  if (sizea1 != sizeb1 || sizea2 != sizeb2)
    return -1;

  int i, j, status, size1 = sizea1, size2 = sizea2;
  double *dists = ut_alloc_1d (size1);
  double *v = ut_alloc_1d (size2);
  int *hint = ut_alloc_1d_int (size1);

  status = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size1; j++)
    {
      ut_array_1d_sub (array[i], array2[j], size2, v);
      dists[j] = ut_array_1d_norm (v, size2);
    }

    pair[i] = ut_array_1d_min_index (dists, size1);
    hint[pair[i]]++;

    if (hint[pair[i]] > 1)
    {
      status = -1;
      break;
    }
  }

  if (!status)
  {
    (*pdist) = 0;
    for (i = 0; i < sizea1; i++)
    {
      ut_array_1d_sub (array[i], array2[pair[i]], size2, v);
      (*pdist) += ut_array_1d_norm (v, size2);
    }
  }

  ut_free_1d (&dists);
  ut_free_1d (&v);
  ut_free_1d_int (&hint);

  return status;
}

int
ut_array_2d_int_pair (int **array, int sizea1, int sizea2, int **array2,
                      int sizeb1, int sizeb2, int *pair)
{
  if (sizea1 != sizeb1 || sizea2 != sizeb2)
    return -1;

  int i, j, status, size1 = sizea1, size2 = sizea2;
  int *hint = ut_alloc_1d_int (size1);

  status = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size1; j++)
      if (ut_array_1d_int_equal (array[i], size2, array2[i], size2))
      {
        pair[i] = j;
        hint[pair[i]]++;
        break;
      }

    if (hint[pair[i]] > 1)
    {
      status = -1;
      break;
    }
  }

  ut_free_1d_int (&hint);

  return status;
}

int
ut_array_1d_int_lists_pair (int **array, int *sizea2, int sizea1,
                            int **array2, int *sizeb2, int sizeb1, int *pair)
{
  if (sizea1 != sizeb1)
    return -1;

  int i, j, status, size1 = sizea1;
  int *hint = ut_alloc_1d_int (size1);

  status = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size1; j++)
      if (ut_array_1d_int_listpair_equal
          (array[i], sizea2[i], array2[j], sizeb2[j]))
      {
        pair[i] = j;
        hint[pair[i]]++;
        break;
      }

    if (hint[pair[i]] > 1)
    {
      status = -1;
      break;
    }
  }

  ut_free_1d_int (&hint);

  return status;
}

int
ut_array_1d_int_lists_pair_samelength (int **array, int sizea2, int sizea1,
                                       int **array2, int sizeb2, int sizeb1,
                                       int *pair)
{
  if (sizea1 != sizeb1 || sizea2 != sizeb2)
    return -1;

  int i, j, status, size1 = sizea1, size2 = sizea2;
  int *hint = ut_alloc_1d_int (size1);

  status = 0;
  for (i = 0; i < size1; i++)
  {
    for (j = 0; j < size1; j++)
      if (ut_array_1d_int_listpair_equal (array[i], size2, array2[j], size2))
      {
        pair[i] = j;
        hint[pair[i]]++;
        break;
      }

    if (hint[pair[i]] > 1)
    {
      status = -1;
      break;
    }
  }

  ut_free_1d_int (&hint);

  return status;
}

void
ut_array_1d_int_bubbleelt (int *array, int pos)
{
  int i, val;

  val = array[pos];

  for (i = pos; i > 0; i--)
    array[i] = array[i - 1];

  array[0] = val;

  return;
}

int
ut_array_rotpos (int beg, int end, int pos, int near)
{
  pos += near;

  if (end < beg)
    abort ();

  while (pos > end)
    pos -= (end - beg + 1);

  while (pos < beg)
    pos += (end - beg + 1);

  return pos;
}
