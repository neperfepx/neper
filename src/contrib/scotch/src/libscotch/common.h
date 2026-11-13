/* Copyright 2004,2007-2016,2018-2026 IPB, Universite de Bordeaux, INRIA & CNRS
**
** This file is part of the Scotch software package for static mapping,
** graph partitioning and sparse matrix ordering.
**
** This software is governed by the CeCILL-C license under French law
** and abiding by the rules of distribution of free software. You can
** use, modify and/or redistribute the software under the terms of the
** CeCILL-C license as circulated by CEA, CNRS and INRIA at the following
** URL: "http://www.cecill.info".
**
** As a counterpart to the access to the source code and rights to copy,
** modify and redistribute granted by the license, users are provided
** only with a limited warranty and the software's author, the holder of
** the economic rights, and the successive licensors have only limited
** liability.
**
** In this respect, the user's attention is drawn to the risks associated
** with loading, using, modifying and/or developing or reproducing the
** software by the user in light of its specific status of free software,
** that may mean that it is complicated to manipulate, and that also
** therefore means that it is reserved for developers and experienced
** professionals having in-depth computer knowledge. Users are therefore
** encouraged to load and test the software's suitability as regards
** their requirements in conditions enabling the security of their
** systems and/or data to be ensured and, more generally, to use and
** operate it in the same conditions as regards security.
**
** The fact that you are presently reading this means that you have had
** knowledge of the CeCILL-C license and that you accept its terms.
*/
/************************************************************/
/**                                                        **/
/**   NAME       : common.h                                **/
/**                                                        **/
/**   AUTHORS    : Francois PELLEGRINI                     **/
/**                David GOUDIN                            **/
/**                Pascal HENON                            **/
/**                Pierre RAMET                            **/
/**                Cedric CHEVALIER (v5.0)                 **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                Clement BARTHELEMY                      **/
/**                                                        **/
/**   FUNCTION   : Part of a parallel direct block solver. **/
/**                These lines are the common data         **/
/**                declarations for all modules.           **/
/**                                                        **/
/**   DATES      : # Version 0.0  : from : 08 may 1998     **/
/**                                 to   : 08 jan 2001     **/
/**                # Version 1.0  : from : 06 jun 2002     **/
/**                                 to   : 06 jun 2002     **/
/**                # Version 2.0  : from : 13 jun 2005     **/
/**                                 to   : 01 jul 2008     **/
/**                # Version 5.1  : from : 09 nov 2008     **/
/**                                 to   : 23 nov 2010     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 21 aug 2020     **/
/**                # Version 6.1  : from : 02 apr 2021     **/
/**                                 to   : 24 jun 2021     **/
/**                # Version 7.0  : from : 03 jun 2018     **/
/**                                 to   : 11 oct 2026     **/
/**                                                        **/
/************************************************************/

#ifndef COMMON_H
#define COMMON_H

/*
** The includes.
*/

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE               600
#endif /* _XOPEN_SOURCE */

#ifdef COMMON_OS_MACOS
#ifndef _DARWIN_C_SOURCE
#define _DARWIN_C_SOURCE
#endif /* _DARWIN_C_SOURCE */
#define HAVE_SYS_SYSCTL_H
#ifndef COMMON_TIMING_OLD
#define COMMON_TIMING_OLD
#endif /* COMMON_TIMING_OLD */
#endif /* COMMON_OS_MACOS */

#ifdef COMMON_PTHREAD
#ifdef COMMON_PTHREAD_AFFINITY_LINUX
#define _GNU_SOURCE                               /* Must define this before any include */
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */
#endif /* COMMON_PTHREAD */

#ifdef COMMON_OS_WINDOWS
#include            <io.h>                        /* For _pipe ()              */
#include            <fcntl.h>                     /* Fow Windows _pipe () call */
#include            <windows.h>
#define HAVE_STDINT_H
#define HAVE_UINT_T
#define HAVE_NOT_SYS_WAIT_H
#ifdef _MSC_VER
#define HAVE_NOT_STRINGS_H
#if (INT_WIDTH == 64)                             /* On WIN32 / MSVC, sizeof (int) == sizeof (long) in all cases, unlike other platforms */
#define __sync_lock_test_and_set(m,v) _InterlockedExchange64 ((long *) (m), (v))
#define __sync_lock_release(m)      _InterlockedExchange64 ((long *) (m), 0L)
#else /* (INT_WIDTH == 64) */
#define __sync_lock_test_and_set(m,v) _InterlockedExchange ((long *) (m), (v))
#define __sync_lock_release(m)      _InterlockedExchange ((long *) (m), 0L)
#endif /* (INT_WIDTH == 64) */
#endif /* _MSC_VER */
#define ssize_t                     SSIZE_T
#if ((defined _WIN32) && (! defined __MINGW32__))
#define strncasecmp                 strnicmp
#define strcasecmp                  stricmp
#endif /* ((defined _WIN32) && (! defined __MINGW32__)) */
#define pipe(fd)                    _pipe (fd, 32768, O_BINARY)
#define sleep(s)                    Sleep (1000 * (s))
#endif /* COMMON_OS_WINDOWS */

#if ((! defined COMMON_OS_WINDOWS) && (! defined HAVE_NOT_UNISTD_H))
#include            <unistd.h>
#endif /* ((! defined COMMON_OS_WINDOWS) && (! defined HAVE_NOT_UNISTD_H)) */
#include            <ctype.h>
#include            <math.h>
#include            <memory.h>
#include            <stddef.h>                    /* For ptrdiff_t */
#include            <stdio.h>
#include            <stdarg.h>
#include            <stdlib.h>
#if (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_STDINT_H))
#include            <stdint.h>
#include            <inttypes.h>
#endif /* (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_STDINT_H)) */
#ifdef HAVE_MALLOC_H
#include            <malloc.h>                    /* Deprecated, but required on some old systems */
#endif /* HAVE_MALLOC_H */
#include            <string.h>
#ifndef HAVE_NOT_STRINGS_H
#include            <strings.h>
#endif /* HAVE_NOT_STRINGS_H */
#include            <time.h>                      /* For the effective calls to clock () */
#include            <limits.h>
#include            <float.h>
#include            <sys/types.h>
#if ((defined COMMON_TIMING_OLD) || (defined HAVE_SYS_TIME_H))
#include            <sys/time.h>
#endif /* ((defined COMMON_TIMING_OLD) || (defined HAVE_SYS_TIME_H)) */
#if ((defined COMMON_TIMING_OLD) || (defined HAVE_SYS_RESOURCE_H))
#include            <sys/resource.h>
#endif /* ((defined COMMON_TIMING_OLD) || (defined HAVE_SYS_RESOURCE_H)) */
#if (defined HAVE_SYS_SYSCTL_H)
#include            <sys/sysctl.h>
#endif /* (defined HAVE_SYS_SYSCTL_H) */
#ifndef HAVE_NOT_SYS_WAIT_H
#include            <sys/wait.h>                  /* For waitpid () */
#endif /* HAVE_NOT_SYS_WAIT_H */

#ifdef COMMON_MPI
#include            <mpi.h>
#endif /* COMMON_MPI */

#ifdef COMMON_PTHREAD
#ifdef COMMON_THREAD_WIN32
#include            "pthread_win32.h"
#else /* COMMON_THREAD_WIN32 */
#include            <pthread.h>
#endif /* COMMON_THREAD_WIN32 */

#ifdef COMMON_PTHREAD_AFFINITY_LINUX
#include <sched.h>
#endif /* COMMON_PTHREAD_AFFINITY_LINUX */
#endif /* COMMON_PTHREAD */

/*
**  Working definitions.
*/

#if ((defined COMMON_MEMORY_TRACE) || (defined COMMON_MEMORY_CHECK))
#define memAlloc(size)              memAllocRecord ((size) | 8)
#define memRealloc(ptr,size)        memReallocRecord ((ptr), ((size) | 8))
#define memFree(ptr)                memFreeRecord ((void *) (ptr))
#else /* ((defined COMMON_MEMORY_TRACE) || (defined COMMON_MEMORY_CHECK)) */
#define memAlloc(size)              malloc ((size) | 8) /* For platforms which return NULL for malloc(0) */
#define memRealloc(ptr,size)        realloc ((ptr), ((size) | 8))
#define memFree(ptr)                free ((char *) (ptr))
#endif /* ((defined COMMON_MEMORY_TRACE) || (defined COMMON_MEMORY_CHECK)) */

#define memSet(ptr,val,siz)         memset ((void *) (ptr), (val), (siz))
#define memCpy(dst,src,siz)         memcpy ((void *) (dst), (void *) (src), (siz))
#define memMov(dst,src,siz)         memmove ((void *) (dst), (void *) (src), (siz))

#ifndef MIN
#define MIN(x,y)                    (((x) < (y)) ? (x) : (y))
#endif /* MIN */
#ifndef MAX
#define MAX(x,y)                    (((x) < (y)) ? (y) : (x))
#endif /* MAX */
#ifndef ABS
#define ABS(x)                      MAX ((x), -(x))
#endif /* ABS */
#ifndef SIGN
#define SIGN(x)                     (((x) < 0) ? -1 : 1)
#endif /* SIGN */

/*
**  Handling of generic types.
*/

#if (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_UINT_T))
#define UINT32                      uint32_t
#define UINT64                      uint64_t
#else /* (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_UINT_T)) */
#define UINT32                      u_int32_t
#define UINT64                      u_int64_t
#endif /* (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_UINT_T)) */

#ifndef INT                                       /* If type not externally overriden */
#ifdef INTSIZE32
#define INT                         int32_t
#define UINT                        UINT32
#define COMM_INT                    MPI_INT32_T
#ifdef PRId32
#define INTSTRING                   "%" PRId32
#define UINTSTRING                  "%" PRIu32
#else /* PRId32 */
#define INTSTRING                   "%d"
#define UINTSTRING                  "%u"
#endif /* PRId32 */
#else /* INTSIZE32 */
#ifdef INTSIZE64
#define INT                         int64_t
#if (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_UINT_T))
#define UINT                        uint64_t
#else /* (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_UINT_T)) */
#define UINT                        u_int64_t
#endif /* (((defined __STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) || (defined HAVE_UINT_T)) */
#define COMM_INT                    MPI_INT64_T
#ifdef PRId64
#define INTSTRING                   "%" PRId64
#define UINTSTRING                  "%" PRIu64
#else /* PRId64 */
#define INTSTRING                   "%lld"
#define UINTSTRING                  "%llu"
#endif /* PRId64 */
#else /* INTSIZE64 */
#ifdef LONG                                       /* Better not use it */
#define INT                         long          /* Long integer type */
#define UINT                        unsigned long
#define COMM_INT                    MPI_LONG
#define INTSTRING                   "%ld"
#define UINTSTRING                  "%lu"
#else /* LONG */
#define INT                         int           /* Default integer type */
#define UINT                        unsigned int
#define COMM_INT                    MPI_INT       /* Generic MPI integer type */
#define INTSTRING                   "%d"
#define UINTSTRING                  "%u"
#endif /* LONG      */
#endif /* INTSIZE64 */
#endif /* INTSIZE32 */
#endif /* INT       */

#ifndef IDX                                       /* If type not externally overriden */
#ifdef IDXSIZE32
#define IDX                         int32_t
#else /* IDXSIZE32 */
#ifdef IDXSIZE64
#define IDX                         int64_t
#else /* IDXSIZE64 */
#define IDX                         INT
#endif /* IDXSIZE64 */
#endif /* IDXSIZE32 */
#endif /* IDX       */

#ifndef INTSIZEBITS
#define INTSIZEBITS                 (sizeof (INT) << 3)
#endif /* INTSIZEBITS */

#define INTVALMAX                   ((INT) (((UINT) 1 << (INTSIZEBITS - 1)) - 1))

#ifndef byte
#define byte unsigned char                        /* Byte type */
#endif /* byte */
#ifndef BYTE
#define BYTE                        byte
#endif /* BYTE */
#ifndef COMM_BYTE
#define COMM_BYTE                   MPI_BYTE
#endif /* COMM_BYTE */
#define COMM_PART                   COMM_BYTE

/*
**  Handling of pseudo-random numbers.
*/

/* The pseudo-random state structure. It is
   based on the xorshift128+ algorithm by Vigna,
   both for 32- and 64-bit integers.             */

typedef struct IntRandState_ {
  UINT64                    randtab[2];           /*+ State vector +*/
} IntRandState;

/** The pseudo-random context. **/

typedef struct IntRandContext_ {
  volatile int              flagval;              /*+ Initialized flag +*/
  int                       procval;              /*+ Process number   +*/
  UINT64                    seedval;              /*+ Seed value       +*/
  IntRandState              statdat;              /*+ State data       +*/
} IntRandContext;

/** The global pseudo-random context. **/

extern IntRandContext       intranddat;           /*+ Global random context +*/

/*
**  Handling of flag arrays.
*/

#define flagSize(n)                 (((n) + (sizeof (int) << 3) - 1) / (sizeof (int) << 3))
#define flagVal(a,n)                (((a)[(n) / (sizeof (int) << 3)] >> ((n) & ((sizeof (int) << 3) - 1))) & 1)
#define flagSet(a,n)                (a)[(n) / (sizeof (int) << 3)] |= (1 << ((n) & ((sizeof (int) << 3) - 1)))

/*
**  Handling of timers.
*/

/** The clock type. **/

typedef struct Clock_ {
  double                    time[2];              /*+ The start and accumulated times +*/
} Clock;

/*
**  Handling of threads.
*/

/** The abstract thread context. **/

struct ThreadContext_;
typedef struct ThreadContext_ ThreadContext;

/** The thread descriptor. **/

typedef struct ThreadDescriptor_ {
  ThreadContext *           contptr;              /*+ Pointer to thread context +*/
  int                       thrdnum;              /*+ Thread instance number    +*/
} ThreadDescriptor;

/** The thread service routines auxiliary function types. **/

typedef void (* ThreadFunc) (ThreadDescriptor * const, void * const);
typedef void (* ThreadReduceFunc) (void * const, void * const, const void * const);
typedef void (* ThreadScanFunc) (void * const, void * const, const int, const int, const void * const);

/*
**  Handling of values.
*/

/*+ The abstract context values datatype. +*/

struct ValuesContext_;
typedef struct ValuesContext_ ValuesContext;

/*
**  Handling of execution contexts.
*/

/** The execution context. **/

typedef struct Context_ {
  ThreadContext *           thrdptr;              /*+ Threading context +*/
  IntRandContext *          randptr;              /*+ Random context    +*/
  ValuesContext *           valuptr;              /*+ Values context    +*/
} Context;

/*+ The context splitting user function. +*/

typedef void (* ContextSplitFunc) (Context * const, const int, void * const);

/*+ The data structure for passing arguments to the context splitting threaded routine. +*/

typedef struct ContextSplit_ {
  Context                   conttab[2];           /*+ Context data for sub-context                     +*/
  ContextSplitFunc          funcptr;              /*+ Pointer to user function to be called by leaders +*/
  void *                    paraptr;              /*+ Parameter data                                   +*/
} ContextSplit;

/*
**  Handling of files.
*/

/** The file flags **/

#define FILEMODE                    0x0001
#define FILEMODER                   0x0000
#define FILEMODEW                   0x0001
#define FILEFREENAME                0x0002

/** The file structure. **/

typedef struct File_ {
  int                       flagval;              /*+ File mode            +*/
  char *                    nameptr;              /*+ File name            +*/
  FILE *                    fileptr;              /*+ File pointer         +*/
  struct FileCompress_ *    compptr;              /*+ (De)compression data +*/
} File;

/*
**  Function prototypes.
*/

int                         envGetInt           (const char * const, const int);
const char *                envGetStr           (const char * const, const char *);

void *                      memAllocGroup       (void **, ...);
void *                      memReallocGroup     (void *, ...);
void *                      memOffset           (void *, ...);
#if ((defined COMMON_MEMORY_TRACE) || (defined COMMON_MEMORY_CHECK))
void *                      memAllocRecord      (size_t);
void *                      memReallocRecord    (void * const, size_t);
void                        memFreeRecord       (void * const);
#endif /* ((defined COMMON_MEMORY_TRACE) || (defined COMMON_MEMORY_CHECK)) */
IDX                         memCur              (void); /* What is internally an intptr_t has to be turned into an interface type */
IDX                         memMax              (void);

void                        usagePrint          (FILE * const, const char (* []));

void                        fileBlockInit       (File * const, const int);
int                         fileBlockOpen       (File * const, const int);
int                         fileBlockOpenDist   (File * const, const int, const int, const int, const int);
void                        fileBlockClose      (File * const, const int);
char *                      fileNameDistExpand  (char * const, const int, const int);

void                        errorProg           (const char * const);
void                        errorPrint          (const char * const, ...);
void                        errorPrintW         (const char * const, ...);

int                         intLoad             (FILE * const, INT * const);
int                         intSave             (FILE * const, const INT);
void                        intAscn             (INT * const, const INT, const INT);
void                        intPerm             (INT * const, const INT, Context * const);
void                        intRandInit         (IntRandContext * const);
int                         intRandLoad         (IntRandContext * const, FILE * const);
void                        intRandProc         (IntRandContext * const, const int);
void                        intRandReset        (IntRandContext * const);
int                         intRandSave         (IntRandContext * const, FILE * const);
void                        intRandSeed         (IntRandContext * const, UINT64);
UINT                        intRandVal          (IntRandContext * const, UINT);
UINT64                      intRandVal2         (IntRandContext * const);
void                        intRandSpawn        (IntRandContext * const, const int, IntRandContext * const);
void                        intSort1asc1        (void * const, const INT);
void                        intSort2asc1        (void * const, const INT);
void                        intSort2asc2        (void * const, const INT);
void                        intSort3asc1        (void * const, const INT);
void                        intSort3asc2        (void * const, const INT);
void                        intPsort2asc1       (void * const, const INT, const int);
INT                         intSearchDicho      (const INT * const, const INT, const INT, const INT);
INT                         intGcd              (INT, INT);

void                        clockInit           (Clock * const);
void                        clockStart          (Clock * const);
void                        clockStop           (Clock * const);
double                      clockVal            (Clock * const);
double                      clockGet            (void);

void                        stringSubst         (char * const, const char * const, const char * const);

int                         threadContextInit   (ThreadContext * const, int, const int * const);
void                        threadContextExit   (ThreadContext * const);
void                        threadContextExit2  (ThreadContext * const);
int                         threadContextBarrier (ThreadContext * const);
void                        threadContextImport1 (ThreadContext * const, const int);
void                        threadContextImport2 (ThreadContext * const, const int);
int                         threadContextNbr    (ThreadContext * const);
void                        threadLaunch        (ThreadContext * const, ThreadFunc const, void * const);
void                        threadReduce        (const ThreadDescriptor * const, void * const, const size_t, ThreadReduceFunc const, const int, const void * const);
void                        threadScan          (const ThreadDescriptor * const, void * const, const size_t, ThreadScanFunc const, const void * const);

void                        contextInit         (Context * const);
void                        contextExit         (Context * const);
int                         contextCommit       (Context * const);
int                         contextRandomClone  (Context * const);
int                         contextThreadInit2  (Context * const, const int, const int * const);
int                         contextThreadInit   (Context * const);
int                         contextThreadLaunchSplit (Context * const, ContextSplitFunc const, void * const);
int                         contextValuesInit   (Context * const, void * const, const size_t, const int, const size_t, const int, const size_t);
int                         contextValuesGetDbl (Context * const, const int, double * const);
int                         contextValuesGetInt (Context * const, const int, INT * const);
int                         contextValuesSetDbl (Context * const, const int, const double);
int                         contextValuesSetInt (Context * const, const int, const INT);

/*
**  Macro definitions.
*/

#define clockInit(clk)              ((clk)->time[0]  = (clk)->time[1] = 0)
#define clockStart(clk)             ((clk)->time[0]  = clockGet ())
#define clockStop(clk)              ((clk)->time[1] += (clockGet () - (clk)->time[0]))
#define clockVal(clk)               ((clk)->time[1])

#define fileBlockFile(b,i)          ((b)[i].fileptr)
#define fileBlockMode(b,i)          ((b)[i].modeptr)
#define fileBlockName(b,i)          ((b)[i].nameptr)

#define threadBarrier(t)            threadContextBarrier ((t)->contptr)
#define threadNbr(t)                threadContextNbr ((t)->contptr)
#define threadNum(t)                ((t)->thrdnum)

#define contextRandom(c)            ((c)->randptr)
#define contextIntRandVal(c,n)      intRandVal ((c)->randptr, (n))
#define contextIntRandVal2(c)       intRandVal2 ((c)->randptr)

#define contextThreadLaunch(c,f,d)  threadLaunch ((c)->thrdptr, (f), (d))
#define contextThreadNbr(c)         threadContextNbr ((c)->thrdptr)

#define DATASIZE(n,p,i)             ((INT) (((n) + ((p) - 1 - (i))) / (p)))
#define DATASCAN(n,p,i)             ((i) * ((INT) (n) / (INT) (p)) + (((i) > ((n) % (p))) ? ((n) % (p)) : (i)))

#define FORTRAN(nu,nl,pl,pc)        FORTRAN2(REPLACE(nu),REPLACE(nl),pl,pc)
#define FORTRAN2(nu,nl,pl,pc)                    \
void nu pl;                                      \
void nl pl                                       \
{ nu pc; }                                       \
void GLUE(nl,_) pl                               \
{ nu pc; }                                       \
void GLUE(nl,__) pl                              \
{ nu pc; }                                       \
void nu pl

#define REPLACE(s)                  s
#define GLUE(p,s)                   p##s

#define STRINGIFY2(n)               #n
#define STRINGIFY(n)                STRINGIFY2(n)

#endif /* COMMON_H */
