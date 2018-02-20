/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_processimage_.h"

void
nev_processimage (char **argv, int *pi)
{
  int i, w1, w2, factor;
  double w;
  char *img1 = NULL;
  char *img2 = NULL;
  char *weight = NULL;
  char *format = ut_alloc_1d_char (10);
  int den;
  char *command = ut_alloc_1d_char (10000);

  if (strcmp (argv[++(*pi)], "average") == 0)
  {
    img1 = ut_alloc_1d_char (strlen (argv[++(*pi)]) + 1);
    sscanf (argv[*pi], "%s", img1);
    img2 = ut_alloc_1d_char (strlen (argv[++(*pi)]) + 1);
    sscanf (argv[*pi], "%s", img2);
    weight = ut_alloc_1d_char (strlen (argv[++(*pi)]) + 1);
    sscanf (argv[*pi], "%s", weight);
    ut_string_format (weight, format);

    int length = strlen (format);
    if (format[length - 1] == 'f')
      ut_string_real (weight, &w);
    else
    {
      double tw;
      int status = sscanf (weight, "%lf,%lf", &w, &tw);
      if (status != 2)
	abort ();
    }

    format[length - 1] = '\0';
    for (i = 0; i < length; i++)
      if (format[i] == '.')
      {
	format += (i + 1);
	break;
      }
    ut_string_int (format, &factor);
    factor = ut_num_d2ri (pow (10, factor + 1));

    if (ut_num_equal (w, 1, 1e-2))
    {
      w1 = 1;
      w2 = 0;
    }
    else if (ut_num_equal (w, 0, 1e-3))
    {
      w1 = 0;
      w2 = 1;
    }
    else
    {
      w1 = ut_num_d2ri (w * factor);
      w2 = ut_num_d2ri ((1 - w) * factor);
    }

    den = ut_num_lcomden (w1, w2);
    w1 /= den;
    w2 /= den;
  }
  else
  {
    printf ("unrecognized operation %s\n", argv[*pi]);
    abort ();
  }

  sprintf (command, "convert ");
  for (i = 0; i < w1; i++)
    sprintf (command, "%s %s ", command, img1);
  for (i = 0; i < w2; i++)
    sprintf (command, "%s %s ", command, img2);

  ut_file_openmessage (argv[++(*pi)], "w");
  sprintf (command, "%s -average %s", command, argv[*pi]);
  if (system (command) == -1)
    abort ();
  ut_file_closemessage (argv[*pi], "w");

  ut_free_1d_char (img1);
  ut_free_1d_char (img2);
  ut_free_1d_char (weight);

  return;
}
