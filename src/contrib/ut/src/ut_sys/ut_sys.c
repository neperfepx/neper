/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#include<stdlib.h>
#include<limits.h>
#include<unistd.h>
#include<stdio.h>
#include<stdarg.h>
#ifndef _WIN32
#include<sys/wait.h>
#endif
#include<sys/time.h>
#include<sys/stat.h>
#include<errno.h>
#include<signal.h>
#include<string.h>
#include<float.h>
#include"ut.h"
#ifdef _WIN32
// (after ut.h: the Windows headers define macros, e.g. rad1, near, far)
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#endif

static struct timeval ctrlc_t;
#ifndef _WIN32
static struct sigaction ctrlc_oldact;
#else
static void (*ctrlc_oldhandler) (int);
#endif

// Thanks to T. Auger for his contribution
// On a first Ctrl+C, the subprocesses (which receive it too) end; on a
// second Ctrl+C within 2 seconds, execution ends.
static void
sighandler (int signum)
{
#ifndef _WIN32
  if (signum == SIGCHLD)
  {
    while (waitpid ((pid_t) (-1), 0, WNOHANG) > 0)
    {
    };
  }
  else if (signum == SIGALRM)
  {
  }
  else
#endif
  if (signum == SIGINT)
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
#ifndef _WIN32
      kill (0, SIGKILL);
#else
      // (the subprocesses of the console receive Ctrl+C themselves)
      _exit (1);
#endif
    }
#ifdef _WIN32
    // (handler reset by the system on Windows)
    signal (SIGINT, sighandler);
#endif
  }

  return;
}

void
ut_sys_ctrlc_begin (struct timeval *pctrlc_t)
{
  ctrlc_t = (*pctrlc_t);

#ifndef _WIN32
  struct sigaction act;

  memset (&act, 0, sizeof (struct sigaction));
  act.sa_handler = sighandler;
  sigaction (SIGINT, &act, &ctrlc_oldact);
#else
  ctrlc_oldhandler = signal (SIGINT, sighandler);
#endif

  return;
}

void
ut_sys_ctrlc_end (struct timeval *pctrlc_t)
{
  (*pctrlc_t) = ctrlc_t;

#ifndef _WIN32
  sigaction (SIGINT, &ctrlc_oldact, 0);
#else
  signal (SIGINT, ctrlc_oldhandler);
#endif

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

#ifndef _WIN32
  ptr = realpath (path, actualpath);
#else
  // (as realpath, NULL if the path does not exist)
  struct stat sb;
  ptr = (stat (path, &sb) == 0) ? _fullpath (actualpath, path, 0) : NULL;
#endif

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

#ifndef _WIN32
  // readlink does not null-terminate, and truncates silently
  r = readlink ("/proc/self/exe", *ppath, bufsize - 1);
#else
  r = GetModuleFileNameA (NULL, *ppath, bufsize - 1);
  if (r == 0)
    r = -1;
#endif

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

#ifndef _WIN32
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
#else
  // (cmd.exe: double quotes; a file name cannot contain double quotes)
  quoted[pos++] = '"';
  for (i = tilde ? 2 : 0; string[i]; i++)
    if (string[i] != '"')
      quoted[pos++] = string[i];
  quoted[pos++] = '"';
#endif
  quoted[pos] = '\0';

  ut_free_1d_char (pquoted);
  (*pquoted) = quoted;

  return;
}


int
ut_sys_system (const char *command)
{
#ifndef _WIN32
  return system (command);
#else
  // (cmd /c strips the outer quotes of a command that begins with a quote,
  // e.g. a quoted program: the command is quoted once more)
  char *tmp = NULL;
  int status;

  ut_string_sprintf (&tmp, "\"%s\"", command);
  status = system (tmp);
  ut_free_1d_char (&tmp);

  return status;
#endif
}

int
ut_sys_which (const char *program)
{
  int status;
  char *quoted = NULL, *command = NULL;

  ut_sys_shellquote (program, &quoted);
#ifndef _WIN32
  ut_string_sprintf (&command, "command -v %s > /dev/null 2>&1", quoted);
#else
  ut_string_sprintf (&command, "where %s > NUL 2>&1", quoted);
#endif
  status = ut_sys_system (command);

  ut_free_1d_char (&quoted);
  ut_free_1d_char (&command);

  return status;
}

#ifdef _WIN32

// Processes and pipes (Windows), for workers (see nem_mesh_gmsh3.c): a
// worker reads its input and writes its output on pipes, whose handles and
// the parent's process id are its last three arguments

#include<io.h>
#include<fcntl.h>
#include<stdint.h>

int
ut_sys_win_spawn (const char *exe, const char *arg, int *pin, int *pout,
                  intptr_t *pprocess)
{
  SECURITY_ATTRIBUTES sa = { sizeof (SECURITY_ATTRIBUTES), NULL, TRUE };
  HANDLE inr = NULL, inw = NULL, outr = NULL, outw = NULL, nul = NULL;
  HANDLE handles[3];
  STARTUPINFOEXA si;
  PROCESS_INFORMATION pi;
  SIZE_T size = 0;
  char *cmdline = NULL;
  int status = -1;

  memset (&si, 0, sizeof (si));
  memset (&pi, 0, sizeof (pi));

  if (!CreatePipe (&inr, &inw, &sa, 0) || !CreatePipe (&outr, &outw, &sa, 0))
    goto end;

  // (the parent's ends are not inherited)
  SetHandleInformation (inw, HANDLE_FLAG_INHERIT, 0);
  SetHandleInformation (outr, HANDLE_FLAG_INHERIT, 0);

  nul = CreateFileA ("NUL", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                     &sa, OPEN_EXISTING, 0, NULL);
  if (nul == INVALID_HANDLE_VALUE)
    goto end;

  // only the worker's ends and NUL are inherited (not the pipes of the other
  // workers, which would then not get the end of their input)
  handles[0] = inr;
  handles[1] = outw;
  handles[2] = nul;
  InitializeProcThreadAttributeList (NULL, 1, 0, &size);
  si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST) malloc (size);
  if (!si.lpAttributeList
      || !InitializeProcThreadAttributeList (si.lpAttributeList, 1, 0, &size)
      || !UpdateProcThreadAttribute (si.lpAttributeList, 0,
                                     PROC_THREAD_ATTRIBUTE_HANDLE_LIST, handles,
                                     sizeof (handles), NULL, NULL))
    goto end;

  si.StartupInfo.cb = sizeof (si);
  si.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
  si.StartupInfo.hStdInput = nul;
  si.StartupInfo.hStdOutput = nul;
  si.StartupInfo.hStdError = nul;

  ut_string_sprintf (&cmdline, "\"%s\" %s %llu %llu %lu", exe, arg,
                     (unsigned long long) (uintptr_t) inr,
                     (unsigned long long) (uintptr_t) outw,
                     (unsigned long) GetCurrentProcessId ());

  if (!CreateProcessA (NULL, cmdline, NULL, NULL, TRUE,
                       EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW, NULL, NULL,
                       &si.StartupInfo, &pi))
    goto end;

  CloseHandle (pi.hThread);
  (*pprocess) = (intptr_t) pi.hProcess;
  (*pin) = _open_osfhandle ((intptr_t) inw, _O_WRONLY | _O_BINARY);
  (*pout) = _open_osfhandle ((intptr_t) outr, _O_RDONLY | _O_BINARY);
  inw = outr = NULL;            // (owned by the file descriptors)
  status = 0;

end:
  if (si.lpAttributeList)
  {
    DeleteProcThreadAttributeList (si.lpAttributeList);
    free (si.lpAttributeList);
  }
  ut_free_1d_char (&cmdline);
  if (inr)
    CloseHandle (inr);
  if (outw)
    CloseHandle (outw);
  if (nul && nul != INVALID_HANDLE_VALUE)
    CloseHandle (nul);
  if (status)
  {
    if (inw)
      CloseHandle (inw);
    if (outr)
      CloseHandle (outr);
  }

  return status;
}

// watchdog of a worker: ends it when its parent has ended
static DWORD WINAPI
ut_sys_win_watchdog (LPVOID parent)
{
  WaitForSingleObject ((HANDLE) parent, INFINITE);
  _exit (EXIT_FAILURE);

  return 0;
}

int
ut_sys_win_child_init (int argc, char **argv, int *pin, int *pout)
{
  unsigned long long in, out;
  unsigned long ppid;
  HANDLE parent;

  if (argc < 5 || sscanf (argv[argc - 3], "%llu", &in) != 1
      || sscanf (argv[argc - 2], "%llu", &out) != 1
      || sscanf (argv[argc - 1], "%lu", &ppid) != 1)
    return -1;

  (*pin) = _open_osfhandle ((intptr_t) in, _O_RDONLY | _O_BINARY);
  (*pout) = _open_osfhandle ((intptr_t) out, _O_WRONLY | _O_BINARY);

  parent = OpenProcess (SYNCHRONIZE, FALSE, (DWORD) ppid);
  if (!parent || !CreateThread (NULL, 0, ut_sys_win_watchdog, parent, 0, NULL))
    return -1;

  return ((*pin) < 0 || (*pout) < 0) ? -1 : 0;
}

int
ut_sys_win_readable (int fd, int timeout_ms)
{
  HANDLE h = (HANDLE) _get_osfhandle (fd);
  DWORD avail = 0, beg = GetTickCount ();

  while (1)
  {
    if (!PeekNamedPipe (h, NULL, 0, NULL, &avail, NULL))
      return -1;                // (broken pipe: the process has ended)
    if (avail > 0)
      return 1;
    if (timeout_ms >= 0 && GetTickCount () - beg >= (DWORD) timeout_ms)
      return 0;
    Sleep (1);
  }
}

int
ut_sys_win_running (intptr_t process)
{
  return WaitForSingleObject ((HANDLE) process, 0) == WAIT_TIMEOUT;
}

void
ut_sys_win_kill (intptr_t process)
{
  TerminateProcess ((HANDLE) process, 1);

  return;
}

void
ut_sys_win_wait (intptr_t process)
{
  WaitForSingleObject ((HANDLE) process, INFINITE);
  CloseHandle ((HANDLE) process);

  return;
}

#endif
