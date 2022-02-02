/* Copyright (C) 2003-2022, Romain Quey */
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

/// \brief Run a command with time limit
/// \param exec: executable
/// \param command: command
/// \param tmax: maximal time
/// \note Ctrl+C also ends execution
/// \return process id on success, -1 on failure
/// \author Romain Quey, Thierry Auger
  extern int ut_sys_runwtime (char *exec, char *command, double tmax,
                              struct timeval *pctrlc_t);

#endif                          /* UT_SYS_H */

#ifdef __cplusplus
}
#endif
