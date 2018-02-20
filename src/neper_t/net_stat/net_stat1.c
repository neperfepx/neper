/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_stat_.h"

void
net_stat (struct IN_T In, struct TESS Tess, struct TESR *pTesr,
	  struct POINT Point)
{
  int i, qty, stattess, stattesr, statpoint;
  FILE *file = NULL;
  char *name = NULL;
  char *ext = ut_alloc_1d_char (100);
  char ***data = ut_alloc_2d_pchar (100, 2);

  qty = 0;

  if (In.stv)
  {
    ut_string_string ("ver", &(data[qty][0]));
    ut_string_string (In.stv, &(data[qty][1]));
    qty++;
  }
  if (In.ste)
  {
    ut_string_string ("edge", &(data[qty][0]));
    ut_string_string (In.ste, &(data[qty][1]));
    qty++;
  }
  if (In.stf)
  {
    ut_string_string ("face", &(data[qty][0]));
    ut_string_string (In.stf, &(data[qty][1]));
    qty++;
  }
  if (In.stp)
  {
    ut_string_string ("poly", &(data[qty][0]));
    ut_string_string (In.stp, &(data[qty][1]));
    qty++;
  }
  if (In.stc)
  {
    ut_string_string ("cell", &(data[qty][0]));
    ut_string_string (In.stc, &(data[qty][1]));
    qty++;
  }
  if (In.stpt)
  {
    ut_string_string ("point", &(data[qty][0]));
    ut_string_string (In.stpt, &(data[qty][1]));
    qty++;
  }
  if (In.sts)
  {
    ut_string_string ("seed", &(data[qty][0]));
    ut_string_string (In.sts, &(data[qty][1]));
    qty++;
  }

  for (i = 0; i < qty; i++)
  {
    stattess = stattesr = statpoint = 0;

    sprintf (ext, "st%s", data[i][0]);
    name = ut_string_addextension (In.body, ext);

    if (!strcmp (data[i][0], "seed") || !strcmp (data[i][0], "cell"))
    {
      if (Tess.Dim > 0)
	stattess = 1;
      else if ((*pTesr).Dim > 0)
	stattesr = 1;
    }
    else if (!strcmp (data[i][0], "ver") || !strcmp (data[i][0], "edge")
	     || !strcmp (data[i][0], "face") || !strcmp (data[i][0], "poly"))
      stattess = 1;
    else if (!strcmp (data[i][0], "point"))
      statpoint = 1;

    if (stattess)
    {
      file = ut_file_open (name, "w");
      net_stat_tess (file, data[i][0], data[i][1], Tess);
      ut_file_close (file, name, "w");
    }
    else if (stattesr)
    {
      file = ut_file_open (name, "w");
      net_stat_tesr (file, data[i][0], data[i][1], pTesr);
      ut_file_close (file, name, "w");
    }
    else if (statpoint)
    {
      file = ut_file_open (name, "w");
      net_stat_point (file, data[i][1], Point, Tess);
      ut_file_close (file, name, "w");
    }
    ut_free_1d_char (name);
  }

  ut_free_1d_char (ext);
  ut_free_3d_char (data, 100, 2);

  return;
}
