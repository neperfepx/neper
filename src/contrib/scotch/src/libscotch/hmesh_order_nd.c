/* Copyright 2004,2007,2008,2018-2021,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : hmesh_order_nd.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module orders mesh nodes using the **/
/**                nested dissection algorithm.            **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 06 jan 2002     **/
/**                                 to   : 05 jan 2005     **/
/**                # Version 5.0  : from : 25 jul 2007     **/
/**                                 to   : 12 sep 2007     **/
/**                # Version 5.1  : from : 09 nov 2008     **/
/**                                 to   : 09 nov 2008     **/
/**                # Version 6.0  : from : 15 may 2018     **/
/**                                 to   : 15 may 2018     **/
/**                # Version 7.0  : from : 12 sep 2019     **/
/**                                 to   : 11 sep 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_HMESH_ORDER_ND

#include "module.h"
#include "common.h"
#include "parser.h"
#include "graph.h"
#include "order.h"
#include "mesh.h"
#include "hmesh.h"
#include "hmesh_order_nd.h"
#include "hmesh_order_st.h"
#include "vmesh.h"
#include "vmesh_separate_st.h"

/***************************/
/*                         */
/* Multi-threaded routine. */
/*                         */
/***************************/

static
void
hmeshOrderNd2 (
Context * restrict const        contptr,          /*+ (Sub-)context                          +*/
const int                       spltnum,          /*+ Rank of sub-context in initial context +*/
const HmeshOrderNdSplit * const spltptr)
{
  Hmesh               orgmeshdat;
  Hmesh               indmeshdat;
  int                 o;

  orgmeshdat = *spltptr->meshptr;                 /* Assign new context to work mesh */
  orgmeshdat.contptr = contptr;

  o = 0;
  if ((hmeshInducePart (&orgmeshdat, spltptr->parttax, spltnum, spltptr->splttab[spltnum].velmnbr,
                        spltptr->splttab[spltnum].vnodnbr, spltptr->vnspnbr, &indmeshdat)) != 0) {
    errorPrint ("hmeshOrderNd2: cannot build induced submesh");
    o = 1;
  }
  if (o == 0) {
    o = hmeshOrderNd (&indmeshdat, spltptr->ordeptr, spltptr->splttab[spltnum].ordenum, spltptr->splttab[spltnum].cblkptr, spltptr->paraptr);

    hmeshExit (&indmeshdat);
  }
  if (o != 0) {                                   /* Report any error */
#ifdef SCOTCH_PTHREAD
    pthread_mutex_lock (&spltptr->ordeptr->mutedat); /* Use ordering lock to avoid race condition */
#endif /* SCOTCH_PTHREAD */
    *spltptr->revaptr = 1;
#ifdef SCOTCH_PTHREAD
    pthread_mutex_unlock (&spltptr->ordeptr->mutedat);
#endif /* SCOTCH_PTHREAD */
  }
}

/*****************************/
/*                           */
/* This is the main routine. */
/*                           */
/*****************************/

/* This routine performs the ordering.
** It returns:
** - 0   : if the ordering could be computed.
** - !0  : on error.
*/

int
hmeshOrderNd (
Hmesh * restrict const            meshptr,
Order * restrict const            ordeptr,
const Gnum                        ordenum,
OrderCblk * restrict const        cblkptr,
const HmeshOrderNdParam * const   paraptr)
{
  Vmesh                     nspmeshdat;           /* Node separation mesh */
  Gnum                      vertnbr;
  int                       o;

  if (hmeshMesh (meshptr, &nspmeshdat.m) != 0) {
    errorPrint ("hmeshOrderNd: cannot create node separation mesh");
    return (1);
  }
  nspmeshdat.ecmpsize[0] = nspmeshdat.m.velmnbr;
  nspmeshdat.ecmpsize[1] = 0;
  nspmeshdat.ncmpload[0] = nspmeshdat.m.vnlosum;
  nspmeshdat.ncmpload[1] = 0;
  nspmeshdat.ncmpload[2] = 0;
  nspmeshdat.ncmploaddlt = nspmeshdat.m.vnlosum;
  nspmeshdat.ncmpsize[0] = nspmeshdat.m.vnodnbr;
  nspmeshdat.ncmpsize[1] = 0;
  nspmeshdat.fronnbr     = 0;
  nspmeshdat.levlnum     = meshptr->levlnum;
  nspmeshdat.contptr     = meshptr->contptr;

  vertnbr = nspmeshdat.m.velmnbr + nspmeshdat.m.vnodnbr;
  if (memAllocGroup ((void **) (void *)
                      &nspmeshdat.parttax, (size_t) (vertnbr * sizeof (GraphPart)),
                      &nspmeshdat.frontab, (size_t) (vertnbr * sizeof (Gnum)), NULL) == NULL) {
    errorPrint ("hmeshOrderNd: out of memory (1)");
    return (1);
  }
  memSet (nspmeshdat.parttax, 0, vertnbr * sizeof (GraphPart)); /* Set all vertices to part 0 */
  nspmeshdat.parttax -= nspmeshdat.m.baseval;

  if (vmeshSeparateSt (&nspmeshdat, paraptr->sepstrat) != 0) { /* Separate mesh */
    vmeshExit (&nspmeshdat);
    return (1);
  }

  if ((nspmeshdat.ncmpsize[0] == 0) ||            /* If could not separate more */
      (nspmeshdat.ncmpsize[1] == 0)) {
    vmeshExit (&nspmeshdat);

    return (hmeshOrderSt (meshptr, ordeptr, ordenum, cblkptr, paraptr->ordstratlea)); /* Order this leaf */
  }

  cblkptr->typeval = ORDERCBLKNEDI;               /* Node becomes a nested dissection node */
  if ((cblkptr->cblktab = (OrderCblk *) memAlloc (3 * sizeof (OrderCblk))) == NULL) {
    errorPrint ("hmeshOrderNd: out of memory (2)");
    vmeshExit  (&nspmeshdat);
    return (1);
  }
  cblkptr->cblktab[0].typeval = ORDERCBLKLEAF;    /* Build column blocks */
  cblkptr->cblktab[0].vnodnbr = nspmeshdat.ncmpsize[0];
  cblkptr->cblktab[0].cblknbr = 0;
  cblkptr->cblktab[0].cblktab = NULL;
  cblkptr->cblktab[1].typeval = ORDERCBLKLEAF;
  cblkptr->cblktab[1].vnodnbr = nspmeshdat.ncmpsize[1];
  cblkptr->cblktab[1].cblknbr = 0;
  cblkptr->cblktab[1].cblktab = NULL;
  cblkptr->cblktab[2].vnodnbr = nspmeshdat.fronnbr;
  cblkptr->cblktab[2].cblknbr = 0;
  cblkptr->cblktab[2].cblktab = NULL;

  cblkptr->typeval = ORDERCBLKNEDI;               /* Node becomes a nested dissection node */
  if (nspmeshdat.fronnbr != 0) {                  /* If separator not empty                */
    Hmesh               indmeshdat;

    cblkptr->cblknbr = 3;                         /* It is a three-cell tree node */
#ifdef SCOTCH_PTHREAD
    pthread_mutex_lock (&ordeptr->mutedat);
#endif /* SCOTCH_PTHREAD */
    ordeptr->cblknbr += 2;                        /* Two more column blocks created */
    ordeptr->treenbr += 3;                        /* Three more tree nodes created  */
#ifdef SCOTCH_PTHREAD
    pthread_mutex_unlock (&ordeptr->mutedat);
#endif /* SCOTCH_PTHREAD */

    cblkptr->cblktab[2].typeval = ORDERCBLKLEAF;
    cblkptr->cblktab[2].vnodnbr = nspmeshdat.fronnbr;
    cblkptr->cblktab[2].cblknbr = 0;
    cblkptr->cblktab[2].cblktab = NULL;

    if (meshInduceSepa (&nspmeshdat.m, nspmeshdat.parttax, nspmeshdat.fronnbr, nspmeshdat.frontab, &indmeshdat.m) != 0) {
      errorPrint ("hmeshOrderNd: cannot build induced subgraph (1)");
      memFree    (nspmeshdat.frontab);            /* Free remaining space */
      return (1);
    }
    indmeshdat.vnohnbr = indmeshdat.m.vnodnbr;    /* Fill halo mesh structure of non-halo mesh */
    indmeshdat.vnohnnd = indmeshdat.m.vnodnnd;
    indmeshdat.vehdtax = indmeshdat.m.vendtax;
    indmeshdat.vnhlsum = indmeshdat.m.vnlosum;
    indmeshdat.enohnbr = indmeshdat.m.edgenbr;
    indmeshdat.levlnum = meshptr->levlnum;        /* Separator mesh is at level of original mesh */
    indmeshdat.contptr = meshptr->contptr;

    o = hmeshOrderSt (&indmeshdat, ordeptr, ordenum + nspmeshdat.ncmpsize[0] + nspmeshdat.ncmpsize[1],
                      cblkptr->cblktab + 2, paraptr->ordstratsep);
    hmeshExit (&indmeshdat);
  }
  else {                                          /* Separator is empty         */
    cblkptr->cblknbr = 2;                         /* It is a two-cell tree node */
#ifdef SCOTCH_PTHREAD
    pthread_mutex_lock (&ordeptr->mutedat);
#endif /* SCOTCH_PTHREAD */
    ordeptr->cblknbr ++;                          /* One more column block created */
    ordeptr->treenbr += 2;                        /* Two more tree nodes created   */
#ifdef SCOTCH_PTHREAD
    pthread_mutex_unlock (&ordeptr->mutedat);
#endif /* SCOTCH_PTHREAD */
    o = 0;                                        /* No separator ordering computed */
  }
  if (o == 0) {
    HmeshOrderNdSplit   spltdat;                  /* Parameters for context splitting */

    spltdat.splttab[0].velmnbr = nspmeshdat.ecmpsize[0];
    spltdat.splttab[0].vnodnbr = nspmeshdat.ncmpsize[0];
    spltdat.splttab[0].ordenum = ordenum;
    spltdat.splttab[0].cblkptr = cblkptr->cblktab;
    spltdat.splttab[1].velmnbr = nspmeshdat.ecmpsize[1];
    spltdat.splttab[1].vnodnbr = nspmeshdat.ncmpsize[1];
    spltdat.splttab[1].ordenum = ordenum + nspmeshdat.ncmpsize[0];
    spltdat.splttab[1].cblkptr = cblkptr->cblktab + 1;
    spltdat.meshptr = meshptr;
    spltdat.vnspnbr = nspmeshdat.fronnbr;
    spltdat.parttax = nspmeshdat.parttax;
    spltdat.ordeptr = ordeptr;
    spltdat.paraptr = paraptr;
    spltdat.revaptr = &o;

#ifndef HMESHORDERNDNOTHREAD
    if (contextThreadLaunchSplit (meshptr->contptr, (ContextSplitFunc) hmeshOrderNd2, &spltdat) != 0) /* If counld not split context to run concurrently */
#endif /* HMESHORDERNDNOTHREAD */
    {
      hmeshOrderNd2 (meshptr->contptr, 0, &spltdat); /* Run tasks in sequence */
      if (o == 0)
        hmeshOrderNd2 (meshptr->contptr, 1, &spltdat);
    }
  }

  vmeshExit (&nspmeshdat);

  return (o);
}
