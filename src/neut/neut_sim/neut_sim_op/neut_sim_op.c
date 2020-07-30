/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_op_.h"

void
neut_sim_set_zero (struct SIM *pSim)
{
  (*pSim).fepxdir = NULL;
  (*pSim).simdir = NULL;
  (*pSim).body = NULL;

  (*pSim).msh = NULL;
  (*pSim).tess = NULL;

  (*pSim).StepQty = 0;
  (*pSim).PartQty = 0;
  (*pSim).EltQty = 0;
  (*pSim).NodeQty = 0;
  (*pSim).PartNodeQty = NULL;
  (*pSim).PartEltQty = NULL;
  (*pSim).SlipSystemQty = 0;

  (*pSim).NodeResQty = 0;
  (*pSim).EltResQty = 0;
  (*pSim).NodeRes = NULL;
  (*pSim).EltRes = NULL;

  (*pSim).OriDes = NULL;

  (*pSim).step = 0;

  return;
}

void
neut_sim_free (struct SIM *pSim)
{
  ut_free_1d_char (&(*pSim).fepxdir);
  ut_free_1d_char (&(*pSim).simdir);
  ut_free_1d_char (&(*pSim).body);

  ut_free_1d_char (&(*pSim).msh);
  ut_free_1d_char (&(*pSim).tess);

  ut_free_1d_int (&(*pSim).PartNodeQty);
  ut_free_1d_int (&(*pSim).PartEltQty);

  ut_free_2d_char (&(*pSim).NodeRes, (*pSim).NodeResQty);
  ut_free_2d_char (&(*pSim).EltRes, (*pSim).EltResQty);

  ut_free_1d_char (&(*pSim).OriDes);

  return;
}

void
neut_sim_addres (struct SIM *pSim, char *entity, char *res, int written)
{
  if (!strncmp (entity, "node", 4))
    neut_sim_addnoderes (pSim, res, written);

  else if (!strncmp (entity, "element", 7)
        || !strncmp (entity, "elt", 3))
    neut_sim_addeltres (pSim, res, written);

  else
    abort ();

  return;
}

void
neut_sim_addnoderes (struct SIM *pSim, char *res, int written)
{
  (*pSim).NodeResQty++;

  (*pSim).NodeRes = ut_realloc_1d_pchar ((*pSim).NodeRes, (*pSim).NodeResQty);
  (*pSim).NodeRes[(*pSim).NodeResQty - 1] = NULL;

  (*pSim).NodeResWritten = ut_realloc_1d_int ((*pSim).NodeResWritten, (*pSim).NodeResQty);
  (*pSim).NodeResWritten[(*pSim).NodeResQty - 1] = written;

  ut_string_string (res, (*pSim).NodeRes + (*pSim).NodeResQty - 1);

  return;
}

void
neut_sim_addeltres (struct SIM *pSim, char *res, int written)
{
  (*pSim).EltResQty++;

  (*pSim).EltRes = ut_realloc_1d_pchar ((*pSim).EltRes, (*pSim).EltResQty);
  (*pSim).EltRes[(*pSim).EltResQty - 1] = NULL;

  (*pSim).EltResWritten = ut_realloc_1d_int ((*pSim).EltResWritten, (*pSim).EltResQty);
  (*pSim).EltResWritten[(*pSim).EltResQty - 1] = written;

  ut_string_string (res, (*pSim).EltRes + (*pSim).EltResQty - 1);

  return;
}

void
neut_sim_rmres (struct SIM *pSim, char *entity, char *res)
{
  if (!strncmp (entity, "node", 4))
    neut_sim_rmnoderes (pSim, res);

  else if (!strncmp (entity, "element", 7)
        || !strncmp (entity, "elt", 3))
    neut_sim_rmeltres (pSim, res);

  else
    abort ();

  return;
}

void
neut_sim_rmnoderes (struct SIM *pSim, char *res)
{
  int i, j;
  char *rescpy = NULL; // needed in  case where res was among (*pSim), pointerwise

  ut_string_string (res, &rescpy);

  for (i = 0; i < (*pSim).NodeResQty; i++)
    if (!strcmp ((*pSim).NodeRes[i], rescpy))
    {
      for (j = i + 1; j < (*pSim).NodeResQty; j++)
        ut_string_string ((*pSim).NodeRes[j], (*pSim).NodeRes + j - 1);
      (*pSim).NodeResQty--;
      i--;
    }

  ut_free_1d_char (&rescpy);

  return;
}

void
neut_sim_rmeltres (struct SIM *pSim, char *res)
{
  int i, j;
  char *rescpy = NULL; // needed in  case where res was among (*pSim), pointerwise

  ut_string_string (res, &rescpy);

  for (i = 0; i < (*pSim).EltResQty; i++)
    if (!strcmp ((*pSim).EltRes[i], rescpy))
    {
      for (j = i + 1; j < (*pSim).EltResQty; j++)
        ut_string_string ((*pSim).EltRes[j], (*pSim).EltRes + j - 1);
      (*pSim).EltResQty--;
      i--;
    }

  ut_free_1d_char (&rescpy);

  return;
}

int
neut_sim_testres (struct SIM Sim, char *entity, char *res)
{
  int i;

  if (neut_sim_entityisnode (entity))
  {
    for (i = 0; i < Sim.NodeResQty; i++)
      if (!strcmp (Sim.NodeRes[i], res))
        return 1;
  }

  else if (neut_sim_entityiselt (entity))
  {
    for (i = 0; i < Sim.EltResQty; i++)
      if (!strcmp (Sim.EltRes[i], res))
        return 1;
  }

  return 0;
}

int
neut_sim_updatenodes (struct SIM Sim, int step, struct NODES *pNodes)
{
  int status;
  FILE *file = NULL;
  char *filename = ut_alloc_1d_char (1000);

  sprintf (filename, "%s/results/nodes/coo/coo.step%d", Sim.simdir, step);

  if (!ut_file_exist (filename))
    status = -1;

  else
  {
    file = ut_file_open (filename, "R");
    ut_array_2d_fscanf (file, (*pNodes).NodeCoo + 1, (*pNodes).NodeQty, 3);
    ut_file_close (file, filename, "R");
    status = 0;
  }

  ut_free_1d_char (&filename);

  return status;
}
