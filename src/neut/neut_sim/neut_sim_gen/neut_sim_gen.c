/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_gen_.h"

int
neut_sim_name_type (char *name, char **ptype)
{
  if (ut_file_exist ("%s/post.report", name))
    ut_string_string ("fepx", ptype);
  else if (ut_file_exist ("%s/report", name))
    ut_string_string ("sim", ptype);
  else
    return -1;

  return 0;
}

int
neut_sim_isvoid (struct SIM Sim)
{
  return (Sim.simdir == NULL);
}

void
neut_sim_fscanf (char *dir, struct SIM *pSim)
{
  int i, status;
  char *filename = NULL, *var = ut_alloc_1d_char (100), *type = NULL;
  FILE *file = NULL;

  status = neut_sim_name_type (dir, &type);

  if (status)
    ut_print_message (2, 2, "Failed to parse directory.\n");

  if (!strcmp (type, "fepx"))
  {
    filename = ut_string_paste (dir, "/post.report");
    ut_string_string (dir, &(*pSim).fepxdir);
  }
  else if (!strcmp (type, "sim"))
  {
    filename = ut_string_paste (dir, "/report");
    ut_string_string (dir, &(*pSim).simdir);
  }
  else
    abort ();

  ut_string_string ("simulation.tess", &(*pSim).tess);
  ut_string_string ("simulation.msh", &(*pSim).msh);

  file = ut_file_open (filename, "r");

  while (fscanf (file, "%s", var) != EOF)
  {
    if (!strcmp (var, "number_of_steps"))
    {
      if (fscanf (file, "%d", &(*pSim).StepQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_partitions"))
    {
      if (fscanf (file, "%d", &(*pSim).PartQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_nodes"))
    {
      if (fscanf (file, "%d", &(*pSim).NodeQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_elements"))
    {
      if (fscanf (file, "%d", &(*pSim).EltQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_elements_bypartition"))
    {
      (*pSim).PartEltQty = ut_alloc_1d_int ((*pSim).PartQty + 1);
      if (ut_array_1d_int_fscanf (file, (*pSim).PartEltQty + 1, (*pSim).PartQty) != 1)
        abort ();
    }
    else if (!strcmp (var, "number_of_nodes_bypartition"))
    {
      (*pSim).PartNodeQty = ut_alloc_1d_int ((*pSim).PartQty + 1);
      if (ut_array_1d_int_fscanf (file, (*pSim).PartNodeQty + 1, (*pSim).PartQty) != 1)
          abort ();
    }
    else if (!strcmp (var, "orientation_definition"))
    {
      (*pSim).OriDes = ut_alloc_1d_char (100);;

      if (fscanf (file, "%s", (*pSim).OriDes) != 1)
        abort ();
    }
    else if (!strcmp (var, "results_nodes"))
    {
      ut_file_nextlinenbwords (file, &(*pSim).NodeResQty);
      (*pSim).NodeRes = ut_alloc_2d_char ((*pSim).NodeResQty, 20);
      for (i = 0; i < (*pSim).NodeResQty; i++)
        if (fscanf (file, "%s", (*pSim).NodeRes[i]) != 1)
          abort ();

      (*pSim).NodeResWritten = ut_alloc_1d_int ((*pSim).NodeResQty);
      if (!strcmp (type, "sim"))
        ut_array_1d_int_set ((*pSim).NodeResWritten, (*pSim).NodeResQty, 1);
    }
    else if (!strcmp (var, "results_elements"))
    {
      ut_file_nextlinenbwords (file, &(*pSim).EltResQty);
      (*pSim).EltRes = ut_alloc_2d_char ((*pSim).EltResQty, 20);
      for (i = 0; i < (*pSim).EltResQty; i++)
        if (fscanf (file, "%s", (*pSim).EltRes[i]) != 1)
          abort ();

      (*pSim).EltResWritten = ut_alloc_1d_int ((*pSim).EltResQty);
      if (!strcmp (type, "sim"))
        ut_array_1d_int_set ((*pSim).EltResWritten, (*pSim).EltResQty, 1);
    }
    else if (!strcmp (var, "number_of_slip_systems"))
    {
      if (fscanf (file, "%d", &(*pSim).SlipSystemQty) != 1)
        abort ();
    }
    else
    {
      printf ("var = %s\n", var);
      ut_print_message (2, 3, "Failed to read file.\n");
    }
  }

  if (!(*pSim).EltQty)
    ut_print_message (2, 3, "`number_of_elements' is missing.\n");
  if (!(*pSim).NodeQty)
    ut_print_message (2, 3, "`number_of_nodes' is missing.\n");

  ut_file_close (file, filename, "r");

  ut_free_1d_char (&var);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&type);

  return;
}

void
neut_sim_fprintf (char *dir, struct SIM Sim, char *mode)
{
  int i;
  char *filename = ut_string_paste (dir, "/report");
  FILE *file = ut_file_open (filename, mode);

  fprintf (file, "number_of_nodes %d\n", Sim.NodeQty);
  fprintf (file, "number_of_elements %d\n", Sim.EltQty);
  fprintf (file, "number_of_slip_systems %d\n", Sim.SlipSystemQty);

  fprintf (file, "orientation_definition %s\n", Sim.OriDes);

  fprintf (file, "results_nodes");
  for (i = 0; i < Sim.NodeResQty; i++)
    if (Sim.NodeResWritten[i])
      fprintf (file, " %s", Sim.NodeRes[i]);
  fprintf (file, "\n");

  fprintf (file, "results_elements");
  for (i = 0; i < Sim.EltResQty; i++)
    if (Sim.EltResWritten[i])
      fprintf (file, " %s", Sim.EltRes[i]);
  fprintf (file, "\n");

  fprintf (file, "number_of_steps %d\n", Sim.StepQty);

  ut_file_close (file, filename, mode);
  ut_free_1d_char (&filename);

  return;
}

int
neut_sim_knownres_type (struct SIM Sim, char *res, char **ptype, int *pcolqty)
{
  if (!strcmp (res, "coo"))
  {
    ut_string_string ("vector", ptype);
    *pcolqty = 3;
  }

  else if (!strcmp (res, "vel"))
  {
    ut_string_string ("vector", ptype);
    *pcolqty = 3;
  }

  else if (!strcmp (res, "crss"))
  {
    ut_string_string ("vector", ptype);
    *pcolqty = 3;
  }

  else if (!strcmp (res, "slip"))
  {
    ut_string_string ("vector", ptype);
    *pcolqty = Sim.SlipSystemQty;
  }

  else if (!strcmp (res, "sliprate"))
  {
    ut_string_string ("vector", ptype);
    *pcolqty = Sim.SlipSystemQty;
  }

  else if (!strcmp (res, "spinrate"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 3;
  }

  else if (!strcmp (res, "velgrad"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 9;
  }

  else if (!strcmp (res, "strain"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 6;
  }

  else if (!strcmp (res, "strain-el"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 6;
  }

  else if (!strcmp (res, "strain-pl"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 6;
  }

  else if (!strcmp (res, "stress"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 6;
  }

  else if (!strcmp (res, "ori"))
  {
    ut_string_string ("ori", ptype);
    *pcolqty = 0;
  }

  else if (!strncmp (res, "work", 4))
  {
    ut_string_string ("real", ptype);
    *pcolqty = 1;
  }

  else if (!strcmp (res, "stress"))
  {
    ut_string_string ("tensor", ptype);
    *pcolqty = 6;
  }

  else  if ((strlen (res) > 0 && isdigit (res[strlen (res) - 1])) || strstr (res, "-eq"))
  {
    ut_string_string ("real", ptype);
    *pcolqty = 1;
  }

  else
    return -1;

  return 0;
}

int
neut_sim_res_type (struct SIM Sim, char *entity, char *res, char **ptype, int *pcolqty)
{
  int status = 0;

  *pcolqty = 0;

  if (!neut_sim_knownres_type (Sim, res, ptype, pcolqty))
    return 0;

  else
  {
    char *simfile = ut_alloc_1d_char (1000);
    char *tmp = ut_alloc_1d_char (1000);
    FILE *file = NULL;

    if (!neut_sim_file_simfile (Sim, entity, res, simfile))
    {
      *pcolqty = ut_file_nbcolumns (simfile);
      if (*pcolqty == 1)
      {
        file = ut_file_open (simfile, "R");
        ut_file_nextstring (file, tmp);
        ut_string_string (strstr (tmp, ".") ? "real" : "int", ptype);
        ut_file_close (file, simfile, "R");
      }
      else if (*pcolqty == 6 || *pcolqty == 9)
        ut_string_string ("tensor", ptype);
      else
        ut_string_string ("vector", ptype);
    }
    else
      status = -1;

    ut_free_1d_char (&simfile);
    ut_free_1d_char (&tmp);
  }

  return status;
}

int
neut_sim_res_rescol (struct SIM Sim, char *entity, char *res, char **pres, int *pcol)
{
  int colqty;
  char *type = NULL, *comp = ut_alloc_1d_char (3);

  *pres = ut_alloc_1d_char (strlen (res) + 1);

  *pcol = INT_MAX;

  if (strlen (res) >= 2 && isdigit (res[strlen (res) - 2]) && isdigit (res[strlen (res) - 1]))
  {
    sscanf (res + strlen (res) - 2, "%s", comp);
    *pres = strncpy (*pres, res, strlen (res) - 2);
  }
  else if (strlen (res) >= 1 && isdigit (res[strlen (res) - 1]))
  {
    sscanf (res + strlen (res) - 1, "%s", comp);
    *pres = strncpy (*pres, res, strlen (res) - 1);
  }
  else
    *pres = strcpy (*pres, res);

  neut_sim_res_type (Sim, entity, *pres, &type, &colqty);

  if (strlen (comp))
  {
    if (!strcmp (type, "tensor") && strlen (comp) != 2)
      ut_print_exprbug (res);

    if (!strcmp (type, "vector"))
      *pcol = atoi (comp);
    else if (!strcmp (type, "tensor"))
    {
      if (colqty == 3)
      {
        if (!strcmp (comp, "11"))
          *pcol = 0;
        else if (!strcmp (comp, "12"))
          *pcol = 1;
        else if (!strcmp (comp, "13"))
          *pcol = 2;
        else if (!strcmp (comp, "21"))
          *pcol = -1;
        else if (!strcmp (comp, "22"))
          *pcol = 0;
        else if (!strcmp (comp, "23"))
          *pcol = 3;
        else if (!strcmp (comp, "31"))
          *pcol = -2;
        else if (!strcmp (comp, "32"))
          *pcol = -3;
        else if (!strcmp (comp, "33"))
          *pcol = 0;
        else
          ut_print_exprbug (res);
      }
      else if (colqty == 6)
      {
        if (!strcmp (comp, "11"))
          *pcol = 1;
        else if (!strcmp (comp, "22"))
          *pcol = 2;
        else if (!strcmp (comp, "33"))
          *pcol = 3;
        else if (!strcmp (comp, "23") || !strcmp (comp, "32"))
          *pcol = 4;
        else if (!strcmp (comp, "31") || !strcmp (comp, "13"))
          *pcol = 5;
        else if (!strcmp (comp, "12") || !strcmp (comp, "21"))
          *pcol = 6;
        else
          ut_print_exprbug (res);
      }
      else if (colqty == 9)
      {
        if (!strcmp (comp, "11"))
          *pcol = 1;
        else if (!strcmp (comp, "12"))
          *pcol = 2;
        else if (!strcmp (comp, "13"))
          *pcol = 3;
        else if (!strcmp (comp, "21"))
          *pcol = 4;
        else if (!strcmp (comp, "22"))
          *pcol = 5;
        else if (!strcmp (comp, "23"))
          *pcol = 6;
        else if (!strcmp (comp, "31"))
          *pcol = 7;
        else if (!strcmp (comp, "32"))
          *pcol = 8;
        else if (!strcmp (comp, "33"))
          *pcol = 9;
        else
          ut_print_exprbug (res);
      }
      else
        ut_print_exprbug (res);
    }
  }

  ut_free_1d_char (&comp);
  ut_free_1d_char (&type);

  return *pcol != INT_MAX ? 0 : -1;
}

int
neut_sim_file_simfile (struct SIM Sim, char *entity_in, char *file, char *simfile)
{
  int status = -1;
  char *entity = ut_alloc_1d_char (100);

  entity = strcpy (entity, entity_in);
  ut_string_fnrs (entity, "elt3d", "element", 1);

  if (ut_file_exist (file))
  {
    sprintf (simfile, "%s", file);
    status = 0;
  }

  else if (!neut_sim_isvoid (Sim) && neut_sim_res_exist (Sim, entity, file))
  {
    int status2, col;
    char *res = ut_alloc_1d_char (1000);

    status2 = neut_sim_res_rescol (Sim, entity, file, &res, &col);

    sprintf (simfile, "%s/results/%ss/%s/%s.step%d", Sim.simdir, entity, res, res, Sim.step);

    if (ut_file_exist (simfile))
    {
      status = 0;

      if (!status2)
        sprintf (simfile, "%s,col=%d", simfile, col);
    }

    ut_free_1d_char (&res);
  }

  else
  {
    sprintf (simfile, "%s", file);
    status = -1;
  }

  ut_free_1d_char (&entity);

  return status;
}

int
neut_sim_res_exist (struct SIM Sim, char *entity, char *res)
{
  int i, status;
  char *entity2 = ut_alloc_1d_char (100);
  char *res2 = NULL;

  ut_string_string (res, &res2);
  for (i = strlen (res2) - 1; i >= 0; i--)
    if (isdigit (res2[i]))
      res2[i] = '\0';

  if (entity)
  {
    entity2 = strcpy (entity2, entity);
    ut_string_fnrs (entity2, "elt3d", "element", 1);
  }

  status = 0;

  if (!strcmp (entity2, "element"))
  {
    for (i = 0; i < Sim.EltResQty; i++)
      if (!strcmp (Sim.EltRes[i], res2))
        status = 1;
  }

  else if (!strncmp (entity2, "node", 4))
  {
    for (i = 0; i < Sim.NodeResQty; i++)
      if (!strcmp (Sim.NodeRes[i], res2))
        status = 1;
  }

  else
    status = 0;

  ut_free_1d_char (&entity2);
  ut_free_1d_char (&res2);

  return status;
}

int
neut_sim_entityisnode (char *entity)
{
  return !strncmp (entity, "node", 4);
}

int
neut_sim_entityiselt (char *entity)
{
  return !strncmp (entity, "element", 7) || !strncmp (entity, "elt", 3);
}

int
neut_sim_entityiselset (char *entity)
{
  return !strncmp (entity, "elset", 5);
}

int
neut_sim_entityismesh (char *entity)
{
  return !strcmp (entity, "mesh");
}

void
neut_sim_entity_dir (char *entity, char **pdir)
{
  if (neut_sim_entityisnode (entity))
    ut_string_string ("results/nodes", pdir);
  else if (neut_sim_entityiselt (entity))
    ut_string_string ("results/elements", pdir);
  else if (neut_sim_entityiselset (entity))
    ut_string_string ("results/elsets", pdir);
  else if (neut_sim_entityismesh (entity))
    ut_string_string ("results/mesh", pdir);
  else
    abort ();

  return;
}
