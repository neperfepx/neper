/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_FREE_H
#define UT_FREE_H

/// \file ut_free.h
/// \brief Memory unallocation functions
/// \remarks Arrays are not set to NULL, except for functions ending by \c _.
/// \author Romain Quey
/// \bug No known bugs

/// \brief Free a 1-D array of \c double and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d (double **parray);

/// \brief Free a 1-D array of \c int and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_int (int **parray);

/// \brief Free a 1-D array of \c unsigned \c short and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_ushort (unsigned short **parray);

/// \brief Free a 1-D array of \c unsigned \c char and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_uchar (unsigned char **parray);

/// \brief Free a 1-D array of \c char and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_char (char **parray);

/// \brief Free a 1-D array of \c int* and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_pint (int ***parray);

/// \brief Free a 1-D array of \c double* and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_pdouble (double ***parray);

/// \brief Free a 1-D array of \c int** and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_ppint (int ****parray);

/// \brief Free a 1-D array of \c double** and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_ppdouble (double ****parray);

/// \brief Free a 1-D array of \c char** and set it to NULL
/// \param parray: pointer to the array
  extern void ut_free_1d_ppchar (char ****parray);

/// \brief Free a 2-D array of \c double and set it to NULL
/// \param parray: pointer to the array
/// \param size1: 1st-index size of the array
  extern void ut_free_2d (double ***parray, unsigned int size1);

/// \brief Free a 2-D array of \c int and set it to NULL
/// \param parray: pointer to the array
/// \param size1: 1st-index size of the array
  extern void ut_free_2d_int (int ***parray, unsigned int size1);

/// \brief Free a 2-D array of \c int and set it to NULL
/// \param parray: pointer to the array
/// \param size1: 1st-index size of the array
  extern void ut_free_2d_uint (unsigned int ***parray, unsigned int size1);

/// \brief Free a 2-D array of \c char and set it to NULL
/// \param parray: pointer to the array
/// \param size1: 1st-index size of the array
  extern void ut_free_2d_char (char ***parray, unsigned int size1);

/// \brief Free a 3-D array of \c unsigned \c char and set it to NULL
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_free_3d (double ****parray, unsigned int size1,
                           unsigned int size2);

/// \brief Free a 3-D array of \c int and set it to NULL
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_free_3d_int (int ****parray, unsigned int size1,
                               unsigned int size2);

/// \brief Free a 3-D array of \c unsigned \c short and set it to NULL
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_free_3d_ushort (unsigned short ****parray,
                                  unsigned int size1, unsigned int size2);

/// \brief Free a 3-D array of \c unsigned \c char and set it to NULL
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_free_3d_uchar (unsigned char ****parray, unsigned int size1,
                                 unsigned int size2);

/// \brief Free a 3-D array of \c char and set it to NULL
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_free_3d_char (char ****parray, unsigned int size1,
                                unsigned int size2);

/// \brief Free a 3-D array of \c float and set it to NULL
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern void ut_free_3d_float (float ****parray, unsigned int size1,
                                 unsigned int size2);

/// \brief Free a 4-D array of \c unsigned \c char
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern void ut_free_4d (double *****parray, unsigned int size1,
                           unsigned int size2, unsigned int size3);

/// \brief Free a 4-D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern void ut_free_4d_int (int *****parray, unsigned int size1,
                               unsigned int size2, unsigned int size3);

#endif                          /* UT_FREE_H */

#ifdef __cplusplus
}
#endif
