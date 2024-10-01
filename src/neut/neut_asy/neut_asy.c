/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_asy_.h"

int
neut_asy_check (char *asy)
{
  int status;
  char *tmp = ut_alloc_1d_char (1000);
  char *tmp2 = ut_alloc_1d_char (1000);

  sprintf (tmp, "tmp%d", getpid ());
  sprintf (tmp2, "which %s 2> /dev/null > %s", asy, tmp);
  status = system (tmp2);

  remove (tmp);

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);

  return status;
}

int
neut_asy_check_error (char *asy)
{
  if (neut_asy_check (asy) != 0)
  {
    ut_print_messagewnc (2, 72,
                         "Povray not found! Make sure it is available at the command line then run Neper again.\n");
    abort ();
  }

  return 0;
}

void
neut_asy_convert (char *asymptote, char *filename, int imagewidth, int imageheight,
                  char *formatstring, int messagetag)
{
  int i, formatqty;
  char *filename2 = NULL;
  char *command = ut_alloc_1d_char (1000);
  char **formats = NULL;
  char *pixels = ut_alloc_1d_char (1000);

  ut_list_break (formatstring, NEUT_SEP_NODEP, &formats, &formatqty);
  sprintf (pixels, " (%dx%d pixels)", imagewidth, imageheight);

  neut_asy_check (asymptote);

  for (i = 0; i < formatqty; i++)
    if (strcmp (formats[i], "asy"))
    {
      ut_string_string (filename, &filename2);
      ut_string_fnrs (filename2, "asy", formats[i], 1);
      remove (filename2);

      if (messagetag >= 0)
        ut_print_message (0, messagetag,
                          "Generating %s file%s...\n", formats[i],
                          !strcmp (formats[i], "png") ? pixels: "");

      ut_file_openmessage (filename2, "w");
      sprintf (command, "%s -f %s -antialias=4 -render %f %s -o %s", asymptote, formats[i],
               imageheight * .00492046785641154627, filename, filename2);

      if (system (command) == -1)
        abort ();
      ut_file_closemessage (filename2, "w");
    }

  ut_free_1d_char (&command);
  ut_free_1d_char (&filename2);
  ut_free_2d_char (&formats, formatqty);
  ut_free_1d_char (&pixels);

  return;
}
