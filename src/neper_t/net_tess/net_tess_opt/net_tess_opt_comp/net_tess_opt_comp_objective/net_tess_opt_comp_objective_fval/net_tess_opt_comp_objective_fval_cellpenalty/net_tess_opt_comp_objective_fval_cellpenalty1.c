/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_comp_objective_fval_cellpenalty_.h"

void
net_tess_opt_comp_objective_fval_cellpenalty (struct TOPT *pTOpt)
{
  int i, j, cell, scell, faceqty, contiguous, faceqty_cell = 0, side;
  double dist;

  for (i = 0; i < (*pTOpt).cellchangedqty; i++)
  {
    cell = (*pTOpt).cellchanged[i];
    faceqty = INT_MAX;
    for (j = 0; j < (*pTOpt).CellSCellQty[cell]; j++)
    {
      scell = (*pTOpt).CellSCellList[cell][j];
      if ((*pTOpt).Poly[scell].FaceQty < faceqty)
      {
        faceqty = ((*pTOpt).Poly[scell]).FaceQty;
        faceqty_cell = scell;
      }
    }

    if (faceqty > 0)
    {
      int **setpoly = NULL;
      int *setpolyqty = NULL;
      int setqty;

      contiguous =
        neut_polys_contiguous ((*pTOpt).Poly, (*pTOpt).SSet.PerSeedSlaveQty,
                               (*pTOpt).SSet.PerSeedSlaveNb,
                               (*pTOpt).CellSCellList[cell],
                               (*pTOpt).CellSCellQty[cell], &setpoly,
                               &setpolyqty, &setqty);

      ut_free_1d_int (&setpolyqty);
      ut_free_2d_int (&setpoly, setqty);

      if (contiguous)
        (*pTOpt).curcellpenalty[cell] = 0;
      else
        (*pTOpt).curcellpenalty[cell] = 1;

      /*
      if (!neut_tesr_isvoid ((*pTOpt).tartesr) && contiguous)
      {
        int m;
        double **bbox = ut_alloc_2d (3, 2);
        int **posbbox = ut_alloc_2d_int (3, 2);

        neut_poly_bbox2 ((*pTOpt).Poly[cell], bbox);
        neut_tesr_bbox_posbbox ((*pTOpt).tartesr, bbox, posbbox);
        for (int k = 0; k < 3; k++)
        {
          posbbox[k][0] = ut_num_max (posbbox[k][0], 1);
          posbbox[k][1] = ut_num_min (posbbox[k][1], (*pTOpt).tartesr.size[k]);
        }

        int inter = 0;

#pragma omp parallel for private(m)
        for (m = posbbox[2][0]; m <= posbbox[2][1]; m++)
        {
#pragma omp flush (inter)
          if (!inter)
          {
            int k, l;
            int *pos = ut_alloc_1d_int (3);
            double *coo = ut_alloc_1d (3);

            for (l = posbbox[1][0]; l <= posbbox[1][1]; l++)
            {
              for (k = posbbox[0][0]; k <= posbbox[0][1]; k++)
              {
                ut_array_1d_int_set_3 (pos, k, l, m);

                if ((*pTOpt).tartesr.VoxCell[pos[0]][pos[1]][pos[2]])
                {
                  neut_tesr_pos_coo ((*pTOpt).tartesr, pos, coo);

                  if (neut_poly_point_in ((*pTOpt).Poly[cell], coo))
                  {
                    inter = 1;
#pragma omp flush (inter)
                  }
                }

                if (inter)
                  break;
              }

              if (inter)
                break;
            }

            ut_free_1d_int (&pos);
            ut_free_1d (&coo);
          }
        }

#pragma omp critical
        if (!inter)
        {
          (*pTOpt).curcellpenalty[cell] = 1;
          printf ("inter = %d penalize %d\n", inter, cell);
        }

        ut_free_2d (&bbox, 3);
        ut_free_2d_int (&posbbox, 3);
      }
      */

      // if cutting primitives are present, we make sure the seeds do not belong to them
      if (!neut_tesr_isvoid ((*pTOpt).tartesr))
        for (j = 0; j < (*pTOpt).PrimQty; j++)
        {
          if ((*pTOpt).CellSCellQty[cell] > 1)
            abort ();

          neut_prim_point_side ((*pTOpt).Prim[j], (*pTOpt).SSet.SeedCoo[cell], &side);

          if (side == -1)
            (*pTOpt).curcellpenalty[cell] = 1;
        }
    }

    else
    {
      (*pTOpt).curcellpenalty[cell] = 0;
      for (j = 0; j < (*pTOpt).CellSCellQty[cell]; j++)
      {
        scell = (*pTOpt).CellSCellList[cell][j];
        (*pTOpt).curcellpenalty[cell] +=
          neut_seedset_voidcell_distance ((*pTOpt).SSet, scell);
      }

      // In the case of a standard tessellation, the above distance may be
      // zero for a void cell, in the case where the seed is far from the
      // domain.  So, adding the distance to the domain as a penalty.
      if (strncmp (((*pTOpt).SSet).Type, "periodic", 8) != 0)
      {
        for (j = 0; j < (*pTOpt).CellSCellQty[cell]; j++)
        {
          scell = (*pTOpt).CellSCellList[cell][j];
          neut_poly_point_dist ((*pTOpt).DomPoly,
                                ((*pTOpt).SSet).SeedCoo[scell], &dist);
          (*pTOpt).curcellpenalty[cell] += dist;
        }
      }
    }

    if (faceqty == 0 && (*pTOpt).curcellpenalty[cell] == 0)
    {
      ut_space_polypts_point_dist_verbosity ((*pTOpt).DomPoly.VerCoo + 1,
                                             (*pTOpt).DomPoly.VerQty,
                                             (*pTOpt).SSet.
                                             SeedCoo[faceqty_cell], &dist, 0);
      ut_print_neperbug ();
    }
  }

  (*pTOpt).curcellpenaltyqty = 0;
  for (i = 1; i <= (*pTOpt).CellQty; i++)
    if ((*pTOpt).curcellpenalty[i] > 0)
      (*pTOpt).curcellpenaltyqty++;

  (*pTOpt).mincellpenaltyqty = ut_num_min ((*pTOpt).mincellpenaltyqty, (*pTOpt).curcellpenaltyqty);

  return;
}
