/* Copyright 2010,2014,2018,2019,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : library_graph_part_ovl.c                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module is the API for the graph    **/
/**                partitioning routines with overlap of   **/
/**                the libSCOTCH library.                  **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 28 may 2010     **/
/**                                 to   : 25 apr 2018     **/
/**                # Version 6.1  : from : 02 dec 2021     **/
/**                                 to   : 20 dec 2021     **/
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
#include "arch.h"
#include "wgraph.h"
#include "wgraph_part_st.h"
#include "scotch.h"

/************************************/
/*                                  */
/* These routines are the C API for */
/* graph partitioning with overlap. */
/*                                  */
/************************************/

/* This routine computes a partition with
** overlap of the given graph structure
** with respect to the given strategy.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
SCOTCH_graphPartOvl (
SCOTCH_Graph * const        libgrafptr,           /*+ Graph to map          +*/
const SCOTCH_Num            partnbr,              /*+ Number of parts       +*/
SCOTCH_Strat * const        straptr,              /*+ Partitioning strategy +*/
SCOTCH_Num * const          parttab)              /*+ Partition array       +*/
{
  Wgraph              grafdat;
  const Strat *       partstraptr;
  CONTEXTDECL        (libgrafptr);
  int                 o;

  o = 1;                                          /* Assume an error */

  if (CONTEXTINIT (libgrafptr)) {
    errorPrint (STRINGIFY (SCOTCH_graphPartOvl) ": cannot initialize context");
    return     (o);
  }

  if (*((Strat **) straptr) == NULL) {            /* Set default partitioning strategy if necessary */
    if (SCOTCH_stratGraphPartOvlBuild (straptr, SCOTCH_STRATQUALITY, (Gnum) partnbr, (double) 0.05))
      goto fail;
  }

  partstraptr = *((Strat **) straptr);
  if (partstraptr->tablptr != &wgraphpartststratab) {
    errorPrint (STRINGIFY (SCOTCH_graphPartOvl) ": not a sequential graph partitioning with overlap strategy");
    goto fail;
  }

  wgraphInit (&grafdat, (Graph *) CONTEXTGETOBJECT (libgrafptr), partnbr); /* Initialize graph from given graph */
  grafdat.parttax = ((Gnum *) parttab) - grafdat.s.baseval; /* Directly use given part array                    */
  grafdat.levlnum = 0;
  grafdat.contptr = CONTEXTGETDATA (libgrafptr);

  if (wgraphAlloc (&grafdat) != 0) {              /* Always allocate graph data when calling */
    errorPrint (STRINGIFY (SCOTCH_graphPartOvl) ": out of memory");
    goto fail;
  }

  o = wgraphPartSt (&grafdat, partstraptr);

  wgraphExit (&grafdat);

fail:
  CONTEXTEXIT (libgrafptr);
  return (o);
}

/* This routine parses the given
** partitioning strategy.
** It returns:
** - 0   : if string successfully scanned.
** - !0  : on error.
*/

int
SCOTCH_stratGraphPartOvl (
SCOTCH_Strat * const        straptr,
const char * const          string)
{
  if (*((Strat **) straptr) != NULL)
    stratExit (*((Strat **) straptr));

  if ((*((Strat **) straptr) = stratInit (&wgraphpartststratab, string)) == NULL) {
    errorPrint (STRINGIFY (SCOTCH_stratGraphPartOvl) ": error in sequential overlap partitioning strategy");
    return (1);
  }

  return (0);
}

/* This routine provides predefined
** overlap partitioning strategies.
** It returns:
** - 0   : if string successfully initialized.
** - !0  : on error.
*/

int
SCOTCH_stratGraphPartOvlBuild (
SCOTCH_Strat * const        straptr,              /*+ Strategy to create            +*/
const SCOTCH_Num            flagval,              /*+ Desired characteristics       +*/
const SCOTCH_Num            partnbr,              /*+ Number of expected parts/size +*/
const double                balrat)               /*+ Desired imbalance ratio       +*/
{
  char                bufftab[8192];              /* Should be enough */
  char                kbaltab[64];

  sprintf (kbaltab, "%lf", balrat);

  if ((flagval & SCOTCH_STRATRECURSIVE) != 0)
    strcpy (bufftab, "<RECU>");                   /* Use only the recursive bipartitioning framework */
  else
    sprintf (bufftab, "m{vert=%ld,low=<RECU>,asc=f{bal=<KBAL>}}", (long) (20 * partnbr));
  stringSubst (bufftab, "<RECU>", "r{sep=m{rat=0.7,vert=100,low=h{pass=10},asc=b{width=3,bnd=f{bal=<KBAL>},org=(|h{pass=10})f{bal=<KBAL>}}}|m{rat=0.7,vert=100,low=h{pass=10},asc=b{width=3,bnd=f{bal=<KBAL>},org=(|h{pass=10})f{bal=<KBAL>}}}}");
  stringSubst (bufftab, "<KBAL>", kbaltab);

  return (SCOTCH_stratGraphPartOvl (straptr, bufftab));
}
