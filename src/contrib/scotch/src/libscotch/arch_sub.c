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
/**   NAME       : arch_sub.c                              **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the sub-            **/
/**                architecture meta-architecture          **/
/**                functions.                              **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 13 mar 2015     **/
/**                                 to   : 15 may 2018     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define ARCH_SUB

#include "module.h"
#include "common.h"
#include "arch.h"
#include "arch_sub.h"

/**********************************/
/*                                */
/* These are the sub-architecture */
/* meta-architecture routines.    */
/*                                */
/**********************************/

/* This routine loads the distance
** graph architecture.
** It returns:
** - 0   : if the architecture has been successfully read.
** - !0  : on error.
*/

int
archSubArchLoad (
ArchSub * restrict const    subarchptr,
FILE * restrict const       stream)
{
  Arch *              orgarchptr;
  Anum                termnbr;
  Anum                termnum;
  Anum * restrict     termtab;
  int                 o;

#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchSub)    > sizeof (ArchDummy)) ||
      (sizeof (ArchSubDom) > sizeof (ArchDomDummy))) {
    errorPrint ("archSubArchLoad: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if (intLoad (stream, &termnbr) != 1) {
    errorPrint ("archSubArchLoad: bad input (1)");
    return     (1);
  }

  if ((termtab = memAlloc (termnbr * sizeof (Anum))) == NULL) {
    errorPrint ("archSubArchLoad: out of memory (1)");
    return     (1);
  }

  for (termnum = 0; termnum < termnbr; termnum ++) {
    if (intLoad (stream, &termtab[termnum]) != 1) {
      errorPrint ("archSubArchLoad: bad input (2)");
      memFree    (termtab);
      return     (1);
    }
  }

  if ((orgarchptr = memAlloc (sizeof (Arch))) == NULL) {
    errorPrint ("archSubArchLoad: out of memory (2)");
    memFree    (termtab);
    return     (1);
  }

  o = archLoad (orgarchptr, stream);
  if (o == 0)
    o = archSubArchBuild (subarchptr, orgarchptr, termnbr, termtab);
  if (o != 0) {
    archExit (orgarchptr);
    memFree  (orgarchptr);
  }

  memFree (termtab);

  return (o);
}

/* This routine saves the
** distance graph architecture.
** It returns:
** - 0   : if the architecture has been successfully written.
** - !0  : on error.
*/

int
archSubArchSave (
const ArchSub * const       archptr,
FILE * restrict const       stream)
{
  Anum                          termnbr;
  Anum                          termnum;
  const ArchSubTerm * restrict  termtab;

#ifdef SCOTCH_DEBUG_ARCH1
  if (sizeof (ArchSub) > sizeof (ArchDummy)) {
    errorPrint ("archSubArchSave: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  termnbr = archptr->termnbr;
  if (fprintf (stream, ANUMSTRING,
               (Anum) termnbr) == EOF) {
    errorPrint ("archSubArchSave: bad output (1)");
    return     (1);
  }

  for (termnum = 0, termtab = archptr->termtab;
       termnum < termnbr; termnum ++) {
    if (fprintf (stream, " " ANUMSTRING,
                 (Anum) termtab[termnum].termnum) == EOF) {
      errorPrint ("archSubArchSave: bad output (2)");
      return     (1);
    }
  }

  if (fprintf (stream, "\n") == EOF) {
    errorPrint ("archSubArchSave: bad output (3)");
    return     (1);
  }

  return (archSave (archptr->archptr, stream));
}

/* This routine frees the sub-architecture structures.
** It returns:
** - 0   : if the architecture has been successfully freed.
** - !0  : on error.
*/

int
archSubArchFree (
ArchSub * const             archptr)
{
#ifdef SCOTCH_DEBUG_ARCH1
  if ((sizeof (ArchSub)    > sizeof (ArchDummy)) ||
      (sizeof (ArchSubDom) > sizeof (ArchDomDummy))) {
    errorPrint ("archSubArchFree: invalid type specification");
    return     (1);
  }
#endif /* SCOTCH_DEBUG_ARCH1 */

  if ((archArch (archptr)->flagval & ARCHFREE) != 0) { /* If sub-architecture needs to be freed too */
    archFree (archptr->archptr);
    memFree  (archptr->archptr);
  }

  memFree (archptr->termtab);                     /* Free group leader */

  return (0);
}

/* This routine builds the recursive coarsening
** tree of an architecture. The coarsening is
** performed by applying a variable function that
** operates on a specific set of data.
** It returns:
** - !NULL  : root of the recursive coarsening tree.
** - NULL   : on error.
*/

ArchSubTree *
archSubArchBuild2 (
void *                      dataptr,
Anum                        funcptr (void *, ArchCoarsenMulti * restrict *),
ArchSubTree * const         finetreetab,
const Anum                  finevertnbr)
{
  ArchCoarsenMulti * restrict coarmulttab;
  ArchSubTree * restrict      coartreetab;
  Anum                        coarvertnbr;
  Anum                        coarvertnum;
  ArchSubTree *               rootptr;

  if (finevertnbr <= 1) {                         /* If recursion achieved, start building architecture */
    finetreetab[-1].sonstab[0] = NULL;            /* TRICK: no other block to link with                 */
    return (finetreetab);                         /* Return root of subdomain tree                      */
  }

  if ((coarvertnbr = funcptr (dataptr, &coarmulttab)) < 0) { /* Perform matching */
    errorPrint ("archSubArchBuild2: cannot create matching");
    return     (NULL);
  }

  if ((coartreetab = memAlloc ((coarvertnbr + 1) * sizeof (ArchSubTree))) == NULL) { /* TRICK: One more slot to link to coarser array */
    errorPrint ("archSubArchBuild2: out of memory");
    return     (NULL);
  }

  finetreetab[-1].sonstab[0] = coartreetab;       /* TRICK: Link to coarse array */
  coartreetab ++;                                 /* Hide first slot             */

  for (coarvertnum = 0; coarvertnum < coarvertnbr; coarvertnum ++) { /* Un-based coarse vertex indices */
    Anum                finevertnum0;
    Anum                finevertnum1;
    Anum                coardomnsiz;
    Anum                coardomnsiz1;
    Anum                coardomnwgt;
    Anum                coartermnum;
    ArchSubTree *       son0ptr;
    ArchSubTree *       son1ptr;

    finevertnum0 = coarmulttab[coarvertnum].vertnum[0]; /* Fine vertices must be un-based else mischief may occur */
    finevertnum1 = coarmulttab[coarvertnum].vertnum[1];

    coardomnsiz = finetreetab[finevertnum0].domnsiz;
    coardomnwgt = finetreetab[finevertnum0].domnwgt;
    coartermnum = finetreetab[finevertnum0].termnum;
    son0ptr = (coardomnsiz > 0) ? &finetreetab[finevertnum0] : NULL;
    son1ptr = NULL;

    if (finevertnum1 != finevertnum0) {           /* If multinode comprises two fine nodes */
      Anum                finetermnum1;

      coardomnsiz1 = finetreetab[finevertnum1].domnsiz; /* Add contributions from second node */
      coardomnsiz += coardomnsiz1;
      coardomnwgt += finetreetab[finevertnum1].domnwgt;
      finetermnum1 = finetreetab[finevertnum1].termnum;
      if (finetermnum1 < coartermnum)             /* If smaller terminal number found */
        coartermnum = finetermnum1;               /* Keep it                          */
      if (coardomnsiz1 > 0)
        son1ptr = &finetreetab[finevertnum1];
    }

    coartreetab[coarvertnum].domnsiz = coardomnsiz;
    coartreetab[coarvertnum].domnwgt = coardomnwgt;
    coartreetab[coarvertnum].termnum = coartermnum;
    coartreetab[coarvertnum].vertnum = coarvertnum; /* Un-based multinode number  */
    coartreetab[coarvertnum].sonstab[0] = son0ptr; /* Create links to son slot(s) */
    coartreetab[coarvertnum].sonstab[1] = son1ptr;
  }

  rootptr = archSubArchBuild2 (dataptr, funcptr, coartreetab, coarvertnbr);
  if (rootptr == NULL)                            /* On error, do not keep tree arrays */
    memFree (coartreetab - 1);

  return (rootptr);
}

/* This routine collapses the recursive coarsening tree
** into a decomposition-defined tree, by removing single
** vertices that have not been collapsed.
** It returns:
** - the first free index in the domain array.
*/

static
Anum
archSubArchBuild3 (
ArchSubData * restrict const        domntab,      /*+ Domain array to fill [domnnbr]          +*/
ArchSubTerm * restrict const        termtab,      /*+ Terminal domain array to fill [termnbr] +*/
const ArchSubTree * restrict const  treeptr,      /*+ Pointer to current node being traversed +*/
const Anum                          domnnum,      /*+ Domain number of current node           +*/
const Anum                          domnidx)      /*+ Current first free slot in domain array +*/
{
  Anum                termnum;
  Anum                didxtmp;
  const ArchSubTree * son0ptr;
  const ArchSubTree * son1ptr;
  const ArchSubTree * sonsptr;

  son0ptr = treeptr->sonstab[0];
  son1ptr = treeptr->sonstab[1];
  sonsptr = NULL;

  if (son0ptr != NULL)                            /* If node is a single node, traverse it without any processing */
    sonsptr = son0ptr;
  if (son1ptr != NULL)
    sonsptr = (sonsptr != NULL) ? NULL : son1ptr;
  if (sonsptr != NULL)
    return (archSubArchBuild3 (domntab, termtab, sonsptr, domnnum, domnidx));

  domntab[domnidx].domnnum = domnnum;
  domntab[domnidx].domnsiz = treeptr->domnsiz;
  domntab[domnidx].domnwgt = treeptr->domnwgt;
  termnum                  =
  domntab[domnidx].termnum = treeptr->termnum;

  didxtmp = domnidx + 1;
  if (son0ptr != NULL) {                          /* If node is a branch node */
    Anum                domntmp;

    domntmp = domnnum * 2;                        /* Domain number of first son */
    domntab[didxtmp].dfatidx    = domnidx;
    domntab[domnidx].dsubidx[0] = didxtmp;
    didxtmp = archSubArchBuild3 (domntab, termtab, son0ptr, domntmp, didxtmp);
    domntab[didxtmp].dfatidx    = domnidx;
    domntab[domnidx].dsubidx[1] = didxtmp;
    didxtmp = archSubArchBuild3 (domntab, termtab, son1ptr, domntmp + 1, didxtmp);
  }
  else {
    domntab[domnidx].dsubidx[0] =
    domntab[domnidx].dsubidx[1] = -1;

    termtab[termnum].domnidx = domnidx;
  }

  return (didxtmp);
}

/* This routine builds the sub-architecture of an
** original architecture.
** It returns:
** - 0  : if the sub-architecture has been successfully built.
** - 1  : if the initial architecture does not support sub architectures.
** - 2  : on error.
*/

int
archSubArchBuild (
ArchSub * const             subarchptr,
Arch * const                orgarchptr,
const Anum                  vnumnbr,              /* Number of terminal domains in sub-architecture */
const Anum * const          vnumtab)              /* Ordered list of remaining terminal numbers     */
{
  Anum                    orgtermnbr;             /* Number of terminals in original graph */
  Anum                    orgtermnum;
  ArchDom                 orgdomndat;             /* Domain in original architecture       */
  ArchMatch               matcdat;                /* Data for successive matchings         */
  ArchSubTree *           rootptr;
  ArchSubTree * restrict  treetab;
  ArchSubTree * restrict  treetmp;
  ArchSubTerm * restrict  termtab;
  ArchSubData * restrict  domntab;
  Anum                    domnnbr;                /* Number of domains                     */
  Anum                    vnumnum;
#ifdef SCOTCH_DEBUG_ARCH1
  const char *            messptr;                /* Pointer to error message              */
#endif /* SCOTCH_DEBUG_ARCH1 */

  if ((orgarchptr->class->flagval & ARCHVAR) != 0) {
    errorPrint ("archSubArchBuild: variable-sized architectures not supported");
    return     (1);
  }
  if (orgarchptr->class->matchInit == NULL) {
    errorPrint ("archSubArchBuild: architecture not supported");
    return     (1);
  }

  archDomFrst (orgarchptr, &orgdomndat);          /* Get number of terminal domains in original architecture */
  orgtermnbr = archDomSize (orgarchptr, &orgdomndat);

#ifdef SCOTCH_DEBUG_ARCH1
  if ((vnumnbr < 0) || (vnumnbr > orgtermnbr)) {
    errorPrint ("archSubArchBuild: invalid number of terminal domains");
    return     (2);
  }
  messptr = NULL;
#endif /* SCOTCH_DEBUG_ARCH1 */

  domnnbr = 2 * vnumnbr - 1;
  if (memAllocGroup ((void **) (void *)
                     &termtab, (size_t) (vnumnbr * sizeof (ArchSubTerm)),
                     &domntab, (size_t) (domnnbr * sizeof (ArchSubData)), NULL) == NULL) {
    errorPrint ("archSubArchBuild: out of memory (1)");
    return     (2);
  }
  if ((treetab = memAlloc ((orgtermnbr + 1) * sizeof (ArchSubTree))) == NULL) { /* TRICK: One more slot to link to coarser array */
    errorPrint ("archSubArchBuild: out of memory (2)");
    memFree    (termtab);
    return     (2);
  }

  intRandInit ();                                 /* Initialize random generator for coarsening */

  if (orgarchptr->class->matchInit (&matcdat, &orgarchptr->data) != 0) { /* Initialize matching structure with original architecture data */
    errorPrint ("archSubArchBuild: cannot initialize matching structure");
    memFree    (treetab);
    memFree    (termtab);
    return     (2);
  }

  treetab ++;                                     /* TRICK: Hide first slot */

  subarchptr->archptr = orgarchptr;
  subarchptr->termnbr = vnumnbr;
  subarchptr->termtab = termtab;
  subarchptr->domnnbr = domnnbr;
  subarchptr->domntab = domntab;

  for (orgtermnum = 0; orgtermnum < orgtermnbr; orgtermnum ++) { /* Initialize values for all vertices of original architecture */
    treetab[orgtermnum].domnsiz =
    treetab[orgtermnum].domnwgt = 0;
    treetab[orgtermnum].termnum =
    treetab[orgtermnum].vertnum = ANUMMAX;
    treetab[orgtermnum].sonstab[0] =
    treetab[orgtermnum].sonstab[1] = NULL;
  }

  for (vnumnum = 0; vnumnum < vnumnbr; vnumnum ++) { /* For all kept terminal numbers         */
    Anum                orgtermnum;               /* Terminal number in original architecture */
#ifdef SCOTCH_DEBUG_ARCH2
    int                 o;
#endif /* SCOTCH_DEBUG_ARCH2 */

    orgtermnum = vnumtab[vnumnum];
#ifdef SCOTCH_DEBUG_ARCH1
    if ((orgtermnum < 0) || (orgtermnum >= orgtermnbr))
      messptr = "archSubArchBuild: invalid vertex number in vertex list";
    else if (treetab[orgtermnum].domnsiz != 0)
      messptr = "archSubArchBuild: duplicate vertex number in vertex list";
    if (messptr != NULL) {
      errorPrint (messptr);
      orgarchptr->class->matchExit (&matcdat);    /* Free matching structure */
      memFree (treetab - 1);
      memFree (termtab);
      return  (2);
    }
#endif /* SCOTCH_DEBUG_ARCH1 */
#ifdef SCOTCH_DEBUG_ARCH2
    o =
#endif /* SCOTCH_DEBUG_ARCH2 */
    archDomTerm (orgarchptr, &orgdomndat, orgtermnum);
#ifdef SCOTCH_DEBUG_ARCH2
    if (o != 0) {
      errorPrint ("archSubArchBuild: internal error (1)");
      return     (2);
    }
#endif /* SCOTCH_DEBUG_ARCH2 */
    termtab[vnumnum].termnum = vnumtab[vnumnum];  /* Record number of original terminal number */

    treetab[orgtermnum].domnsiz = 1;              /* Here lies one terminal vertex */
    treetab[orgtermnum].domnwgt = archDomWght (orgarchptr, &orgdomndat);
    treetab[orgtermnum].termnum = vnumnum;        /* Sub-architecture terminal number in original architecture */
    treetab[orgtermnum].vertnum = orgtermnum;     /* Vertex number in original architecture                    */
  }

  rootptr = archSubArchBuild2 (&matcdat, orgarchptr->class->matchMate, treetab, vnumnbr);

  orgarchptr->class->matchExit (&matcdat);        /* Free matching structure */

  if (rootptr == NULL) {
    errorPrint ("archSubArchBuild: cannot create sub-architecture (1)");
    memFree    (treetab - 1);
    return     (2);
  }
#ifdef SCOTCH_DEBUG_ARCH2
  if (rootptr->domnsiz != vnumnbr) {
    errorPrint ("archSubArchBuild: internal error (2)");
    return     (2);
  }
#endif /* SCOTCH_DEBUG_ARCH2 */

  if (archSubArchBuild3 (domntab, termtab, rootptr, 1, 0) != domnnbr) {
    errorPrint ("archSubArchBuild: cannot create sub-architecture (2)");
    memFree    (treetab - 1);
    return     (2);
  }
  domntab[0].dfatidx = -1;                        /* Set index of root father as -1 */

  for (treetab --; treetab != NULL; treetab = treetmp) { /* Free all tree blocks    */
    treetmp = treetab[0].sonstab[0];              /* Get pointer to next tree block */
    memFree (treetab);
  }

  return (0);
}

/* This routine initializes the matching
** data structure according to the number
** of vertices to be managed.
** It returns:
** - 0   : if the data structure has been
**         successfully initialized.
** - !0  : on error.
*/

int
archSubMatchInit (
ArchSubMatch * restrict const   matcptr,
const ArchSub * restrict const  archptr)
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
    errorPrint ("archSubMatchInit: out of memory");
    return     (1);
  }

  matcptr->domntab = archptr->domntab;            /* Keep pointer to sub-architecture domain array */
  matcptr->levlnum =
  matcptr->levlmax = levlnbr - 1;

  return (0);
}

/* This routine frees the matching data
** structure.
** It returns:
** - void  : in all cases.
*/

void
archSubMatchExit (
ArchSubMatch * restrict const   matcptr)
{
  memFree (matcptr->multtab);
}

/* This routine computes a matching from
** the current state of the matching structure.
** It returns:
** - >=0  : size of matching.
** - <0   : on error.
*/

static
void
archSubMatchMate2 (
ArchSubMatch * restrict const       matcptr,
const ArchSubData * restrict const  domnptr,
const Anum                          levlnum)
{
  const ArchSubData * restrict  domntab;
  Anum                          levlcur;

  levlcur = matcptr->levlnum;

  if (domnptr->dsubidx[0] == -1) {                /* If node is terminal */
    Anum                vertnum;
    Anum                multnum;

    ArchCoarsenMulti * restrict const multtab = matcptr->multtab;

    vertnum = (levlcur == matcptr->levlmax) ? domnptr->termnum : matcptr->vertnum ++; /* If last level, provide terminal number, else vertex number */
    multnum = matcptr->multnum ++;                /* One more multinode created   */
    multtab[multnum].vertnum[0] =                 /* In fact, it is a single node */
    multtab[multnum].vertnum[1] = vertnum;

    return;
  }

  domntab = matcptr->domntab;

  if (levlnum == levlcur) {                       /* If recursion achieved proper level */
    Anum                vertnum0;
    Anum                vertnum1;
    Anum                multnum;

    ArchCoarsenMulti * restrict const multtab = matcptr->multtab;

    if (levlcur == matcptr->levlmax) {            /* If last level, get terminal numbers */
      vertnum0 = domntab[domnptr->dsubidx[0]].termnum;
      vertnum1 = domntab[domnptr->dsubidx[1]].termnum;
    }
    else {                                        /* Else compute vertex numbers from previous level */
      vertnum0 = matcptr->vertnum;
      vertnum1 = vertnum0 + 1;
      matcptr->vertnum = vertnum0 + 2;
    }

    multnum = matcptr->multnum ++;                /* Create multinode */
    multtab[multnum].vertnum[0] = vertnum0;
    multtab[multnum].vertnum[1] = vertnum1;
  }
  else {                                          /* Else perform recursion */
    archSubMatchMate2 (matcptr, &domntab[domnptr->dsubidx[0]], levlnum + 1);
    archSubMatchMate2 (matcptr, &domntab[domnptr->dsubidx[1]], levlnum + 1);
  }
}

Anum
archSubMatchMate (
ArchSubMatch * restrict const       matcptr,
ArchCoarsenMulti ** restrict const  multptr)
{

  if (matcptr->levlnum < 0)                       /* If already reached root of tree, abort */
    return (-1);

  matcptr->multnum =                              /* Reset counters for current level */
  matcptr->vertnum = 0;
  archSubMatchMate2 (matcptr, &matcptr->domntab[0], 0); /* Build multinodes for current level */

  matcptr->levlnum --;                            /* Prepare to perform previous level */

  *multptr = matcptr->multtab;                    /* Always provide same mating array */

  return (matcptr->multnum);                      /* Return number of multinodes computed at this level */
}

/* This function returns the representative
** terminal domain number for the given domain.
*/

ArchDomNum
archSubDomNum (
const ArchSub * const       archptr,
const ArchSubDom * const    domnptr)
{
  return (archptr->domntab[domnptr->domnidx].termnum);
}

/* This function returns the terminal domain associated
** with the given terminal number in the architecture.
** It returns:
** - 0  : if label is valid and domain has been updated.
** - 1  : if label is invalid.
** - 2  : on error.
*/

int
archSubDomTerm (
const ArchSub * const       archptr,
ArchSubDom * const          domnptr,
const ArchDomNum            termnum)
{
  if (termnum >= archptr->termnbr)
    return (1);

  domnptr->domnidx = archptr->termtab[termnum].domnidx;

  return (0);
}

/* This function returns the number of
** elements in the domain.
*/

Anum
archSubDomSize (
const ArchSub * const       archptr,
const ArchSubDom * const    domnptr)
{
  return (archptr->domntab[domnptr->domnidx].domnsiz);
}

/* This function returns the weight of
** the given distance domain.
*/

Anum
archSubDomWght (
const ArchSub * const       archptr,
const ArchSubDom * const    domnptr)
{
  return (archptr->domntab[domnptr->domnidx].domnwgt);
}

/* This function returns the average
** distance between two distance
** subdomains.
*/

Anum
archSubDomDist (
const ArchSub * const       archptr,
const ArchSubDom * const    dom0ptr,
const ArchSubDom * const    dom1ptr)
{
  ArchDom             dom0dat;                    /* Domains of underlying architecture */
  ArchDom             dom1dat;

  archDomTerm (archptr->archptr, &dom0dat, archptr->termtab[archptr->domntab[dom0ptr->domnidx].termnum].termnum); /* Build corresponding subdomains in underlying architecture */
  archDomTerm (archptr->archptr, &dom1dat, archptr->termtab[archptr->domntab[dom1ptr->domnidx].termnum].termnum);

  return (archDomDist (archptr->archptr, &dom0dat, &dom1dat));
}

/* This function sets the biggest
** domain available for this
** architecture.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
archSubDomFrst (
const ArchSub * const       archptr,
ArchSubDom * restrict const domnptr)
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
archSubDomLoad (
const ArchSub * const       archptr,
ArchSubDom * restrict const domnptr,
FILE * const                stream)
{
  if ((intLoad (stream, &domnptr->domnidx) != 1) ||
      (domnptr->domnidx < 0)                     ||
      (domnptr->domnidx >= archptr->domnnbr)) {
    errorPrint ("archSubDomLoad: bad input");
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
archSubDomSave (
const ArchSub * const       archptr,
const ArchSubDom * const    domnptr,
FILE * const                stream)
{
  if (fprintf (stream, ANUMSTRING " ",
               (Anum) domnptr->domnidx) == EOF) {
    errorPrint ("archSubDomSave: bad output");
    return     (1);
  }

  return (0);
}

/* This function tries to split a distance
** graph domain into two subdomains.
** It returns:
** - 0  : if bipartitioning succeeded.
** - 1  : if bipartitioning could not be performed.
** - 2  : on error.
*/

int
archSubDomBipart (
const ArchSub * const       archptr,
const ArchSubDom * const    domnptr,
ArchSubDom * restrict const dom0ptr,
ArchSubDom * restrict const dom1ptr)
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
archSubDomIncl (
const ArchSub * const       archptr,
const ArchSubDom * const    dom0ptr,
const ArchSubDom * const    dom1ptr)
{
  Anum          dom0num;
  Anum          dom1num;

  for (dom1num = archptr->domntab[dom1ptr->domnidx].domnnum, dom0num = archptr->domntab[dom0ptr->domnidx].domnnum; dom1num != 0; dom1num >>= 1)
    if (dom1num == dom0num)
      return (1);

  return (0);
}

/* This function creates the MPI_Datatype for
** distance graph domains.
** It returns:
** - 0  : if type could be created.
** - 1  : on error.
*/

#ifdef SCOTCH_PTSCOTCH
int
archSubDomMpiType (
const ArchSub * const         archptr,
MPI_Datatype * const          typeptr)
{
  *typeptr = ANUM_MPI;

  return (0);
}
#endif /* SCOTCH_PTSCOTCH */
