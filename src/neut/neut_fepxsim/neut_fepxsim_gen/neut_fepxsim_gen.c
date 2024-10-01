/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the CGENYING file in the top-level directory. */

#include"neut_fepxsim_gen_.h"

void
neut_fepxsim_res_file (struct FEPXSIM FSim, char *res, int core, char *filename)
{
  if (!FSim.RestartId)
    sprintf (filename, "%s/post.%s.core%d", FSim.fepxdir, res, core);
  else
    sprintf (filename, "%s/post.%s.rst%d.core%d", FSim.fepxdir, res, FSim.RestartId, core);

  return;
}
