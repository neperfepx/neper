/* Copyright 2007-2013,2018,2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : wgraph_part_fm.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Jun-Ho HER (v6.0)                       **/
/**                Charles-Edmond BICHOT (v5.1b)           **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module is the improved Fiduccia-   **/
/**                Mattheyses refinement routine for the   **/
/**                vertex overlapped graph partitioning.   **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 01 dec 2007     **/
/**                                 to   : 01 jul 2008     **/
/**                # Version 6.0  : from : 05 nov 2009     **/
/**                                 to   : 31 may 2018     **/
/**                # Version 6.1  : from : 30 jul 2020     **/
/**                                 to   : 26 aug 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define WGRAPH_PART_FM

/* #define SCOTCH_DEBUG_WGRAPH3 */

#include "module.h"
#include "common.h"
#include "gain.h"
#include "graph.h"
#include "wgraph.h"
#include "wgraph_part_gg.h"
#include "wgraph_part_fm.h"

/*
**  The static and global variables.
*/

static WgraphPartFmVertex   wgraphpartfmvertexdummy; /* Dummy graph vertex acting as sentinel for linked lists */

/*********************************/
/*                               */
/* Gain table handling routines. */
/*                               */
/*********************************/

/* This routine inserts the links of a given
** frontier vertex into the gain table. Each link
** corresponds to the move of the frontier vertex
** to one of the parts to which it already belongs,
** increasing the frontier by all the vertices that
** belong to other parts and are not already part
** of the frontier.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static
int
wgraphPartFmTablAdd (
GainTabl * restrict const             tablptr,    /*+ Gain table                                +*/
Wgraph * restrict const               grafptr,    /*+ Active graph                              +*/
WgraphPartFmVertex * restrict const   hashtab,
const Gnum                            hashmsk,
WgraphPartFmLinkData * restrict const linkptr,
WgraphPartFmPartList * restrict const nplstab,
const Gnum                            cplomin,    /*+ Minimum with respect to average part load +*/
const Gnum                            cplomax,    /*+ Maximum with respect to average part load +*/
WgraphPartFmVertex * const            vexxptr)
{
  Anum                npmipartnum;                /* Part number of lighter part around frontier vertex        */
  Gnum                npmiloadval;                /* Weight of lighter part around frontier vertex             */
  Anum                npmspartnum;                /* Part number of second-lighter part around frontier vertex */
  Anum                nplsidx;                    /* Chained list of neighboring parts around vertex           */
  Anum                nplsnbr;                    /* Number of parts around vertex                             */
  WgraphPartFmLink *  linktab;                    /* Array of links (may change after resizing)                */
  Gnum                linkidx;                    /* Index of first link structure for vertex                  */
  Gnum                gainval;
  Gnum                vertnum;
  Gnum                edgenum;

  const Gnum * restrict const verttax = grafptr->s.verttax;
  const Gnum * restrict const vendtax = grafptr->s.vendtax;
  const Gnum * restrict const velotax = grafptr->s.velotax;
  const Gnum * restrict const edgetax = grafptr->s.edgetax;
  const Gnum * restrict const parttax = grafptr->parttax;
  const Gnum * restrict const comploadtab = grafptr->compload;

  vertnum = vexxptr->vertnum;
  npmipartnum =                                   /* No parts of minimum load found yet */
  npmspartnum = -1;
  npmiloadval = GNUMMAX;                          /* TRICK: indicates no part found */
  nplsidx = -1;                                   /* No neighboring parts found yet */
  nplsnbr = 0;
  gainval = (velotax != NULL) ? - velotax[vertnum] : -1; /* Gain for frontier if frontier vertex leaves it */

  for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) {
    Gnum                  vertend;
    Gnum                  hashend;
    WgraphPartFmVertex *  vexxend;
    Anum                  partend;

    vertend = edgetax[edgenum];
    for (hashend = (vertend * WGRAPHPARTFMHASHPRIME) & hashmsk, vexxend = hashtab + hashend; ; /* Find slot of end vertex to get its part */
         hashend = (hashend + 1) & hashmsk, vexxend = hashtab + hashend) {
      if (vexxend->vertnum == vertend) {      /* If hash slot found     */
        partend = vexxend->partnum;           /* Get current part value */
        break;
      }
      if (vexxend->vertnum == ~0) {           /* If vertex not found    */
        partend = parttax[vertend];           /* Get initial part value */
#ifdef SCOTCH_DEBUG_WGRAPH2
        if (partend == -1) {                  /* If vertex was not found un hash table */
          errorPrint ("wgraphPartFmTablAdd: vertex not in frontier array");
          return (1);
        }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
        break;
      }
    }

    if (partend != -1) {                          /* If end vertex part not in frontier */
      Gnum                veloend;

      if (nplstab[partend].nextidx == -2) {       /* If part not yet considered */
        Gnum                comploadend;

        nplstab[partend].nextidx = nplsidx;       /* Link it for further processing */
        nplstab[partend].loadgainval = 0;         /* No gain recorded yet           */
        nplsidx = partend;
        nplsnbr ++;

        comploadend = comploadtab[partend];
        if (comploadend < npmiloadval) {          /* TRICK: if first part, or if found lighter part */
          npmspartnum = npmipartnum;
          npmipartnum = partend;
          npmiloadval = comploadend;
        }
        else                                      /* Else record part as second choice anyway */
          npmspartnum = partend;
      }

      veloend = (velotax != NULL) ? velotax[vertend] : 1;
      nplstab[partend].loadgainval += veloend;    /* Sum gains to frontier from part if vertex goes to another part    */
      gainval += veloend;                         /* Sum overall gains to frontier if frontier vertex goes to any part */
    }
  }

  while (wgraphPartFmLinkClaim (linkptr, nplsnbr)) { /* Make sure there are enough links available in array */
    if (wgraphPartFmLinkResize (linkptr, tablptr) != 0) {
      errorPrint ("wgraphPartFmTablAdd: cannot resize link array");
      return (1);
    }
  }

  linktab = linkptr->linktab;                     /* Get pointer to link array after potential resizing */

  linkidx = -1;                                   /* Assume list of links is empty                  */
  while (nplsidx != -1) {                         /* For each part to which frontier vertex belongs */
    Gnum                linknum;
    Anum                nplstmp;

    linknum = wgraphPartFmLinkGet (linkptr);      /* Create new link slot */
#ifdef SCOTCH_DEBUG_WGRAPH2
    if (linknum < 0) {
      errorPrint ("wgraphPartFmTablAdd: internal error");
      return (1);
    }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
    linktab[linknum].nextidx = linkidx;           /* Chain slot to working list */
    linkidx = linknum;

    linktab[linknum].partnum = nplsidx;
    linktab[linknum].vertnum = vertnum;
    linktab[linknum].gainval = gainval - nplstab[nplsidx].loadgainval; /* Gain for frontier is loads of neighbor vertices belonging to all other parts */
    linktab[linknum].npmipartnum = (nplsidx != npmipartnum) ? npmipartnum : npmspartnum;

    if ((linktab[linknum].npmipartnum == -1) ||   /* If link would not cause imbalance */
        ((comploadtab[nplsidx] < cplomax) &&
         (comploadtab[linktab[linknum].npmipartnum] > cplomin)))
      gainTablAdd (tablptr, (GainLink *) &linktab[linknum], linktab[linknum].gainval); /* Add link to gain table */
    else
      linktab[linknum].gainlink.tabl = NULL;      /* Set link as not chained */

    nplstmp = nplstab[nplsidx].nextidx;
    nplstab[nplsidx].nextidx = -2;
    nplsidx = nplstmp;
  }

  vexxptr->linkidx = linkidx;                     /* Link part list to frontier vertex */

  return (0);
}

/* This routine returns the vertex of best gain
** whose swap will keep the balance correct.
** It returns:
** - !NULL  : pointer to the vertex gainlink.
** - NULL   : if no more vertices available.
*/

static
WgraphPartFmLink *
wgraphPartFmTablGet (
GainTabl * restrict const     tablptr,            /*+ Gain table                                +*/
const Wgraph * restrict const grafptr,            /*+ Active graph                              +*/
const Gnum                    cplomin,            /*+ Minimum with respect to average part load +*/
const Gnum                    cplomax)            /*+ Maximum with respect to average part load +*/
{
  Gnum                        gainbst;            /* Frontier gain of best link       */
  Gnum                        cdltbst;            /* Part load imbalance of best link */
  GainEntr *                  tablbst;            /* Gain table entry of best link    */
  WgraphPartFmLink *          linkbst;            /* Pointer to best link found       */
  WgraphPartFmLink *          linkptr;            /* Pointer to current gain link     */

  const Gnum * restrict const comploadtab = grafptr->compload;

  linkbst = NULL;                                 /* Assume no candidate vertex found yet */
  tablbst = tablptr->tend;
  gainbst = GAINMAX;

  for (linkptr = (WgraphPartFmLink *) gainTablFrst (tablptr); /* Select candidate vertices */
       (linkptr != NULL) && (linkptr->gainlink.tabl <= tablbst);
       linkptr = (WgraphPartFmLink *) gainTablNext (tablptr, (GainLink *) linkptr)) {
    Anum                      partnum;            /* Destination part of vertex    */
    Anum                      pminnum;            /* Smallest impacted part        */
    Gnum                      gaincur;            /* Frontier gain of current link */

    partnum = linkptr->partnum;
    pminnum = linkptr->npmipartnum;
    gaincur = linkptr->gainval;                   /* Get frontier gain */

    if (pminnum == -1)                            /* If frontier vertex can go to one part only */
      return (linkptr);                           /* Return its link as it is an obvious gain   */

    if ((comploadtab[pminnum] <= cplomin) ||      /* If move would cause part imbalance, reject it */
        (comploadtab[partnum] >= cplomax))
      continue;

    if ((gaincur < gainbst) ||                    /* If it gives better gain than gain max */
        ((gaincur == gainbst) &&                  /* Or reduces imbalance most             */
         ((comploadtab[partnum] - comploadtab[pminnum]) < cdltbst))) {
      linkbst = linkptr;                          /* Select it */
      gainbst = gaincur;
      tablbst = linkptr->gainlink.tabl;
      cdltbst = comploadtab[partnum] - comploadtab[pminnum];
    }
  }

  return (linkbst);                               /* Return best link found */
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

int
wgraphPartFm (
Wgraph * restrict const         grafptr,    /*+ Active graph      +*/
const WgraphPartFmParam * const paraptr)    /*+ Method parameters +*/
{
  int                             passnbr;        /* Maximum number of passes to go                */
  int                             moveflag;       /* Flag set if useful moves made                 */
  Gnum                            partnum;
  Gnum                            partnbr;
  Gnum                            frlosum;
  Gnum                            frlobst;
  Gnum                            fronnbr;
  Gnum                            fronnum;
  WgraphPartFmVertex *            vexxptr;
  GainTabl * restrict             tablptr;        /* Pointer to gain table                         */
  WgraphPartFmHashData            hashdat;        /* Hash table management data                    */
  WgraphPartFmLinkData            linkdat;        /* Link array management data                    */
  WgraphPartFmSaveData            savedat;        /* Save array management data                    */
  WgraphPartFmPartList * restrict nplstab;        /* Array of neighboring parts of frontier vertex */
  int                             o;

  const Gnum * restrict const verttax = grafptr->s.verttax;
  const Gnum * restrict const vendtax = grafptr->s.vendtax;
  const Gnum * restrict const velotax = grafptr->s.velotax;
  const Gnum * restrict const edgetax = grafptr->s.edgetax;
  Gnum * restrict const       parttax = grafptr->parttax;
  Gnum * restrict const       comploadtab = grafptr->compload;
  Gnum * restrict const       compsizetab = grafptr->compsize;

  if (grafptr->fronnbr == 0) {                    /* If no frontier defined */
    WgraphPartGgParam   paradat;

    paradat.passnbr = 5;                          /* Use a standard algorithm */
    wgraphPartGg (grafptr, &paradat);

    if (grafptr->fronnbr == 0)                    /* If new partition has no frontier */
      return (0);                                 /* This algorithm is still useless  */
  }

  partnbr = grafptr->partnbr;                     /* Work with current number of parts */

#ifdef SCOTCH_DEBUG_WGRAPH2
  hashdat.hashsiz = 16;
  linkdat.linksiz = 2;
  savedat.savesiz = 2;
#else /* SCOTCH_DEBUG_WGRAPH2 */
  hashdat.hashnbr = 4 * (grafptr->fronnbr + grafptr->s.degrmax) + 1; /* TRICK: (degrmax + 1) so no need to resize within edge loops */
  if (hashdat.hashnbr > grafptr->s.vertnbr)
    hashdat.hashnbr = 2 * grafptr->s.vertnbr;
  hashdat.hashnbr *= 2;
  for (hashdat.hashsiz = 512; hashdat.hashsiz < hashdat.hashnbr; hashdat.hashsiz <<= 1) ; /* Get upper power of two */
  linkdat.linksiz = hashdat.hashnbr;
  savedat.savesiz = hashdat.hashnbr;
#endif /* SCOTCH_DEBUG_WGRAPH2 */
  savedat.savenbr = 0;                            /* No moves saved (yet) */

  if (((tablptr = gainTablInit (GAINMAX, WGRAPHPARTFMGAINBITS)) == NULL) || /* Use logarithmic array only         */
      ((nplstab = memAlloc ((partnbr + 1) * sizeof (WgraphPartFmPartList))) == NULL) || /* TRICK: +1 for frontier */
      ((hashdat.hashtab = memAlloc (hashdat.hashsiz * sizeof (WgraphPartFmVertex))) == NULL) ||
      ((linkdat.linktab = memAlloc (linkdat.linksiz * sizeof (WgraphPartFmLink)))   == NULL) ||
      ((savedat.savetab = memAlloc (savedat.savesiz * sizeof (WgraphPartFmSave)))   == NULL)) {
    errorPrint ("wgraphPartFm: out of memory");
    if (tablptr != NULL) {
      if (nplstab != NULL) {
        if (hashdat.hashtab != NULL) {
          if (linkdat.linktab != NULL)
            memFree (linkdat.linktab);
          memFree (hashdat.hashtab);
        }
        memFree (nplstab);
      }
      gainTablExit (tablptr);
    }
    return (1);
  }
  nplstab ++;                                     /* TRICK: move one slot ahead for frontier */
  hashdat.hashmax = hashdat.hashsiz >> 2;         /* Use hash table at 1/4 of its capacity   */
  hashdat.hashmsk = hashdat.hashsiz - 1;
  hashdat.hashnbr = 0;
  hashdat.lockptr = &wgraphpartfmvertexdummy;     /* Initialize locked vertex list to add frontier vertices */

  wgraphPartFmLinkInit (&linkdat);
  wgraphPartFmSaveInit (&savedat);

  memSet (hashdat.hashtab, ~0, hashdat.hashsiz * sizeof (WgraphPartFmVertex)); /* All vertnum's set to -1 */

  for (partnum = -1; partnum < partnbr; partnum ++) /* Initialize part list array (TRICK: with frontier) */
    nplstab[partnum].nextidx = -2;

  o = 1;                                          /* Assume an error */

  while (wgraphPartFmHashClaim (&hashdat, grafptr->fronnbr)) { /* Prepare hash table for inserting frontier vertices */
    if (wgraphPartFmHashResize (&hashdat) != 0) {
      errorPrint ("wgraphPartFm: cannot resize hash array (1)");
      goto abort;
    }
  }

  fronnbr = grafptr->fronnbr;
  for (fronnum = 0; fronnum < fronnbr; fronnum ++) { /* Allocate hash slots for frontier vertices */
    Gnum                  hashnum;
    Gnum                  vertnum;
    WgraphPartFmVertex *  vexxptr;

    vertnum = grafptr->frontab[fronnum];
    for (hashnum = (vertnum * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxptr = hashdat.hashtab + hashnum; ; /* Create slot for frontier vertex */
         hashnum = (hashnum + 1) & hashdat.hashmsk, vexxptr = hashdat.hashtab + hashnum) {
      if (vexxptr->vertnum == ~0) {               /* If vertex not found, create slot in hash table */
        vexxptr->vertnum = vertnum;
        vexxptr->partnum = -1;                    /* Vertex belongs to frontier */
#ifdef SCOTCH_DEBUG_WGRAPH2
        vexxptr->linkidx = -1;                    /* No links attached to vertex */
#endif /* SCOTCH_DEBUG_WGRAPH2 */
        vexxptr->nlokptr = hashdat.lockptr;       /* Chain vertex to locked list */
        hashdat.lockptr = vexxptr;
        hashdat.hashnbr ++;
#ifdef SCOTCH_DEBUG_WGRAPH2
        if (hashdat.hashnbr > hashdat.hashmax) {  /* Table should not be filled-in to capacity */
          errorPrint ("wgraphPartFm: internal error (1)");
          goto abort;
        }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
        vexxptr->nlstptr = NULL;                  /* Vertex not in working list */
        break;
      }
#ifdef SCOTCH_DEBUG_WGRAPH2
      if (vexxptr->vertnum == vertnum) {
        errorPrint ("wgraphPartFm: duplicate vertex in frontier array");
        continue;
      }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
    }
  }

  frlosum =
  frlobst = grafptr->fronload;                    /* Get load of frontier */
  passnbr = paraptr->passnbr;
  do {                                            /* As long as there is improvement             */
    Gnum                cplosum;                  /* Sum of vertex loads in parts and boundaries */
    Gnum                cplomin;                  /* Minimum acceptable part load                */
    Gnum                cplomax;                  /* Maximum acceptable part load                */
    WgraphPartFmLink *  linkptr;
    Gnum                movenbr;                  /* Number of uneffective moves done */

    for (partnum = 0, cplosum = 0; partnum < partnbr; partnum ++) /* Set initial part load sum (after unroll) */
      cplosum += comploadtab[partnum];
    cplomin = (Gnum) ((float) cplosum * (1.0F - paraptr->deltrat) / (float) partnbr);
    cplomax = (Gnum) ((float) cplosum * (1.0F + paraptr->deltrat) / (float) partnbr);

    while (hashdat.lockptr != &wgraphpartfmvertexdummy) { /* Purge list of locked vertices */
      WgraphPartFmVertex *  vexxptr;

      vexxptr = hashdat.lockptr;                  /* Remove vertex from locked list */
      hashdat.lockptr = vexxptr->nlokptr;
      vexxptr->nlokptr = NULL;

      if (vexxptr->partnum == -1)                 /* If vertex belongs to frontier, link it */
        wgraphPartFmTablAdd (tablptr, grafptr, hashdat.hashtab, hashdat.hashmsk, &linkdat, nplstab, cplomin, cplomax, vexxptr);
    }

#ifdef SCOTCH_DEBUG_WGRAPH2
#ifdef SCOTCH_DEBUG_WGRAPH3
    grafptr->fronload = frlosum;
    grafptr->fronnbr  = fronnbr;
    if (wgraphPartFmCheck (grafptr, &hashdat, cplosum) != 0) {
      errorPrint ("wgraphPartFm: internal error (2)");
      goto abort;
    }
#endif /* SCOTCH_DEBUG_WGRAPH3 */
#endif /* SCOTCH_DEBUG_WGRAPH2 */

    movenbr  = 0;                                 /* No uneffective moves yet                                   */
    moveflag = 0;                                 /* No moves to date                                           */
    while ((movenbr < paraptr->movenbr) &&        /* As long as we can find effective links to vertices to move */
           ((linkptr = wgraphPartFmTablGet (tablptr, grafptr, cplomin, cplomax)) != NULL)) {
      Gnum                  vertnum;
      Gnum                  gainval;              /* Gain value for frontier: - velotax[vertnum] */
      WgraphPartFmVertex *  vexxptr;
      WgraphPartFmVertex *  vexxend;
      WgraphPartFmVertex *  listptr;              /* List of vertices to process during a move   */
      Gnum                  listnbr;
      Gnum                  hashnum;
      Anum                  partnum;
      Anum                  pminnum;
      Anum                  partend;
      Gnum                  edgenum;
      Anum                  nplsidx;
      Gnum                  linknum;

      vertnum = linkptr->vertnum;

      while (wgraphPartFmHashClaim (&hashdat, (vendtax[vertnum] - verttax[vertnum]))) { /* Possibly resize hash table before creating working list */
        if (wgraphPartFmHashResize (&hashdat) != 0) {
          errorPrint ("wgraphPartFm: cannot resize hash array (2)");
          goto abort;
        }
      }

      for (hashnum = (vertnum * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxptr = hashdat.hashtab + hashnum; /* Search for vertex in hash table */
           vexxptr->vertnum != vertnum; hashnum = (hashnum + 1) & hashdat.hashmsk, vexxptr = hashdat.hashtab + hashnum) {
#ifdef SCOTCH_DEBUG_WGRAPH2
        if (vexxptr->vertnum == ~0) {             /* If vertex not present */
          errorPrint ("wgraphPartFm: internal error (3)");
          goto abort;
        }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
      }

#ifdef SCOTCH_DEBUG_WGRAPH2
      if ((vexxptr->nlokptr != NULL) ||           /* Vertex should not be already locked or in working list */
          (vexxptr->nlstptr != NULL)) {
        errorPrint ("wgraphPartFm: internal error (4)");
        goto abort;
      }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

      partnum = linkptr->partnum;                 /* Get vertex target part     */
      pminnum = linkptr->npmipartnum;             /* Get smallest impacted part */
      gainval = (velotax != NULL) ? - velotax[vertnum] : -1;
      nplsidx = partnum;                          /* Initialize vertex part list: first cell is frontier */
      nplstab[partnum].nextidx = -1;
      nplstab[partnum].loadgainval = 0;           /* Vertex remains in its destination part */
      nplstab[partnum].sizegainval = 0;
      nplstab[-1].loadgainval = gainval;          /* Vertex leaves frontier */
      nplstab[-1].sizegainval = -1;
      for (linknum = vexxptr->linkidx; linknum != -1; ) { /* For all of its former potential target parts */
        Anum                partend;
        Gnum                linktmp;

        if (linkdat.linktab[linknum].gainlink.tabl != NULL) /* If link was chained in gain table, remove it */
          gainTablDel (tablptr, (GainLink *) &linkdat.linktab[linknum].gainlink);
        partend = linkdat.linktab[linknum].partnum;
        if (partend != partnum) {
#ifdef SCOTCH_DEBUG_WGRAPH2
          if (nplstab[partend].nextidx != -2) {
            errorPrint ("wgraphPartFm: internal error (5)");
            goto abort;
          }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
          nplstab[partend].nextidx = nplsidx;
          nplsidx = partend;
          nplstab[partend].loadgainval = gainval; /* Vertex leaves this part as well */
          nplstab[partend].sizegainval = -1;
          nplstab[partend].vechnum =              /* Initialize neighbor vertex indices for this part */
          nplstab[partend].vencnum = ~0;
        }
        linktmp = linknum;
        linknum = linkdat.linktab[linknum].nextidx; /* Get next link        */
        wgraphPartFmLinkPut (&linkdat, linktmp);  /* Free old link in table */
      }
      vexxptr->linkidx = -1;                      /* No more active links for vertex         */
      vexxptr->nlokptr = hashdat.lockptr;         /* Add vertex to locked list for this pass */
      hashdat.lockptr  = vexxptr;
      vexxptr->nlstptr = &wgraphpartfmvertexdummy; /* Flag vertex so not taken as frontier neighbor of neighbor */

      listptr = &wgraphpartfmvertexdummy;         /* Empty working list of end vertices                       */
      listnbr = 1;                                /* TRICK: Already one item to account for vertex itself     */
      for (edgenum = verttax[vertnum]; edgenum < vendtax[vertnum]; edgenum ++) { /* For all neighbor vertices */
        Gnum                  vertend;
        WgraphPartFmVertex *  vexxend;
        Gnum                  hashend;
        Anum                  partend;

        vertend = edgetax[edgenum];
        for (hashend = (vertend * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxend = hashdat.hashtab + hashend; ; /* Find slot for/of end vertex to get its part */
             hashend = (hashend + 1) & hashdat.hashmsk, vexxend = hashdat.hashtab + hashend) {
          if (vexxend->vertnum == vertend) {      /* If hash slot found     */
            partend = vexxend->partnum;           /* Get current part value */
            break;
          }
          if (vexxend->vertnum == ~0) {           /* If vertex not found    */
            partend = parttax[vertend];           /* Get initial part value */
#ifdef SCOTCH_DEBUG_WGRAPH2
            if (partend == -1) {                  /* If vertex was not in original frontier array */
              errorPrint ("wgraphPartFm: vertex not in frontier array (1)");
              goto abort;
            }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
            if (partend == partnum)               /* If neighbor in same part as vertex destination */
              break;                              /* No need to create slot in hash table           */

            vexxend->vertnum = vertend;           /* Set vertex number                                */
            vexxend->partnum = partend;           /* Set old part, for testing neighbors of neighbors */
            vexxend->linkidx = -1;                /* No target parts linked to vertex yet             */
            vexxend->nlokptr = NULL;              /* Vertex not in locked list                        */
            vexxend->nlstptr = NULL;              /* Vertex not in working list (yet)                 */
            hashdat.hashnbr ++;
#ifdef SCOTCH_DEBUG_WGRAPH2
            if (hashdat.hashnbr > hashdat.hashmax) { /* Table should not be filled-in to capacity */
              errorPrint ("wgraphPartFm: internal error (6)");
              goto abort;
            }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
            break;
          }
        }

        if (partend == partnum)                   /* If end vertex in destination part, nothing to do */
          continue;

        if (partend != -1) {                      /* If end vertex will move to frontier */
          Gnum                veloend;
          Gnum                edgeend;

          veloend = (velotax != NULL) ? velotax[vertend] : 1;
          nplstab[-1].loadgainval += veloend;     /* Account for its load in frontier */
          nplstab[-1].sizegainval ++;
          nplstab[partnum].loadgainval += veloend; /* It also moves to destination part */
          nplstab[partnum].sizegainval ++;

          for (edgeend = verttax[vertend]; edgeend < vendtax[vertend]; edgeend ++) { /* Explore its neighbor vertices one by one */
            Gnum                  vertent;
            WgraphPartFmVertex *  vexxent;
            Gnum                  hashent;
            Anum                  partent;

            vertent = edgetax[edgeend];           /* Number of neighbor of neighbor */
            for (hashent = (vertent * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxent = hashdat.hashtab + hashent; ; /* Find slot for end vertex to get its part */
                 hashent = (hashent + 1) & hashdat.hashmsk, vexxent = hashdat.hashtab + hashent) {
              if (vexxent->vertnum == vertent) {  /* If hash slot found     */
                partent = vexxent->partnum;       /* Get current part value */
                break;
              }
              if (vexxent->vertnum == ~0) {       /* If vertex not found    */
                partent = parttax[vertent];       /* Get initial part value */
#ifdef SCOTCH_DEBUG_WGRAPH2
                if (partent == -1) {              /* If vertex was not in original frontier array */
                  errorPrint ("wgraphPartFm: vertex not in frontier array (2)");
                  goto abort;
                }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
                break;
              }
            }

            if ((partent == -1) &&                /* If vertex is a frontier vertex that may be removed from part of neighbor */
                (vexxent->nlstptr == NULL)) {     /* And not already in working list                                          */
              vexxent->nlstptr = listptr;         /* Add vertex to working list                                               */
              listptr = vexxent;
              listnbr ++;
            }
          }
        }

        if (vexxend->nlstptr == NULL) {           /* If neighbor vertex not already caught as neighbor of neighbor */
          vexxend->nlstptr = listptr;             /* Vertex is/will be in frontier                                 */
          listptr = vexxend;                      /* Link it for further processing                                */
          listnbr ++;                             /* One more vertex in list                                       */
        }
      }
      vexxptr->nlstptr = NULL;                    /* Unlock chosen vertex */

      while (wgraphPartFmSaveClaim (&savedat, listnbr + (vendtax[vertnum] - verttax[vertnum]))) { /* TRICK: vertex is already accounted for in listnbr */
        if (wgraphPartFmSaveResize (&savedat) != 0) {
          errorPrint ("wgraphPartFm: cannot resize save array");
          goto abort;
        }
      }

      savedat.savetab[savedat.savenbr].typeval = WGRAPHPARTFMSAVEMOVE; /* Record move from frontier */
      savedat.savetab[savedat.savenbr].u.movedat.vertnum = vertnum;
      savedat.savetab[savedat.savenbr].u.movedat.partnum = -1; /* Vertex belonged to frontier */
      savedat.savenbr ++;

      for (vexxend = listptr; vexxend != &wgraphpartfmvertexdummy; vexxend = vexxend->nlstptr) { /* For all neighbors and relevant neigbors-of-neighbors */
        Anum                partend;

        partend = vexxend->partnum;
        if (partend == -1) {                      /* If pre-existing frontier vertex */
          Gnum                vertend;
          Gnum                veloend;
          Gnum                edgeend;
          Gnum                linknum;
          Anum                partent;
          int                 flagval;            /* Flag for saving part change */

          vertend = vexxend->vertnum;
          veloend = (velotax != NULL) ? velotax[vertend] : 1;
          flagval = 0;

          for (edgeend = verttax[vertend]; edgeend < vendtax[vertend]; edgeend ++) { /* Explore its neighbor vertices one by one */
            Gnum                  vertent;
            WgraphPartFmVertex *  vexxent;
            Gnum                  hashent;
            Anum                  partent;

            vertent = edgetax[edgeend];           /* Number of neighbor of neighbor */
            for (hashent = (vertent * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxent = hashdat.hashtab + hashent; ; /* Find slot for end vertex */
                 hashent = (hashent + 1) & hashdat.hashmsk, vexxent = hashdat.hashtab + hashent) {
              if (vexxent->vertnum == ~0) {
                partent = parttax[vertent];
#ifdef SCOTCH_DEBUG_WGRAPH2
                if (partent == -1) {              /* If vertex was not in original frontier array */
                  errorPrint ("wgraphPartFm: vertex not in frontier array (3)");
                  goto abort;
                }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
                vexxent = NULL;
                break;
              }
              if (vexxent->vertnum == vertent) {  /* If hash slot found */
                partent = vexxent->partnum;       /* Get (old) part     */
                if (vexxent == vexxptr)           /* If direct neighbor */
                  flagval |= 1;                   /* Flag it as such    */
                break;
              }
            }

            if (partent == partnum) {             /* If neighbor has another neighbor in destination part (TRICK: vertnum part not yet changed) */
              flagval |= 2;                       /* Flag it as such                                                                            */
              continue;                           /* End vertices of same part have no further impact on neighbor so skip to next end vertex    */
            }

            if (nplstab[partent].nextidx == -2)   /* If end vertex part is not subject to change (TRICK:) or is frontier, skip it */
              continue;

            if ((vexxent == NULL) ||              /* If we have a neighbor in this part which is not changed */
                (vexxent->nlstptr == NULL)) {
              nplstab[partent].vencnum = vertend; /* Flag that we have an end vertex in this part that will not change */
            }
            else
              nplstab[partent].vechnum = vertend; /* Else flag that we have an end vertex in this part that will change */
          }

          if (flagval == 1) {                     /* If direct neighbor frontier vertex not already in destination part */
            nplstab[partnum].loadgainval += veloend; /* Frontier vertex moves to destination part                       */
            nplstab[partnum].sizegainval ++;
          }

          for (partent = nplsidx; partent != partnum; partent = nplstab[partent].nextidx) { /* For all parts to be changed */
            if ((nplstab[partent].vechnum == vertend) && /* If neighbor frontier vertex was in part but will no longer be  */
                (nplstab[partent].vencnum != vertend)) {
              nplstab[partent].loadgainval -= veloend; /* Frontier vertex leaves this part */
              nplstab[partent].sizegainval --;
            }
          }

          for (linknum = vexxend->linkidx; linknum != -1; ) { /* As frontier vertex incurs change, remove its links if any */
            Gnum                linktmp;

            if (linkdat.linktab[linknum].gainlink.tabl != NULL) /* If link was chained in gain table, remove it */
              gainTablDel (tablptr, (GainLink *) &linkdat.linktab[linknum].gainlink);
            linktmp = linknum;
            linknum = linkdat.linktab[linknum].nextidx; /* Get next link       */
            wgraphPartFmLinkPut (&linkdat, linktmp); /* Free old link in table */
          }
          vexxend->linkidx = -1;                  /* No more active links for vertex */
        }
        else {                                    /* Vertex will enter the frontier */
          Gnum                vertend;
          Gnum                edgeend;

          vertend = vexxend->vertnum;

          for (edgeend = verttax[vertend]; edgeend < vendtax[vertend]; edgeend ++) { /* Explore its neighbor vertices one by one */
            Gnum                  vertent;
            WgraphPartFmVertex *  vexxent;
            Gnum                  hashent;
            Anum                  partent;

            vertent = edgetax[edgeend];           /* Number of neighbor of neighbor */
            for (hashent = (vertent * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxent = hashdat.hashtab + hashent; ; /* Find slot for end vertex */
                 hashent = (hashent + 1) & hashdat.hashmsk, vexxent = hashdat.hashtab + hashent) {
              if (vexxent->vertnum == vertent) {  /* If hash slot found */
                partent = vexxent->partnum;       /* Get (old) part     */
                break;
              }
              if (vexxent->vertnum == ~0) {
                partent = parttax[vertent];
#ifdef SCOTCH_DEBUG_WGRAPH2
                if (partent == -1) {              /* If vertex was not in original frontier array */
                  errorPrint ("wgraphPartFm: vertex not in frontier array (3)");
                  goto abort;
                }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
                vexxent = NULL;
                break;
              }
            }

            if ((partent == partend) &&           /* If we have a neighbor in the same part which is not changed */
                ((vexxent == NULL) ||
                 (vexxent->nlstptr == NULL)))
              break;                              /* No need to process other neighbors */
          }

          if (edgeend == vendtax[vertend]) {      /* If vertex has no neighbor remaining in its old part */
            Gnum                veloend;

            veloend = (velotax != NULL) ? velotax[vertend] : 1;
            nplstab[partend].loadgainval -= veloend; /* Vertex will no longer belong to its old part */
            nplstab[partend].sizegainval --;
          }
        }

        savedat.savetab[savedat.savenbr].typeval = WGRAPHPARTFMSAVEMOVE;
        savedat.savetab[savedat.savenbr].u.movedat.vertnum = vexxend->vertnum;
        savedat.savetab[savedat.savenbr].u.movedat.partnum = vexxend->partnum;
        savedat.savenbr ++;
      }

      partend = nplsidx;
      do {                                        /* For all (possibly) updated parts including part -1 */
        Anum                parttmp;

        savedat.savetab[savedat.savenbr].typeval = WGRAPHPARTFMSAVELOAD; /* Record change in load */
        savedat.savetab[savedat.savenbr].u.loaddat.partnum = partend;
        if (partend >= 0) {
          savedat.savetab[savedat.savenbr].u.loaddat.loadval = comploadtab[partend];
          savedat.savetab[savedat.savenbr].u.loaddat.sizeval = compsizetab[partend];
          comploadtab[partend] += nplstab[partend].loadgainval;
          compsizetab[partend] += nplstab[partend].sizegainval;
          cplosum += nplstab[partend].loadgainval; /* Update sum of part loads */
        }
        else {
          savedat.savetab[savedat.savenbr].u.loaddat.loadval = frlosum;
          savedat.savetab[savedat.savenbr].u.loaddat.sizeval = fronnbr;
          frlosum += nplstab[partend].loadgainval;
          fronnbr += nplstab[partend].sizegainval;
        }
        savedat.savenbr ++;

        parttmp = partend;
        partend = nplstab[parttmp].nextidx;
        nplstab[parttmp].nextidx = -2;
      } while (partend != -2);

      vexxptr->partnum = partnum;                 /* Move vertex to its destination part (TRICK: after neighbors scanned it)            */
      for (vexxend = listptr; vexxend != &wgraphpartfmvertexdummy; vexxend = vexxend->nlstptr) /* Move all linked neighbors to frontier */
        vexxend->partnum = -1;

      for (vexxend = listptr; vexxend != &wgraphpartfmvertexdummy; ) { /* For each linked, unlocked neighbor */
        WgraphPartFmVertex *  vexxtmp;

        if ((vexxend->partnum == -1) &&           /* If vertex belongs to frontier */
            (vexxend->nlokptr == NULL))           /* And is not locked, re-link it */
          wgraphPartFmTablAdd (tablptr, grafptr, hashdat.hashtab, hashdat.hashmsk, &linkdat, nplstab, cplomin, cplomax, vexxend);

        vexxtmp = vexxend;                        /* Remove vertex from working list */
        vexxend = vexxtmp->nlstptr;
        vexxtmp->nlstptr = NULL;
      }

      cplomin = (Gnum) ((float) cplosum * (1.0F - paraptr->deltrat) / (float) partnbr); /* Adjust minimum part load */
      cplomax = (Gnum) ((float) cplosum * (1.0F + paraptr->deltrat) / (float) partnbr); /* Adjust minimum part load */

#ifdef SCOTCH_DEBUG_WGRAPH2
#ifdef SCOTCH_DEBUG_WGRAPH3
      grafptr->fronload = frlosum;
      grafptr->fronnbr  = fronnbr;
      if (wgraphPartFmCheck (grafptr, &hashdat, cplosum) != 0) {
        errorPrint ("wgraphPartFm: internal error (7)");
        goto abort;
      }
#endif /* SCOTCH_DEBUG_WGRAPH3 */
#endif /* SCOTCH_DEBUG_WGRAPH2 */

      movenbr ++;                                 /* One more move dore */
      if (pminnum == -1) {
        savedat.savenbr = 0;
        movenbr = 0;
        if (frlosum < frlobst) {
          moveflag = 1;
          frlobst  = frlosum;
        }
      }
      else if (frlosum < frlobst) {
        savedat.savenbr = 0;
        movenbr  =
        moveflag = 1;
        frlobst  = frlosum;
      }
      else if ((frlosum == frlobst) && ((comploadtab[partnum] - comploadtab[pminnum]) < 0)) {
        savedat.savenbr = 0;
        movenbr  = 0;
        moveflag = 1;
      }
      else if (comploadtab[partnum] < cplomin) {
        savedat.savenbr = 0;
        movenbr = 0;
        frlobst = frlosum;
      }
    }

    while (savedat.savenbr > 0) {                 /* Roll-back useless moves */
      WgraphPartFmVertex *  vexxptr;
      Gnum                  vertnum;
      Gnum                  hashnum;
      Gnum                  linknum;
      Anum                  partnum;


      savedat.savenbr --;
      switch (savedat.savetab[savedat.savenbr].typeval) {
        case WGRAPHPARTFMSAVEMOVE :               /* Moved-back vertices will be placed in locked list for next pass (if any) */
          vertnum = savedat.savetab[savedat.savenbr].u.movedat.vertnum;
          for (hashnum = (vertnum * WGRAPHPARTFMHASHPRIME) & hashdat.hashmsk, vexxptr = hashdat.hashtab + hashnum; /* Search for vertex in hash table */
               vexxptr->vertnum != vertnum; hashnum = (hashnum + 1) & hashdat.hashmsk, vexxptr = hashdat.hashtab + hashnum) {
#ifdef SCOTCH_DEBUG_WGRAPH2
            if (vexxptr->vertnum == ~0) {         /* If vertex not present */
              errorPrint ("wgraphPartFm: internal error (8)");
              goto abort;
            }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
          }

          for (linknum = vexxptr->linkidx; linknum != -1; ) { /* For all potential links of vertex (if frontier vertex) */
            Gnum                linktmp;

            if (linkdat.linktab[linknum].gainlink.tabl != NULL) /* If link was chained in gain table, remove it */
              gainTablDel (tablptr, (GainLink *) &linkdat.linktab[linknum].gainlink);
            linktmp = linknum;
            linknum = linkdat.linktab[linknum].nextidx;
            wgraphPartFmLinkPut (&linkdat, linktmp); /* Release link from vertex */
          }
          vexxptr->linkidx = -1;                  /* No more active links for vertex                    */
          vexxptr->partnum = savedat.savetab[savedat.savenbr].u.movedat.partnum; /* Set old vertex part */

          if ((vexxptr->nlokptr == NULL) &&       /* If vertex not already in locked list */
              (vexxptr->partnum == -1)) {         /* And may remain in frontier           */
            vexxptr->nlokptr = hashdat.lockptr;   /* Link vertex to locked list           */
            hashdat.lockptr = vexxptr;
          }
          break;
        case WGRAPHPARTFMSAVELOAD :
          partnum = savedat.savetab[savedat.savenbr].u.loaddat.partnum;
          if (partnum != -1) {
            comploadtab[partnum] = savedat.savetab[savedat.savenbr].u.loaddat.loadval;
            compsizetab[partnum] = savedat.savetab[savedat.savenbr].u.loaddat.sizeval;
          }
          else {
            frlosum = savedat.savetab[savedat.savenbr].u.loaddat.loadval;
            fronnbr = savedat.savetab[savedat.savenbr].u.loaddat.sizeval;
          }
          break;
      }
    }
  } while ((moveflag != 0) &&                     /* As long as vertices are moved */
           (-- passnbr > 0));                     /* And we are allowed to loop    */

  grafptr->fronload = frlosum;                    /* Write back frontier loads */
  grafptr->fronnbr  = fronnbr;

#ifdef SCOTCH_DEBUG_WGRAPH2
  frlosum = 0;                                    /* Recompute frontier load sum */
#endif /* SCOTCH_DEBUG_WGRAPH2 */
  for (vexxptr = hashdat.hashtab, fronnbr = 0;    /* Write back part array and build new frontier from hash table */
       vexxptr < hashdat.hashtab + hashdat.hashsiz; vexxptr ++) {
    Gnum                vertnum;
    Anum                partnum;

    vertnum = vexxptr->vertnum;
    if (vertnum == ~0)                            /* Skip empty slots */
      continue;

    partnum = vexxptr->partnum;
    parttax[vertnum] = partnum;                   /* Set part from vertex slot */
    if (partnum == -1) {
#ifdef SCOTCH_DEBUG_WGRAPH2
      frlosum += (velotax != NULL) ? velotax[vertnum] : 1; /* Recompute frontier load sum */
#endif /* SCOTCH_DEBUG_WGRAPH2 */
      grafptr->frontab[fronnbr ++] = vertnum;
    }
  }
#ifdef SCOTCH_DEBUG_WGRAPH2
  if ((grafptr->fronnbr  != fronnbr) ||           /* Compare with loads written back */
      (grafptr->fronload != frlosum)) {
    errorPrint ("wgraphPartFm: internal error (9)");
    goto abort;
  }

  if (wgraphCheck (grafptr) != 0) {
    errorPrint ("wgraphPartFm: inconsistent graph data");
    goto abort;
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  o = 0;                                          /* Everything went well */
abort:
  memFree (savedat.savetab);
  memFree (linkdat.linktab);
  memFree (hashdat.hashtab);
  memFree (nplstab - 1);                          /* TRICK: array starts one cell before */
  gainTablExit (tablptr);

  return (o);
}

/******************************************/
/*                                        */
/* This routine checks the consistency of */
/* the internal data structures.          */
/*                                        */
/******************************************/

/* This routine checks the consistency of
** the hash structures.
** It returns:
** - 0   : in case of success.
** - !0  : in case of error.
*/

#ifdef SCOTCH_DEBUG_WGRAPH3
static
int
wgraphPartFmCheck (
const Wgraph * restrict const               grafptr,
const WgraphPartFmHashData * restrict const hashptr,
const Gnum                                  cplosum)
{
  Gnum                      vertnum;
  Gnum                      partnum;
  Gnum                      fronnbr;
  Gnum                      frlosum;
  Gnum                      cplotmp;
  Gnum * restrict           comploadtab;
  Gnum * restrict           compsizetab;
  Gnum * restrict           flagtab;
  int                       o;

  if (memAllocGroup ((void **) (void *)
                     &flagtab,     (size_t) (grafptr->partnbr * sizeof (Gnum)),
                     &comploadtab, (size_t) (grafptr->partnbr * sizeof (Gnum)),
                     &compsizetab, (size_t) (grafptr->partnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("wgraphPartFmCheck: out of memory");
    return (1);
  }

  o = 1;                                          /* Assume an error */
  fronnbr =
  frlosum = 0;
  memSet (comploadtab, 0, grafptr->partnbr * sizeof (Gnum)); /* Reset loads */
  memSet (compsizetab, 0, grafptr->partnbr * sizeof (Gnum));
  memSet (flagtab,    ~0, grafptr->partnbr * sizeof (Gnum)); /* Reset flag array */

  for (vertnum = grafptr->s.baseval; vertnum < grafptr->s.vertnnd; vertnum ++) {
    Gnum                veloval;
    Gnum                hashnum;
    Anum                partnum;

    veloval = (grafptr->s.velotax != NULL) ? grafptr->s.velotax[vertnum] : 1;
    for (hashnum = (vertnum * WGRAPHPARTFMHASHPRIME) & hashptr->hashmsk; ; /* Find potential slot of vertex to get its part */
         hashnum = (hashnum + 1) & hashptr->hashmsk) {
      if (hashptr->hashtab[hashnum].vertnum == vertnum) { /* If hash slot found */
        partnum = hashptr->hashtab[hashnum].partnum; /* Get current part value  */
        break;
      }
      if (hashptr->hashtab[hashnum].vertnum == ~0) { /* If vertex not found */
        partnum = grafptr->parttax[vertnum];      /* Get initial part value */
        break;
      }
    }

    if (partnum == -1) {
      Gnum                edgenum;

      frlosum += veloval;
      fronnbr ++;

      for (edgenum = grafptr->s.verttax[vertnum];
           edgenum < grafptr->s.vendtax[vertnum]; edgenum ++) {
        Gnum                vertend;
        Gnum                hashend;
        Anum                partend;

        vertend = grafptr->s.edgetax[edgenum];
        for (hashend = (vertend * WGRAPHPARTFMHASHPRIME) & hashptr->hashmsk; ; /* Find potential slot of vertex to get its part */
             hashend = (hashend + 1) & hashptr->hashmsk) {
          if (hashptr->hashtab[hashend].vertnum == vertend) { /* If hash slot found */
            partend = hashptr->hashtab[hashend].partnum; /* Get current part value  */
            break;
          }
          if (hashptr->hashtab[hashend].vertnum == ~0) { /* If vertex not found */
            partend = grafptr->parttax[vertend];  /* Get initial part value     */
            break;
          }
        }

        if ((partend != -1) &&
            (flagtab[partend] != vertnum)) {
          comploadtab[partend] += veloval;
          compsizetab[partend] ++;
          flagtab[partend] = vertnum;
        }
      }
    }
    else {
      comploadtab[partnum] += veloval;
      compsizetab[partnum] ++;
    }
  }

  for (partnum = 0, cplotmp = 0; partnum < grafptr->partnbr; partnum ++) {
    if (grafptr->compsize[partnum] != compsizetab[partnum]) {
      errorPrint ("wgraphPartFmCheck: invalid part size array");
      goto abort;
    }
    if (grafptr->compload[partnum] != comploadtab[partnum]) {
      errorPrint ("wgraphPartFmCheck: invalid part load array");
      goto abort;
    }
    cplotmp += comploadtab[partnum];
  }

  if (grafptr->fronload != frlosum) {
    errorPrint ("wgraphPartFmCheck: invalid frontier load");
    goto abort;
  }
  if (grafptr->fronnbr != fronnbr) {
    errorPrint ("wgraphPartFmCheck: invalid frontier size");
    goto abort;
  }
  if (cplotmp != cplosum) {
    errorPrint ("wgraphPartFmCheck: invalid part load sum");
    goto abort;
  }

  o = 0;                                          /* Everything went all right */
abort :
  memFree (flagtab);                              /* Free group leader */

  return (o);
}
#endif /* SCOTCH_DEBUG_WGRAPH3 */

/*****************************************/
/*                                       */
/* These routines handle the link array. */
/*                                       */
/*****************************************/

static
void
wgraphPartFmLinkInit (
WgraphPartFmLinkData * const  ldatptr)
{
  Gnum                linknum;

  ldatptr->lfrenbr = ldatptr->linksiz;            /* Set number of free link cells            */
  ldatptr->lfreidx = 0;                           /* First free cell is at beginning of array */

  for (linknum = 0; linknum < (ldatptr->linksiz - 1); linknum ++) /* Initialize free list of links */
    ldatptr->linktab[linknum].nextidx = linknum + 1;
  ldatptr->linktab[ldatptr->linksiz - 1].nextidx = -1; /* Set end of free list */
}

/* This routine retrieves a free link slot from
** the link array.
** It returns:
** - >=0  : if slot available.
** - -1   : on error.
*/

static
Gnum
wgraphPartFmLinkGet (
WgraphPartFmLinkData * const  ldatptr)
{
  Gnum                linkidx;

#ifdef SCOTCH_DEBUG_WGRAPH2
  if (ldatptr->lfrenbr <= 0) {
    errorPrint ("wgraphPartFmLinkGet: empty free list");
    return (-1);
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  linkidx = ldatptr->lfreidx;
  ldatptr->lfreidx = ldatptr->linktab[linkidx].nextidx;
  ldatptr->lfrenbr --;

  return (linkidx);
}

/* This routine gives back a free slot to the link array.
** It returns:
** - void  : in all cases.
*/

static
void
wgraphPartFmLinkPut (
WgraphPartFmLinkData * const  ldatptr,
const Gnum                    linkidx)
{
#ifdef SCOTCH_DEBUG_WGRAPH2
  if (ldatptr->lfrenbr >= ldatptr->linksiz) {
    errorPrint ("wgraphPartFmLinkPut: full free list");
    return;
  }
#endif /* SCOTCH_DEBUG_WGRAPH2 */

  ldatptr->linktab[linkidx].nextidx = ldatptr->lfreidx;
  ldatptr->lfreidx = linkidx;
  ldatptr->lfrenbr ++;
}

/* This routine increases the size of the
** link array.
** It returns:
** - 0   : if resizing succeeded.
** - !0  : if out of memory.
*/

static
int
wgraphPartFmLinkResize (
WgraphPartFmLinkData * const  ldatptr,
GainTabl * restrict const     tablptr)
{
  WgraphPartFmLink *  linktab;                    /* Pointer to reallocated array */
  Gnum                linksiz;                    /* Size of reallocated array    */
  Gnum                linknum;
  ptrdiff_t           addrdlt;

  linksiz  = ldatptr->linksiz;                    /* Increase array size by 25% */
  linksiz += (linksiz >> 2) + 4;

  if ((linktab = (WgraphPartFmLink *) memRealloc (ldatptr->linktab, linksiz * sizeof (WgraphPartFmLink))) == NULL) {
    errorPrint ("wgraphPartFmLinkResize: out of memory");
    return (1);
  }

  addrdlt = (byte *) linktab - (byte *) ldatptr->linktab;
  if (addrdlt != 0)                               /* If array changed of location             */
    gainTablMove (tablptr, addrdlt);              /* Skew gain table pointers into link table */

  for (linknum = ldatptr->linksiz; linknum < (linksiz - 1); linknum ++) /* Initialize new free list of links */
    linktab[linknum].nextidx = linknum + 1;
  linktab[linksiz - 1].nextidx = ldatptr->lfreidx; /* Chain last free cell to preexisting free cells */
  ldatptr->lfreidx  = ldatptr->linksiz;
  ldatptr->lfrenbr += linksiz - ldatptr->linksiz;
  ldatptr->linksiz  = linksiz;
  ldatptr->linktab  = linktab;

  return (0);
}

/*****************************************/
/*                                       */
/* These routines handle the save array. */
/*                                       */
/*****************************************/

/* This routine increases the size of the save array.
** It returns:
** - 0   : if resizing succeeded.
** - !0  : if out of memory.
*/

static
int
wgraphPartFmSaveResize (
WgraphPartFmSaveData * const  sdatptr)
{
  WgraphPartFmSave *  savetab;                    /* Pointer to reallocated array */
  Gnum                savesiz;                    /* Size of reallocated array    */

  savesiz  = sdatptr->savesiz;                    /* Increase array size by 25% */
  savesiz += (savesiz >> 2) + 4;

  if ((savetab = (WgraphPartFmSave *) memRealloc (sdatptr->savetab, savesiz * sizeof (WgraphPartFmLink))) == NULL) {
    errorPrint ("wgraphPartFmSaveResize: out of memory");
    return (1);
  }

  sdatptr->savetab = savetab;
  sdatptr->savesiz = savesiz;

  return (0);
}

/*****************************************/
/*                                       */
/* These routines handle the hash array. */
/*                                       */
/*****************************************/

/* This routine doubles the size all of the arrays
** involved in handling the hash table and hash
** vertex arrays.
** It returns:
** - 0   : if resizing succeeded.
** - !0  : if out of memory.
*/

static
int
wgraphPartFmHashResize (
WgraphPartFmHashData * restrict const hdatptr)    /*+ Pointer to hash data structure +*/
{
  WgraphPartFmVertex *  hashtab;
  Gnum                  hashsiz;
  Gnum                  hashmsk;
  Gnum                  hashnum;
  WgraphPartFmVertex *  hasoptr;
  WgraphPartFmVertex *  lockptr;                  /* Pointer to new locked list */

  hashsiz = hdatptr->hashsiz * 2;                 /* Double hash array size compared to current one */

  if ((hashtab = memAlloc (hashsiz * sizeof (WgraphPartFmVertex))) == NULL) {
    errorPrint ("wgraphPartFmHashResize: out of memory");
    return (1);
  }

  for (hashnum = 0; hashnum < hashsiz; hashnum ++) /* Initialize new hash table */
    hashtab[hashnum].vertnum = ~0;

  hashmsk = hashsiz - 1;                          /* New hash mask                */
  lockptr = &wgraphpartfmvertexdummy;             /* No vertex in locked list yet */
  for (hasoptr = hdatptr->hashtab; hasoptr < hdatptr->hashtab + hdatptr->hashsiz; hasoptr ++) { /* For all old hash slots  */
    WgraphPartFmVertex *  hasnptr;                /* Pointer to new hash slot */
    Gnum                  vertnum;

    vertnum = hasoptr->vertnum;                   /* Get vertex number in old slot   */
    if (vertnum == ~0)                            /* If old hash slot empty, skip it */
      continue;

    for (hashnum = (vertnum * WGRAPHPARTFMHASHPRIME) & hashmsk, hasnptr = hashtab + hashnum; /* Find a proper slot */
         hasnptr->vertnum != ~0; hashnum = (hashnum + 1) & hashmsk, hasnptr = hashtab + hashnum) {
#ifdef SCOTCH_DEBUG_WGRAPH2
      if (hasnptr->vertnum == vertnum) {          /* Item should not already be present */
        errorPrint ("wgraphPartFmHashResize: duplicate hash slot");
        return (1);
      }
#endif /* SCOTCH_DEBUG_WGRAPH2 */
    }

    hasnptr->vertnum = vertnum;                   /* Fill new hash slot */
    hasnptr->partnum = hasoptr->partnum;
    hasnptr->linkidx = hasoptr->linkidx;
    if (hasoptr->nlokptr == NULL)                 /* If vertex is not locked */
      hasnptr->nlokptr = NULL;
    else {                                        /* Else add it to new locked list */
      hasnptr->nlokptr = lockptr;
      lockptr = hasnptr;
    }
    hasnptr->nlstptr = NULL;                      /* Vertices not linked when resizing */
  }

  memFree (hdatptr->hashtab);                     /* Free old hash array */
  hdatptr->hashtab = hashtab;
  hdatptr->hashsiz = hashsiz;
  hdatptr->hashmax = hashsiz / 4;
  hdatptr->hashmsk = hashsiz - 1;
  hdatptr->lockptr = lockptr;

  return (0);
}
