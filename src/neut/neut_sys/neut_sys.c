/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_sys_.h"

#define DEFAULT_SYSTEM_DATA_DIR "/usr/local/share/neper/"
#define DEFAULT_USER_DATA_DIR "/.local/share/neper"

int
neut_sys_data (char **pdata, int verbosity)
{
  if (pdata)
    ut_free_1d_char (pdata);

  // environment variable
  if (getenv ("NEPER_DATA_DIR"))
  {
    if (ut_sys_isdir (getenv ("NEPER_DATA_DIR")))
    {
      if (pdata)
        ut_string_string (getenv ("NEPER_DATA_DIR"), pdata);
      return 0;
    }
    else if (verbosity)
      ut_print_message (1, 1, "Ignoring NEPER_DATA_DIR (defined but invalid)...\n");
  }

  // default user location
  if (ut_sys_isdir ("%s%s", getenv ("HOME"), DEFAULT_USER_DATA_DIR))
  {
    if (pdata)
    {
      (*pdata) = ut_alloc_1d_char (strlen (getenv ("HOME")) + strlen (DEFAULT_USER_DATA_DIR) + 1);
      (*pdata) = ut_string_paste (getenv ("HOME"), DEFAULT_USER_DATA_DIR);
    }
    return 0;
  }

  // default system location
  if (ut_sys_isdir (DEFAULT_SYSTEM_DATA_DIR))
  {
    if (pdata)
    {
      (*pdata) = ut_alloc_1d_char (strlen (DEFAULT_SYSTEM_DATA_DIR) + 1);
      ut_string_string (DEFAULT_SYSTEM_DATA_DIR, pdata);
    }
    return 0;
  }

  return -1;
}
