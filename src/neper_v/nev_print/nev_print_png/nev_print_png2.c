/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_print_png_.h"

void
nev_print_png_convert (char *povray, char *filename, int imagewidth,
                   int imageheight, int antialiasing, int messagetag)
{
  char *pngfilename = NULL;
  char *command = ut_alloc_1d_char (1000);

  neut_povray_check (povray);

  ut_string_string (filename, &pngfilename);
  ut_string_fnrs (pngfilename, ".pov", ".png", 1);

  if (messagetag >= 0)
    ut_print_message (0, messagetag,
                      "Generating png file (%dx%d pixels)...\n", imagewidth,
                      imageheight);

  remove (pngfilename);

  ut_file_openmessage (pngfilename, "w");

  sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D %s 2>/dev/null",
           povray, filename, pngfilename, imagewidth, imageheight,
           antialiasing > 0 ? "+A0.2" : "");

  if (system (command) == -1 || !ut_file_exist (pngfilename))
  {
    ut_print_message (2, 3, "File `%s' could not be generated!\n",
                      pngfilename);

    sprintf (command, "%s Input_File_Name=%s +O%s +W%d +H%d -D %s", povray,
             filename, pngfilename, imagewidth, imageheight,
             antialiasing > 0 ? "+A0.2" : "");

    // otherwise, printing debugging info.
    ut_print_message (2, 3,
                      "To debug: use option `-imageformat pov' and run command `%s'.\n",
                      command);
  }

  ut_file_closemessage (pngfilename, "w");

  ut_free_1d_char (&pngfilename);
  ut_free_1d_char (&command);

  return;
}
