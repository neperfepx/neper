/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_sort_.h"

void
net_sort (struct IN_T In, struct TESS *pTess, struct TESR *pTesr)
{
  if (pTess)
    net_sort_tess (pTess, In.sortstring);

  if (pTesr)
    ut_print_message (2, 1, "Not available yet...\n");

  return;
}
