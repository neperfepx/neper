/* Copyright (C) 2003-2020, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_ALLOC_H
#define UT_ALLOC_H

/// \file ut_alloc.h
/// \brief Memory allocation functions
/// \remarks Arrays are initialized to 0 (using \c calloc).
/// \author Romain Quey
/// \bug No known bugs
/// \todo Should we keep the _double functions?

/// \brief Allocate a 1-D array of \c double
/// \param size: size of the array
/// \return pointer to the array
  extern double *ut_alloc_1d (unsigned int size);

/// \brief Allocate a 1-D array of \c double
/// \param size: size of the array
/// \return pointer to the array
  extern double *ut_alloc_1d_double (unsigned int size);

/// \brief Allocate a 1-D array of \c float
/// \param size: size of the array
/// \return pointer to the array
  extern float *ut_alloc_1d_float (unsigned int size);

/// \brief Allocate a 1-D array of \c int
/// \param size: size of the array
/// \return pointer to the array
  extern int *ut_alloc_1d_int (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c short
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned short *ut_alloc_1d_ushort (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c int
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned int *ut_alloc_1d_uint (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c char
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned char *ut_alloc_1d_uchar (unsigned int size);

/// \brief Allocate a 1-D array of \c char
/// \param size: size of the array
/// \return pointer to the array
  extern char *ut_alloc_1d_char (unsigned int size);

/// \brief Allocate a 1-D array of \c size_t
/// \param size: size of the array
/// \return pointer to the array
  extern size_t *ut_alloc_1d_sizet (unsigned int size);

/// \brief Allocate a 1-D array of \c FILE*
/// \param size: size of the array
/// \return pointer to the array
  extern FILE **ut_alloc_1d_pfile (unsigned int size);

/// \brief Allocate a 1-D array of \c int*
/// \param size: size of the array
/// \return pointer to the array
  extern int **ut_alloc_1d_pint (unsigned int size);

/// \brief Allocate a 1-D array of \c double*
/// \param size: size of the array
/// \return pointer to the array
  extern double **ut_alloc_1d_pdouble (unsigned int size);

/// \brief Allocate a 1-D array of \c char*
/// \param size: size of the array
/// \return pointer to the array
  extern char **ut_alloc_1d_pchar (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c short*
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned short **ut_alloc_1d_pushort (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c char*
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned char **ut_alloc_1d_puchar (unsigned int size);

/// \brief Allocate a 1-D array of \c int**
/// \param size: size of the array
/// \return pointer to the array
  extern int ***ut_alloc_1d_ppint (unsigned int size);

/// \brief Allocate a 1-D array of \c double**
/// \param size: size of the array
/// \return pointer to the array
  extern double ***ut_alloc_1d_ppdouble (unsigned int size);

/// \brief Allocate a 1-D array of \c char**
/// \param size: size of the array
/// \return pointer to the array
  extern char ***ut_alloc_1d_ppchar (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c short**
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned short ***ut_alloc_1d_ppushort (unsigned int size);

/// \brief Allocate a 1-D array of \c unsigned \c char**
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned char ***ut_alloc_1d_ppuchar (unsigned int size);

/// \brief Allocate a 2-D array of \c double
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \return pointer to the array
  extern double **ut_alloc_2d (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c double
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern double **ut_alloc_2d_double (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c int
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern int **ut_alloc_2d_int (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c unsigned \c short
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern unsigned short **ut_alloc_2d_ushort (unsigned int size1,
                                              unsigned int size2);

/// \brief Allocate a 2-D array of \c unsigned \c char
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern unsigned char **ut_alloc_2d_uchar (unsigned int size1,
                                            unsigned int size2);

/// \brief Allocate a 2-D array of \c char
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern char ***ut_alloc_2d_pchar (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c int*
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern int ***ut_alloc_2d_pint (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c unsigned \c int
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern unsigned int **ut_alloc_2d_uint (unsigned int size1,
                                          unsigned int size2);

/// \brief Allocate a 2-D array of \c float
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern float **ut_alloc_2d_float (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c char
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
  extern char **ut_alloc_2d_char (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c double with contiguous elements
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \author https://stackoverflow.com/questions/31641509/assigning-memory-for-contiguous-2d-array
/// \note Never been used
  extern double **ut_alloc_2d_contig (unsigned int size1, unsigned int size2);

/// \brief Allocate a 2-D array of \c long \c int with contiguous elements
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \author https://stackoverflow.com/questions/31641509/assigning-memory-for-contiguous-2d-array
/// \note Never been used
  extern long **ut_alloc_2d_contig_long (unsigned int size1,
                                         unsigned int size2);

/// \brief Allocate a 3-D array of \c double
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern double ***ut_alloc_3d (unsigned int size1, unsigned int size2,
                                unsigned int size3);

/// \brief Allocate a 3-D array of \c double
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern double ***ut_alloc_3d_double (unsigned int size1, unsigned int size2,
                                       unsigned int size3);

/// \brief Allocate a 3-D array of \c int
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern int ***ut_alloc_3d_int (unsigned int size1, unsigned int size2,
                                 unsigned int size3);

/// \brief Allocate a 3-D array of \c unsigned \c short
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern unsigned short ***ut_alloc_3d_ushort (unsigned int size1,
                                               unsigned int size2,
                                               unsigned int size3);

/// \brief Allocate a 3-D array of \c unsigned \c char
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern unsigned char ***ut_alloc_3d_uchar (unsigned int size1,
                                             unsigned int size2,
                                             unsigned int size3);

/// \brief Allocate a 3-D array of \c unsigned \c int
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern unsigned int ***ut_alloc_3d_uint (unsigned int size1,
                                           unsigned int size2,
                                           unsigned int size3);

/// \brief Allocate a 3-D array of \c float
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern float ***ut_alloc_3d_float (unsigned int size1, unsigned int size2,
                                     unsigned int size3);

/// \brief Allocate a 3-D array of \c char
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern char ***ut_alloc_3d_char (unsigned int size1, unsigned int size2,
                                   unsigned int size3);

/// \brief Allocate a 3-D array of \c int*
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
  extern int ****ut_alloc_3d_pint (unsigned int size1, unsigned int size2,
                                   unsigned int size3);

/// \brief Allocate a 4-D array of \c double
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param size4: 4th-index size of the array
  extern double ****ut_alloc_4d (unsigned int size1, unsigned int size2,
                                 unsigned int size3, unsigned int size4);

/// \brief Allocate a 4-D array of \c double
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param size4: 4th-index size of the array
  extern double ****ut_alloc_4d_double (unsigned int size1,
                                        unsigned int size2,
                                        unsigned int size3,
                                        unsigned int size4);

/// \brief Allocate a 3-D array of \c int
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the array
/// \param size3: 3rd-index size of the array
/// \param size4: 4th-index size of the array
  extern int ****ut_alloc_4d_int (unsigned int size1, unsigned int size2,
                                  unsigned int size3, unsigned int size4);

/// \brief Reallocate a 1-D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern double *ut_realloc_1d (double *array, unsigned int size);

/// \brief Reallocate a 1-D array of \c double
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern double *ut_realloc_1d_double (double *array, unsigned int size);

/// \brief Reallocate a 1-D array of \c int
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern int *ut_realloc_1d_int (int *array, unsigned int size);

/// \brief Reallocate a 1-D array of \char
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern char *ut_realloc_1d_char (char *array, unsigned int size);

/// \brief Reallocate a 1-D array of \c unsigned \c short
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned short *ut_realloc_1d_ushort (unsigned short *array,
                                               unsigned int size);

/// \brief Reallocate a 1-D array of \c unsigned \c char
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned char *ut_realloc_1d_uchar (unsigned char *array,
                                             unsigned int size);

/// \brief Reallocate a 1-D array of \c double*
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern double **ut_realloc_1d_pdouble (double **array, unsigned int size);

/// \brief Reallocate a 1-D array of \c int*
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern int **ut_realloc_1d_pint (int **array, unsigned int size);

/// \brief Reallocate a 1-D array of \c char
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern char **ut_realloc_1d_pchar (char **array, unsigned int size);

/// \brief Reallocate a 1-D array of \c unsigned \c short*
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned short **ut_realloc_1d_pushort (unsigned short **array,
                                                 unsigned int size);

/// \brief Reallocate a 1-D array of \c unsigned \c char
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern unsigned char **ut_realloc_1d_puchar (unsigned char **array,
                                               unsigned int size);

/// \brief Reallocate a 1-D array of \c int**
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern int ***ut_realloc_1d_ppint (int ***array, unsigned int size);

/// \brief Reallocate a 1-D array of \c double**
/// \param array: array
/// \param size: size of the array
/// \return pointer to the array
  extern double ***ut_realloc_1d_ppdouble (double ***array,
                                           unsigned int size);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern double **ut_realloc_2d_addline (double **array, int size1,
                                         int size2);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern double **ut_realloc_2d_double_addline (double **array, int size1,
                                                int size2);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern int **ut_realloc_2d_int_addline (int **array, int size1, int size2);

/// \brief Reallocate a 2-D array of \¢ char
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern char **ut_realloc_2d_char_addline (char **array, int size1,
                                            int size2);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size_before: initial size of the array
/// \param size_after: final size of the array
/// \return pointer to the array
  extern double **ut_realloc_2d_delline (double **array, int size_before,
                                         int size_after);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size_before: initial size of the array
/// \param size_after: final size of the array
/// \return pointer to the array
  extern double **ut_realloc_2d_double_delline (double **array,
                                                int size_before,
                                                int size_after);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size_before: initial size of the array
/// \param size_after: final size of the array
/// \return pointer to the array
  extern int **ut_realloc_2d_int_delline (int **array, int size_before,
                                          int size_after);

/// \brief Reallocate a 2-D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2_before: initial 2st-index size of the array
/// \param size2_after: final 2nd-index size of the array
/// \return pointer to the array
  extern int **ut_realloc_2d_int_size2 (int **array, int size1,
                                        int size2_before, int size2_after);

/// \brief Reallocate a 3-D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \param size3: 3rd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern double ***ut_realloc_3d_addarray (double ***array, int size1,
                                           int size2, int size3);

/// \brief Reallocate a 3-D array of \c double
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \param size3: 3rd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern double ***ut_realloc_3d_double_addarray (double ***array, int size1,
                                                  int size2, int size3);

/// \brief Reallocate a 3-D array of \c int
/// \param array: array
/// \param size1: 1st-index size of the array
/// \param size2: 2nd-index size of the new element of the array
/// \param size3: 3rd-index size of the new element of the array
/// \attention To be used only to add 1 1st-index element to the array (otherwise, intermediate elements will not be initialized)
/// \return pointer to the array
  extern int ***ut_realloc_3d_int_addarray (int ***array, int size1,
                                            int size2, int size3);

#endif                          /* UT_ALLOC_H */

#ifdef __cplusplus
}
#endif
