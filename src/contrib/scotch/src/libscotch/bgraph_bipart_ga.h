/* Copyright 2023 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_ga.h                      **/
/**                                                        **/
/**   AUTHOR     : Connor MAYON                            **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the function       **/
/**                declarations for the genetic algorithm  **/
/**                bipartitioning method.                  **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 15 feb 2023     **/
/**                                 to   : 31 may 2023     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

/*
**  The type and structure definitions.
*/

/*+ Method parameters. +*/

typedef struct BgraphBipartGaParam_ {
  INT                       passnbr;              /*+ Number of generations +*/
  INT                       popunbr;              /*+ Number of individuals +*/
} BgraphBipartGaParam;

/*+ The thread-specific data block. +*/

typedef struct BgraphBipartGaThread_ {
  Gnum                      fronnnd[2];           /*+ After-last frontier vertex index; [2] for scan +*/
  Gnum                      compload1[2];         /*+ State return values to aggregate               +*/
  Gnum                      compsize1[2];
  Gnum                      commloadextn[2];
  Gnum                      commloadintn[2];
  Gnum                      commgainextn[2];
  Gnum                      veexsum;              /*+ Area for reducing sums of external gains       +*/
  Gnum                      veexsum1;
} BgraphBipartGaThread;

/*+ The sort structure. +*/

typedef struct BgraphBipartGaSort_ {
  Gnum                      cmloval;              /*+ Communication load               +*/
  Gnum                      cplodlt;              /*+ Absolute value of load imbalance +*/
  GraphPart *               parttab;              /*+ Pointer to part array            +*/
} BgraphBipartGaSort;

/*+ The loop routine parameter
    structure. It contains the
    thread-independent data.   +*/

typedef struct BgraphBipartGaData_ {
  Bgraph *                  grafptr;              /*+ Graph to work on                             +*/
  BgraphBipartGaSort *      sorttab;              /*+ Shared array to sort individualsby fitness   +*/
  Gnum *                    matetab;              /*+ Mating array, of pairs of indices in sorttab +*/
  BgraphBipartGaThread *    thrdtab;              /*+ Array of thread-specific data                +*/
  INT                       passnbr;              /*+ Number of generations                        +*/
  INT                       popunbr;              /*+ Number of individuals per thread             +*/
  INT                       abrtval;              /*+ Abort value                                  +*/
} BgraphBipartGaData;

/*
**  The function prototypes.
*/

int                         bgraphBipartGa      (Bgraph * restrict const, const BgraphBipartGaParam * const);
