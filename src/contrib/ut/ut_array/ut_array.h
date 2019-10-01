/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef HAVE_GSL
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>
#endif // HAVE_GSL

#ifndef UT_ARRAY_H
#define UT_ARRAY_H

extern void ut_array_1d_scale (double *, int, double);
extern void ut_array_1d_d2ri (double *, int, int *);

extern void ut_array_1d_normalize (double *, int);
extern void ut_array_1d_int_scale (int *, int, double);
extern void ut_array_1d_add (double *, double *, int, double *);
extern void ut_array_1d_addval (double *, int, double, double *);
extern void ut_array_1d_int_add (int *, int *, int, int *);
extern void ut_array_2d_add (double **, double **, int, int, double **);
extern void ut_array_3d_add (double ***, double ***, int, int, int,
			     double ***);
extern void ut_array_3d_add_float (float ***, float ***, int, int, int,
				   float ***);
extern void ut_array_1d_int_addval (int *, int, int, int *);
extern void ut_array_2d_int_addval (int **, int, int, int, int **);
extern double ut_array_1d_norm (double *, int);
extern double ut_array_1d_norm_int (int *, int);
extern void ut_array_1d_zero (double *, int);
extern void ut_array_1d_int_zero (int *, int);
extern void ut_array_2d_zero (double **, int, int);
extern void ut_array_3d_zero (double ***, int, int, int);
extern void ut_array_3d_zero_float (float ***, int, int, int);
extern void ut_array_2d_int_zero (int **, int, int);
extern void ut_array_3d_int_zero (int ***, int, int, int);
extern void ut_array_1d_zero_be (double *, int, int);
extern void ut_array_1d_float_zero_be (float *, int, int);
extern void ut_array_1d_int_zero_be (int *, int, int);
extern void ut_array_2d_zero_be (double **, int, int, int, int);
extern void ut_array_2d_float_zero_be (float **, int, int, int, int);
extern void ut_array_2d_int_zero_be (int **, int, int, int, int);
extern int ut_array_1d_fscanf (FILE *, double *, int);
extern int ut_array_1d_fscanfn (char *, double *, int);
extern int ut_array_1d_int_fscanf (FILE *, int *, int);
extern int ut_array_1d_char_fscanf (FILE *, char **, int);
extern int ut_array_1d_int_fscanfn (char *, int *, int);
extern int ut_array_2d_fscanf (FILE *, double **, int, int);
extern int ut_array_2d_fscanfn (char *, double **, int, int);
extern int ut_array_2d_fscanfn_wcard (char *, double **, int, int, char *);
extern int ut_array_1d_fscanfn_wcard (char *, double *, int, char *);
extern int ut_array_1d_int_fscanfn_wcard (char *, int *, int, char *);
extern int ut_array_1d_fprintf (FILE *, double *, int, const char *);
extern int ut_array_1d_float_fprintf (FILE *, float *, int, const char *);
extern int ut_array_1d_int_fprintf (FILE *, int *, int, const char *);
extern int ut_array_1d_int_fprintf_nonl (FILE *, int *, int, const char *);
extern int ut_array_1d_fprintf_nonl (FILE *, double *, int, const char *);
extern int ut_array_1d_int_fprintf_sep (FILE *, int *, int, char *,
    const char*);
extern int ut_array_1d_uint_fprintf (FILE *, unsigned int *, int, const char *);
extern int ut_array_2dns_int_fprintf (FILE *, int **, int, int *, const char *);
extern int ut_array_2d_fprintf (FILE *, double **, int, int, const char *);
extern int ut_array_2d_fprintf_oneline (FILE * fileid, double **array,
				    int size1, int size2, const char *format);
extern int ut_array_2d_fprintf_col (FILE *, double **, int, int, int *, int,
				    const char *);
extern int ut_array_2d_float_fprintf (FILE *, float **, int, int, const char *);
extern int ut_array_2d_int_fprintf (FILE *, int **, int, int, const char *);
extern int ut_array_3d_int_fprintf (FILE *, int ***, int, int, int,
    const char *);

extern int ut_array_2d_int_fscanf (FILE *, int **, int, int);

extern void ut_array_2d_scale (double **, int, int, double);

extern double ut_array_1d_min (double *, int);
extern int ut_array_2d_int_min (int**, int, int);
extern double ut_array_2d_col_min (double **, int, int);
extern double ut_array_2d_col_max (double **, int, int);
extern int ut_array_1d_min_index (double *, int);
extern double ut_array_1d_max (double *, int);
extern int ut_array_2d_int_max (int**, int, int);
extern double ut_array_1d_mean (double *, int);
extern double ut_array_1d_mean_pow (double *, int, double);
extern double ut_array_1d_mean_powstring (double *, int, char*);
extern double ut_array_1d_gmean (double *, int);
extern double ut_array_1d_int_mean (int *, int);
extern double ut_array_1d_int_gmean (int *, int);
extern double ut_array_1d_wmean (double *, double *, int);
extern double ut_array_1d_stddev (double *, double, int);
extern double ut_array_1d_wstddev (double *, double *, double, int);
extern double ut_array_2d_stddev (double **, double, int, int);
extern double ut_array_1d_absmax (double *, int);
extern double ut_array_1d_absmin (double *, int);
extern int ut_array_1d_max_index (double *, int);
extern int ut_array_1d_int_max_index (int *, int);
extern int ut_array_1d_absmax_index (double *, int);
extern int ut_array_1d_absmin_index (double *, int);
extern int ut_array_1d_int_min (int *, int);
extern int ut_array_1d_int_max (int *, int);
extern float ut_array_1d_float_min (float *, int);
extern float ut_array_1d_float_max (float *, int);
extern float ut_array_2d_float_min (float **, int, int);
extern float ut_array_2d_float_max (float **, int, int);
extern double ut_array_2d_min (double **, int, int);
extern double ut_array_2d_mean (double **, int, int);
extern double ut_array_2d_col_mean (double **, int, int);
extern double ut_array_2d_gmean (double **, int, int);
extern double ut_array_3d_mean (double ***, int, int, int);
extern double ut_array_3d_gmean (double ***, int, int, int);
extern double ut_array_3d_min (double ***, int, int, int);
extern double ut_array_3d_max (double ***, int, int, int);
extern double ut_array_3d_stddev (double ***, double, int, int, int);
extern float ut_array_3d_float_mean (float ***, int, int, int);
extern float ut_array_3d_float_gmean (float ***, int, int, int);
extern float ut_array_3d_float_min (float ***, int, int, int);
extern float ut_array_3d_float_max (float ***, int, int, int);
extern float ut_array_3d_float_stddev (float ***, float, int, int, int);
extern double ut_array_3d_int_mean (int ***, int, int, int);
extern double ut_array_3d_int_gmean (int ***, int, int, int);
extern int ut_array_3d_int_min (int ***, int, int, int);
extern int ut_array_3d_int_max (int ***, int, int, int);
extern double ut_array_3d_int_stddev (int ***, double, int, int, int);
extern double ut_array_3d_uchar_mean (unsigned char ***, int, int, int);
extern double ut_array_3d_uchar_gmean (unsigned char ***, int, int, int);
extern unsigned char ut_array_3d_uchar_min (unsigned char ***, int, int, int);
extern unsigned char ut_array_3d_uchar_max (unsigned char ***, int, int, int);
extern double ut_array_3d_uchar_mean (unsigned char ***, int, int, int);
extern double ut_array_3d_uchar_gmean (unsigned char ***, int, int, int);
extern unsigned short ut_array_3d_ushort_min (unsigned short ***, int, int,
					      int);
extern unsigned short ut_array_3d_ushort_max (unsigned short ***, int, int,
					      int);
extern double ut_array_3d_ushort_stddev (unsigned short ***, double, int, int,
					 int);
extern double ut_array_3d_uchar_stddev (unsigned char ***, double, int, int,
					int);
extern double ut_array_2d_max (double **, int, int);
extern double ut_array_2d_absmax (double **, int, int);
extern int ut_array_1d_int_max_index (int *, int);
extern double ut_array_1d_sum (double *, int);
extern double ut_array_1d_prod (double *, int);
extern int ut_array_1d_int_scalprod (int *, int *, int);
extern double ut_array_1d_scalprod (double *, double *, int);
extern int ut_array_1d_int_sum (int *, int);
extern int ut_array_1d_int_prod (int *, int);
extern unsigned int ut_array_1d_uint_sum (unsigned int *, int);
extern double ut_array_1d_abssum (double *, int);
extern int ut_array_1d_int_abssum (int *, int);
extern void ut_array_1d_abs (double *, int);
extern void ut_array_1d_int_abs (int *, int);
extern void ut_array_1d_int_sgn (int *, int, int *);

extern void ut_array_1d_memcpy (double *, int, double *);
extern void ut_array_1d_memcpy_fromint (double *, int, int *);
extern void ut_array_1d_int_memcpy (int *, int, int *);
extern void ut_array_1d_uint_memcpy (unsigned int *, int, unsigned int *);
extern void ut_array_2d_memcpy (double **, int, int, double **);
extern void ut_array_2d_int_memcpy (int **, int, int, int **);
extern void ut_array_1d_pchar_memcpy (char **, int, char **);
extern void ut_array_3d_memcpy (double ***, int, int, int, double ***);
extern void ut_array_4d_memcpy (double ****, int, int, int, int, double ****);
extern void ut_array_3d_int_memcpy (int ***, int, int, int, int ***);
extern void ut_array_3d_uchar_memcpy (unsigned char ***, int, int, int,
				      unsigned char ***);
extern void ut_array_3d_float_memcpy (float ***, int, int, int, float ***);

extern void ut_array_1d_switch (double *, int, int);
extern void ut_array_1d_int_switch (int *, int, int);
extern void ut_array_1d_sort (double *, int);
extern void ut_array_1d_sort_des (double *, int);
extern void ut_array_1d_int_sort_des (int *, int);
extern void ut_array_1d_sort_index (double *, int, int *);
extern void ut_array_1d_sort_perm (double *, int, int *);
extern void ut_array_1d_sort_index_perm (int *, int, int *);
extern void ut_array_1d_int_sort (int *, int);
extern void ut_array_1d_int_sort_index (int *, int, int *);
extern void ut_array_1d_int_dsort_index (int *, int, int *);
extern void ut_array_1d_pchar_sort (char **, int);
extern void ut_array_1d_abssort (double *, int);
extern void ut_array_1d_abssort_index (double *, int, int *);
extern void ut_array_1d_int_abssort_index (int *, int, int *);
extern int ut_array_1d_int_find (int *, int, int);
extern void ut_array_1d_int_findnreplace (int *, int, int, int);

extern void ut_array_2d_int_sortbycol (int **, int, int, int);
extern void ut_array_2d_int_sortbycol_des (int **, int, int, int);

extern void ut_array_1d_view (double *, int);
extern void ut_array_2d_view (double **, int, int);
extern void ut_array_1d_int_view (int *, int);
extern void ut_array_2d_int_view (int **, int, int);

extern void ut_array_2d_switchlines (double **, int, int, int);
extern void ut_array_2d_int_switchlines_length (int **, int, int, int, int);
extern void ut_array_2d_int_switchlines (int **, int, int, int);

extern void ut_array_1d_int_bubbleelt (int *, int, int);

extern void ut_array_1d_int_rotate (int *, int, int);
extern void ut_array_1d_rotate (double *, int, int);

extern void ut_array_1d_int_reverseelts (int *, int);
extern void ut_array_1d_sort_des_index (double *, int, int *);
extern void ut_array_1d_int_sort_des_index (int *, int, int *);
extern void ut_array_1d_reverseelts (double *, int);
extern int ut_array_1d_max_int_index (int *, int);

extern void ut_array_1d_permutation (double *, int, int *);
extern void ut_array_1d_permutation_int (int *, int, int *);
extern void ut_array_2d_permutation_int (int **, int, int, int *);

extern void ut_array_distribparam (double *data, int n, double step, double *pfirst,
		       int *plength);
extern void ut_array_distrib (double *, int, double, double, int, double *);

extern void ut_array_1d_set (double *, int, double);
extern void ut_array_1d_int_set (int *, int, int);
extern void ut_array_1d_uint_set (unsigned int *, int, unsigned int);
extern void ut_array_2d_set (double **, int, int, double);
extern void ut_array_2d_int_set (int **, int, int, int);
extern void ut_array_2d_int_col_set (int **, int, int, int);
extern void ut_array_2d_col_set (double **, int, int, double);
extern void ut_array_3d_set (double ***, int, int, int, double);
extern void ut_array_3d_int_set (int ***, int, int, int, int);
extern int ut_array_1d_rotpos (int, int, int, int);
extern int ut_array_1d_int_eltpos (int *, int, int);
extern int ut_array_1d_eltpos (double *, int, double);
extern int ut_array_1d_uint_eltpos (unsigned int *array, int size, unsigned int val);
extern int ut_array_2d_int_linepos (int **, int, int, int *);
extern int ut_array_2dns_int_linepos (int **, int, int *, int *, int);
extern int ut_array_1d_int_eltpos_all (int *, int, int, int *);
extern int ut_array_2d_int_eltpos (int **, int, int, int, int *, int *);
extern int ut_array_1d_string_eltpos (char **, int, char *);

extern double ut_array_1d_valuepos (double *, int, double);
extern void ut_array_1d_int_uniq (int *, int *);
extern void ut_array_1d_int_sort_uniq (int *, int, int *);
extern void ut_array_1d_sub (double *, double *, int, double *);

extern void ut_array_2d_sub (double **, double **, int, int, double **);

extern void ut_array_1d_sample (double *, int, double, double **, int *);
extern void ut_array_2d_sample (double **, int, int, double, double ***,
				int *);

extern int ut_array_1d_fscanf_sample (FILE *, int, double, double **, int *);
extern int ut_array_2d_fscanf_sample (FILE *, int, int, double, double ***,
				      int *);

extern int ut_array_1d_int_nbofthisval (int *, int, int);
extern int ut_array_1d_uint_nbofthisval (unsigned int *, int, unsigned int);
extern void ut_array_1d_commonelts (int *, int, int *, int, int **, int *);
extern int *ut_array_1d_int_cat (int *, int, int *, int);
extern int ut_array_1d_int_deletencompress (int *, int, int, int);
extern void ut_array_1d_int_memcpy_be (int *, int *, int, int);

extern void ut_array_1d_int_union (int *, int, int *, int, int *, int *);
extern void ut_array_1d_int_minus (int *, int, int *, int, int *, int);

extern void ut_array_1d_int_inter (int *, int, int *, int, int *, int *);
extern int ut_array_1d_int_min_index (int *, int);

extern int ut_array_1d_int_inv (int *, int, int **, int *);

extern void ut_array_1d_int_sort_index2 (int *, int, int *);
extern void ut_array_1d_sort_index2 (double *, int, int *);

extern int ut_array_2dns_int_fprintf (FILE *, int **, int, int *, const char *);
extern int ut_array_1d_int_equal (int *, int, int *, int);
extern int ut_array_1d_int_opposite (int *, int *, int);
extern int ut_array_1d_equal (double *, double *, int, double);
extern int ut_array_1d_requal (double *, double *, int, double);

extern int ut_array_2d_equal (double **, double **, int, int, double);

extern double ut_array_2d_cov (double **, double, double, int, int, int);
extern double ut_array_2d_linearcorr (double **, int, int, int);

extern void ut_array_2d_transpose (double **, int, int, double ***);
extern void ut_array_2d_int_transpose (int **, int, int, int ***);

extern int ut_array_1d_char_find (char *, int, char);

extern int ut_array_1d_int_list_addelt_nocheck (int **parray, int *psize, int val);
extern int ut_array_1d_int_list_addelt (int **parray, int *psize, int val);
extern void ut_array_1d_int_list_addelts (int **parray, int *psize, int* elts, int eltqty);
extern void ut_array_1d_int_list_rmelt (int **parray, int *psize, int val);
extern void ut_array_1d_int_list_rmelts (int **parray, int *psize, int *vals, int valqty);
extern void ut_array_1d_int_valqty (int *array, int size, int ***parray2,
				    int *parray2qty);

extern int ut_array_1d_int_diff (int *, int, int *, int);

extern int ut_array_1d_int_percent (int *array, int size, int *percent);

extern int ut_array_3d_int_1d (int*** array, int size1, int size2, int size3,
                    int** parray1d);

extern int ut_array_1d_int_set_2 (int* array, int v1, int v2);
extern int ut_array_1d_int_set_3 (int* array, int v1, int v2, int v3);
extern int ut_array_1d_int_set_4 (int* array, int v1, int v2, int v3, int v4);
extern int ut_array_1d_int_set_5 (int* array, int v1, int v2, int v3, int v4, int v5);
extern int ut_array_1d_set_2 (double* array, double v1, double v2);
extern int ut_array_1d_set_3 (double* array, double v1, double v2, double v3);
extern int ut_array_1d_set_4 (double* array, double v1, double v2, double v3, double v4);
extern int ut_array_1d_set_5 (double* array, double v1, double v2, double v3, double v4, double v5);
extern int ut_array_1d_set_6 (double* array, double v1, double v2, double v3, double v4, double v5, double v6);
extern int ut_array_1d_set_7 (double* array, double v1, double v2, double v3, double v4, double v5, double v6, double v7);

extern void ut_array_1d_int_set_id (int* array, int size);

extern void ut_array_1d_sort_permarray_int (double *array, int size,
    int* permarray);

extern void ut_array_1d_int_shift (int *array, int n1, int num);

extern int ut_array_0d_fscanf_filter_prefix (FILE* file, double* pval,
    char *flag);
extern int ut_array_0d_char_fscanf_filter_prefix (FILE* file, char* pval,
    char *flag);
extern int ut_array_1d_char_fscanf_filter_prefix (FILE* file, char** pval,
    int size, char *flag);
extern int ut_array_1d_fscanf_filter_prefix (FILE* file, double* array,
    int size, char *flag);
extern int ut_array_1d_int_fscanf_filter_prefix (FILE* file, int
    *array, int size, char *flag);
extern int ut_array_2d_fscanf_filter_prefix (FILE *file, double** array,
    int size1, int size2, char *flag);
extern int ut_array_2d_int_fscanf_filter_prefix (FILE *file, int** array,
    int size1, int size2, char *flag);

extern int ut_array_0d_fscanf_filter_prefix_int (FILE* file, double* pval,
    int flag);
extern int ut_array_1d_fscanf_filter_prefix_int (FILE* file, double* array,
    int size, int flag);
extern int ut_array_2d_fscanf_filter_prefix_int (FILE *file, double** array,
    int size1, int size2, int flag);
extern int ut_array_0d_char_fscanf_filter_prefix_int (FILE* file, char* pval,
    int flag);

extern void ut_array_3d_switcharrays (double ***array, int size1, int
				      size2, int line1, int line2);
extern void ut_array_3d_int_switcharrays (int ***array, int size1, int
				      size2, int line1, int line2);

extern void ut_array_1d_int_lists_merge (int** listvals, int *listvalqty,
                                        int listqty, int **pvals,
                                        int *pvalqty);

extern int ut_array_1d_int_listpair_merge (int* list1, int qty1, int*
					   list2, int qty2, int** plist,
					   int* pqty);

extern int ut_array_1d_int_listpair_equal (int* list1, int qty1, int*
					   list2, int qty2);

extern int ut_array_1d_int_duplicates (int* array, int qty);

#ifdef HAVE_GSL
extern void ut_array_1d_set_rand (double *, int, double, double, gsl_rng *r);
extern void ut_array_1d_int_set_rand (int *, int, int, int, gsl_rng *r);
extern void ut_array_1d_int_choose (int *src, int srcsize, int *dest, int destsize, gsl_rng *r);
#endif

extern void ut_array_1d_round_keepsum (double *a, int size, double *b);

extern int ut_array_2d_pair (double **a, int sizea1, int sizea2, double **b,
                             int sizeb1, int sizeb2, int *pair, double *pdist);

extern int ut_array_2d_int_pair (int **a, int sizea1, int sizea2, int **b,
                                 int sizeb1, int sizeb2, int *pair);

extern int ut_array_2d_int_list_pair (int **a, int sizea1, int sizea2, int **b,
                                      int sizeb1, int sizeb2, int *pair);

extern int ut_array_2d_int_list_pair_2 (int **a, int sizea1, int *sizea2,
                                        int **b, int sizeb1, int *sizeb2,
                                        int *pair);

#endif /* UT_ARRAY_H */

#ifdef __cplusplus
}
#endif
