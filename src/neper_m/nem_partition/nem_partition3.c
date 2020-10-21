/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_partition_.h"
#ifdef HAVE_LIBSCOTCH

void
nem_partition_writearch (FILE * file, int procqty, int proccoreqty, int dcore,
                         int dncor, int dproc)
{
  int i, j, ip;
  int coreqty = procqty * proccoreqty;

  fprintf (file, "deco 0\n");
  fprintf (file, "%d %d\n", coreqty, coreqty * 2 - 1);

  for (i = 0; i < coreqty; i++)
    fprintf (file, "%d 1 %d\n", i, i + coreqty);

  for (i = 1; i < coreqty; i++)
  {
    for (j = 1; j <= i; j++)
    {
      ip = proccoreqty * (i / proccoreqty);

      if (j <= ip)
        fprintf (file, "%d", dproc);
      else if (j < i)
        fprintf (file, "%d", dncor);
      else
        fprintf (file, "%d", dcore);

      fprintf (file, (j < i) ? " " : "\n");
    }
  }

  return;
}

void
nem_partition_elts_direct (struct MESH *Mesh, struct PART *pPart)
{
  int part, elt, dim = neut_mesh_array_dim (Mesh);
  int eltnodeqty =
    neut_elt_nodeqty ("tri", Mesh[dim].Dimension, Mesh[dim].EltOrder);
  int *eltnodepartqty = NULL, **eltnodeparts = NULL, **eltnodepartnodeqty = NULL;
  int **nodeparteltqty = NULL, ***nodepartelts = NULL;

  nem_partition_elts_direct_pre (Mesh, *pPart, &eltnodepartqty, &eltnodeparts,
                                 &eltnodepartnodeqty, &nodepartelts,
                                 &nodeparteltqty);

  nem_partition_elts_direct_recordinteriorelts (pPart, eltnodeqty,
                                                nodepartelts, nodeparteltqty);

  while (!nem_partition_elts_direct_findelt (Mesh[dim], *pPart, eltnodeparts, eltnodepartqty,
                                             eltnodepartnodeqty,
                                             nodepartelts, nodeparteltqty,
                                             eltnodeqty, &part, &elt))
    nem_partition_elts_direct_recordelt (pPart, part, elt, eltnodepartqty,
                                         eltnodeparts, eltnodepartnodeqty,
                                         nodepartelts, nodeparteltqty);

  ut_free_2d_int (&nodeparteltqty, (*pPart).qty);
  ut_free_3d_int (&nodepartelts, (*pPart).qty, eltnodeqty + 1);
  ut_free_1d_int (&eltnodepartqty);
  ut_free_2d_int (&eltnodeparts, Mesh[dim].EltQty + 1);
  ut_free_2d_int (&eltnodepartnodeqty, Mesh[dim].EltQty + 1);

  return;
}

void
nem_partition_elts_bynumber (struct MESH *Mesh, struct PART *pPart)
{
  int j;
  int dim = neut_mesh_array_dim (Mesh);

  for (j = 1; j <= Mesh[dim].EltQty; j++)
  {
    (*pPart).elt_parts[j] = (*pPart).qty * (j - 1) / Mesh[dim].EltQty;
    (*pPart).EltQty[(*pPart).elt_parts[j]]++;
  }

  return;
}

#endif
