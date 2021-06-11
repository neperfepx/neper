/* Copyright 2015-2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : arch_deco2.c                            **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the type-2          **/
/**                decomposition-defined target            **/
/**                architecture.                           **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 30 apr 2015     **/
/**                                 to   : 15 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ARCH_DECO2

#include "module.h"
#include "common.h"
#include "graph.h"
#include "arch.h"
#include "arch_deco2.h"
#include "arch_sub.h"

/***************************************/
/*                                     */
/* These are the type-2 decomposition- */
/* defined architecture routines.      */
/*                                     */
/***************************************/

/* This routine frees the decomposition
** architecture structures.
** It returns:
** - 0   : if the decomposition has been successfully freed.
** - !0  : on error.
*/

int
archDeco2ArchFree (
ArchDeco2 * const           archptr)
{
  ArchDeco2Levl *     levltab;

  if (archptr->vnumtab != NULL)
    memFree (archptr->vnumtab);

  if ((levltab = archptr->levltab) != NULL) {
    ArchDeco2Levl *     levlptr;

    for (levlptr = levltab + archptr->levlmax; levlptr >= levltab; levlptr --)
      graphExit (&levlptr->grafdat);

    memFree (levltab);
  }

  if (archptr->termtab != NULL)                   /* Free group leader */
    memFree (archptr->termtab);

  return (0);
}

/* This routine loads and computes the version-2,
** decomposition-defined architecture tables.
** It is called by archDecoArchLoad.
** It returns:
** - 0   : if the decomposition has been successfully read.
** - !0  : on error.
*/

int
archDeco2ArchLoad2 (
ArchDeco2 * restrict const  archptr,
FILE * restrict const       stream)
{
  ArchSubTerm * restrict    termtab;
  Anum                      termnbr;
  Anum                      termnum;
  ArchSubData * restrict    domntab;
  Anum                      domnnbr;
  Anum                      domnnum;
  ArchDeco2Data * restrict  doextab;
  ArchDeco2Levl * restrict  levltab;
  Anum                      levlnbr;
  Anum                      levlnum;
  Anum * restrict           vnumtab;
  Anum                      vnummax;
  Anum                      vnumnbr;
  Anum                      vnumnum;
  Gnum                      vertnbr;

#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchDeco2)    > sizeof (ArchDummy)) ||
      (sizeof (ArchDeco2Dom) > sizeof (ArchDomDummy))) {
    errorPrint ("archDeco2ArchLoad2: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if ((intLoad (stream, &archptr->termnbr) != 1) ||
      (intLoad (stream, &archptr->levlmax) != 1) || /* Read levlnbr, not lvlmax */
      (intLoad (stream, &archptr->vnumnbr) != 1) ||
      (archptr->termnbr < 1) ||
      (archptr->levlmax <= 0)) {
    errorPrint ("archDeco2ArchLoad2: bad input (1)");
    return     (1);
  }
  archptr->termtab = NULL;                        /* Assume nothing allocated yet */
  archptr->vnumtab = NULL;
  archptr->levltab = NULL;

  termnbr = archptr->termnbr;
  domnnbr = 2 * termnbr - 1;
  levlnbr = archptr->levlmax --;                  /* Get levlmax */
  if (memAllocGroup ((void **) (void *)
                     &archptr->termtab, (size_t) (termnbr * sizeof (ArchSubTerm)),
                     &archptr->domntab, (size_t) (domnnbr * sizeof (ArchSubData)),
                     &archptr->doextab, (size_t) (domnnbr * sizeof (ArchDeco2Data)), NULL) == NULL) {
    errorPrint ("archDeco2ArchLoad2: out of memory (1)");
    return     (1);
  }
  termtab = archptr->termtab;
  domntab = archptr->domntab;
  doextab = archptr->doextab;

  for (termnum = 0; termnum < termnbr; termnum ++) { /* For all declared terminals */
    if ((intLoad (stream, &termtab[termnum].domnidx) != 1) ||
        (intLoad (stream, &termtab[termnum].termnum) != 1) ||
        (termtab[termnum].domnidx < 0)        ||
        (termtab[termnum].domnidx >= domnnbr) ||
        (termtab[termnum].termnum < 0)) {
      errorPrint ("archDeco2ArchLoad2: bad input (2)");
      goto abort;
    }
  }

  vnummax = -1;
  for (domnnum = 0; domnnum < domnnbr; domnnum ++) { /* For all declared domains */
    if ((intLoad (stream, &domntab[domnnum].domnnum)    != 1) ||
        (intLoad (stream, &domntab[domnnum].domnsiz)    != 1) ||
        (intLoad (stream, &domntab[domnnum].domnwgt)    != 1) ||
        (intLoad (stream, &domntab[domnnum].termnum)    != 1) ||
        (intLoad (stream, &domntab[domnnum].dfatidx)    != 1) ||
        (intLoad (stream, &domntab[domnnum].dsubidx[0]) != 1) ||
        (intLoad (stream, &domntab[domnnum].dsubidx[1]) != 1) ||
        (intLoad (stream, &doextab[domnnum].levlnum)    != 1) ||
        (intLoad (stream, &doextab[domnnum].vnumidx)    != 1) ||
        (domntab[domnnum].domnnum < 0)           ||
        (domntab[domnnum].domnsiz < 1)           ||
        (domntab[domnnum].domnwgt < 1)           ||
        (domntab[domnnum].termnum < 0)           ||
        (domntab[domnnum].termnum >= termnbr)    ||
        (domntab[domnnum].dfatidx < -1)          ||
        (domntab[domnnum].dfatidx >= domnnbr)    ||
        (domntab[domnnum].dsubidx[0] < -1)       ||
        (domntab[domnnum].dsubidx[0] >= domnnbr) ||
        (domntab[domnnum].dsubidx[1] < -1)       ||
        (domntab[domnnum].dsubidx[1] >= domnnbr) ||
        (doextab[domnnum].levlnum < 0)           ||
        (doextab[domnnum].levlnum >= levlnbr)    ||
        (doextab[domnnum].vnumidx < 0)) {
      errorPrint ("archDeco2ArchLoad2: bad input (3)");
      goto abort;
    }
    if (doextab[domnnum].vnumidx > vnummax)       /* Keep track of max index */
      vnummax = doextab[domnnum].vnumidx;
  }

  if ((levltab = memAlloc (levlnbr * sizeof (ArchDeco2Levl))) == NULL) {
    errorPrint ("archDeco2ArchLoad2: out of memory (2)");
    goto abort;
  }
  archptr->levltab = levltab;

  for (levlnum = 0; levlnum < levlnbr; levlnum ++) {
    if (graphLoad (&levltab[levlnum].grafdat, stream, -1, 0) != 0) {
      errorPrint ("archDeco2ArchLoad2: bad input (4)");
      archptr->levlmax = levlnum - 1;             /* Only free existing levels */
      goto abort;
    }
    if (intLoad (stream, &levltab[levlnum].wdiaval) != 1) { /* Read graph diameter */
      errorPrint ("archDeco2ArchLoad2: bad input (5)");
      archptr->levlmax = levlnum;                 /* Free this level as well */
      goto abort;
    }
  }
  archptr->levlmax = levlnbr - 1;                 /* All levels have been read */
  archptr->baseval = levltab[0].grafdat.baseval;  /* Get base value            */

  vnumnbr = archptr->vnumnbr;
  if (vnummax >= vnumnbr) {
    errorPrint ("archDeco2ArchLoad2: bad input (6)");
    goto abort;
  }

  if ((vnumtab = memAlloc (vnumnbr * sizeof (Anum))) == NULL) {
    errorPrint ("archDeco2ArchLoad2: out of memory (3)");
    goto abort;
  }
  archptr->vnumtab = vnumtab;

  vertnbr = levltab[0].grafdat.vertnbr;           /* Size of largest graph     */
  for (vnumnum = 0; vnumnum < vnumnbr; vnumnum ++) { /* For all vertex indices */
    if ((intLoad (stream, &vnumtab[vnumnum]) != 1) ||
        (vnumtab[vnumnum] < 0)                     ||
        (vnumtab[vnumnum] >= vertnbr)) {
      errorPrint ("archDeco2ArchLoad2: bad input (7)");
      goto abort;
    }
  }

#ifdef SCOTCH_DEBUG_ARCH1
  for (domnnum = 0; domnnum < domnnbr; domnnum ++) {
    if (doextab[domnnum].vnumidx > vnumnbr) {
      errorPrint ("archDeco2ArchLoad2: bad input (8)");
      goto abort;
    }
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  return (0);

abort:
  archDeco2ArchFree (archptr);
  return (1);
}

/* This routine saves the given target architecture
** as compiled decomposition tables.
** It returns:
** - 0   : if the decomposition has been successfully written.
** - !0  : on error.
*/

int
archDeco2ArchSave (
const ArchDeco2 * restrict const  archptr,
FILE * restrict const             stream)
{
  Anum                termnum;
  Anum                domnnum;
  Anum                levlnum;
  Anum                vnumnum;
  int                 o;

  const Anum                            termnbr = archptr->termnbr;
  const ArchSubTerm * restrict const    termtab = archptr->termtab;
  const Anum                            domnnbr = archptr->domnnbr;
  const ArchSubData * restrict const    domntab = archptr->domntab;
  const ArchDeco2Data * restrict const  doextab = archptr->doextab;
  const Anum                            levlmax = archptr->levlmax;
  const ArchDeco2Levl * restrict const  levltab = archptr->levltab;
  const Anum                            vnumnbr = archptr->vnumnbr;
  const Anum * restrict const           vnumtab = archptr->vnumtab;

#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchDeco2)    > sizeof (ArchDummy)) ||
      (sizeof (ArchDeco2Dom) > sizeof (ArchDomDummy))) {
    errorPrint ("archDeco2ArchSave: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if (fprintf (stream, "2\n" ANUMSTRING "\t" ANUMSTRING "\t" ANUMSTRING "\n", /* Write file header */
               (Anum) archptr->termnbr,
               (Anum) (archptr->levlmax + 1),
               (Anum) archptr->vnumnbr) == EOF) {
    errorPrint ("archDeco2ArchSave: bad output (1)");
    return     (1);
  }

  for (termnum = 0; termnum < termnbr; termnum ++) {
    if (fprintf (stream, ANUMSTRING "\t" ANUMSTRING "\n",
                 (Anum) termtab[termnum].domnidx,
                 (Anum) termtab[termnum].termnum) == EOF) {
      errorPrint ("archDeco2ArchSave: bad output (2)");
      return     (1);
    }
  }

  for (domnnum = 0; domnnum < domnnbr; domnnum ++) { /* For all declared domains */
    if (fprintf (stream, ANUMSTRING "\t" ANUMSTRING "\t" ANUMSTRING "\t"
                         ANUMSTRING "\t" ANUMSTRING "\t" ANUMSTRING "\t"
                         ANUMSTRING "\t" ANUMSTRING "\t" ANUMSTRING "\n",
                 (Anum) domntab[domnnum].domnnum,
                 (Anum) domntab[domnnum].domnsiz,
                 (Anum) domntab[domnnum].domnwgt,
                 (Anum) domntab[domnnum].termnum,
                 (Anum) domntab[domnnum].dfatidx,
                 (Anum) domntab[domnnum].dsubidx[0],
                 (Anum) domntab[domnnum].dsubidx[1],
                 (Anum) doextab[domnnum].levlnum,
                 (Anum) doextab[domnnum].vnumidx) == EOF) {
      errorPrint ("archDeco2ArchSave: bad output (3)");
      return     (1);
    }
  }

  for (levlnum = 0; levlnum <= levlmax; levlnum ++) {
    if (graphSave (&levltab[levlnum].grafdat, stream) != 0) {
      errorPrint ("archDeco2ArchSave: bad output (4)");
      return     (1);
    }
    if (fprintf (stream, ANUMSTRING "\n",
                 (Anum) levltab[levlnum].wdiaval) == EOF) {
      errorPrint ("archDeco2ArchSave: bad output (5)");
      return     (1);
    }
  }

  o = 0;
  for (vnumnum = 0; vnumnum < (vnumnbr - 1); vnumnum ++) {
    if (fprintf (stream, ANUMSTRING "\t", (Anum) vnumtab[vnumnum]) == EOF) {
      o = 1;
      break;
    }
  }
  if (vnumnum < vnumnbr) {
    if (fprintf (stream, ANUMSTRING "\n", (Anum) vnumtab[vnumnum]) == EOF)
      o = 1;
  }
  if (o != 0) {
    errorPrint ("archDeco2ArchSave: bad output (6)");
    return     (1);
  }

  return (0);
}

/* This routine initializes the matching data
** structure according to the number of vertices
** to be managed.
** Its purpose is to help compute a "sub"
** architecture of a "deco2" architecture.
** Since "deco2" and "sub" architectures posess
** the same hierarchical domain structure, it
** can be re-used at no additional cost.
** It returns:
** - 0   : if the data structure has been
**         successfully initialized.
** - !0  : on error.
*/

int
archDeco2MatchInit (
ArchSubMatch * restrict const     matcptr,        /* Use "sub" matching data structure instead of propre one */
const ArchDeco2 * restrict const  archptr)
{
  Anum                termnbr;
  Anum                termnum;
  Anum                domnmax;
  Anum                levlnbr;
  Anum                multnbr;

  const ArchSubTerm * restrict const  termtab = archptr->termtab;
  const ArchSubData * restrict const  domntab = archptr->domntab;

  for (termnum = 0, termnbr = archptr->termnbr, domnmax = 0; /* Get maximum domain number */
       termnum < termnbr; termnum ++) {
    Anum                domnval;

    domnval = domntab[termtab[termnum].domnidx].domnnum;
    if (domnval > domnmax)
      domnmax = domnval;
  }

  for (levlnbr = 1; domnmax > 0; levlnbr ++, domnmax >>= 1) ; /* Compute maximum recursion level in bipartitioning tree */

  multnbr = 1 << levlnbr;                         /* Maximum number of multinodes is the size of last level */
  if ((matcptr->multtab = memAlloc (multnbr * sizeof (ArchCoarsenMulti))) == NULL) {
    errorPrint ("archDeco2MatchInit: out of memory");
    return     (1);
  }

  matcptr->domntab = archptr->domntab;            /* Keep pointer to sub-architecture domain array */
  matcptr->levlnum =
  matcptr->levlmax = levlnbr - 1;

  return (0);
}

/* This function returns the smallest number
** of terminal domain included in the given
** domain.
*/

ArchDomNum
archDeco2DomNum (
const ArchDeco2 * const     archptr,
const ArchDeco2Dom * const  domnptr)
{
  return (archptr->domntab[domnptr->domnidx].termnum + archptr->baseval); /* Return based terminal number from un-based number */
}

/* This function returns the terminal domain associated
** with the given terminal number in the architecture.
** It returns:
** - 0  : if label is valid and domain has been updated.
** - 1  : if label is invalid.
** - 2  : on error.
*/

int
archDeco2DomTerm (
const ArchDeco2 * const     archptr,
ArchDeco2Dom * const        domnptr,
const ArchDomNum            termnum)
{
  Gnum                termtmp;

  termtmp = termnum - archptr->baseval;                    /* Un-base terminal number      */
  if ((termtmp < 0) ||                                     /* Check if it was not un-based */
      (termtmp >= archptr->termnbr))
    return (1);

  domnptr->domnidx = archptr->termtab[termtmp].domnidx;

  return (0);
}

/* This function returns the number of
** elements in the given domain.
*/

Anum
archDeco2DomSize (
const ArchDeco2 * const     archptr,
const ArchDeco2Dom * const  domnptr)
{
  return (archptr->domntab[domnptr->domnidx].domnsiz);
}

/* This function returns the weight of
** the given domain.
*/

Anum
archDeco2DomWght (
const ArchDeco2 * const     archptr,
const ArchDeco2Dom * const  domnptr)
{
  return (archptr->domntab[domnptr->domnidx].domnwgt);
}

/* This function returns the average distance
** between two domains, which is extracted
** from the table.
*/

Anum
archDeco2DomDist (
const ArchDeco2 * const     archptr,
const ArchDeco2Dom * const  dom0ptr,
const ArchDeco2Dom * const  dom1ptr)
{
  Anum                  levlnum;
  Anum                  levlnum0;
  Anum                  levlnum1;
  Anum                  domnidx0;
  Anum                  domnidx1;
  const ArchDeco2Levl * levlptr;
  Gnum * restrict       queutab;
  Gnum * restrict       disttax;
  Anum                  distmin;
  Gnum                  baseval;
  Gnum                  vertnbr;
#ifdef SCOTCH_DEBUG_ARCH2
  Anum                  domnnum0;
  Anum                  domnnum1;
#endif /* SCOTCH_DEBUG_ARCH2 */

  const ArchSubData * restrict const    domntab = archptr->domntab;
  const ArchDeco2Data * restrict const  doextab = archptr->doextab;
  const Gnum * restrict const           vnumtab = archptr->vnumtab;

  domnidx0 = dom0ptr->domnidx;
  domnidx1 = dom1ptr->domnidx;
  if (domnidx0 == domnidx1)                       /* If same domains, zero distance */
    return (0);

  levlnum0 = doextab[domnidx0].levlnum;
  levlnum1 = doextab[domnidx1].levlnum;
  levlnum  = MAX (levlnum0, levlnum1);            /* Level where to start distance computations    */
  levlptr  = &archptr->levltab[levlnum];          /* Point to start level for distance computation */
  vertnbr  = levlptr->grafdat.vertnbr;            /* Get upper bound on number of vertices         */
#ifdef SCOTCH_DEBUG_ARCH2
  if (vertnbr <= 1) {                             /* If coarsest graph hit without exiting before */
    errorPrint ("archDeco2DomDist: internal error (1)");
    return     (-1);
  }
#endif /* SCOTCH_DEBUG_ARCH2 */

#ifdef SCOTCH_DEBUG_ARCH2
  domnnum0 = domntab[domnidx0].domnnum;
  domnnum1 = domntab[domnidx1].domnnum;
  if ((domntab[domnidx0].domnnum != domnnum0) ||
      (domntab[domnidx1].domnnum != domnnum1)) {
    errorPrint ("archDeco2DomDist: internal error (2)");
    return     (-1);
  }
#endif /* SCOTCH_DEBUG_ARCH2 */
  while (levlnum0 < levlnum1) {                   /* Level subdomains as much as possible */
    Anum                domntmp0;
    Anum                levltmp0;

    domntmp0 = domntab[domnidx0].dfatidx;
    levltmp0 = doextab[domntmp0].levlnum;
    if (levltmp0 > levlnum1)                      /* If cannot find coarser subdomain */
      break;

    domnidx0 = domntmp0;                          /* Father domain becomes current domain */
    levlnum0 = levltmp0;                          /* Set new current level for new domain */
#ifdef SCOTCH_DEBUG_ARCH2
    domnnum0 >>= 1;
    if ((doextab[domnidx0].levlnum != levlnum0) ||
        (domntab[domnidx0].domnnum != domnnum0)) {
      errorPrint ("archDeco2DomDist: internal error (3)");
      return     (-1);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */
  }
  while (levlnum1 < levlnum0) {                   /* Level subdomains as much as possible */
    Anum                domntmp1;
    Anum                levltmp1;

    domntmp1 = domntab[domnidx1].dfatidx;
    levltmp1 = doextab[domntmp1].levlnum;
    if (levltmp1 > levlnum0)                      /* If cannot find coarser subdomain */
      break;

    domnidx1 = domntmp1;
    levlnum1 = levltmp1;
#ifdef SCOTCH_DEBUG_ARCH2
    domnnum1 >>= 1;
    if ((doextab[domnidx1].levlnum != levlnum1) ||
        (domntab[domnidx1].domnnum != domnnum1)) {
      errorPrint ("archDeco2DomDist: internal error (4)");
      return     (-1);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */
  }

  if (domnidx0 == domnidx1)                       /* If same domains after leveling, half distance of largest domain */
    return (levlptr->grafdat.velotax[vnumtab[domnidx0]]);

  if (memAllocGroup ((void **) (void *)
                     &queutab, (size_t) ((vertnbr + ARCHDECO2PASSNBR + 1) * sizeof (Anum)), /* Room for pass markers */
                     &disttax, (size_t) (vertnbr * sizeof (Anum)), NULL) == NULL) {
    errorPrint ("archDeco2DomDist: out of memory");
    return     (0);                               /* Nothing to free because group allocation failed */
  }
  baseval  = levlptr->grafdat.baseval;
  disttax -= baseval;

  while (1) {                                     /* As long as coarsest graph not hit */
    const Gnum * restrict         verttax;
    const Gnum * restrict         vendtax;
    const Gnum * restrict         velotax;
    const Gnum * restrict         edgetax;
    const Gnum * restrict         edlotax;
    Gnum                          vertnum0;
    Gnum                          vertnum1;
    Anum                          domntmp0;
    Anum                          domntmp1;
    Gnum                          queuheadidx;
    Gnum                          queutailidx;
    int                           passnbr;
    int                           flagval;

    vertnum0 = vnumtab[doextab[domnidx0].vnumidx - (levlnum - levlnum0)]; /* Find vertices at proper level for domain indices */
    vertnum1 = vnumtab[doextab[domnidx1].vnumidx - (levlnum - levlnum1)];
#ifdef SCOTCH_DEBUG_ARCH2
    if ((vertnum0 < 0) || (vertnum0 >= vertnbr) ||
        (vertnum1 < 0) || (vertnum1 >= vertnbr)) {
      errorPrint ("archDeco2DomDist: internal error (5)");
      return     (-1);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */

    distmin = GNUMMAX;
    flagval = 1;

    verttax = levlptr->grafdat.verttax;
    vendtax = levlptr->grafdat.vendtax;
    velotax = levlptr->grafdat.velotax;
    edgetax = levlptr->grafdat.edgetax;
    edlotax = levlptr->grafdat.edlotax;

    memSet (disttax + baseval, ~0, vertnbr * sizeof (Gnum)); /* Set flag and distance array to -1 */
    disttax[vertnum0] = (velotax != NULL)
                        ? ((velotax[vertnum0] + velotax[vertnum1]) / 2) /* Account for half of traversal costs of end vertices */
                        : 0;

    passnbr     = ARCHDECO2PASSNBR;
    queutab[0]  = vertnum0;
    queutab[1]  = -1;                             /* Enqueue pass marker as end of pass 0 */
    queutailidx = 0;
    queuheadidx = 2;
    while (queutailidx != queuheadidx) {
      Gnum                vertnum;
      Gnum                edgenum;
      Gnum                edgennd;
      Gnum                distval;

loop: vertnum = queutab[queutailidx ++];          /* Get vertex number from queue        */
      if (vertnum < 0) {                          /* If vertex is pass marker            */
        if (passnbr == 0)                         /* If vertex found or passes exhausted */
          break;
        queutab[queuheadidx ++] = vertnum;        /* Re-enqueue pass marker  */
        passnbr --;                               /* One more pass completed */

        goto loop;                                /* Get another vertex number from queue */
      }

      distval = disttax[vertnum];
      for (edgenum = verttax[vertnum], edgennd = vendtax[vertnum];
           edgenum < edgennd; edgenum ++) {
        Gnum                vertend;
        Gnum                disttmp;

        vertend = edgetax[edgenum];
        disttmp = distval + ((edlotax != NULL) ? edlotax[edgenum] : 1);

        if (vertend == vertnum1) {                /* If path to end vertex found */
          if (disttmp < distmin)
            distmin = disttmp;
          passnbr = 0;                            /* Abort at end of this pass       */
          flagval = 0;                            /* Record path found for this pass */
        }
        else {
          disttmp += (velotax != NULL) ? velotax[vertend] : 0; /* Vertex will have to be traversed fully */

          if (disttax[vertend] < 0) {             /* If vertex not already considered */
            queutab[queuheadidx ++] = vertend;    /* Enqueue new vertex               */
            disttax[vertend] = disttmp;
          }
          else if (disttax[vertend] > disttmp)    /* If path improves distance at this pass */
            disttax[vertend] = disttmp;
        }
      }
    }

    if (flagval == 0)                             /* If path found, don't search in coarser graphs */
      break;

    levlnum ++;
    levlptr ++;                                   /* Consider coarser level     */
    vertnbr = levlptr->grafdat.vertnbr;           /* Reduce size of work arrays */

    domntmp0 = domntab[domnidx0].dfatidx;         /* For both subdomains independently    */
    if (doextab[domntmp0].levlnum <= levlnum) {   /* If father domain is at proper level  */
      domnidx0 = domntmp0;                        /* Father domain becomes current domain */
      levlnum0 = levlnum;                         /* Set new current level for new domain */
#ifdef SCOTCH_DEBUG_ARCH2
      domnnum0 >>= 1;
      if ((doextab[domnidx0].levlnum != levlnum0) ||
          (domntab[domnidx0].domnnum != domnnum0)) {
        errorPrint ("archDeco2DomDist: internal error (6)");
        return     (-1);
      }
#endif /* SCOTCH_DEBUG_ARCH2 */
    }
    domntmp1 = domntab[domnidx1].dfatidx;
    if (doextab[domntmp1].levlnum <= levlnum) {
      domnidx1 = domntmp1;
      levlnum1 = levlnum;
#ifdef SCOTCH_DEBUG_ARCH2
      domnnum1 >>= 1;
      if ((doextab[domnidx1].levlnum != levlnum1) ||
          (domntab[domnidx1].domnnum != domnnum1)) {
        errorPrint ("archDeco2DomDist: internal error (7)");
        return     (-1);
      }
#endif /* SCOTCH_DEBUG_ARCH2 */
    }

    if (domnidx0 == domnidx1) {                   /* If same domains after leveling, half distance of largest domain */
      distmin = levlptr->grafdat.velotax[vnumtab[domnidx0]];
      break;
    }
#ifdef SCOTCH_DEBUG_ARCH2
    if (vertnbr <= 1) {                           /* If coarsest graph hit without exiting before */
      errorPrint ("archDeco2DomDist: internal error (8)");
      return     (-1);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */
  }

  memFree (queutab);                              /* Free group leader */

  return (distmin);
}

/* This function sets the biggest
** domain available for this
** architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archDeco2DomFrst (
const ArchDeco2 * const       archptr,
ArchDeco2Dom * restrict const domnptr)
{
  domnptr->domnidx = 0;                           /* First domain index is slot 0 */

  return (0);
}

/* This routine reads domain information
** from the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archDeco2DomLoad (
const ArchDeco2 * const       archptr,
ArchDeco2Dom * restrict const domnptr,
FILE * restrict const         stream)
{
  if ((intLoad (stream, &domnptr->domnidx) != 1) ||
      (domnptr->domnidx < 0)                     ||
      (domnptr->domnidx >= archptr->domnnbr)) {
    errorPrint ("archDeco2DomLoad: bad input");
    return     (1);
  }

  return (0);
}

/* This routine saves domain information
** to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archDeco2DomSave (
const ArchDeco2 * const     archptr,
const ArchDeco2Dom * const  domnptr,
FILE * restrict const       stream)
{
  if (fprintf (stream, ANUMSTRING " ",
               (Anum) domnptr->domnidx) == EOF) {
    errorPrint ("archDeco2DomSave: bad output");
    return     (1);
  }

  return (0);
}

/* This function tries to split a
** decomposition domain into two
** subdomains.
** It returns:
** - 0  : if bipartitioning succeeded.
** - 1  : if bipartitioning could not be performed.
** - 2  : on error.
*/

int
archDeco2DomBipart (
const ArchDeco2 * const       archptr,
const ArchDeco2Dom * const    domnptr,
ArchDeco2Dom * restrict const dom0ptr,
ArchDeco2Dom * restrict const dom1ptr)
{
  Anum                dom0idx;

  dom0idx = archptr->domntab[domnptr->domnidx].dsubidx[0];
  if (dom0idx < 0)                                /* If domain is terminal */
    return (1);

  dom0ptr->domnidx = dom0idx;
  dom1ptr->domnidx = archptr->domntab[domnptr->domnidx].dsubidx[1];

  return (0);
}

/* This function checks if dom1 is
** included in dom0.
** It returns:
** - 0  : if dom1 is not included in dom0.
** - 1  : if dom1 is included in dom0.
** - 2  : on error.
*/

int
archDeco2DomIncl (
const ArchDeco2 * const     archptr,
const ArchDeco2Dom * const  dom0ptr,
const ArchDeco2Dom * const  dom1ptr)
{
  Anum          dom0num;
  Anum          dom1num;

  for (dom1num = archptr->domntab[dom1ptr->domnidx].domnnum,
       dom0num = archptr->domntab[dom0ptr->domnidx].domnnum;
       dom1num != 0; dom1num >>= 1)
    if (dom1num == dom0num)
      return (1);

  return (0);
}

/* This function creates the MPI_Datatype for
** decomposition-described domains.
** It returns:
** - 0  : if type could be created.
** - 1  : on error.
*/

#ifdef SCOTCH_PTSCOTCH
int
archDeco2DomMpiType (
const ArchDeco2 * const       archptr,
MPI_Datatype * const          typeptr)
{
  *typeptr = ANUM_MPI;

  return (0);
}
#endif /* SCOTCH_PTSCOTCH */
