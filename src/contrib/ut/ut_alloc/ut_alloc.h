/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_ALLOC_H
#define UT_ALLOC_H

/// \brief Allocate a 1-D array of double-precision numbers
///
/// The input is the size of the array.
///
extern double* ut_alloc_1d (unsigned int);

/// \brief Allocate a 1-D array of double-precision numbers
///
/// The input is the size of the array.
///
extern double* ut_alloc_1d_double (unsigned int);

/// \brief Allocate a 1-D array of single-precision numbers
///
/// The input is the size of the array.
///
extern float* ut_alloc_1d_float (unsigned int);

/// \brief Allocate a 1-D array of integer numbers
///
/// The input is the size of the array.
///
extern int* ut_alloc_1d_int (unsigned int);

/// \brief Allocate a 1-D array of unsigned short integer numbers
///
/// The input is the size of the array.
///
extern unsigned short* ut_alloc_1d_ushort (unsigned int);

/// \brief Allocate a 1-D array of unsigned integer numbers
///
/// The input is the size of the array.
///
extern unsigned int* ut_alloc_1d_uint (unsigned int);

/// \brief Allocate a 1-D array of unsigned characters
///
/// The input is the size of the array.
///
extern unsigned char* ut_alloc_1d_uchar (unsigned int);

/// \brief Allocate a 1-D array of characters
///
/// The input is the size of the array.
///
extern char* ut_alloc_1d_char (unsigned int);

/// \brief Allocate a 1-D array of size_t
///
/// The input is the size of the array.
///
extern size_t* ut_alloc_1d_sizet (unsigned int);

/// \brief Allocate a 1-D array of file pointers
///
/// The input is the size of the array.
///
extern FILE** ut_alloc_1d_pfile (unsigned int);

/// \brief Allocate a 1-D array of pointers of integer numbers
///
/// The input is the size of the array.
///
extern int** ut_alloc_1d_pint (unsigned int);

/// \brief Allocate a 1-D array of pointers of double-precision numbers
///
/// The input is the size of the array.
///
extern double** ut_alloc_1d_pdouble (unsigned int);

/// \brief Allocate a 1-D array of pointers of characters
///
/// The input is the size of the array.
///
extern char** ut_alloc_1d_pchar (unsigned int);

/// \brief Allocate a 1-D array of pointers of unsigned short integer numbers
///
/// The input is the size of the array.
///
extern unsigned short** ut_alloc_1d_pushort (unsigned int);

/// \brief Allocate a 1-D array of pointers of unsigned characters
///
/// The input is the size of the array.
///
extern unsigned char** ut_alloc_1d_puchar (unsigned int);

/// \brief Allocate a 1-D array of pointers of pointers of integer numbers
///
/// The input is the size of the array.
///
extern int*** ut_alloc_1d_ppint (unsigned int);

/// \brief Allocate a 1-D array of pointers of pointers of double-precision numbers
///
/// The input is the size of the array.
///
extern double*** ut_alloc_1d_ppdouble (unsigned int);

/// \brief Allocate a 1-D array of pointers of pointers of characters
///
/// The input is the size of the array.
///
extern char*** ut_alloc_1d_ppchar (unsigned int);

/// \brief Allocate a 1-D array of pointers of pointers of unsigned short integer numbers.
///
/// The input is the size of the array.
///
extern unsigned short*** ut_alloc_1d_ppushort (unsigned int);

/// \brief Allocate a 1-D array of pointers of pointers of unsigned characters
///
/// The input is the size of the array.
///
extern unsigned char*** ut_alloc_1d_ppuchar (unsigned int);

extern int** ut_alloc_2d_int (unsigned int, unsigned int);
extern unsigned short** ut_alloc_2d_ushort (unsigned int, unsigned int);
extern unsigned char** ut_alloc_2d_uchar (unsigned int, unsigned int);
extern char*** ut_alloc_2d_pchar (unsigned int, unsigned int);
extern int*** ut_alloc_2d_pint (unsigned int, unsigned int);
extern unsigned int** ut_alloc_2d_uint (unsigned int, unsigned int);
extern float** ut_alloc_2d_float (unsigned int, unsigned int);
extern double** ut_alloc_2d_double (unsigned int, unsigned int);
extern char** ut_alloc_2d_char (unsigned int, unsigned int);
extern double** ut_alloc_2d (unsigned int, unsigned int);
extern int*** ut_alloc_3d_int (unsigned int, unsigned int, unsigned int);
extern unsigned short*** ut_alloc_3d_ushort (unsigned int, unsigned int, unsigned int);
extern unsigned char*** ut_alloc_3d_uchar (unsigned int, unsigned int, unsigned int);
extern unsigned int*** ut_alloc_3d_uint (unsigned int, unsigned int, unsigned int);
extern double*** ut_alloc_3d_double (unsigned int, unsigned int,
				     unsigned int);
extern float*** ut_alloc_3d_float (unsigned int, unsigned int,
				     unsigned int);
extern double*** ut_alloc_3d (unsigned int, unsigned int, unsigned int);
extern char*** ut_alloc_3d_char (unsigned int, unsigned int, unsigned int);

extern int**** ut_alloc_3d_pint (unsigned int, unsigned int, unsigned int);
extern int**** ut_alloc_4d_int (unsigned int, unsigned int,
			       unsigned int, unsigned int);
extern double**** ut_alloc_4d_double (unsigned int, unsigned int,
				      unsigned int, unsigned int);
extern double**** ut_alloc_4d (unsigned int, unsigned int,
				      unsigned int, unsigned int);

extern double** ut_realloc_1d_pdouble (double**, unsigned int);
extern int* ut_realloc_1d_int (int*, unsigned int);

extern char* ut_realloc_1d_char (char*, unsigned int);
extern int** ut_realloc_1d_pint (int**, unsigned int);
extern unsigned short** ut_realloc_1d_pushort (unsigned short**, unsigned int);
extern unsigned char** ut_realloc_1d_puchar (unsigned char**, unsigned int);
extern int*** ut_realloc_1d_ppint (int***, unsigned int);
extern double*** ut_realloc_1d_ppdouble (double***, unsigned int);
extern char** ut_realloc_1d_pchar (char**, unsigned int);

extern unsigned short* ut_realloc_1d_ushort (unsigned short*, unsigned int);
extern unsigned char* ut_realloc_1d_uchar (unsigned char*, unsigned int);
extern double* ut_realloc_1d (double*, unsigned int);
extern int** ut_realloc_2d_int_addline (int**, int, int);
extern int ** ut_realloc_2d_int_size2 (int **array, int line, int col1,
    int col2);
extern double** ut_realloc_2d_addline (double**, int, int);
extern double*** ut_realloc_3d_addarray (double***, int, int, int);
extern int*** ut_realloc_3d_int_addarray (int***, int, int, int);
extern char** ut_realloc_2d_char_addline (char**, int, int);
extern int** ut_realloc_2d_int_delline (int**, int, int);
extern double** ut_realloc_2d_delline (double**, int, int);

#endif /* UT_ALLOC_H */

#ifdef __cplusplus
}
#endif
