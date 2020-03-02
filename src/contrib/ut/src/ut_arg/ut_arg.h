/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_ARG_H
#define UT_ARG_H

/// \file ut_arg.h
/// \brief Argument-related functions
/// \author Romain Quey
/// \bug No known bugs

  extern void ut_arg_nextasint (char **, int *, char *, int, int, int *);
  extern void ut_arg_nextaslogical (char **, int *, char *, int *);
  extern void ut_arg_nextasreal (char **, int *, char *, double, double,
                                 double *);
  extern void ut_arg_nextasstring (char **, int *, char *, char **);
  extern void ut_arg_badarg (void);
  extern void ut_arg_error (char *, char *);
  extern int ut_arg_listqty (char *);
  extern char *ut_arg_listmember (char *, int, int);

  extern void ut_arg_expand (int, char **, int *, char ***);

/// \brief Read args from an arg file
/// \param argfile: arg file (e.g. \c ~/.neperrc)
/// \param program: program (e.g. \c neper)
/// \param module: module (e.g. \c -T)
/// \param pargc: pointer to the number of args
/// \param pargv: pointer to the args
  extern void ut_arg_readfromfile (char *argfile, char *program, char *module,
                                   int *pargc, char ***pargv);

#endif                          /* UT_ARG_H */

#ifdef __cplusplus
}
#endif
