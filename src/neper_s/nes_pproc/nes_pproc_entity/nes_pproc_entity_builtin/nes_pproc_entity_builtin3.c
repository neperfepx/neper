/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_pproc_entity_builtin_.h"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int
nes_pproc_entity_builtin_nodes_disp (struct SIM *pSim, char *dir, char *res,
                                  struct SIMRES *pSimRes)
{
  int step;
  char *simfile = ut_alloc_1d_char (1000);
  char *filename = ut_alloc_1d_char (1000);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double ***coo = ut_alloc_3d (2, (*pSim).NodeQty, 3);
  double **disp = ut_alloc_2d ((*pSim).NodeQty, 3);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  neut_sim_setstep (pSim, 0);

  ut_array_2d_fnscanf ((*pSimRes).file, coo[0], (*pSim).NodeQty, 3, "R");

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);

    if (!ut_file_exist ((*pSimRes).file))
      abort ();

    ut_array_2d_fnscanf ((*pSimRes).file, coo[1], (*pSim).NodeQty, 3, "R");

    ut_array_2d_sub (coo[0], coo[1], (*pSim).NodeQty, 3, disp);

    sprintf (filename, "%s/%s.step%d", dir, res, step);

    file = ut_file_open (filename, "W");
    ut_array_2d_fprintf (file, disp, (*pSim).NodeQty, 3, "%.12f");
    ut_file_close (file, filename, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  ut_free_3d (&coo, 2, (*pSim).NodeQty);
  ut_free_2d (&disp, (*pSim).NodeQty);

  neut_sim_addres (pSim, "node", res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&simfile);
  ut_free_1d_char (&filename);
  ut_free_1d_char (&prev);

  return 0;
}

void
nes_pproc_entity_builtin_elsets_pre (struct SIM *pSim, struct TESS Tess,
                                  struct MESH *Mesh,
                                  char *entity, char *res,
                                  struct SIMRES *pSimRes,
                                  struct SIMRES *pSimRes2,
                                  int ***pelsets, int *pelsetqty)
{
  int i, pos;
  char *type = NULL;

  neut_sim_entity_pos (*pSim, entity, &pos);
  neut_sim_entity_type (*pSim, entity, &type);

  neut_simres_set_zero (pSimRes);
  neut_simres_set_zero (pSimRes2);

  neut_sim_simres (*pSim, entity, res, pSimRes);
  neut_sim_simres (*pSim, "elt", res, pSimRes2);

  *pelsetqty = (*pSim).EntityMemberQty[pos];
  (*pelsets) = ut_alloc_1d_pint (*pelsetqty + 1);

  if (!strcmp (entity, "elset"))
    for (i = 1; i <= *pelsetqty; i++)
    {
      (*pelsets)[i] = ut_alloc_1d_int (Mesh[Tess.Dim].Elsets[i][0] + 1);
      ut_array_1d_int_memcpy (Mesh[Tess.Dim].Elsets[i], Mesh[Tess.Dim].Elsets[i][0] + 1, (*pelsets)[i]);
    }

  else if (!strcmp (entity, "mesh"))
    neut_mesh_aselsets (Mesh[Tess.Dim], pelsets, pelsetqty);

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
nes_pproc_entity_builtin_elsets_ori (struct SIM *pSim, struct TESS Tess,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes,
                                  struct SIMRES *pSimRes2)
{
  int step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (pSimRes2, step);

    neut_sim_simres (*pSim, "elts", res, pSimRes2);

    if (!(*pSimRes2).file)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    if (!Tess.CellCrySym)
      ut_print_message (2, 5, "\nCrystal symmetry not defined.\n");

    neut_ori_fnscanf ((*pSimRes2).file, (*pSim).OriDes, "ascii", eltdata + 1, NULL, Mesh[Tess.Dim].EltQty, NULL, "R");
    neut_mesh_eltdata_elsetdata_ori (*pNodes, Mesh[Tess.Dim], elsets, elsetqty,
                                     eltdata, Tess.CellCrySym, elsetdata);

    neut_ori_fprintf (file, (*pSim).OriDes, "ascii", elsetdata + 1, NULL, NULL, elsetqty, NULL);

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);

  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&prev);
  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);

  return;
}

void
nes_pproc_entity_builtin_elsets_oridis (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, int **elsets,
                                     int elsetqty, struct SIMRES *pSimRes,
                                     struct SIMRES *pSimRes2)
{
  int i, step;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, 4);
  double ***evect = ut_alloc_3d (Mesh[Tess.Dim].ElsetQty + 1, 3, 3);
  double **eval = ut_alloc_2d (Mesh[Tess.Dim].ElsetQty + 1, 3);
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (pSimRes2, step);

    neut_sim_simres (*pSim, "elts", "ori", pSimRes2);

    if (!(*pSimRes2).file)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
    neut_ori_fnscanf ((*pSimRes2).file, (*pSim).OriDes, "ascii", eltdata + 1, NULL, Mesh[Tess.Dim].EltQty, NULL, "R");
    neut_mesh_eltdata_elsetdata_oridis (*pNodes, Mesh[Tess.Dim], elsets, elsetqty,
                                        eltdata, Tess.CellCrySym, evect,
                                        eval);

    for (i = 1; i <= elsetqty; i++)
    {
      if (!strcmp (res, "oridisanisoangles"))
        ut_array_1d_fprintf (file, eval[i], 3, "%.12f");
      else if (!strcmp (res, "oridisanisoaxes"))
        ut_array_2d_fprintf_oneline (file, evect[i], 3, 3, "%.12f");
      else if (!strcmp (res, "oridisanisofact"))
        fprintf (file, "%.12f\n", eval[i][0] / pow (ut_array_1d_prod (eval[i], 3), 1. / 3));
    }

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_3d (&evect, Mesh[Tess.Dim].ElsetQty + 1, 3);
  ut_free_2d (&eval, Mesh[Tess.Dim].ElsetQty + 1);
  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);
  ut_free_1d_char (&prev);

  return;
}

void
nes_pproc_entity_builtin_elsets_odf (struct SIM *pSim, struct TESS Tess,
                                     struct NODES *pNodes, struct MESH *Mesh,
                                     char *entity, char *res, struct SIMRES *pSimRes,
                                     struct SIMRES *pSimRes2)
{
  int i, step, size;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, 4);
  double **elsetdata = ut_alloc_2d (Mesh[Tess.Dim].ElsetQty + 1, 4);
  double ***evect = ut_alloc_3d (Mesh[Tess.Dim].ElsetQty + 1, 3, 3);
  double **eval = ut_alloc_2d (Mesh[Tess.Dim].ElsetQty + 1, 3);
  char *prev = ut_alloc_1d_char (1000);
  char *filename = NULL;
  FILE *file = NULL;
  struct OL_SET OSet;
  struct ODF Odf;
  char *fct = NULL, **vars = NULL, **vals = NULL;

  ut_string_function (res, &fct, &vars, &vals, &size);

  neut_sim_orispace (*pSim, &filename);

  neut_odf_space_fnscanf (filename, &Odf, "R");

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (pSimRes2, step);

    neut_sim_simres (*pSim, "elsets", "ori", pSimRes2);

    // general case
    if ((*pSim).StepQty > 0 || ut_file_exist ((*pSimRes2).file))
    {
      if (!(*pSimRes2).file || !ut_file_exist ((*pSimRes2).file))
        abort ();

      neut_ori_fnscanf ((*pSimRes2).file, (*pSim).OriDes, "ascii", elsetdata + 1,
                        NULL, Mesh[Tess.Dim].ElsetQty, NULL, "R");
    }

    // only initial state and no ori in results
    else
      ut_array_2d_memcpy (Mesh[Tess.Dim].ElsetOri + 1, Mesh[Tess.Dim].ElsetQty, 4, elsetdata + 1);

    file = ut_file_open ((*pSimRes).file, "W");

    neut_mesh_elsets_olset (*pNodes, Mesh[Tess.Dim], elsetdata, Tess.CellCrySym,
                            NULL, Mesh[Tess.Dim].ElsetQty, &OSet);
    ut_string_string (Tess.CellCrySym, &(OSet.crysym));

    int user_sigma = 0;
    for (i = 0; i < size; i++)
    {
      if (!strcmp (vars[i], "sigma"))
      {
        sscanf (vals[i], "%lf", &Odf.sigma);
        user_sigma = 1;
      }
    }
    if (!user_sigma)
      neut_odf_sigma ("avthetaeq", &OSet, &Odf);

    printf ("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b (sigma = %8.6f) ", Odf.sigma);

    ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

    if (!strcmp (fct, "odf"))
    {
      neut_odf_comp ("m", "5", &OSet, &Odf);
      for (i = 0; i < Odf.odfqty; i++)
        fprintf (file, "%.12f\n", Odf.odf[i]);
    }
    else if (!strcmp (fct, "odfn"))
    {
      neut_odf_comp ("n", "5", &OSet, &Odf);
      for (i = 0; i < Odf.odfnqty; i++)
        fprintf (file, "%.12f\n", Odf.odfn[i]);
    }

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  // neut_odf_free (&Odf);
  ut_free_1d_char (&filename);
  ut_free_3d (&evect, Mesh[Tess.Dim].ElsetQty + 1, 3);
  ut_free_2d (&eval, Mesh[Tess.Dim].ElsetQty + 1);
  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, Mesh[Tess.Dim].ElsetQty + 1);
  ut_free_1d_char (&prev);
  ut_free_2d_char (&vars, size);
  ut_free_2d_char (&vals, size);

  return;
}

void
nes_pproc_entity_builtin_elsets_gen (struct SIM *pSim, struct TESS Tess,
                                  struct NODES *pNodes, struct MESH *Mesh,
                                  char *entity, char *res, int **elsets,
                                  int elsetqty, struct SIMRES *pSimRes,
                                  struct SIMRES *pSimRes2)
{
  int i, step;
  char *prev = ut_alloc_1d_char (1000);
  FILE *file = NULL;
  double **eltdata = ut_alloc_2d (Mesh[Tess.Dim].EltQty + 1, (*pSimRes2).colqty);
  double **elsetdata = ut_alloc_2d (elsetqty + 1, (*pSimRes2).colqty);

  ut_print_progress (stdout, 0, (*pSim).StepQty + 1, "%3.0f%%", prev);

  for (step = 0; step <= (*pSim).StepQty; step++)
  {
    neut_sim_setstep (pSim, step);
    neut_simres_setstep (pSimRes, step);
    neut_simres_setstep (pSimRes2, step);

    neut_sim_simres (*pSim, "elts", res, pSimRes2);

    if (!(*pSimRes2).file)
      abort ();

    file = ut_file_open ((*pSimRes).file, "W");

    ut_array_2d_fnscanf ((*pSimRes2).file, eltdata + 1, Mesh[Tess.Dim].EltQty, (*pSimRes2).colqty, "R");

    neut_mesh_eltdata_elsetdata (*pNodes, Mesh[Tess.Dim], elsets, elsetqty,
                                 eltdata, (*pSimRes2).colqty, elsetdata);

    for (i = 1; i <= elsetqty; i++)
      ut_array_1d_fprintf (file, elsetdata[i], (*pSimRes2).colqty, "%.12f");

    ut_file_close (file, (*pSimRes).file, "W");

    ut_print_progress (stdout, step + 1, (*pSim).StepQty + 1, "%3.0f%%", prev);
  }

  neut_sim_setstep (pSim, 0);
  neut_sim_addres (pSim, entity, res, NULL);
  neut_sim_fprintf ((*pSim).simdir, *pSim, "W");

  ut_free_1d_char (&prev);
  ut_free_2d (&eltdata, Mesh[Tess.Dim].EltQty + 1);
  ut_free_2d (&elsetdata, elsetqty + 1);

  return;
}
