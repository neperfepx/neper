/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_data_nodes_.h"

void
nev_print_data_nodes (struct SIM Sim, struct TESS *pTess, struct NODES *pNodes,
                struct MESH **pMesh, char *attribute, char *datastring,
                struct DATA *pData)
{
  char *datatype = NULL, *datavalue = NULL;

  struct DATAINPUT DataInput;
  neut_datainput_set_default (&DataInput);
  ut_string_string ("nodes", &DataInput.input);
  DataInput.pSim = &Sim;
  DataInput.pTess = pTess;
  DataInput.pNodes = pNodes;
  DataInput.pMesh = pMesh;

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", "node", attribute);

  neut_data_datastring_type_value ("nodes", attribute, datastring, &datatype, &datavalue);

  if (!strcmp (attribute, "colscheme"))
    ut_string_string (datastring, &(*pData).ColScheme);

  else if (!strcmp (attribute, "scale"))
    ut_string_string (datastring, &(*pData).Scale);

  else if (!strcmp (attribute, "scaletitle"))
    ut_string_string (datastring, &(*pData).ScaleTitle);

  else if (!strcmp (attribute, "coofact"))
    (*pData).CooFact = atof (datavalue);

  else if (!strcmp (attribute, "col") || !strcmp (attribute, "rad"))
    neut_data_fscanf_general (DataInput, "node", 0,
                         (*pNodes).NodeQty, attribute, datatype, datavalue, pData);

  else if (!strcmp (attribute, "coo"))
    neut_data_fscanf_coo (Sim, "node", (*pNodes).NodeQty, datatype, datavalue, pData);

  else
    ut_print_exprbug (attribute);

  ut_free_1d_char (&datatype);
  ut_free_1d_char (&datavalue);

  return;
}
