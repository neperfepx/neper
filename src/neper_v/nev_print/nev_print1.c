/* This fil40e is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_.h"

void
nev_print (struct IN_V In,
           struct PRINT *pPrint, struct SIM *pSim,
           struct TESS *pTess, struct DATA *TessData, struct TESR Tesr,
           struct DATA *pTesrData, struct NODES Nodes, struct MESH *Mesh,
           struct DATA *pNodeData, struct DATA **MeshData,
           struct DATA *pCsysData, struct POINT *Points, int PointQty,
           struct DATA *PointData)
{
  int status;
  char *basename = NULL;
  int SQty = 0;
  struct NODES *SNodes = NULL;
  struct MESH **SMesh = NULL;
  struct DATA *SNodeData = NULL;
  struct DATA ***SMeshData = NULL;
  int **SElt2dElt3d = NULL;
  int stepqty = 0, *steps = NULL, steppos, islaststep;
  struct PF Pf;

  neut_pf_set_zero (&Pf);

  ut_string_string (In.print, &basename);

  if (!strcmp (In.step, "final"))
  {
    stepqty = 1;
    steps = ut_alloc_1d_int (stepqty);
    steps[0] = (*pSim).StepQty;
  }
  else if (!strcmp (In.step, "all"))
  {
    stepqty = (*pSim).StepQty + 1;
    steps = ut_alloc_1d_int (stepqty);
    ut_array_1d_int_set_id (steps, stepqty);
  }
  else
    ut_list_break_int_range (In.step, NEUT_SEP_NODEP, &steps, &stepqty);

  for (steppos = 0; steppos < stepqty; steppos++)
  {
    islaststep = (steppos == stepqty - 1);

    status = neut_sim_setstep (pSim, steps[steppos]);

    if (status)
      ut_print_message (2, 2, "Step %d not available.\n", steps[steppos]);

    nev_print_data (In, *pSim, pTess, &Tesr, &Nodes, &Mesh, &Points,
                    PointQty, TessData, pTesrData, pNodeData, MeshData,
                    pCsysData, &PointData);

    nev_print_show (In, pTess, Tesr, Nodes, Mesh, Points, PointQty, pPrint);

    nev_print_init (In, pPrint, &Pf, *pTess, TessData, Tesr, pTesrData, Nodes, Mesh,
                    pNodeData, MeshData, &SQty, &SNodes, &SMesh,
                    &SNodeData, &SMeshData, &SElt2dElt3d, pCsysData, Points,
                    PointQty, PointData);

    if (!strcmp (In.space, "real") || !strcmp (In.space, "rodrigues"))
    {
      if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov")
       || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "pov:objects")
       || ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "png"))
        nev_print_png (In, basename, *pPrint, *pSim, *pTess, TessData, Tesr, pTesrData, Nodes,
                       Mesh, SQty, SNodes, SMesh, pNodeData, MeshData, pCsysData,
                       Points, PointQty, PointData, SNodeData, SMeshData, SElt2dElt3d);

      if (ut_list_testelt (In.imageformat, NEUT_SEP_NODEP, "vtk"))
        nev_print_vtk (In, basename, *pSim, Nodes, Mesh, pNodeData, MeshData);
    }

    else if (!strcmp (In.space, "pf") || !strcmp (In.space, "ipf"))
      nev_print_pf (In, &Pf, basename, *pPrint, steps, stepqty, steppos, *pSim,
                    *pTess, Tesr, TessData, pTesrData, Mesh, MeshData,
                    *pCsysData, Points, PointQty, PointData);

    else if (!strcmp (In.space, "tree") && islaststep)
      nev_print_tree (In, basename, *pSim);

    if ((*pPrint).showscale && islaststep)
      nev_print_scale (In, basename, *pSim, TessData, pTesrData, pNodeData,
                     MeshData, PointQty, PointData);
  }

  ut_free_1d_char (&basename);
  ut_free_1d_int (&steps);
  neut_pf_free (&Pf);

  return;
}
