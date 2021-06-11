/* Copyright 2019,2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_dump.c                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the source   **/
/**                graph handling routines of the          **/
/**                libSCOTCH library.                      **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 28 dec 2019     **/
/**                                 to   : 09 jan 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define LIBRARY

#include "module.h"
#include "common.h"
#include "graph.h"
#include "graph_dump.h"
#include "scotch.h"

/************************************/
/*                                  */
/* These routines are the C API for */
/* the graph dumping routine.       */
/*                                  */
/************************************/

/*+ This routine dumps the given graph on the form
*** of C source code, using a prefix and a suffix.
*** It returns:
*** - 0   : on success.
*** - !0  : on error.
+*/

int
SCOTCH_graphDump (
const SCOTCH_Graph * const  libgrafptr,
const char * const          prefptr,
const char * const          suffptr,
FILE * const                fileptr)
{
  const Graph *       grafptr;
  const char *        prefstr;
  const char *        suffstr;
  int                 o;

  prefstr = (prefptr != NULL) ? prefptr : "";
  suffstr = (suffptr != NULL) ? suffptr : "";
  grafptr = (const Graph *) libgrafptr;

  o = graphDumpArrays (grafptr, "SCOTCH_Num", prefstr, suffstr, fileptr);

  o |= (fprintf (fileptr,
                 "int\n"
                 "%sGraphBuild%s (\n"
                 "SCOTCH_Graph *              grafptr)\n"
                 "{\n"
                 "  return (SCOTCH_graphBuild (grafptr, " GNUMSTRING ", " GNUMSTRING ", %sverttab%s",
                 prefstr, suffstr, grafptr->baseval, grafptr->vertnbr, prefstr, suffstr) < 0);
  if (grafptr->vendtax != (grafptr->verttax + 1))
    o |= (fprintf (fileptr, ", %svendtab%s", prefstr, suffstr) < 0);
  else
    o |= (fprintf (fileptr, ", NULL") < 0);
  if (grafptr->velotax != NULL)
    o |= (fprintf (fileptr, ", %svelotab%s", prefstr, suffstr) < 0);
  else
    o |= (fprintf (fileptr, ", NULL") < 0);
  if (grafptr->vlbltax != NULL)
    o |= (fprintf (fileptr, ", %svlbltab%s", prefstr, suffstr) < 0);
  else
    o |= (fprintf (fileptr, ", NULL") < 0);

  o |= (fprintf (fileptr,
                 ",\n                             " GNUMSTRING ", %sedgetab%s",
                 grafptr->edgenbr, prefstr, suffstr) < 0);
  if (grafptr->edlotax != NULL)
    o |= (fprintf (fileptr, ", %sedlotab%s", prefstr, suffstr) < 0);
  else
    o |= (fprintf (fileptr, ", NULL") < 0);

  o |= (fprintf (fileptr,
                 "));\n"
                 "}\n") < 0);

  return (o);
}
