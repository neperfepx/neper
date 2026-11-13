/* Copyright (C) 2003-2026, Romain Quey, CNRS.*/
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_SYS_H
#define UT_SYS_H

/// \file ut_sys.h
/// \brief System functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Create a directory
/// \param dir: directory
/// \note Several arguments can be provided, à la printf
/// \note Abort on failure
  extern void ut_sys_mkdir (const char *dir, ...);

/// \brief Check if path is a directory
/// \param path: absolute or relative path
/// \return 1 if directory, 0 otherwise
  extern int ut_sys_isdir (const char *path, ...);

/// \brief Get the canonicalized absolute pathname
/// \param path: input path
/// \return real path
/// \note See \c man \c 3 \c realpath
  extern char *ut_sys_realpath (const char *path);

/// \brief Get the path of the current binary
/// \param *ppath: path
/// \return 0 on success, -1 on failure
  extern int ut_sys_currentbin_path (char **ppath);

/// \brief Get the system endianness
/// \return 0 for little endian, 1 for big endian
  extern int ut_sys_endian (void);

/// \brief Quote a string for use as a single word of a shell command
/// \param string: string
/// \param *pquoted: quoted string (reallocated)
/// \note A leading \c ~/ is left unquoted, so that the shell expands it
  extern void ut_sys_shellquote (const char *string, char **pquoted);

/// \brief Handle Ctrl+C while a subprocess runs: the subprocess (which
/// receives Ctrl+C too) ends, and Ctrl+C pressed again within 2 seconds
/// ends execution
/// \param pctrlc_t: time of the last Ctrl+C
/// \note ut_sys_ctrlc_begin is called before running the subprocess, and
/// ut_sys_ctrlc_end after
  extern void ut_sys_ctrlc_begin (struct timeval *pctrlc_t);
  extern void ut_sys_ctrlc_end (struct timeval *pctrlc_t);

#endif                          /* UT_SYS_H */

#ifdef __cplusplus
}
#endif
