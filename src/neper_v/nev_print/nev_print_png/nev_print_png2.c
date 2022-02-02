/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_.h"

void
nev_print_png_pov2png (char *povray, char *filename, int imagewidth,
                   int imageheight, int imageantialias, int messagetag)
{
  char *outfilename = NULL;
  char *command = ut_alloc_1d_char (1000);

  neut_povray_check (povray);

  ut_string_string (filename, &outfilename);
  outfilename[strlen (outfilename) - 2] = 'n';
  outfilename[strlen (outfilename) - 1] = 'g';

  if (messagetag >= 0)
    ut_print_message (0, messagetag,
                      "Generating png file (%dx%d pixels)...\n", imagewidth,
                      imageheight);

  remove (outfilename);

  ut_file_openmessage (outfilename, "w");

  sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D %s 2>/dev/null",
           povray, filename, outfilename, imagewidth, imageheight,
           imageantialias > 0 ? "+A0.2" : "");

  if (system (command) == -1 || !ut_file_exist (outfilename))
  {
    ut_print_message (2, 3, "File `%s' could not be generated!\n",
                      outfilename);

    sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D %s", povray,
             filename, outfilename, imagewidth, imageheight,
             imageantialias > 0 ? "+A0.2" : "");

    // otherwise, printing debugging info.
    ut_print_message (2, 3,
                      "To debug: use option `-imageformat pov' and run command `%s'.\n",
                      command);
  }

  ut_file_closemessage (outfilename, "w");

  ut_free_1d_char (&outfilename);
  ut_free_1d_char (&command);

  return;
}
