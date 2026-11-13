/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_.h"

void
net_reg_merge_print (int loop, double LengthRatio, int ThisDelQty, int edge,
                     int minid, double fmax, double minmaxff)
{
  ut_print_lineheader (0);

  if (ThisDelQty == 1)          /* 1 edge deleted */
    printf ("[%d/%4.2f] %4d: ff=%5.1f....................... %4d deleted.\n",
            loop, LengthRatio, edge, fmax, edge);
  else if (ThisDelQty >= 2)     /* several edges deleted */
    printf ("[%d/%4.2f] %4d: ff=%5.1f + %4d...: ff=%5.1f... %d deleted.\n",
            loop, LengthRatio, edge, fmax, minid, minmaxff, ThisDelQty);
  else                          /* no edge deleted */
    printf ("[%d/%4.2f] %4d: ff=%5.1f...................... not deleted.\n",
            loop, LengthRatio, edge, fmax);

  return;
}

int
net_reg_merge_nextedge (struct TESS Tess, struct REG Reg,
                        double *plengthratio)
{
  int i;                        /* mute variable */
  int edgenb = -1;              /* returned value */
  int master, edgeqty, *edges = NULL, **offsets = NULL;
  double lratio;

  (*plengthratio) = 1.;

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] == 0 && Tess.EdgeDel[i] == 0)
    {
      lratio = Tess.EdgeLength[i] / Reg.EdgeSel[i];

      if (lratio < *plengthratio)
      {
        (*plengthratio) = lratio;
        edgenb = i;
      }
    }

  // periodic tessellation: the copies of an edge have the same length up
  // to round-off errors, so the master edge (copy of lowest id, computed
  // from the current topology) is returned, for determinism
  if (edgenb != -1 && !strcmp (Tess.Type, "periodic"))
  {
    neut_tess_edge_copies (Tess, edgenb, &edges, &offsets, &edgeqty);

    master = ut_array_1d_int_min (edges, edgeqty);
    if (master != edgenb && Tess.EdgeState[master] == 0
        && Tess.EdgeDel[master] == 0)
    {
      edgenb = master;
      (*plengthratio) = Tess.EdgeLength[edgenb] / Reg.EdgeSel[edgenb];
    }

    ut_free_1d_int (&edges);
    ut_free_2d_int (&offsets, edgeqty);
  }

  return edgenb;
}

/*
   net_reg_merge_forcemerge deletes the edges specified manually
void net_reg_merge_forcemerge (struct TESS* pTess,struct REG Reg)
{
    int i;

    for(i=1;i<=Reg.forcemergeqty;i++)
	net_reg_merge_del(pTess,Reg.forcemerge[i]);

    return;
}
*/

// Allocating the periodicity of the vertices of a buffer tessellation, in
// the case of a periodic tessellation
void
net_reg_merge_buffer_per (struct TESS Tess, struct TESS *pTessBuf)
{
  if (!Tess.PerVerMaster)
    return;

  (*pTessBuf).PerVerMaster = ut_alloc_1d_int (Tess.VerQty + 1);
  (*pTessBuf).PerVerShift = ut_alloc_2d_int (Tess.VerQty + 1, 3);
  (*pTessBuf).PerVerSlaveQty = ut_alloc_1d_int (Tess.VerQty + 1);
  (*pTessBuf).PerVerSlaveNb = ut_alloc_1d_pint (Tess.VerQty + 1);

  return;
}
