/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_vtk_.h"

void
nev_print_vtk_coodata (FILE *file, struct DATA Data)
{
  fprintf (file, "VECTORS %s double\n", Data.CooDataName);
  ut_array_2d_fprintf (file, Data.Coo + 1, Data.Qty, 3, "%g");

  return;
}

void
nev_print_vtk_coldata (FILE *file, struct DATA Data)
{
  int i;

  if (!ut_string_strcmp (Data.ColDataType, "int"))
  {
    fprintf (file, "SCALARS %s int\n", Data.ColDataName);
    fprintf (file, "LOOKUP_TABLE default\n");
    ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, 1, "%.0f");
  }
  else if (!ut_string_strcmp (Data.ColDataType, "real"))
  {
    fprintf (file, "SCALARS %s double\n", Data.ColDataName);
    fprintf (file, "LOOKUP_TABLE default\n");
    ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, 1, "%g");
  }
  else if (!ut_string_strcmp (Data.ColDataType, "vector"))
  {
    if (Data.ColDataSize == 3)
    {
      fprintf (file, "VECTORS %s double\n", Data.ColDataName);

      ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, Data.ColDataSize, "%g");
    }
    else
    {
      fprintf (file, "FIELD %s 1\n", Data.ColDataName);
      fprintf (file, "%s %d %d double\n", Data.ColDataName,
                                          Data.ColDataSize,
                                          Data.Qty);

      ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, Data.ColDataSize, "%g");
    }
  }
  else if (!ut_string_strcmp (Data.ColDataType, "tensor"))
  {
    fprintf (file, "TENSORS %s double\n", Data.ColDataName);

    if (Data.ColDataSize == 9) // all components
      ut_array_2d_fprintf (file, Data.ColData + 1, Data.Qty, Data.ColDataSize, "%g");

    else if (Data.ColDataSize == 6) // Voigt convention
      for (i = 1; i <= Data.Qty; i++)
        fprintf (file, "%g %g %g %g %g %g %g %g %g\n",
                 Data.ColData[i][0], Data.ColData[i][5],
                 Data.ColData[i][4], Data.ColData[i][5],
                 Data.ColData[i][1], Data.ColData[i][3],
                 Data.ColData[i][4], Data.ColData[i][3],
                 Data.ColData[i][2]);
  }

  return;
}
