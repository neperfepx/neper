/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_input_.h"

void
nem_input_treatargs (int fargc, char **fargv, int argc, char **argv,
		     struct IN_M *pIn)
{
  /* Setting default options */
  nem_input_options_default (pIn);

  /* Reading options. */
  nem_input_options_set (pIn, fargc, fargv);
  nem_input_options_set (pIn, argc, argv);

  // Checking out the elttype value
  (*pIn).elttype = ut_realloc_1d_char ((*pIn).elttype, 5);
  if (!strcmp ((*pIn).elttype, "tet"))
    strcpy ((*pIn).elttype, "tri");
  else if (!strcmp ((*pIn).elttype, "hex"))
    strcpy ((*pIn).elttype, "quad");

  if (strcmp ((*pIn).elttype, "tri") != 0
      && strcmp ((*pIn).elttype, "quad") != 0)
  {
    ut_print_message (2, 0, "Unknown elt type `%s'.\n", (*pIn).elttype);
    abort ();
  }

  char *tmp = ut_alloc_1d_char (1000);
  sprintf (tmp, "%s --version 2> .nepertmp", (*pIn).gmsh);
  if (system (tmp) == -1)
    abort ();

  FILE *file = ut_file_open (".nepertmp", "R");
  if (fscanf (file, "%s", tmp) != 1)
    ut_print_message (2, 3, "Option `-gmsh' does not provide a valid path to the gmsh binary.\n");
  else if (strstr (tmp, "git"))
    ut_print_message (1, 3, "Using a development Gmsh version (%s)...\n", tmp);
  else if (!strcmp (tmp, "2.5.1") || !strcmp (tmp, "2.14.1"))
    ut_print_message (2, 3, "Gmsh %s is known to produce error.  Please update.\n", tmp);

  ut_file_close (file, ".nepertmp", "R");
  remove (".nepertmp");
  ut_free_1d_char (tmp);

  if ((*pIn).partstring)
  {
    int i;
    char *format = ut_alloc_1d_char (10);

    ut_string_format ((*pIn).partstring, format);

    if (format[strlen (format) - 1] != 'd')
    {
      if (format[strlen (format) - 1] == 's')
      {
	char **list = NULL;
	int qty;

	ut_string_separate ((*pIn).partstring, NEUT_SEP_DEP, &list, &qty);
	if (qty == 1)
	{
	  if (ut_file_exist ((*pIn).partstring) != 1)
	    ut_print_message
	      (2, 0, "option -part %s: file not found.\n", (*pIn).partstring);
	}
	else if (qty == 2)
	{
	  for (i = 0; i < 2; i++)
	  {
	    ut_string_format (list[i], format);
	    if (format[strlen (format) - 1] != 'd')
	      ut_print_message
		(2, 0, "option `-part %s': argument not valid.\n",
		 (*pIn).partstring);
	  }

	  for (i = 0; i < 2; i++)
	  {
	    double tmp, tmp2;
	    sscanf (list[i], "%lf", &tmp);
	    tmp2 = log (tmp) / log (2);
	    tmp2 = fabs (tmp2 / ut_num_d2ri (tmp2) - 1);
	    if (tmp2 > 1e-6)
	      ut_print_message (2, 2,
				"option `-part %s': power of 2 inputs required.\n",
				(*pIn).partstring);
	  }
	}
	else
	  ut_print_message
	    (2, 0, "option `-part %s': argument not valid.\n",
	     (*pIn).partstring);

	ut_free_2d_char (list, qty);
      }
    }

    ut_free_1d_char (format);

    /*
       if ((*pIn).partmode == 1 && ut_file_exist ((*pIn).partarchfile) == 0)
       {
       ut_print_message (2, 0, "Architecture file does not exist.\n");
       abort ();
       }
     */

#ifndef HAVE_LIBSCOTCH
    ut_print_messagewnc (2, 72,
			 "Options `-part*' (mesh partitioning) are not available since this version of Neper has been built without libscotch.");
    abort ();
#endif
  }

  if ((!(*pIn).partstring)
      && ut_string_inlist ((*pIn).format, NEUT_SEP_DEP, "fepx"))
  {
    ut_print_message (1, 3,
		      "Options `-part ...' should be used for parallel\n");
    ut_print_message (1, 3, "calculations with FEpX.\n");
  }

  /* Recording body */
  if ((*pIn).body == NULL)
  {
    if ((*pIn).loadmesh != NULL)
      ut_string_body ((*pIn).loadmesh, ".", &((*pIn).body));
    else if ((*pIn).tess && strlen ((*pIn).tess))
      ut_string_body ((*pIn).tess, ".", &((*pIn).body));
    else if ((*pIn).tesr && strlen ((*pIn).tesr))
      ut_string_body ((*pIn).tesr, ".", &((*pIn).body));
    else if ((*pIn).mesh && strlen ((*pIn).mesh))
      ut_string_body ((*pIn).mesh, ".:", &((*pIn).body));
  }

  /* Writing all file names */
  if ((*pIn).body)
  {
    (*pIn).geof = ut_string_addextension ((*pIn).body, ".geof");
    (*pIn).msh = ut_string_addextension ((*pIn).body, ".msh");
    (*pIn).per = ut_string_addextension ((*pIn).body, ".per");
    (*pIn).abq = ut_string_addextension ((*pIn).body, ".inp");
    (*pIn).vtk = ut_string_addextension ((*pIn).body, ".vtk");
    (*pIn).fepx1 = ut_string_addextension ((*pIn).body, ".parms");
    (*pIn).fepx2 = ut_string_addextension ((*pIn).body, ".mesh");
    (*pIn).fepx3 = ut_string_addextension ((*pIn).body, ".surf");
    (*pIn).fepx4 = ut_string_addextension ((*pIn).body, ".opt");
    (*pIn).fepx5 = ut_string_addextension ((*pIn).body, ".bcs");
    (*pIn).outtess = ut_string_addextension ((*pIn).body, ".tess");
    (*pIn).intf = ut_string_addextension ((*pIn).body, ".intf");
  }

  return;
}
