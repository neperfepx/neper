/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#include<stdlib.h>
#include<limits.h>
#include<unistd.h>
#include<stdio.h>
#include<stdarg.h>
#include<sys/wait.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<errno.h>
#include<signal.h>
#include<string.h>
#include<float.h>
#include"ut.h"

static struct timeval ctrlc_t;
static struct sigaction ctrlc_oldact;

// Thanks to T. Auger for his contribution
// On a first Ctrl+C, the subprocesses (which receive it too) end; on a
// second Ctrl+C within 2 seconds, execution ends.
static void
sighandler (int signal)
{
  if (signal == SIGCHLD)
  {
    while (waitpid ((pid_t) (-1), 0, WNOHANG) > 0)
    {
    };
  }
  else if (signal == SIGALRM)
  {
  }
  else if (signal == SIGINT)
  {
    double t;
    struct timeval now_t;

    gettimeofday (&now_t, NULL);

    t = ut_time_subtract (&ctrlc_t, &now_t);

    if (t > 2)
    {
      // write, as printf is not async-signal-safe
      const char msg[] =
        "\nSubprocess killed. Press Ctrl-C again in the next 2 seconds to quit.\n";
      if (write (STDOUT_FILENO, msg, sizeof (msg) - 1) < 0)
      {
      }
      gettimeofday (&ctrlc_t, NULL);
    }
    else
    {
      gettimeofday (&ctrlc_t, NULL);
      kill (0, SIGKILL);
    }
  }

  return;
}

void
ut_sys_ctrlc_begin (struct timeval *pctrlc_t)
{
  struct sigaction act;

  ctrlc_t = (*pctrlc_t);

  memset (&act, 0, sizeof (struct sigaction));
  act.sa_handler = sighandler;
  sigaction (SIGINT, &act, &ctrlc_oldact);

  return;
}

void
ut_sys_ctrlc_end (struct timeval *pctrlc_t)
{
  (*pctrlc_t) = ctrlc_t;

  sigaction (SIGINT, &ctrlc_oldact, 0);

  return;
}

void
ut_sys_mkdir (const char *dir, ...)
{
  int status;
  char *tmp = NULL;
  char *p = NULL;
  size_t len;

  va_list args, args2;

  va_start (args, dir);
  va_copy (args2, args);
  tmp = ut_alloc_1d_char (vsnprintf (NULL, 0, dir, args) + 1);
  va_end (args);
  vsprintf (tmp, dir, args2);
  va_end (args2);

  len = strlen (tmp);
  if (len == 0)
  {
    ut_free_1d_char (&tmp);
    return;
  }
  if (len > 1 && tmp[len - 1] == '/')
    tmp[len - 1] = 0;
  for (p = tmp + 1; *p; p++)
    if (*p == '/')
    {
      *p = 0;
      mkdir (tmp, S_IRWXU | S_IRGRP | S_IXGRP);
      *p = '/';
    }

  status = mkdir (tmp, S_IRWXU | S_IRGRP | S_IXGRP);

  if (status && errno != EEXIST)
    ut_print_message (2, 2, "%s cannot be created.\n", tmp);

  ut_free_1d_char (&tmp);

  return;
}

int
ut_sys_isdir (const char *path, ...)
{
  int status;
  struct stat sb;
  char *tmp = NULL;
  va_list args, args2;

  va_start (args, path);
  va_copy (args2, args);
  tmp = ut_alloc_1d_char (vsnprintf (NULL, 0, path, args) + 1);
  va_end (args);
  vsprintf (tmp, path, args2);
  va_end (args2);

  status = (stat (tmp, &sb) == 0 && S_ISDIR (sb.st_mode));

  ut_free_1d_char (&tmp);

  return status;
}

char *
ut_sys_realpath (const char *path)
{
  char *actualpath = NULL;      // ut_alloc_1d_char (PATH_MAX + 1);
  char *ptr = NULL;

  ptr = realpath (path, actualpath);

  if (!ptr)
    ut_free_1d_char (&actualpath);

  return ptr;
}

int
ut_sys_currentbin_path (char **ppath)
{
  ssize_t r;
  // ref http://linux.die.net/man/2/readlink for how not to use a fixed bufsize
  int bufsize = 1024;

  (*ppath) = ut_alloc_1d_char (bufsize);

  // readlink does not null-terminate, and truncates silently
  r = readlink ("/proc/self/exe", *ppath, bufsize - 1);

  if (r == -1 || r == bufsize - 1)
    return -1;

  (*ppath)[r] = '\0';

  return 0;
}

// 0 is little, 1 is big
int
ut_sys_endian (void)
{
  unsigned int x = 0x76543210;
  char *c = (char *) &x;

  if (*c == 0x10)
    return 0;
  else
    return 1;
}

void
ut_sys_shellquote (const char *string, char **pquoted)
{
  int i, pos = 0, tilde = !strncmp (string, "~/", 2);
  char *quoted = ut_alloc_1d_char (4 * strlen (string) + 5);

  if (tilde)
  {
    strcpy (quoted, "~/");
    pos = 2;
  }

  quoted[pos++] = '\'';

  for (i = tilde ? 2 : 0; string[i]; i++)
    if (string[i] == '\'')
    {
      strcpy (quoted + pos, "'\\''");
      pos += 4;
    }
    else
      quoted[pos++] = string[i];

  quoted[pos++] = '\'';
  quoted[pos] = '\0';

  ut_free_1d_char (pquoted);
  (*pquoted) = quoted;

  return;
}
