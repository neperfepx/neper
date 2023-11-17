/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input_.h"

/* SetDefaultOptions set the options to their default values */
void
net_input_options_default (struct IN_T *pIn)
{
  int i;

  (*pIn).dim = 3;
  ut_string_string ("default", &((*pIn).domain));
  ut_string_string ("none", &((*pIn).periodicstring));
  ut_string_string ("20", &((*pIn).tesrsizestring));

  (*pIn).optiqty = 2;
  (*pIn).optitype = ut_alloc_1d_pchar ((*pIn).optiqty);
  ut_string_string ("morpho", (*pIn).optitype);
  ut_string_string ("ori", (*pIn).optitype + 1);

  (*pIn).nstring = NULL;        // needed for proper levelqty determination
  ut_string_string ("1", &(*pIn).idstring);
  ut_string_string ("voronoi", &(*pIn).morphostring);

  // new -----------------
  (*pIn).optialgostring = ut_alloc_1d_pchar (2);
  for (i = 0; i < (*pIn).optiqty; i++)
    ut_string_string ("default", (*pIn).optialgostring + i);

  (*pIn).optidofstring = ut_alloc_1d_pchar (2);
  for (i = 0; i < (*pIn).optiqty; i++)
    ut_string_string ("default", (*pIn).optidofstring + i);

  (*pIn).optistopstring = ut_alloc_1d_pchar (2);
  for (i = 0; i < (*pIn).optiqty; i++)
    ut_string_string ("default", (*pIn).optistopstring + i);
  // end new -------------

  ut_string_string ("default", &(*pIn).optiinistring);
  ut_string_string ("max(varnb,1000)", &(*pIn).optialgomaxiterstring);
  ut_string_string ("default", &(*pIn).optiobjectivestring);
  ut_string_string ("default", &(*pIn).optigridstring);
  ut_string_string ("default", &(*pIn).optismoothstring);
  ut_string_string ("none", &(*pIn).optilogtimestring);
  ut_string_string ("none", &(*pIn).optilogvarstring);
  ut_string_string ("none", &(*pIn).optilogdisstring);
  ut_string_string ("none", &(*pIn).optilogtesrstring);
  ut_string_string ("none", &(*pIn).optilogvalstring);
  ut_string_string ("1", &(*pIn).optimultiseedstring);
  ut_string_string ("HUGE_VAL", &(*pIn).optideltamaxstring);
  ut_string_string ("default", &(*pIn).optiinistepstring);

  (*pIn).reg = 0;
  (*pIn).fmax = 20;
  ut_string_string ("rel", &((*pIn).seltype));
  ut_string_string ("1", &((*pIn).selstring));
  (*pIn).mloop = 2;
  (*pIn).maxedgedelqty = INT_MAX;
  ut_string_string ("none", &(*pIn).sortstring);        // to change into sort
  ut_string_string ("none", &(*pIn).transform);
  (*pIn).checktess = 0;
  ut_string_string ("none", &(*pIn).load);
  ut_string_string ("none", &(*pIn).loadpoint);

  ut_string_string ("none", &(*pIn).format);
  ut_string_string ("binary16", &(*pIn).tesrformat);

  ut_string_string ("default", &(*pIn).oristring);
  ut_string_string ("random", &(*pIn).orisamplingstring);
  ut_string_string ("none", &(*pIn).orispreadstring);
  ut_string_string ("default", &(*pIn).orioptineighstring);
  ut_string_string ("default", &(*pIn).orioptiinistring);
  ut_string_string ("default", &(*pIn).orioptifixstring);
  ut_string_string ("default", &((*pIn).crysymstring));
  ut_string_string ("none", &(*pIn).orioptilogvarstring);

  ut_string_string (NEUT_DEFAULT_ORIDESFULL, &((*pIn).orides));
  ut_string_string ("plain", &((*pIn).oriformat));

  // copying defaults to first line of multiscale parameters
  (*pIn).n = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).nstring, (*pIn).n);
  (*pIn).id = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).idstring, (*pIn).id);
  (*pIn).morpho = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).morphostring, (*pIn).morpho);
  (*pIn).optiini = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optiinistring, (*pIn).optiini);

  // new ------------
  (*pIn).optialgo = ut_alloc_2d_pchar ((*pIn).optiqty, 1);
  for (i = 0; i < (*pIn).optiqty; i++)
    ut_string_string ((*pIn).optialgostring[i], (*pIn).optialgo[i]);

  (*pIn).optidof = ut_alloc_2d_pchar ((*pIn).optiqty, 1);
  for (i = 0; i < (*pIn).optiqty; i++)
    ut_string_string ((*pIn).optidofstring[i], (*pIn).optidof[i]);

  (*pIn).optistop = ut_alloc_2d_pchar ((*pIn).optiqty, 1);
  for (i = 0; i < (*pIn).optiqty; i++)
    ut_string_string ((*pIn).optistopstring[i], (*pIn).optistop[i]);
  // end new --------

  (*pIn).optialgomaxiter = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optialgomaxiterstring,
                    (*pIn).optialgomaxiter);
  (*pIn).optiobjective = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optiobjectivestring,
                    (*pIn).optiobjective);
  (*pIn).optigrid = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optigridstring, (*pIn).optigrid);
  (*pIn).optismooth = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optismoothstring, (*pIn).optismooth);
  (*pIn).optilogtime = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optilogtimestring, (*pIn).optilogtime);
  (*pIn).optilogvar = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optilogvarstring, (*pIn).optilogvar);
  (*pIn).optilogdis = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optilogdisstring, (*pIn).optilogdis);
  (*pIn).optilogtesr = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optilogtesrstring, (*pIn).optilogtesr);
  (*pIn).optilogval = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optilogvalstring, (*pIn).optilogval);
  (*pIn).optimultiseed = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optimultiseedstring,
                    (*pIn).optimultiseed);
  (*pIn).optideltamax = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optideltamaxstring,
                    (*pIn).optideltamax);
  (*pIn).optiinistep = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).optiinistepstring, (*pIn).optiinistep);

  (*pIn).ori = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).ori);
  (*pIn).orisampling = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).orisamplingstring, (*pIn).orisampling);
  (*pIn).crysym = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).crysymstring, (*pIn).crysym);
  (*pIn).orioptineigh = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).orioptineigh);
  (*pIn).orioptiini = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).oristring, (*pIn).orioptiini);
  (*pIn).orioptifix = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).orioptifixstring, (*pIn).orioptifix);
  (*pIn).orioptilogvar = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).orioptilogvarstring, (*pIn).orioptilogvar);
  (*pIn).orispread = ut_alloc_1d_pchar (1);
  ut_string_string ((*pIn).orispreadstring, (*pIn).orispread);

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
  strcpy (ArgList[++ArgQty], "-ori");

  for (i = 0; i < (*pIn).optiqty; i++)
    strcpy (ArgList[++ArgQty], ut_string_paste3 ("-", (*pIn).optitype[i], "optialgo"));

  strcpy (ArgList[++ArgQty], "-morphooptiini");
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

  strcpy (ArgList[++ArgQty], "-orioptidof");

  strcpy (ArgList[++ArgQty], "-orisampling");
  strcpy (ArgList[++ArgQty], "-orioptistop");
  strcpy (ArgList[++ArgQty], "-orioptineigh");
  strcpy (ArgList[++ArgQty], "-orioptiini");
  strcpy (ArgList[++ArgQty], "-orioptifix");
  strcpy (ArgList[++ArgQty], "-oridescriptor");
  strcpy (ArgList[++ArgQty], "-oricrysym");
  strcpy (ArgList[++ArgQty], "-oriformat");
  strcpy (ArgList[++ArgQty], "-orioptilogvar");
  strcpy (ArgList[++ArgQty], "-orispread");

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
  strcpy (ArgList[++ArgQty], "-group");
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

  strcpy (ArgList[++ArgQty], "-crysym");
  // Post-processing ---------------------------------------------------
  strcpy (ArgList[++ArgQty], "-stattess");
  strcpy (ArgList[++ArgQty], "-stattesr");
  strcpy (ArgList[++ArgQty], "-statver");
  strcpy (ArgList[++ArgQty], "-statedge");
  strcpy (ArgList[++ArgQty], "-statface");
  strcpy (ArgList[++ArgQty], "-statpoly");
  strcpy (ArgList[++ArgQty], "-statcell");
  strcpy (ArgList[++ArgQty], "-statseed");
  strcpy (ArgList[++ArgQty], "-statpoint");
  strcpy (ArgList[++ArgQty], "-statvox");
  strcpy (ArgList[++ArgQty], "-statgroup");
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
    Res = ut_string_completion (argv[i], ArgList, ArgQty, &Arg);
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
        printf
          ("Option `-scale' was removed; use `-transform scale' instead.\n");
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
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optiinistring));
    else if (!strcmp (Arg, "-morphooptialgo"))
      ut_arg_nextasstring (argv, &i, Arg, (*pIn).optialgostring);
    else if (!strcmp (Arg, "-morphooptialgomaxiter"))
      ut_arg_nextasstring (argv, &i, Arg,
                           &((*pIn).optialgomaxiterstring));
    else if (!strcmp (Arg, "-morphooptiobjective"))
      ut_arg_nextasstring (argv, &i, Arg,
                           &((*pIn).optiobjectivestring));
    else if (!strcmp (Arg, "-morphooptigrid"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optigridstring));
    else if (!strcmp (Arg, "-morphooptismooth"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optismoothstring));
    else if (!strcmp (Arg, "-morphooptistop"))
      ut_arg_nextasstring (argv, &i, Arg, (*pIn).optistopstring);
    else if (!strcmp (Arg, "-morphooptilogtime"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optilogtimestring));
    else if (!strcmp (Arg, "-morphooptilogvar"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optilogvarstring));
    else if (!strcmp (Arg, "-morphooptilogdis"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optilogdisstring));
    else if (!strcmp (Arg, "-morphooptilogtesr"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optilogtesrstring));
    else if (!strcmp (Arg, "-morphooptilogval"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optilogvalstring));
    else if (!strcmp (Arg, "-morphooptidof"))
      ut_arg_nextasstring (argv, &i, Arg, ((*pIn).optidofstring));
    else if (!strcmp (Arg, "-morphooptideltamax"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optideltamaxstring));
    else if (!strcmp (Arg, "-morphooptimultiseed"))
      ut_arg_nextasstring (argv, &i, Arg,
                           &((*pIn).optimultiseedstring));
    else if (!strcmp (Arg, "-morphooptiinistep"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).optiinistepstring));
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
    else if (!strcmp (Arg, "-group"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).group));

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
 * orientation options */

    else if (!strcmp (Arg, "-ori"))
    {
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).oristring));
      if (strstr ((*pIn).oristring, "uniform"))
        ut_print_message (2, 3, "`uniform' has been moved to option `-orisampling'.\n");
    }

    // new ----------------------
    else if (!strcmp (Arg, "-orioptialgo"))
      ut_arg_nextasstring (argv, &i, Arg, (*pIn).optialgostring + 1);
    else if (!strcmp (Arg, "-orioptidof"))
      ut_arg_nextasstring (argv, &i, Arg, (*pIn).optidofstring + 1);
    else if (!strcmp (Arg, "-orioptistop"))
      ut_arg_nextasstring (argv, &i, Arg, (*pIn).optistopstring + 1);
    // end new ------------------

    else if (!strcmp (Arg, "-orisampling"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orisamplingstring));
    else if (!strcmp (Arg, "-orioptineigh"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptineighstring));
    else if (!strcmp (Arg, "-orioptiini"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptiinistring));
    else if (!strcmp (Arg, "-orioptifix"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptifixstring));
    else if (!strcmp (Arg, "-oridescriptor"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orides));
    else if (!strcmp (Arg, "-oricrysym") || !strcmp (Arg, "-crysym"))
    {
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).crysymstring));
      if (!strcmp (Arg, "-oricrysym"))
        ut_print_message (1, 2, "Option `-oricrysym' will be removed in a future version.  Use `-crysym` instead.\n");
    }
    else if (!strcmp (Arg, "-oriformat"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).oriformat));
    else if (!strcmp (Arg, "-orioptilogvar"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orioptilogvarstring));
    else if (!strcmp (Arg, "-orispread"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orispreadstring));

/*----------------------------------------------------------------------
* post-processing options */
    else if (!strcmp (Arg, "-format"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).format));
    else if (!strcmp (Arg, "-tesrformat"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).tesrformat));
    else if (!strcmp (Arg, "-tesrsize"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).tesrsizestring));

    else if (!strcmp (Arg, "-statcell"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stc));
    else if (!strcmp (Arg, "-stattess"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).sttess));
    else if (!strcmp (Arg, "-stattesr"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).sttesr));
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
    else if (!strcmp (Arg, "-statgroup"))
      ut_arg_nextasstring (argv, &i, Arg, &((*pIn).stgroup));
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

  ut_free_1d_char (&Arg);
  ut_free_2d_char (&ArgList, 101);

  return;
}

int
net_input_treatargs_multiscale (char *option, char **pargstring, int scaleqty,
                                char ***pvals)
{
  int i, argqty, size;
  char **args = NULL;
  char *def = NULL;

  ut_string_string ((*pvals)[0], &def);

  if (!*pargstring && scaleqty <= 0)
    return 0;

  ut_list_break (*pargstring, NEUT_SEP_FRAC, &args, &argqty);

  if (scaleqty > 0 && argqty > scaleqty)
    ut_print_message (2, 2, "Option `%s': number of scales exceeds %d.\n",
                      option, scaleqty);

  if (scaleqty < 0)
    scaleqty = argqty;

  // start null at 1, not 0 (0 is already filled)
  (*pvals) = ut_realloc_1d_pchar_null (*pvals, scaleqty + 1, scaleqty);
  ut_string_string (def, *pvals);

  for (i = 1; i <= argqty; i++)
    ut_string_string (args[i - 1], (*pvals) + i);
  for (i = argqty + 1; i <= scaleqty; i++)
    ut_string_string ((*pvals)[0], (*pvals) + i);

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

  ut_free_2d_char (&args, argqty);
  ut_free_1d_char (&def);

  return argqty;
}
