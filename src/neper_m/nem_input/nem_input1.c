/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_input_.h"

void
nem_input (struct IN_M *pIn, int fargc, char **fargv, int argc, char **argv)
{
  ut_print_message (0, 2, "Reading arguments...\n");
  nem_input_treatargs (fargc, fargv, argc, argv, pIn);

  return;
}

void
nem_input_init_dim_tess (struct IN_M *pIn, struct TESS Tess)
{
  nem_input_init_dim_datadim (pIn, Tess.Dim);

  return;
}

void
nem_input_init_dim_tesr (struct IN_M *pIn, struct TESR Tesr)
{
  nem_input_init_dim_datadim (pIn, Tesr.Dim);

  return;
}

int
nem_input_init_dim_mesh (struct IN_M *pIn, struct MESH *Mesh)
{
  int datadim;

  datadim = neut_mesh_array_dim (Mesh);

  if (datadim == -1)
    return -1;

  nem_input_init_dim_datadim (pIn, datadim);

  return 0;
}

void
nem_input_init_dim_datadim (struct IN_M *pIn, int datadim)
{
  int i;

  if (strstr ((*pIn).dimstring, "."))
    ut_print_message (2, 2,
                      "Failed to process expression `%s' (option -dim).\n",
                      (*pIn).dimstring);

  if (ut_list_testelt ((*pIn).dimstring, NEUT_SEP_NODEP, "none"))
  {
    (*pIn).dim = -1;
    (*pIn).dimout = ut_alloc_1d_char (1);
    (*pIn).dimout_msh = ut_alloc_1d_char (1);
    return;
  }
  else if (ut_list_testelt ((*pIn).dimstring, NEUT_SEP_NODEP, "all"))
  {
    (*pIn).dim = datadim;
    (*pIn).dimout = ut_alloc_1d_char (10);
    (*pIn).dimout_msh = ut_alloc_1d_char (10);
    strcpy ((*pIn).dimout, "0");
    for (i = 1; i <= datadim; i++)
      sprintf ((*pIn).dimout + strlen ((*pIn).dimout), ",%d", i);
    strcpy ((*pIn).dimout_msh, (*pIn).dimout);
  }
  else
  {
    int dim, qty, qty0;
    char **list = NULL;
    char **list0 = NULL;
    char *string = ut_alloc_1d_char (strlen ((*pIn).dimstring) + 1);
    char *datadimstring = ut_alloc_1d_char (2);
    sprintf (datadimstring, "%d", datadim);

    strcpy (string, (*pIn).dimstring);
    ut_string_fnrs (string, "inputdim", datadimstring, INT_MAX);

    // Setting dim
    ut_list_break (string, NEUT_SEP_DEP, &list0, &qty0);
    ut_list_break (list0[0], NEUT_SEP_NODEP, &list, &qty);

    (*pIn).dim = -1;
    (*pIn).dimout = ut_alloc_1d_char (strlen (string) + 1);
    (*pIn).dimout_msh = ut_alloc_1d_char (100);
    for (i = 0; i < qty; i++)
    {
      ut_string_int (list[i], &dim);

      (*pIn).dim = ut_num_max ((*pIn).dim, dim);
      sprintf ((*pIn).dimout + strlen ((*pIn).dimout), "%s%d",
               (strlen ((*pIn).dimout) == 0) ? "" : NEUT_SEP_NODEP, dim);
    }

    // dimout_msh

    if (qty0 == 2 && !strcmp (list0[1], "msh"))
      strcpy ((*pIn).dimout_msh, (*pIn).dimout);
    else
      for (i = 0; i <= (*pIn).dim; i++)
        sprintf ((*pIn).dimout_msh + strlen ((*pIn).dimout_msh), "%s%d",
                 (strlen ((*pIn).dimout_msh) == 0) ? "" : NEUT_SEP_NODEP, i);

    ut_free_2d_char (&list0, qty0);
    ut_free_2d_char (&list, qty);
    ut_free_1d_char (&string);
    ut_free_1d_char (&datadimstring);
  }

  return;
}

int
nem_input_outputismesh (struct IN_M In)
{
  if (strstr (In.format, "msh") || strstr (In.format, "geof")
      || strstr (In.format, "inp") || strstr (In.format, "vtk"))
    return 1;
  else
    return 0;
}
