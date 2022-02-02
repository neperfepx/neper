/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nev_utils_.h"

void
nev_fixarg (char **parg)
{
  if (!strcmp (*parg, "-showshadow"))
  {
    ut_print_message (1, 2, "`-showshadow' has been renamed `-sceneshadow' and will be removed in a future version.\n");
    ut_string_string ("-sceneshadow", parg);
  }

  else if (!strcmp (*parg, "-imagebackground"))
  {
    ut_print_message (1, 2, "`-imagebackground' has been renamed `-scenebackground' and will be removed in a future version.\n");
    ut_string_string ("-scenebackground", parg);
  }

  return;
}
