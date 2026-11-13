/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"net_input_.h"

void
net_input_treatargs (int fargc, char **fargv, int argc, char **argv,
                     struct IN_T *pIn)
{
  int i;
  char *option = NULL;

  net_input_options_default (pIn);

  // Reading options ---------------------------------------------------

  net_input_options_set (pIn, fargc, fargv);
  net_input_options_set (pIn, argc, argv);

  // Testing options ---------------------------------------------------

  /*
  if (!strcmp ((*pIn).morpho))
    ut_string_string ("from_morpho", (*pIn).nstring);
  */

  /*
  if (!(*pIn).input)
    ut_print_message (2, 2, "Missing input data (`-n' or `-load*').\n");
  */

  // Expanding args and setting to default values, when applicable -----

  // n
  if (!strcmp ((*pIn).load, "none"))
  {
    if ((*pIn).nstring)
      (*pIn).levelqty =
        net_input_treatargs_multiscale ("-n", &(*pIn).nstring, -1, &((*pIn).n));
    else if (strcmp ((*pIn).morphostring, "voronoi"))
    {
      // morpho is expanded below
      ut_list_break ((*pIn).morphostring, NEUT_SEP_FRAC, NULL, &(*pIn).levelqty);
      if ((*pIn).levelqty == 1)
        ut_string_string ("from_morpho", &(*pIn).nstring);
      ut_string_string ("n", &(*pIn).input);
      net_input_treatargs_multiscale ("-n", &(*pIn).nstring, -1, &((*pIn).n));
    }
    else
      abort ();
  }
  else
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
      if ((!strncmp ((*pIn).morpho[i], "square", 6) || !strncmp ((*pIn).morpho[i], "hex", 3)) && (*pIn).dim != 2)
        ut_print_message (2, 2, "Option `-morpho square|hex' requires `-dim 2'.");
    // testing consistency between morpho = lamellar and n, which must be from_morpho
    /*
    for (i = 1; i <= (*pIn).levelqty; i++)
      if ((!strncmp ((*pIn).morpho[i], "cube", 4))
          && strcmp ((*pIn).n[i], "from_morpho"))
        ut_print_message (2, 2, "`-morpho %s' requires `-n from_morpho'.\n",
                          (*pIn).morpho[i]);
    */

    // optialgo
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optialgo");
      net_input_treatargs_multiscale (option, (*pIn).optialgostring + i,
                                      (*pIn).levelqty, (*pIn).optialgo + i);
      ut_free_1d_char (&option);
    }

    // replacing "default" by its value, for morpho and ori
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optialgo[0][i], "default"))
        net_input_ternary_map ((*pIn).morpho[i], "centroid:seed,centroidal",
                               "lloyd(1.9)", "subplex",
                               &((*pIn).optialgo[0][i]));

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optialgo[1][i], "default"))
        ut_string_string ("subplex", &((*pIn).optialgo[1][i]));

    // optidof
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optidof");
      net_input_treatargs_multiscale (option, (*pIn).optidofstring + i,
                                      (*pIn).levelqty, (*pIn).optidof + i);
      ut_free_1d_char (&option);
    }

    // optiini
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optiini");
      net_input_treatargs_multiscale (option, (*pIn).optiinistring + i,
                                      (*pIn).levelqty, (*pIn).optiini + i);
      ut_free_1d_char (&option);
    }

    // replacing "default" by its value
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optiini[1][i], "default"))
        ut_string_string ("random", (*pIn).optiini[1] + i);

    // optineigh
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optineigh");
      net_input_treatargs_multiscale (option, (*pIn).optineighstring + i,
                                      (*pIn).levelqty, (*pIn).optineigh + i);
      ut_free_1d_char (&option);
    }

    // replacing "default" by its value
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optineigh[1][i], "default"))
        ut_string_string ("Nstar<10000?pi:20*dr", (*pIn).optineigh[1] + i);

    // optifix
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optifix");
      net_input_treatargs_multiscale (option, (*pIn).optifixstring + i,
                                      (*pIn).levelqty, (*pIn).optifix + i);
      ut_free_1d_char (&option);
    }

    // replacing "default" by its value
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optifix[1][i], "default"))
        ut_string_string ("none", (*pIn).optifix[1] + i);

    // optistop
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optistop");
      net_input_treatargs_multiscale (option, (*pIn).optistopstring + i,
                                      (*pIn).levelqty, (*pIn).optistop + i);
      ut_free_1d_char (&option);
    }

    // optiinistep
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optiinistep");
      net_input_treatargs_multiscale (option, (*pIn).optiinistepstring + i,
                                      (*pIn).levelqty, (*pIn).optiinistep + i);
      ut_free_1d_char (&option);
    }

    // optideltamax
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optideltamax");
      net_input_treatargs_multiscale (option, (*pIn).optideltamaxstring + i,
                                      (*pIn).levelqty, (*pIn).optideltamax + i);
      ut_free_1d_char (&option);
    }

    // optiboundlower
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optiboundlower");
      net_input_treatargs_multiscale (option, (*pIn).optiboundlstring + i,
                                      (*pIn).levelqty, (*pIn).optiboundl + i);
      ut_free_1d_char (&option);
    }

    // optiboundupper
    for (i = 0; i < (*pIn).optiqty; i++)
    {
      option = ut_string_paste3 ("-", (*pIn).optitype[i], "optiboundupper");
      net_input_treatargs_multiscale (option, (*pIn).optiboundustring + i,
                                      (*pIn).levelqty, (*pIn).optiboundu + i);
      ut_free_1d_char (&option);
    }

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optistop[0][i], "default"))
        net_input_ternary_map ((*pIn).optialgo[0][i], "lloyd*",
                               "val=1e-4||iter=10000", "eps=1e-6||val=1e-12",
                               (*pIn).optistop[0] + i);

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).optistop[1][i], "default"))
        ut_string_string ("thomson:reps<1e-3||iter>=1e3,general:eps<1e-6||val<1e-12", (*pIn).optistop[1] + i);

    // -----------------------

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
          ("diameq:regular(-1,10,1100),size:regular(-1,100,5050),sphericity:regular(-0.1,1.1,1200),1-sphericity:regular(-0.1,1.1,1200),ori:odf,anisofact:regular(0,10,1000),anisofact-1:regular(-1,10,1100)",
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

    // optilogvar
    net_input_treatargs_multiscale ("-morphooptilogvar", (*pIn).optilogvarstring, (*pIn).levelqty, ((*pIn).optilogvar));
    net_input_treatargs_multiscale ("-orioptilogvar", (*pIn).optilogvarstring + 1, (*pIn).levelqty, (*pIn).optilogvar + 1);

    // optilogval
    net_input_treatargs_multiscale ("-morphooptilogval", (*pIn).optilogvalstring, (*pIn).levelqty, (*pIn).optilogval);
    net_input_treatargs_multiscale ("-orioptilogval", (*pIn).optilogvalstring + 1, (*pIn).levelqty, (*pIn).optilogval + 1);

    // optilogtime
    net_input_treatargs_multiscale ("-morphooptilogtime", &(*pIn).optilogtimestring, (*pIn).levelqty, &((*pIn).optilogtime));

    // optilogdis
    net_input_treatargs_multiscale ("-morphooptilogdis", &(*pIn).optilogdisstring, (*pIn).levelqty, &((*pIn).optilogdis));

    // optilogtesr
    net_input_treatargs_multiscale ("-morphooptilogtesr", &(*pIn).optilogtesrstring, (*pIn).levelqty, &((*pIn).optilogtesr));

    // optimultiseed
    net_input_treatargs_multiscale ("-morphooptimultiseed", &(*pIn).optimultiseedstring, (*pIn).levelqty, &((*pIn).optimultiseed));

    // morphosampling
    net_input_treatargs_multiscale ("-morphosampling", &(*pIn).morphosamplingstring,
                                    (*pIn).levelqty, &((*pIn).morphosampling));
    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).morphosampling[i], "default"))
        ut_string_string ("random", (*pIn).morphosampling + i);

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
      if (!strcmp ((*pIn).orisampling[i], "default"))
        ut_string_string ("random", (*pIn).orisampling + i);

    // oricrysym
    net_input_treatargs_multiscale ("-crysym", &(*pIn).crysymstring,
                                    (*pIn).levelqty, &((*pIn).crysym));

    for (i = 1; i <= (*pIn).levelqty; i++)
      if (!strcmp ((*pIn).crysym[i], "default"))
        ut_string_string ("triclinic", (*pIn).crysym + i);

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
    {
      ut_string_body ((*pIn).load, ".", &((*pIn).body));
      ut_string_fnrs ((*pIn).body, "file(", "", 1);
    }
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
  (*pIn).ovm = ut_string_addextension ((*pIn).body, ".ovm");
  (*pIn).svg = ut_string_addextension ((*pIn).body, ".svg");
  (*pIn).debug = ut_string_addextension ((*pIn).body, ".debug");
  (*pIn).mtess = ut_string_addextension ((*pIn).body, ".mtess");
  (*pIn).mgeo = ut_string_addextension ((*pIn).body, ".mgeo");

  return;
}

// Uniform sampling only applies to the random parts of -ori and to an -ori
// starting by odf in tess mode (see net_ori and net_tess); warning otherwise.
void
net_input_orisampling_check (struct IN_T In)
{
  int i, j, partqty;
  char **parts = NULL, *part = NULL;

  if (strcmp (In.load, "none") || !In.ori || !In.orisampling)
    return;

  for (i = 1; i <= In.levelqty; i++)
  {
    if (strcmp (In.orisampling[i], "uniform") || strstr (In.optiini[1][i], "ori:"))
      continue;

    ut_list_break (In.ori[i], "+", &parts, &partqty);

    for (j = 0; j < partqty; j++)
    {
      part = strrchr (parts[j], '*') ? strrchr (parts[j], '*') + 1 : parts[j];

      if (strcmp (part, "random")
          && (j || strncmp (In.ori[i], "odf", 3) || strcmp (In.mode, "tess")))
        ut_print_message (1, 2, "Option `-orisampling uniform' has no effect on `%s'.\n", part);
    }

    ut_free_2d_char (&parts, partqty);
  }

  return;
}

// -orioptiini is either random, file(...) or a list of <attribute>:<value>
void
net_input_orioptiini_check (struct IN_T In)
{
  int i, j, qty, *qty1 = NULL;
  char ***parts = NULL;

  if (strcmp (In.load, "none") || !In.ori)
    return;

  for (i = 1; i <= In.levelqty; i++)
  {
    if (!strcmp (In.optiini[1][i], "random") || ut_string_isfilename (In.optiini[1][i]))
      continue;

    ut_list_break2 (In.optiini[1][i], NEUT_SEP_NODEP, NEUT_SEP_DEP, &parts, &qty1, &qty);

    for (j = 0; j < qty; j++)
      if (qty1[j] != 2
          || (strcmp (parts[j][0], "ori") && strcmp (parts[j][0], "theta")
              && strcmp (parts[j][0], "weight")))
        ut_print_message (2, 2,
                          "Option `-orioptiini': failed to process `%s' (should be `random' or use `ori:', `theta:' or `weight:').\n",
                          In.optiini[1][i]);

    ut_free_3d_char (&parts, qty, 2);
    ut_free_1d_int (&qty1);
  }

  return;
}
