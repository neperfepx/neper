/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_t_.h"

int
neper_t (int fargc, char **fargv, int argc, char **argv)
{
  // ###################################################################
  // ### INITIALIZING ##################################################

  // Variable declaration ###
  int i, status, tess, level, ParentTessId, SSetQty;
  struct IN_T In;
  struct REG Reg;
  struct MTESS MTess;
  struct TESS *Tess = NULL;
  struct TESS FTess;
  struct SEEDSET *SSet = NULL;
  struct TESR Tesr;
  struct POINT Point;
  // struct ODF Odf;

  net_in_set_zero (&In);
  net_reg_set_zero (&Reg);
  neut_tesr_set_zero (&Tesr);
  neut_mtess_set_zero (&MTess);
  neut_tess_set_zero (&FTess);
  neut_point_set_zero (&Point);
  SSetQty = 0;
  SSet = realloc (SSet, sizeof (struct SEEDSET));
  neut_seedset_set_zero (SSet);

  // Printing program header ###
  ut_print_moduleheader ("-T", fargc, fargv, argc, argv);

  // Reading input data ###
  ut_print_message (0, 1, "Reading input data...\n");
  net_input (&In, fargc, fargv, argc, argv);

  // ###################################################################
  // ### CREATING DOMAIN ###############################################
  if (!strcmp (In.load, "none"))
  {
    // Creating domain ###
    neut_mtess_set_dom (&MTess, &Tess);
    if (!strcmp (In.mode, "tess") && strcmp (In.input, "tess") != 0
        && strcmp (In.input, "tesr") != 0)
    {
      ut_print_message (0, 1, "Creating domain...\n");
      net_domain (In, &MTess, Tess);
    }
  }

  // ###################################################################
  // ### RUNNING MULTILEVEL TESSELLATION ###############################
  // ###################################################################

  if (!strcmp (In.mode, "tess"))
  {
    if (In.checktess == 1)
    {
      ut_print_message (0, 1, "Checking tessellation...\n");
      Tess = calloc (2, sizeof (struct TESS));
      neut_tess_fnscanf (In.load, &(Tess[1]));
      return 0;
    }

    else if (In.levelqty >= 1)
    {
      MTess.LevelQty = In.levelqty;
      MTess.LevelTessQty =
        ut_realloc_1d_int (MTess.LevelTessQty, In.levelqty + 1);
      for (i = 1; i <= In.levelqty; i++)
        MTess.LevelTess =
          ut_realloc_2d_int_addline (MTess.LevelTess, i + 1, 2);

      ut_print_message (0, 1, "Creating tessellation%s...\n",
                        (In.levelqty <= 1) ? "" : "s");

      if (In.levelqty > 1)
        ut_print_message (0, 1,
                          "  [level] Running in multiscale mode (%d scales)...\n",
                          In.levelqty);

      int cell, tessqty, level_cellqty, prevlevel_cellqty;

      prevlevel_cellqty = 1;
      for (level = 1; level <= In.levelqty; level++)
      {
        for (tess = 1; tess <= MTess.LevelTessQty[level - 1]; tess++)
          ParentTessId = MTess.LevelTess[level - 1][tess];

        MTess.LevelTessQty[level] = 0;

        // ###############################################################
        // ### COMPUTING TESSELLATIONS ###################################

        tessqty = 0;
        level_cellqty = 0;
        for (tess = 1; tess <= MTess.LevelTessQty[level - 1]; tess++)
        {
          ParentTessId = MTess.LevelTess[level - 1][tess];

          for (cell = 1; cell <= Tess[ParentTessId].CellQty; cell++)
          {
            MTess.LevelTessQty[level]++;

            MTess.LevelTess[level] =
              ut_realloc_1d_int (MTess.LevelTess[level],
                                 MTess.LevelTessQty[level] + 1);
            MTess.LevelTess[level][MTess.LevelTessQty[level]] =
              ++MTess.TessQty;

            MTess.DomTess[ParentTessId][cell] = MTess.TessQty;

            MTess.TessDom =
              ut_realloc_2d_int_addline (MTess.TessDom, MTess.TessQty + 1, 2);
            MTess.TessDom[MTess.TessQty][0] = ParentTessId;
            MTess.TessDom[MTess.TessQty][1] = cell;

            Tess = realloc (Tess, (MTess.TessQty + 1) * sizeof (struct TESS));
            neut_tess_set_zero (Tess + MTess.TessQty);

            SSetQty = MTess.TessQty;
            SSet = realloc (SSet, (SSetQty + 1) * sizeof (struct SEEDSET));
            neut_seedset_set_zero (SSet + SSetQty);

            // ### COMPUTING TESSELLATION ###############################

            if (In.levelqty == 1)
            {
            }
            else
            {
              ut_print_clearline (stdout, 72);
              ut_print_message (0, 1, "  [%5d]  %d/%d ", level, ++tessqty,
                                prevlevel_cellqty);
            }

            /*
               net_seed (In, MTess, Tess, ParentTessId, cell, MTess.TessQty,
               &SSet);
             */

            status =
              net_tess (In, level, Tess, ParentTessId, cell, SSet,
                        MTess.TessQty, &MTess);

            if (status != 0)
              ut_print_message (2, 2, "Tessellation failed.\n");

            if (Tess[MTess.TessQty].CellId && ut_array_1d_int_duplicates
                (Tess[MTess.TessQty].CellId + 1, Tess[MTess.TessQty].CellQty))
            {
              printf ("\n");
              ut_print_message (0, 2, "Merging cells with same ids...");
              neut_tess_merge (&(Tess[MTess.TessQty]));
            }

            MTess.DomTess =
              ut_realloc_2d_int_addline (MTess.DomTess, MTess.TessQty + 1,
                                         Tess[MTess.TessQty].PolyQty + 1);

            MTess.DomTessFaceNb =
              ut_realloc_3d_int_addarray (MTess.DomTessFaceNb,
                                          MTess.TessQty + 1,
                                          Tess[MTess.TessQty].FaceQty + 1, 2);

            level_cellqty += SSet[MTess.TessQty].N;
          }
        }

        prevlevel_cellqty = level_cellqty;

        // Sorting cells ###
        if (level == 1 && strcmp (In.sortstring, "none"))
        {
          printf ("\n");
          if (MTess.LevelQty > 1)
            ut_print_message (0, 1, "[%5d] ", level);
          else
            ut_print_message (0, 2, "", level);
          printf ("Sorting cells...");
          if ((Tess[1]).CellQty > 0)
            net_sort (In, Tess + 1, NULL);
        }

        /*
           if (strcmp (In.scalestring, "none") && level == 1)
           {
           ut_print_message (0, 1, "[%5d] ", level);
           printf ("Scaling tessellation...\n");
           neut_tess_scale (Tess + 1, In.scale[0], In.scale[1], In.scale[2]);
           }
         */
      }

      // #############################################################
    }

    else if (strcmp (In.load, "none"))
    {
      ut_print_message (0, 1, "Importing tessellation...\n");
      neut_mtess_set_tess (&MTess, &Tess);
      neut_tess_fnscanf (In.load, Tess + 1);

      // Sorting cells ###
      if (strcmp (In.sortstring, "none"))
      {
        ut_print_message (0, 1, "Sorting cells...\n");
        net_sort (In, Tess + 1, NULL);
      }

      /*
         if (strcmp (In.scalestring, "none"))
         {
         ut_print_message (0, 1, "Scaling tessellation...\n");
         neut_tess_scale (Tess + 1, In.scale[0], In.scale[1], In.scale[2]);
         }
       */
    }

    if (In.levelqty > 1)
    {
      ut_print_message (0, 1, "Flattening %d-scale tessellation...\n",
                        MTess.LevelQty);
      net_flatten (MTess, Tess, &FTess);

      if (In.dim == 2)
        net_dim (2, &FTess);
    }
    else if (In.levelqty == 1 || strcmp (In.load, "none"))
      neut_tess_tess (Tess[1], &FTess);

    neut_ori_des_expand (In.orides, &FTess.CellOriDes);
  }

  // ###################################################################
  // ### COMPUTING VOXEL DATA ##########################################

  else if (!strcmp (In.mode, "tesr"))
  {
    if (!strcmp (In.load, "none"))
      ut_print_neperbug ();

    ut_print_message (0, 1, "Importing raster tessellation...\n");
    neut_tesr_fnscanf (In.load, &Tesr);
    ut_print_message (0, 2, "Volume has %dx%dx%d (%g) %s and %d %s.\n",
                      Tesr.size[0], Tesr.size[1], Tesr.size[2],
                      (double) (Tesr.size[0] * Tesr.size[1] * Tesr.size[2]),
                      (Tesr.size[0] * Tesr.size[1] * Tesr.size[2] <
                       2) ? "point" : "points", Tesr.CellQty,
                      (Tesr.CellQty < 2) ? "cell" : "cells");

    if (Tesr.VoxCell)
    {
      int i, j, k;
      int min_id = INT_MAX, max_id = INT_MIN;
      for (k = 1; k <= Tesr.size[2]; k++)
        for (j = 1; j <= Tesr.size[1]; j++)
          for (i = 1; i <= Tesr.size[0]; i++)
          {
            min_id = ut_num_min (min_id, Tesr.VoxCell[i][j][k]);
            max_id = ut_num_max (max_id, Tesr.VoxCell[i][j][k]);
          }

      if ((min_id == 0 || min_id == 1) && max_id == Tesr.CellQty)
      {
        ut_print_message (0, 3,
                          "The cells are numbered contiguously from 1.\n");
        if (min_id == 0)
          ut_print_message (0, 3, "Some voxels are empty.\n");
      }
      else
        ut_print_message (1, 3, "The cell ids range between %d and %d.\n",
                          min_id, max_id);
    }

    neut_ori_des_expand (In.orides, &Tesr.CellOriDes);
  }

  // ###################################################################
  // ### COMPUTING ORIENTATION DATA ####################################

  else if (!strcmp (In.mode, "ori"))
  {
    ut_print_message (0, 1, "Generating crystal orientations...\n");

    net_ori_pre (In, MTess, Tess, &SSet, &SSetQty);

    net_ori (In, 1, MTess, Tess, SSet, 0, 1, SSet + 1, 2);

    net_seedset_tess (SSet[1], &FTess);

    neut_ori_des_expand (In.orides, &FTess.CellOriDes);
  }

  // #################################################################
  // ### Rastering tessellation if necessary ########################

  if (MTess.TessQty > 0
      && (ut_list_testelt (In.format, NEUT_SEP_NODEP, "tesr") == 1
          || ut_list_testelt (In.format, NEUT_SEP_NODEP, "raw") == 1
          || ut_list_testelt (In.format, NEUT_SEP_NODEP, "vtk") == 1))
  {
    ut_print_message (0, 1, "Rasterizing tessellation...\n");
    net_tess_tesr (In.tesrsizestring, FTess, &Tesr);

    if (neut_tesr_hascellorispread (Tesr))
      net_ori_post (&Tesr);
  }

  // ###################################################################
  // ### TRANSFORMING TESSELLATION #####################################

  if (strcmp (In.transform, "none") && (FTess.Dim > 0 || Tesr.Dim > 0))
  {
    ut_print_message (0, 1, "Transforming tessellation...\n");
    net_transform (In, Tess, &FTess, &Tesr);
  }

  if (!strcmp (In.mode, "tess") && In.reg)
  {
    ut_print_message (0, 1, "Regularizing tessellation...\n");
    net_reg (In, &FTess, &Reg);
  }

  if (In.group)
  {
    ut_print_message (0, 1, "Computing cell groups...\n");
    net_group (In, &FTess, &Tesr);
  }

  // ###################################################################
  // ### IMPORTING POINTS ##############################################

  if (strcmp (In.loadpoint, "none"))
  {
    ut_print_message (0, 1, "Importing points...\n");
    neut_point_fnscanf (In.loadpoint, &Point);
  }

  // ###################################################################
  // ### WRITING RESULTS ###############################################

  ut_print_message (0, 1, "Writing results...\n");
  net_res (In, FTess, Tesr);

  // ###################################################################
  // ### WRITING STATISTICS  ###########################################

  if (In.stc || In.stv || In.ste || In.stf || In.stp || In.sts || In.stpt
      || In.stvox || In.sttess || In.sttesr || In.stgroup)
  {
    ut_print_message (0, 1, "Writing statistics...\n");
    if (!strcmp (In.load, "none") && !ut_list_testelt (In.format, NEUT_SEP_NODEP, "tess"))
      neut_tess_reset (&FTess);
    net_stat (In, &FTess, &Tesr, Point);
  }

  // ###################################################################
  // ### CLOSING #######################################################

  net_reg_free (Reg);
  neut_tesr_free (&Tesr);
  neut_point_free (&Point);
  neut_tess_free (&FTess);

  for (i = 1; i <= SSetQty; i++)
    neut_seedset_free (&(SSet[i]));
  free (SSet);

  neut_mtess_free (&MTess, &Tess);
  net_in_free (&In);

  return EXIT_SUCCESS;
}
