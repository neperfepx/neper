  /* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_input_.h"

/* SetDefaultOptions set the options to their default values */
void
nev_input_options_default (struct IN_V *pIn)
{
  (*pIn).input = NULL;

  /* General options --------------------------------------- */
  (*pIn).povray = NULL;
  ut_string_string ("povray", &((*pIn).povray));

  (*pIn).povrayantialiasing = 1;

  // asymptote ------------------------------------------------------------
  (*pIn).asymptote = NULL;
  ut_string_string ("asy", &((*pIn).asymptote));

  (*pIn).dataqty = 0;
  (*pIn).data = NULL;
  (*pIn).showqty = 0;
  (*pIn).show = NULL;

  (*pIn).space = NULL;
  ut_string_string ("real", &((*pIn).space));

  (*pIn).outdir = NULL;
  ut_string_string (".", &(*pIn).outdir);

  (*pIn).step = NULL;
  ut_string_string ("0", &(*pIn).step);

  (*pIn).crysym = NULL;

  (*pIn).slice = NULL;

  (*pIn).scenebackground = NULL;
  ut_string_string ("white", &(*pIn).scenebackground);

  (*pIn).includepov = NULL;

  (*pIn).imagesize = ut_alloc_1d_char (100);
  sprintf ((*pIn).imagesize, "1200%s900", NEUT_SEP_DEP);
  (*pIn).imageformat = ut_alloc_1d_char (10);
  sprintf ((*pIn).imageformat, "png");

  (*pIn).cameracoo= ut_alloc_1d_char (100);
  sprintf ((*pIn).cameracoo, "x+length*vx%sy+length*vy%sz+length*vz",
           NEUT_SEP_DEP, NEUT_SEP_DEP);

  (*pIn).cameralookat= ut_alloc_1d_char (100);
  sprintf ((*pIn).cameralookat, "x%sy%sz", NEUT_SEP_DEP,
           NEUT_SEP_DEP);

  (*pIn).camerasky= NULL;
  ut_string_string ("default", &(*pIn).camerasky);

  (*pIn).cameraangle= NULL;
  ut_string_string ("25", &(*pIn).cameraangle);

  (*pIn).cameraprojection = NULL;
  ut_string_string ("default", &(*pIn).cameraprojection);

  (*pIn).lightsource= NULL;
  ut_string_string ("default", &(*pIn).lightsource);
  (*pIn).lightambient= NULL;
  ut_string_string ("default", &(*pIn).lightambient);
  (*pIn).lightdiffuse= NULL;
  ut_string_string ("default", &(*pIn).lightdiffuse);
  (*pIn).lightreflection= NULL;
  ut_string_string ("default", &(*pIn).lightreflection);

  (*pIn).pfpolestring = NULL;
  ut_string_string ("default", &(*pIn).pfpolestring);
  (*pIn).pfpolelabel = NULL;
  ut_string_string ("default", &(*pIn).pfpolelabel);
  (*pIn).pfprojlabel = NULL;
  ut_string_string ("default", &(*pIn).pfprojlabel);
  (*pIn).pfprojection = NULL;
  ut_string_string ("stereographic", &(*pIn).pfprojection);
  (*pIn).pfmode = NULL;
  ut_string_string ("symbol", &(*pIn).pfmode);
  (*pIn).pffont = NULL;
  ut_string_string ("TimesRoman", &(*pIn).pffont);
  (*pIn).pfsym = NULL;
  ut_string_string ("monoclinic", &(*pIn).pfsym);
  (*pIn).pfclustering = 1;
  (*pIn).pfgridsize = NULL;
  ut_string_string ("200", &(*pIn).pfgridsize);
  (*pIn).pfkernel = NULL;
  ut_string_string ("normal(3)", &(*pIn).pfkernel);

  (*pIn).pfdirstring = NULL;
  ut_string_string ("y:-x", &(*pIn).pfdirstring);

  (*pIn).pfshape = NULL;
  ut_string_string ("full", &(*pIn).pfshape);

  (*pIn).print = NULL;

  return;
}

void
nev_input_options_set (int argc, char **argv, struct IN_V *pIn, int *ppos)
{
  int i, ArgQty, print, Res;
  char **ArgList = ut_alloc_2d_char (301, 101);
  char *Arg = NULL;

  ut_free_1d_char (&(*pIn).print);

  /* This is the possible argument list. */
  ArgQty = 0;
  strcpy (ArgList[++ArgQty], "-povray");
  strcpy (ArgList[++ArgQty], "-asymptote");
  strcpy (ArgList[++ArgQty], "-simstep");
  strcpy (ArgList[++ArgQty], "-step");
  strcpy (ArgList[++ArgQty], "-crysym");
  strcpy (ArgList[++ArgQty], "-space");
  strcpy (ArgList[++ArgQty], "-slicemesh");
  strcpy (ArgList[++ArgQty], "-cameracoo");
  strcpy (ArgList[++ArgQty], "-cameralookat");
  strcpy (ArgList[++ArgQty], "-camerasky");
  strcpy (ArgList[++ArgQty], "-cameraangle");
  strcpy (ArgList[++ArgQty], "-cameraprojection");
  strcpy (ArgList[++ArgQty], "-lightsource");
  strcpy (ArgList[++ArgQty], "-lightambient");
  strcpy (ArgList[++ArgQty], "-lightdiffuse");
  strcpy (ArgList[++ArgQty], "-lightreflection");
  strcpy (ArgList[++ArgQty], "-scenebackground");
  strcpy (ArgList[++ArgQty], "-pfpole");
  strcpy (ArgList[++ArgQty], "-pfpolelabel");
  strcpy (ArgList[++ArgQty], "-pfprojection");
  strcpy (ArgList[++ArgQty], "-pfprojectionlabel");
  strcpy (ArgList[++ArgQty], "-pfmode");
  strcpy (ArgList[++ArgQty], "-pffont");
  strcpy (ArgList[++ArgQty], "-pfgridsize");
  strcpy (ArgList[++ArgQty], "-pfkernel");
  strcpy (ArgList[++ArgQty], "-pfdir");
  strcpy (ArgList[++ArgQty], "-pfsym");
  strcpy (ArgList[++ArgQty], "-pfclustering");
  strcpy (ArgList[++ArgQty], "-pfshape");
  strcpy (ArgList[++ArgQty], "-ipfpole");
  strcpy (ArgList[++ArgQty], "-ipfpolelabel");
  strcpy (ArgList[++ArgQty], "-ipfprojection");
  strcpy (ArgList[++ArgQty], "-ipfprojectionlabel");
  strcpy (ArgList[++ArgQty], "-ipfmode");
  strcpy (ArgList[++ArgQty], "-ipffont");
  strcpy (ArgList[++ArgQty], "-ipfgridsize");
  strcpy (ArgList[++ArgQty], "-ipfkernel");
  strcpy (ArgList[++ArgQty], "-ipfdir");
  strcpy (ArgList[++ArgQty], "-ipfsym");
  strcpy (ArgList[++ArgQty], "-ipfclustering");
  strcpy (ArgList[++ArgQty], "-ipfshape");
  strcpy (ArgList[++ArgQty], "-outdir");
  strcpy (ArgList[++ArgQty], "-imageformat");
  strcpy (ArgList[++ArgQty], "-imagesize");
  strcpy (ArgList[++ArgQty], "-includepov");
  strcpy (ArgList[++ArgQty], "-povrayantialiasing");
  strcpy (ArgList[++ArgQty], "-print");
  strcpy (ArgList[++ArgQty], "-datacell");
  strcpy (ArgList[++ArgQty], "-datacellcol");
  strcpy (ArgList[++ArgQty], "-datapolycol");
  strcpy (ArgList[++ArgQty], "-datafacecol");
  strcpy (ArgList[++ArgQty], "-dataedgecol");
  strcpy (ArgList[++ArgQty], "-datavercol");
  strcpy (ArgList[++ArgQty], "-dataseedcol");
  strcpy (ArgList[++ArgQty], "-datacrystalcol");
  strcpy (ArgList[++ArgQty], "-datavoxcol");
  strcpy (ArgList[++ArgQty], "-datavoidvoxcol");
  strcpy (ArgList[++ArgQty], "-datacrystaledgecol");
  strcpy (ArgList[++ArgQty], "-datacelledgecol");
  strcpy (ArgList[++ArgQty], "-datavoxedgecol");
  strcpy (ArgList[++ArgQty], "-datacellcolscheme");
  strcpy (ArgList[++ArgQty], "-datapolycolscheme");
  strcpy (ArgList[++ArgQty], "-datafacecolscheme");
  strcpy (ArgList[++ArgQty], "-dataedgecolscheme");
  strcpy (ArgList[++ArgQty], "-datavercolscheme");
  strcpy (ArgList[++ArgQty], "-dataseedcolscheme");
  strcpy (ArgList[++ArgQty], "-datacrystalcolscheme");
  strcpy (ArgList[++ArgQty], "-datavoxcolscheme");
  strcpy (ArgList[++ArgQty], "-datacelltrs");
  strcpy (ArgList[++ArgQty], "-datapolytrs");
  strcpy (ArgList[++ArgQty], "-datafacetrs");
  strcpy (ArgList[++ArgQty], "-dataedgetrs");
  strcpy (ArgList[++ArgQty], "-datavertrs");
  strcpy (ArgList[++ArgQty], "-dataseedtrs");
  strcpy (ArgList[++ArgQty], "-datacrystaltrs");

  strcpy (ArgList[++ArgQty], "-datacellrad");
  strcpy (ArgList[++ArgQty], "-datapolyrad");
  strcpy (ArgList[++ArgQty], "-datafacerad");
  strcpy (ArgList[++ArgQty], "-dataedgerad");
  strcpy (ArgList[++ArgQty], "-dataverrad");
  strcpy (ArgList[++ArgQty], "-dataseedrad");
  strcpy (ArgList[++ArgQty], "-datacrystalrad");
  strcpy (ArgList[++ArgQty], "-datacrystaledgerad");
  strcpy (ArgList[++ArgQty], "-datacelledgerad");
  strcpy (ArgList[++ArgQty], "-datavoxedgerad");

  strcpy (ArgList[++ArgQty], "-datacellscale");
  strcpy (ArgList[++ArgQty], "-datapolyscale");
  strcpy (ArgList[++ArgQty], "-datafacescale");
  strcpy (ArgList[++ArgQty], "-dataedgescale");
  strcpy (ArgList[++ArgQty], "-dataverscale");
  strcpy (ArgList[++ArgQty], "-dataseedscale");
  strcpy (ArgList[++ArgQty], "-datacrystalscale");
  strcpy (ArgList[++ArgQty], "-datavoxscale");

  strcpy (ArgList[++ArgQty], "-datacellscaletitle");
  strcpy (ArgList[++ArgQty], "-datapolyscaletitle");
  strcpy (ArgList[++ArgQty], "-datafacescaletitle");
  strcpy (ArgList[++ArgQty], "-dataedgescaletitle");
  strcpy (ArgList[++ArgQty], "-dataverscaletitle");
  strcpy (ArgList[++ArgQty], "-dataseedscaletitle");
  strcpy (ArgList[++ArgQty], "-datacrystalscaletitle");
  strcpy (ArgList[++ArgQty], "-datavoxscaletitle");

  strcpy (ArgList[++ArgQty], "-datacellsymbol");

  strcpy (ArgList[++ArgQty], "-dataelset");
  strcpy (ArgList[++ArgQty], "-dataelsetcol");

  strcpy (ArgList[++ArgQty], "-dataeltcol");
  strcpy (ArgList[++ArgQty], "-dataelt0dcol");
  strcpy (ArgList[++ArgQty], "-dataelt1dcol");
  strcpy (ArgList[++ArgQty], "-dataelt2dcol");
  strcpy (ArgList[++ArgQty], "-dataelt3dcol");
  strcpy (ArgList[++ArgQty], "-dataelsetcol");
  strcpy (ArgList[++ArgQty], "-dataelset0dcol");
  strcpy (ArgList[++ArgQty], "-dataelset1dcol");
  strcpy (ArgList[++ArgQty], "-dataelset2dcol");
  strcpy (ArgList[++ArgQty], "-dataelset3dcol");

  strcpy (ArgList[++ArgQty], "-datanodecol");
  strcpy (ArgList[++ArgQty], "-datameshcol");
  strcpy (ArgList[++ArgQty], "-dataelt2dedgecol");
  strcpy (ArgList[++ArgQty], "-dataelt3dedgecol");

  strcpy (ArgList[++ArgQty], "-dataeltcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelt0dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelt1dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelt2dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelt3dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelsetcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelset0dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelset1dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelset2dcolscheme");
  strcpy (ArgList[++ArgQty], "-dataelset3dcolscheme");

  strcpy (ArgList[++ArgQty], "-dataeltscale");
  strcpy (ArgList[++ArgQty], "-dataelt0dscale");
  strcpy (ArgList[++ArgQty], "-dataelt1dscale");
  strcpy (ArgList[++ArgQty], "-dataelt2dscale");
  strcpy (ArgList[++ArgQty], "-dataelt3dscale");
  strcpy (ArgList[++ArgQty], "-dataelsetscale");
  strcpy (ArgList[++ArgQty], "-dataelset0dscale");
  strcpy (ArgList[++ArgQty], "-dataelset1dscale");
  strcpy (ArgList[++ArgQty], "-dataelset2dscale");
  strcpy (ArgList[++ArgQty], "-dataelset3dscale");

  strcpy (ArgList[++ArgQty], "-dataeltscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelt0dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelt1dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelt2dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelt3dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelsetscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelset0dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelset1dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelset2dscaletitle");
  strcpy (ArgList[++ArgQty], "-dataelset3dscaletitle");

  strcpy (ArgList[++ArgQty], "-dataelt0drad");
  strcpy (ArgList[++ArgQty], "-dataelt1drad");
  strcpy (ArgList[++ArgQty], "-datanoderad");
  strcpy (ArgList[++ArgQty], "-dataelt2dedgerad");
  strcpy (ArgList[++ArgQty], "-dataelt3dedgerad");
  strcpy (ArgList[++ArgQty], "-dataelset0drad");
  strcpy (ArgList[++ArgQty], "-dataelset1drad");
  strcpy (ArgList[++ArgQty], "-dataelset2drad");
  strcpy (ArgList[++ArgQty], "-dataelset3drad");

  strcpy (ArgList[++ArgQty], "-datanodecoo");
  strcpy (ArgList[++ArgQty], "-datanodecoofact");

  strcpy (ArgList[++ArgQty], "-datacsyscoo");
  strcpy (ArgList[++ArgQty], "-datacsyslength");
  strcpy (ArgList[++ArgQty], "-datacsysrad");
  strcpy (ArgList[++ArgQty], "-datacsyslabel");
  strcpy (ArgList[++ArgQty], "-datacsyscol");
  strcpy (ArgList[++ArgQty], "-datacsysfontsize");

  strcpy (ArgList[++ArgQty], "-showtess");
  strcpy (ArgList[++ArgQty], "-showtesr");
  strcpy (ArgList[++ArgQty], "-showmesh");
  strcpy (ArgList[++ArgQty], "-showmeshslice");
  strcpy (ArgList[++ArgQty], "-showcsys");
  strcpy (ArgList[++ArgQty], "-showscale");
  strcpy (ArgList[++ArgQty], "-showcell");
  strcpy (ArgList[++ArgQty], "-showpoly");
  strcpy (ArgList[++ArgQty], "-showface");
  strcpy (ArgList[++ArgQty], "-showedge");
  strcpy (ArgList[++ArgQty], "-showver");
  strcpy (ArgList[++ArgQty], "-showseed");
  strcpy (ArgList[++ArgQty], "-showcrystal");
  strcpy (ArgList[++ArgQty], "-showfaceinter");
  strcpy (ArgList[++ArgQty], "-showvox");
  strcpy (ArgList[++ArgQty], "-showelt");
  strcpy (ArgList[++ArgQty], "-showelt0d");
  strcpy (ArgList[++ArgQty], "-showelt1d");
  strcpy (ArgList[++ArgQty], "-showelt2d");
  strcpy (ArgList[++ArgQty], "-showelt3d");
  strcpy (ArgList[++ArgQty], "-showelset");
  strcpy (ArgList[++ArgQty], "-showelset0d");
  strcpy (ArgList[++ArgQty], "-showelset1d");
  strcpy (ArgList[++ArgQty], "-showelset2d");
  strcpy (ArgList[++ArgQty], "-showelset3d");
  strcpy (ArgList[++ArgQty], "-shownode");

  print = 0;
  /* Reading arguments ----------------------------------- */
  for (i = (*ppos); i < argc; i++)
  {
    if (argv[i][0] == '-' && i == argc)
      ut_arg_badarg ();

    /* Input data --------------------------------------------------- */
    if (argv[i][0] != '-')
    {
      if (!(*pIn).input)
        ut_string_string (argv[i], &(*pIn).input);
      else
        ut_print_message (2, 0, "Input file already loaded\n");
    }
    // abort ();

    else
    {
      // Searching option name (string completion stuff)
      Res = ut_string_completion (argv[i], ArgList, ArgQty, &Arg);
      if (Res == 1)
      {
        ut_print_lineheader (2);
        printf ("Several possibilities for option `%s'.\n", argv[i]);
        ut_arg_badarg ();
      }
      else if (Res == -1)
      {
        if (strncmp (Arg, "-data", 5) && strncmp (Arg, "-show", 5))
        {
          ut_print_lineheader (2);
          printf ("Unknown option `%s'.\n", argv[i]);
          ut_arg_badarg ();
        }
      }
      ut_string_fnrs (Arg, "-ipf", "-pf", 1);

      /* Prerequisites ---------------------------------------------- */

      if (!strcmp (Arg, "-povray"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).povray));

      else if (!strcmp (Arg, "-asymptote"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).asymptote));

      else if (!strcmp (Arg, "-simstep") || !strcmp (Arg, "-step"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).step));

      else if (!strcmp (Arg, "-crysym"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).crysym));

      else if (!strcmp (Arg, "-space"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).space));

      else if (!strncmp (Arg, "-data", 5))
      {
        (*pIn).dataqty++;

        (*pIn).data = ut_realloc_1d_ppchar ((*pIn).data, (*pIn).dataqty);
        (*pIn).data[(*pIn).dataqty - 1] = ut_alloc_1d_pchar (2);

        ut_string_string (Arg + 5, (*pIn).data[(*pIn).dataqty - 1]);
        ut_string_string (argv[++i], (*pIn).data[(*pIn).dataqty - 1] + 1);
      }

      else if (!strncmp (Arg, "-show", 5))
      {
        (*pIn).showqty++;

        (*pIn).show = ut_realloc_1d_ppchar ((*pIn).show, (*pIn).showqty);
        (*pIn).show[(*pIn).showqty - 1] = ut_alloc_1d_pchar (2);

        ut_string_string (Arg + 5, (*pIn).show[(*pIn).showqty - 1]);
        ut_string_string (argv[++i], (*pIn).show[(*pIn).showqty - 1] + 1);
      }

      else if (!strcmp (Arg, "-slicemesh"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).slice));

      // camera
      else if (!strcmp (Arg, "-cameracoo"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).cameracoo));

      else if (!strcmp (Arg, "-cameralookat"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).cameralookat));

      else if (!strcmp (Arg, "-camerasky"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).camerasky));

      else if (!strcmp (Arg, "-cameraangle"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).cameraangle));

      else if (!strcmp (Arg, "-cameraprojection"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).cameraprojection));

      // light
      else if (!strcmp (Arg, "-lightsource"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).lightsource));

      else if (!strcmp (Arg, "-lightambient"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).lightambient));

      else if (!strcmp (Arg, "-lightdiffuse"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).lightdiffuse));

      else if (!strcmp (Arg, "-lightreflection"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).lightreflection));

      else if (!strcmp (Arg, "-scenebackground"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).scenebackground));

      // pf and ipf
      else if (!strcmp (Arg, "-pfpole"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfpolestring);
      else if (!strcmp (Arg, "-pfpolelabel"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfpolelabel);
      else if (!strcmp (Arg, "-pfprojection"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfprojection);
      else if (!strcmp (Arg, "-pfprojectionlabel"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfprojlabel);
      else if (!strcmp (Arg, "-pfmode"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfmode);
      else if (!strcmp (Arg, "-pffont"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pffont);
      else if (!strcmp (Arg, "-pfgridsize"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfgridsize);
      else if (!strcmp (Arg, "-pfkernel"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfkernel);
      else if (!strcmp (Arg, "-pfdir"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfdirstring);
      else if (!strcmp (Arg, "-pfsym"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfsym);
      else if (!strcmp (Arg, "-pfclustering"))
        ut_arg_nextasint (argv, &i, Arg, 0, 1, &(*pIn).pfclustering);
      else if (!strcmp (Arg, "-pfshape"))
        ut_arg_nextasstring (argv, &i, Arg, &(*pIn).pfshape);

      else if (!strcmp (Arg, "-outdir"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).outdir));

      else if (!strcmp (Arg, "-imageformat"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).imageformat));

      else if (!strcmp (Arg, "-imagesize"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).imagesize));

      else if (!strcmp (Arg, "-includepov"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).includepov));

      else if (!strcmp (Arg, "-povrayantialiasing"))
        ut_arg_nextaslogical (argv, &i, Arg, &((*pIn).povrayantialiasing));

      else if (!strcmp (Arg, "-print"))
      {
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).print));
        print = 1;
        break;
      }

      /*
      else
        ut_arg_error (Arg, "");
      */
    }
  }

  if (print)
    (*ppos) = i + 1;
  else
    (*ppos) = -1;

  ut_free_2d_char (&ArgList, 301);
  ut_free_1d_char (&Arg);

  return;
}
