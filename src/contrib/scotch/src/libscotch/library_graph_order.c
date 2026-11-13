/* Copyright 2004,2007,2008,2010,2012-2014,2018,2019,2023-2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_order.c                   **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the graph    **/
/**                ordering routines of the libSCOTCH      **/
/**                library.                                **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 19 aug 1998     **/
/**                                 to   : 22 aug 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 27 mar 1999     **/
/**                # Version 4.0  : from : 29 jan 2002     **/
/**                                 to   : 08 sep 2006     **/
/**                # Version 5.0  : from : 19 dec 2006     **/
/**                                 to   : 04 aug 2007     **/
/**                # Version 5.1  : from : 30 oct 2007     **/
/**                                 to   : 14 aug 2010     **/
/**                # Version 6.0  : from : 08 jan 2012     **/
/**                                 to   : 29 sep 2019     **/
/**                # Version 7.0  : from : 07 may 2019     **/
/**                                 to   : 08 feb 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "context.h"
#include "parser.h"
#include "graph.h"
#include "order.h"
#include "hgraph.h"
#include "hgraph_order_st.h"
#include "library_order.h"
#include "scotch.h"

/************************************/
/*                                  */
/* These routines are the C API for */
/* the graph ordering routines.     */
/*                                  */
/************************************/

/* This routine initializes an API ordering
** with respect to the given source graph
** and the locations of output parameters.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderInit (
const SCOTCH_Graph * const  grafptr,              /*+ Graph to order                     +*/
SCOTCH_Ordering * const     ordeptr,              /*+ Ordering structure to initialize   +*/
SCOTCH_Num * const          permtab,              /*+ Direct permutation array           +*/
SCOTCH_Num * const          peritab,              /*+ Inverse permutation array          +*/
SCOTCH_Num * const          cblkptr,              /*+ Pointer to number of column blocks +*/
SCOTCH_Num * const          rangtab,              /*+ Column block range array           +*/
SCOTCH_Num * const          treetab)              /*+ Separator tree array               +*/
{
#ifdef SCOTCH_DEBUG_LIBRARY1
  if (sizeof (SCOTCH_Ordering) < sizeof (LibOrder)) {
    errorPrint (STRINGIFY (SCOTCH_graphOrderInit) ": internal error");
    return (1);
  }
#endif /* SCOTCH_DEBUG_LIBRARY1 */

  const Graph * const srcgrafptr = (Graph *) CONTEXTOBJECT (grafptr); /* Use structure as source graph */
  LibOrder * const    libordeptr = (LibOrder *) ordeptr;

  libordeptr->permtab = ((permtab == NULL) || ((void *) permtab == (void *) grafptr)) ? NULL : (Gnum *) permtab;
  libordeptr->peritab = ((peritab == NULL) || ((void *) peritab == (void *) grafptr)) ? NULL : (Gnum *) peritab;
  libordeptr->cblkptr = ((cblkptr == NULL) || ((void *) cblkptr == (void *) grafptr)) ? NULL : (Gnum *) cblkptr;
  libordeptr->rangtab = ((rangtab == NULL) || ((void *) rangtab == (void *) grafptr)) ? NULL : (Gnum *) rangtab;
  libordeptr->treetab = ((treetab == NULL) || ((void *) treetab == (void *) grafptr)) ? NULL : (Gnum *) treetab;

  return (orderInit (&libordeptr->o, srcgrafptr->baseval, srcgrafptr->vertnbr, libordeptr->peritab));
}

/* This routine frees an API ordering.
** It returns:
** - void  : in all cases.
*/

void
SCOTCH_graphOrderExit (
const SCOTCH_Graph * const  grafptr,
SCOTCH_Ordering * const     ordeptr)
{
  orderExit (&((LibOrder *) ordeptr)->o);
}

/* This routine loads the contents of
** the given ordering from the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderLoad (
const SCOTCH_Graph * const        grafptr,        /*+ Graph to order   +*/
SCOTCH_Ordering * restrict const  ordeptr,        /*+ Ordering to load +*/
FILE * restrict const             stream)         /*+ Output stream    +*/
{
  const Graph * const srcgrafptr = (Graph *) CONTEXTOBJECT (grafptr); /* Use structure as source graph */
  LibOrder * const    libordeptr = (LibOrder *) ordeptr;

  if (orderLoad (&libordeptr->o, srcgrafptr->vlbltax, stream) != 0)
    return (1);

  if (libordeptr->permtab != NULL)                /* Build inverse permutation if wanted */
    orderPeri (libordeptr->o.peritab, srcgrafptr->baseval, libordeptr->o.vnodnbr, libordeptr->permtab, srcgrafptr->baseval);

  return (0);
}

/* This routine saves the contents of
** the given ordering to the given stream.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderSave (
const SCOTCH_Graph * const    grafptr,            /*+ Graph to order   +*/
const SCOTCH_Ordering * const ordeptr,            /*+ Ordering to save +*/
FILE * const                  stream)             /*+ Output stream    +*/
{
  const Graph * const srcgrafptr = (Graph *) CONTEXTOBJECT (grafptr);

  return (orderSave (&((LibOrder *) ordeptr)->o, srcgrafptr->vlbltax, stream));
}

/* This routine saves to the given stream
** the mapping data associated with the
** given ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderSaveMap (
const SCOTCH_Graph * const    grafptr,            /*+ Graph to order   +*/
const SCOTCH_Ordering * const ordeptr,            /*+ Ordering to save +*/
FILE * const                  stream)             /*+ Output stream    +*/
{
  const Graph * const srcgrafptr = (Graph *) CONTEXTOBJECT (grafptr);

  return (orderSaveMap (&((LibOrder *) ordeptr)->o, srcgrafptr->vlbltax, stream));
}

/* This routine saves to the given stream
** the separator tree data associated with
** the given ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderSaveTree (
const SCOTCH_Graph * const    grafptr,            /*+ Graph to order   +*/
const SCOTCH_Ordering * const ordeptr,            /*+ Ordering to save +*/
FILE * const                  stream)             /*+ Output stream    +*/
{
  const Graph * const srcgrafptr = (Graph *) CONTEXTOBJECT (grafptr);

  return (orderSaveTree (&((LibOrder *) ordeptr)->o, srcgrafptr->vlbltax, stream));
}

/* This routine computes an ordering
** of the API ordering structure with
** respect to the given strategy.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderCompute (
SCOTCH_Graph * const        grafptr,              /*+ Graph to order      +*/
SCOTCH_Ordering * const     ordeptr,              /*+ Ordering to compute +*/
SCOTCH_Strat * const        straptr)              /*+ Ordering strategy   +*/
{
  return (SCOTCH_graphOrderComputeList (grafptr, ordeptr, ((Graph *) CONTEXTOBJECT (grafptr))->vertnbr, NULL, straptr));
}

/* This routine computes a partial ordering
** of the listed vertices of the API ordering
** structure graph with respect to the given
** strategy.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderComputeList (
SCOTCH_Graph * const        libgrafptr,           /*+ Graph to order                  +*/
SCOTCH_Ordering * const     ordeptr,              /*+ Ordering to compute             +*/
const SCOTCH_Num            listnbr,              /*+ Number of vertices in list      +*/
const SCOTCH_Num * const    listtab,              /*+ List of vertex indices to order +*/
SCOTCH_Strat * const        straptr)              /*+ Ordering strategy               +*/
{
  Hgraph              halgrafdat;                 /* Halo source graph structure     */
  Hgraph              halgraftmp;                 /* Halo source graph structure     */
  Hgraph *            halgrafptr;                 /* Pointer to halo graph structure */
  CONTEXTDECL        (libgrafptr);
  const Strat *       ordstraptr;                 /* Pointer to ordering strategy    */
  OrderCblk *         cblkptr;
  int                 o;

  o = 1;                                          /* Assume an error */

  if (CONTEXTINIT (libgrafptr)) {
    errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": cannot initialize context");
    return (o);
  }

  const Graph * const srcgrafptr = (Graph *) CONTEXTGETOBJECT (libgrafptr);
  LibOrder * const    libordeptr = (LibOrder *) ordeptr;

#ifdef SCOTCH_DEBUG_LIBRARY1
  if ((listnbr < 0) || (listnbr > srcgrafptr->vertnbr)) {
    errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": invalid parameters (1)");
    goto fail;
  }
#endif /* SCOTCH_DEBUG_LIBRARY1 */
#ifdef SCOTCH_DEBUG_LIBRARY2
  if (graphCheck (srcgrafptr) != 0) {
    errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": invalid input graph");
    goto fail;
  }
#endif /* SCOTCH_DEBUG_LIBRARY2 */

  if (listnbr == 0) {                             /* If empty list, return identity permutation */
    intAscn (libordeptr->o.peritab, srcgrafptr->vertnbr, srcgrafptr->baseval);
    o = 0;                                        /* Not an error */
    goto skip;
  }

  if (*((Strat **) straptr) == NULL) {            /* Set default ordering strategy if necessary */
    if (SCOTCH_stratGraphOrderBuild (straptr, SCOTCH_STRATQUALITY, 0, 0.2))
      goto fail;
  }

  ordstraptr = *((Strat **) straptr);
  if (ordstraptr->tablptr != &hgraphorderststratab) {
    errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": not a sequential graph ordering strategy");
    goto fail;
  }

  memCpy (&halgrafdat.s, srcgrafptr, sizeof (Graph)); /* Copy non-halo graph data   */
  halgrafdat.s.flagval &= ~GRAPHFREETABS;         /* Do not allow to free arrays    */
  halgrafdat.s.edlotax  = NULL;                   /* Never mind about edge loads    */
  halgrafdat.s.vlbltax  = NULL;                   /* Do not propagate vertex labels */
  hgraphUnhalo2 (&halgrafdat);                    /* Set default halo members       */
  halgrafdat.levlnum = 0;                         /* No nested dissection yet       */
  halgrafdat.contptr = CONTEXTGETDATA (libgrafptr);

  if (listnbr == srcgrafptr->vertnbr) {           /* If work on full graph */
    halgrafptr = &halgrafdat;
    cblkptr    = &libordeptr->o.rootdat;
  }
  else {
    Gnum * restrict       peritax;
    Gnum                  listnum;
    Gnum                  vertnum;
    Gnum                  halonum;

    if ((cblkptr = (OrderCblk *) memAlloc (2 * sizeof (OrderCblk))) == NULL) {
      errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": out of memory");
      goto fail;
    }
    libordeptr->o.treenbr = 3;
    libordeptr->o.cblknbr = 2;
    libordeptr->o.rootdat.typeval = ORDERCBLKDICO; /* Node is a set of two disconnected components */
    libordeptr->o.rootdat.vnodnbr = srcgrafptr->vertnbr;
    libordeptr->o.rootdat.cblknbr = 2;
    libordeptr->o.rootdat.cblktab = cblkptr;

    cblkptr[0].typeval = ORDERCBLKLEAF;           /* Build column blocks */
    cblkptr[0].vnodnbr = listnbr;
    cblkptr[0].cblknbr = 0;
    cblkptr[0].cblktab = NULL;
    cblkptr[1].typeval = ORDERCBLKLEAF;
    cblkptr[1].vnodnbr = srcgrafptr->vertnbr - listnbr;
    cblkptr[1].cblknbr = 0;
    cblkptr[1].cblktab = NULL;

    memSet (libordeptr->o.peritab, 0, srcgrafptr->vertnbr * sizeof (Gnum)); /* Fill inverse permutation with dummy values */
    for (listnum = 0, peritax = libordeptr->o.peritab - srcgrafptr->baseval;
         listnum < listnbr; listnum ++) {
#ifdef SCOTCH_DEBUG_LIBRARY2
      if ((listtab[listnum] <  srcgrafptr->baseval) ||
          (listtab[listnum] >= srcgrafptr->vertnnd)) {
        errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": invalid parameters (2)");
        goto fail;
      }
#endif /* SCOTCH_DEBUG_LIBRARY2 */
      peritax[listtab[listnum]] = ~0;             /* TRICK: use peritab as flag array to mark used vertices */
    }
    for (vertnum = halonum = srcgrafptr->vertnnd - 1; vertnum >= srcgrafptr->baseval; vertnum --) {
      if (peritax[vertnum] == 0)
        peritax[halonum --] = vertnum;
    }
#ifdef SCOTCH_DEBUG_LIBRARY2
    if (halonum != (listnbr + srcgrafptr->baseval - 1)) {
      errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": internal error");
      goto fail;
    }
#endif /* SCOTCH_DEBUG_LIBRARY2 */

    if (hgraphInduceList (&halgrafdat, listnbr, (Gnum * const) listtab, srcgrafptr->vertnbr - listnbr, &halgraftmp) != 0) {
      errorPrint (STRINGIFY (SCOTCH_graphOrderComputeList) ": cannot create induced subgraph");
      goto fail;
    }
    halgrafptr = &halgraftmp;
  }

  o = hgraphOrderSt (halgrafptr, &libordeptr->o, 0, cblkptr, ordstraptr);

  if (halgrafptr != &halgrafdat)                  /* If induced subgraph created */
    hgraphExit (halgrafptr);                      /* Free it                     */

  if (o != 0)
    goto fail;

skip:
#ifdef SCOTCH_DEBUG_LIBRARY2
  if (orderCheck (&libordeptr->o) != 0) {
    o = 1;
    goto fail;
  }
#endif /* SCOTCH_DEBUG_LIBRARY2 */

  if (libordeptr->permtab != NULL)                /* Build direct permutation if wanted */
    orderPeri (libordeptr->o.peritab, srcgrafptr->baseval, libordeptr->o.vnodnbr, libordeptr->permtab, srcgrafptr->baseval);
  if (libordeptr->rangtab != NULL)                /* Build range array if column block data wanted */
    orderRang (&libordeptr->o, libordeptr->rangtab);
  if (libordeptr->treetab != NULL)                /* Build separator tree array if wanted */
      orderTree (&libordeptr->o, libordeptr->treetab);
  if (libordeptr->cblkptr != NULL)                /* Set number of column blocks if wanted */
    *(libordeptr->cblkptr) = libordeptr->o.cblknbr;

fail:
  CONTEXTEXIT (libgrafptr);
  return (o);
}

/* This routine computes an ordering
** of the API ordering structure with
** respect to the given strategy.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrder (
SCOTCH_Graph * const        grafptr,              /*+ Graph to order                     +*/
SCOTCH_Strat * const        straptr,              /*+ Ordering strategy                  +*/
SCOTCH_Num * const          permtab,              /*+ Ordering permutation               +*/
SCOTCH_Num * const          peritab,              /*+ Inverse permutation array          +*/
SCOTCH_Num * const          cblkptr,              /*+ Pointer to number of column blocks +*/
SCOTCH_Num * const          rangtab,              /*+ Column block range array           +*/
SCOTCH_Num * const          treetab)              /*+ Separator tree array               +*/
{
  SCOTCH_Ordering     ordedat;
  int                 o;

  if (SCOTCH_graphOrderInit (grafptr, &ordedat, permtab, peritab, cblkptr, rangtab, treetab) != 0)
    return (1);

  o = SCOTCH_graphOrderCompute (grafptr, &ordedat, straptr);
  SCOTCH_graphOrderExit (grafptr, &ordedat);

  return (o);
}

/* This routine computes an ordering
** of the subgraph of the API ordering
** structure graph induced by the given
** vertex list, with respect to the given
** strategy.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderList (
SCOTCH_Graph * const        grafptr,              /*+ Graph to order                     +*/
const SCOTCH_Num            listnbr,              /*+ Number of vertices in list         +*/
const SCOTCH_Num * const    listtab,              /*+ List of vertex indices to order    +*/
SCOTCH_Strat * const        straptr,              /*+ Ordering strategy                  +*/
SCOTCH_Num * const          permtab,              /*+ Ordering permutation               +*/
SCOTCH_Num * const          peritab,              /*+ Inverse permutation array          +*/
SCOTCH_Num * const          cblkptr,              /*+ Pointer to number of column blocks +*/
SCOTCH_Num * const          rangtab,              /*+ Column block range array           +*/
SCOTCH_Num * const          treetab)              /*+ Column block range array           +*/
{
  SCOTCH_Ordering     ordedat;
  int                 o;

  SCOTCH_graphOrderInit (grafptr, &ordedat, permtab, peritab, cblkptr, rangtab, treetab);
  o = SCOTCH_graphOrderComputeList (grafptr, &ordedat, listnbr, listtab, straptr);
  SCOTCH_graphOrderExit (grafptr, &ordedat);

  return (o);
}

/* This routine checks the consistency
** of the given graph ordering.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphOrderCheck (
const SCOTCH_Graph * const    grafptr,
const SCOTCH_Ordering * const ordeptr)            /*+ Ordering to check +*/
{
  return (orderCheck (&((LibOrder *) ordeptr)->o));
}

/* This routine parses the given
** graph ordering strategy.
** It returns:
** - 0   : if string successfully scanned.
** - !0  : on error.
*/

int
SCOTCH_stratGraphOrder (
SCOTCH_Strat * const        straptr,
const char * const          string)
{
  if (*((Strat **) straptr) != NULL)
    stratExit (*((Strat **) straptr));

  if ((*((Strat **) straptr) = stratInit (&hgraphorderststratab, string)) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_stratGraphOrder) ": error in ordering strategy");
    return (1);
  }

  return (0);
}

/* This routine provides predefined
** ordering strategies.
** It returns:
** - 0   : if string successfully initialized.
** - !0  : on error.
*/

int
SCOTCH_stratGraphOrderBuild (
SCOTCH_Strat * const        straptr,              /*+ Strategy to create                 +*/
const SCOTCH_Num            flagval,              /*+ Desired characteristics            +*/
const SCOTCH_Num            levlnbr,              /*+ Number of nested dissection levels +*/
const double                balrat)               /*+ Desired imbalance ratio            +*/
{
  char                bufftab[8192];              /* Should be enough */
  char                levltab[32];
  char                bbaltab[32];
  char *              sepaptr;
  char *              tstsptr;
  char *              oleaptr;
  char *              osepptr;

  sprintf (bbaltab, "%lf", balrat);
  sprintf (levltab, GNUMSTRING, levlnbr);

  sprintf (bufftab, (((flagval & SCOTCH_STRATDISCONNECTED) != 0) ? "o{strat=%s}" : "%s"),
           "c{rat=0.7,cpr=n{sep=/(<TSTS>)?m{rat=0.7,vert=100,low=h{pass=10},asc=b{width=3,bnd=f{bal=<BBAL>},org=(|h{pass=10})f{bal=<BBAL>}}}<SEPA>;,ole=<OLEA>,ose=<OSEP>},unc=n{sep=/(<TSTS>)?m{rat=0.7,vert=100,low=h{pass=10},asc=b{width=3,bnd=f{bal=<BBAL>},org=(|h{pass=10})f{bal=<BBAL>}}}<SEPA>;,ole=<OLEA>,ose=<OSEP>}}");

  switch (flagval & (SCOTCH_STRATLEVELMIN | SCOTCH_STRATLEVELMAX)) {
    case SCOTCH_STRATLEVELMIN :
      tstsptr = "(levl<<LEVL>)|(vert>240)";
      break;
    case SCOTCH_STRATLEVELMAX :
      tstsptr = "(levl<<LEVL>)&(vert>240)";
      break;
    case (SCOTCH_STRATLEVELMIN | SCOTCH_STRATLEVELMAX) :
      tstsptr = "levl<<LEVL>";
      break;
    default :
      tstsptr = "vert>240";
      break;
  }

  sepaptr = ((flagval & SCOTCH_STRATSPEED) != 0)
            ? ""
            : "|m{rat=0.7,vert=100,low=h{pass=10},asc=b{width=3,bnd=f{bal=<BBAL>},org=(|h{pass=10})f{bal=<BBAL>}}}";

  oleaptr = ((flagval & SCOTCH_STRATLEAFSIMPLE) != 0)
            ? "s"
            : "f{cmin=15,cmax=100000,frat=0.0}";

  osepptr = ((flagval & SCOTCH_STRATSEPASIMPLE) != 0)
            ? "s"
            : "g";

  stringSubst (bufftab, "<SEPA>", sepaptr);
  stringSubst (bufftab, "<TSTS>", tstsptr);
  stringSubst (bufftab, "<LEVL>", levltab);
  stringSubst (bufftab, "<OLEA>", oleaptr);
  stringSubst (bufftab, "<OSEP>", osepptr);
  stringSubst (bufftab, "<BBAL>", bbaltab);

  return (SCOTCH_stratGraphOrder (straptr, bufftab));
}
