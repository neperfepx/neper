/* Copyright (C) 2003-2017 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<math.h>
#include "ut.h"

int
oneDIntEltPos (int *array, int beg, int end, int value)
{
  int i;
  int pos = -1;

  for (i = beg; i <= end; i++)
    if (array[i] == value)
    {
      pos = i;
      break;
    }

  if (pos != -1)
    return pos;
  else
    return -1;
}

int
oneDIntCompress (int *array, int beg, int end)
{
  int i;
  int end2;
  int *array2;

  array2 = ut_alloc_1d_int (end + 1);

  end2 = beg - 1;
  for (i = beg; i <= end - 1; i++)
    if (array[i] != array[i + 1])
    {
      end2++;
      array2[end2] = i;
    }
  end2++;
  array2[end2] = end;

  for (i = beg; i <= end2; i++)
    array[i] = array[array2[i]];
  for (i = end2 + 1; i <= end; i++)
    array[i] = 0;

  ut_free_1d_int (array2);

  return end2;
}

int
oneDIntDeleteNCompress (int *array, int beg, int end, int value, int loop)
{
  int l;
  int i;
  int qty = 0;
  int pos;

  for (l = 1; l <= loop; l++)
  {
    pos = oneDIntEltPos (array, beg, end, value);
    if (pos != -1)
    {
      for (i = pos; i <= end - 1; i++)
	array[i] = array[i + 1];
      qty++;
    }
    else
      break;
  }

  return qty;
}
