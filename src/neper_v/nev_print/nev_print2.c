/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_.h"

void
nev_print_pov2png (char *povray, char *filename, int imagewidth,
                   int imageheight, int imageantialias, int messagetag)
{
  char *outfilename = ut_alloc_1d_char (strlen (filename) + 1);
  char *command = ut_alloc_1d_char (1000);

  strcpy (outfilename, filename);
  outfilename[strlen (outfilename) - 2] = 'n';
  outfilename[strlen (outfilename) - 1] = 'g';

  if (messagetag >= 0)
    ut_print_message (0, messagetag,
                      "Generating png file (%dx%d pixels)...\n", imagewidth,
                      imageheight);

  remove (outfilename);

  ut_file_openmessage (outfilename, "w");

  if (imageantialias > 0)
    sprintf (command,
             "%s Input_File_Name=%s +O%s +W%d +H%d -D +A0.2 2>/dev/null",
             povray, filename, outfilename, imagewidth, imageheight);
  else
    sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D 2>/dev/null",
             povray, filename, outfilename, imagewidth, imageheight);

  if (system (command) == -1 || !ut_file_exist (outfilename))
  {
    ut_print_message (2, 3, "File `%s' could not be generated!\n",
                      outfilename);

    // checking (again) if povray is present
    neut_povray_check_error ();

    if (imageantialias > 0)
      sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D +A0.2",
               povray, filename, outfilename, imagewidth, imageheight);
    else
      sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D", povray,
               filename, outfilename, imagewidth, imageheight);

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
