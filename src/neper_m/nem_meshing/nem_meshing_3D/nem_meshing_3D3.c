/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_3D_.h"

void
nem_meshing_3D_poly_algo (struct IN_M In, double cl, struct MULTIM
			  *pMultim, int algo, struct timeval *pctrlc_t,
			  double *pallowed_t, double *pmax_elapsed_t,
			  struct TESS Tess, struct NODES Nodes,
			  struct MESH *Mesh, int poly,
			  struct NODES *pN, struct MESH *pM,
			  double *pmOsize, double *pelapsed_t)
{
  int iter, status = -1;
  double rnd;
  int var_qty = 2;
  char **vars = ut_alloc_2d_char (var_qty, 10);
  double *vals = ut_alloc_1d (var_qty);
  strcpy (vars[0], "Odis");
  strcpy (vars[1], "Osize");

  /* mesh (iterate if meshing fails) */
  for (iter = 0; iter < In.mesh3diter; iter++)
  {
    rnd = iter * 1.e-5;

    status = nem_mesh_3d_gmsh (Tess, poly, Nodes, Mesh, cl,
			       In.mesh3dclconv, In.gmsh, In.tmp,
			       (*pMultim).algos[algo][0],
			       (*pMultim).algos[algo][1],
			       rnd, *pallowed_t, pN,
			       pM, pmOsize, pctrlc_t, pelapsed_t);

    if (status == 0)		// success
    {
      (*pmax_elapsed_t) = ut_num_max ((*pmax_elapsed_t), *pelapsed_t);
      (*pallowed_t) =
	ut_num_min (In.mesh3dmaxtime, In.mesh3drmaxtime * (*pmax_elapsed_t));
      break;
    }
    if (status == 2)		// meshing succeeded, but skin mesh changed
      break;
  }

  // recording meshquality information in Multim
  if (!status)
  {
    neut_mesh_Odis (*pN, *pM, In.meshqualdisexpr,
		    &((*pMultim).mOdis[poly][algo]));
    (*pMultim).mOsize[poly][algo] = (*pmOsize);

    if (!strcmp (In.meshqualexpr, "Odis^0.8*Osize^0.2"))
      (*pMultim).mO[poly][algo] =
	pow ((*pMultim).mOdis[poly][algo], 0.8)
	* pow ((*pMultim).mOsize[poly][algo], 0.2);
    else
    {
      vals[0] = (*pMultim).mOdis[poly][algo];
      vals[1] = (*pMultim).mOsize[poly][algo];
      ut_math_eval (In.meshqualexpr, var_qty, vars, vals,
		    &((*pMultim).mO[poly][algo]));
    }

    if ((*pMultim).Oalgo[poly] == -1
	|| ((*pMultim).mO[poly][algo] >
	    (*pMultim).mO[poly][(*pMultim).Oalgo[poly]]))
    {
      (*pMultim).Oalgo[poly] = algo;
      (*pMultim).Odis[poly] = (*pMultim).mOdis[poly][(*pMultim).Oalgo[poly]];
      (*pMultim).Osize[poly] =
	(*pMultim).mOsize[poly][(*pMultim).Oalgo[poly]];
      (*pMultim).O[poly] = (*pMultim).mO[poly][(*pMultim).Oalgo[poly]];
    }

    if (In.mesh3dreport)
      nem_meshing_3D_report_poly_algo (poly, algo, *pN, *pM);
  }

  else
  {
    (*pMultim).mOdis[poly][algo] = 0;
    (*pMultim).mOsize[poly][algo] = 0;
    (*pMultim).mO[poly][algo] = 0;
  }

  ut_free_2d_char (vars, var_qty);
  ut_free_1d (vals);

  return;
}

void
nem_meshing_3D_report_poly (struct MULTIM Multim, int poly)
{
  int a;
  FILE *file = NULL;
  char *tmp = ut_alloc_1d_char (128);

  ut_sys_mkdirp ("neper-report");

  for (a = 0; a < Multim.algoqty; a++)
  {
    sprintf (tmp, "neper-report/poly%d-a%d-prop", poly, a);
    file = ut_file_open (tmp, "W");

    fprintf (file, "%f ", Multim.mOdis[poly][a]);
    fprintf (file, "%f ", Multim.mOsize[poly][a]);
    fprintf (file, "%f\n", Multim.mO[poly][a]);

    ut_file_close (file, tmp, "W");
  }

  sprintf (tmp, "neper-report/poly%d-prop", poly);
  file = ut_file_open (tmp, "W");

  fprintf (file, "%d %f %f %f\n", Multim.Oalgo[poly], Multim.Odis[poly],
	   Multim.Osize[poly], Multim.O[poly]);

  ut_file_close (file, tmp, "W");

  ut_free_1d_char (tmp);

  return;
}

void
nem_meshing_3D_report_poly_algo (int poly, int a,
				 struct NODES N, struct MESH M)
{
  int i;
  double rr, length;
  char *tmpstring = ut_alloc_1d_char (1000);
  sprintf (tmpstring, "neper-report/poly%d-a%d", poly, a);
  FILE *file = ut_file_open (tmpstring, "W");

  ut_sys_mkdirp ("neper-report");

  for (i = 1; i <= M.EltQty; i++)
  {
    neut_mesh_elt_rr (N, M, i, &rr);
    neut_mesh_elt_lengths (N, M, i, &length, NULL, NULL);
    fprintf (file, "%.5g %.5g\n", rr, length);
  }

  ut_file_close (file, tmpstring, "W");
  ut_free_1d_char (tmpstring);

  return;
}
