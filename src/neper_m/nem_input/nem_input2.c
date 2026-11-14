/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
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

  // Checking out the elttype value: internally, tri for free meshing (tri
  // elts, or quad elts obtained by splitting tri elts) and quad or quad9 for
  // grid meshing
  if (!strcmp ((*pIn).elttype, "quad9"))
  {
    ut_print_message (1, 2,
                      "Elt type `quad9' is deprecated and may vanish in future versions; use `quad9grid' instead.\n");
    ut_string_string ("quad9grid", &(*pIn).elttype);
  }

  if (!strcmp ((*pIn).elttype, "tri") || !strcmp ((*pIn).elttype, "tet"))
    ut_string_string ("tri", &(*pIn).elttype);
  else if (!strcmp ((*pIn).elttype, "hexgrid")
           || !strcmp ((*pIn).elttype, "quadgrid"))
    ut_string_string ("quad", &(*pIn).elttype);
  else if (!strcmp ((*pIn).elttype, "quad9grid"))
  {
    ut_string_string ("quad9", &(*pIn).elttype);

    if ((*pIn).order == 1)
      ut_print_message (2, 0, "Elt type `quad9grid' needs `-order 2`.\n");
  }
  // free meshing into tri elts, which are then split
  else if (!strcmp ((*pIn).elttype, "hex") || !strcmp ((*pIn).elttype, "quad"))
  {
    ut_string_string ("tri", &(*pIn).elttype);
    (*pIn).eltsplit = 1;

    if (strcmp ((*pIn).interface, "continuous"))
      ut_print_message (2, 0, "Option `-interface' is not available for elt type `hex' or `quad'.\n");
  }
  else
  {
    ut_print_message (2, 0, "Unknown elt type `%s'.\n", (*pIn).elttype);
    abort ();
  }

  // Checking out the format values
  {
    int i, j, formatqty;
    char **formats = NULL;
    const char *valid[] = { "msh", "msh:ascii", "msh:binary", "sim", "vtk", "inp", "abq",
                            "geof", "ori", "phase", "bcs", "per", "tess", "etree", "ntree" };
    int validqty = sizeof (valid) / sizeof (valid[0]);

    ut_list_break ((*pIn).format, NEUT_SEP_NODEP, &formats, &formatqty);

    for (i = 0; i < formatqty; i++)
    {
      for (j = 0; j < validqty; j++)
        if (!strcmp (formats[i], valid[j]))
          break;

      if (j == validqty)
        ut_print_message (2, 0, "Unknown format `%s'.\n", formats[i]);
    }

    ut_free_2d_char (&formats, formatqty);
  }

  if (!strcmp ((*pIn).faset, "all"))
    ut_string_string ("faces", &(*pIn).faset);

  // -part: <n> or <n>:<m>, with <n> and <m> positive integers (-part 0: no partitioning)
  if ((*pIn).partstring)
  {
    int i, qty, status = 0;
    char **list = NULL;
    char *format = ut_alloc_1d_char (10);

    ut_list_break ((*pIn).partstring, NEUT_SEP_DEP, &list, &qty);

    for (i = 0; i < qty; i++)
    {
      ut_string_format (list[i], format);
      if (strcmp (format, "%d") || atoi (list[i]) < (qty == 1 ? 0 : 1))
        status = -1;
    }

    if (qty < 1 || qty > 2 || status)
      ut_print_message (2, 0, "Option -part: `%s' is not valid (expected <n> or <n>:<m>).\n",
                        (*pIn).partstring);

    ut_free_2d_char (&list, qty);
    ut_free_1d_char (&format);
  }

  /* Recording body */
  if (!(*pIn).body)
  {
    if ((*pIn).loadmesh)
      ut_string_body ((*pIn).loadmesh, ".", &((*pIn).body));
    else if ((*pIn).tess && strlen ((*pIn).tess))
      ut_string_body ((*pIn).tess, ".", &((*pIn).body));
    else if ((*pIn).tesr && strlen ((*pIn).tesr))
      ut_string_body ((*pIn).tesr, ".", &((*pIn).body));
    else if ((*pIn).mesh && strlen ((*pIn).mesh))
      ut_string_body ((*pIn).mesh, ".", &((*pIn).body));
  }

  /* Writing all file names */
  if ((*pIn).body)
  {
    (*pIn).geof = ut_string_addextension ((*pIn).body, ".geof");
    (*pIn).msh = ut_string_addextension ((*pIn).body, ".msh");
    (*pIn).per = ut_string_addextension ((*pIn).body, ".per");
    (*pIn).abq = ut_string_addextension ((*pIn).body, ".inp");
    (*pIn).ori = ut_string_addextension ((*pIn).body, ".ori");
    (*pIn).phase = ut_string_addextension ((*pIn).body, ".phase");
    (*pIn).bcs = ut_string_addextension ((*pIn).body, ".bcs");
    (*pIn).vtk = ut_string_addextension ((*pIn).body, ".vtk");
    (*pIn).outtess = ut_string_addextension ((*pIn).body, ".tess");
    (*pIn).intf = ut_string_addextension ((*pIn).body, ".intf");
    (*pIn).sim = ut_string_addextension ((*pIn).body, ".sim");
    (*pIn).etree = ut_string_addextension ((*pIn).body, ".etree");
    (*pIn).ntree = ut_string_addextension ((*pIn).body, ".ntree");
  }

  return;
}
