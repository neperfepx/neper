/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input_.h"

/* SetDefaultOptions set the options to their default values */
void
net_input_options_default (struct IN_T *pIn)
{
  (*pIn).dim = 3;
  ut_string_string ("default", &((*pIn).domain));
  ut_string_string ("none", &((*pIn).periodicstring));
  ut_string_string ("20", &((*pIn).tesrsizestring));

  (*pIn).nstring = NULL; // needed for proper levelqty determination
  ut_string_string ("1", &(*pIn).idstring);
  ut_string_string ("voronoi", &(*pIn).morphostring);
  ut_string_string ("default", &(*pIn).morphooptialgostring);
  ut_string_string ("default", &(*pIn).morphooptiinistring);
  ut_string_string ("nanoflann", &(*pIn).morphooptialgoneighstring);
  ut_string_string ("max(varnb,1000)", &(*pIn).morphooptialgomaxiterstring);
  ut_string_string ("default", &(*pIn).morphooptiobjectivestring);
  ut_string_string ("default", &(*pIn).morphooptigridstring);
  ut_string_string ("default", &(*pIn).morphooptismoothstring);
  ut_string_string ("default", &(*pIn).morphooptistopstring);
  ut_string_string ("none", &(*pIn).morphooptilogtimestring);
  ut_string_string ("none", &(*pIn).morphooptilogvarstring);
  ut_string_string ("none", &(*pIn).morphooptilogdisstring);
  ut_string_string ("none", &(*pIn).morphooptilogtesrstring);
  ut_string_string ("none", &(*pIn).morphooptilogvalstring);
  ut_string_string ("1", &(*pIn).morphooptimultiseedstring);
  ut_string_string ("default", &(*pIn).morphooptidofstring);
  ut_string_string ("HUGE_VAL", &(*pIn).morphooptideltamaxstring);
  ut_string_string ("avdiameq/10", &(*pIn).morphooptiinistepstring);

  (*pIn).reg = 0;
  (*pIn).fmax = 20;
  ut_string_string ("rel", &((*pIn).seltype));
  ut_string_string ("1", &((*pIn).selstring));
  (*pIn).mloop = 2;
  (*pIn).maxedgedelqty = INT_MAX;
  ut_string_string ("none", &(*pIn).sortstring); // to change into sort
  ut_string_string ("none", &(*pIn).transform);
  (*pIn).checktess = 0;
  ut_string_string ("none", &(*pIn).load);
  ut_string_string ("none", &(*pIn).loadpoint);

  ut_string_string ("none", &(*pIn).format);
  ut_string_string ("binary16", &(*pIn).tesrformat);

  ut_string_string ("default", &((*pIn).oristring));
  ut_string_string ("default", &(*pIn).orioptistopstring);
  ut_string_string ("default", &(*pIn).orioptineighstring);
  ut_string_string ("default", &(*pIn).orioptiinistring);
  ut_string_string ("default", &(*pIn).orioptifixstring);
  ut_string_string ("default", &((*pIn).oricrysymstring));
  ut_string_string ("none", &(*pIn).orioptilogvarstring);

  ut_string_string ("e", &((*pIn).orides));
  ut_string_string ("plain", &((*pIn).oriformat));

  // copying defaults to first line of multiscale parameters
  (*pIn).n = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).nstring, (*pIn).n);
  (*pIn).id = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).idstring, (*pIn).id);
  (*pIn).morpho = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphostring, (*pIn).morpho);
  (*pIn).morphooptiini = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptiinistring, (*pIn).morphooptiini);
  (*pIn).morphooptialgo = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptialgostring, (*pIn).morphooptialgo);
  (*pIn).morphooptialgoneigh = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptialgoneighstring, (*pIn).morphooptialgoneigh);
  (*pIn).morphooptialgomaxiter = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptialgomaxiterstring, (*pIn).morphooptialgomaxiter);
  (*pIn).morphooptiobjective = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptiobjectivestring, (*pIn).morphooptiobjective);
  (*pIn).morphooptigrid = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptigridstring, (*pIn).morphooptigrid);
  (*pIn).morphooptismooth = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptismoothstring, (*pIn).morphooptismooth);
  (*pIn).morphooptistop = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptistopstring, (*pIn).morphooptistop);
  (*pIn).morphooptilogtime = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptilogtimestring, (*pIn).morphooptilogtime);
  (*pIn).morphooptilogvar = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptilogvarstring, (*pIn).morphooptilogvar);
  (*pIn).morphooptilogdis = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptilogdisstring, (*pIn).morphooptilogdis);
  (*pIn).morphooptilogtesr = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptilogtesrstring, (*pIn).morphooptilogtesr);
  (*pIn).morphooptilogval = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptilogvalstring, (*pIn).morphooptilogval);
  (*pIn).morphooptimultiseed = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptimultiseedstring, (*pIn).morphooptimultiseed);
  (*pIn).morphooptidof = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptidofstring, (*pIn).morphooptidof);
  (*pIn).morphooptideltamax = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptideltamaxstring, (*pIn).morphooptideltamax);
  (*pIn).morphooptiinistep = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphooptiinistepstring, (*pIn).morphooptiinistep);

  (*pIn).ori = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).ori);
  (*pIn).oricrysym = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oricrysymstring, (*pIn).oricrysym);
  (*pIn).orioptistop = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).orioptistop);
  (*pIn).orioptineigh = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).orioptineigh);
  (*pIn).orioptiini = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).orioptiini);
  (*pIn).orioptifix = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).orioptifixstring, (*pIn).orioptifix);
  (*pIn).orioptilogvar = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).orioptilogvarstring, (*pIn).orioptilogvar);

  return;
}

void
net_input_options_set (struct IN_T *pIn, int argc, char **argv)
{
  int i, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = NULL;

  // Argument list =====================================================

  ArgQty = 0;
  // Input data --------------------------------------------------------
  strcpy (ArgList[++ArgQty], "-n");
  strcpy (ArgList[++ArgQty], "-id");
  strcpy (ArgList[++ArgQty], "-morpho");
  strcpy (ArgList[++ArgQty], "-morphooptiini");
  strcpy (ArgList[++ArgQty], "-morphooptialgo");
  strcpy (ArgList[++ArgQty], "-morphooptialgoneigh");
  strcpy (ArgList[++ArgQty], "-morphooptialgoini");
  strcpy (ArgList[++ArgQty], "-morphooptialgomaxiter");
  strcpy (ArgList[++ArgQty], "-morphooptiobjective");
  strcpy (ArgList[++ArgQty], "-morphooptigrid");
  strcpy (ArgList[++ArgQty], "-morphooptismooth");
  strcpy (ArgList[++ArgQty], "-morphooptideltamax");
  strcpy (ArgList[++ArgQty], "-morphooptiinistep");
  strcpy (ArgList[++ArgQty], "-morphooptistop");
  strcpy (ArgList[++ArgQty], "-morphooptilogtime");
  strcpy (ArgList[++ArgQty], "-morphooptilogvar");
  strcpy (ArgList[++ArgQty], "-morphooptilogdis");
  strcpy (ArgList[++ArgQty], "-morphooptilogtesr");
  strcpy (ArgList[++ArgQty], "-morphooptilogval");
  strcpy (ArgList[++ArgQty], "-morphooptidof");
  strcpy (ArgList[++ArgQty], "-morphooptimultiseed");
  strcpy (ArgList[++ArgQty], "-dim");
  strcpy (ArgList[++ArgQty], "-periodicity");
  // General options ---------------------------------------------------
  strcpy (ArgList[++ArgQty], "-o");
  strcpy (ArgList[++ArgQty], "-v");
  strcpy (ArgList[++ArgQty], "-libpath");
  // Tessellation options ----------------------------------------------
  strcpy (ArgList[++ArgQty], "-domain");
  strcpy (ArgList[++ArgQty], "-domainfacet");
  strcpy (ArgList[++ArgQty], "-transform");
  // Regularization options --------------------------------------------
  strcpy (ArgList[++ArgQty], "-regularization");
  strcpy (ArgList[++ArgQty], "-fmax");
  strcpy (ArgList[++ArgQty], "-sel");
  strcpy (ArgList[++ArgQty], "-rsel");
  strcpy (ArgList[++ArgQty], "-mloop");
  strcpy (ArgList[++ArgQty], "-maxedgedelqty");
  // Output options ----------------------------------------------------
  strcpy (ArgList[++ArgQty], "-format");
  strcpy (ArgList[++ArgQty], "-tesrformat");
  strcpy (ArgList[++ArgQty], "-tesrsize");
  strcpy (ArgList[++ArgQty], "-ori");
  strcpy (ArgList[++ArgQty], "-orioptistop");
  strcpy (ArgList[++ArgQty], "-orioptineigh");
  strcpy (ArgList[++ArgQty], "-orioptiini");
  strcpy (ArgList[++ArgQty], "-orioptifix");
  strcpy (ArgList[++ArgQty], "-oridescriptor");
  strcpy (ArgList[++ArgQty], "-oricrysym");
  strcpy (ArgList[++ArgQty], "-oriformat");
  strcpy (ArgList[++ArgQty], "-orioptilogvar");
  // Post-processing ---------------------------------------------------
  strcpy (ArgList[++ArgQty], "-statver");
  strcpy (ArgList[++ArgQty], "-statedge");
  strcpy (ArgList[++ArgQty], "-statface");
  strcpy (ArgList[++ArgQty], "-statpoly");
  strcpy (ArgList[++ArgQty], "-statcell");
  strcpy (ArgList[++ArgQty], "-statseed");
  strcpy (ArgList[++ArgQty], "-statpoint");
  strcpy (ArgList[++ArgQty], "-statvox");
  strcpy (ArgList[++ArgQty], "-sort");
  // Restart a job -----------------------------------------------------
  strcpy (ArgList[++ArgQty], "-loadtess");
  strcpy (ArgList[++ArgQty], "-loadtesr");
  strcpy (ArgList[++ArgQty], "-loadpoint");
  // Debugging ---------------------------------------------------------
  strcpy (ArgList[++ArgQty], "-checktess");

  // Treating arguments ================================================

  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
      ut_arg_badarg ();

    if (i == argc - 1)
      ut_arg_badarg ();

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
      if (!strcmp (argv[i], "-scale"))
        printf ("Option `-scale' was removed; use `-transform scale' instead.\n");
      else
        printf ("Unknown option `%s'.\n", argv[i]);
      ut_arg_badarg ();
    }

/*----------------------------------------------------------------------
* input data */
    if (!strcmp (Arg, "-n") && i < argc - 1)
    {
      if ((*pIn).input == NULL)
      {
	(*pIn).input = ut_alloc_1d_char (5);
	strcpy ((*pIn).input, "n");
	ut_arg_nextasstring (argv, &i, Arg, &((*pIn).nstring));

	if ((*pIn).morphostring
	    && (strncmp ((*pIn).morphostring, "cube", 4) == 0
	    || strncmp ((*pIn).morphostring, "dodeca", 6) == 0
	    || strncmp ((*pIn).morphostring, "tocta", 5) == 0
	    || strncmp ((*pIn).morphostring, "square", 6) == 0))
	  strcpy ((*pIn).input, "n_reg");
      }
      else
	ut_arg_badarg ();
    }
    else if (!strcmp (Arg, "-id") && i < argc - 1)
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).idstring));
    else if (!strcmp (Arg, "-morpho"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphostring));
    else if (!strcmp (Arg, "-morphooptiini"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptiinistring));
    else if (!strcmp (Arg, "-morphooptialgo"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptialgostring));
    else if (!strcmp (Arg, "-morphooptialgoneigh"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptialgoneighstring));
    else if (!strcmp (Arg, "-morphooptialgomaxiter"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptialgomaxiterstring));
    else if (!strcmp (Arg, "-morphooptiobjective"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptiobjectivestring));
    else if (!strcmp (Arg, "-morphooptigrid"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptigridstring));
    else if (!strcmp (Arg, "-morphooptismooth"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptismoothstring));
    else if (!strcmp (Arg, "-morphooptistop"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptistopstring));
    else if (!strcmp (Arg, "-morphooptilogtime"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptilogtimestring));
    else if (!strcmp (Arg, "-morphooptilogvar"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptilogvarstring));
    else if (!strcmp (Arg, "-morphooptilogdis"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptilogdisstring));
    else if (!strcmp (Arg, "-morphooptilogtesr"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptilogtesrstring));
    else if (!strcmp (Arg, "-morphooptilogval"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptilogvalstring));
    else if (!strcmp (Arg, "-morphooptidof"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptidofstring));
    else if (!strcmp (Arg, "-morphooptideltamax"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptideltamaxstring));
    else if (!strcmp (Arg, "-morphooptimultiseed"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptimultiseedstring));
    else if (!strcmp (Arg, "-morphooptiinistep"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).morphooptiinistepstring));
    else if (!strcmp (Arg, "-dim"))
      ut_arg_nextasint (argv, &i, Arg, 2, 3, &((*pIn).dim));
    else if (!strcmp (Arg, "-periodicity"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).periodicstring));

/*----------------------------------------------------------------------
* general options */
    else if (!strcmp (Arg, "-o"))
    {
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).body));
      ut_string_body ((*pIn).body, ".", &((*pIn).body));
    }

/*----------------------------------------------------------------------
* tessellation options */
    else if (!strcmp (Arg, "-domain"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).domain));
    else if (!strcmp (Arg, "-transform"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).transform));

/*----------------------------------------------------------------------
* regularization options */
    else if (!strcmp (Arg, "-regularization"))
      ut_arg_nextasint (argv, &i, Arg, 0, 1, &(*pIn).reg);
    else if (!strcmp (Arg, "-fmax"))
      ut_arg_nextasreal (argv, &i, Arg, 0, 180, &(*pIn).fmax);
    else if (!strcmp (Arg, "-sel"))
    {
      strcpy ((*pIn).seltype, "abs");
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).selstring));
    }
    else if (!strcmp (Arg, "-rsel"))
    {
      strcpy ((*pIn).seltype, "rel");
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).selstring));
    }
    else if (!strcmp (Arg, "-mloop"))
      ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX, &((*pIn).mloop));
    else if (!strcmp (Arg, "-maxedgedelqty"))
      ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX, &((*pIn).maxedgedelqty));

/*----------------------------------------------------------------------
* post-processing options */
    else if (!strcmp (Arg, "-format"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).format));
    else if (!strcmp (Arg, "-tesrformat"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).tesrformat));
    else if (!strcmp (Arg, "-tesrsize"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).tesrsizestring));
    else if (!strcmp (Arg, "-ori"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).oristring));
    else if (!strcmp (Arg, "-orioptistop"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptistopstring));
    else if (!strcmp (Arg, "-orioptineigh"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptineighstring));
    else if (!strcmp (Arg, "-orioptiini"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptiinistring));
    else if (!strcmp (Arg, "-orioptifix"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptifixstring));
    else if (!strcmp (Arg, "-oridescriptor"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orides));
    else if (!strcmp (Arg, "-oricrysym"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).oricrysymstring));
    else if (!strcmp (Arg, "-oriformat"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).oriformat));
    else if (!strcmp (Arg, "-orioptilogvar"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptilogvarstring));
    else if (!strcmp (Arg, "-statcell"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stc));
    else if (!strcmp (Arg, "-statver"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stv));
    else if (!strcmp (Arg, "-statedge"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).ste));
    else if (!strcmp (Arg, "-statface"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stf));
    else if (!strcmp (Arg, "-statpoly"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stp));
    else if (!strcmp (Arg, "-statseed"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).sts));
    else if (!strcmp (Arg, "-statpoint"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stpt));
    else if (!strcmp (Arg, "-statvox"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stvox));
    else if (!strcmp (Arg, "-sort"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).sortstring));

/*----------------------------------------------------------------------
* load capability */
    else if ((!strcmp (Arg, "-loadtess")) || (!strcmp (Arg, "-checktess")))
    {
      ut_string_string ("tess", &(*pIn).input);
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).load));

      if (!strcmp (Arg, "-checktess"))
	(*pIn).checktess = 1;
    }
    else if (!strcmp (Arg, "-loadtesr"))
    {
      ut_string_string ("tesr", &(*pIn).input);
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).load));
    }
    else if (!strcmp (Arg, "-loadpoint"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).loadpoint));

    else
      ut_arg_badarg ();
  }

  ut_free_1d_char (Arg);
  ut_free_2d_char (ArgList, 101);

  return;
}

int
net_input_treatargs_multiscale (char *option, char **pargstring, int scaleqty,
				char ***pvals)
{
  int i, argqty, size;
  char **args = NULL;
  char* def = NULL;

  ut_string_string ((*pvals)[0], &def);

  if (!*pargstring && scaleqty <= 0)
    return 0;

  ut_string_separate (*pargstring, NEUT_SEP_FRAC, &args, &argqty);

  if (scaleqty > 0 && argqty > scaleqty)
    ut_print_message (2, 2, "Option `%s': number of scales exceeds %d.\n", option, scaleqty);

  if (scaleqty < 0)
    scaleqty = argqty;

  (*pvals) = ut_realloc_1d_pchar (*pvals, scaleqty + 1);
  // start at 1, not 0 (0 is already filled)
  for (i = 1; i <= scaleqty; i++)
    (*pvals)[i] = NULL;
  ut_string_string (def, *pvals);

  for (i = 1; i <= argqty; i++)
    ut_string_string (args[i - 1], (*pvals) + i);
  for (i = argqty + 1; i <= scaleqty; i++)
    ut_string_string ((*pvals)[0], (*pvals) + i);

  ut_free_1d_char (*pargstring);
  (*pargstring) = NULL;
  ut_string_string ("", pargstring);
  for (i = 1; i <= scaleqty; i++)
  {
    size = strlen (*pargstring) + strlen ((*pvals)[i]) + 1;
    if (i < scaleqty)
      size += strlen (NEUT_SEP_FRAC);

    (*pargstring) = ut_realloc_1d_char (*pargstring, size);
    (*pargstring) = strcat (*pargstring, (*pvals)[i]);

    if (i < scaleqty)
      (*pargstring) = strcat (*pargstring, NEUT_SEP_FRAC);
  }

  ut_free_2d_char (args, argqty);
  ut_free_1d_char (def);

  return argqty;
}
