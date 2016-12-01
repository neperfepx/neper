/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_STRING_H
#define UT_STRING_H

extern int ut_string_fnr (char *, char, char);
extern char* ut_string_paste (char *, char*);
extern char* ut_string_array_paste (char **, int);
extern char* ut_string_array_paste_cmp (char** string, int qty);
extern char* ut_string_array_paste_separator (char** string, int qty,
    char c);
extern int ut_string_find (const char*, char);
extern int ut_string_finds (const char*, const char*);
extern int ut_string_fnrs (char *, const char*, const char*, int);
extern int ut_string_comp (char *, char **, int, char **);
extern void ut_string_body (char*, char*, char**);
extern char* ut_string_changeextension (char *, char *);
extern char* ut_string_extension (char *);
extern char* ut_string_addextension (const char*, const char*, ...);
extern int ut_string_untilchar (char *, char, int *, char *);
extern int ut_string_untilstring (const char *string, const char* s, int *ppos, char *res);
extern int ut_string_thischarqty (char *, char);
extern int ut_string_thischarpos (char *, char, int);
extern int ut_string_stringpos (char *, char*, int);
extern void ut_string_separate (const char*, const char*, char***, int*);
extern void ut_string_function_separate (char *string, char** pfct,
                             char ***vars, char ***vals, int *pqty);
extern void ut_string_separate2 (const char *, const char*, const char*, char****, int**, int*);
extern int ut_string_inlist (const char*, const char*, const char*);
extern int ut_string_inlist_int (char*, char*, int);
extern void ut_string_partqty (char*, char, int*);
extern int ut_string_part (char*, char, int, char*);
extern int ut_string_nbwords (char*);
extern double ut_string_exp_value (char*, unsigned int, double*, double*);
extern int ut_string_format (const char*, char*);
extern int ut_string_format_strcmp (char*, char*);
extern int ut_string_memcpy (char*, char**);
extern int ut_string_isdigits (char *string);
extern int ut_string_real (char* string, double* pres);
extern int ut_string_int (char* string, int* pres);
extern void ut_string_string (const char* input, char** poutput);

extern int ut_string_filename (char* string);

extern int ut_string_iter_test (char* string, char* sep, int iter);

extern int ut_string_substrings (char *string, char ***psubstrings, int
    *psubstringqty);

extern int ut_string_section_level (char *string, int *plevel);

extern void ut_string_uppercase (char *string);
extern int ut_string_addtolist (char **pstring, char *c, char *);

#endif /* UT_STRING_H */

#ifdef __cplusplus
}
#endif
