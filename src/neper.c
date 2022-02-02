/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neper_.h"
#include"neper_config.h"

void neper_head (char *);
void neper_foot (void);
void neper_info (void);
int neper_diff (int argc, char **argv);

int
main (int argc, char **argv)
{
  int modqty, status, fargc = 0;
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
  strcpy (modlist[++modqty], "--diff");
  strcpy (modlist[++modqty], "-T");
  strcpy (modlist[++modqty], "-M");
  strcpy (modlist[++modqty], "-V");
  strcpy (modlist[++modqty], "-S");

  // Testing input -----------------------------------------------------

  if (argc > 1)
  {
    status = ut_string_completion (argv[1], modlist, modqty, &mod);

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

  else if (argc >= 2 && !strcmp (mod, "--diff"))
    return neper_diff (argc, argv);

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

    status = ut_string_completion (argv[arg_pos + 1], modlist, modqty, &mod);

    fargc = 0;
    if (strcmp (rcfile, "none") != 0)
    {
      if (ut_file_exist (rcfile))
      {
        ut_print_message (0, 0, "Loading initialization file `%s'...\n",
                          rcfile);
        ut_arg_readfromfile (rcfile, "neper", mod, &fargc, &fargv);
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
    else if (!strcmp (mod, "-S"))
      neper_s (fargc, fargv, argc - 1 - arg_pos, argv + 1 + arg_pos);

    // Closing ---

    gettimeofday (&end_time, NULL);
    t = ut_time_subtract (&beg_time, &end_time);

    ut_print_message (0, 1, "Elapsed time: %.3f secs.\n", t);

    neper_foot ();
  }

  ut_free_2d_char (&modlist, 10);
  ut_free_1d_char (&mod);
  ut_free_2d_char (&fargv, fargc + 1);
  ut_free_1d_char (&rcfile);

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

  ut_print_message (0, 0, "Built with: ");

#ifdef HAVE_GSL
  printf ("%sgsl", comma ? "|" : "");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_MUPARSER
  printf ("%smuparser", comma ? "|" : "");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_OPENGJK
  printf ("%sopengjk", comma ? "|" : ":");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_OPENMP
  printf ("%sopenmp", comma ? "|" : ":");
  comma = 1;
#else
  alldeps = 0;
#endif

#ifdef HAVE_NLOPT
  printf ("%snlopt", comma ? "|" : ":");
  comma = 1;
#else
  alldeps = 0;
#endif

  printf ("%slibscotch", comma ? "|" : ":");
  comma = 1;

  printf ("%s\n", alldeps ? " (full)" : "");

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
  ut_print_message (0, 0, "<https://neper.info>\n");

  ut_print_message (0, 0,
                    "Copyright (C) 2003-2022, and GNU GPL'd, by Romain Quey.\n");

  return;
}

int
neper_diff (int argc, char **argv)
{
  int status, diff = 0;
  double val1, val2, eps = 1e-6;
  char *string1 = NULL, *string2 = NULL;
  FILE *file1 = NULL, *file2 = NULL;

  if (argc < 4)
  {
    printf ("Usage: neper --diff file1 file2 [eps]\n");
    return -1;
  }

  string1 = ut_alloc_1d_char (10000);
  string2 = ut_alloc_1d_char (10000);
  file1 = ut_file_open (argv[2], "R");
  file2 = ut_file_open (argv[3], "R");

  if (argc == 5)
    eps = atof (argv[4]);

  while (fscanf (file1, "%s", string1) == 1)
  {
    status = fscanf (file2, "%s", string2);

    if (status != 1)
    {
      diff = 1;
      break;
    }

    else if (sscanf (string1, "%lf", &val1))
    {
      status = sscanf (string2, "%lf", &val2);
      if (status != 1 || (!ut_num_equal (val1, val2, eps) && !ut_num_requal (val1, val2, eps)))
      {
        diff = 1;
        break;
      }
    }

    else if (strcmp (string1, string2))
    {
      diff = 1;
      break;
    }
  }

  ut_file_close (file1, argv[2], "R");
  ut_file_close (file2, argv[3], "R");
  ut_free_1d_char (&string1);
  ut_free_1d_char (&string2);

  return diff;
}
