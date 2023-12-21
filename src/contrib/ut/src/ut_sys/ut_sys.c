/* Copyright (C) 2003-2022, Romain Quey */
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

int
ut_sys_runwtime (char *exec, char *command, double tmax,
                 struct timeval *pctrlc_t)
{
  int status;
  pid_t pid;
  struct sigaction act;
  struct itimerval value = { {0, 0}, {0, 0} };
  char **list = NULL;
  int qty;

  if (!strcmp (EXEC_METHOD, "system"))
  {
    char *tmp = ut_alloc_1d_char (strlen (exec) + strlen (command) + 2);
    sprintf (tmp, "%s %s", exec, command);

    return system (tmp);
  }

  else if (!strcmp (EXEC_METHOD, "exec"))
  {

    ctrlc_t = (*pctrlc_t);

    memset (&act, 0, sizeof (struct sigaction));
    act.sa_handler = sighandler;
    // act.sa_flags = SA_RESTART;

    value.it_value.tv_sec = (long int) (tmax);
    value.it_value.tv_usec =
      (long int) ((double) (tmax - value.it_value.tv_sec) * 1000000);

    setitimer (ITIMER_REAL, &value, 0);

    sigaction (SIGCHLD, &act, 0);
    sigaction (SIGINT, &act, 0);
    sigaction (SIGALRM, &act, 0);

    ut_list_break (command, " ", &list, &qty);
    list = ut_realloc_1d_pchar_null (list, qty + 1, 1);
    pid = fork ();
    if (pid == 0)
    {
      execvp (exec, list);
      _exit (EXIT_FAILURE);
    }
    else if (pid < 0)
      /* The fork failed. Report failure. */
      status = -1;
    else
    {
      /* printf ("waiting for pid = %d\n", pid); */
      if (waitpid (pid, &status, 0) != pid)
        status = -1;

      /* printf ("waiting for pid = %d\n", pid); */

      /* it is likely that this should not be necessary */
      if (status == -1)
      {
        /* printf ("killing %d\n", pid); */
        kill (pid, 9);
      }
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
