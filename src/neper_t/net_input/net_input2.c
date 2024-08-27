/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input_.h"

void
net_input_treatargs (int fargc, char **fargv, int argc, char **argv,
                     struct IN_T *pIn)
{
  int i;

  net_input_options_default (pIn);

  // Reading options ---------------------------------------------------

  net_input_options_set (pIn, fargc, fargv);
  net_input_options_set (pIn, argc, argv);

  // Testing options ---------------------------------------------------

  if (!(*pIn).input)
    ut_print_message (2, 2, "Missing input data (`-n' or `-load*').\n");

  // Expanding args and setting to default values, when applicable -----

  // n
  (*pIn).levelqty =
    net_input_treatargs_multiscale ("-n", &(*pIn).nstring, -1, &((*pIn).n));

  if ((*pIn).levelqty > 0)
  {
    // id
    net_input_treatargs_multiscale ("-id", &(*pIn).idstring, (*pIn).levelqty,
                                    &((*pIn).id));

    // morpho
    net_input_treatargs_multiscale ("-morpho", &(*pIn).morphostring,
                                    (*pIn).levelqty, &((*pIn).morpho));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).morpho[i], "centroidal"))
        ut_string_string ("centroid:seed", &((*pIn).morpho[i]));

    for (i = 2; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).morpho[i], "tocta")
          || !strcmp ((*pIn).morpho[i], "square"))
        ut_print_message (2, 0,
                          "`-morpho `%s' is only available at level 1.\n",
                          (*pIn).morpho);
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strncmp ((*pIn).morpho[i], "square", 6) && (*pIn).dim != 2)
        ut_print_message (2, 2, "Option `-morpho square' requires `-dim 2'.");
    // testing consistency between morpho = lamellar and n, which must be from_morpho
    for (i = 1; i <= (*pIn).levelqty; i++)
      if ((!strncmp ((*pIn).morpho[i], "cube", 4))
          && strcmp ((*pIn).n[i], "from_morpho"))
        ut_print_message (2, 2, "`-morpho %s' requires `-n from_morpho'.\n",
                          (*pIn).morpho[i]);

    // optialgo
    for (i = 0; i < (*pIn).optiqty; i++)
      net_input_treatargs_multiscale (ut_string_paste3 ("-", (*pIn).optitype[i], "optialgo"),
                                      (*pIn).optialgostring + i,
                                      (*pIn).levelqty, (*pIn).optialgo + i);

    // replacing "default" by its value, for morpho and ori
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optialgo[0][i], "default"))
      {
        if (!strcmp ((*pIn).morpho[i], "centroid:seed"))
          ut_string_string ("lloyd(1.9)", &((*pIn).optialgo[0][i]));
        else
          ut_string_string ("subplex,praxis", &((*pIn).optialgo[0][i]));
      }

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optialgo[1][i], "default"))
        ut_string_string ("subplex,praxis", &((*pIn).optialgo[1][i]));

    // optidof
    for (i = 0; i < (*pIn).optiqty; i++)
      net_input_treatargs_multiscale (ut_string_paste3 ("-", (*pIn).optitype[i], "optidof"),
                                      (*pIn).optidofstring + i,
                                      (*pIn).levelqty, (*pIn).optidof + i);

    // optistop
    for (i = 0; i < (*pIn).optiqty; i++)
      net_input_treatargs_multiscale (ut_string_paste3 ("-", (*pIn).optitype[i], "optistop"),
                                      (*pIn).optistopstring + i,
                                      (*pIn).levelqty, (*pIn).optistop + i);

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optistop[0][i], "default"))
      {
        if (!strncmp ((*pIn).optialgo[0][i], "lloyd", 5))
          ut_string_string ("val=1e-4||iter=10000",
                            (*pIn).optistop[0] + i);
        else
          ut_string_string ("eps=1e-6", (*pIn).optistop[0] + i);
      }

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optistop[1][i], "default"))
        ut_string_string ("reps<1e-3||iter>=1e3", (*pIn).optistop[1] + i);

    // -----------------------

    // optiini
    net_input_treatargs_multiscale ("-morphooptiini",
                                    &(*pIn).optiinistring,
                                    (*pIn).levelqty, &((*pIn).optiini));

    // optialgomaxiter
    net_input_treatargs_multiscale ("-morphooptialgomaxiter",
                                    &(*pIn).optialgomaxiterstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optialgomaxiter));

    // optiobjective
    net_input_treatargs_multiscale ("-morphooptiobjective",
                                    &(*pIn).optiobjectivestring,
                                    (*pIn).levelqty,
                                    &((*pIn).optiobjective));

    // optigrid
    net_input_treatargs_multiscale ("-morphooptigrid",
                                    &(*pIn).optigridstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optigrid));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optigrid[i], "default"))
        ut_string_string
          ("diameq:regular(-1,10,1100),size:regular(-1,10,1100),sphericity:regular(-0.1,1.1,1200),1-sphericity:regular(-0.1,1.1,1200),ori:odf",
           (*pIn).optigrid + i);

    // optismooth
    net_input_treatargs_multiscale ("-morphooptismooth",
                                    &(*pIn).optismoothstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optismooth));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optismooth[i], "default"))
        ut_string_string
          ("diameq:0.05,size:0.05,sphericity:0.005,1-sphericity:0.005",
           (*pIn).optismooth + i);

    // optideltamax
    net_input_treatargs_multiscale ("-morphooptideltamax",
                                    &(*pIn).optideltamaxstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optideltamax));

    // optiinistep
    net_input_treatargs_multiscale ("-morphooptiinistep",
                                    &(*pIn).optiinistepstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optiinistep));

    // optilogtime
    net_input_treatargs_multiscale ("-morphooptilogtime",
                                    &(*pIn).optilogtimestring,
                                    (*pIn).levelqty,
                                    &((*pIn).optilogtime));

    // optilogvar
    net_input_treatargs_multiscale ("-morphooptilogvar",
                                    &(*pIn).optilogvarstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optilogvar));

    // optilogdis
    net_input_treatargs_multiscale ("-morphooptilogdis",
                                    &(*pIn).optilogdisstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optilogdis));

    // optilogtesr
    net_input_treatargs_multiscale ("-morphooptilogtesr",
                                    &(*pIn).optilogtesrstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optilogtesr));

    // optilogval
    net_input_treatargs_multiscale ("-morphooptilogval",
                                    &(*pIn).optilogvalstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optilogval));

    // optimultiseed
    net_input_treatargs_multiscale ("-morphooptimultiseed",
                                    &(*pIn).optimultiseedstring,
                                    (*pIn).levelqty,
                                    &((*pIn).optimultiseed));

    // ori
    net_input_treatargs_multiscale ("-ori", &(*pIn).oristring,
                                    (*pIn).levelqty, &((*pIn).ori));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).ori[i], "default"))
        ut_string_string ("random", (*pIn).ori + i);

    // orisampling
    net_input_treatargs_multiscale ("-orisampling", &(*pIn).orisamplingstring,
                                    (*pIn).levelqty, &((*pIn).orisampling));
    for (i = 1; i <= (*pIn).levelqty; i++)
    {
      if (!strcmp ((*pIn).orisampling[i], "default"))
        ut_string_string ("random", (*pIn).orisampling + i);
      else if (!strcmp ((*pIn).orisampling[i], "uniform"))
      {
        if (strcmp ((*pIn).ori[i], "random"))
          ut_print_message (2, 2, "`-orisampling uniform' only available with `-ori random'.\n");
      }
    }

    // oricrysym
    net_input_treatargs_multiscale ("-crysym", &(*pIn).crysymstring,
                                    (*pIn).levelqty, &((*pIn).crysym));

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).crysym[i], "default"))
        ut_string_string ("triclinic", (*pIn).crysym + i);

    // orioptineigh
    net_input_treatargs_multiscale ("-orioptineigh",
                                    &(*pIn).orioptineighstring,
                                    (*pIn).levelqty, &((*pIn).orioptineigh));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).orioptineigh[i], "default"))
        ut_string_string ("Nstar<10000?pi:20*dr", (*pIn).orioptineigh + i);

    // orioptiini
    net_input_treatargs_multiscale ("-orioptiini", &(*pIn).orioptiinistring,
                                    (*pIn).levelqty, &((*pIn).orioptiini));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).orioptiini[i], "default"))
        ut_string_string ("random", (*pIn).orioptiini + i);

    // orioptifix
    net_input_treatargs_multiscale ("-orioptifix", &(*pIn).orioptifixstring,
                                    (*pIn).levelqty, &((*pIn).orioptifix));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).orioptifix[i], "default"))
        ut_string_string ("none", (*pIn).orioptifix + i);

    // orioptilogvar
    net_input_treatargs_multiscale ("-orioptilogvar",
                                    &(*pIn).orioptilogvarstring,
                                    (*pIn).levelqty, &((*pIn).orioptilogvar));

    // orispread
    net_input_treatargs_multiscale ("-orispread", &(*pIn).orispreadstring,
                                    (*pIn).levelqty, &((*pIn).orispread));

    // Processing periodicstring & periodic
    (*pIn).periodic = ut_alloc_1d_int (3);
    if (strcmp ((*pIn).periodicstring, "none"))
    {
      int qty;
      char **vars = NULL;
      ut_list_break ((*pIn).periodicstring, NEUT_SEP_NODEP, &vars, &qty);

      for (i = 0; i < qty; i++)
      {
        if (!strcmp (vars[i], "1"))
          ut_array_1d_int_set ((*pIn).periodic, 3, 1);
        else if (!strcmp (vars[i], "all"))
          ut_array_1d_int_set ((*pIn).periodic, 3, 1);
        else if (!strcmp (vars[i], "0"))
          ut_array_1d_int_set ((*pIn).periodic, 3, 0);
        else if (!strcmp (vars[i], "none"))
          ut_array_1d_int_set ((*pIn).periodic, 3, 0);
        else if (!strcmp (vars[i], "x"))
          (*pIn).periodic[0] = 1;
        else if (!strcmp (vars[i], "y"))
          (*pIn).periodic[1] = 1;
        else if (!strcmp (vars[i], "z"))
          (*pIn).periodic[2] = 1;
        else
          abort ();
      }

      ut_free_2d_char (&vars, qty);

      if (!strncmp ((*pIn).domain, "rodrigues", 9))
        if (strcmp ((*pIn).n[1], "1"))
          ut_print_message (2, 2, "'-domain rodrigues -periodic' requires `-n 1'.\n");
    }

    for (i = (*pIn).dim; i < 3; i++)
      (*pIn).periodic[i] = 0;
  }

  if (!strcmp ((*pIn).domain, "default"))
  {
    if ((*pIn).dim == 3)
    {
      (*pIn).domain =
        ut_realloc_1d_char ((*pIn).domain, strlen ("cube(1,1,1)") + 1);
      strcpy ((*pIn).domain, "cube(1,1,1)");
    }
    else if ((*pIn).dim == 2)
    {
      (*pIn).domain =
        ut_realloc_1d_char ((*pIn).domain, strlen ("square(1,1)") + 1);
      strcpy ((*pIn).domain, "square(1,1)");
    }
    else if ((*pIn).dim == 1)
      ut_string_string ("segment(1)", &((*pIn).domain));
    else
      ut_print_neperbug ();
  }

  if ((*pIn).levelqty > 1 && ut_array_1d_int_sum ((*pIn).periodic, 3) != 0)
  {
    ut_print_messagewnc (2, 72,
                         "Option `-periodic' is not available for multiscale tessellations.");
    abort ();
  }

  if ((*pIn).reg == 1
      && ((*pIn).periodic && ut_array_1d_int_sum ((*pIn).periodic, 3) != 0))
  {
    ut_print_messagewnc (2, 72,
                         "Option `-regularization 1' is not available for periodic tessellations.");
    abort ();
  }

  if (((*pIn).dim == 3
       && (!strncmp ((*pIn).domain, "square", 6)
           || !strncmp ((*pIn).domain, "circle", 6)
           || !strncmp ((*pIn).domain, "stdtriangle", 11)))
      || ((*pIn).dim == 2
          && (!strncmp ((*pIn).domain, "cube", 4)
              || !strncmp ((*pIn).domain, "cylinder", 8)
              || !strncmp ((*pIn).domain, "sphere", 6)
              || !strncmp ((*pIn).domain, "rodrigues", 9))))
    ut_print_messagewnc (2, 72, "Options `-dim' and `-domain' conflict.");

  // Setting file names ------------------------------------------------

  if (!(*pIn).body)
  {
    (*pIn).body = ut_alloc_1d_char (1000);

    if (!strcmp ((*pIn).input, "n"))
      sprintf ((*pIn).body, "n%s-id%s", (*pIn).nstring, (*pIn).idstring);
    else if (!strcmp ((*pIn).input, "tess") || !strcmp ((*pIn).input, "tesr"))
      ut_string_body ((*pIn).load, ".", &((*pIn).body));
    else if (!strcmp ((*pIn).input, "n_reg"))
      sprintf ((*pIn).body, "n%s-%s", (*pIn).nstring, (*pIn).morpho[1]);
    else
      abort ();
  }

  (*pIn).tess = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).tesr = ut_string_addextension ((*pIn).body, ".tesr");
  (*pIn).vtk = ut_string_addextension ((*pIn).body, ".vtk");
  (*pIn).raw = ut_string_addextension ((*pIn).body, ".raw");
  (*pIn).orif = ut_string_addextension ((*pIn).body, ".ori");
  (*pIn).sim = ut_string_addextension ((*pIn).body, ".sim");
  (*pIn).geo = ut_string_addextension ((*pIn).body, ".geo");
  (*pIn).ply = ut_string_addextension ((*pIn).body, ".ply");
  (*pIn).stl = ut_string_addextension ((*pIn).body, ".stl");
  (*pIn).dec = ut_string_addextension ((*pIn).body, ".3dec");
  (*pIn).fe = ut_string_addextension ((*pIn).body, ".fe");
  (*pIn).obj = ut_string_addextension ((*pIn).body, ".obj");
  (*pIn).debug = ut_string_addextension ((*pIn).body, ".debug");
  (*pIn).mtess = ut_string_addextension ((*pIn).body, ".mtess");
  (*pIn).mgeo = ut_string_addextension ((*pIn).body, ".mgeo");

  return;
}
