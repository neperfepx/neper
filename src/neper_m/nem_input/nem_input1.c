/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
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
nem_in_set_zero (struct IN_M *pIn)
{
  (*pIn).abq = NULL;
  (*pIn).body = NULL;
  (*pIn).elttype = NULL;
  (*pIn).faset = NULL;
  (*pIn).fepx1 = NULL;
  (*pIn).fepx2 = NULL;
  (*pIn).fepx3 = NULL;
  (*pIn).fepx4 = NULL;
  (*pIn).fepx5 = NULL;
  (*pIn).format = NULL;
  (*pIn).performat = NULL;
  (*pIn).geof = NULL;
  (*pIn).gmsh = NULL;
  (*pIn).tmp = NULL;
  (*pIn).outtess = NULL;
  (*pIn).loadmesh = NULL;
  (*pIn).loadpoint = NULL;
  (*pIn).mesh2dalgo = NULL;
  (*pIn).mesh3dalgo = NULL;
  (*pIn).meshface = NULL;
  (*pIn).meshpoly = NULL;
  (*pIn).interface = NULL;
  (*pIn).msh = NULL;
  (*pIn).per = NULL;
  (*pIn).nset = NULL;
  (*pIn).elset = NULL;
  (*pIn).partmethod = NULL;
  (*pIn).partstring = NULL;
  (*pIn).transform = NULL;
  (*pIn).stelt = ut_alloc_1d_pchar (5);
  (*pIn).stelset = ut_alloc_1d_pchar (5);
  (*pIn).stn = NULL;
  (*pIn).stpt = NULL;
  (*pIn).tess = NULL;
  (*pIn).tesr = NULL;
  (*pIn).intf = NULL;

  (*pIn).meshqualexpr = NULL;
  (*pIn).meshqualdisexpr = NULL;
  // (*pIn).meshoptisizeexpr = NULL;
  (*pIn).dimstring = NULL;
  (*pIn).dimout = NULL;
  (*pIn).dimout_msh = NULL;

  (*pIn).cltype = NULL;
  (*pIn).clstring = NULL;
  (*pIn).clfacetype = NULL;
  (*pIn).clfacestring = NULL;
  (*pIn).cledgetype = NULL;
  (*pIn).cledgestring = NULL;
  (*pIn).clvertype = NULL;
  (*pIn).clverstring = NULL;
  (*pIn).clratiostring = NULL;

  return;
}

void
nem_in_free (struct IN_M In)
{
  ut_free_1d_char (In.body);
  ut_free_1d_char (In.gmsh);
  ut_free_1d_char (In.tmp);
  ut_free_1d_char (In.format);
  ut_free_1d_char (In.performat);
  ut_free_1d_char (In.mesh2dalgo);
  ut_free_1d_char (In.mesh3dalgo);
  ut_free_1d_char (In.nset);
  ut_free_1d_char (In.elset);
  ut_free_1d_char (In.faset);
  ut_free_1d_char (In.partstring);
  ut_free_1d_char (In.transform);
  ut_free_1d_char (In.partmethod);
  ut_free_1d_char (In.tess);
  ut_free_1d_char (In.tesr);
  ut_free_1d_char (In.stn);
  ut_free_2d_char (In.stelt, 5);
  ut_free_2d_char (In.stelset, 5);
  ut_free_1d_char (In.stpt);
  ut_free_1d_char (In.msh);
  ut_free_1d_char (In.per);
  ut_free_1d_char (In.abq);
  ut_free_1d_char (In.geof);
  ut_free_1d_char (In.fepx1);
  ut_free_1d_char (In.fepx2);
  ut_free_1d_char (In.fepx3);
  ut_free_1d_char (In.fepx4);
  ut_free_1d_char (In.fepx5);
  ut_free_1d_char (In.outtess);
  ut_free_1d_char (In.dimstring);
  ut_free_1d_char (In.dimout);
  ut_free_1d_char (In.dimout_msh);
  ut_free_1d_char (In.meshqualexpr);
  ut_free_1d_char (In.meshqualdisexpr);
  ut_free_1d_char (In.elttype);
  ut_free_1d_char (In.cltype);
  ut_free_1d_char (In.clstring);
  ut_free_1d_char (In.clfacetype);
  ut_free_1d_char (In.clfacestring);
  ut_free_1d_char (In.cledgetype);
  ut_free_1d_char (In.cledgestring);
  ut_free_1d_char (In.clvertype);
  ut_free_1d_char (In.clverstring);
  ut_free_1d_char (In.clratiostring);
  ut_free_1d_char (In.transportstring);
  ut_free_1d_char (In.transportfepxstring);
  ut_free_1d_char (In.transporteltmethodstring);
  ut_free_1d_char (In.interface);
  ut_free_1d_char (In.intf);

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
    ut_print_message (2, 2, "Failed to process expression `%s' (option -dim).\n", (*pIn).dimstring);

  if (ut_string_inlist ((*pIn).dimstring, NEUT_SEP_NODEP, "none"))
  {
    (*pIn).dim = -1;
    (*pIn).dimout = ut_alloc_1d_char (1);
    (*pIn).dimout_msh = ut_alloc_1d_char (1);
    return;
  }
  else if (ut_string_inlist ((*pIn).dimstring, NEUT_SEP_NODEP, "all"))
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
    ut_string_separate (string, NEUT_SEP_DEP, &list0, &qty0);
    ut_string_separate (list0[0], NEUT_SEP_NODEP, &list, &qty);

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

    ut_free_2d_char (list0, qty0);
    ut_free_2d_char (list, qty);
    ut_free_1d_char (string);
    ut_free_1d_char (datadimstring);
  }

  return;
}

int
nem_input_outputismesh (struct IN_M In)
{
  if (strstr (In.format, "msh")
      || strstr (In.format, "geof")
      || strstr (In.format, "inp")
      || strstr (In.format, "vtk") || strstr (In.format, "fepx"))
    return 1;
  else
    return 0;
}
