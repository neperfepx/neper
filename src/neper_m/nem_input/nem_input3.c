  /* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_input_.h"

/* SetDefaultOptions set the options to their default values */
void
nem_input_options_default (struct IN_M *pIn)
{
  /* General options --------------------------------------- */

  (*pIn).nset = ut_alloc_1d_char (1000);
  strcpy ((*pIn).nset, "default");
  (*pIn).faset = ut_alloc_1d_char (100);
  strcpy ((*pIn).faset, "none");

  (*pIn).tess = NULL;
  (*pIn).tesr = NULL;
  (*pIn).mesh = NULL;
  (*pIn).msh = NULL;
  (*pIn).per = NULL;
  (*pIn).geof = NULL;
  (*pIn).fepx1 = NULL;
  (*pIn).fepx2 = NULL;
  (*pIn).fepx3 = NULL;
  (*pIn).intf = NULL;
  (*pIn).stn = NULL;
  (*pIn).stelt[0] = NULL;
  (*pIn).stelt[1] = NULL;
  (*pIn).stelt[2] = NULL;
  (*pIn).stelt[3] = NULL;
  (*pIn).stelt[4] = NULL;
  (*pIn).stelset[0] = NULL;
  (*pIn).stelset[1] = NULL;
  (*pIn).stelset[2] = NULL;
  (*pIn).stelset[3] = NULL;
  (*pIn).stelset[4] = NULL;
  (*pIn).body = NULL;

  (*pIn).format = ut_alloc_1d_char (5);
  strcpy ((*pIn).format, "msh");

  /* Options for meshing ------------------------------- */
  (*pIn).order = 1;
  (*pIn).dimstring = ut_alloc_1d_char (9);
  strcpy ((*pIn).dimstring, "inputdim");

  (*pIn).elttype = ut_alloc_1d_char (5);
  strcpy ((*pIn).elttype, "tri");

  (*pIn).singnodedup = 0;
  (*pIn).dupnodemerge = -1;

  (*pIn).cltype = ut_alloc_1d_char (4);
  strcpy ((*pIn).cltype, "rel");	// abs or rel
  (*pIn).clstring = ut_alloc_1d_char (2);
  strcpy ((*pIn).clstring, "1");
  (*pIn).clmin = 0;
  (*pIn).pl = 2;

  (*pIn).mesh2dalgo = ut_alloc_1d_char (100);
  (*pIn).mesh3dalgo = ut_alloc_1d_char (100);
  (*pIn).mesh3dreport = 0;
  ut_string_string ("all", &((*pIn).meshface));
  ut_string_string ("all", &((*pIn).meshpoly));

  (*pIn).meshqualexpr = ut_alloc_1d_char (100);
  strcpy ((*pIn).meshqualexpr, "Odis^0.8*Osize^0.2");
  (*pIn).meshqualdisexpr = ut_alloc_1d_char (100);
  strcpy ((*pIn).meshqualdisexpr, "dis^(exp((dis^0.1)/(dis^0.1-1)))");
  (*pIn).meshqualmin = 0.9;

  strcpy ((*pIn).mesh2dalgo, "default");
  strcpy ((*pIn).mesh3dalgo, "default");

  ut_string_string ("continuous", &((*pIn).interface));

  (*pIn).partmethod = ut_alloc_1d_char (10);
  strcpy ((*pIn).partmethod, "default");
  (*pIn).partstring = NULL;
  (*pIn).partbalancing = .5;
  (*pIn).scale = NULL;

  ut_string_string ("none", &(*pIn).scalestring);

  (*pIn).gmsh = ut_alloc_1d_char (5);
  strcpy ((*pIn).gmsh, "gmsh");
  ut_string_string (".", &(*pIn).tmp);

  (*pIn).mesh2dmaxtime = 10000;
  (*pIn).mesh2drmaxtime = 100;
  (*pIn).mesh2diter = 3;
  (*pIn).mesh3dmaxtime = 10000;
  (*pIn).mesh3drmaxtime = 100;
  (*pIn).mesh3diter = 3;
  (*pIn).mesh3dclconv = 0.02;

  /* Options for remeshing --------------------------------- */
  (*pIn).transportstring = NULL;

  (*pIn).loadmesh = NULL;
  (*pIn).loadpoint = NULL;

  (*pIn).tesrsmooth = ut_alloc_1d_char (21);
  strcpy ((*pIn).tesrsmooth, "laplacian");

  (*pIn).tesrsmoothfact = 0.5;
  (*pIn).tesrsmoothitermax = 5;

  return;
}

void
nem_input_options_set (struct IN_M *pIn, int argc, char **argv)
{
  int i, j, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = NULL;
  char *format = ut_alloc_1d_char (100);
  char *tmpstring = NULL;
  char *tmpstring2 = ut_alloc_1d_char (1000);

  /* This is the possible argument list. */
  ArgQty = 0;

  // General options ---------------------------------------------------
  strcpy (ArgList[++ArgQty], "-o");
  strcpy (ArgList[++ArgQty], "-v");
  strcpy (ArgList[++ArgQty], "-gmsh");
  strcpy (ArgList[++ArgQty], "-tmp");

  // General meshing options -------------------------------------------
  strcpy (ArgList[++ArgQty], "-elttype");
  strcpy (ArgList[++ArgQty], "-cl");
  strcpy (ArgList[++ArgQty], "-rcl");
  strcpy (ArgList[++ArgQty], "-clratio");
  strcpy (ArgList[++ArgQty], "-pl");
  strcpy (ArgList[++ArgQty], "-clmin");
  strcpy (ArgList[++ArgQty], "-order");
  strcpy (ArgList[++ArgQty], "-dim");
  strcpy (ArgList[++ArgQty], "-meshreconstruct");	//
  strcpy (ArgList[++ArgQty], "-mesh2dalgo");
  strcpy (ArgList[++ArgQty], "-mesh3dalgo");
  strcpy (ArgList[++ArgQty], "-meshqualexpr");
  strcpy (ArgList[++ArgQty], "-meshqualdisexpr");
  strcpy (ArgList[++ArgQty], "-meshqualmin");

  strcpy (ArgList[++ArgQty], "-singnodedup");
  strcpy (ArgList[++ArgQty], "-dupnodemerge");

  // Raster tessellation meshing options ---------------------------------------------
  strcpy (ArgList[++ArgQty], "-tesrsmooth");
  strcpy (ArgList[++ArgQty], "-tesrsmoothfact");
  strcpy (ArgList[++ArgQty], "-tesrsmoothitermax");

  // development options
  strcpy (ArgList[++ArgQty], "-meshface");
  strcpy (ArgList[++ArgQty], "-meshpoly");
  strcpy (ArgList[++ArgQty], "-interface");
  strcpy (ArgList[++ArgQty], "-mesh3dreport");

  // transformation options
  strcpy (ArgList[++ArgQty], "-scale");

  // Mesh partitionning options ----------------------------------------
  strcpy (ArgList[++ArgQty], "-part");
  strcpy (ArgList[++ArgQty], "-partbalancing");
  strcpy (ArgList[++ArgQty], "-partmethod");

  // Output options ----------------------------------------------------
  strcpy (ArgList[++ArgQty], "-format");
  strcpy (ArgList[++ArgQty], "-nset");
  strcpy (ArgList[++ArgQty], "-surf");	// deprecated
  strcpy (ArgList[++ArgQty], "-faset");

  // Tessellation and mesh statistics options ------------------------------
  strcpy (ArgList[++ArgQty], "-statnode");
  strcpy (ArgList[++ArgQty], "-statelt");
  strcpy (ArgList[++ArgQty], "-statelt0d");
  strcpy (ArgList[++ArgQty], "-statelt1d");
  strcpy (ArgList[++ArgQty], "-statelt2d");
  strcpy (ArgList[++ArgQty], "-statelt3d");
  strcpy (ArgList[++ArgQty], "-statelset");
  strcpy (ArgList[++ArgQty], "-statelset0d");
  strcpy (ArgList[++ArgQty], "-statelset1d");
  strcpy (ArgList[++ArgQty], "-statelset2d");
  strcpy (ArgList[++ArgQty], "-statelset3d");
  strcpy (ArgList[++ArgQty], "-statpoint");

  // Advanced options --------------------------------------------------
  strcpy (ArgList[++ArgQty], "-mesh2dmaxtime");
  strcpy (ArgList[++ArgQty], "-mesh2drmaxtime");
  strcpy (ArgList[++ArgQty], "-mesh2diter");
  strcpy (ArgList[++ArgQty], "-mesh3dmaxtime");
  strcpy (ArgList[++ArgQty], "-mesh3drmaxtime");
  strcpy (ArgList[++ArgQty], "-mesh3diter");
  strcpy (ArgList[++ArgQty], "-mesh3dclconv");

  // Field transport ---------------------------------------------------
  strcpy (ArgList[++ArgQty], "-transport");

  // Restart a job -----------------------------------------------------
  strcpy (ArgList[++ArgQty], "-loadtess");	// not documented
  strcpy (ArgList[++ArgQty], "-loadmesh");
  strcpy (ArgList[++ArgQty], "-loadpoint");

  /* Reading arguments ----------------------------------- */
  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-' && i == argc - 1)
      ut_arg_badarg ();

    /* Input data --------------------------------------------------- */
    if (argv[i][0] != '-')
    {
      if (!(*pIn).tess && !(*pIn).tesr && !(*pIn).mesh)
      {
	i--;

	char *tmp = NULL;
	ut_arg_nextasstring (argv, &i, Arg, &tmp);
	char **list = NULL;
	int qty;

	ut_string_separate (tmp, NEUT_SEP_NODEP, &list, &qty);

	for (j = 0; j < qty; j++)
	{
	  char *format = NULL;
	  char **list2 = NULL;
	  int qty2;

	  ut_string_separate (list[j], NEUT_SEP_DEP, &list2, &qty2);

	  ut_file_format (list2[0], &format);

	  if (format && (!strcmp (format, "tess") || !strcmp (format,
							      "mtess")))
	    ut_string_string (list[j], &((*pIn).tess));
	  else if (format && (!strcmp (format, "tesr")))
	    ut_string_string (list[j], &((*pIn).tesr));
	  else if (format && (!strcmp (format, "gmsh_msh")))
	    ut_string_string (list[j], &((*pIn).mesh));
	  else
	    ut_print_message (2, 3, "Unknown input file format.");

	  ut_free_2d_char (list2, qty2);
	  ut_free_1d_char (format);
	}

	ut_free_1d_char (tmp);
	ut_free_2d_char (list, qty);
      }
      else
      {
	ut_print_message (2, 0, "Input file already loaded\n");
	abort ();
      }
    }

    else
    {
      /* Searching option name (string completion stuff) */
      Res = ut_string_comp (argv[i], ArgList, ArgQty, &Arg);
      if (Res == 1)
      {
	ut_print_lineheader (2);
	printf ("Several possibilities for option `%s'.\n", argv[i]);
	ut_arg_badarg ();
      }
      else if (Res == -1)
      {
	ut_print_lineheader (2);
	printf ("Unknown option `%s'.\n", argv[i]);
	ut_arg_badarg ();
      }

      /* Prerequisites ---------------------------------------------- */

      if (!strcmp (Arg, "-gmsh"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).gmsh));

      else if (!strcmp (Arg, "-tmp"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).tmp));

      /* General options ------------------------------------------------ */

      else if (!strcmp (Arg, "-o"))
      {
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).body));
	ut_string_body ((*pIn).body, ".", &((*pIn).body));
      }

      /* Options for the meshing ----------------------------------------- */

      else if (!strcmp (Arg, "-cl"))
      {
	strcpy ((*pIn).cltype, "abs");
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).clstring));
      }
      else if (!strcmp (Arg, "-rcl"))
      {
	strcpy ((*pIn).cltype, "rel");
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).clstring));
      }
      else if (!strcmp (Arg, "-clratio"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).clratiostring));
      else if (!strcmp (Arg, "-clmin"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX, &((*pIn).clmin));
      else if ((!strcmp (Arg, "-pl")))
	ut_arg_nextasreal (argv, &i, Arg, 1, DBL_MAX, &((*pIn).pl));
      else if (!strcmp (Arg, "-tesrsmooth"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).tesrsmooth));
      else if ((!strcmp (Arg, "-tesrsmoothfact")))
	ut_arg_nextasreal (argv, &i, Arg, 0, 1, &((*pIn).tesrsmoothfact));
      else if ((!strcmp (Arg, "-tesrsmoothitermax")))
	ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX,
			  &((*pIn).tesrsmoothitermax));
      else if (!strcmp (Arg, "-mesh2dalgo"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).mesh2dalgo));
      else if (!strcmp (Arg, "-mesh3dalgo"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).mesh3dalgo));
      else if (!strcmp (Arg, "-meshqualexpr"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).meshqualexpr));
      else if (!strcmp (Arg, "-meshqualdisexpr"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).meshqualdisexpr));
      else if (!strcmp (Arg, "-meshqualmin"))
	ut_arg_nextasreal (argv, &i, Arg, 0, 1, &((*pIn).meshqualmin));
      else if (!strcmp (Arg, "-singnodedup"))
	ut_arg_nextasint (argv, &i, Arg, 0, 1, &((*pIn).singnodedup));
      else if (!strcmp (Arg, "-dupnodemerge"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX, &((*pIn).dupnodemerge));
      else if (!strcmp (Arg, "-interface") )
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).interface));
      // development options
      else if (!strcmp (Arg, "-mesh3dreport"))
	ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX, &((*pIn).mesh3dreport));
      else if (!strcmp (Arg, "-meshpoly"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).meshpoly));
      else if (!strcmp (Arg, "-meshface") )
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).meshface));
      // end of development options

      else if ((!strcmp (Arg, "-order")))
	ut_arg_nextasint (argv, &i, Arg, 1, 2, &((*pIn).order));
      else if (!strcmp (Arg, "-dim"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).dimstring));
      else if ((!strcmp (Arg, "-elttype")))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).elttype));
      else if (!strcmp (Arg, "-statnode"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stn));
      else if (!strcmp (Arg, "-statelt0d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelt[0])));
      else if (!strcmp (Arg, "-statelt1d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelt[1])));
      else if (!strcmp (Arg, "-statelt2d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelt[2])));
      else if (!strcmp (Arg, "-statelt3d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelt[3])));
      else if (!strcmp (Arg, "-statelt"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelt[4])));
      else if (!strcmp (Arg, "-statelset0d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelset[0])));
      else if (!strcmp (Arg, "-statelset1d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelset[1])));
      else if (!strcmp (Arg, "-statelset2d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelset[2])));
      else if (!strcmp (Arg, "-statelset3d"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelset[3])));
      else if (!strcmp (Arg, "-statelset"))
	ut_arg_nextasstring (argv, &i, Arg, &(((*pIn).stelset[4])));
      else if (!strcmp (Arg, "-statpoint"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stpt));
      else if (!strcmp (Arg, "-mesh3dmaxtime"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX,
			   &((*pIn).mesh3dmaxtime));
      else if (!strcmp (Arg, "-mesh2drmaxtime"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX,
			   &((*pIn).mesh2drmaxtime));
      else if (!strcmp (Arg, "-mesh2diter"))
	ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX, &((*pIn).mesh2diter));
      else if (!strcmp (Arg, "-mesh2dmaxtime"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX,
			   &((*pIn).mesh2dmaxtime));
      else if (!strcmp (Arg, "-mesh3drmaxtime"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX,
			   &((*pIn).mesh3drmaxtime));
      else if (!strcmp (Arg, "-mesh3diter"))
	ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX, &((*pIn).mesh3diter));
      else if (!strcmp (Arg, "-mesh3dclconv"))
	ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX, &((*pIn).mesh3dclconv));
      else if (!strcmp (Arg, "-nset"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).nset));
      else if (!strcmp (Arg, "-faset"))
      {
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).faset));
	if ((*pIn).faset[0] == '2')
	  ut_arg_nextasstring (argv, &i, Arg, &((*pIn).faset));
      }
      else if (!strcmp (Arg, "-loadmesh"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).loadmesh));
      else if (!strcmp (Arg, "-loadpoint"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).loadpoint));
      else if ((!strcmp (Arg, "-transport")))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).transportstring));
      else if (!strcmp (Arg, "-format"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).format));
      else if (!strcmp (Arg, "-part"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).partstring));
      else if (!strcmp (Arg, "-partmethod"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).partmethod));
      else if (!strcmp (Arg, "-partbalancing"))
	ut_arg_nextasreal (argv, &i, Arg, 0, 1, &((*pIn).partbalancing));
      else if (!strcmp (Arg, "-scale"))
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).scalestring));
      else
	ut_arg_error (Arg, "");
    }
  }

  ut_free_2d_char (ArgList, 101);
  ut_free_1d_char (Arg);
  ut_free_1d_char (format);
  ut_free_1d_char (tmpstring);
  ut_free_1d_char (tmpstring2);

  return;
}
