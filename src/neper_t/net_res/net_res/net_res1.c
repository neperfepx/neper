/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_res_.h"

void
net_res (struct IN_T In, struct TESS *pTess, struct TESR *pTesr,
          struct POINT Point)
{
  int i, qty, restess, restesr, respoint;
  FILE *file = NULL;
  char *name = NULL;
  char *ext = ut_alloc_1d_char (100);
  char ***data = ut_alloc_2d_pchar (100, 2);

  qty = 0;

  if (In.stv)
  {
    ut_string_string ("ver", data[qty]);
    ut_string_string (In.stv, data[qty] + 1);
    qty++;
  }
  if (In.ste)
  {
    ut_string_string ("edge", data[qty]);
    ut_string_string (In.ste, data[qty] + 1);
    qty++;
  }
  if (In.stf)
  {
    ut_string_string ("face", data[qty]);
    ut_string_string (In.stf, data[qty] + 1);
    qty++;
  }
  if (In.stp)
  {
    ut_string_string ("poly", data[qty]);
    ut_string_string (In.stp, data[qty] + 1);
    qty++;
  }
  if (In.stc)
  {
    ut_string_string ("cell", data[qty]);
    ut_string_string (In.stc, data[qty] + 1);
    qty++;
  }
  if (In.sttess)
  {
    ut_string_string ("tess", data[qty]);
    ut_string_string (In.sttess, data[qty] + 1);
    qty++;
  }
  if (In.sttesr)
  {
    ut_string_string ("tesr", data[qty]);
    ut_string_string (In.sttesr, data[qty] + 1);
    qty++;
  }
  if (In.stpt)
  {
    ut_string_string ("point", data[qty]);
    ut_string_string (In.stpt, data[qty] + 1);
    qty++;
  }
  if (In.sts)
  {
    ut_string_string ("seed", data[qty]);
    ut_string_string (In.sts, data[qty] + 1);
    qty++;
  }
  if (In.stvox)
  {
    ut_string_string ("vox", data[qty]);
    ut_string_string (In.stvox, data[qty] + 1);
    qty++;
  }
  if (In.stgroup)
  {
    ut_string_string ("group", data[qty]);
    ut_string_string (In.stgroup, data[qty] + 1);
    qty++;
  }

  for (i = 0; i < qty; i++)
  {
    restess = restesr = respoint = 0;

    sprintf (ext, "st%s", data[i][0]);
    name = ut_string_addextension (In.body, ext);

    if (!strcmp (data[i][0], "seed") || !strcmp (data[i][0], "cell"))
    {
      if ((*pTess).Dim > 0)
        restess = 1;
      else if ((*pTesr).Dim > 0)
        restesr = 1;
    }
    if (!strcmp (data[i][0], "group"))
    {
      if ((*pTess).Dim > 0)
        restess = 1;
      else if ((*pTesr).Dim > 0)
        restesr = 1;
    }
    else if (!strcmp (data[i][0], "ver") || !strcmp (data[i][0], "edge")
             || !strcmp (data[i][0], "face") || !strcmp (data[i][0], "poly")
             || !strcmp (data[i][0], "tess"))
      restess = 1;
    else if (!strcmp (data[i][0], "vox") || !strcmp (data[i][0], "tesr"))
      restesr = 1;
    else if (!strcmp (data[i][0], "point"))
      respoint = 1;

    if (restess)
    {
      file = ut_file_open (name, "w");
      net_res_tess (file, data[i][0], data[i][1], pTess);
      ut_file_close (file, name, "w");
    }
    else if (restesr)
    {
      file = ut_file_open (name, "w");
      net_res_tesr (file, data[i][0], data[i][1], pTesr);
      ut_file_close (file, name, "w");
    }
    else if (respoint)
    {
      file = ut_file_open (name, "w");
      net_res_point (file, data[i][1], Point, (*pTess));
      ut_file_close (file, name, "w");
    }
    ut_free_1d_char (&name);
  }

  ut_free_1d_char (&ext);
  ut_free_3d_char (&data, 100, 2);

  return;
}
