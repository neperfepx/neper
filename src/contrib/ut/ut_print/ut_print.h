/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_PRINT_H
#define UT_PRINT_H

extern void ut_print_wnc_int (FILE *, int, int *, int);
extern void ut_print_wnc_int_header (FILE *, int, int *, int, const char*);
extern int  ut_print_wnc_char (FILE *, const char *, int *, int);
extern void ut_print_wncall_char (FILE *, const char *, int *, int);
extern void ut_print_wnc (FILE*, int*, int, const char*, ...);
extern void ut_print_wncall (FILE*, int*, int, const char*, ...);
extern void ut_print_wnc_wncall (FILE*, int*, int, int*, int, const char*, ...);
extern void ut_print_1lwnc (FILE *, const char *, unsigned int *, unsigned int);
extern void ut_print_lineheader (int);
extern void ut_print_message (int, int, const char *, ...);
extern void ut_print_message_inc (int head, int level, int inc, int
    incf, const char *s, ...);
extern void ut_print_messagewnc (int, int, const char *);
extern void ut_print_missingfile (const char *);
extern void ut_print_moduleheader (const char *, int, char **, int, char **);
extern void ut_print_progress (FILE*, long, long, const char*, char*);
extern void ut_print_progress_nonl (FILE*, long, long, const char*, char*);
extern void ut_print_reportbug (void);
extern void ut_print_gplv3 (FILE*);
extern void ut_error_reportbug (void);
extern void ut_error_expression (char *expr);
extern void ut_print_clearline (FILE* file, int nb);

#endif /* UT_PRINT_H */

#ifdef __cplusplus
}
#endif
