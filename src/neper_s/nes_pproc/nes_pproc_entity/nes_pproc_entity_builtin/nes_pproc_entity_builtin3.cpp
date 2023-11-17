/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"
#include "neut_oset/neut_oset.hpp"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin_nodes_disp (struct SIM *pSim, struct NODES Nodes,
                                     char *dir, char *res,
                                     struct SIMRES *pSimRes)
{
  int step;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double ***coo = ut_alloc_3d (2, Nodes.NodeQty, 3);
  double **disp = ut_alloc_2d (Nodes.NodeQty, 3);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  neut_sim_setstep (pSim, 0);

  ut_array_2d_fnscanf ((*pSimRes).file, coo[0], Nodes.NodeQty, 3, (char *) "R");

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    if (!ut_file_exist ((*pSimRes).file))
      abort ();

    ut_array_2d_fnscanf ((*pSimRes).file, coo[1], Nodes.NodeQty, 3, (char *) "R");

    ut_array_2d_sub (coo[0], coo[1], Nodes.NodeQty, 3, disp);

    sprintf (filename, (char *) "%s/%s.step%d", dir, res, step);

    file = ut_file_open (filename, (char *) "W");
    ut_array_2d_fprintf (file, disp, Nodes.NodeQty, 3, REAL_PRINT_FORMAT);
    ut_file_close (file, filename, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  ut_free_3d (&coo, 2, Nodes.NodeQty);
  ut_free_2d (&disp, Nodes.NodeQty);

  neut_sim_addres (pSim, (char *) "node", res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);

  return 0;
}

void
nes_pproc_entity_builtin_elsets_pre (struct SIM *pSim,
                                  struct MESH *Mesh,
                                  char *entity, char *res,
                                  struct SIMRES *pSimRes,
                                  int ***pelsets, int *pelsetqty)
{
  int i, pos;
  char *type = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  neut_sim_entity_pos (*pSim, entity, &pos);
  neut_sim_entity_type (*pSim, entity, &type);

  neut_simres_set_zero (pSimRes);

  neut_sim_simres (*pSim, entity, res, pSimRes);

  *pelsetqty = (*pSim).EntityMemberQty[pos];
  (*pelsets) = ut_alloc_1d_pint (*pelsetqty + 1);

  if (!strcmp (entity, (char *) "elset"))
    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i] = ut_alloc_1d_int (Mesh[dim].Elsets[i][0] + 1);
      ut_array_1d_int_memcpy (Mesh[dim].Elsets[i], Mesh[dim].Elsets[i][0] + 1, (*pelsets)[i]);
    }

  else if (!strcmp (entity, (char *) "elt"))
  {
    *pelsetqty = Mesh[dim].EltQty;
    *pelsets = ut_alloc_2d_int (*pelsetqty + 1, 2);

    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i][0] = 1;
      (*pelsets)[i][1] = i;
    }
  }

  else if (!strcmp (entity, (char *) "mesh"))
    neut_mesh_aselsets (Mesh[dim], pelsets, pelsetqty);

  else if (neut_sim_entity_hasexpr (*pSim, entity))
    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i] = ut_alloc_1d_int ((*pSim).EntityMembers[pos][i - 1][0] + 1);
      ut_array_1d_int_memcpy ((*pSim).EntityMembers[pos][i - 1],
                              (*pSim).EntityMembers[pos][i - 1][0] + 1, (*pelsets)[i]);
    }

  else
    abort ();

  ut_free_1d_char (&type);

  return;
}

void
nes_pproc_entity_builtin_elsets_ori (struct SIM *pSim,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double **eltdata = NULL;
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  struct SIMRES SimRes2;
  int dim = neut_mesh_array_dim (Mesh);

  eltdata = ut_alloc_2d (Mesh[dim].EltQty + 1, 4);

  neut_simres_set_zero (&SimRes2);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    if (!strcmp (entity, (char *) "elt"))
    {
      if (Mesh[dim].EltOri)
        ut_array_2d_memcpy (Mesh[dim].EltOri + 1, Mesh[dim].EltQty, 4, eltdata);
      else
      {
        for (i = 1; i <= Mesh[dim].EltQty; i++)
          ol_q_memcpy (Mesh[dim].ElsetOri[Mesh[dim].EltElset[i]], eltdata[i]);
      }

      neut_ori_fprintf (file, (*pSim).OriDes, (char *) "ascii", eltdata + 1, NULL, NULL, Mesh[dim].EltQty, NULL);
    }

    else if (!strcmp (entity, (char *) "elset"))
    {
      neut_sim_simres (*pSim, (char *) "elts", res, &SimRes2);

      neut_simres_setstep (&SimRes2, step);

      if (!SimRes2.file)
        abort ();

      if (!Mesh[dim].ElsetCrySym)
        ut_print_message (2, 5, (char *) "\nCrystal symmetry not defined.\n");

      if (Mesh[dim].SimEltOri)
        ut_free_2d (&(Mesh[dim].SimEltOri), Mesh[dim].EltQty + 1);
      Mesh[dim].SimEltOri = ut_alloc_2d (Mesh[dim].EltQty + 1, 4);

      neut_ori_fnscanf (SimRes2.file, (*pSim).OriDes, (char *) "ascii",
                        Mesh[dim].SimEltOri + 1, NULL,
                        Mesh[dim].EltQty, NULL, (char *) "R");
      neut_mesh_eltdata_elsetdata_ori (*pNodes, Mesh[dim], elsets, elsetqty,
                                       elsetdata);

      neut_ori_fprintf (file, (*pSim).OriDes, (char *) "ascii", elsetdata + 1, NULL, NULL, elsetqty, NULL);
    }

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&prev);
  ut_free_2d (&eltdata, Mesh[dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  neut_simres_free (&SimRes2);

  return;
}

void
nes_pproc_entity_builtin_cells_ori (struct SIM *pSim, struct TESS Tess,
                                    struct TESR Tesr, char *entity, char *res,
                                    struct SIMRES *pSimRes)
{
  int step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    if (!neut_tess_isvoid (Tess) && !Tess.CellCrySym)
      ut_print_message (2, 5, (char *) "\nCrystal symmetry not defined.\n");
    else if (!neut_tesr_isvoid (Tesr) && !Tesr.CellCrySym)
      ut_print_message (2, 5, (char *) "\nCrystal symmetry not defined.\n");

    neut_tesr_oriaverage (&Tesr);
    neut_ori_fprintf (file, (*pSim).OriDes, (char *) "ascii", Tesr.CellOri + 1, NULL, NULL, Tesr.CellQty, NULL);

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_cells_gos (struct SIM *pSim, struct TESS Tess,
                                    struct TESR Tesr, char *entity, char *res,
                                    struct SIMRES *pSimRes)
{
  int i, step;
  double gos;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    if (!neut_tess_isvoid (Tess) && !Tess.CellCrySym)
      ut_print_message (2, 5, (char *) "\nCrystal symmetry not defined.\n");
    else if (!neut_tesr_isvoid (Tesr) && !Tesr.CellCrySym)
      ut_print_message (2, 5, (char *) "\nCrystal symmetry not defined.\n");

    if (!neut_tesr_isvoid (Tesr))
      for (i = 1; i <= Tesr.CellQty; i++)
      {
        neut_tesr_cell_gos (Tesr, i, &gos);
        fprintf (file, REAL_PRINT_FORMAT "\n", gos);
      }

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_cells_pre (struct SIM *pSim, struct TESS Tess,
                                  struct TESR Tesr, char *entity, char *res,
                                  struct SIMRES *pSimRes,
                                  int ***pcells, int *pcellqty)
{
  int i, pos;
  char *type = NULL;

  neut_sim_entity_pos (*pSim, entity, &pos);
  neut_sim_entity_type (*pSim, entity, &type);

  neut_simres_set_zero (pSimRes);

  neut_sim_simres (*pSim, entity, res, pSimRes);

  if (!strcmp (entity, (char *) "tess"))
  {
    *pcellqty = 1;
    (*pcells) = ut_alloc_1d_pint (2);
    (*pcells)[1] = ut_alloc_1d_int (Tess.CellQty + 1);
    ut_array_1d_int_set_id ((*pcells)[1], Tess.CellQty + 1);
    (*pcells)[1][0] = Tess.CellQty;
  }

  else if (!strcmp (entity, (char *) "tesr"))
  {
    *pcellqty = 1;
    (*pcells) = ut_alloc_1d_pint (2);
    (*pcells)[1] = ut_alloc_1d_int (Tesr.CellQty + 1);
    ut_array_1d_int_set_id ((*pcells)[1], Tesr.CellQty + 1);
    (*pcells)[1][0] = Tesr.CellQty;
  }

  else if (!strcmp (entity, (char *) "cell"))
  {
    if (!neut_tess_isvoid (Tess))
    {
      *pcellqty = Tess.CellQty;
      *pcells = ut_alloc_2d_int (*pcellqty + 1, 2);

      for (i = 1; i <= *pcellqty; i++)
      {
        (*pcells)[i][0] = 1;
        (*pcells)[i][1] = i;
      }
    }
    else if (!neut_tesr_isvoid (Tesr))
    {
      *pcellqty = Tesr.CellQty;
      *pcells = ut_alloc_2d_int (*pcellqty + 1, 2);

      for (i = 1; i <= *pcellqty; i++)
      {
        (*pcells)[i][0] = 1;
        (*pcells)[i][1] = i;
      }
    }
  }

  else if (neut_sim_entity_hasexpr (*pSim, entity))
    for (i = 1; i <= *pcellqty; i++)
    {
      (*pcells)[i] = ut_alloc_1d_int ((*pSim).EntityMembers[pos][i - 1][0] + 1);
      ut_array_1d_int_memcpy ((*pSim).EntityMembers[pos][i - 1],
                              (*pSim).EntityMembers[pos][i - 1][0] + 1, (*pcells)[i]);
    }

  else
    abort ();

  ut_free_1d_char (&type);

  return;
}

void
nes_pproc_entity_builtin_cells_odf (struct SIM *pSim, struct TESS *pTess, struct TESR Tesr,
                                    char *entity, char *res, struct SIMRES *pSimRes)
{
  int i, step, varqty;
  char *prev = ut_alloc_1d_char (1000);
  char *filename = NULL;
  struct OL_SET OSet;
  struct ODF Odf;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  char *weight = NULL, *oriinput = NULL;
  int clustering;
  char *thetastring = ut_alloc_1d_char (100);
  char *cutoff = NULL;
  struct SIMRES SimRes2;

  // variables defined from tess or tesr
  char *input = NULL;           // tess or tesr
  int CellQty;
  double **CellOri = NULL;      // pointer only
  char **CellOriDistrib = NULL; // pointer only
  char *crysym = NULL;          // crysym

  ol_set_zero (&OSet);

  neut_simres_set_zero (&SimRes2);

  ut_string_function (res, &fct, &vars, &vals, &varqty);

  neut_sim_orispace (*pSim, &Odf, (char *) "R");

  if (!neut_tess_isvoid (*pTess))
  {
    ut_string_string ((char *) "tess", &input);
    CellQty = (*pTess).CellQty;
    CellOri = (*pTess).CellOri;
    CellOriDistrib = (*pTess).CellOriDistrib;
    ut_string_string ((*pTess).CellCrySym, &crysym);
  }
  else if (!neut_tesr_isvoid (Tesr))
  {
    ut_string_string ((char *) "tesr", &input);
    CellQty = Tesr.CellQty;
    CellOri = Tesr.CellOri;
    CellOriDistrib = Tesr.CellOriDistrib;
    ut_string_string (Tesr.CellCrySym, &crysym);
  }
  else
    abort ();

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    // Setting defaults and parsing arguments
    Odf.sigma = -1;
    ut_string_string ((char *) "size", &weight);
    ut_string_string ((char *) "all", &cutoff);
    ut_string_string ((char *) "cells", &oriinput);
    clustering = -1;

    // Parsing arguments
    for (i = 0; i < varqty; i++)
    {
      if (!strcmp (vars[i], (char *) "theta"))
        Odf.sigma = atof (vals[i]) * M_PI / 180;
      else if (!strcmp (vars[i], (char *) "weight"))
        ut_string_string (vals[i], &weight);
      else if (!strcmp (vars[i], (char *) "input"))
        ut_string_string (vals[i], &oriinput);
      else if (!strcmp (vars[i], (char *) "cutoff"))
        ut_string_string (vals[i], &cutoff);
      else if (!strcmp (vars[i], (char *) "clustering"))
        clustering = atoi (vals[i]);
      else
        ut_print_exprbug (vars[i]);
    }

    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, (char *) "cells", (char *) "ori", &SimRes2);

    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (&SimRes2, step);

    ut_string_fnrs (oriinput, (char *) "cells", (char *) "cell", 1);
    ut_string_fnrs (oriinput, (char *) "pixels", (char *) "pixel", 1);
    ut_string_fnrs (oriinput, (char *) "voxels", (char *) "voxel", 1);
    ut_string_fnrs (oriinput, (char *) "pixel", (char *) "voxel", 1);

    // Setting orientations
    if (!strcmp (oriinput, (char *) "cell"))
    {
      OSet = ol_set_alloc (CellQty, crysym);

      // general case
      if (ut_file_exist (SimRes2.file))
        neut_ori_fnscanf (SimRes2.file, (*pSim).OriDes, (char *) "ascii", OSet.q,
                          NULL, CellQty, NULL, (char *) "R");

      // only initial state and no ori in results
      else if (!step)
        ut_array_2d_memcpy (CellOri + 1, CellQty, 4, OSet.q);

      else
        abort ();
    }

    else if (!strcmp (input, (char *) "tesr") && !strcmp (oriinput, (char *) "voxel"))
      neut_tesr_voxels_olset (Tesr, &OSet);

    else
      ut_print_exprbug (oriinput);

    // Managing defaults
    if (Odf.sigma == -1)
      neut_odf_setsigma (&Odf, (char *) "avthetaeq", OSet.size, OSet.crysym);

    if (clustering == -1)
    {
      if (!strcmp (oriinput, (char *) "cell"))
        clustering = 0;
      else if (!strcmp (oriinput, (char *) "voxel"))
        clustering = 1;
      else
        abort ();
    }

    // Printing to terminal
    sprintf (thetastring, (char *) " (theta = %9.6f°)     ", Odf.sigma * 180 / M_PI);
    ut_print_clearline (stdout, strlen (thetastring) - 1);
    printf ((char *) "%s", thetastring);
    ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

    /*
    printf ((char *) "\n");
    ut_print_message (0, 5, (char *) "(theta = %4.1f°) ", Odf.sigma * 180 / M_PI);
    for (i = 0; i < 27 - ut_num_tenlen (step + 1) + ut_num_tenlen ((*pSim).StepQty + 1); i++)
      printf ((char *) ".");
    printf ((char *) " %d/%d: ", step + 1, (*pSim).StepQty + 1);
    */

    // Computing weights
    if (!strcmp (oriinput, (char *) "cell"))
    {
      if (!strcmp (input, (char *) "tess"))
        for (i = 1; i <= CellQty; i++)
          neut_tess_expr_val_one (pTess, NULL, NULL, NULL, (char *) "cell", i, weight,
                                 OSet.weight + i - 1, NULL);
      else
        for (i = 1; i <= CellQty; i++)
          neut_tesr_expr_val_one (Tesr, (char *) "cell", i, weight, OSet.weight + i - 1,
                                 NULL);
    }

    for (i = 0; i < CellQty; i++)
      if (OSet.weight[i] < 0)
        ut_print_message (2, 2, (char *) "Negative weight.\n");

    if (clustering)
      neut_oset_odf_clustering (OSet, Odf, NULL, &OSet);

    if (!strcmp (oriinput, (char *) "cell") && CellOriDistrib)
    {
      if (clustering)
        ut_print_message (2, 2, (char *) "Clustering and cell with theta are mutually exclusive\n");

      OSet.theta = ut_alloc_1d (OSet.size);

      for (i = 0; i < (int) OSet.size; i++)
        sscanf (CellOriDistrib[i + 1], (char *) "normal(%lf)", OSet.theta + i);

      ut_array_1d_scale (OSet.theta, OSet.size, M_PI / 180);
    }

    if (!strcmp (fct, (char *) "odf"))
      neut_odf_comp ((char *) "m", cutoff, &OSet, &Odf, 1);
    else if (!strcmp (fct, (char *) "odfn"))
      neut_odf_comp ((char *) "n", cutoff, &OSet, &Odf, 1);

    // Writing results
    if (!strcmp (fct, (char *) "odf"))
      ut_array_1d_fnprintf_column ((*pSimRes).file, Odf.odf, Odf.odfqty, REAL_PRINT_FORMAT, (char *) "W");
    else if (!strcmp (fct, (char *) "odfn"))
      ut_array_1d_fnprintf_column ((*pSimRes).file, Odf.odfn, Odf.odfnqty, REAL_PRINT_FORMAT, (char *) "W");
  }

  nes_pproc_entity_builtin_odf_writeconfig (pSimRes, fct, Odf);

  ol_set_free (&OSet);

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  // neut_odf_free (&Odf);
  ut_free_1d_char (&crysym);
  ut_free_1d_char (&filename);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  ut_free_1d_char (&input);
  ut_free_1d_char (&weight);
  neut_simres_free (&SimRes2);
  ut_free_1d_char (&cutoff);
  ut_free_1d_char (&oriinput);

  return;
}

void
nes_pproc_entity_builtin_elsets_gos (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  struct SIMRES SimRes2;
  double *gos = ut_alloc_1d (elsetqty + 1);

  neut_simres_set_zero (&SimRes2);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_sim_simres (*pSim, (char *) "elts", (char *) "ori", &SimRes2);
    neut_simres_setstep (&SimRes2, step);

    if (!SimRes2.file)
      abort ();

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
    neut_ori_fnscanf (SimRes2.file, (*pSim).OriDes, (char *) "ascii", eltdata + 1, NULL, Mesh[Tess.Dim].EltQty, NULL, (char *) "R");

    neut_mesh_eltdata_elsetdata_origos (*pNodes, Mesh[Tess.Dim], elsets,
                                        elsetqty, gos);

    for (i = 1; i <= elsetqty; i++)
      fprintf (file, REAL_PRINT_FORMAT "\n", gos[i]);

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  ut_free_1d_char (&prev);
  neut_simres_free (&SimRes2);
  ut_free_1d (&gos);

  return;
}

void
nes_pproc_entity_builtin_elsets_oridis (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  double ***evect = ut_alloc_3d (Mesh[Tess.Dim].ElsetQty + 1, 3, 3);
  double **eval = ut_alloc_2d (Mesh[Tess.Dim].ElsetQty + 1, 3);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_mesh_init_simeltori (Mesh + Tess.Dim);

    neut_mesh_eltdata_elsetdata_oridis (*pNodes, Mesh[Tess.Dim], elsets, elsetqty,
                                        evect, eval);

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    for (i = 1; i <= elsetqty; i++)
    {
      if (!strcmp (res, (char *) "oridisanisoangles"))
        ut_array_1d_fprintf (file, eval[i], 3, REAL_PRINT_FORMAT);
      else if (!strcmp (res, (char *) "oridisanisoaxes"))
        ut_array_2d_fprintf_oneline (file, evect[i], 3, 3, REAL_PRINT_FORMAT);
      else if (!strcmp (res, (char *) "oridisanisofact"))
        fprintf (file, REAL_PRINT_FORMAT "\n", eval[i][0] / pow (ut_array_1d_prod (eval[i], 3), 1. / 3));
    }

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_mesh_free_simeltori (Mesh + Tess.Dim);

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_3d (&evect, Mesh[Tess.Dim].ElsetQty + 1, 3);
  ut_free_2d (&eval, Mesh[Tess.Dim].ElsetQty + 1);
  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_elsets_odf (struct SIM *pSim, struct TESS *pTess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, struct SIMRES *pSimRes)
{
  int i, step, varqty, status, oriqty;
  double **oris = NULL;
  char *prev = ut_alloc_1d_char (1000);
  char *filename = NULL;
  FILE *file = NULL;
  struct OL_SET OSet;
  struct ODF Odf;
  char *fct = NULL, **vars = NULL, **vals = NULL;
  char *weight = NULL, *oriinput = NULL;
  int clustering;
  char *thetastring = ut_alloc_1d_char (100);
  char *cutoff = NULL;
  struct SIMRES SimRes2;

  ol_set_zero (&OSet);

  neut_simres_set_zero (&SimRes2);

  ut_string_function (res, &fct, &vars, &vals, &varqty);

  neut_sim_orispace (*pSim, &Odf, (char *) "R");

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    // Setting defaults and parsing arguments
    Odf.sigma = -1;
    ut_string_string ((char *) "size", &weight);
    ut_string_string ((char *) "all", &cutoff);
    ut_string_string ((char *) "elsets", &oriinput);
    clustering = -1;

    // Parsing arguments
    for (i = 0; i < varqty; i++)
    {
      if (!strcmp (vars[i], (char *) "theta"))
        Odf.sigma = atof (vals[i]) * M_PI / 180;
      else if (!strcmp (vars[i], (char *) "weight"))
        ut_string_string (vals[i], &weight);
      else if (!strcmp (vars[i], (char *) "input"))
        ut_string_string (vals[i], &oriinput);
      else if (!strcmp (vars[i], (char *) "cutoff"))
        ut_string_string (vals[i], &cutoff);
      else if (!strcmp (vars[i], (char *) "clustering"))
        clustering = atoi (vals[i]);
      else
        ut_print_exprbug (vars[i]);
    }

    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, oriinput, (char *) "ori", &SimRes2);

    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (&SimRes2, step);

    ut_string_fnrs (oriinput, (char *) "elts", (char *) "elt", 1);
    ut_string_fnrs (oriinput, (char *) "elsets", (char *) "elset", 1);

    // Setting orientations
    oriqty = 0;

    if (!strcmp (oriinput, (char *) "elset"))
    {
      oriqty = Mesh[(*pTess).Dim].ElsetQty;
      oris = ut_alloc_2d (oriqty + 1, 4);
      status = neut_mesh_elsetori (Mesh[(*pTess).Dim], oris);
      if (status)
        abort ();

      neut_mesh_elsets_olset (*pNodes, Mesh[(*pTess).Dim], oris,
                              NULL, oriqty, &OSet);

      ut_free_2d (&oris, oriqty);
    }

    else if (!strcmp (oriinput, (char *) "elt"))
    {
      OSet = ol_set_alloc (Mesh[(*pTess).Dim].EltQty, Mesh[(*pTess).Dim].ElsetCrySym);
      neut_mesh_eltori (Mesh[(*pTess).Dim], OSet.q - 1);
      for (i = 1; i <= Mesh[(*pTess).Dim].EltQty; i++)
        neut_mesh_elt_size (*pNodes, Mesh[(*pTess).Dim], i, OSet.weight + i - 1);
    }

    else
      ut_print_exprbug (oriinput);

    // Managing defaults
    if (Odf.sigma == -1)
      neut_odf_setsigma (&Odf, (char *) "avthetaeq", OSet.size, OSet.crysym);

    if (clustering == -1)
    {
      if (!strcmp (oriinput, (char *) "elset"))
        clustering = 0;
      else if (!strcmp (oriinput, (char *) "elt"))
        clustering = 1;
      else
        abort ();
    }

    // Printing to terminal
    sprintf (thetastring, (char *) " (theta = %9.6f°)     ", Odf.sigma * 180 / M_PI);
    ut_print_clearline (stdout, strlen (thetastring) - 1);
    printf ((char *) "%s", thetastring);
    ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

    // Computing weights if different from default "size"
    if (strcmp (weight, (char *) "size"))
      neut_mesh_entity_expr_val (*pNodes, Mesh, pTess,
                                 NULL, NULL, NULL, NULL, oriinput, weight,
                                 OSet.weight - 1, NULL);

    if (clustering)
      neut_oset_odf_clustering (OSet, Odf, NULL, &OSet);

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    if (!strcmp (fct, (char *) "odf"))
      neut_odf_comp ((char *) "m", cutoff, &OSet, &Odf, 0);
    else if (!strcmp (fct, (char *) "odfn"))
      neut_odf_comp ((char *) "n", cutoff, &OSet, &Odf, 0);

    // Writing results
    if (!strcmp (fct, (char *) "odf"))
      ut_array_1d_fnprintf_column ((*pSimRes).file, Odf.odf, Odf.odfqty, REAL_PRINT_FORMAT, (char *) "W");
    else if (!strcmp (fct, (char *) "odfn"))
      ut_array_1d_fnprintf_column ((*pSimRes).file, Odf.odfn, Odf.odfnqty, REAL_PRINT_FORMAT, (char *) "W");

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  nes_pproc_entity_builtin_odf_writeconfig (pSimRes, fct, Odf);

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  // neut_odf_free (&Odf);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);
  ut_free_1d_char (&thetastring);
  ut_free_2d_char (&vars, varqty);
  ut_free_2d_char (&vals, varqty);
  neut_simres_free (&SimRes2);
  ut_free_1d_char (&weight);
  ut_free_1d_char (&oriinput);
  ut_free_1d_char (&cutoff);

  return;
}

void
nes_pproc_entity_builtin_elsets_readodf (struct SIM *pSim, struct TESS Tess,
                                         struct MESH *Mesh,
                                         char *entity, char *res, struct SIMRES *pSimRes)
{
  struct ODF Odf;
  char *orides = NULL;
  char *prev = ut_alloc_1d_char (1000);
  int i, step;
  FILE *file = NULL;
  int entityqty;
  double **ori = NULL;
  struct SIMRES SimRes2;
  double *oriodf = NULL;

  neut_simres_set_zero (&SimRes2);

  if (!strcmp (entity, (char *) "elset"))
    entityqty = Mesh[Tess.Dim].ElsetQty;
  else if (!strcmp (entity, (char *) "elt"))
    entityqty = Mesh[Tess.Dim].EltQty;
  else
    abort ();

  ori = ut_alloc_2d (entityqty + 1, 4);
  oriodf = ut_alloc_1d (entityqty + 1);

  neut_sim_orispace (*pSim, &Odf, (char *) "R");

  neut_odf_orides (Odf, &orides);
  Odf.odfqty = Odf.Mesh[3].EltQty;
  Odf.odf = ut_alloc_1d (Odf.odfqty + 1);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_sim_simres (*pSim, (char *) "mesh", res, &SimRes2);

    // loading mesh ODF
    ut_array_1d_fnscanf (SimRes2.file, Odf.odf + 1, Odf.odfqty, (char *) "R");

    // loading elset orientations
    if (!strcmp (entity, (char *) "elt"))
      neut_mesh_eltori (Mesh[Tess.Dim], ori);
    else if (!strcmp (entity, (char *) "elset"))
      neut_mesh_elsetori (Mesh[Tess.Dim], ori);

#pragma omp parallel for private(i)
    for (i = 1; i <= entityqty; i++)
    {
      int elt;
      double *R = ol_R_alloc ();

      if (!strcmp (orides, (char *) "rodrigues"))
      {
        ol_q_R (ori[i], R);
        ol_R_Rcrysym (R, Mesh[Tess.Dim].ElsetCrySym, R);
      }

      neut_mesh_point_elt (Odf.Nodes, Odf.Mesh[3], R, &elt);

      if (elt == -1)
        abort ();

      oriodf[i] = Odf.odf[elt];

      ol_R_free (R);
    }

    file = ut_file_open ((*pSimRes).file, (char *) "W");
    ut_array_1d_fprintf_column (file, oriodf + 1, entityqty, REAL_PRINT_FORMAT);
    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&orides);
  ut_free_1d_char (&prev);
  ut_free_2d (&ori, entityqty + 1);
  neut_simres_free (&SimRes2);
  ut_free_1d (&oriodf);

  return;
}

void
nes_pproc_entity_builtin_cells_readodf (struct SIM *pSim, struct TESS Tess,
                                        struct TESR Tesr,
                                        char *entity, char *res, struct SIMRES *pSimRes)
{
  struct ODF Odf;
  char *orides = NULL;
  char *prev = ut_alloc_1d_char (1000);
  int i, step;
  FILE *file = NULL;
  int entityqty;
  double **ori = NULL;
  struct SIMRES SimRes2;
  double *oriodf = NULL;
  char *parent = NULL;
  char *crysym = NULL;

  neut_simres_set_zero (&SimRes2);

  if (!strcmp (entity, (char *) "cell"))
  {
    if (!neut_tess_isvoid (Tess))
    {
      entityqty = Tess.CellQty;
      ut_string_string ((char *) "tess", &parent);
      ut_string_string (Tess.CellCrySym, &crysym);
    }
    else if (!neut_tesr_isvoid (Tesr))
    {
      entityqty = Tesr.CellQty;
      ut_string_string ((char *) "tesr", &parent);
      ut_string_string (Tesr.CellCrySym, &crysym);
    }
    else
      abort ();
  }
  else
    abort ();

  ori = ut_alloc_2d (entityqty + 1, 4);
  oriodf = ut_alloc_1d (entityqty + 1);

  neut_sim_orispace (*pSim, &Odf, (char *) "R");

  neut_odf_orides (Odf, &orides);
  Odf.odfqty = Odf.Mesh[3].EltQty;
  Odf.odf = ut_alloc_1d (Odf.odfqty + 1);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    neut_sim_simres (*pSim, parent, res, &SimRes2);

    // loading mesh ODF
    ut_array_1d_fnscanf (SimRes2.file, Odf.odf + 1, Odf.odfqty, (char *) "R");

    // loading cell orientations
    if (!strcmp (parent, (char *) "tess"))
      neut_tess_cellori (Tess, ori);
    else if (!strcmp (parent, (char *) "tesr"))
      neut_tesr_cellori (Tesr, ori);

#pragma omp parallel for private(i)
    for (i = 1; i <= entityqty; i++)
    {
      int elt;
      double *R = ol_R_alloc ();

      if (!strcmp (orides, (char *) "rodrigues"))
      {
        ol_q_R (ori[i], R);
        ol_R_Rcrysym (R, crysym, R);
      }

      neut_mesh_point_elt (Odf.Nodes, Odf.Mesh[3], R, &elt);

      if (elt == -1)
      {
        ol_R_fprintf (stdout, R, (char *) "%f");
        abort ();
      }

      oriodf[i] = Odf.odf[elt];

      ol_R_free (R);
    }

    file = ut_file_open ((*pSimRes).file, (char *) "W");
    ut_array_1d_fprintf_column (file, oriodf + 1, entityqty, REAL_PRINT_FORMAT);
    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&orides);
  ut_free_1d_char (&prev);
  ut_free_2d (&ori, entityqty + 1);
  neut_simres_free (&SimRes2);
  ut_free_1d (&oriodf);
  ut_free_1d_char (&parent);
  ut_free_1d_char (&crysym);

  return;
}

void
nes_pproc_entity_builtin_elsets_gen (struct SIM *pSim,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res_in, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes)
{
  int i, step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double **eltdata = NULL;
  double **elsetdata = NULL;
  struct SIMRES SimRes2;
  char *res = NULL;
  char *op = NULL;
  int dim = neut_mesh_array_dim (Mesh);

  ut_string_string (res_in, &res);
  ut_string_string ((char *) "mean", &op);
  if (strstr (res_in, (char *) "_mean"))
  {
    ut_string_fnrs (res, (char *) "_mean", (char *) "", 1);
    ut_string_string ((char *) "mean", &op);
  }
  else if (strstr (res_in, (char *) "_stddev"))
  {
    ut_string_fnrs (res, (char *) "_stddev", (char *) "", 1);
    ut_string_string ((char *) "stddev", &op);
  }
  else if (strstr (res_in, (char *) "_var"))
  {
    ut_string_fnrs (res, (char *) "_var", (char *) "", 1);
    ut_string_string ((char *) "var", &op);
  }
  else if (strstr (res_in, (char *) "_prval"))
  {
    ut_string_fnrs (res, (char *) "_prval", (char *) "", 1);
    ut_string_string ((char *) "prval", &op);
  }
  else if (strstr (res_in, (char *) "_prvect"))
  {
    ut_string_fnrs (res, (char *) "_prvect", (char *) "", 1);
    ut_string_string ((char *) "prvect", &op);
  }

  neut_simres_set_zero (&SimRes2);
  neut_sim_simres (*pSim, (char *) "elts", res, &SimRes2);

  eltdata = ut_alloc_2d (Mesh[dim].EltQty + 1, SimRes2.colqty);
  elsetdata = ut_alloc_2d (elsetqty + 1, SimRes2.colqty);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_sim_simres (*pSim, (char *) "elts", res, &SimRes2);
    neut_simres_setstep (pSimRes, step);

    if (!SimRes2.file)
      abort ();

    file = ut_file_open ((*pSimRes).file, (char *) "W");

    ut_array_2d_fnscanf (SimRes2.file, eltdata + 1, Mesh[dim].EltQty, SimRes2.colqty, (char *) "R");

    neut_mesh_eltdata_elsetdata_stat (*pNodes, Mesh[dim], elsets, elsetqty,
                                      eltdata, SimRes2.colqty, op, elsetdata);

    for (i = 1; i <= elsetqty; i++)
      ut_array_1d_fprintf (file, elsetdata[i], SimRes2.colqty, REAL_PRINT_FORMAT);

    ut_file_close (file, (*pSimRes).file, (char *) "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, (char *) "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res_in, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, (char *) "W");

  ut_free_1d_char (&prev);
  ut_free_2d (&eltdata, Mesh[dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  neut_simres_free (&SimRes2);
  ut_free_1d_char (&res);
  ut_free_1d_char (&op);

  return;
}
