/* Copyright 2007,2008,2010,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_file.c                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles files and file      **/
/**                names.                                  **/
/**                                                        **/
/**   DATES      : # Version 5.0  : from : 21 may 2007     **/
/**                                 to   : 16 mar 2008     **/
/**                # Version 5.1  : from : 27 jun 2010     **/
/**                                 to   : 27 jun 2010     **/
/**                # Version 6.0  : from : 10 nov 2014     **/
/**                                 to   : 14 jul 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define COMMON_FILE

#ifndef COMMON_NOMODULE
#include "module.h"
#endif /* COMMON_NOMODULE */
#include "common.h"
#include "common_file.h"

/*********************************/
/*                               */
/* Basic routines for filenames. */
/*                               */
/*********************************/

/* This routine expands distributed filenames
** according to process numbers.
** It returns:
** - NULL     : on error.
** - nameptr  : no expansion took place.
** - !NULL    : expanded string.
*/

char *
fileNameDistExpand (
char * const                nameptr,              /*+ Pointer to name string pointer +*/
const int                   procnbr,              /*+ Number of processes            +*/
const int                   procnum)              /*+ Number of current process      +*/
{
  int                 namemax;
  int                 namenum;
  char *              naexptr;
  int                 naexmax;
  int                 naexnum;
  int                 flagval;                    /* Flag set if expansion took place */

  namemax = strlen (nameptr);
  naexmax = namemax + FILENAMEDISTEXPANDNBR * 2;

  if ((naexptr = memAlloc ((naexmax + 1) * sizeof (char))) == NULL) /* "+ 1" for terminating character */
    return (NULL);

#ifdef COMMON_DEBUG
  sprintf (naexptr, FILENAMEDISTEXPANDSTR, procnbr); /* TRICK: Test if FILENAMEDISTEXPANDNBR is a size large enough */
  if (atoi (naexptr) != procnbr) {
    errorPrint ("fileNameDistExpand: undersized integer string size");
    memFree    (naexptr);
    return     (NULL);
  }
#endif /* COMMON_DEBUG */

  for (namenum = naexnum = flagval = 0; namenum < namemax; ) {
    char                charval;
    int                 dataval = 0;
    int                 datasiz;

    charval = nameptr[namenum ++];                /* Get current character                */
    datasiz = 1;                                  /* Assume individual expanded character */
    if (charval == '%') {
      char                chnxval;                /* Value of next char */

      switch (chnxval = nameptr[namenum ++]) {
        case 'p' :                                /* "%p" translates into number of processes */
          flagval = 1;
          datasiz = FILENAMEDISTEXPANDNBR;
          dataval = procnbr;
          break;
        case 'r' :                                /* "%r" translates into process rank */
          flagval = 1;
          datasiz = FILENAMEDISTEXPANDNBR;
          dataval = procnum;
          break;
        case '-' :                                /* "%-" translates into nothing but indicates distributed file */
          datasiz = 0;
          flagval = 1;
          break;
        case '%' :                                /* "%%" translates into '%' */
          break;
        default :
          charval = chnxval;                      /* Unrecognized character */
      }
    }
    if (datasiz > 0) {
      if ((naexnum + datasiz) > naexmax) {
        char *              nanwptr;

        naexmax += datasiz + FILENAMEDISTEXPANDNBR;
        if ((nanwptr = memRealloc (naexptr, (naexmax + 1) * sizeof (char))) == NULL) { /* "+ 1" for terminating character */
          memFree (naexptr);
          return  (NULL);
        }
        naexptr = nanwptr;
      }
      if (datasiz == 1)
        naexptr[naexnum ++] = charval;
      else {
        sprintf (&naexptr[naexnum], FILENAMEDISTEXPANDSTR, dataval); /* TRICK: Change format string if FILENAMEDISTEXPANDNBR changes */
        naexptr[naexnum + FILENAMEDISTEXPANDNBR] = ' ';
        naexnum = strchr (&naexptr[naexnum], ' ') - naexptr;
      }
    }
  }
  naexptr[naexnum] = '\0';                        /* Set terminating character as there is always room for it */

  if (flagval == 0) {                             /* If no expansion took place      */
    memFree (naexptr);                            /* No need for the expanded string */
    naexptr = nameptr;
  }
  return (naexptr);
}

/* This routine initializes a block of
** file descriptor structures.
** It returns:
** - void  : in all cases.
*/

void
fileBlockInit (
File * const                filetab,
const int                   filenbr)
{
  int                 i;

  for (i = 0; i < filenbr; i ++) {                /* For all file names     */
    filetab[i].nameptr = "-";                     /* Assume standard stream */
    filetab[i].fileptr = ((filetab[i].flagval & FILEMODE) == FILEMODER) ? stdin : stdout;
    filetab[i].compptr = NULL;                    /* No (de)compression yet */
  }
}

/* This routine opens a block of file
** descriptor structures.
** It returns:
** - 0  : on success.
** - 1  : if could not open a stream.
** - 2  : if (de)compression method not implemented.
*/

int
fileBlockOpen (
File * const                filetab,
const int                   filenbr)
{
  int                 i, j;

  for (i = 0; i < filenbr; i ++) {                /* For all file names             */
    if (filetab[i].fileptr == NULL)               /* If unwanted stream, do nothing */
      continue;

    for (j = 0; j < i; j ++) {
      if ((((filetab[i].flagval ^ filetab[j].flagval) & FILEMODE) == 0) && /* If very same name with same opening mode */
          (filetab[j].nameptr != NULL)                                  &&
          (strcmp (filetab[i].nameptr, filetab[j].nameptr) == 0)) {
        filetab[i].fileptr = filetab[j].fileptr;  /* Share pointer to already processed stream */
        filetab[i].nameptr = NULL;                /* Do not close this stream multiple times   */
        break;
      }
    }
    if (j == i) {                                 /* If original stream */
      int                 compval;                /* Compression type   */

      if (filetab[i].nameptr[0] != '-') {         /* If not standard stream, open it */
        if ((filetab[i].fileptr = fopen (filetab[i].nameptr, ((filetab[i].flagval & FILEMODE) == FILEMODER) ? "r" : "w")) == NULL) { /* Open the file */
          errorPrint ("fileBlockOpen: cannot open file (%d)", i);
          return     (1);
        }
      }
      compval = (((filetab[i].flagval & FILEMODE) == FILEMODER) ? fileDecompressType : fileCompressType) (filetab[i].nameptr);
      if (compval < 0) {
        errorPrint ("fileBlockOpen: (de)compression method not implemented");
        return     (2);
      }
      if ((((filetab[i].flagval & FILEMODE) == FILEMODER) ? fileDecompress : fileCompress) (&filetab[i], compval) != 0) {
        errorPrint ("fileBlockOpen: cannot create (de)compression subprocess");
        return     (1);
      }
    }
  }

  return (0);
}

/* This routine opens a block of eventually
** distributed file descriptor structures.
** It returns:
** - 0  : on success.
** - !0 : on error.
*/

int
fileBlockOpenDist (
File * const                filetab,
const int                   filenbr,
const int                   procglbnbr,
const int                   proclocnum,
const int                   protglbnum)
{
  int                 i;

  for (i = 0; i < filenbr; i ++) {                /* For all file names */
    char *              naexptr;

    if (filetab[i].fileptr == NULL)               /* If unwanted stream, do nothing */
      continue;

    if ((naexptr = fileNameDistExpand (filetab[i].nameptr, procglbnbr, proclocnum)) == NULL) {
      errorPrint ("fileBlockOpenDist: cannot create file name (%d)", i);
      return     (1);
    }
    if (naexptr == filetab[i].nameptr) {          /* If centralized stream */
      if (proclocnum != protglbnum) {             /* If not root process   */
        filetab[i].nameptr = NULL;                /* Remove stream         */
        filetab[i].fileptr = NULL;
        continue;
      }
    }
    else {                                        /* Else keep expanded distributed file name */
      filetab[i].flagval |= FILEFREENAME;
      filetab[i].nameptr  = naexptr;
    }
  }

  return (fileBlockOpen (filetab, filenbr));      /* Open remaining files */
}

/* This routine opens a block of file
** descriptor structures.
** It returns:
** - 0  : on success.
** - !0 : on error.
*/

void
fileBlockClose (
File * const                filetab,
const int                   filenbr)
{
  int                i;

  for (i = 0; i < filenbr; i ++) {                /* For all file names   */
    if ((filetab[i].fileptr != NULL) &&           /* If stream exists     */
        (filetab[i].nameptr != NULL) &&           /* And it has a name    */
        (filetab[i].nameptr[0] != '-')) {         /* Which is not default */
      fclose (filetab[i].fileptr);                /* Close the stream     */
      if (filetab[i].flagval & FILEFREENAME)
        memFree (filetab[i].nameptr);
    }

    fileCompressExit (&filetab[i]);               /* After stream closed, if there is (de)compression data to free */
  }
}
