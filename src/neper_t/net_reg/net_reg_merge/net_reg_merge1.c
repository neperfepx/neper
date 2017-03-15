/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_reg_merge_.h"

int
net_reg_merge (struct TESS *pTess, struct REG Reg, int *pDelId)
{
  int i, loop, trash, testtess;
  int edge, edgeb, newver;
  double lratio1;
  double tmp;
  double fmax, minmaxff = 0;
  int minid = 0;
  int ThisDelQty, DelQty = 0;
  struct TESS TessBuf;
  struct TESS TessBuf2;
  double lratio2;
  int **buf = ut_alloc_2d_int (4, 100);
  int **buf2 = ut_alloc_2d_int (4, 100);

  int verbosity = 0;

  neut_tess_set_zero (&TessBuf);
  neut_tess_set_zero (&TessBuf2);

  neut_tess_alloc (&TessBuf, (*pTess).VerQty, (*pTess).EdgeQty,
		   (*pTess).FaceQty, (*pTess).PolyQty);
  neut_tess_alloc (&TessBuf2, (*pTess).VerQty, (*pTess).EdgeQty,
		   (*pTess).FaceQty, (*pTess).PolyQty);
  TessBuf.Dim = (*pTess).Dim;
  TessBuf2.Dim = (*pTess).Dim;

  char *string = ut_alloc_1d_char (100);
  char *prevstring = ut_alloc_1d_char (100);

  for (loop = 1; loop <= Reg.mloop; loop++)
  {
    edge = net_reg_merge_nextedge (*pTess, Reg, &lratio1);

    while (edge != -1)
    {
      if ((*pDelId) >= Reg.maxedgedelqty)
	break;

      ThisDelQty = 0;

      testtess = net_reg_merge_del (pTess, edge, &newver, &fmax, buf,
				    &TessBuf);

      if ((*pTess).Dim == 2)
      {
	if (testtess < 0)
	  net_reg_merge_undel (pTess, buf, TessBuf);
	else
	  ThisDelQty = 1;
      }

      else if ((*pTess).Dim == 3)
      {
	/* if (testtess == 0 and) fmax < max, the edge is deleted */
	/* else (fmax > max), we try with another edge */
	/* even if testtess == -1, the deletion continues:  we let the
	 * program a chance to find a suitable solution, and generally it
	 * does.
	 */

	/* if the max ff is acceptable, we record the modif into Tess
	 * else, we re-initialize it.
	 */
	if (testtess == 0 || testtess == -1)
	{
	  if (fmax < Reg.fmax)
	    ThisDelQty = 1;
	  else
	  {
	    if (verbosity >= 2)
	    {
	      printf ("newver = %d\n", newver);
	      printf ("Deleting neighbouring edges, in turn: %d: ",
		      (*pTess).VerEdgeQty[newver]);
	      for (i = 0; i <= (*pTess).VerEdgeQty[newver] - 1; i++)
		printf ("%d ", (*pTess).VerEdgeNb[newver][i]);
	      printf ("\n");
	    }

	    minmaxff = 180;
	    minid = 0;
	    for (i = 0; i < (*pTess).VerEdgeQty[newver]; i++)
	    {
	      edgeb = (*pTess).VerEdgeNb[newver][i];

	      lratio2 = (*pTess).EdgeLength[edgeb] / Reg.EdgeSel[edgeb];

	      int status = 0;
	      int ver1, ver2;
	      ver1 = (*pTess).EdgeVerNb[edgeb][0];
	      ver2 = (*pTess).EdgeVerNb[edgeb][1];

	      if ((*pTess).EdgeDom[edgeb][0] == 2)
	      {
		// means that the 2 vertices are on (different) domain edges
		// the edge would shrink to a third vertex (which is a domain
		// vertex), which is not handle properly.
		if ((*pTess).VerDom[ver1][0] == 1
		    && (*pTess).VerDom[ver2][0] == 1)
		  status = -1;
	      }
	      else if ((*pTess).EdgeDom[edgeb][0] == 1)
	      {
		if ((*pTess).VerDom[ver1][0] == 0
		    && (*pTess).VerDom[ver2][0] == 0)
		  status = -1;
	      }

	      if (status == 0 && lratio1 + lratio2 < 2)
	      {
		testtess = net_reg_merge_del (pTess, edgeb, &trash,
					      &tmp, buf2, &TessBuf2);
		net_reg_merge_undel (pTess, buf2, TessBuf2);
		net_reg_merge_undel_free (buf2, &TessBuf2);

		if (testtess == 0 && tmp < minmaxff)
		{
		  minmaxff = tmp;
		  minid = edgeb;
		}
	      }
	      else if (verbosity >= 3)
	      {
		ut_print_lineheader (0);
		printf ("edgedel %d=-1 --> skipping\n", edgeb);
	      }
	    }

	    if (minmaxff < Reg.fmax)
	    {
	      ThisDelQty = 2;

	      net_reg_merge_del (pTess, minid, &newver, &tmp, NULL, NULL);

	      // this node has been deleted: edgedel <- 1
	      (*pTess).EdgeDel[minid] = 1;
	    }
	    else
	    {
	      ThisDelQty = 0;
	      net_reg_merge_undel (pTess, buf, TessBuf);
	    }
	  }
	}
      }

      net_reg_merge_undel_free (buf, &TessBuf);

      /* in all cases, edgedel <- 1 */
      (*pTess).EdgeDel[edge] = 1;

      DelQty += ThisDelQty;
      (*pDelId) += ThisDelQty;

      if (verbosity >= 1)
	net_reg_merge_print (loop, lratio1, ThisDelQty, edge, minid, fmax,
			     minmaxff);

      /* Searching next edge to delete */
      edge = net_reg_merge_nextedge (*pTess, Reg, &lratio1);

      sprintf (string, "loop %d/%d: %%3.0f%%%% del=%d",
	       loop, Reg.mloop, DelQty);
      ut_print_progress_nonl (stdout, 100 * lratio1, 100, string, prevstring);
    }

    /* ForceMerge deletes the edges specified manually
     * net_reg_merge_forcemerge (pTess,Reg);
     */

    for (i = 1; i <= (*pTess).EdgeQty; i++)
      (*pTess).EdgeDel[i] = 0;

    if (DelQty == 0)
      break;
  }

  ut_free_2d_int (buf, 4);
  ut_free_2d_int (buf2, 4);
  neut_tess_free (&TessBuf);
  neut_tess_free (&TessBuf2);

  return DelQty;
}
