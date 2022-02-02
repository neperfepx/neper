/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_data_tesr_.h"

void
nev_data_tesr (struct SIM Sim, struct TESR *pTesr, char *entity, char *attribute,
                     char *datastring, struct DATA *pTesrData)
{
  struct DATA TesrDataCell;
  struct DATA *pData = NULL;

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity, attribute);

  neut_data_set_default (&TesrDataCell);

  if (strcmp (entity, "cell"))
    pData = pTesrData;
  else
  {
    TesrDataCell.Qty = (*pTesr).CellQty;
    pData = &TesrDataCell;
  }

  if (!strcmp (entity, "cell"))
    nev_data_tesr_cell (Sim, pTesr, pData, attribute, datastring);

  else if (!strcmp (entity, "vox"))
    nev_data_tesr_vox (Sim, pTesr, pData, attribute, datastring);

  else if (!strcmp (entity, "edge"))
    nev_data_tesr_edge (pData, attribute, datastring);

  else if (!strcmp (entity, "voidvox"))
    nev_data_tesr_voidvox (pData, attribute, datastring);

  else if (!strcmp (entity, "voxedge"))
    nev_data_tesr_voxedge (pData, attribute, datastring);

  else
    abort ();

  if (!strcmp (entity, "cell"))
    nev_data_tesr_cell2vox (*pTesr, attribute, TesrDataCell, pTesrData);

  return;
}
