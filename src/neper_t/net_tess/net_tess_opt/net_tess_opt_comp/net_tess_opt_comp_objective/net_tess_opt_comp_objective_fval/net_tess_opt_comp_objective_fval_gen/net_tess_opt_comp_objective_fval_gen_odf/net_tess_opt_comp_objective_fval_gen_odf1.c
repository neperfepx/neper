/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_gen_odf_.h"

void
net_tess_opt_comp_objective_fval_gen_odf (struct TOPT *pTOpt, int var)
{
  int i, j, k, seed, id, *specialids = NULL, specialidqty = 0;
  struct OL_SET OSet;
  struct ODF Odf;
  char *data = NULL;
  neut_sys_data (&data, 0);

  // alloc'int CellOdfs
  if (!(*pTOpt).CellOdf)
  {
    (*pTOpt).CellOdf = calloc ((*pTOpt).CellQty + 1, sizeof (struct ODF));
    for (i = 0; i <= (*pTOpt).CellQty; i++)
    {
      neut_odf_set_zero ((*pTOpt).CellOdf + i);
      (*pTOpt).CellOdf[i].Sp.pqcloud = (*pTOpt).tarOdf.Sp.pqcloud;
      (*pTOpt).CellOdf[i].Sp.pqtree = (*pTOpt).tarOdf.Sp.pqtree;
      ut_string_string ((*pTOpt).tarOdf.Sp.space, &((*pTOpt).CellOdf[i].Sp.space));
      ut_string_string ((*pTOpt).tarOdf.Sp.spaceunit, &((*pTOpt).CellOdf[i].Sp.spaceunit));

      neut_nodes_memcpy ((*pTOpt).tarOdf.Sp.Nodes, &((*pTOpt).CellOdf[i].Sp.Nodes));
      neut_mesh_memcpy ((*pTOpt).tarOdf.Sp.Mesh[3], (*pTOpt).CellOdf[i].Sp.Mesh + 3);

      (*pTOpt).CellOdf[i].Sp.Mesh[3].EltOri = ut_alloc_2d ((*pTOpt).CellOdf[i].Sp.Mesh[3].EltQty + 1, 4);
      ut_array_2d_memcpy ((*pTOpt).tarOdf.Sp.Mesh[3].EltOri + 1, (*pTOpt).CellOdf[i].Sp.Mesh[3].EltQty, 4, (*pTOpt).CellOdf[i].Sp.Mesh[3].EltOri + 1);
      (*pTOpt).CellOdf[i].Sp.Mesh[3].EltWeight = ut_alloc_1d ((*pTOpt).CellOdf[i].Sp.Mesh[3].EltQty + 1);
      ut_array_1d_memcpy ((*pTOpt).tarOdf.Sp.Mesh[3].EltWeight + 1, (*pTOpt).CellOdf[i].Sp.Mesh[3].EltQty, (*pTOpt).CellOdf[i].Sp.Mesh[3].EltWeight + 1);
      (*pTOpt).CellOdf[i].odfqty = (*pTOpt).tarOdf.Sp.Mesh[3].EltQty;
      (*pTOpt).CellOdf[i].odf = ut_alloc_1d ((*pTOpt).CellOdf[i].odfqty);
      (*pTOpt).CellOdf[i].sigma = (*pTOpt).CellOdf[i].sigma;
    }
  }

  // Do we need to load special ODFs?
  if ((*pTOpt).iter == 1)
  {
    specialidqty = 0;
    for (i = 1; i <= (*pTOpt).CellQty; i++)
      if (ol_q_isspecial ((*pTOpt).SSet.SeedOri[i]))
      {
        id = ol_q_specialid ((*pTOpt).SSet.SeedOri[i]);
        ut_array_1d_int_list_addval (&specialids, &specialidqty, id);
      }

    // we do
    if (specialidqty)
    {
      char *dir = ut_alloc_1d_char (1000);

      sprintf (dir, "%s/specialodf", data);
      if (!ut_sys_isdir (dir))
      {
        printf ("\n");
        ut_print_message (2, 2, "`specialodf' plugin missing.  See the documentation for instructions.\n");
      }

      (*pTOpt).SpecialOdfQty = ut_array_1d_int_max (specialids, specialidqty);

      (*pTOpt).SpecialOdf = calloc ((*pTOpt).SpecialOdfQty + 1, sizeof (struct FCT *));

      for (i = 0; i < specialidqty; i++)
      {
        char *label = ut_alloc_1d_char (1000);
        char *prev = ut_alloc_1d_char (1000);

        id = specialids[i];
        ol_specialid_label (id, label);

        if (id != UNIFORM)
        {
          char *filename = ut_alloc_1d_char (1000);
          double *tmp = ut_alloc_1d ((*pTOpt).tarOdf.odfqty);
          int *thetas = NULL;
          char **files = NULL;
          int thetaqty;

          sprintf (dir, "%s/specialodf/%s/%s", data, label, Odf.Sp.fr);

          ut_dir_allfiles (dir, &files, &thetaqty);
          thetas = ut_alloc_1d_int (thetaqty);
          for (j = 0; j < thetaqty; j++)
            thetas[j] = atoi (files[j]);
          ut_array_1d_int_sort (thetas, thetaqty);

          (*pTOpt).SpecialOdf[id] = calloc ((*pTOpt).tarOdf.odfqty, sizeof (struct FCT));
          for (j = 0; j < (*pTOpt).tarOdf.odfqty; j++)
          {
            ut_fct_set_zero ((*pTOpt).SpecialOdf[id] + j);
            (*pTOpt).SpecialOdf[id][j].size = thetaqty;
            (*pTOpt).SpecialOdf[id][j].x = ut_alloc_1d ((*pTOpt).SpecialOdf[id][j].size);
            (*pTOpt).SpecialOdf[id][j].y = ut_alloc_1d ((*pTOpt).SpecialOdf[id][j].size);
            ut_string_string ("numerical", &((*pTOpt).SpecialOdf[id][j].type));
          }
          for (k = 0; k < thetaqty; k++)
          {
            sprintf (filename, "%s/%d", dir, thetas[k]);
            ut_array_1d_fnscanf (filename, tmp, (*pTOpt).tarOdf.odfqty, "R");
            for (j = 0; j < (*pTOpt).tarOdf.odfqty; j++)
            {
              (*pTOpt).SpecialOdf[id][j].x[k] = thetas[k] * M_PI / 180;
              (*pTOpt).SpecialOdf[id][j].y[k] = tmp[j];
            }
          }
          ut_free_1d_int (&thetas);
          ut_free_2d_char (&files, thetaqty);
          ut_free_1d_char (&filename);
          ut_free_1d (&tmp);

          for (j = 0; j < (*pTOpt).tarOdf.odfqty; j++)
            ut_fct_init_interp ((*pTOpt).SpecialOdf[id] + j);
        }
        ut_free_1d_char (&label);
        ut_free_1d_char (&prev);
      }

      ut_free_1d_char (&dir);
    }
  }

  OSet = ol_set_alloc (1, (*pTOpt).SSet.crysym);
  OSet.weight = ut_alloc_1d (OSet.size);
  if ((*pTOpt).SSet.SeedOriTheta)
    OSet.theta = ut_alloc_1d (OSet.size);

  neut_odf_set_zero (&Odf);
  Odf.odfqty = (*pTOpt).tarOdf.Sp.Mesh[3].EltQty;
  Odf.odf = ut_alloc_1d (Odf.odfqty);

  for (i = 0; i < (*pTOpt).TDyn.seedmovedqty; i++)
  {
    seed = (*pTOpt).TDyn.seedmoved[i];

    OSet.weight[0] = 1;
    if ((*pTOpt).SSet.SeedOriTheta)
      OSet.theta[0] = (*pTOpt).SSet.SeedOriTheta[seed];
    ut_array_1d_memcpy ((*pTOpt).SSet.SeedOri[seed], 4, OSet.q[0]);

    if (ol_q_isspecial ((*pTOpt).SSet.SeedOri[seed]))
    {
      id = ol_q_specialid ((*pTOpt).SSet.SeedOri[seed]);
      if (id == UNIFORM)
        ut_array_1d_set ((*pTOpt).CellOdf[seed].odf, (*pTOpt).tarOdf.odfqty, 1);
      else
      {
        for (k = 0; k < (*pTOpt).CellOdf[seed].odfqty; k++)
          (*pTOpt).CellOdf[seed].odf[k] = ut_fct_eval ((*pTOpt).SpecialOdf[id][k], (*pTOpt).SSet.SeedOriTheta[seed]);
        ut_array_1d_scale ((*pTOpt).CellOdf[seed].odf, (*pTOpt).CellOdf[seed].odfqty,
                            1. / ut_array_1d_wmean ((*pTOpt).CellOdf[seed].odf, (*pTOpt).tarOdf.Sp.Mesh[3].EltWeight + 1,
                            (*pTOpt).CellOdf[seed].odfqty));
      }
    }
    else
      neut_odf_comp ("elt", "5", &OSet, (*pTOpt).CellOdf + seed, 0);
  }

  for (i = 1; i <= (*pTOpt).CellQty; i++)
    for (j = 0; j < Odf.odfqty; j++)
      Odf.odf[j] += (*pTOpt).SSet.SeedOriWeight[i] * (*pTOpt).CellOdf[i].odf[j];
  ut_array_1d_scale (Odf.odf, Odf.odfqty, 1. / ut_array_1d_wmean (Odf.odf, (*pTOpt).tarOdf.Sp.Mesh[3].EltWeight + 1, Odf.odfqty));

  net_tess_opt_comp_objective_fval_gen_odf_evaluate (pTOpt, Odf, var);

  ol_set_free (&OSet);

  neut_odf_free (&Odf);
  ut_free_1d_char (&data);

  return;
}
