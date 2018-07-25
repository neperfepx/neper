/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef UT_FILE_H
#define UT_FILE_H

extern int ut_file_skip (FILE*, int);
extern int ut_file_nblines (const char*);
extern int ut_file_nblines_pointer (FILE *);
extern int ut_file_nbwords (const char*);
extern int ut_file_nbcolumns (const char*);
extern int ut_file_nbwords_test (const char*, int);
extern int ut_file_nbwords_testwmessage (const char*, int);
extern int ut_file_exist (const char*, ...);
extern FILE* ut_file_open (const char*, const char*);
extern void ut_file_openmessage (const char*, const char*);
extern void ut_file_close (FILE *, const char*, const char*);
extern void ut_file_closemessage (const char*, const char*);
extern void ut_file_close_nonl (FILE *, const char*, const char*);
extern void ut_file_closemessage_nonl (const char*, const char*);
extern int ut_file_skip_char (FILE *, int);
extern int ut_file_skip_line (FILE *, int);
extern int ut_file_tofile (FILE *, int, FILE *, const char*, const char*);
extern int ut_file_readwcomma (FILE*, double*);
extern int ut_file_format (const char*, char**);
extern int ut_file_testformat (const char*, const char*);
extern int ut_file_nbwords_pointer (FILE *);

extern int ut_file_string_goto (FILE*, const char*);
extern int ut_file_string_untilchar (FILE*, char*, char);
extern int ut_file_string_scanncomp (FILE*, const char*);
extern int ut_file_string_nextpos (FILE*, const char*);
extern int ut_file_string_number (FILE*, const char*, const char*);

extern int ut_file_thisstring (FILE*, char*);
extern int ut_file_thischar (FILE*, char*);
extern int ut_file_thisint  (FILE*, int*);
extern int ut_file_thisreal (FILE*, double*);
extern int ut_file_nextint  (FILE*, int*);
extern int ut_file_nextchar  (FILE*, char*);
extern int ut_file_nextstring  (FILE*, char*);
extern int ut_file_nextstring_section_level (FILE * file, int *plevel);
extern int ut_file_nextline (FILE *file, char** pline);
extern int ut_file_nextline_nbwords (FILE * file, int* pqty);

extern int ut_file_testpos (FILE*, const char*);
extern int ut_file_go (FILE*, const char*);

extern int ut_file_line_nbwords_pointer (FILE*, int*);

extern int ut_file_cp_line (FILE *src, FILE *dest);
extern int ut_file_cp_untilchar (FILE *src, FILE *dest, char c);
extern int ut_file_cp_lines (FILE *src, FILE *dest, int *lines, int lineqty);
extern int ut_file_cp (const char* frompath, const char* topath);

extern int ut_file_filename (const char *name, char **pname);

extern int ut_file_goto_lineheader (FILE *fp, const char* header);


#endif /* UT_FILE_H */

#ifdef __cplusplus
}
#endif
