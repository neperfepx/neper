/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_order_.h"

void
nem_order_init_3d (char *elttype, int ***pfir3, int ***psec3,
		   int *pnodeqty_3d1, int *pnodeqty_3d)
{
  *pnodeqty_3d1 = neut_elt_nodeqty (elttype, 3, 1);
  *pnodeqty_3d = neut_elt_nodeqty (elttype, 3, 2);

  (*pfir3) = ut_alloc_2d_int (*pnodeqty_3d, 2);
  (*psec3) = ut_alloc_2d_int (*pnodeqty_3d1, *pnodeqty_3d1);

  if (!strcmp (elttype, "tri"))
  {
/*----------------------------------------------------------------------
 * numbering:
 * nodes 0 1 2 3 at the vertices
 * 4: 0 - 1
 * 5: 1 - 2
 * 6: 2 - 0
 * 7: 3 - 0
 * 8: 2 - 3
 * 9: 3 - 1
 */
    (*psec3)[0][1] = (*psec3)[1][0] = 4;
    (*psec3)[1][2] = (*psec3)[2][1] = 5;
    (*psec3)[2][0] = (*psec3)[0][2] = 6;
    (*psec3)[3][0] = (*psec3)[0][3] = 7;
    (*psec3)[2][3] = (*psec3)[3][2] = 8;
    (*psec3)[3][1] = (*psec3)[1][3] = 9;

    (*pfir3)[4][0] = 0;
    (*pfir3)[4][1] = 1;
    (*pfir3)[5][0] = 1;
    (*pfir3)[5][1] = 2;
    (*pfir3)[6][0] = 2;
    (*pfir3)[6][1] = 0;
    (*pfir3)[7][0] = 3;
    (*pfir3)[7][1] = 0;
    (*pfir3)[8][0] = 2;
    (*pfir3)[8][1] = 3;
    (*pfir3)[9][0] = 3;
    (*pfir3)[9][1] = 1;
  }
  else if (!strcmp (elttype, "quad"))
  {
    (*psec3)[0][1] = (*psec3)[1][0] = 8;
    (*psec3)[4][5] = (*psec3)[5][4] = 16;
    (*psec3)[3][2] = (*psec3)[2][3] = 13;
    (*psec3)[7][6] = (*psec3)[6][7] = 19;

    (*psec3)[0][4] = (*psec3)[4][0] = 10;
    (*psec3)[1][5] = (*psec3)[5][1] = 12;
    (*psec3)[3][7] = (*psec3)[7][3] = 15;
    (*psec3)[2][6] = (*psec3)[6][2] = 14;

    (*psec3)[0][3] = (*psec3)[3][0] = 9;
    (*psec3)[1][2] = (*psec3)[2][1] = 11;
    (*psec3)[4][7] = (*psec3)[7][4] = 17;
    (*psec3)[5][6] = (*psec3)[6][5] = 18;

    (*pfir3)[8][0] = 0;
    (*pfir3)[8][1] = 1;
    (*pfir3)[9][0] = 0;
    (*pfir3)[9][1] = 3;
    (*pfir3)[10][0] = 0;
    (*pfir3)[10][1] = 4;
    (*pfir3)[11][0] = 1;
    (*pfir3)[11][1] = 2;
    (*pfir3)[12][0] = 1;
    (*pfir3)[12][1] = 5;
    (*pfir3)[13][0] = 2;
    (*pfir3)[13][1] = 3;
    (*pfir3)[14][0] = 2;
    (*pfir3)[14][1] = 6;
    (*pfir3)[15][0] = 3;
    (*pfir3)[15][1] = 7;
    (*pfir3)[16][0] = 4;
    (*pfir3)[16][1] = 5;
    (*pfir3)[17][0] = 4;
    (*pfir3)[17][1] = 7;
    (*pfir3)[18][0] = 5;
    (*pfir3)[18][1] = 6;
    (*pfir3)[19][0] = 6;
    (*pfir3)[19][1] = 7;
  }

  return;
}

void
nem_order_init_2d (char *elttype, int ***pfir2, int ***psec2,
		   int *pnodeqty_2d1, int *pnodeqty_2d)
{
  *pnodeqty_2d1 = neut_elt_nodeqty (elttype, 2, 1);
  *pnodeqty_2d = neut_elt_nodeqty (elttype, 2, 2);

  (*pfir2) = ut_alloc_2d_int (*pnodeqty_2d, 2);
  (*psec2) = ut_alloc_2d_int (*pnodeqty_2d1, *pnodeqty_2d1);

  if (!strcmp (elttype, "tri"))
  {
    (*psec2)[0][1] = (*psec2)[1][0] = 3;
    (*psec2)[1][2] = (*psec2)[2][1] = 4;
    (*psec2)[2][0] = (*psec2)[0][2] = 5;

    (*pfir2)[3][0] = 0;
    (*pfir2)[3][1] = 1;
    (*pfir2)[4][0] = 1;
    (*pfir2)[4][1] = 2;
    (*pfir2)[5][0] = 2;
    (*pfir2)[5][1] = 0;
  }
  else if (!strcmp (elttype, "quad"))
  {
    (*psec2)[0][1] = (*psec2)[1][0] = 4;
    (*psec2)[1][2] = (*psec2)[2][1] = 5;
    (*psec2)[2][3] = (*psec2)[3][2] = 6;
    (*psec2)[3][0] = (*psec2)[0][3] = 7;

    (*pfir2)[4][0] = 0;
    (*pfir2)[4][1] = 1;
    (*pfir2)[5][0] = 1;
    (*pfir2)[5][1] = 2;
    (*pfir2)[6][0] = 2;
    (*pfir2)[6][1] = 3;
    (*pfir2)[7][0] = 3;
    (*pfir2)[7][1] = 0;
  }
  else
    abort ();

  return;
}

void
nem_order_init_1d (char *elttype, int ***pfir1, int ***psec1,
		   int *pnodeqty_1d1, int *pnodeqty_1d)
{
  *pnodeqty_1d1 = neut_elt_nodeqty (elttype, 1, 1);
  *pnodeqty_1d = neut_elt_nodeqty (elttype, 1, 2);

  (*pfir1) = ut_alloc_2d_int (*pnodeqty_1d, 2);
  (*psec1) = ut_alloc_2d_int (*pnodeqty_1d1, *pnodeqty_1d1);

  (*psec1)[0][1] = (*psec1)[1][0] = 2;

  (*pfir1)[2][0] = 0;
  (*pfir1)[2][1] = 1;

  return;
}
