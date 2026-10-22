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
#include<spawn.h>
#include<fcntl.h>
#include"ut.h"

extern char **environ;

struct timeval ctrlc_t;

// Thanks to T. Auger for his contribution
void
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
      printf
        ("\nSubprocess killed. Press Ctrl-C again in the next 2 seconds to quit.\n");
      gettimeofday (&ctrlc_t, NULL);
      kill (1, 9);
    }
    else
    {
      gettimeofday (&ctrlc_t, NULL);
      kill (0, 9);
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
  sigaction (SIGINT, &act, 0);

  return;
}

void
ut_sys_ctrlc_end (struct timeval *pctrlc_t)
{
  (*pctrlc_t) = ctrlc_t;

  return;
}

int
ut_sys_runwtime (char *exec, char *command, double tmax,
                 struct timeval *pctrlc_t)
{
  int status;
  pid_t pid, res;
  struct sigaction act;
  struct timeval beg_t, now_t;
  char **list = NULL;
  int qty;
  posix_spawn_file_actions_t actions;

  ctrlc_t = (*pctrlc_t);

  memset (&act, 0, sizeof (struct sigaction));
  act.sa_handler = sighandler;

  // only SIGINT is caught: the function can be called from several
  // (OpenMP) threads at once, so the process-wide SIGCHLD / SIGALRM
  // (setitimer) cannot be used to wait for a given child
  sigaction (SIGINT, &act, 0);

  ut_list_break (command, " ", &list, &qty);
  list = ut_realloc_1d_pchar_null (list, qty + 1, 1);
  // the standard output and error are discarded (the command is not run
  // through a shell, so it cannot include redirections)
  posix_spawn_file_actions_init (&actions);
  posix_spawn_file_actions_addopen (&actions, STDOUT_FILENO, "/dev/null",
                                    O_WRONLY, 0);
  posix_spawn_file_actions_addopen (&actions, STDERR_FILENO, "/dev/null",
                                    O_WRONLY, 0);
  // posix_spawnp is used instead of fork + exec, as fork copies the page
  // tables of the (possibly large) calling process, which is slow.
  status = posix_spawnp (&pid, exec, &actions, NULL, list, environ);
  posix_spawn_file_actions_destroy (&actions);
  if (status != 0)
    /* The spawn failed. Report failure. */
    status = -1;
  else
  {
    // polling the child, and killing it once tmax is reached (tmax <= 0
    // means no time limit)
    gettimeofday (&beg_t, NULL);
    while (1)
    {
      res = waitpid (pid, &status, WNOHANG);

      if (res == pid)
        break;

      if (res == -1 && errno != EINTR)
      {
        status = -1;
        break;
      }

      gettimeofday (&now_t, NULL);
      if (tmax > 0 && ut_time_subtract (&beg_t, &now_t) > tmax)
      {
        kill (pid, 9);
        waitpid (pid, NULL, 0);
        status = -1;
        break;
      }

      usleep (1000);
    }
  }

  (*pctrlc_t) = ctrlc_t;

  ut_free_2d_char (&list, qty + 1);

  return status;
}

void
ut_sys_mkdir (const char *dir, ...)
{
  int status;
  char tmp[1000];
  char *p = NULL;
  size_t len;

  va_list args;

  va_start (args, dir);
  vsnprintf (tmp, sizeof (tmp), dir, args);
  va_end (args);

  len = strlen (tmp);
  if (tmp[len - 1] == '/')
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
  {
    ut_print_message (2, 2, "%s cannot be created.\n", dir);
    abort ();
  }

  return;
}

int
ut_sys_isdir (const char *path, ...)
{
  struct stat sb;

  char tmp[1000];
  va_list args;

  va_start (args, path);
  vsnprintf (tmp, sizeof (tmp), path, args);
  va_end (args);

  return (stat (tmp, &sb) == 0 && S_ISDIR (sb.st_mode));
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

  r = readlink ("/proc/self/exe", *ppath, bufsize);

  return (r == -1) ? -1 : 0;
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
