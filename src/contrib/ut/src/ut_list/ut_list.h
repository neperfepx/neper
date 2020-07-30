/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_LIST_H
#define UT_LIST_H

/// \file ut_list.h
/// \brief List-related functions
/// \author Romain Quey
/// \bug No known bugs
/// \todo improve ut_list_iter_test

/// \brief Break a list
/// \param list: list
/// \param sep: separator, e.g. "," or ":"
/// \param *pparts: parts
/// \param *psize: number of parts
  extern void ut_list_break (const char *list, const char *sep,
                             char ***pparts, int *psize);

/// \brief Break a list twice
/// \param list: list
/// \param sep1: first separator, e.g. ","
/// \param sep2: second separator, e.g. ":"
/// \param *pparts: array of parts, \c [0:*psize1-1]
/// \param *psize1: number of primary parts (split using \c sep1)
/// \param *psize2: array of number of secondary parts (split using \c sep2)
  extern void ut_list_break2 (const char *list, const char *sep1,
                              const char *sep2, char ****pparts, int **psize2,
                              int *psize1);

/// \brief Break a list into a series of double-precision numbers
/// \param list: list
/// \param sep: separator, e.g. "," or ":"
/// \param *pparts: parts
/// \param *psize: number of parts
  extern void ut_list_break_double (const char *list, const char *sep,
                                    double **pparts, int *psize);

/// \brief Create a list
/// \param parts: parts
/// \param size: number of parts
/// \param sep: separator, e.g. "," or ":"
/// \param *plist: list
  extern void ut_list_create (char **parts, int size, const char *sep,
                              char **plist);

/// \brief Test if an element belongs to a list
/// \param list: list
/// \param sep: separator, e.g. "," or ":"
/// \param elt: element
/// \param 1 on success and 0 on failure
  extern int ut_list_testelt (const char *list, const char *sep,
                              const char *elt);

/// \brief Test if an \c int element belongs to a list
/// \param list: list
/// \param sep: separator, e.g. "," or ":"
/// \param elt: element
/// \return 1 on success and 0 on failure
  extern int ut_list_testelt_int (char *list, char *sep, int elt);

/// \brief Add an element to a list
/// \param *plist: list
/// \param sep: separator, e.g. "," or ":"
/// \param elt: element
  extern void ut_list_addelt (char **plist, char *sep, char *elt);

/// \brief Remove an element from a list
/// \param *plist: list
/// \param sep: separator, e.g. "," or ":"
/// \param elt: element
  extern int ut_list_rmelt (char **plist, char *sep, char *elt);

/// \todo To improve
  extern int ut_list_iter_test (char *list, char *sep, int iter);

#endif                          /* UT_LIST_H */

#ifdef __cplusplus
}
#endif
