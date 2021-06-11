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
/**   NAME       : hgraph_dump.c                           **/
/**                                                        **/
/**   AUTHORS    : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module dumps the contents of a     **/
/**                halo graph on the form of a C source    **/
/**                code.                                   **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 27 dec 2019     **/
/**                                 to   : 25 jan 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define HGRAPH_DUMP

#include "module.h"
#include "common.h"
#include "graph.h"
#include "graph_dump.h"
#include "hgraph.h"
#include "hgraph_dump.h"

/* This routine dumps the given halo graph on
** the form of C source code.
** - 0   : on success.
** - !0  : on error.
*/

int
hgraphDump (
const Hgraph * restrict const grafptr,            /* Graph to dump */
const char * const            prefptr,            /* Prefix name   */
const char * const            suffptr,            /* Suffix name   */
FILE * const                  fileptr)            /* Output stream */
{
  int                 o;

  o  = graphDumpArrays (&grafptr->s, "Gnum", prefptr, suffptr, fileptr); /* Dump source graph data arrays */
  if (grafptr->vnhdtax != grafptr->s.vendtax)
    o |= graphDumpArray  (grafptr->vnhdtax + grafptr->s.baseval, grafptr->vnohnbr, "Gnum", prefptr, "vnhdtab", suffptr, fileptr);

  o |= (fprintf (fileptr,
                 "void\n"
                 "%sHgraphBuild%s (\n"
                 "Hgraph *              grafptr)\n"
                 "{\n"
                 "  hgraphInit (grafptr);\n",
                 prefptr, suffptr) < 0);
  o |= graphDump2 (&grafptr->s, "grafptr->s.", prefptr, suffptr, fileptr);

  o |= (fprintf (fileptr,
                 "  grafptr->vnohnbr = " GNUMSTRING ";\n"
                 "  grafptr->vnohnnd = " GNUMSTRING ";\n",
                 grafptr->vnohnbr,
                 grafptr->vnohnnd) < 0);
  if (grafptr->vnhdtax != grafptr->s.vendtax)
    o |= (fprintf (fileptr,
                   "  grafptr->vnhdtax = %svnhdtab%s - " GNUMSTRING ";\n",
                   prefptr, suffptr, grafptr->s.baseval) < 0);
  else
    o |= (fprintf (fileptr,
                   "  grafptr->vnhdtax = grafptr->s.vendtax;\n") < 0);
  o |= (fprintf (fileptr,
                 "  grafptr->enohnbr = " GNUMSTRING ";\n"
                 "  grafptr->enlosum = " GNUMSTRING ";\n",
                 grafptr->enohnbr,
                 grafptr->enlosum) < 0);

  o |= (fprintf (fileptr,
                 "}\n") < 0);

  return (o);
}
