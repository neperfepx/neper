/* Copyright 2004,2007-2010,2012,2014,2018,2019 Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : dummysizes.c                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : Part of the libScotch compilation job.  **/
/**                This small program processes files that **/
/**                are in fact pattern header files for    **/
/**                the libScotch library, and replaces     **/
/**                symbolic sizes of the opaque libScotch  **/
/**                by the proper integer values according  **/
/**                to the machine on which it is run.      **/
/**                                                        **/
/**   DATES      : # Version 3.4  : from : 22 oct 2001     **/
/**                                 to   : 22 nov 2001     **/
/**                # Version 4.0  : from : 25 nov 2001     **/
/**                                 to   : 06 jan 2006     **/
/**                # Version 5.0  : from : 26 apr 2006     **/
/**                                 to   : 03 apr 2008     **/
/**                # Version 5.1  : from : 16 jun 2008     **/
/**                                 to   : 15 aug 2010     **/
/**                # Version 6.0  : from : 01 dec 2012     **/
/**                                 to   : 26 oct 2019     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define DUMMYSIZES

#define CHARMAX                     2048          /* Maximum line size */

#define SUBSMAX                     256           /* Maximum number of substitutions */

#define C_FILENBR                   2             /* Number of files in list                */
#define C_FILEARGNBR                2             /* Number of files which can be arguments */

#define C_filenamehedinp            C_fileTab[0].nameptr /* Source graph input file name */
#define C_filenamehedout            C_fileTab[1].nameptr /* Statistics output file name  */

#define C_filepntrhedinp            C_fileTab[0].fileptr /* Source graph input file */
#define C_filepntrhedout            C_fileTab[1].fileptr /* Statistics output file  */

#define EXPAND(s)                   EXPANDTWO(s)
#define EXPANDTWO(s)                #s

#define subsFill(a,b)               {                            \
                                      substab[subsnbr][0] = (a); \
                                      substab[subsnbr][1] = (b); \
                                      subsnbr ++;                \
                                    }
#define subsSuffix(a)               subsFill (a, subsSuffix2 (a, suffptr))

#ifdef SCOTCH_NAME_SUFFIX
#include <regex.h>
#endif /* SCOTCH_NAME_SUFFIX */
#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "geom.h"
#include "mesh.h"
#include "arch.h"
#include "mapping.h"
#include "order.h"
#ifdef SCOTCH_PTSCOTCH
#include "dgraph.h"
#include "dgraph_halo.h"
#include "dmapping.h"
#include "dorder.h"
#include "library_dmapping.h"
#endif /* SCOTCH_PTSCOTCH */
#include "library_mapping.h"
#include "library_order.h"

/*
**  The static definitions.
*/

static int                  C_fileNum = 0;        /* Number of file in arg list */
static File                 C_fileTab[C_FILENBR] = { /* The file array          */
                              { FILEMODER },
                              { FILEMODEW } };

/******************************/
/*                            */
/* This is the main function. */
/*                            */
/******************************/

char *
subsSize (
int                         subsval)
{
  char *              subsptr;

  subsptr = malloc (16 * sizeof (char));
  sprintf (subsptr, "%d", (int) ((subsval + sizeof (double) - 1) / sizeof (double)));

  return (subsptr);
}

char *
subsSuffix2 (
char *                      nameptr,
char *                      suffptr)
{
  char *              subsptr;

  subsptr = malloc (128 * sizeof (char));
  sprintf (subsptr, "%s%s", nameptr, suffptr);

  return (subsptr);
}

/******************************/
/*                            */
/* This is the main function. */
/*                            */
/******************************/

int
main (
int                         argc,
char *                      argv[])
{
#ifdef SCOTCH_NAME_SUFFIX
  regex_t             regedat;                    /* Regular expression for function names */
  regmatch_t          matcdat;                    /* Matching structure                    */
#endif /* SCOTCH_NAME_SUFFIX */
  char                chartab[CHARMAX];
  char                chartmp[CHARMAX];
  char *              substab[SUBSMAX][2];        /* Substitution array                    */
  char *              suffptr;                    /* Pointer to suffix array               */
  int                 subsnbr;
  int                 i;

  if ((argc >= 2) && (argv[1][0] == '?')) {       /* If need for help */
    printf ("Usage is:\ndummysizes [<input pattern header file> [<output header file>]]\n");
    return (((argv[1][0] == '?') && argv[1][1] == '\0') ? 0 : 1);
  }

  suffptr = "";                                   /* No suffix */

  for (i = 0; i < C_FILENBR; i ++)                /* Set default stream pointers */
    C_fileTab[i].fileptr = ((C_fileTab[i].flagval & FILEMODE) == FILEMODER) ? stdin : stdout;
  for (i = 1; i < argc; i ++) {                   /* Loop for all option codes */
    if ((argv[i][0] != '+') &&                    /* If found a file name      */
        ((argv[i][0] != '-') || (argv[i][1] == '\0'))) {
      if (C_fileNum < C_FILEARGNBR)               /* A file name has been given */
        C_fileTab[C_fileNum ++].nameptr = argv[i];
      else {
        fprintf (stderr, "dummysizes: ERROR: main: too many file names given");
        exit    (1);
      }
    }
    else {                                        /* If found an option name */
      switch (argv[i][1]) {
        case 'H' :                                /* Give the usage message */
        case 'h' :
          printf ("Usage is:\ndummysizes [<input pattern header file> [<output header file>]]\n");
          exit       (0);
        case 'S' :
        case 's' :
          suffptr = &argv[i][2];
          break;
        case 'V' :
          fprintf (stderr, "dummysizes, version " SCOTCH_VERSION_STRING "\n");
          fprintf (stderr, "Copyright 2004,2007-2010,2018 IPB, Universite de Bordeaux, INRIA & CNRS, France\n");
          fprintf (stderr, "This software is libre/free software under CeCILL-C -- see the user's manual for more information\n");
          return  (0);
        default :
          fprintf (stderr, "dummysizes: ERROR: main: unprocessed option (\"%s\")", argv[i]);
          exit    (1);
      }
    }
  }

  for (i = 0; i < C_FILENBR; i ++) {              /* For all file names     */
    if ((C_fileTab[i].nameptr[0] != '-') ||       /* If not standard stream */
        (C_fileTab[i].nameptr[1] != '\0')) {
      if ((C_fileTab[i].fileptr = fopen (C_fileTab[i].nameptr, ((C_fileTab[i].flagval & FILEMODE) == FILEMODER) ? "r" : "w")) == NULL) { /* Open the file */
          fprintf (stderr, "dummysizes: ERROR: main: cannot open file (%d)", i);
          exit    (1);
      }
    }
  }

  subsnbr = 0;
#ifdef SCOTCH_PTSCOTCH
  subsFill ("library_pt.h", "ptscotch.h  ");
  subsFill ("library_pt_f.h", "ptscotchf.h   ");
  subsSuffix ("PTSCOTCH_H");
#else /* SCOTCH_PTSCOTCH */
  subsFill ("library.h", "scotch.h ");
  subsFill ("library_f.h", "scotchf.h  ");
  subsSuffix ("SCOTCH_H");
#endif /* SCOTCH_PTSCOTCH */
  subsFill ("DUMMYIDX", EXPAND (IDX));
  subsFill ("DUMMYINT", EXPAND (INT));
  subsFill ("DUMMYMAXINT", EXPAND (INTVALMAX));
  subsFill ("DUMMYNUMSTRING", "\"" GNUMSTRING "\"");
  subsFill ("DUMMYVERSION", EXPAND (SCOTCH_VERSION_NUM));
  subsFill ("DUMMYRELEASE", EXPAND (SCOTCH_RELEASE_NUM));
  subsFill ("DUMMYPATCHLEVEL", EXPAND (SCOTCH_PATCHLEVEL_NUM));
  subsFill ("DUMMYSIZEARCHDOM", subsSize (sizeof (ArchDom)));
  subsFill ("DUMMYSIZEARCH", subsSize (sizeof (Arch)));
  subsFill ("DUMMYSIZEGEOM", subsSize (sizeof (Geom)));
  subsFill ("DUMMYSIZEGRAPH", subsSize (sizeof (Graph)));
  subsFill ("DUMMYSIZEMESH",  subsSize (sizeof (Mesh)));
  subsFill ("DUMMYSIZEMAP", subsSize (sizeof (LibMapping)));
  subsFill ("DUMMYSIZEORDER", subsSize (sizeof (LibOrder)));
  subsFill ("DUMMYSIZESTRAT", subsSize (sizeof (Strat *)));
#ifdef SCOTCH_PTSCOTCH
  subsFill ("DUMMYSIZEDGRAPHHALOREQ", subsSize (sizeof (DgraphHaloRequest))); /* TRICK: before DUMMYSIZEDGRAPH */
  subsFill ("DUMMYSIZEDGRAPH", subsSize (sizeof (Dgraph)));
  subsFill ("DUMMYSIZEDMAP", subsSize (sizeof (LibDmapping)));
  subsFill ("DUMMYSIZEDORDER", subsSize (sizeof (Dorder)));
#else /* SCOTCH_PTSCOTCH */
  subsFill ("DUMMYSIZEDGRAPHHALOREQ", "1");       /* TRICK: before DUMMYSIZEDGRAPH */
  subsFill ("DUMMYSIZEDGRAPH", "1");
  subsFill ("DUMMYSIZEDMAP", "1");
  subsFill ("DUMMYSIZEDORDER", "1");
#endif /* SCOTCH_PTSCOTCH */
  subsFill ("##SCOTCH_NAME_SUFFIX", suffptr);
  if (suffptr[0] != '\0') {                       /* If suffix provided */
#ifdef SCOTCH_RENAME_ALL
    subsSuffix ("SCOTCH_VERSION");
    subsSuffix ("SCOTCH_RELEASE");
    subsSuffix ("SCOTCH_PATCHLEVEL");
    subsSuffix ("SCOTCH_COARSENNONE");
    subsSuffix ("SCOTCH_COARSENFOLD");
    subsSuffix ("SCOTCH_COARSENFOLDDUP");
    subsSuffix ("SCOTCH_COARSENNOMERGE");
    subsSuffix ("SCOTCH_STRATDEFAULT");
    subsSuffix ("SCOTCH_STRATQUALITY");
    subsSuffix ("SCOTCH_STRATSPEED");
    subsSuffix ("SCOTCH_STRATBALANCE");
    subsSuffix ("SCOTCH_STRATSAFETY");
    subsSuffix ("SCOTCH_STRATSCALABILITY");
    subsSuffix ("SCOTCH_STRATRECURSIVE");
    subsSuffix ("SCOTCH_STRATREMAP");
    subsSuffix ("SCOTCH_STRATLEVELMAX");
    subsSuffix ("SCOTCH_STRATLEVELMIN");
    subsSuffix ("SCOTCH_STRATLEAFSIMPLE");
    subsSuffix ("SCOTCH_STRATSEPASIMPLE");
#endif /* SCOTCH_RENAME_ALL */
#ifdef SCOTCH_PTSCOTCH
#else /* SCOTCH_PTSCOTCH */
    subsSuffix ("SCOTCH_Idx");
    subsSuffix ("SCOTCH_Num");
    subsSuffix ("SCOTCH_NUMMAX");
    subsSuffix ("SCOTCH_NUMSTRING");
    subsSuffix ("SCOTCH_Arch");
    subsSuffix ("SCOTCH_Geom");
    subsSuffix ("SCOTCH_Graph");
    subsSuffix ("SCOTCH_Mesh");
    subsSuffix ("SCOTCH_Mapping");
    subsSuffix ("SCOTCH_Ordering");
    subsSuffix ("SCOTCH_Strat");
#endif /* SCOTCH_PTSCOTCH */
#ifdef SCOTCH_NAME_SUFFIX
    if (regcomp (&regedat, " SCOTCH_[a-z][0-9a-zA-Z_]*", 0) != 0) {
      fprintf (stderr, "dummysizes: ERROR: cannot compile regular expression\n");
      exit    (1);
    }
#endif /* SCOTCH_NAME_SUFFIX */
  }

  while (fgets (chartab, CHARMAX, C_filepntrhedinp) != NULL) { /* Loop on file lines */
    int                 charnbr;
    int                 subsnum;

    if (((charnbr = strlen (chartab)) >= (CHARMAX - 1)) && /* If line read is at least as long as maximum size     */
        (chartab[CHARMAX - 1] != '\n')) {         /* And last character is not a newline, that is, some is missing */
      fprintf (stderr, "dummysizes: ERROR: line too long\n");
      exit    (1);
    }

    for (subsnum = 0; subsnum < subsnbr; subsnum ++) { /* Perform substitutions */
      char *              charptr;                /* Place where token found    */

      charptr = chartab;                          /* Start from beginning of string                      */
      while ((charptr = strstr (charptr, substab[subsnum][0])) != NULL) { /* As long as a matching found */
        int                 subslen;

        subslen = strlen (substab[subsnum][0]);
        if (isalnum (charptr[subslen]) || (charptr[subslen] == '_')) { /* If next character is part of a longer identifier */
          charptr += subslen + 1;                 /* Skip substitution */
          continue;
        }

        strcpy (chartmp, charptr + subslen);      /* Save remaining of line                                          */
        sprintf (charptr, "%s%s", substab[subsnum][1], chartmp); /* Replace remaining of line with substituted token */
        charptr += strlen (substab[subsnum][1]);  /* Restart search from end of substituted token                    */
      }
    }
#ifdef SCOTCH_NAME_SUFFIX
    if (suffptr[0] != '\0') {                     /* If suffix provided                     */
      if (regexec (&regedat, chartab, 1, &matcdat, 0) == 0) { /* If matched a function name */
        strcpy (chartmp, chartab + matcdat.rm_eo); /* Save remaining of line                */
        sprintf (chartab + matcdat.rm_eo, "%s%s", suffptr, chartmp); /* Add suffix to name  */
      }
    }
#endif /* SCOTCH_NAME_SUFFIX */

    fputs (chartab, C_filepntrhedout);            /* Output possibly updated line */
  }

#ifdef SCOTCH_NAME_SUFFIX
  if (suffptr[0] != '\0')                         /* If suffix provided      */
    regfree (&regedat);                           /* Free regular expression */
#endif /* SCOTCH_NAME_SUFFIX */

#ifdef SCOTCH_DEBUG_MAIN1
  for (i = 0; i < C_FILENBR; i ++) {              /* For all file names     */
    if ((C_fileTab[i].name[0] != '-') ||          /* If not standard stream */
        (C_fileTab[i].name[1] != '\0')) {
      fclose (C_fileTab[i].pntr);                 /* Close the stream */
    }
  }
#endif /* SCOTCH_DEBUG_MAIN1 */

  exit (0);
}
