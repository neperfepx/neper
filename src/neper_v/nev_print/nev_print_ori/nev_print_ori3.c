/* This id is part of the 'hermes' program. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYINORIG id in the top-level directory. */

#include"nev_print_ori_.h"

void
nev_print_ori_file_data_ori (int input, struct ORI *pOri, double **q,
                            double *weights, int qty, int *show,
                            double ***ppts, double **pptwgts,
                            int **pptcells, int *pptqty)
{
  int i, j;
  struct OL_SET Set;
  double *qref = ol_q_alloc ();

  (void) input;

  Set = ol_set_alloc (qty, (*pOri).crysym);

  for (i = 0; i < qty; i++)
  {
    ol_q_refcsys (q[i], qref, Set.q[i]);
    Set.weight[i] = weights ? weights[i] : 1;
  }

  /*
  else if (! strcmp (InORI.SamplingType, "rel"))
    ol_set_fscanf_sample (in, InORI.Sampling[id], &Set, NULL);
  else if (! strcmp (InORI.SamplingType, "abs"))
    ol_set_fscanf_sample_nb (in, InORI.Sampling[id], &Set, NULL);
  else
    abort ();
  */

  (*ppts) = ut_alloc_2d (1, 3);
  (*pptcells) = ut_alloc_1d_int (1);
  (*pptwgts) = ut_alloc_1d (1);

  (*pptqty) = 0;
  for (i = 0; i < (int) Set.size; i++)
    if (!show || show[i])
    {
      if (!strncmp ((*pOri).space, "rodrigues", 9))
      {

        // special orientation (fiber)
        if (ol_q_isspecial (Set.q[i]))
        {
          struct OL_SET OSet;
          double *R = ol_R_alloc ();

          ol_q_special_set (Set.q[i], &OSet);

          for (j = 0; j < (int) OSet.size; j++)
          {
            double **Rs = NULL;
            int Rqty, k;

            ol_q_R (OSet.q[j], R);

            neut_ori_R_Rcrysym_all (*pOri, R, &Rs, &Rqty);

            for (k = 0; k < Rqty; k++)
            {
              (*pptqty)++;
              (*ppts) = ut_realloc_2d_addline (*ppts, *pptqty, 3);
              (*pptcells) = ut_realloc_1d_int (*pptcells, *pptqty);
              (*pptwgts) = ut_realloc_1d (*pptwgts, *pptqty);

              ut_array_1d_memcpy (Rs[k], 3, (*ppts)[(*pptqty - 1)]);

              (*pptcells)[(*pptqty) - 1] = i + 1;
              (*pptwgts)[(*pptqty) - 1] = Set.weight[i] / OSet.size;
            }
            ut_free_2d (&Rs, Rqty);
          }

          ol_R_free (R);
          ol_set_free (&OSet);
        }

        // standard orientation
        else
        {
          (*ppts) = ut_realloc_2d_addline (*ppts, (*pptqty) + 1, 3);
          (*pptcells) = ut_realloc_1d_int (*pptcells, (*pptqty) + 1);
          (*pptwgts) = ut_realloc_1d (*pptwgts, (*pptqty) + 1);

          ol_q_R (Set.q[i], (*ppts)[*pptqty]);
          ol_R_Rcrysym ((*ppts)[*pptqty], (*pOri).crysym, (*ppts)[*pptqty]);

          (*pptcells)[(*pptqty)] = i + 1;
          (*pptwgts)[(*pptqty)] = Set.weight[i];
          (*pptqty)++;
        }
      }
    }

  ol_q_free (qref);

  return;
}

void
nev_print_ori_ptsprint_symbols (struct IN_V In, struct PRINT Print, int input, struct ORI *pOri,
                               FILE *file, struct DATA Data, double **pts,
                               int *ptcells, int ptqty)
{
  int i;
  struct POINT Point;
  struct DATA PointData;
  int *show = NULL;

  (void) In;
  (void) input;
  (void) pOri;

  neut_data_set_default (&PointData);

  PointData.Qty = ptqty;
  PointData.Rad = ut_alloc_1d (PointData.Qty + 1);
  PointData.Col = ut_alloc_2d_int (PointData.Qty + 1, 3);
  PointData.Coo = ut_alloc_2d (PointData.Qty + 1, 3);
  PointData.Trs = ut_alloc_1d (PointData.Qty + 1);
  ut_string_string (Data.RadDataType, &PointData.RadDataType);
  PointData.SymbolDataType = NULL;

  neut_point_set_zero (&Point);

  Point.Dim = 3;

  for (i = 0; i < ptqty; i++)
  {
    int cell = ptcells[i];
    char *symbol = ut_alloc_1d_char (20);
    double *params = ut_alloc_1d (20);

    nev_print_symbol_expr_params (Data.Rad[cell], Data.Symbol[cell], symbol, params);
    neut_point_addpoint (&Point, pts[i], params[0]);

    ut_array_1d_memcpy (pts[i], 3, PointData.Coo[i + 1]);
    ut_array_1d_int_memcpy (Data.Col[cell], 3, PointData.Col[i + 1]);
    PointData.Rad[i + 1] = Data.Rad[cell];
    PointData.Trs[i + 1] = Data.Trs[cell];

    ut_free_1d_char (&symbol);
    ut_free_1d (&params);
  }

  show = ut_alloc_1d_int (Point.Qty + 1);
  for (i = 1; i <= Point.Qty; i++)
    show[i] = 1;

  nev_print_real_png_point (file, Point, PointData, Print, show);

  ut_free_1d_int (&show);
  neut_data_free (&PointData);

  return;
}
