/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<limits.h>
#include<dirent.h>
#include"ut.h"

void
ut_dir_allfiles (char *dir, char ***pfiles, int *pfileqty)
{
  struct dirent *entry;
  DIR *dp;

  dp = opendir(dir);

  if (dp == NULL) {
      perror("opendir");
      abort ();
  }

  (*pfileqty) = 0;
  // Read entries in the directory
  while ((entry = readdir(dp)) != NULL)
    if (strcmp (entry->d_name, ".") && strcmp (entry->d_name, ".."))
    {
      (*pfileqty)++;
      (*pfiles) = ut_realloc_1d_pchar_null (*pfiles, *pfileqty, 1);
      ut_string_string (entry->d_name, (*pfiles) + (*pfileqty) - 1);
    }

  // Close the directory
  closedir(dp);

  return;
}
