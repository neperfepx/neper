/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_NUM_H
#define UT_NUM_H

extern int ut_num_min_int (int, int);
extern int ut_num_max_int (int, int);
extern int ut_num_bound_int (int, int, int);
extern double ut_num_min (double, double);
extern double ut_num_max (double, double);
extern double ut_num_bound (double, double, double);

extern double ut_num_atan2 (double, double);
extern int ut_num_d2ri (double);

extern int ut_num_tenlen (double);
extern int ut_num_tenlen_int (int);
extern double ut_num_acos (double);
extern double ut_num_sqrt (double);
extern double ut_num_sgn (double);
extern int ut_num_sgn_int (int);
extern int ut_num_fabs_int (int);

extern int ut_num_rotpos (int, int, int, int);

extern int ut_num_equal_int (int, int);
extern int ut_num_equal (double, double, double);
extern int ut_num_requal (double, double, double);
extern int ut_num_equal_double (double, double, double);
extern int ut_num_requal (double, double, double);

extern int ut_num_lcomden (int, int);

extern double ut_num_readwcomma (char *);

extern int ut_num_even (int);
extern int ut_num_odd (int);
extern double ut_num_log (double);

extern void ut_num_switch (double*, double*);
extern void ut_num_switch_int (int*, int*);

extern double ut_num_percent (double a, double b);

extern double ut_num_reverseendian (double a);

#endif /* UT_NUM_H */

#ifdef __cplusplus
}
#endif
