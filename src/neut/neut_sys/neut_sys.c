/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include "neut_sys_.h"

#define DEFAULT_USER_DATA_DIR "/.local/share/neper"

// Data directory, looked for in (successively):
// - $NEPER_DATA_DIR;
// - <bindir>/../share/neper, where <bindir> is the directory of the neper
//   binary, so that an installed binary uses its own data files (make
//   install, make userinstall, conda, etc.), even if moved;
// - $HOME/.local/share/neper (default of make userinstall, and used by the
//   binary of the build directory);
// - <prefix>/share/neper, where <prefix> is the installation prefix.
// Returns 0 if found, -1 otherwise.
int
neut_sys_data (char **pdata, int verbosity)
{
  char *dir = NULL, *bindir = NULL;
  char *home = getenv ("HOME");

  if (pdata)
    ut_free_1d_char (pdata);

  // environment variable
  if (getenv ("NEPER_DATA_DIR"))
  {
    if (ut_sys_isdir (getenv ("NEPER_DATA_DIR")))
      ut_string_string (getenv ("NEPER_DATA_DIR"), &dir);
    else if (verbosity)
      ut_print_message (1, 1, "Ignoring NEPER_DATA_DIR (defined but invalid)...\n");
  }

  // next to the binary
  if (!dir && !neut_sys_exedir (&bindir))
  {
    char *tmp = ut_string_paste (bindir, (char *) "/../share/neper/");
    if (ut_sys_isdir (tmp))
      dir = realpath (tmp, NULL);
    if (dir)
    {
      // realpath () allocates with malloc and strips the trailing slash
      char *tmp2 = ut_string_paste (dir, (char *) "/");
      free (dir);
      dir = tmp2;
    }
    ut_free_1d_char (&tmp);
  }

  // user location
  if (!dir && home && home[0] == '/')
  {
    char *tmp = ut_string_paste (home, (char *) DEFAULT_USER_DATA_DIR);
    if (ut_sys_isdir (tmp))
      dir = tmp;
    else
      ut_free_1d_char (&tmp);
  }

  // system location (installation prefix, set at configure time)
  if (!dir && ut_sys_isdir (NEPER_SYSTEM_DATA_DIR))
    ut_string_string (NEPER_SYSTEM_DATA_DIR, &dir);

  ut_free_1d_char (&bindir);

  if (!dir)
  {
    if (verbosity)
      ut_print_message (1, 1, "Data files not found (see NEPER_DATA_DIR).\n");
    return -1;
  }

  if (pdata)
    (*pdata) = dir;
  else
    ut_free_1d_char (&dir);

  return 0;
}

// Directory of the neper binary (absolute, symbolic links resolved).
// Returns 0 on success, -1 otherwise.
int
neut_sys_exedir (char **pdir)
{
  char *path = NULL, *p = NULL;

  ut_free_1d_char (pdir);

#if defined(__linux__)
  path = realpath ("/proc/self/exe", NULL);
#elif defined(__APPLE__)
  uint32_t size = 0;
  _NSGetExecutablePath (NULL, &size);
  char *tmp = (char *) malloc (size + 1);
  if (tmp && !_NSGetExecutablePath (tmp, &size))
    path = realpath (tmp, NULL);
  free (tmp);
#endif

  if (!path)
    return -1;

  p = strrchr (path, '/');
  if (!p)
  {
    free (path);
    return -1;
  }
  (*p) = '\0';

  ut_string_string (path, pdir);
  free (path);

  return 0;
}

// Cache directory: $XDG_CACHE_HOME/neper (or $HOME/.cache/neper), with
// subdirectory subdir (if not NULL), created if needed.  Returns 0 if the
// directory is available (and writable), -1 otherwise (silently).
int
neut_sys_cache (char *subdir, char **pcache)
{
  char *p = NULL;
  char *xdg = getenv ("XDG_CACHE_HOME");
  char *home = getenv ("HOME");

  ut_free_1d_char (pcache);

  if (xdg && xdg[0] == '/')
    (*pcache) = ut_string_paste (xdg, (char *) "/neper");
  else if (home && home[0] == '/')
    (*pcache) = ut_string_paste (home, (char *) "/.cache/neper");
  else
    return -1;

  if (subdir && strlen (subdir) > 0)
  {
    p = ut_string_paste3 (*pcache, (char *) "/", subdir);
    ut_free_1d_char (pcache);
    (*pcache) = p;
  }

  // mkdir -p
  for (p = (*pcache) + 1; *p; p++)
    if (*p == '/')
    {
      *p = '\0';
      if (mkdir (*pcache, S_IRWXU) && errno != EEXIST)
      {
        *p = '/';
        ut_free_1d_char (pcache);
        return -1;
      }
      *p = '/';
    }

  if ((mkdir (*pcache, S_IRWXU) && errno != EEXIST) || access (*pcache, W_OK))
  {
    ut_free_1d_char (pcache);
    return -1;
  }

  return 0;
}
