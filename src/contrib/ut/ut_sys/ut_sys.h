/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#ifdef __cplusplus
extern "C" {
#endif

#include<sys/time.h>

extern int ut_sys_runwtime (char*, char*, double, struct timeval*);
extern int ut_sys_mkdirp (const char *dir, ...);
extern int ut_sys_isdir (const char *pathname, ...);
extern char* ut_sys_realpath(const char *pathname);

extern int ut_sys_file_cp_untilchar (FILE *src, FILE *dest, char a);
extern int ut_sys_file_cp_line (FILE *src, FILE *dest);
extern int ut_sys_file_cp_lines (FILE *src, FILE *dest, int *lines, int lineqty);
extern int ut_sys_currentbin_path (char **ppath);
extern int ut_sys_endian (void);

#ifdef __cplusplus
}
#endif
