/* Copyright (C) 2003-2018, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_STAT_H
#define UT_STAT_H

extern int ut_stat_test_ks (double*, double*, int, double*);
extern int ut_stat_test_kuiper (double* cd1, double* cd2, int qty,
    double* pres);

#endif /* UT_STAT_H */

#ifdef __cplusplus
}
#endif
