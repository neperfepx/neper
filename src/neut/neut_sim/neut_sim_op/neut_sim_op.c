/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_op_.h"

void
neut_sim_set_zero (struct SIM *pSim)
{
  (*pSim).simdir = NULL;
  (*pSim).StepQty = 0;
  (*pSim).NodeResQty = 0;
  (*pSim).EltResQty = 0;
  (*pSim).ElsetResQty = 0;
  (*pSim).NodeRes = NULL;
  (*pSim).EltRes = NULL;
  (*pSim).ElsetRes = NULL;
  (*pSim).NodeResExpr = NULL;
  (*pSim).EltResExpr = NULL;
  (*pSim).ElsetResExpr = NULL;
  (*pSim).NodeResWritten = NULL;
  (*pSim).EltResWritten = NULL;
  (*pSim).ElsetResWritten = NULL;
  (*pSim).OriDes = NULL;
  (*pSim).StepState = NULL;
  (*pSim).RestartFiles = 0;
  (*pSim).RestartId = 0;

  (*pSim).body = NULL;
  (*pSim).tess = NULL;
  (*pSim).tesr = NULL;
  (*pSim).msh = NULL;
  (*pSim).bcs = NULL;
  (*pSim).ori = NULL;
  (*pSim).phase = NULL;

  (*pSim).fepxdir = NULL;
  (*pSim).EltQty = 0;
  (*pSim).NodeQty = 0;
  (*pSim).ElsetQty = 0;
  (*pSim).GroupQty = 0;
  (*pSim).PartQty = 0;
  (*pSim).PartNodeQty = NULL;
  (*pSim).PartEltQty = NULL;
  (*pSim).SlipSystemQty = NULL;

  (*pSim).step = 0;

  return;
}

void
neut_sim_free (struct SIM *pSim)
{
  ut_free_1d_char (&(*pSim).simdir);
  ut_free_2d_char (&(*pSim).NodeRes, (*pSim).NodeResQty);
  ut_free_2d_char (&(*pSim).EltRes, (*pSim).EltResQty);
  ut_free_2d_char (&(*pSim).ElsetRes, (*pSim).ElsetResQty);
  ut_free_2d_char (&(*pSim).NodeResExpr, (*pSim).NodeResQty);
  ut_free_2d_char (&(*pSim).EltResExpr, (*pSim).EltResQty);
  ut_free_2d_char (&(*pSim).ElsetResExpr, (*pSim).ElsetResQty);
  ut_free_1d_int (&(*pSim).NodeResWritten);
  ut_free_1d_int (&(*pSim).EltResWritten);
  ut_free_1d_int (&(*pSim).ElsetResWritten);
  ut_free_1d_char (&(*pSim).OriDes);

  ut_free_1d_char (&(*pSim).body);
  ut_free_1d_char (&(*pSim).tess);
  ut_free_1d_char (&(*pSim).tesr);
  ut_free_1d_char (&(*pSim).msh);
  ut_free_1d_char (&(*pSim).bcs);
  ut_free_1d_char (&(*pSim).ori);
  ut_free_1d_char (&(*pSim).phase);

  ut_free_1d_char (&(*pSim).fepxdir);
  ut_free_1d_int (&(*pSim).PartNodeQty);
  ut_free_1d_int (&(*pSim).PartEltQty);
  ut_free_1d_int (&(*pSim).SlipSystemQty);

  return;
}

void
neut_sim_addres (struct SIM *pSim, char *entity, char *res, char *expr, int written)
{
  if (!strncmp (entity, "node", 4))
    neut_sim_addnoderes (pSim, res, expr, written);

  else if (!strncmp (entity, "element", 7)
        || !strncmp (entity, "elt", 3))
    neut_sim_addeltres (pSim, res, expr, written);

  else if (!strcmp (entity, "elset"))
    neut_sim_addelsetres (pSim, res, expr, written);

  else
    abort ();

  return;
}

void
neut_sim_addnoderes (struct SIM *pSim, char *res, char *expr, int written)
{
  (*pSim).NodeResQty++;

  (*pSim).NodeRes = ut_realloc_1d_pchar ((*pSim).NodeRes, (*pSim).NodeResQty);
  (*pSim).NodeRes[(*pSim).NodeResQty - 1] = NULL;

  ut_string_string (res, (*pSim).NodeRes + (*pSim).NodeResQty - 1);

  (*pSim).NodeResWritten = ut_realloc_1d_int ((*pSim).NodeResWritten, (*pSim).NodeResQty);
  (*pSim).NodeResWritten[(*pSim).NodeResQty - 1] = written;

  (*pSim).NodeResExpr = ut_realloc_1d_pchar ((*pSim).NodeResExpr, (*pSim).NodeResQty);
  (*pSim).NodeResExpr[(*pSim).NodeResQty - 1] = NULL;
  if (expr)
    ut_string_string (expr, (*pSim).NodeResExpr + (*pSim).NodeResQty - 1);

  return;
}

void
neut_sim_addeltres (struct SIM *pSim, char *res, char *expr, int written)
{
  (*pSim).EltResQty++;

  (*pSim).EltRes = ut_realloc_1d_pchar ((*pSim).EltRes, (*pSim).EltResQty);
  (*pSim).EltRes[(*pSim).EltResQty - 1] = NULL;

  ut_string_string (res, (*pSim).EltRes + (*pSim).EltResQty - 1);

  (*pSim).EltResWritten = ut_realloc_1d_int ((*pSim).EltResWritten, (*pSim).EltResQty);
  (*pSim).EltResWritten[(*pSim).EltResQty - 1] = written;

  (*pSim).EltResExpr = ut_realloc_1d_pchar ((*pSim).EltResExpr, (*pSim).EltResQty);
  (*pSim).EltResExpr[(*pSim).EltResQty - 1] = NULL;
  if (expr)
    ut_string_string (expr, (*pSim).EltResExpr + (*pSim).EltResQty - 1);

  return;
}

void
neut_sim_addelsetres (struct SIM *pSim, char *res, char *expr, int written)
{
  (*pSim).ElsetResQty++;

  (*pSim).ElsetRes = ut_realloc_1d_pchar ((*pSim).ElsetRes, (*pSim).ElsetResQty);
  (*pSim).ElsetRes[(*pSim).ElsetResQty - 1] = NULL;

  ut_string_string (res, (*pSim).ElsetRes + (*pSim).ElsetResQty - 1);

  (*pSim).ElsetResWritten = ut_realloc_1d_int ((*pSim).ElsetResWritten, (*pSim).ElsetResQty);
  (*pSim).ElsetResWritten[(*pSim).ElsetResQty - 1] = written;

  (*pSim).ElsetResExpr = ut_realloc_1d_pchar ((*pSim).ElsetResExpr, (*pSim).ElsetResQty);
  (*pSim).ElsetResExpr[(*pSim).ElsetResQty - 1] = NULL;
  if (expr)
    ut_string_string (expr, (*pSim).ElsetResExpr + (*pSim).ElsetResQty - 1);

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

  else if (!strcmp (entity, "elset"))
    neut_sim_rmelsetres (pSim, res);

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
      {
        ut_string_string ((*pSim).NodeRes[j], (*pSim).NodeRes + j - 1);
        ut_string_string ((*pSim).NodeResExpr[j], (*pSim).NodeResExpr + j - 1);
      }
      for (j = i + 1; j < (*pSim).NodeResQty; j++)
        (*pSim).NodeResWritten[j - 1] = (*pSim).NodeResWritten[j];
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
      {
        ut_string_string ((*pSim).EltRes[j], (*pSim).EltRes + j - 1);
        ut_string_string ((*pSim).EltResExpr[j], (*pSim).EltResExpr + j - 1);
      }
      for (j = i + 1; j < (*pSim).EltResQty; j++)
        (*pSim).EltResWritten[j - 1] = (*pSim).EltResWritten[j];
      (*pSim).EltResQty--;
      i--;
    }

  ut_free_1d_char (&rescpy);

  return;
}

void
neut_sim_rmelsetres (struct SIM *pSim, char *res)
{
  int i, j;
  char *rescpy = NULL; // needed in  case where res was among (*pSim), pointerwise

  ut_string_string (res, &rescpy);

  for (i = 0; i < (*pSim).ElsetResQty; i++)
    if (!strcmp ((*pSim).ElsetRes[i], rescpy))
    {
      for (j = i + 1; j < (*pSim).ElsetResQty; j++)
      {
        ut_string_string ((*pSim).ElsetRes[j], (*pSim).ElsetRes + j - 1);
        ut_string_string ((*pSim).ElsetResExpr[j], (*pSim).ElsetResExpr + j - 1);
      }
      for (j = i + 1; j < (*pSim).ElsetResQty; j++)
        (*pSim).ElsetResWritten[j - 1] = (*pSim).ElsetResWritten[j];
      (*pSim).ElsetResQty--;
      i--;
    }

  ut_free_1d_char (&rescpy);

  return;
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

int
neut_sim_setstep (struct SIM *pSim, int step)
{
  if (step >= 0 && step <= (*pSim).StepQty && !(*pSim).StepState[step])
  {
    (*pSim).step = step;
    return 0;
  }
  else
    return -1;
}

void
neut_sim_init_elsetqty (struct SIM *pSim)
{
  int i;
  char *filename = ut_string_paste3 ((*pSim).simdir, "/inputs/", (*pSim).msh);
  struct NODES Nodes;
  struct MESH *Mesh = calloc (5, sizeof (struct MESH));

  neut_nodes_set_zero (&Nodes);
  for (i = 0; i < 5; i++)
    neut_mesh_set_zero (Mesh + i);

  neut_mesh_fnscanf_msh (filename, &Nodes, Mesh, Mesh + 1, Mesh + 2, Mesh + 3, Mesh + 4, NULL);
  (*pSim).ElsetQty = Mesh[3].ElsetQty;

  neut_nodes_free (&Nodes);
  for (i = 0; i < 5; i++)
    neut_mesh_free (Mesh + i);

  ut_free_1d_char (&filename);

  return;
}
