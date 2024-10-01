/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_.h"

void
net_reg_merge_print (int loop, double LengthRatio, int ThisDelQty, int edge,
                     int minid, double fmax, double minmaxff)
{
  if (ThisDelQty == 1)          /* 1 edge deleted */
  {
    ut_print_lineheader (0);
    printf ("[%d/%4.2f] %4d: ff=%5.1f....................... %4d deleted.\n",
            loop, LengthRatio, edge, fmax, edge);
  }
  else if (ThisDelQty == 2)     /* 2 edges deleted */
  {
    ut_print_lineheader (0);
    printf ("[%d/%4.2f] %4d: ff=%5.1f + %4d: ff=%5.1f...... %4d deleted.\n",
            loop, LengthRatio, edge, fmax, minid, minmaxff, edge);
    printf ("\t\t\t\t\t\t\t  %4d deleted.\n", minid);
  }
  else if (ThisDelQty == 0 && minid != 0)       /* none edge del. bcse ff too high */
  {
    ut_print_lineheader (0);
    printf ("[%d/%4.2f] %4d: ff=%5.1f + %4d: ff=%5.1f......  ff too high.\n",
            loop, LengthRatio, edge, fmax, minid, minmaxff);
  }
  else if (ThisDelQty == 0 && minid == 0)       /* none edge del. bcse no 2nd edge */
  {
    ut_print_lineheader (0);
    printf ("[%d/%4.2f] %4d: ff=%5.1f + none: ff=%5.1f......  ff too high.\n",
            loop, LengthRatio, edge, fmax, fmax);
  }
  else                          /* there is a pb */
    abort ();

  return;
}

int
net_reg_merge_nextedge (struct TESS Tess, struct REG Reg,
                        double *plengthratio)
{
  int i;                        /* mute variable */
  int edgenb = -1;              /* returned value */
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
