/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_tesr_.h"

void
nev_print_data_tesr (struct SIM Sim, struct TESR *pTesr, char *entity, char *attribute,
                     char *datastring, struct DATA *TesrData)
{
  struct DATA *pData = NULL;

  ut_print_message (0, 1, "Reading data (%s%s%s)...\n", entity, strlen (attribute) ? ", " : "", attribute);

  if (!strcmp (entity, "cell"))
    pData = TesrData;
  else
    pData = TesrData + 1;

  if (!strcmp (entity, "cell") || !strcmp (entity, "vox"))
    nev_print_data_tesr_gen (Sim, pTesr, pData, entity, attribute, datastring);

  else if (!strcmp (entity, "edge"))
    nev_print_data_tesr_edge (pData, attribute, datastring);

  else if (!strcmp (entity, "voidvox"))
    nev_print_data_tesr_voidvox (pData, attribute, datastring);

  else if (!strcmp (entity, "voxedge"))
    nev_print_data_tesr_voxedge (pData, attribute, datastring);

  else
    abort ();

  if (!strcmp (entity, "cell"))
    nev_print_data_tesr_cell2vox (*pTesr, attribute, TesrData[0], TesrData + 1);

  return;
}
