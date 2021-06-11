/* Copyright 2019 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_dump.c                            **/
/**                                                        **/
/**   AUTHORS    : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module dumps the contents of a     **/
/**                graph on the form of a C source code.   **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 27 dec 2019     **/
/**                                 to   : 29 dec 2019     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define GRAPH_DUMP

#include "module.h"
#include "common.h"
#include "graph.h"
#include "graph_dump.h"

/* This routine dumps the given graph on
** the form of C source code.
** - 0   : on success.
** - !0  : on error.
*/

int
graphDump (
const Graph * const         grafptr,              /* Graph to dump */
const char * const          prefptr,              /* Prefix name   */
const char * const          suffptr,              /* Suffix name   */
FILE * const                fileptr)              /* Output stream */
{
  int                 o;

  o  = graphDumpArrays (grafptr, "Gnum", prefptr, suffptr, fileptr); /* Dump data arrays */
  o |= (fprintf (fileptr,
                 "void\n"
                 "%sGraphBuild%s (\n"
                 "Graph *               grafptr)\n"
                 "{\n"
                 "  graphInit (grafptr);\n",
                 prefptr, suffptr) < 0);
  o |= graphDump2 (grafptr, "grafptr->", prefptr, suffptr, fileptr);
  o |= (fprintf (fileptr,
                 "}\n") < 0);

  return (o);
}

int
graphDump2 (
const Graph * const         grafptr,              /* Graph to dump       */
const char * const          dataptr,              /* Data structure name */
const char * const          prefptr,              /* Prefix name         */
const char * const          suffptr,              /* Suffix name         */
FILE * const                fileptr)              /* Output stream       */
{
  int                 o;

  o = (fprintf (fileptr,
                "  %sbaseval = " GNUMSTRING ";\n"
                "  %svertnbr = " GNUMSTRING ";\n"
                "  %svertnnd = " GNUMSTRING ";\n"
                "  %sverttax = %sverttab%s - " GNUMSTRING ";\n",
                dataptr, grafptr->baseval,
                dataptr, grafptr->vertnbr,
                dataptr, grafptr->vertnnd,
                dataptr, prefptr, suffptr, grafptr->baseval) < 0);
  if (grafptr->vendtax != (grafptr->verttax + 1))
    o |= (fprintf (fileptr,
                   "  %svendtax = %svendtab%s - " GNUMSTRING ";\n",
                   dataptr, prefptr, suffptr, grafptr->baseval) < 0);
  else
    o |= (fprintf (fileptr,
                   "  %svendtax = %sverttax + 1;\n",
                   dataptr, dataptr) < 0);
  if (grafptr->velotax != NULL)
    o |= (fprintf (fileptr,
                   "  %svelotax = %svelotab%s - " GNUMSTRING ";\n",
                   dataptr, prefptr, suffptr, grafptr->baseval) < 0);
  if (grafptr->vlbltax != NULL)
    o |= (fprintf (fileptr,
                   "  %svlbltax = %svlbltab%s - " GNUMSTRING ";\n",
                   dataptr, prefptr, suffptr, grafptr->baseval) < 0);
  o |= (fprintf (fileptr,
                 "  %svelosum = " GNUMSTRING ";\n"
                 "  %sedgenbr = " GNUMSTRING ";\n"
                 "  %sedgetax = %sedgetab%s - " GNUMSTRING ";\n",
                 dataptr, grafptr->velosum,
                 dataptr, grafptr->edgenbr,
                 dataptr, prefptr, suffptr, grafptr->baseval) < 0);
  if (grafptr->edlotax != NULL)
    o |= (fprintf (fileptr,
                   "  %sedlotax = %sdelotab%s - " GNUMSTRING ";\n",
                   dataptr, prefptr, suffptr, grafptr->baseval) < 0);
  o |= (fprintf (fileptr,
                 "  %sedlosum = " GNUMSTRING ";\n"
                 "  %sdegrmax = " GNUMSTRING ";\n",
                 dataptr, grafptr->edlosum,
                 dataptr, grafptr->degrmax) < 0);

  return (o);
}

/* This routine dumps a data array on
** the form of C source code.
** - 0   : on success.
** - !0  : on error.
*/

int
graphDumpArray (
const Gnum * const          datatab,
const Gnum                  datanbr,
const char * const          tyinptr,              /* Integer type name */
const char * const          prefptr,              /* Prefix name       */
const char * const          nameptr,              /* Infix name        */
const char * const          suffptr,              /* Suffix name       */
FILE * const                fileptr)
{
  Gnum                datanum;

  if (fprintf (fileptr, "%s %s%s%s[] = {",
               tyinptr, prefptr, nameptr, suffptr) < 0)
    return (1);
  for (datanum = 0; datanum < (datanbr - 1); datanum ++) {
    if (fprintf (fileptr, "%s" GNUMSTRING ",",
	         (((datanum % 16) == 0) ? "\n  " : " "), datatab[datanum]) < 0)
      return (1);
  }
  if (datanbr > 0) {
    if (fprintf (fileptr, "%s" GNUMSTRING,
	         (((datanum % 16) == 0) ? "\n  " : " "), datatab[datanum]) < 0)
      return (1);
  }
  if (fprintf (fileptr, "\n};\n\n") < 0)
    return (1);

  return (0);
}

/* This routine dumps graph data arrays
** on the form of C source code.
** - 0   : on success.
** - !0  : on error.
*/

int
graphDumpArrays (
const Graph * const         grafptr,              /* Graph to dump     */
const char * const          tyinptr,              /* Integer type name */
const char * const          prefptr,              /* Prefix name       */
const char * const          suffptr,              /* Suffix name       */
FILE * const                fileptr)              /* Output stream     */
{
  int                 o;

  const Gnum                        baseval = grafptr->baseval;
  const Gnum                        vertnbr = grafptr->vertnbr;
  const Gnum                        edgenbr = grafptr->edgenbr;

  o = 0;
  if (grafptr->vendtax != (grafptr->verttax + 1)) {
    o |= graphDumpArray (grafptr->verttax + baseval, vertnbr, tyinptr, prefptr, "verttab", suffptr, fileptr);
    o |= graphDumpArray (grafptr->vendtax + baseval, vertnbr, tyinptr, prefptr, "vendtab", suffptr, fileptr);
  }
  else
    o |= graphDumpArray (grafptr->verttax + baseval, vertnbr + 1, tyinptr, prefptr, "verttab", suffptr, fileptr);
  if (grafptr->velotax != NULL)
    o |= graphDumpArray (grafptr->velotax + baseval, vertnbr, tyinptr, prefptr, "velotab", suffptr, fileptr);
  if (grafptr->vlbltax != NULL)
    o |= graphDumpArray (grafptr->vlbltax + baseval, vertnbr, tyinptr, prefptr, "vlbltab", suffptr, fileptr);

  o |= graphDumpArray (grafptr->edgetax + baseval, edgenbr, tyinptr, prefptr, "edgetab", suffptr, fileptr);
  if (grafptr->edlotax != NULL)
    o |= graphDumpArray (grafptr->edlotax + baseval, edgenbr, tyinptr, prefptr, "edlotab", suffptr, fileptr);

  return (o);
}
