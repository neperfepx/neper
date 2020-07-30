/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_tess_.h"

struct DATA *
nev_data_tess_init (struct TESS Tess, struct DATA *TessData,
                           char **pentity, char *attribute,
                           char *datastring, int *pdim, int *pentityqty,
                           char **ptype, char **pvalue)
{
  ut_print_message (0, 1, "Reading data (%s, %s)...\n", *pentity, attribute);

  neut_tess_entity_dim (*pentity, pdim);

  neut_tess_entity_qty (Tess, *pentity, pentityqty);

  if (*pdim < 0 || *pentityqty < 0)
    abort ();

  neut_data_datastring_type_value (*pentity, attribute, datastring, ptype, pvalue);

  return TessData + (*pdim);
}
