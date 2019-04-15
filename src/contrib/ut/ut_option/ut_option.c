/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ut.h"

/* Reading options from optionsfile. */
void
ut_option_read (char *optionsfile, int *pfargc, char ***pfargv,
		char *pname, char *mname)
{
  char Trash[1000];
  int test;
  FILE* file = NULL;

  if (ut_file_exist (optionsfile) == 0)
  {
    (*pfargc) = 0;
    return;
  }

  file = ut_file_open (optionsfile, "R");

  /* Skipping data until `neper -S' is reached. */
  while (fscanf (file, "%s", Trash) == 1)
  {
    if (strcmp (Trash, pname) == 0)
    {
      if (fscanf (file, "%s", Trash) == 1)
	if (strcmp (Trash, mname) == 0)
	  break;
    }
  }

  /* Reading options */
  (*pfargc) = 1;
  (*pfargv) = ut_alloc_1d_pchar ((*pfargc) + 1);
  (*pfargv)[0] = NULL;
  do
  {
    (*pfargv) = ut_realloc_1d_pchar ((*pfargv), (*pfargc) + 1);
    (*pfargv)[(*pfargc)] = ut_alloc_1d_char (1000);
    test = fscanf (file, "%s", (*pfargv)[(*pfargc)]);
    (*pfargv)[(*pfargc)]
      = ut_realloc_1d_char ((*pfargv)[(*pfargc)], strlen ((*pfargv)[(*pfargc)]) + 1);
    (*pfargc)++;
  }
  while (test != -1 && strcmp ((*pfargv)[(*pfargc) - 1], pname) != 0);

  (*pfargc)--;			/* the last one is nok. */

  ut_file_close (file, optionsfile, "R");

  return;
}
