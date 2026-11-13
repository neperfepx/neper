/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_asy_.h"

int
neut_asy_check (char *asy)
{
  int status;
  char *binary = NULL, *command = NULL;

  ut_sys_shellquote (asy, &binary);
  ut_string_sprintf (&command, "command -v %s > /dev/null 2>&1", binary);

  status = system (command);

  ut_free_1d_char (&binary);
  ut_free_1d_char (&command);

  return status;
}

int
neut_asy_check_error (char *asy)
{
  if (neut_asy_check (asy) != 0)
    ut_print_message (2, 0, "Asymptote (command `%s') not found.  Install it or specify it using option `-asymptote'.\n", asy);

  return 0;
}

void
neut_asy_convert (char *asymptote, char *filename, int imagewidth, int imageheight,
                  char *formatstring, int messagetag)
{
  int i, formatqty;
  char *filename2 = NULL;
  char *command = NULL, *binary = NULL, *input = NULL, *output = NULL;
  char **formats = NULL;
  char *pixels = ut_alloc_1d_char (1000);

  ut_list_break (formatstring, NEUT_SEP_NODEP, &formats, &formatqty);
  sprintf (pixels, " (%dx%d pixels)", imagewidth, imageheight);

  ut_sys_shellquote (asymptote, &binary);
  ut_sys_shellquote (filename, &input);

  for (i = 0; i < formatqty; i++)
    if (strcmp (formats[i], "asy"))
    {
      neut_asy_check_error (asymptote);

      // replacing the asy extension
      ut_string_sprintf (&filename2, "%.*s%s", (int) strlen (filename) - 3, filename, formats[i]);
      remove (filename2);

      if (messagetag >= 0)
        ut_print_message (0, messagetag,
                          "Generating %s file%s...\n", formats[i],
                          !strcmp (formats[i], "png") ? pixels: "");

      ut_file_openmessage (filename2, "w");

      // output name without extension: asy appends it (version 3.05 appends it
      // even if already present, e.g. img.png.png)
      ut_string_sprintf (&output, "%.*s", (int) strlen (filename) - 4, filename);
      ut_sys_shellquote (output, &output);
      ut_string_sprintf (&command, "%s -f %s -antialias=4 -render %f %s -o %s", binary, formats[i],
                         imageheight * .00492046785641154627, input, output);

      if (system (command) == -1 || !ut_file_exist (filename2))
        ut_print_message (2, 3, "File `%s' could not be generated!  To debug: use option `-imageformat asy' and run command `%s'.\n",
                          filename2, command);

      ut_file_closemessage (filename2, "w");
    }

  ut_free_1d_char (&command);
  ut_free_1d_char (&binary);
  ut_free_1d_char (&input);
  ut_free_1d_char (&output);
  ut_free_1d_char (&filename2);
  ut_free_2d_char (&formats, formatqty);
  ut_free_1d_char (&pixels);

  return;
}
