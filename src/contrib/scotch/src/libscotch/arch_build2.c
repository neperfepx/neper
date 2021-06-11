/* Copyright 2015,2016,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_build2.c                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module builds a type-2             **/
/**                decomposition-described architecture    **/
/**                from a source graph.                    **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 02 may 2015     **/
/**                                 to   : 22 feb 2018     **/
/**                                                        **/
/**   NOTES      : # The code of the main routine derives  **/
/**                  from that of archSubArchBuild().      **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ARCH_DECO2
#define ARCH_BUILD2

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "arch_sub.h"
#include "arch_deco2.h"
#include "graph_coarsen.h"
#include "arch_build2.h"

/***********************************/
/*                                 */
/* The matching handling routines. */
/*                                 */
/***********************************/

/* This routine frees the matching data
** structure.
** It returns:
** - void  : in all cases.
*/

static
void
archDeco2BuildMatchExit (
ArchDeco2BuildMatch * restrict const  matcptr)
{
  ArchDeco2BuildLevl *  lewgtab;
  ArchDeco2Levl *       levltab;

  if ((lewgtab = matcptr->lewgtab) != NULL) {
    ArchDeco2BuildLevl *  lewgptr;

    for (lewgptr = lewgtab + matcptr->levlnum; lewgptr >= lewgtab; lewgptr --) {
      Gnum *              edwgtab;

      if ((edwgtab = lewgptr->edwgtab) != NULL)
        memFree (edwgtab);                        /* Free group leader */
    }
    memFree (lewgtab);
  }

  if ((levltab = matcptr->levltab) != NULL) {
    ArchDeco2Levl *     levlptr;

    for (levlptr = levltab + matcptr->levlnum; levlptr >= levltab; levlptr --)
      graphExit (&levlptr->grafdat);

    memFree (levltab);
  }

  memFree (matcptr->multtab);                     /* Free group leader */
  memFree (matcptr->hashtab);
}

/* This routine initializes the matching
** data structure according to the original
** graph to be coarsened.
** It returns:
** - 0   : if the data structure has been
**         successfully initialized.
** - !0  : on error.
*/

static
int
archDeco2BuildMatchInit (
ArchDeco2BuildMatch * restrict const  matcptr,
const Graph * restrict const          grafptr)
{
  Gnum                verttmp;
  Gnum                levlmax;                    /* Estimated upper bound on number of coarsening levels */
  Gnum                multnbr;
  Gnum                hashmax;
  Gnum                hashsiz;

  const Gnum          vertnbr = grafptr->vertnbr;

#ifdef SCOTCH_DEBUG_ARCH1
  if (sizeof (ArchCoarsenMulti) != sizeof (GraphCoarsenMulti)) {
    errorPrint ("archDeco2BuildMatchInit: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  for (hashsiz = 32, hashmax = grafptr->degrmax * 4; /* Compute size of hash table */
       hashsiz < hashmax; hashsiz *= 2) ;
  matcptr->hashsiz = hashsiz;
  if ((matcptr->hashtab = memAlloc (hashsiz * sizeof (ArchDeco2BuildHash))) == NULL) {
    errorPrint ("archDeco2BuildMatchInit: out of memory (1)");
    return     (1);
  }

  multnbr = vertnbr - 1;                          /* In case graph is star-like, only one matching will take place */
  if (memAllocGroup ((void **) (void *)
                     &matcptr->multtab, (size_t) (multnbr * sizeof (ArchCoarsenMulti)),
                     &matcptr->ficotab, (size_t) (vertnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("archDeco2BuildMatchInit: out of memory (2)");
    memFree    (matcptr->hashtab);
    return     (1);
  }

  for (verttmp = vertnbr, levlmax = 1; verttmp != 0; verttmp >>= 1, levlmax ++) ;
  levlmax *= 2;                                   /* Compute estimated upper bound on number of coarsening levels */
#ifdef SCOTCH_DEBUG_ARCH2
  levlmax = 1;                                    /* Set levlmax to a small value to test reallocation */
#endif /* SCOTCH_DEBUG_ARCH2 */

  matcptr->levltab = NULL;                        /* In case of error */
  matcptr->levlmax = levlmax;
  matcptr->levlnum = 0;
  if ((matcptr->lewgtab = memAlloc (levlmax * sizeof (ArchDeco2BuildLevl))) == NULL) {
    errorPrint ("archDeco2BuildMatchInit: out of memory (3)");
    archDeco2BuildMatchExit (matcptr);
    return (1);
  }
  matcptr->lewgtab[0].edwgtab = NULL;             /* TRICK: in case of error             */
  matcptr->lewgtab[0].vewgtab = NULL;             /* No vertex traversal cost at level 0 */
  matcptr->lewgtab[0].vewgsum = 0;
  matcptr->lewgtab[0].edwgsum = grafptr->edlosum; /* Assume graph has no edge weights */

  if ((matcptr->levltab = memAlloc (levlmax * sizeof (ArchDeco2Levl))) == NULL) {
    errorPrint ("archDeco2BuildMatchInit: out of memory (4)");
    archDeco2BuildMatchExit (matcptr);
    return (1);
  }

  if (graphClone (grafptr, &matcptr->levltab[0].grafdat) != 0) { /* Clone original graph in level 0 */
    errorPrint ("archDeco2BuildMatchInit: cannot create clone graph");
    archDeco2BuildMatchExit (matcptr);
    return (1);
  }

  if (matcptr->levltab[0].grafdat.edlotax != NULL) { /* If original graph has edge loads */
    Gnum                edgenbr;

    edgenbr = matcptr->levltab[0].grafdat.edgenbr;
    if ((matcptr->lewgtab[0].edwgtab = memAlloc (edgenbr * sizeof (Gnum))) == NULL) {
      errorPrint ("archDeco2BuildMatchInit: out of memory (5)");
      archDeco2BuildMatchExit (matcptr);
      return (1);
    }
    matcptr->lewgtab[0].edwgsum = grafptr->edlosum; /* Record edge load sum for finest graph */
    memCpy (matcptr->lewgtab[0].edwgtab, matcptr->levltab[0].grafdat.edlotax + grafptr->baseval, edgenbr * sizeof (Gnum));  /* Keep edge costs (if any) */
    matcptr->levltab[0].grafdat.edlosum =         /* Record edge load sum of inversed loads */
    graphIelo (&matcptr->levltab[0].grafdat, matcptr->levltab[0].grafdat.edlotax, matcptr->levltab[0].grafdat.edlotax); /* Inverse edge costs for coarsening */
  }

  return (0);
}

/* This routine computes a matching from
** the current state of the matching structure.
** It returns:
** - >=0  : size of matching.
** - <0   : on error.
*/

Anum
archDeco2BuildMatchMate (
ArchDeco2BuildMatch * restrict const          matcptr,
ArchCoarsenMulti * restrict * restrict const  multptr)
{
  const Graph * restrict          finegrafptr;
  const Gnum * restrict           fineedwgtax;
  const Gnum * restrict           finevewgtax;
  const Gnum * restrict           fineedgetax;
  const Gnum * restrict           fineverttax;
  const Gnum * restrict           finevendtax;
  Anum                            finevertnbr;
  Gnum * restrict                 finecoartax;
  const ArchDeco2Levl * restrict  finelevlptr;
  ArchDeco2Levl * restrict        coarlevlptr;
  ArchCoarsenMulti * restrict     coarmulttax;
  Graph * restrict                coargrafptr;
  Gnum * restrict                 coaredwgtax;
  Gnum * restrict                 coarvewgtax;
  Gnum                            coarvewgsum;
  Gnum                            coaredwgsum;
  Gnum                            coarvertnnd;
  Gnum                            coarvertnum;
  Gnum                            coarvertsum;    /* Sum of all coarse vertices kept in coarsening tree       */
  Gnum * restrict                 coarvelotax;
  Gnum                            coaredgenum;
  ArchDeco2BuildHash * restrict   coarhashtab;
  Gnum                            coarhashmax;
  Gnum                            coarhashsiz;
  Gnum                            coarhashmsk;
  Gnum                            levlmax;
  Gnum                            levlnum;

  levlnum     = matcptr->levlnum;
  finelevlptr = &matcptr->levltab[levlnum];
  finevertnbr = finelevlptr->grafdat.vertnbr;
  if (finevertnbr <= 1)                           /* This routine is not meant to handle single-vertex graphs */
    return (-1);

  levlmax = matcptr->levlmax;
  if (++ levlnum >= levlmax) {                    /* If level array already full */
    ArchDeco2BuildLevl *  lewgtmp;
    ArchDeco2Levl *       levltmp;

    levlmax += (levlmax >> 2) + 1;                /* Increase size by 25% */

    if ((lewgtmp = memRealloc (matcptr->lewgtab, levlmax * sizeof (ArchDeco2BuildLevl))) == NULL) {
      errorPrint ("archDeco2BuildMatchMate: out of memory (1)");
      return     (-1);
    }
    matcptr->lewgtab = lewgtmp;

    if ((levltmp = memRealloc (matcptr->levltab, levlmax * sizeof (ArchDeco2Levl))) == NULL) {
      errorPrint ("archDeco2BuildMatchMate: out of memory (2)");
      return     (-1);
    }
    matcptr->levltab = levltmp;
    matcptr->levlmax = levlmax;
    finelevlptr      = &levltmp[levlnum - 1];     /* In case array was moved */
  }

  *multptr    =                                   /* Provide address of multinode array */
  coarmulttax = matcptr->multtab;                 /* Un-based multinode array           */
  finecoartax = matcptr->ficotab;                 /* Un-based fine-to-coarse array      */
  finegrafptr = &finelevlptr->grafdat;
  coarlevlptr = &matcptr->levltab[levlnum];
  coargrafptr = &coarlevlptr->grafdat;
  if (graphCoarsen (&finelevlptr->grafdat, coargrafptr,
                    (Gnum **) &finecoartax, (GraphCoarsenMulti **) &coarmulttax,
                    0, 1.0, GRAPHCOARSENNONE, NULL, NULL, 0, NULL) != 0) {
    errorPrint ("archDeco2BuildMatchMate: cannot coarsen graph");
    return     (-1);
  }

  matcptr->levlnum = levlnum;                     /* Record new level since coarse graph must be freed */
  matcptr->lewgtab[levlnum].edwgtab = NULL;       /* Group leader not yet allocated                    */

  for (coarhashsiz = matcptr->hashsiz, coarhashmax = coargrafptr->degrmax * 4; /* Recompute size of hash table */
       coarhashsiz < coarhashmax; coarhashsiz *= 2) ;
  if (coarhashsiz > matcptr->hashsiz) {           /* Reallocate hash table if it has to be expanded */
    ArchDeco2BuildHash *  coarhashtmp;

    if ((coarhashtmp = memRealloc (matcptr->hashtab, coarhashsiz * sizeof (ArchDeco2BuildHash))) == NULL) {
      errorPrint ("archDeco2BuildMatchMate: out of memory (3)");
      return     (-1);
    }
    matcptr->hashtab = coarhashtmp;
    matcptr->hashsiz = coarhashsiz;
  }
  coarhashmsk = coarhashsiz - 1;
  coarhashtab = matcptr->hashtab;
  memSet (coarhashtab, ~0, coarhashsiz * sizeof (ArchDeco2BuildHash)); /* Initialize hash table area */

  if (memAllocGroup ((void **) (void *)
                     &matcptr->lewgtab[levlnum].edwgtab, (size_t) (coargrafptr->edgenbr * sizeof (Gnum)),
                     &matcptr->lewgtab[levlnum].vewgtab, (size_t) (coargrafptr->vertnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("archDeco2BuildMatchMate: out of memory (4)");
    return     (-1);
  }

  coarvewgtax = matcptr->lewgtab[levlnum].vewgtab - coargrafptr->baseval;
  coaredwgtax = matcptr->lewgtab[levlnum].edwgtab - coargrafptr->baseval;
  coarvelotax = coargrafptr->velotax;
  coarmulttax -= coargrafptr->baseval;
  finecoartax -= coargrafptr->baseval;
  fineverttax = finegrafptr->verttax;
  finevendtax = finegrafptr->vendtax;
  fineedgetax = finegrafptr->edgetax;
  finevewgtax = matcptr->lewgtab[levlnum - 1].vewgtab;
  fineedwgtax = matcptr->lewgtab[levlnum - 1].edwgtab;
  if (finevewgtax != NULL)                        /* Fine edge and vertex weight arrays may be NULL */
    finevewgtax-= finegrafptr->baseval;
  if (fineedwgtax != NULL)
    fineedwgtax-= finegrafptr->baseval;

  coarvewgsum =
  coaredwgsum = 0;
  for (coarvertnum = coaredgenum = coargrafptr->baseval, coarvertnnd = coargrafptr->vertnnd, coarvertsum = 0;
       coarvertnum < coarvertnnd; coarvertnum ++) {
    Gnum                coaredgetmp;
    Gnum                coarvewgval;              /* Aggregated vertex traversal cost */
    Gnum                coaredwgval;              /* Aggregated edge traversal cost   */
    Gnum                finevertnum;
    int                 i;

    if (coarvelotax[coarvertnum] != 0)            /* If coarse vertex does not have a zero weight */
      coarvertsum ++;                             /* Account for it in tree                       */

    coaredgetmp = coaredgenum;                    /* Record edge position in array                              */
    coarvewgval = 0;                              /* Accumulate coarse vertex traversal costs                   */
    coaredwgval = 0;                              /* Accumulate coarse edge traversal costs for collapsed edges */

    i = 0;
    do {                                          /* For all fine edges of multinode vertices */
      Gnum                fineedgenum;

      finevertnum = coarmulttax[coarvertnum].vertnum[i];

      coarvewgval += (finevewgtax != NULL) ? finevewgtax[finevertnum] : 0; /* Accumulate vertex traversal costs */

      for (fineedgenum = fineverttax[finevertnum];
           fineedgenum < finevendtax[finevertnum]; fineedgenum ++) {
        Gnum                coarvertend;          /* Number of coarse vertex which is end of fine edge */
        Gnum                h;

        coarvertend = finecoartax[fineedgetax[fineedgenum]];
        if (coarvertend != coarvertnum) {         /* If not end of collapsed edge */
          for (h = (coarvertend * GRAPHCOARSENHASHPRIME) & coarhashmsk; ; h = (h + 1) & coarhashmsk) {
            if (coarhashtab[h].coarvertnum != coarvertnum) { /* If old slot           */
              coarhashtab[h].coarvertnum = coarvertnum; /* Mark it in reference array */
              coarhashtab[h].coarvertend = coarvertend;
              coarhashtab[h].coaredgenum = coaredgenum; /* Edge location in coarse graph */
              coarhashtab[h].fineedwgmin = (fineedwgtax != NULL) ? fineedwgtax[fineedgenum] : 1;
              coaredwgtax[coaredgenum] = h;       /* Record edge location for summing up */
              coaredgenum ++;                     /* One more edge created               */
              break;                              /* Give up hashing                     */
            }
            if (coarhashtab[h].coarvertend == coarvertend) { /* If coarse edge already exists */
              Gnum                fineedwgval;

              fineedwgval = (fineedwgtax != NULL) ? fineedwgtax[fineedgenum] : 1;
              if (coarhashtab[h].fineedwgmin > fineedwgval) /* Take minimum of edge weights */
                coarhashtab[h].fineedwgmin = fineedwgval;
              break;                              /* Give up hashing */
            }
          }
        }
        else                                      /* If collapsed edge, accumulate traversal cost (twice: once for each arc) */
          coaredwgval += (fineedwgtax != NULL) ? fineedwgtax[fineedgenum] : 1;
      }
    } while (i ++, finevertnum != coarmulttax[coarvertnum].vertnum[1]); /* Skip to next matched vertex if both vertices not equal */

    coarvewgsum             +=                    /* Accumulate vertex load sum of coarse graph                 */
    coarvewgtax[coarvertnum] = coarvewgval + coaredwgval; /* Add vertex traversal cost plus edge traversal cost */

#ifdef SCOTCH_DEBUG_ARCH2
    if (coaredgenum != coargrafptr->verttax[coarvertnum + 1]) { /* If not same number of coarse edges found */
      errorPrint ("archDeco2BuildMatchMate: internal error (1)");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */

    for ( ; coaredgetmp < coaredgenum; coaredgetmp ++) { /* Finalize building of edge traversal cost array */
      Gnum                h;

      h = coaredwgtax[coaredgetmp];               /* Replace hash table index with average edge traversal cost */
#ifdef SCOTCH_DEBUG_ARCH2
      if (coarhashtab[h].coaredgenum != coaredgetmp) {
        errorPrint ("archDeco2BuildMatchMate: internal error (2)");
        return     (1);
      }
#endif /* SCOTCH_DEBUG_ARCH2 */
      coaredwgsum             +=
      coaredwgtax[coaredgetmp] = coarhashtab[h].fineedwgmin;
    }
  }
  matcptr->lewgtab[levlnum].edwgsum = coaredwgsum;
  matcptr->lewgtab[levlnum].vewgsum = coarvewgsum;

  matcptr->vertsum += coarvertsum;                /* Sum all vertices in coarsening tree */

  return (coargrafptr->vertnbr);
}

/******************************************/
/*                                        */
/* The main routine, which computes the   */
/* type-2, decomposition-described target */
/* architecture.                          */
/*                                        */
/******************************************/

/* This routine collapses the recursive coarsening tree
** into a decomposition-defined tree, by removing single
** vertices that have not been collapsed. Unlike the
** archSubArchBuild3() routine from which it derives, it
** preserves some information on removed single vertices,
** so as to identify the relevant vertices at each level,
** to start distance computations.
** It returns:
** - the first free index in the domain array.
*/

static
Anum
archDeco2BuildBuild3 (
ArchDeco2Build3 * restrict const    dataptr,      /*+ Data area                               +*/
const ArchSubTree * restrict const  treeptr,      /*+ Pointer to current node being traversed +*/
const Anum                          domnnum,      /*+ Domain number of current node           +*/
const Anum                          domnidx,      /*+ Current first free slot in domain array +*/
const Anum                          levlnum)      /*+ Current tree level                      +*/
{
  Anum                termnum;
  const ArchSubTree * son0ptr;
  const ArchSubTree * son1ptr;
  const ArchSubTree * sonsptr;
  Anum                levltmp;
  Anum                didxtmp;
  Gnum                vnumidx;

  ArchSubData * restrict const    domntab = dataptr->domntab;
  ArchDeco2Data * restrict const  doextab = dataptr->doextab;
  ArchSubTerm * restrict const    termtab = dataptr->termtab;

  vnumidx = dataptr->vnumidx ++;                  /* One more slot for vertex number        */
  dataptr->vnumtab[vnumidx] = treeptr->vertnum;   /* Record vertex number of collapsed node */

  son0ptr = treeptr->sonstab[0];
  son1ptr = treeptr->sonstab[1];
  sonsptr = NULL;

  levltmp = levlnum - 1;                          /* Level 0 is finest graph, so proceed in decreasing order      */
  if (son0ptr != NULL)                            /* If node is a single node, traverse it without any processing */
    sonsptr = son0ptr;
  if (son1ptr != NULL)
    sonsptr = (sonsptr != NULL) ? NULL : son1ptr;
  if (sonsptr != NULL)
    return (archDeco2BuildBuild3 (dataptr, sonsptr, domnnum, domnidx, levltmp));

  domntab[domnidx].domnnum = domnnum;
  domntab[domnidx].domnsiz = treeptr->domnsiz;
  domntab[domnidx].domnwgt = treeptr->domnwgt;
  termnum                  =
  domntab[domnidx].termnum = treeptr->termnum;
  doextab[domnidx].levlnum = levlnum;
  doextab[domnidx].vnumidx = vnumidx;             /* Record index of vertex number for this level */

  didxtmp = domnidx + 1;
  if (son0ptr != NULL) {                          /* If node is a branch node */
    Anum                domntmp;

    domntmp = domnnum * 2;                        /* Domain number of first son */
    domntab[didxtmp].dfatidx    = domnidx;
    domntab[domnidx].dsubidx[0] = didxtmp;
    didxtmp = archDeco2BuildBuild3 (dataptr, son0ptr, domntmp, didxtmp, levltmp);
    domntab[didxtmp].dfatidx    = domnidx;
    domntab[domnidx].dsubidx[1] = didxtmp;
    didxtmp = archDeco2BuildBuild3 (dataptr, son1ptr, domntmp + 1, didxtmp, levltmp);
  }
  else {
    domntab[domnidx].dsubidx[0] =
    domntab[domnidx].dsubidx[1] = -1;

    termtab[termnum].domnidx = domnidx;
  }

  return (didxtmp);
}

/*
** This routine builds a type-2 target architecture
** from the given source graph and the optional
** vertex list. If vnumtab is null, all vertices
** are part of the target architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archDeco2ArchBuild (
Arch * restrict const       archptr,              /*+ Decomposition architecture to build    +*/
const Graph * const         grafptr,              /*+ Source graph modeling the architecture +*/
const Gnum                  vnumnbr,              /*+ Number of vertices in list             +*/
const Gnum * restrict const vnumtab)              /*+ vertex array                           +*/
{
  ArchDeco2Build3     datadat;                    /* Data for building decomposition tree */
  ArchDeco2BuildMatch matcdat;                    /* Data for successive matchings        */
  ArchSubTree *       rootptr;
  ArchSubTree *       treetab;
  ArchSubTree *       treetmp;
  ArchSubTerm *       termtab;
  ArchSubData *       domntab;
  ArchDeco2Data *     doextab;
  Anum                domnnbr;                    /* Number of domains                    */
  Gnum                vertnum;
  Gnum * restrict     velotmp;                    /* Temporary vertex load array          */
  Anum                vnumnum;
  Gnum *              vnumtmp;
  Anum                levlnum;
  const char *        messptr;                    /* Pointer to error message             */

  ArchDeco2 * restrict const  decoptr = &archptr->data.deco2;
  const Gnum                  baseval = grafptr->baseval;
  const Gnum                  vertnbr = grafptr->vertnbr; /* Number of terminals in original graph */
  const Gnum * restrict const velotab = (grafptr->velotax != NULL) ? grafptr->velotax + baseval : NULL; /* Un-based original vertex load array */

  if (baseval != 0) {                             /* Because of current limitations in archSubArchBuild2() */
    errorPrint ("archDeco2ArchBuild: input graph must have base value set to 0");
    return     (1);
  }

#ifdef SCOTCH_DEBUG_ARCH1
  if ((vnumnbr < 0) || (vnumnbr > vertnbr)) {
    errorPrint ("archDeco2ArchBuild: invalid number of terminal domains");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  archInit (archptr);                             /* Initialize architecture body  */
  archptr->class = archClass2 ("deco", 1);        /* Set type-2 architecture class */

  domnnbr = 2 * vnumnbr - 1;
  if (memAllocGroup ((void **) (void *)
                     &termtab, (size_t) (vnumnbr * sizeof (ArchSubTerm)),
                     &domntab, (size_t) (domnnbr * sizeof (ArchSubData)),
                     &doextab, (size_t) (domnnbr * sizeof (ArchDeco2Data)), NULL) == NULL) {
    errorPrint ("archDeco2ArchBuild: out of memory (1)");
    return     (1);
  }
  if ((treetab = memAlloc ((vertnbr + 1) * sizeof (ArchSubTree))) == NULL) { /* TRICK: One more slot to link to coarser array */
    errorPrint ("archDeco2ArchBuild: out of memory (2)");
    memFree    (termtab);                         /* Free group leader */
    return     (1);
  }

  velotmp = NULL;                                 /* Assume no need for temporary load array     */
  if (vnumnbr < vertnbr) {                        /* If restriction of the target architecture   */
    if ((velotmp = memAlloc (vertnbr * sizeof (Gnum))) == NULL) { /* Create temporary load array */
      errorPrint ("archDeco2ArchBuild: out of memory (3)");
      memFree    (treetab);
      memFree    (termtab);
      return     (1);
    }
    memSet (velotmp, 0, vertnbr * sizeof (Gnum)); /* Assume no vertex kept in target architecture */
  }

  treetab ++;                                     /* TRICK: Hide first slot */

  for (vertnum = 0; vertnum < vertnbr; vertnum ++) { /* Initialize values for all vertices of original architecture */
    treetab[vertnum].domnsiz =
    treetab[vertnum].domnwgt = 0;
    treetab[vertnum].termnum =
    treetab[vertnum].vertnum = ANUMMAX;
    treetab[vertnum].sonstab[0] =
    treetab[vertnum].sonstab[1] = NULL;
  }

  messptr = NULL;
  for (vnumnum = 0; vnumnum < vnumnbr; vnumnum ++) { /* For all kept terminal numbers         */
    Gnum                vertnum;                  /* Terminal number in original architecture */

    vertnum = (vnumtab != NULL) ? (vnumtab[vnumnum] - baseval) : vnumnum; /* Un-base vertex number */
    if ((vertnum < 0) || (vertnum >= vertnbr))
      messptr = "archDeco2ArchBuild: invalid vertex number in vertex list";
    else if (treetab[vertnum].domnsiz != 0)
      messptr = "archDeco2ArchBuild: duplicate vertex number in vertex list";
    if (messptr != NULL) {
      errorPrint (messptr);
      if (velotmp != NULL)
        memFree (velotmp);
      memFree (treetab - 1);
      memFree (termtab);
      return  (1);
    }

    termtab[vnumnum].termnum = vertnum;           /* Record number of original terminal number */

    if (velotmp != NULL)
      velotmp[vertnum] = 1;                       /* Vertex belongs to restricted architecture */

    treetab[vertnum].domnsiz = 1;                 /* Here lies one terminal vertex */
    treetab[vertnum].domnwgt = (velotab != NULL) ? velotab[vertnum] : 1;
    treetab[vertnum].vertnum = vertnum;           /* Record number of original vertex in original graph        */
    treetab[vertnum].termnum = vnumnum;           /* Sub-architecture terminal number in original architecture */
  }

  if (archDeco2BuildMatchInit (&matcdat, grafptr) != 0) { /* Initialize matching structure */
    errorPrint ("archDeco2ArchBuild: cannot initialize matching structure");
    if (velotmp != NULL)
      memFree (velotmp);
    memFree (treetab - 1);
    memFree (termtab);
    return  (1);
  }

  if (velotmp != NULL) {                          /* If restricted architecture, keep track of present vertices */
    matcdat.levltab[0].grafdat.velotax = velotmp - baseval; /* Graph is cloned, so velotax can be modified      */
    matcdat.levltab[0].grafdat.velosum = vnumnbr;
  }

  decoptr->baseval = baseval;
  decoptr->termnbr = vnumnbr;
  decoptr->termtab = termtab;                     /* From now on, termtab will be freed on its own */
  decoptr->domnnbr = domnnbr;
  decoptr->domntab = domntab;
  decoptr->doextab = doextab;
#ifdef SCOTCH_DEBUG_ARCH1
  decoptr->vnumtab = NULL;
#endif /* SCOTCH_DEBUG_ARCH1 */

  intRandInit ();                                 /* Initialize random generator for coarsening */

  matcdat.vertsum = vnumnbr;                      /* Account for kept vertices of original graph */
  rootptr = archSubArchBuild2 (&matcdat, (Anum (*) (void *, ArchCoarsenMulti * restrict *)) archDeco2BuildMatchMate, treetab, vnumnbr);

  if (velotmp != NULL)                            /* Temporary vertex load array will no longer be necessary */
    memFree (velotmp);

  if (rootptr == NULL) {
    errorPrint ("archDeco2ArchBuild: cannot create sub-architecture (1)");
    archDeco2BuildMatchExit (&matcdat);
    memFree (treetab - 1);
    return  (1);
  }
#ifdef SCOTCH_DEBUG_ARCH2
  if (rootptr->domnsiz != vnumnbr) {
    errorPrint ("archDeco2ArchBuild: internal error (2)");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH2 */
  for (levlnum = matcdat.levlnum; levlnum >= 0; levlnum --) {
    ArchDeco2BuildLevl *  lewgptr;
    ArchDeco2Levl *       levlptr;

    lewgptr = &matcdat.lewgtab[levlnum];
    levlptr = &matcdat.levltab[levlnum];
    if (lewgptr->vewgtab != NULL) {
      memCpy (levlptr->grafdat.velotax + levlptr->grafdat.baseval, lewgptr->vewgtab, levlptr->grafdat.vertnbr * sizeof (Gnum));
      levlptr->grafdat.velosum = lewgptr->vewgsum;
    }
    else {
      levlptr->grafdat.velotax = NULL;
      levlptr->grafdat.velosum = levlptr->grafdat.vertnbr;
    }
    if (lewgptr->edwgtab != NULL) {
      memCpy (levlptr->grafdat.edlotax + levlptr->grafdat.baseval, lewgptr->edwgtab, levlptr->grafdat.edgenbr * sizeof (Gnum));
      levlptr->grafdat.edlosum = lewgptr->edwgsum;
      memFree (lewgptr->edwgtab);
      lewgptr->edwgtab = NULL;
    }
    else {
      levlptr->grafdat.edlotax = NULL;
      levlptr->grafdat.edlosum = levlptr->grafdat.edgenbr;
    }
#ifdef SCOTCH_DEBUG_ARCH2
    if (graphCheck (&levlptr->grafdat) != 0) {
      errorPrint ("archDeco2ArchBuild: internal error (3)");
      return     (1);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */
    if ((levlptr->wdiaval = graphDiamPV (&levlptr->grafdat)) < 0) {
      errorPrint ("archDeco2ArchBuild: cannot compute graph diameter");
      return     (1);
    }
    if (levlptr->wdiaval < 1)                     /* No division by zero allowed */
      levlptr->wdiaval = 1;
  }

  if ((vnumtmp = memAlloc (matcdat.vertsum * sizeof (Gnum))) == NULL) {
    errorPrint ("archDeco2ArchBuild: out of memory (4)");
    archDeco2BuildMatchExit (&matcdat);
    memFree (treetab - 1);
    return  (1);
  }
  decoptr->vnumnbr = matcdat.vertsum;
  decoptr->vnumtab = vnumtmp;

  datadat.domntab = domntab;
  datadat.doextab = doextab;
  datadat.termtab = termtab;
  datadat.vnumtab = vnumtmp;
  datadat.vnumidx = 0;                            /* Vertex number index array is empty */
  if (archDeco2BuildBuild3 (&datadat, rootptr, 1, 0, matcdat.levlnum) != domnnbr) {
    errorPrint ("archDeco2ArchBuild: cannot create sub-architecture (2)");
    memFree    (treetab - 1);
    return     (1);
  }
  domntab[0].dfatidx = -1;                        /* Set index of root father as -1 */
#ifdef SCOTCH_DEBUG_ARCH2
  if (datadat.vnumidx != matcdat.vertsum) {
    errorPrint ("archDeco2ArchBuild: internal error (3)");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH2 */

  for (treetab --; treetab != NULL; treetab = treetmp) { /* Free all tree blocks    */
    treetmp = treetab[0].sonstab[0];              /* Get pointer to next tree block */
    memFree (treetab);
  }

  decoptr->levlmax = matcdat.levlnum;             /* Save maximum level number                      */
  decoptr->levltab = matcdat.levltab;             /* Reuse graph array for architecture             */
  matcdat.levltab  = NULL;                        /* Will not be freed in archDeco2BuildMatchExit() */

  archDeco2BuildMatchExit (&matcdat);             /* Free data structures used for matching */

  return (0);
}
