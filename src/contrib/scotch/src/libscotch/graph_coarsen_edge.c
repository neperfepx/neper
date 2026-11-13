/* Copyright 2004,2007,2009,2012,2018,2025 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_coarsen_edge.c                    **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This commodity file contains the edge   **/
/**                arrays building subroutine which is     **/
/**                duplicated, with minor modifications,   **/
/**                into graph_coarsen.c.                   **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 17 dec 2001     **/
/**                                 to   : 25 feb 2004     **/
/**                # Version 5.0  : from : 13 dec 2006     **/
/**                                 to   : 14 dec 2006     **/
/**                # Version 5.1  : from : 30 oct 2009     **/
/**                                 to   : 30 oct 2009     **/
/**                # Version 6.0  : from : 28 oct 2012     **/
/**                                 to   : 28 feb 2015     **/
/**                # Version 7.0  : from : 28 jul 2018     **/
/**                                 to   : 28 oct 2025     **/
/**                                                        **/
/************************************************************/

static
void
GRAPHCOARSENEDGENAME (
const GraphCoarsenData * restrict const coarptr, /* The coarptr structure is constant during all of this process */
GraphCoarsenThread * restrict const     thrdptr)
{
  Gnum                coarvertnum;
  Gnum                coarvertnnd;
  Gnum                coaredgenum;
#ifndef GRAPHCOARSENEDGECOUNT
  Gnum                coaredloadj;                /* Edge load sum adjust with respect to fine graph edge load sum */
  Gnum                coardegrmax;
#endif /* GRAPHCOARSENEDGECOUNT */

  const Graph * restrict const              finegrafptr = coarptr->finegrafptr;
  const Gnum * restrict const               fineverttax = finegrafptr->verttax;
  const Gnum * restrict const               finevendtax = finegrafptr->vendtax;
  const Gnum * restrict const               fineedgetax = finegrafptr->edgetax;
#ifdef GRAPHCOARSENEDLOTAB
  const Gnum * restrict const               fineedlotax = finegrafptr->edlotax;
#endif /* GRAPHCOARSENEDLOTAB */
  Gnum * restrict const                     finecoartax = coarptr->finematetax;
#ifndef GRAPHCOARSENEDGECOUNT
  const Gnum * restrict const               finevelotax = finegrafptr->velotax;
  const Graph * restrict const              coargrafptr = coarptr->coargrafptr;
  Gnum * restrict const                     coarverttax = coargrafptr->verttax;
  Gnum * restrict const                     coarvelotax = coargrafptr->velotax;
  Gnum * restrict const                     coaredgetax = coargrafptr->edgetax;
  Gnum * restrict const                     coaredlotax = coargrafptr->edlotax;
#endif /* GRAPHCOARSENEDGECOUNT */
  GraphCoarsenHash * restrict const         coarhashtab = thrdptr->coarhashtab; /* Hash table is thread-dependent for memory locality */
  const Gnum                                coarhashmsk = coarptr->coarhashmsk;
  const GraphCoarsenMulti * restrict const  coarmulttax = coarptr->coarmulttab - finegrafptr->baseval;

#ifndef GRAPHCOARSENEDGECOUNT
  coaredloadj = 0;
  coardegrmax = 0;
#endif /* GRAPHCOARSENEDGECOUNT */
  for (coarvertnum = thrdptr->coarvertbas,        /* For all local coarse vertices */
       coarvertnnd = thrdptr->coarvertnnd, coaredgenum = thrdptr->coaredgebas;
       coarvertnum < coarvertnnd; coarvertnum ++) {
    Gnum                finevertnum;
    int                 i;

#ifndef GRAPHCOARSENEDGECOUNT                     /* If we do not only want to count */
    Gnum                coarveloval;              /* Load of coarse vertex           */
    Gnum                coaredgetmp;              /* Current index in edge array     */

    coarverttax[coarvertnum] =                    /* Set vertex edge index */
    coaredgetmp = coaredgenum;
    coarveloval = 0;
#endif /* GRAPHCOARSENEDGECOUNT */
    i = 0;
    do {                                          /* For all fine edges of multinode vertices */
      Gnum                fineedgenum;

      finevertnum = coarmulttax[coarvertnum].vertnum[i];
#ifndef GRAPHCOARSENEDGECOUNT                     /* If we do not only want to count */
      coarveloval += (finevelotax != NULL) ? finevelotax[finevertnum] : 1;
#endif /* GRAPHCOARSENEDGECOUNT */

      for (fineedgenum = fineverttax[finevertnum];
           fineedgenum < finevendtax[finevertnum]; fineedgenum ++) {
        Gnum                coarvertend;          /* Number of coarse vertex which is end of fine edge */
        Gnum                hashnum;

        coarvertend = finecoartax[fineedgetax[fineedgenum]];
        if (coarvertend != coarvertnum) {         /* If not end of collapsed edge */
          for (hashnum = (coarvertend * GRAPHCOARSENHASHPRIME) & coarhashmsk; ; hashnum = (hashnum + 1) & coarhashmsk) {
            if (coarhashtab[hashnum].vertorgnum != coarvertnum) { /* If old slot           */
              coarhashtab[hashnum].vertorgnum = coarvertnum; /* Mark it in reference array */
              coarhashtab[hashnum].vertendnum = coarvertend;
              coarhashtab[hashnum].edgenum    = coaredgenum;
#ifndef GRAPHCOARSENEDGECOUNT                     /* If we do not only want to count */
              coaredgetax[coaredgenum]  = coarvertend; /* One more edge created      */
#ifdef GRAPHCOARSENEDLOTAB
              coaredlotax[coaredgenum]  = fineedlotax[fineedgenum];
#else /* GRAPHCOARSENEDLOTAB */
              coaredlotax[coaredgenum]  = 1;
#endif /* GRAPHCOARSENEDLOTAB */
#endif /* GRAPHCOARSENEDGECOUNT */
              coaredgenum ++;
              break;                              /* Give up hashing */
            }
            if (coarhashtab[hashnum].vertendnum == coarvertend) { /* If coarse edge already exists */
#ifndef GRAPHCOARSENEDGECOUNT
#ifdef GRAPHCOARSENEDLOTAB
              coaredlotax[coarhashtab[hashnum].edgenum] += fineedlotax[fineedgenum]; /* Accumulate edge load */
#else /* GRAPHCOARSENEDLOTAB */
              coaredlotax[coarhashtab[hashnum].edgenum] ++;
#endif /* GRAPHCOARSENEDLOTAB */
#endif /* GRAPHCOARSENEDGECOUNT */
              break;                              /* Give up hashing */
            }
          }
        }
#ifndef GRAPHCOARSENEDGECOUNT
        else {
#ifdef GRAPHCOARSENEDLOTAB
          coaredloadj -= fineedlotax[fineedgenum];
#else /* GRAPHCOARSENEDLOTAB */
          coaredloadj --;
#endif /* GRAPHCOARSENEDLOTAB */
        }
#endif /* GRAPHCOARSENEDGECOUNT */
      }
    } while (i ++, finevertnum != coarmulttax[coarvertnum].vertnum[1]); /* Skip to next matched vertex if both vertices not equal */

#ifndef GRAPHCOARSENEDGECOUNT                     /* If we do not only want to count  */
    coarvelotax[coarvertnum] = coarveloval;       /* Create coarse vertex load array  */
    coaredgetmp = coaredgenum - coaredgetmp;      /* Compute degree of current vertex */
    if (coardegrmax < coaredgetmp)
      coardegrmax = coaredgetmp;
#ifndef GRAPHCOARSENNOTHREAD
    if ((coarptr->flagval & GRAPHCOARSENNOCOMPACT) != 0) /* In all cases except for non-compact, multi-threaded graphs */
      coargrafptr->vendtax[coarvertnum] = coaredgenum;
#endif /* GRAPHCOARSENNOTHREAD */
#endif /* GRAPHCOARSENEDGECOUNT */
  }                                               /* End of (local) edge array not marked since will be done by next or main thread */

  thrdptr->coaredgebas = coaredgenum;             /* Record counted number of edges  */
#ifndef GRAPHCOARSENEDGECOUNT
  thrdptr->coaredloadj = coaredloadj;
  thrdptr->coardegrmax = coardegrmax;
#endif /* GRAPHCOARSENEDGECOUNT */
}
