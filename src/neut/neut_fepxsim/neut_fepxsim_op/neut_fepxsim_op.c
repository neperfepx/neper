/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_fepxsim_op_.h"

void
neut_fepxsim_set_zero (struct FEPXSIM *pFSim)
{
  (*pFSim).fepxdir = NULL;

  (*pFSim).NodeQty = 0;
  (*pFSim).EltQty = 0;
  (*pFSim).PartQty = 0;
  (*pFSim).PartNodeQty = NULL;
  (*pFSim).PartEltQty = NULL;

  (*pFSim).OriDes = NULL;

  (*pFSim).StepQty = 0;
  (*pFSim).StepState = NULL;

  (*pFSim).RestartId = 0;
  (*pFSim).RestartFiles = 0;

  (*pFSim).tess = NULL;
  (*pFSim).tesr = NULL;
  (*pFSim).msh = NULL;
  (*pFSim).bcs = NULL;
  (*pFSim).ori = NULL;
  (*pFSim).phase = NULL;
  (*pFSim).config = NULL;

  return;
}

void
neut_fepxsim_free (struct FEPXSIM *pFSim)
{
  ut_free_1d_char (&(*pFSim).fepxdir);

  ut_free_1d_char (&(*pFSim).OriDes);

  ut_free_1d_char (&(*pFSim).tess);
  ut_free_1d_char (&(*pFSim).tesr);
  ut_free_1d_char (&(*pFSim).msh);
  ut_free_1d_char (&(*pFSim).bcs);
  ut_free_1d_char (&(*pFSim).ori);
  ut_free_1d_char (&(*pFSim).phase);
  ut_free_1d_char (&(*pFSim).config);

  ut_free_1d_int (&(*pFSim).PartNodeQty);
  ut_free_1d_int (&(*pFSim).PartEltQty);

  return;
}
