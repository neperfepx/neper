/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_map_alloc.h"

struct OL_MAP
ol_map_alloc (size_t xsize, size_t ysize, double stepsize, char* crysym)
{
  unsigned int i, j, k;
  struct OL_MAP Map;

  Map.xsize = xsize;
  Map.ysize = ysize;
  Map.stepsize = stepsize;

  Map.gmap = ol_g_alloc ();
  ol_g_set_id (Map.gmap);

  Map.id = ut_alloc_2d_uint (xsize, ysize);
  Map.q = ut_alloc_3d (xsize, ysize, 4);
  Map.rgb = ut_alloc_3d_int (xsize, ysize, 3);
  for (i = 0; i < xsize; i++)
    for (j = 0; j < ysize; j++)
      for (k = 0; k < 3; k++)
	Map.rgb[i][j][k] = 255;

  if (crysym)
  {
    Map.crysym = ut_alloc_1d_char (strlen (crysym) + 1);
    strcpy (Map.crysym, crysym);
  }
  else
    Map.crysym = NULL;

  return Map;
}

void
ol_map_free (struct OL_MAP Map)
{
  if (Map.id)
    ut_free_2d_uint (Map.id, Map.xsize);

  if (Map.q)
    ut_free_3d (Map.q, Map.xsize, Map.ysize);

  if (Map.rgb)
    ut_free_3d_int (Map.rgb, Map.xsize, Map.ysize);

  if (Map.gmap)
    ol_g_free (Map.gmap);

  return;
}

int
ol_map_fscanf (FILE * file, struct OL_MAP *pMap)
{
  size_t i;
  int qty;
  unsigned int id, x, y;
  double *e = ol_e_alloc ();
  unsigned int xsize, ysize;
  char* crysym = ut_alloc_1d_char (1000);

  ut_file_nextline_nbwords (file, &qty);
  if (qty == 3)
  {
    if (fscanf (file, "%u%u%lf",
		&xsize, &ysize, &((*pMap).stepsize)) != 3)
      return -1;
    sprintf (crysym, "cubic");
  }
  else if (qty == 4)
  {
    if (fscanf (file, "%u%u%lf%s",
		&xsize, &ysize, &((*pMap).stepsize), crysym) != 4)
      return -1;
  }
  else
    return -1;

  (*pMap).xsize = (size_t) xsize;	/* stuff useful for compilation with */
  (*pMap).ysize = (size_t) ysize;	/* -pedantic (code portability) */

  (*pMap) = ol_map_alloc ((*pMap).xsize, (*pMap).ysize,
                          (*pMap).stepsize, crysym);

  for (i = 0; i < (*pMap).xsize * (*pMap).ysize; i++)
  {
    if (fscanf (file, "%u%u%u", &id, &x, &y) != 3)
    {
      ol_map_free (*pMap);
      return 0;
    }

    if (x > (*pMap).xsize - 1 || y > (*pMap).ysize - 1)
      abort ();

    (*pMap).id[x][y] = id;
    if (ol_e_fscanf (file, e) != 1)
    {
      ol_map_free (*pMap);
      return 0;
    }

    ol_e_q (e, (*pMap).q[x][y]);
  }

  ol_e_free (e);
  ut_free_1d_char (crysym);

  return 1;
}

int
ol_map_fprintf (FILE * file, struct OL_MAP Map, char *format)
{
  unsigned int i, j;
  int res;
  double *e0 = ol_e_alloc ();
  double *e = ol_e_alloc ();

  fprintf (file, "%u %u ", (unsigned int) Map.xsize,
	   (unsigned int) Map.ysize);
  fprintf (file, format, Map.stepsize);
  res = fprintf (file, "\n");
  if (res < 0)
    return res;

  for (j = 0; j < Map.ysize; j++)
    for (i = 0; i < Map.xsize; i++)
    {
      fprintf (file, "%d %u %u ", Map.id[i][j], i, j);
      if (Map.id[i][j] == 0)
	res = ol_e_fprintf (file, e0, format);
      else
      {
	ol_q_e_rad (Map.q[i][j], e);
	ol_e_rad2deg (e, e);
	res = ol_e_fprintf (file, e, format);
      }
    }

  ol_e_free (e);
  ol_e_free (e0);

  return res;
}

int
ol_map_fscanf_ch5 (FILE * file, struct OL_MAP *pMap)
{
  unsigned int i, j;
  int k, DataQty, res = 1;
  double tmp, size;
  double *e = ol_e_alloc ();
  double **coo = ut_alloc_2d (2, 2);

  /* determining quantity of data */
  DataQty = ut_file_nblines_pointer (file);

  /* Parsing input file to determine the map sizes. */
  size = 0;
  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[0][j]));
  ut_file_skip (file, 9);

  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[1][j]));
  ut_file_skip (file, 9);

  if (ut_num_equal (coo[0][1], coo[1][1], 1e-6))
    (*pMap).stepsize = coo[1][0] - coo[0][0];
  else if (ut_num_equal (coo[0][0], coo[1][0], 1e-6))
    (*pMap).stepsize = coo[1][1] - coo[0][1];
  else
    abort ();

  ut_file_skip (file, 1);
  ut_file_readwcomma (file, &tmp);
  while (tmp > size)
  {
    size = tmp;
    ut_file_skip (file, 11);
    ut_file_readwcomma (file, &tmp);
  }
  (*pMap).xsize = ut_num_d2ri (size / (*pMap).stepsize + 1);
  (*pMap).ysize = DataQty / (*pMap).xsize;

  fseek (file, 0, 0);

  /* allocating memory */
  (*pMap).q = ut_alloc_3d ((*pMap).xsize, (*pMap).ysize, 4);
  (*pMap).id = ut_alloc_2d_uint ((*pMap).xsize, (*pMap).ysize);

  /* reading data */
  for (j = 0; j < (*pMap).ysize; j++)
    for (i = 0; i < (*pMap).xsize; i++)
    {
      /* read should be obtained from ut_file_readwcomma ... */
      res = fscanf (file, "%u", &(*pMap).id[i][j]);
      ut_file_skip (file, 2);
      for (k = 0; k < 3; k++)
	ut_file_readwcomma (file, &(e[k]));
      ol_e_q (e, (*pMap).q[i][j]);
      ut_file_skip (file, 6);
    }

  ol_e_free (e);
  ut_free_2d (coo, 2);

  return res;
}

int
ol_map_fscanf_ch5_short (FILE * file, struct OL_MAP *pMap)
{
  unsigned int i, j;
  int k, DataQty, res = 1;
  double tmp, size;
  double *e = ol_e_alloc ();
  double **coo = ut_alloc_2d (2, 2);

  /* determining quantity of data */
  DataQty = ut_file_nblines_pointer (file);

  /* Parsing input file to determine the map sizes. */
  size = 0;
  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[0][j]));
  ut_file_skip (file, 3);

  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[1][j]));
  ut_file_skip (file, 3);

  if (ut_num_equal (coo[0][1], coo[1][1], 1e-6))
    (*pMap).stepsize = coo[1][0] - coo[0][0];
  else if (ut_num_equal (coo[0][0], coo[1][0], 1e-6))
    (*pMap).stepsize = coo[1][1] - coo[0][1];
  else
    abort ();

  ut_file_skip (file, 1);
  ut_file_readwcomma (file, &tmp);
  while (tmp > size)
  {
    size = tmp;
    ut_file_skip (file, 5);
    ut_file_readwcomma (file, &tmp);
  }
  (*pMap).xsize = ut_num_d2ri (size / (*pMap).stepsize + 1);
  (*pMap).ysize = DataQty / (*pMap).xsize;

  fseek (file, 0, 0);

  /* allocating memory */
  (*pMap).q = ut_alloc_3d ((*pMap).xsize, (*pMap).ysize, 4);
  (*pMap).id = ut_alloc_2d_uint ((*pMap).xsize, (*pMap).ysize);

  /* reading data */
  for (j = 0; j < (*pMap).ysize; j++)
    for (i = 0; i < (*pMap).xsize; i++)
    {
      /* read should be obtained from ut_file_readwcomma ... */
      res = fscanf (file, "%u", &(*pMap).id[i][j]);
      ut_file_skip (file, 2);
      for (k = 0; k < 3; k++)
	ut_file_readwcomma (file, &(e[k]));
      ol_e_q (e, (*pMap).q[i][j]);
    }

  ol_e_free (e);
  ut_free_2d (coo, 2);

  return res;
}

int
ol_map_fscanf_ch5_rad (FILE * file, struct OL_MAP *pMap)
{
  unsigned int i, j;
  int k, DataQty, res = 1;
  double tmp, size;
  double *e = ol_e_alloc ();
  double **coo = ut_alloc_2d (2, 2);

  /* determining quantity of data */
  DataQty = ut_file_nblines_pointer (file);

  /* Parsing input file to determine the map sizes. */
  size = 0;
  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[0][j]));
  ut_file_skip (file, 9);

  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[1][j]));
  ut_file_skip (file, 9);

  if (ut_num_equal (coo[0][1], coo[1][1], 1e-6))
    (*pMap).stepsize = coo[1][0] - coo[0][0];
  else if (ut_num_equal (coo[0][0], coo[1][0], 1e-6))
    (*pMap).stepsize = coo[1][1] - coo[0][1];
  else
    abort ();

  ut_file_skip (file, 1);
  ut_file_readwcomma (file, &tmp);
  while (tmp > size)
  {
    size = tmp;
    ut_file_skip (file, 11);
    ut_file_readwcomma (file, &tmp);
  }
  (*pMap).xsize = ut_num_d2ri (size / (*pMap).stepsize + 1);
  (*pMap).ysize = DataQty / (*pMap).xsize;

  fseek (file, 0, 0);

  /* allocating memory */
  (*pMap).q = ut_alloc_3d ((*pMap).xsize, (*pMap).ysize, 4);
  (*pMap).id = ut_alloc_2d_uint ((*pMap).xsize, (*pMap).ysize);

  /* reading data */
  for (j = 0; j < (*pMap).ysize; j++)
    for (i = 0; i < (*pMap).xsize; i++)
    {
      /* read should be obtained from ut_file_readwcomma ... */
      res = fscanf (file, "%u", &(*pMap).id[i][j]);
      ut_file_skip (file, 2);
      for (k = 0; k < 3; k++)
	ut_file_readwcomma (file, &(e[k]));
      ol_e_q_rad (e, (*pMap).q[i][j]);
      ut_file_skip (file, 6);
    }

  ol_e_free (e);
  ut_free_2d (coo, 2);

  return res;
}

int
ol_map_fscanf_ch5_short_rad (FILE * file, struct OL_MAP *pMap)
{
  unsigned int i, j;
  int k, DataQty, res = 1;
  double tmp, size;
  double *e = ol_e_alloc ();
  double **coo = ut_alloc_2d (2, 2);

  /* determining quantity of data */
  DataQty = ut_file_nblines_pointer (file);

  /* Parsing input file to determine the map sizes. */
  size = 0;
  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[0][j]));
  ut_file_skip (file, 3);

  ut_file_skip (file, 1);
  for (j = 0; j < 2; j++)
    ut_file_readwcomma (file, &(coo[1][j]));
  ut_file_skip (file, 3);

  if (ut_num_equal (coo[0][1], coo[1][1], 1e-6))
    (*pMap).stepsize = coo[1][0] - coo[0][0];
  else if (ut_num_equal (coo[0][0], coo[1][0], 1e-6))
    (*pMap).stepsize = coo[1][1] - coo[0][1];
  else
    abort ();

  ut_file_skip (file, 1);
  ut_file_readwcomma (file, &tmp);
  while (tmp > size)
  {
    size = tmp;
    ut_file_skip (file, 5);
    ut_file_readwcomma (file, &tmp);
  }
  (*pMap).xsize = ut_num_d2ri (size / (*pMap).stepsize + 1);
  (*pMap).ysize = DataQty / (*pMap).xsize;

  fseek (file, 0, 0);

  /* allocating memory */
  (*pMap).q = ut_alloc_3d ((*pMap).xsize, (*pMap).ysize, 4);
  (*pMap).id = ut_alloc_2d_uint ((*pMap).xsize, (*pMap).ysize);

  /* reading data */
  for (j = 0; j < (*pMap).ysize; j++)
    for (i = 0; i < (*pMap).xsize; i++)
    {
      /* read should be obtained from ut_file_readwcomma ... */
      res = fscanf (file, "%u", &(*pMap).id[i][j]);
      ut_file_skip (file, 2);
      for (k = 0; k < 3; k++)
	ut_file_readwcomma (file, &(e[k]));
      ol_e_q_rad (e, (*pMap).q[i][j]);
    }

  ol_e_free (e);
  ut_free_2d (coo, 2);

  return res;
}

int
ol_map_fprintf_ch5 (FILE * file, struct OL_MAP Map, char *format)
{
  unsigned int i, j;
  int res = 1;
  double *e = ol_e_alloc ();

  for (j = 0; j < Map.ysize; j++)
    for (i = 0; i < Map.xsize; i++)
    {
      fprintf (file, "%d %f %f ", Map.id[i][j], i * Map.stepsize,
	       j * Map.stepsize);
      ol_q_e (Map.q[i][j], e);
      ol_e_e (e, e);
      res = ol_e_fprintf (file, e, format);
    }

  ol_e_free (e);

  return res;
}
