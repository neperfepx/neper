/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nes_input_.h"

/* SetDefaultOptions set the options to their default values */
void
nes_input_options_default (struct IN_S *pIn)
{
  (*pIn).fepxdir = NULL;
  (*pIn).simdir = NULL;
  ut_string_string ("none", &((*pIn).orispace));

  return;
}

void
nes_input_options_set (struct IN_S *pIn, int argc, char **argv)
{
  int i, readinput = 0, status, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = ut_alloc_1d_char (101);
  char *type = NULL;

  /* This is the possible argument list. */
  ArgQty = 4;
  sprintf (ArgList[++ArgQty], "-o");
  sprintf (ArgList[++ArgQty], "-orispace");
  sprintf (ArgList[++ArgQty], "-entity");
  sprintf (ArgList[++ArgQty], "-step");

  for (i = 1; i <= argc - 1; i++)
    if (argv[i][0] != '-')
    {
      if (readinput)
        ut_print_message (2, 0, "Input directory already loaded.\n");

      status = neut_sim_name_type (argv[i], &type, NULL, NULL);

      if (status == -1)
        ut_print_message (2, 2, "Failed to parse directory.\n");

      if (!strcmp (type, "fepx"))
        ut_string_string (argv[i], &(*pIn).fepxdir);
      else if (!strcmp (type, "sim"))
      {
        if ((*pIn).simdir)
          ut_print_messagewnc (2, 72, "Using `-o' and loading a simulation directory as input are mutually exclusive.\n");
        ut_string_string (argv[i], &(*pIn).simdir);
      }
      else if (!strcmp (type, "merge"))
        ut_string_string (argv[i], &(*pIn).mergedir);
      else
        abort ();

      readinput = 1;
    }
    else
    {
      Res = ut_string_completion (argv[i], ArgList, ArgQty, &Arg);
      if (Res == 1)
      {
        ut_print_lineheader (2);
        printf ("Several possibilities for option `%s'.\n", argv[i]);
        ut_arg_badarg ();
      }
      else if (Res == -1)
      {
        if (!strncmp (argv[i], "-res", 4))
          ut_string_string (argv[i], &Arg);
        else
        {
          ut_print_lineheader (2);
          printf ("Unknown option `%s'.\n", argv[i]);
          ut_arg_badarg ();
        }
      }

      if (!strcmp (Arg, "-o"))
      {
        if (!ut_string_strcmp (type, "sim"))
          ut_print_messagewnc (2, 72, "Using `-o' and loading a simulation directory as input are mutually exclusive.\n");

        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).simdir));
      }
      else if (!strcmp (Arg, "-orispace"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).orispace));
      else if (!strncmp (Arg, "-res", 4))
        nes_in_addres (Arg, argv[++i], pIn);
      else if (!strcmp (Arg, "-entity"))
        ut_arg_nextasstring (argv, &i, Arg, &((*pIn).entity));
      else if (!strcmp (Arg, "-step"))
        ut_arg_nextasint (argv, &i, Arg, 0, INT_MAX, &((*pIn).stepqty));
      else
        ut_arg_badarg ();
    }

  ut_free_2d_char (&ArgList, 101);
  ut_free_1d_char (&Arg);
  ut_free_1d_char (&type);

  return;
}
