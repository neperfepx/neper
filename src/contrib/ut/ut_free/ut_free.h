/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_FREE_H
#define UT_FREE_H

extern void ut_free_1d_int (int *);
extern void ut_free_1d_ushort (unsigned short *);
extern void ut_free_1d_uchar (unsigned char *);
extern void ut_free_1d_uint (unsigned int *);
extern void ut_free_1d_sizet (size_t *);
extern void ut_free_1d_float (float *);
extern void ut_free_1d_double (double *);
extern void ut_free_1d_pint (int **);
extern void ut_free_1d_pchar (char **);
extern void ut_free_1d_ppint (int ***);
extern void ut_free_1d_ppint_ (int ****);
extern void ut_free_1d_ppchar (char ***);
extern void ut_free_1d_char (char *);
extern void ut_free_1d_char_ (char **);
extern void ut_free_1d (double *);
extern void ut_free_1d_pfile (FILE **);
extern void ut_free_1d_pdouble (double **);
extern void ut_free_1d_ppdouble (double ***);
extern void ut_free_2d_int (int **, unsigned int);
extern void ut_free_2d_int_ (int ***, unsigned int);
extern void ut_free_2d_ (double ***, unsigned int);
extern void ut_free_2d_ushort (unsigned short **, unsigned int);
extern void ut_free_2d_uchar (unsigned char **, unsigned int);
extern void ut_free_2d_uint (unsigned int **, unsigned int);
extern void ut_free_2d_float (float **, unsigned int);
extern void ut_free_2d_double (double **, unsigned int);
extern void ut_free_2d_char (char **, unsigned int);
extern void ut_free_2d (double **, unsigned int);
extern void ut_free_3d_int (int ***, unsigned int, unsigned int);
extern void ut_free_3d_ushort (unsigned short ***, unsigned int, unsigned int);
extern void ut_free_3d_uchar (unsigned char ***, unsigned int, unsigned int);
extern void ut_free_3d_char (char ***, unsigned int, unsigned int);
extern void ut_free_3d_double (double ***, unsigned int, unsigned int);
extern void ut_free_3d_float (float ***, unsigned int, unsigned int);
extern void ut_free_3d (double ***, unsigned int, unsigned int);
extern void ut_free_3d_char (char ***, unsigned int, unsigned int);
extern void ut_free_4d (double ****, unsigned int, unsigned int, unsigned int);
extern void ut_free_4d_int (int ****, unsigned int, unsigned int, unsigned int);

extern void ut_free_1d_int_ (int **);
extern void ut_free_1d_ (double **);

extern void ut_free_2d_contig (double **p);
extern void ut_free_2d_contig_long (long **p);

#endif /* UT_FREE_H */

#ifdef __cplusplus
}
#endif
