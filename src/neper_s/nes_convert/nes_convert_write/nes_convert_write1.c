/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nes_convert_write_.h"

void
nes_convert_write (struct IN_S In, struct SIM *pSim)
{
  /*
  if (ut_file_exist (In.simdir))
    ut_print_message (2, 2, "Directory `%s' already exists\n", In.simdir);
  */

  ut_sys_mkdir (In.simdir);

  ut_print_message (0, 2, "Writing inputs...\n");
  nes_convert_write_inputs (In, pSim);

  ut_print_message (0, 2, "Writing results...\n");
  nes_convert_write_results (In, pSim);

  if ((*pSim).RestartFiles)
  {
    ut_print_message (0, 2, "Writing restart...\n");
    nes_convert_write_restart ((*pSim).fepxdir, (*pSim).simdir, *pSim);
  }

  return;
}
