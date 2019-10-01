/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neper_.h"
#include"neper_config.h"

void neper_head (char *);
void neper_foot (void);
void neper_info (void);

int
main (int argc, char **argv)
{
  int modqty, status, fargc;
  double t;
  char **modlist = NULL, *mod = NULL, **fargv = NULL, *rcfile = NULL;
  struct timeval beg_time, end_time;

  gettimeofday (&beg_time, NULL);

  // Module list -------------------------------------------------------

  modlist = ut_alloc_2d_char (10, 10);
  modqty = 0;
  strcpy (modlist[++modqty], "--help");
  strcpy (modlist[++modqty], "--version");
  strcpy (modlist[++modqty], "--license");
  strcpy (modlist[++modqty], "--rcfile");
  strcpy (modlist[++modqty], "--test");
  strcpy (modlist[++modqty], "-T");
  strcpy (modlist[++modqty], "-M");
  strcpy (modlist[++modqty], "-V");

  // Testing input -----------------------------------------------------

  if (argc > 1)
  {
    status = ut_string_comp (argv[1], modlist, modqty, &mod);

    if (status == 1)
    {
      ut_print_lineheader (2);
      printf ("Several possibilities for option `%s'.\n", argv[1]);
      ut_arg_badarg ();
    }
    else if (status == -1)
    {
      ut_print_lineheader (2);
      printf ("Unknown option `%s'.\n", argv[1]);
      ut_arg_badarg ();
    }
  }

  // Processing input --------------------------------------------------

  if (argc == 1 || (argc >= 2 && !strcmp (mod, "--help")))
  {
    neper_head (NEPER_VERSION);
    neper_info ();
    neper_foot ();
  }

  else if (argc >= 2 && !strcmp (mod, "--version"))
    printf ("%s\n", NEPER_VERSION);

  else if (argc >= 2 && !strcmp (mod, "--license"))
    ut_print_gplv3 (stdout);

  else if (argc >= 2 && !strcmp (mod, "--test"))
  {
    int largc = 5;
    char **largv = ut_alloc_2d_char (largc + 1, 100);
    strcpy (largv[0], "neper");
    strcpy (largv[1], "-D");
    strcpy (largv[2], "all");
    strcpy (largv[3], "-runmode");
    strcpy (largv[4], "fast");
    ut_free_2d_char (largv, largc + 1);

    neper_foot ();
  }

  else
  {
    neper_head (NEPER_VERSION);
    neper_info ();

    // Rc file ---

    rcfile = ut_alloc_1d_char (1000);
    int arg_pos = 0;

    if (!strcmp (mod, "--rcfile"))
    {
      arg_pos += 2;
      strcpy (rcfile, argv[2]);
    }
    else
      sprintf (rcfile, "%s/.neperrc", getenv ("HOME"));

    status = ut_string_comp (argv[arg_pos + 1], modlist, modqty, &mod);

    fargc = 0;
    fargv = NULL;
    if (strcmp (rcfile, "none") != 0)
    {
      if (ut_file_exist (rcfile))
      {
        ut_print_message (0, 0, "Loading initialization file `%s'...\n",
                          rcfile);
        ut_option_read (rcfile, &fargc, &fargv, "neper", mod);
      }
      else
        ut_print_message (0, 0, "No initialization file found (`%s').\n",
                          rcfile);
    }
    else
      ut_print_message (0, 0, "Ignoring initialization file.\n");

    // Modules ---

    if (!strcmp (mod, "-T"))
      neper_t (fargc, fargv, argc - 1 - arg_pos, argv + 1 + arg_pos);
    else if (!strcmp (mod, "-M"))
      neper_m (fargc, fargv, argc - 1 - arg_pos, argv + 1 + arg_pos);
    else if (!strcmp (mod, "-V"))
      neper_v (fargc, fargv, argc - 1 - arg_pos, argv + 1 + arg_pos);

    // Closing ---

    gettimeofday (&end_time, NULL);
    t = ut_time_subtract (&beg_time, &end_time);

    ut_print_message (0, 1, "Elapsed time: %.3f secs.\n", t);

    neper_foot ();
  }

  ut_free_2d_char (fargv, fargc + 1);
  ut_free_2d_char (modlist, modqty + 1);
  ut_free_1d_char (mod);
  ut_free_1d_char (rcfile);

  return EXIT_SUCCESS;
}

void
neper_head (char *neper_version)
{
  int comma = 0, alldeps = 1;

  printf
    ("\n========================    N   e   p   e   r    =======================\n");

  ut_print_message (0, 0,
                    "A software package for polycrystal generation and meshing.\n");

  ut_print_message (0, 0, "Version %s\n", neper_version);

  ut_print_message (0, 0, "Built with:");
#ifdef HAVE_GSL
  printf ("%s gsl", comma ? "," : "");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_NLOPT
  printf ("%s nlopt", comma ? "," : ":");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_LIBSCOTCH
  printf ("%s libscotch", comma ? "," : ":");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_OPENMP
  printf ("%s openmp", comma ? "," : ":");
  comma = 1;
#else
  alldeps = 0;
#endif

  printf ("%s\n", alldeps ? " (full build)" : "");

#ifndef DEVEL_OPTIMIZATION
  ut_print_message (1, 0,
                    "Built with: no optimization.            NOT FOR PRODUCTION USE.\n");
#endif

#ifdef DEVEL_PROFILING
  ut_print_message (1, 0,
                    "Built with: profiling.                  NOT FOR PRODUCTION USE.\n");
#endif

#ifdef DEVEL_DEBUGGING_TEST
  ut_print_message (1, 0,
                    "Built with: debugging tests.            NOT FOR PRODUCTION USE.\n");
#endif

#ifdef DEVEL_DEBUGGING_FLAG
  ut_print_message (1, 0,
                    "Built with: debugging compilation flag. NOT FOR PRODUCTION USE.\n");
#endif

#ifdef HAVE_OPENMP
#pragma omp parallel
  if (omp_get_thread_num () == 0)
    ut_print_message (0, 0, "Running on %d threads.\n",
                      omp_get_num_threads ());
#endif

  return;
}

void
neper_foot ()
{
  printf
    ("========================================================================\n\n");

  return;
}

void
neper_info ()
{
  ut_print_message (0, 0, "<http://neper.info> <http://neper.info/community>\n");

  ut_print_message (0, 0,
                    "Copyright (C) 2003-2019, and GNU GPL'd, by Romain Quey.\n");

  return;
}
