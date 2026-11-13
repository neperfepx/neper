/* Copyright 2012,2015,2018-2021,2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : graph_match.h                           **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declarations   **/
/**                for the centralized source graph        **/
/**                matching routines.                      **/
/**                                                        **/
/**   DATES      : # Version 6.0  : from : 02 oct 2012     **/
/**                                 to   : 21 feb 2020     **/
/**                # Version 7.0  : from : 01 aug 2018     **/
/**                                 to   : 19 jan 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

/** Prime number for cache-friendly perturbations. **/

#define GRAPHMATCHSCANPERTPRIME     179           /* Prime number */

/** Function block building macro. **/

#define GRAPHMATCHFUNCBLOCK(t)      graphMatch##t##NfNe, \
                                    graphMatch##t##NfEl, \
                                    graphMatch##t##FxNe, \
                                    graphMatch##t##FxEl

#define GRAPHMATCHFUNCDECL(t)       static void graphMatch##t##NfNe (GraphCoarsenData * restrict const, GraphCoarsenThread * restrict const); \
                                    static void graphMatch##t##NfEl (GraphCoarsenData * restrict const, GraphCoarsenThread * restrict const); \
                                    static void graphMatch##t##FxNe (GraphCoarsenData * restrict const, GraphCoarsenThread * restrict const); \
                                    static void graphMatch##t##FxEl (GraphCoarsenData * restrict const, GraphCoarsenThread * restrict const)

/*
**  The function prototypes.
*/

#ifdef SCOTCH_GRAPH_MATCH
GRAPHMATCHFUNCDECL (Seq);
#ifndef GRAPHMATCHNOTHREAD
GRAPHMATCHFUNCDECL (Thr);
#endif /* GRAPHMATCHNOTHREAD */
#endif /* SCOTCH_GRAPH_MATCH */

int                         graphMatchInit      (GraphCoarsenData * restrict, const int);
void                        graphMatch          (ThreadDescriptor * restrict const, GraphCoarsenData * const);
