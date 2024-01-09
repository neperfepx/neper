/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_op_.h"

void
neut_sim_set_zero (struct SIM *pSim)
{
  (*pSim).simdir = NULL;
  (*pSim).StepQty = 0;

  (*pSim).EntityQty = 0;
  (*pSim).Entities = NULL;

  (*pSim).EntityType = NULL;

  (*pSim).EntityMemberQty = NULL;
  (*pSim).EntityMemberExpr = NULL;
  (*pSim).EntityMembers = NULL;

  (*pSim).EntityResQty = NULL;
  (*pSim).EntityRes = NULL;
  (*pSim).EntityResExpr = NULL;

  (*pSim).OriDes = NULL;
  (*pSim).StepState = NULL;
  (*pSim).RestartFiles = 0;
  (*pSim).RestartId = 0;

  (*pSim).OriSpace = NULL;

  (*pSim).tess = NULL;
  (*pSim).tesr = NULL;
  (*pSim).msh = NULL;
  (*pSim).bcs = NULL;
  (*pSim).ori = NULL;
  (*pSim).phase = NULL;
  (*pSim).cfg = NULL;

  (*pSim).CellQty  = 0;

  (*pSim).EltQty = 0;
  (*pSim).NodeQty = 0;
  (*pSim).ElsetQty = 0;
  (*pSim).PartQty = 0;

  (*pSim).step = 0;

  return;
}

void
neut_sim_free (struct SIM *pSim)
{
  int i;

  for (i = 0; i < (*pSim).EntityQty; i++)
  {
    ut_free_2d_char ((*pSim).EntityRes + i, (*pSim).EntityResQty[i]);
    ut_free_2d_char ((*pSim).EntityResExpr + i, (*pSim).EntityResQty[i]);
  }

  ut_free_2d_char (&(*pSim).EntityType, (*pSim).EntityQty);

  for (i = 0; i < (*pSim).EntityQty; i++)
  {
    if ((*pSim).EntityMemberExpr[i])
      ut_free_2d_char ((*pSim).EntityMemberExpr + i, (*pSim).EntityMemberQty[i]);
    if ((*pSim).EntityMembers[i])
      ut_free_2d_int ((*pSim).EntityMembers + i, (*pSim).EntityMemberQty[i]);
  }

  free ((*pSim).EntityMemberExpr);
  free ((*pSim).EntityMembers);

  ut_free_2d_char (&(*pSim).Entities, (*pSim).EntityQty);

  ut_free_1d_int (&(*pSim).EntityMemberQty);

  ut_free_1d_ppchar (&(*pSim).EntityRes);
  ut_free_1d_ppchar (&(*pSim).EntityResExpr);
  ut_free_1d_int (&(*pSim).EntityResQty);

  ut_free_1d_char (&(*pSim).simdir);

  ut_free_1d_char (&(*pSim).OriDes);

  ut_free_1d_char (&(*pSim).OriSpace);

  ut_free_1d_char (&(*pSim).tess);
  ut_free_1d_char (&(*pSim).tesr);
  ut_free_1d_char (&(*pSim).msh);
  ut_free_1d_char (&(*pSim).bcs);
  ut_free_1d_char (&(*pSim).ori);
  ut_free_1d_char (&(*pSim).phase);
  ut_free_1d_char (&(*pSim).cfg);

  return;
}

void
neut_simres_set_zero (struct SIMRES *pSimRes)
{
  (*pSimRes).status  = NULL;
  ut_string_string ("unknown", &(*pSimRes).status);

  (*pSimRes).step = 0;
  (*pSimRes).entity = NULL;
  (*pSimRes).res = NULL;
  (*pSimRes).expr = NULL;
  (*pSimRes).file = NULL;
  (*pSimRes).dir = NULL;
  (*pSimRes).newfile = NULL;

  (*pSimRes).parentres = NULL;
  (*pSimRes).parentcol = 0;
  (*pSimRes).parentfile = NULL;
  (*pSimRes).parentdir = NULL;

  (*pSimRes).type = NULL;
  (*pSimRes).colqty = 0;

  return;
}

void
neut_simres_free (struct SIMRES *pSimRes)
{
  ut_free_1d_char (&(*pSimRes).status);

  ut_free_1d_char (&(*pSimRes).type);

  (*pSimRes).step = 0;
  ut_free_1d_char (&(*pSimRes).entity);
  ut_free_1d_char (&(*pSimRes).res);
  ut_free_1d_char (&(*pSimRes).expr);
  ut_free_1d_char (&(*pSimRes).file);
  ut_free_1d_char (&(*pSimRes).dir);
  ut_free_1d_char (&(*pSimRes).newfile);

  ut_free_1d_char (&(*pSimRes).parentres);
  (*pSimRes).parentcol = 0;
  ut_free_1d_char (&(*pSimRes).parentfile);
  ut_free_1d_char (&(*pSimRes).parentdir);

  (*pSimRes).colqty = 0;

  return;
}

void
neut_sim_addentity (struct SIM *pSim, char *entity_in)
{
  char *entity = NULL, *expr = NULL;

  ut_list_break_2 (entity_in, NEUT_SEP_DEP, &entity, &expr);
  neut_sim_entity_entity (entity, &entity);

  (*pSim).EntityQty++;

  (*pSim).Entities = ut_realloc_1d_pchar_null ((*pSim).Entities, (*pSim).EntityQty, 1);
  ut_string_string (entity, (*pSim).Entities + (*pSim).EntityQty - 1);

  (*pSim).EntityType = ut_realloc_1d_pchar_null ((*pSim).EntityType, (*pSim).EntityQty, 1);
  (*pSim).EntityMemberQty = ut_realloc_1d_int_zero ((*pSim).EntityMemberQty, (*pSim).EntityQty, 1);

  if (!strcmp (entity, "node"))
  {
    ut_string_string ("node", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = (*pSim).NodeQty;
  }
  else if (!strcmp (entity, "elt"))
  {
    ut_string_string ("elt", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = (*pSim).EltQty;
  }
  else if (!strcmp (entity, "elset"))
  {
    ut_string_string ("elset", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = (*pSim).ElsetQty;
  }
  else if (!strcmp (entity, "mesh")) // mesh treated as an elset, internally
  {
    ut_string_string ("elset", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = 1;
  }
  else if (!strcmp (entity, "cell"))
  {
    ut_string_string ("cell", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = (*pSim).CellQty;
  }
  else if (!strcmp (entity, "tess")) // tess treated as cells, internally
  {
    ut_string_string ("cell", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = 1;
  }
  else if (!strcmp (entity, "tesr")) // tesr treated as cells, internally
  {
    ut_string_string ("cell", (*pSim).EntityType + (*pSim).EntityQty - 1);
    (*pSim).EntityMemberQty[(*pSim).EntityQty - 1] = 1;
  }
  else
  {
    ut_string_string ("unknown", (*pSim).EntityType + (*pSim).EntityQty - 1);
  }

  (*pSim).EntityMemberExpr = ut_realloc_1d_ppchar_null ((*pSim).EntityMemberExpr, (*pSim).EntityQty, 1);
  (*pSim).EntityMembers = ut_realloc_1d_ppint_null ((*pSim).EntityMembers, (*pSim).EntityQty, 1);

  (*pSim).EntityResQty = ut_realloc_1d_int_zero ((*pSim).EntityResQty, (*pSim).EntityQty, 1);
  (*pSim).EntityRes = ut_realloc_1d_ppchar_null ((*pSim).EntityRes, (*pSim).EntityQty, 1);
  (*pSim).EntityResExpr = ut_realloc_1d_ppchar_null ((*pSim).EntityResExpr, (*pSim).EntityQty, 1);

  if (expr)
    neut_sim_entity_addexpr (pSim, entity, expr);

  ut_free_1d_char (&entity);
  ut_free_1d_char (&expr);

  return;
}

void
neut_sim_entity_addexpr (struct SIM *pSim, char *entity_in, char *expr)
{
  // special case of file(...): calling the function with its content
  if (!strncmp (expr, "file(", 5))
  {
    char *expr2 = ut_alloc_1d_char (1000);
    FILE *fp = ut_file_open (expr, "R");

    while (fscanf (fp, "%s", expr2) == 1)
      neut_sim_entity_addexpr (pSim, entity_in, expr2);

    ut_file_close (fp, expr, "R");
    ut_free_1d_char (&expr2);
  }

  // general case
  else
  {
    int pos;
    char *entity = NULL;

    neut_sim_entity_entity (entity_in, &entity);
    neut_sim_entity_pos (*pSim, entity, &pos);
    if (pos == -1)
      abort ();

    (*pSim).EntityMemberQty[pos]++;
    (*pSim).EntityMemberExpr[pos] = ut_realloc_1d_pchar_null ((*pSim).EntityMemberExpr[pos],
                                                             (*pSim).EntityMemberQty[pos], 1);
    ut_string_string (expr, (*pSim).EntityMemberExpr[pos] + (*pSim).EntityMemberQty[pos] - 1);
    (*pSim).EntityMembers[pos] = ut_realloc_1d_pint_null ((*pSim).EntityMembers[pos],
                                                      (*pSim).EntityMemberQty[pos], 1);

    ut_free_1d_char (&entity);
  }

  return;
}

void
neut_sim_addres (struct SIM *pSim, char *entity, char *res_in, char *expr_in)
{
  int pos;
  char *res = NULL, *expr = NULL;

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    abort ();

  if (expr_in)
  {
    ut_string_string (res_in, &res);
    ut_string_string (expr_in, &expr);
  }
  else
    ut_list_break_2 (res_in, NEUT_SEP_DEP, &res, &expr);

  (*pSim).EntityResQty[pos]++;
  (*pSim).EntityRes[pos]
    = ut_realloc_1d_pchar_null ((*pSim).EntityRes[pos], (*pSim).EntityResQty[pos], 1);
  ut_string_string (res, (*pSim).EntityRes[pos] + (*pSim).EntityResQty[pos] - 1);

  (*pSim).EntityResExpr[pos]
    = ut_realloc_1d_pchar_null ((*pSim).EntityResExpr[pos], (*pSim).EntityResQty[pos], 1);

  if (expr)
    ut_string_string (expr, (*pSim).EntityResExpr[pos] + (*pSim).EntityResQty[pos] - 1);

  ut_free_1d_char (&res);
  ut_free_1d_char (&expr);

  return;
}

void
neut_sim_rmres (struct SIM *pSim, char *entity, char *res)
{
  int i, j, pos;
  char *rescpy = NULL; // needed in  case where res was among (*pSim), pointerwise

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    abort ();

  ut_string_string (res, &rescpy);

  for (i = 0; i < (*pSim).EntityResQty[pos]; i++)
    if (!strcmp ((*pSim).EntityRes[pos][i], rescpy))
    {
      for (j = i + 1; j < (*pSim).EntityResQty[pos]; j++)
      {
        ut_string_string ((*pSim).EntityRes[pos][j], (*pSim).EntityRes[pos] + j - 1);
        ut_string_string ((*pSim).EntityResExpr[pos][j], (*pSim).EntityResExpr[pos] + j - 1);
      }
      (*pSim).EntityResQty[pos]--;
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
  // if step is valid (>= 0 && <= (*pSim).StepQty) AND
  // ((StepQty > 0 && state) OR (StepQty == 0))
  if (step >= 0 && step <= (*pSim).StepQty
      && (((*pSim).StepQty && (*pSim).StepState[step]) || !(*pSim).StepQty))
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

  neut_mesh_fnscanf_msh (filename, &Nodes, Mesh, Mesh + 1, Mesh + 2, Mesh + 3, Mesh + 4, NULL, "r");
  (*pSim).ElsetQty = Mesh[3].ElsetQty;

  neut_nodes_free (&Nodes);
  for (i = 0; i < 5; i++)
    neut_mesh_free (Mesh + i);

  ut_free_1d_char (&filename);

  return;
}

void
neut_simres_setstep (struct SIMRES *pSimRes, int step)
{
  (*pSimRes).step = step;

  if (strcmp ((*pSimRes).status, "subresult"))
  {
    sprintf ((*pSimRes).file, "%s/%s.step%d", (*pSimRes).dir,
             (*pSimRes).res, (*pSimRes).step);
  }

  else
  {
    sprintf ((*pSimRes).parentfile, "%s/%s.step%d", (*pSimRes).parentdir,
             (*pSimRes).parentres, (*pSimRes).step);

    sprintf ((*pSimRes).file, "%s/%s.step%d,col=%d", (*pSimRes).parentdir,
             (*pSimRes).parentres, (*pSimRes).step, (*pSimRes).parentcol);

    sprintf ((*pSimRes).newfile, "%s/%s.step%d", (*pSimRes).dir,
             (*pSimRes).res, (*pSimRes).step);
  }

  return;
}

void
neut_sim_entity_init_members (struct SIM *pSim, struct TESS *pTess,
                              struct NODES Nodes, struct MESH *Mesh,
                              char *entity)
{
  int i, pos;

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    abort ();

  if (!(*pSim).EntityMemberExpr[pos])
    return;

  (*pSim).EntityMembers[pos] = ut_realloc_1d_pint ((*pSim).EntityMembers[pos],
                                                  (*pSim).EntityMemberQty[pos]);

  for (i = 0; i < (*pSim).EntityMemberQty[pos]; i++)
  {
    if (strstr ((*pSim).EntityMemberExpr[pos][i], "fiber(")
        && !strcmp ((*pTess).CellCrySym, "triclinic"))
      ut_print_message (1, 4, "Computing fiber using `%s' crystal symmetry.\n", (*pTess).CellCrySym);

    int eltqty, *elts = NULL;
    neut_mesh_entity_expr_matches (pTess, Nodes, Mesh, "elt",
                                   (*pSim).EntityMemberExpr[pos][i],
                                   &elts, &eltqty);

    (*pSim).EntityMembers[pos][i] = ut_alloc_1d_int (eltqty + 1);
    (*pSim).EntityMembers[pos][i][0] = eltqty;
    ut_array_1d_int_memcpy (elts, eltqty, (*pSim).EntityMembers[pos][i] + 1);
    ut_free_1d_int (&elts);
  }

  return;
}

void
neut_sim_reset (struct SIM *pSim)
{
  neut_sim_free (pSim);

  neut_sim_set_zero (pSim);

  return;
}

void
neut_sim_entity_init_type (struct SIM *pSim, char *entity)
{
  int pos;

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    abort ();

  if (!strcmp (entity, "node"))
    ut_string_string ("node", (*pSim).EntityType + pos);
  else if (!strcmp (entity, "elt"))
    ut_string_string ("elt", (*pSim).EntityType + pos);
  else if (!strcmp (entity, "elset"))
    ut_string_string ("elset", (*pSim).EntityType + pos);
  else if (!strcmp (entity, "mesh"))
    ut_string_string ("mesh", (*pSim).EntityType + pos);
  else if (!strcmp (entity, "cell"))
    ut_string_string ("cell", (*pSim).EntityType + pos);

  return;
}

void
neut_sim_entity_init_memberqty (struct SIM *pSim, char *entity)
{
  int pos;

  neut_sim_entity_pos (*pSim, entity, &pos);
  if (pos == -1)
    abort ();

  if (!strcmp (entity, "cell"))
    (*pSim).EntityMemberQty[pos] = (*pSim).CellQty;
  else if (!strcmp (entity, "node"))
    (*pSim).EntityMemberQty[pos] = (*pSim).NodeQty;
  else if (!strcmp (entity, "elt"))
    (*pSim).EntityMemberQty[pos] = (*pSim).EltQty;
  else if (!strcmp (entity, "elset"))
    (*pSim).EntityMemberQty[pos] = (*pSim).ElsetQty;
  else if (!strcmp (entity, "mesh"))
    (*pSim).EntityMemberQty[pos] = 1;

  return;
}
