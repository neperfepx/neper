/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing_para_pl_.h"

/* The progression factors of the polys, faces and edges are set to -pl,
 * and then to -plpoly, -plface and -pledge, if specified (which can be
 * expressions involving "default", the -pl value).  The mesh size
 * increases from the boundaries to the interiors of the polys, faces and
 * edges (3D, 2D and 1D meshing), at rate ln (pl), so that the ratio
 * between the lengths of two adjacent elements is about pl. */
int
nem_meshing_para_pl (struct IN_M In, struct TESS *pTess,
                     struct MESHPARA *pMeshPara)
{
  int i, j, qty;
  char *entity[3] = { (char *) "poly", (char *) "face", (char *) "edge" };
  char *string[3] = { In.plpolystring, In.plfacestring, In.pledgestring };
  int entityqty[3] = { (*pTess).PolyQty, (*pTess).FaceQty, (*pTess).EdgeQty };
  double **ppl[3] = { &(*pMeshPara).poly_pl, &(*pMeshPara).face_pl,
                      &(*pMeshPara).edge_pl };

  for (i = 0; i < 3; i++)
  {
    qty = entityqty[i];

    (*ppl[i]) = ut_alloc_1d (qty + 1);
    ut_array_1d_set ((*ppl[i]) + 1, qty, In.pl);

    if ((*pMeshPara).input == 't' && qty > 0 && strcmp (string[i], "default"))
    {
      neut_tess_entity_expr_val (pTess, entity[i], string[i], *ppl[i], NULL);

      for (j = 1; j <= qty; j++)
        if ((*ppl[i])[j] < 1)
        {
          printf ("\n");
          ut_print_message (2, 3, "Option -pl%s: value of %s %d is lower than 1.\n",
                            entity[i], entity[i], j);
          abort ();
        }
    }
  }

  return 0;
}
