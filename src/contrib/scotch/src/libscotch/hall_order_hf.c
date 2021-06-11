/* This file is part of the Scotch distribution.
** It does not include the standard Scotch header because it is an
** adaptation of the MUMPS_HAMF4 routine which was released under
** the BSD3 license.
** Consequently, this file is distributed according to the terms of
** the BSD3 licence, see copyright notice below.
*/
/* Copyright 2004,2007,2010,2012,2018-2020 IPB, Universite de Bordeaux, INRIA & CNRS & others
*/
/************************************************************/
/**                                                        **/
/**   NAME       : hall_order_hf.c                         **/
/**                                                        **/
/**   AUTHOR     : Patrick AMESTOY & al.                   **/
/**                Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module orders a halo graph or mesh **/
/**                structure using the block-oriented Halo **/
/**                Approximate (Multiple) Minimum Fill     **/
/**                algorithm, with super-variable          **/
/**                accounting HAMF4 (v20190828).           **/
/**                                                        **/
/**   DATES      : # Version 3.4  : from : 15 may 2001     **/
/**                                 to   : 23 nov 2001     **/
/**                # Version 4.0  : from : 10 jan 2003     **/
/**                                 to   : 29 aug 2007     **/
/**                # Version 5.1  : from : 08 dec 2010     **/
/**                                 to   : 08 dec 2010     **/
/**                # Version 6.0  : from : 08 mar 2012     **/
/**                                 to   : 30 apr 2018     **/
/**                # Version 6.1  : from : 29 oct 2019     **/
/**                                 to   : 10 feb 2020     **/
/**                                                        **/
/**   NOTES      : # This module contains pieces of code   **/
/**                  that belong to other people; see      **/
/**                  below.                                **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define HALL_ORDER_HF

#if 1                                             /* Take simplest version; do not care for exact halfway case specification */
#define anint(x)                    floor ((x) + 0.5)
#else
#define anint(x)                    (((x) > 0) ? floor ((x) + 0.5) : ceil ((x) - 0.5))
#endif

#include "module.h"
#include "common.h"
#include "graph.h"
#include "hall_order_hf.h"

/*  -- translated by f2c (version 19970219).       */
/*  -- hand-made adaptation (as of HAMF 20191101). */
/*  -- hand-made adaptation (as of HAMF 20200111). */

/** -------------------------------------------------------------------- **/
/** Version generated on October 29th 2019                               **/
/**                                                                      **/
/** This file includes various modifications of an original              **/
/** LGPL/ CeCILL-C compatible                                            **/
/** code implementing the AMD (Approximate Minimum Degree) ordering      **/
/**   Patrick Amestoy, Timothy A. Davis, and Iain S. Duff,               **/
/**    "An approximate minimum degree ordering algorithm,"               **/
/**    SIAM J. Matrix Analysis  vol 17, pages=886--905 (1996)            **/
/**    MUMPS_ANA_H is based on the original AMD code:                    **/
/**                                                                      **/
/**    AMD, Copyright (c), 1996-2016, Timothy A. Davis,                  **/
/**    Patrick R. Amestoy, and Iain S. Duff.  All Rights Reserved.       **/
/**    Used in MUMPS under the BSD 3-clause license.                     **/
/**    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            **/
/**    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       **/
/**    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          **/
/**    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          **/
/**    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE     **/
/**    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR      **/
/**    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,             **/
/**    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,   **/
/**    OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   **/
/**    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR    **/
/**    TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT   **/
/**    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY   **/
/**    OF SUCH DAMAGE.                                                   **/
/**                                                                      **/
/**   MUMPS_HAMF4 is a major modification of AMD                         **/
/**   since the metric used to select pivots in not anymore the          **/
/**   degree but an approximation of the fill-in.                        **/
/**   In this approximation                                              **/
/**   all cliques of elements adjacent to the variable are deducted.     **/
/**   MUMPS_HAMF4 also enables to take into account a halo in the graph. **/
/**   The graph is composed is partitioned in two types of nodes         **/
/**   the so called internal nodes and the so called halo nodes.         **/
/**   Halo nodes cannot be selected the both the initial degrees         **/
/**   and updated degrees of internal node should be taken               **/
/**   into account.                                                      **/
/**   Written by Patrick Amestoy between 1999 and 2019.                  **/
/**   and used by F. Pellegrini in SCOTCH since 2000.                    **/
/**                                                                      **/
/**   Unique version to order both graph of variables and                **/
/**   graphs with both elements and variables.                           **/
/**                                                                      **/
/**   Notations used:                                                    **/
/**   Let us refer to as                                                 **/
/**     Gv a graph with only variables                                   **/
/**     Ge a graph with both variables and elements                      **/
/**                                                                      **/
/**   Let V be the set of nodes in the graph                             **/
/**       V = Ve + V0 + V1                                               **/
/**           V0 = Set of variable nodes (not in halo)                   **/
/**           V1 = Set of variable nodes (in halo)                       **/
/**           Ve = Set of element nodes |Ve|=nbelts                      **/
/**                                                                      **/
/**   All 3 sets are disjoint, Ve and V1 can be empty                    **/
/**   If nbelts>0 then a bipartite graph bewteen                         **/
/**      (V0 U V1) and Ve is provided on entry.                          **/
/**                                                                      **/
/**   A graph of elements and variables is a bipartite                   **/
/**   graph between the set of variables (that may                       **/
/**   include halo variables) and the set of elements.                   **/
/**   Thus variables are only adjacent to elements and                   **/
/**   in the element list we only have variables.                        **/
/**   Elements are "considered" as already eliminated and                **/
/**   are provided here only to describe the adjacency between           **/
/**   variables. Only variables in V0 need to be eliminated.             **/
/**                                                                      **/
/**   Comments relative to 64/32 bits integer representation:            **/
/**   This is [based on] a restrictive integer 64 bit variant:           **/
/**   It is assumed that IW array size can exceed 32-bit integer and     **/
/**   thus IWLEN is INTEGER(8) and PE is an INTEGER(8) array.            **/
/**   Graph size n must be smaller than 2x10^9 but number of             **/
/**   edges is a 64-bit integer.                                         **/
/**                                                                      **/
/** Parameters                                                           **/
/**    Input only, not modified on output :                              **/
/**      N : number of nodes in the complete graph including halo        **/
/**          N = size of V0 U V1                                         **/
/**      NBBUCK : should be greater than norig                           **/
/**               advised value is 2*norig                               **/
/**      IWLEN : Should be uint64_t # Patch FP: used Gnum as well #      **/
/**    Input, undefined on output:                                       **/
/**      LEN(1..N)                                                       **/
/**      IW(1..IWLEN)                                                    **/
/**      NV(N) : also meaningful as input to encode compressed graphs    **/
/**    Output only :                                                     **/
/**      NCMPA                                                           **/
/**      not ELEN(N)  # Patch FP: always initialized before call #       **/
/**      LAST(N)                                                         **/
/**    Input/output                                                      **/
/**      ELEN(N)  # Patch PA+FP #                                        **/
/**      PFREE                                                           **/
/**      PE(N) : Should be uint64_t # Patch FP: used Gnum as well #      **/
/**    Internal workspace only                                           **/
/**      Min fill approximation one extra array of size NBBUCK+2         **/
/**       is also needed                                                 **/
/**      NEXT(N)                                                         **/
/**      DEGREE(N)                                                       **/
/**      W(N)                                                            **/
/**      HEAD(0:NBBUCK+1)                                                **/
/**      WF(N)                                                           **/
/**                                                                      **/
/**  Comments on the OUTPUT :                                            **/
/**  ------------------------                                            **/
/**  Let V= V0 U V1 the nodes of the initial graph (|V|=n).              **/
/**  The assembly tree corresponds to the tree                           **/
/**    of the supernodes (or supervariables). Each node of the           **/
/**    assembly tree is then composed of one principal variable          **/
/**    and a list of secondary variables. The list of                    **/
/**    variable of a node (principal + secondary variables) then         **/
/**    describes the structure of the diagonal bloc of the supernode.    **/
/**  The elimination tree denotes the tree of all the variables(=node)   **/
/**    and is therefore of order n.                                      **/
/**                                                                      **/
/**  The arrays NV(N) and PE(N) give a description of the assembly tree. **/
/**                                                                      **/
/**   1/ Description of array nv(N) (on OUTPUT)                          **/
/**    nv(i)=0 i is a secondary variable                                 **/
/**    nv(i) >0 i is a principal variable, nv(i) holds the number of     **/
/**      elements in column i of L (true degree of i)                    **/
/**    nv(i) can be greater than N since degree can be as large as NORIG.**/
/**                                                                      **/
/**   2/ Description of array PE(N) (on OUTPUT)                          **/
/**       Note that on                                                   **/
/**       pe(i) = -(father of variable/node i) in the elimination tree:  **/
/**       If nv (i) .gt. 0, then i represents a node in the assembly     **/
/**       tree, and the parent of i is -pe (i), or zero if i is a root.  **/
/**       If nv (i) = 0, then (i,-pe (i)) represents an edge in a        **/
/**       subtree, the root of which is a node in the assembly tree.     **/
/**                                                                      **/
/**   3/ Example:                                                        **/
/**      Let If be a root node father of Is in the assembly tree.        **/
/**      If is the principal variable of the node If and let If1, If2,   **/
/**      If3 be the secondary variables of node If.                      **/
/**      Is is the principal variable of the node Is and let Is1, Is2 be **/
/**      the secondary variables of node Is.                             **/
/**                                                                      **/
/**      THEN:                                                           **/
/**        NV(If1) = NV(If2) = NV(If3) = 0  (secondary variables)        **/
/**        NV(Is1) = NV(Is2) = 0  (secondary variables)                  **/
/**        NV(If) > 0  (principal variable)                              **/
/**        NV(Is) > 0  (principal variable)                              **/
/**        PE(If) = 0 (root node)                                        **/
/**        PE(Is) = -If (If is the father of Is in the assembly tree)    **/
/**        PE(If1)=PE(If2)=PE(If3)= -If  ( If is the principal variable) **/
/**        PE(Is1)=PE(Is2)= -Is  ( Is is the principal variable)         **/
/**                                                                      **/
/**                                                                      **/
/**  Comments on the OUTPUT:                                             **/
/**  ----------------------                                              **/
/**  Let V= V0 U V1 the nodes of the initial graph (|V|=n).              **/
/**  The assembly tree corresponds to the tree                           **/
/**    of the supernodes (or supervariables). Each node of the           **/
/**    assembly tree is then composed of one principal variable          **/
/**    and a list of secondary variables. The list of                    **/
/**    variable of a node (principal + secondary variables) then         **/
/**    describes the structure of the diagonal block of the              **/
/**    supernode.                                                        **/
/**  The elimination tree denotes the tree of all the variables(=node)   **/
/**    and is therefore of order n.                                      **/
/**                                                                      **/
/**  The arrays NV(N) and PE(N) give a description of the                **/
/**  assembly tree.                                                      **/
/**                                                                      **/
/**   1/ Description of array nv(N) (on OUTPUT)                          **/
/**    nv(i)=0 i is a secondary variable                                 **/
/**    nv(i) >0 i is a principal variable, nv(i) holds the number of     **/
/**      elements in column i of L (true degree of i)                    **/
/**    with compressed graph (nv(1).ne.-1 on input), nv(i) can be        **/
/**    greater than N since degree can be as large as NORIG.             **/
/**                                                                      **/
/**   2/ Description of array PE(N) (on OUTPUT)                          **/
/**       Note that on                                                   **/
/**       pe(i) = -(father of variable/node i) in the elimination tree:  **/
/**       If nv (i) .gt. 0, then i represents a node in the assembly     **/
/**       tree, and the parent of i is -pe (i), or zero if i is a root.  **/
/**       If nv (i) = 0, then (i,-pe (i)) represents an edge in a        **/
/**       subtree, the root of which is a node in the assembly tree.     **/
/**                                                                      **/
/**   3/ Example:                                                        **/
/**      Let If be a root node father of Is in the assembly tree.        **/
/**      If is the principal                                             **/
/**      variable of the node If and let If1, If2, If3 be the secondary  **/
/**      variables of node If.                                           **/
/**      Is is the principal                                             **/
/**      variable of the node Is and let Is1, Is2 be the secondary       **/
/**      variables of node Is.                                           **/
/**                                                                      **/
/**      THEN:                                                           **/
/**        NV(If1)=NV(If2)=NV(If3) = 0  (secondary variables)            **/
/**        NV(Is1)=NV(Is2) = 0  (secondary variables)                    **/
/**        NV(If) > 0  ( principal variable)                             **/
/**        NV(Is) > 0  ( principal variable)                             **/
/**        PE(If)  = 0 (root node)                                       **/
/**        PE(Is)  = -If (If is the father of Is in the assembly tree)   **/
/**        PE(If1)=PE(If2)=PE(If3)= -If  ( If is the principal variable) **/
/**        PE(Is1)=PE(Is2)= -Is  ( Is is the principal variable)         **/
/**                                                                      **/
/**                                                                      **/
/**                                                                      **/
/** HALOAMD_V1: (September 1997)                                         **/
/** **********                                                           **/
/** Initial version designed to experiment the numerical (fill-in)       **/
/** impact of taking into account the halo. This code should be able     **/
/** to experiment no-halo, partial halo, complete halo.                  **/
/** DATE: September 17th 1997                                            **/
/**                                                                      **/
/** HALOAMD is designed to process a gragh composed of two types         **/
/**            of nodes, V0 and V1, extracted from a larger gragh.       **/
/**            V0^V1 = {},                                               **/
/**                                                                      **/
/**            We used Min. degree heuristic to order only               **/
/**            nodes in V0, but the adjacency to nodes                   **/
/**            in V1 is taken into account during ordering.              **/
/**            Nodes in V1 are odered at last.                           **/
/**            Adjacency between nodes of V1 need not be provided,       **/
/**            however |len(i)| must always corresponds to the number    **/
/**            of edges effectively provided in the adjacency list of i. **/
/**          On input :                                                  **/
/**          ********                                                    **/
/**            Nodes INODE in V1 are flagged with len(INODE) = -degree   **/
/**                           if len(i) =0 and i \in V1 then             **/
/**                           len(i) must be set on input to -NORIG-1    **/
/**          ERROR return (negative values in ncmpa)                     **/
/**          ************                                                **/
/**            negative value in ncmpa indicates an error detected       **/
/**               by HALOAMD.                                            **/
/**                                                                      **/
/**            The graph provided MUST follow the rule:                  **/
/**             if (i,j) is an edge in the gragh then                    **/
/**             j must be in the adjacency list of i AND                 **/
/**             i must be in the adjacency list of j.                    **/
/**    REMARKS                                                           **/
/**    -------                                                           **/
/**                                                                      **/
/**        1/  Providing edges between nodes of V1 should not            **/
/**            affect the final ordering, only the amount of edges       **/
/**            of the halo should effectively affect the solution.       **/
/**            This code should work in the following cases:             **/
/**              1/ halo not provided                                    **/
/**              2/ halo partially provided                              **/
/**              3/ complete halo                                        **/
/**              4/ complete halo+interconnection between nodes of V1.   **/
/**                                                                      **/
/**              1/ should run and provide identical results (w.r.t to   **/
/**                 current implementation of AMD in SCOTCH).            **/
/**              3/ and 4/ should provide identical results.             **/
/**                                                                      **/
/**        2/ All modifications of the AMD initial code are indicated    **/
/**           with begin HALO .. end HALO                                **/
/**                                                                      **/
/**                                                                      **/
/**   Given a representation of the nonzero pattern of a symmetric       **/
/**       matrix A, (excluding the diagonal) perform an approximate      **/
/**       minimum fill-in heuristic. Aggresive absorption is             **/
/**       used to tighten the bound on the degree.  This can result an   **/
/**       significant improvement in the quality of the ordering for     **/
/**       some matrices.                                                 **/
/**--------------------------------------------------------------------- **/
/** INPUT ARGUMENTS (unaltered):                                         **/
/**--------------------------------------------------------------------- **/
/** n:    number of nodes in the complete graph including halo           **/
/**       n = size of V0 U V1 U Ve                                       **/
/**       Restriction:  n .ge. 1                                         **/
/**                                                                      **/
/** norig   if compressed graph (nv(1).ne.-1) then                       **/
/**            norig is the sum(nv(i)) for i \in [1:n]                   **/
/**            and could be larger than n                                **/
/**         else norig = n                                               **/
/**                                                                      **/
/** nbelts number of elements (size of Ve)                               **/
/**            =0 if Gv (graph of variables)                             **/
/**            >0 if Ge                                                  **/
/**         nbelts > 0 extends/changes the meaning of                    **/
/**                   len/elen on entry (see below)                      **/
/**                                                                      **/
/**                                                                      **/
/** iwlen:        The length of iw (1..iwlen).  On input, the matrix is  **/
/**       stored in iw (1..pfree-1).  However, iw (1..iwlen) should be   **/
/**       slightly larger than what is required to hold the matrix, at   **/
/**       least iwlen .ge. pfree + n is recommended.  Otherwise,         **/
/**       excessive compressions will take place.                        **/
/**       *** We do not recommend running this algorithm with ***        **/
/**       ***      iwlen .lt. pfree + n.                      ***        **/
/**       *** Better performance will be obtained if          ***        **/
/**       ***      iwlen .ge. pfree + n                       ***        **/
/**       *** or better yet                                   ***        **/
/**       ***      iwlen .gt. 1.2 * pfree                     ***        **/
/**       *** (where pfree is its value on input).            ***        **/
/**       The algorithm will not run at all if iwlen .lt. pfree-1.       **/
/**                                                                      **/
/**       Restriction: iwlen .ge. pfree-1                                **/
/**--------------------------------------------------------------------- **/
/** INPUT/OUPUT ARGUMENTS:                                               **/
/**--------------------------------------------------------------------- **/
/** pe:   On input, pe (i) is the index in iw of the start of row i, or  **/
/**       zero if row i has no off-diagonal non-zeros.                   **/
/**                                                                      **/
/**       During execution, it is used for both supervariables and       **/
/**       elements:                                                      **/
/**                                                                      **/
/**       * Principal supervariable i:  index into iw of the             **/
/**               description of supervariable i.  A supervariable       **/
/**               represents one or more rows of the matrix              **/
/**               with identical nonzero pattern.                        **/
/**       * Non-principal supervariable i:  if i has been absorbed       **/
/**               into another supervariable j, then pe (i) = -j.        **/
/**               That is, j has the same pattern as i.                  **/
/**               Note that j might later be absorbed into another       **/
/**               supervariable j2, in which case pe (i) is still -j,    **/
/**               and pe (j) = -j2.                                      **/
/**       * Unabsorbed element e:  the index into iw of the description  **/
/**               of element e, if e has not yet been absorbed by a      **/
/**               subsequent element.  Element e is created when         **/
/**               the supervariable of the same name is selected as      **/
/**               the pivot.                                             **/
/**       * Absorbed element e:  if element e is absorbed into element   **/
/**               e2, then pe (e) = -e2.  This occurs when the pattern   **/
/**               of e (that is, Le) is found to be a subset of the      **/
/**               pattern of e2 (that is, Le2).  If element e is "null"  **/
/**               (it has no nonzeros outside its pivot block), then     **/
/**               pe (e) = 0.                                            **/
/**                                                                      **/
/**       On output, pe holds the assembly tree/forest, which implicitly **/
/**       represents a pivot order with identical fill-in as the actual  **/
/**       order (via a depth-first search of the tree).                  **/
/**                                                                      **/
/**       On output:                                                     **/
/**       If nv (i) .gt. 0, then i represents a node in the assembly     **/
/**       tree, and the parent of i is -pe (i), or zero if i is a root.  **/
/**       If nv (i) = 0, then (i,-pe (i)) represents an edge in a        **/
/**       subtree, the root of which is a node in the assembly tree.     **/
/**                                                                      **/
/** pfree:        On input, the matrix is stored in iw (1..pfree-1) and  **/
/**       the rest of the array iw is free.                              **/
/**       During execution, additional data is placed in iw, and pfree   **/
/**       is modified so that components  of iw from pfree are free.     **/
/**       On output, pfree is set equal to the size of iw that           **/
/**       would have been needed for no compressions to occur.  If       **/
/**       ncmpa is zero, then pfree (on output) is less than or equal    **/
/**       to iwlen, and the space iw (pfree+1 ... iwlen) was not used.   **/
/**       Otherwise, pfree (on output) is greater than iwlen, and all    **/
/**       the memory in iw was used.                                     **/
/**                                                                      **/
/** nv:   On input, encoding of compressed graph:                        **/
/**       During execution, abs (nv (i)) is equal to the number of rows  **/
/**       that are represented by the principal supervariable i.  If i   **/
/**       is a nonprincipal variable, then nv (i) = 0.  Initially,       **/
/**       nv (i) = 1 for all i.  nv (i) .lt. 0 signifies that i is a     **/
/**       principal variable in the pattern Lme of the current pivot     **/
/**       element me.  On output, nv (e) holds the true degree of        **/
/**       element e at the time it was created (including the diagonal   **/
/**       part).                                                         **/
/** begin HALO                                                           **/
/**       On output, nv(I) can be used to find node in set V1.           **/
/**       Not true anymore : ( nv(I) = N+1 characterizes nodes in V1.    **/
/**                 instead nodes in V1 are considered as a dense root   **/
/**                 node)                                                **/
/** end HALO                                                             **/
/**--------------------------------------------------------------------- **/
/** INPUT/MODIFIED (undefined on output):                                **/
/**--------------------------------------------------------------------- **/
/** len:  On input, len (i)                                              **/
/**           positive or null (>=0) : i \in V0 U Ve and                 **/
/**                   if (nbelts==0) then (graph of variables)           **/
/**                     len(i) holds the number of entries in row i of   **/
/**                     the matrix, excluding the diagonal.              **/
/**                   else (graph of elements+variables)                 **/
/**                     if i\in V0 then len(i) = nb of elements adjacent **/
/**                         to i                                         **/
/**                     if i\in Ve then len(i) = nb of variables         **/
/**                         adjacent to i                                **/
/**                   endif                                              **/
/**           negative (<0) : i \in V1, and                              **/
/**                   if (nbelts==0) then (graph of variables)           **/
/**                     -len(i) hold the number of entries in row i of   **/
/**                      the matrix, excluding the diagonal.             **/
/**                      len(i) = - | Adj(i) | if i \in V1               **/
/**                              or -N -1 if | Adj(i) | = 0 and i \in V1 **/
/**                   else  (graph of elements+variables)                **/
/**                     -len(i) nb of elements adjacent to i             **/
/**                   endif                                              **/
/**       The content of len (1..n) is undefined on output.              **/
/**                                                                      **/
/** elen: defined on input only if nbelts>0                              **/
/**       if e \in Ve then elen (e) = -N-1                               **/
/**       if v \in V0 then elen (v) = External degree of v               **/
/**                                   that should thus be provided       **/
/**                                   on entry to initialize degree      **/
/**                                                                      **/
/**       if v \in V1 then elen (v) = 0                                  **/
/**                                                                      **/
/** iw:   On input, iw (1..pfree-1) holds the description of each row i  **/
/**       in the matrix.  The matrix must be symmetric, and both upper   **/
/**       and lower triangular parts must be present.  The diagonal must **/
/**       not be present.  Row i is held as follows:                     **/
/**                                                                      **/
/**               len (i):  the length of the row i data structure       **/
/**               iw (pe (i) ... pe (i) + len (i) - 1):                  **/
/**                       the list of column indices for nonzeros        **/
/**                       in row i (simple supervariables), excluding    **/
/**                       the diagonal.  All supervariables start with   **/
/**                       one row/column each (supervariable i is just   **/
/**                       row i).                                        **/
/**               if len (i) is zero on input, then pe (i) is ignored    **/
/**               on input.                                              **/
/**                                                                      **/
/**               Note that the rows need not be in any particular       **/
/**               order, and there may be empty space between the rows.  **/
/**                                                                      **/
/**       During execution, the supervariable i experiences fill-in.     **/
/**       This is represented by placing in i a list of the elements     **/
/**       that cause fill-in in supervariable i:                         **/
/**                                                                      **/
/**               len (i):  the length of supervariable i                **/
/**               iw (pe (i) ... pe (i) + elen (i) - 1):                 **/
/**                       the list of elements that contain i.  This     **/
/**                       list is kept short by removing absorbed        **/
/**                       elements.                                      **/
/**               iw (pe (i) + elen (i) ... pe (i) + len (i) - 1):       **/
/**                       the list of supervariables in i.  This list    **/
/**                       is kept short by removing nonprincipal         **/
/**                       variables, and any entry j that is also        **/
/**                       contained in at least one of the elements      **/
/**                       (j in Le) in the list for i (e in row i).      **/
/**                                                                      **/
/**       When supervariable i is selected as pivot, we create an        **/
/**       element e of the same name (e=i):                              **/
/**                                                                      **/
/**               len (e):  the length of element e                      **/
/**               iw (pe (e) ... pe (e) + len (e) - 1):                  **/
/**                       the list of supervariables in element e.       **/
/**                                                                      **/
/**       An element represents the fill-in that occurs when             **/
/**       supervariable i is selected as pivot (which represents the     **/
/**       selection of row i and all non-principal variables whose       **/
/**       principal variable is i). We use the term Le to denote the     **/
/**       set of all supervariables in element e. Absorbed               **/
/**       supervariables and elements are pruned from these lists when   **/
/**       computationally convenient.                                    **/
/**                                                                      **/
/**       CAUTION:  THE INPUT MATRIX IS OVERWRITTEN DURING COMPUTATION.  **/
/**       The contents of iw are undefined on output.                    **/
/**                                                                      **/
/**--------------------------------------------------------------------- **/
/** OUTPUT (need not be set on input):                                   **/
/**--------------------------------------------------------------------- **/
/** elen: See the description of iw above.  At the start of execution,   **/
/**       elen (i) is set to zero.  During execution, elen (i) is the    **/
/**       number of elements in the list for supervariable i.  When e    **/
/**       becomes an element, elen (e) = -nel is set, where nel is the   **/
/**       current step of factorization.  elen (i) = 0 is done when i    **/
/**       becomes nonprincipal.                                          **/
/**                                                                      **/
/**       For variables, elen (i) .ge. 0 holds                           **/
/**       For elements, elen (e) .lt. 0 holds.                           **/
/**                                                                      **/
/** last: In a degree list, last (i) is the supervariable preceding i,   **/
/**       or zero if i is the head of the list.  In a hash bucket,       **/
/**       last (i) is the hash key for i.  last (head (hash)) is also    **/
/**       used as the head of a hash bucket if head (hash) contains a    **/
/**       degree list (see head, below).                                 **/
/**                                                                      **/
/** ncmpa:        The number of times iw was compressed.  If this is     **/
/**       excessive, then the execution took longer than what could      **/
/**       have been. To reduce ncmpa, try increasing iwlen to be 10%     **/
/**       or 20% larger than the value of pfree on input (or at least    **/
/**       iwlen .ge. pfree + n).  The fastest performance will be        **/
/**       obtained when ncmpa is returned as zero.  If iwlen is set to   **/
/**       the value returned by pfree on *output*, then no compressions  **/
/**       will occur.                                                    **/
/** begin HALO                                                           **/
/**        on output ncmpa <0 --> error detected during HALO_AMD:        **/
/**           error 1: ncmpa = -NORIG , ordering was stopped.            **/
/** end HALO                                                             **/
/**                                                                      **/
/**--------------------------------------------------------------------- **/
/** LOCAL (not input or output - used only during execution):            **/
/**--------------------------------------------------------------------- **/
/** degree:       If i is a supervariable, then degree (i) holds the     **/
/**       current approximation of the external degree of row i (an      **/
/**       upper bound).  The external degree is the number of nonzeros   **/
/**       minus abs (nv (i)) (the diagonal part).  The bound is equal to **/
/**       in row i, the external degree if elen (i) is less than or      **/
/**       equal to two.                                                  **/
/**       We also use the term "external degree" for elements e to refer **/
/**       to |Le \ Lme|.  If e is an element, then degree (e) holds      **/
/**       |Le|, which is the degree of the off-diagonal part of the      **/
/**       element e (not including the diagonal part).                   **/
/** begin HALO                                                           **/
/**       degree(I) = n+1 indicates that i belongs to V1                 **/
/** end HALO                                                             **/
/**                                                                      **/
/** head: head is used for degree lists.  head (deg) is the first        **/
/**       supervariable in a degree list (all supervariables i in a      **/
/**       degree list deg have the same approximate degree, namely,      **/
/**       deg = degree (i)).  If the list deg is empty then              **/
/**       head (deg) = 0.                                                **/
/**                                                                      **/
/**       During supervariable detection head (hash) also serves as a    **/
/**       pointer to a hash bucket.                                      **/
/**       If head (hash) .gt. 0, there is a degree list of degree hash.  **/
/**               The hash bucket head pointer is last (head (hash)).    **/
/**       If head (hash) = 0, then the degree list and hash bucket are   **/
/**               both empty.                                            **/
/**       If head (hash) .lt. 0, then the degree list is empty, and      **/
/**               -head (hash) is the head of the hash bucket.           **/
/**       After supervariable detection is complete, all hash buckets    **/
/**       are empty, and the (last (head (hash)) = 0) condition is       **/
/**       restored for the non-empty degree lists.                       **/
/** next: next (i) is the supervariable following i in a link list, or   **/
/**       zero if i is the last in the list.  Used for two kinds of      **/
/**       lists:  degree lists and hash buckets (a supervariable can be  **/
/**       in only one kind of list at a time).                           **/
/** w:    The flag array w determines the status of elements and         **/
/**       variables, and the external degree of elements.                **/
/**                                                                      **/
/**       for elements:                                                  **/
/**          if w (e) = 0, then the element e is absorbed                **/
/**          if w (e) .ge. wflg, then w (e) - wflg is the size of        **/
/**               the set |Le \ Lme|, in terms of nonzeros (the          **/
/**               sum of abs (nv (i)) for each principal variable i that **/
/**               is both in the pattern of element e and NOT in the     **/
/**               pattern of the current pivot element, me).             **/
/**          if wflg .gt. w (e) .gt. 0, then e is not absorbed and has   **/
/**               not yet been seen in the scan of the element lists in  **/
/**               the computation of |Le\Lme| in loop 150 below.         **/
/**                                                                      **/
/**       for variables:                                                 **/
/**          during supervariable detection, if w (j) .ne. wflg then j   **/
/**          is not in the pattern of variable i                         **/
/**                                                                      **/
/**       The w array is initialized by setting w (i) = 1 for all i,     **/
/**       and by setting wflg = 2.  It is reinitialized if wflg becomes  **/
/**       too large (to ensure that wflg+n does not cause integer        **/
/**       overflow).                                                     **/
/**                                                                      **/
/** wf : integer array  used to store the already filled area of         **/
/**      the variables adajcent to current pivot.                        **/
/**      wf is then used to update the score of variable i.              **/

void
hallOrderHfR3Hamdf4 (
const Gnum          norig,                        /* Uncompressed matrix order                */
const Gnum          n,                            /* Current matrix order                     */
const Gnum          nbelts,                       /* Number of elements                       */
const Gnum          nbbuck,                       /* Number of buckets                        */
const Gnum          iwlen,                        /* Length of array iw                       */
Gnum * restrict     pe /* [n] */,                 /* Array of indexes in iw of start of row i */
Gnum                pfree,                        /* Useful size in iw                        */
Gnum * restrict     len /* [n] */,                /* Array of lengths of adjacency lists      */
Gnum * restrict     iw /* [iwlen] */,             /* Adjacency list array                     */
Gnum * restrict     nv /* [n] */,                 /* Array of element degrees (weights)       */
Gnum * restrict     elen /* [n] */,               /* Array that holds the inverse permutation */
Gnum * restrict     last /* [n] */,               /* Array that holds the permutation         */
Gnum * restrict     ncmpaptr,                     /* Number of times array iw was compressed  */
Gnum * restrict     degree /* [n] */,             /* Array that holds degree data             */
Gnum * restrict     wf /* [n] */,                 /* Flag array                               */
Gnum * restrict     next /* [n] */,               /* Linked list structure                    */
Gnum * restrict     w /* [n] */,                  /* Flag array                               */
Gnum * restrict     head /* [0:nbbuck+1] */)      /* Linked list structure                    */
{
  Gnum                deg, degme, dext, dmax, e, elenme, eln, i,
                      ilast, inext, j, jlast, jnext, k, knt1, knt2, knt3,
                      lenj, ln, me, mindeg, nel,
                      nleft, nvi, nvj, nvpiv, slenme, we, wflg, wnvi, x,
                      nbflag, nreal, lastd, nelme, wf3, wf4, n2, pas;
  Gnum                knt1_updated, knt2_updated;
  Gnum                maxmem, mem, newmem;
  Gnum                maxint_n;
  Gnum                hash, hmod;
  float               rmf, rmf1;
  float               dummy;
  Gnum                idummy;
/** deg :       the degree of a variable or element                      **/
/** degme :     size, |Lme|, of the current element, me (= degree (me))  **/
/** dext :      external degree, |Le \ Lme|, of some element e           **/
/** dmax :      largest |Le| seen so far                                 **/
/** e :         an element                                               **/
/** elenme :    the length, elen (me), of element list of pivotal var.   **/
/** eln :       the length, elen (...), of an element list               **/
/** hash :      the computed value of the hash function                  **/
/** hmod :      the hash function is computed modulo hmod = max (1,n-1)  **/
/** i :         a supervariable                                          **/
/** ilast :     the entry in a link list preceding i                     **/
/** inext :     the entry in a link list following i                     **/
/** j :         a supervariable                                          **/
/** jlast :     the entry in a link list preceding j                     **/
/** jnext :     the entry in a link list, or path, following j           **/
/** k :         the pivot order of an element or variable                **/
/** knt1 :      loop counter used during element construction            **/
/** knt2 :      loop counter used during element construction            **/
/** knt3 :      loop counter used during compression                     **/
/** lenj :      len (j)                                                  **/
/** ln :        length of a supervariable list                           **/
/** maxint_n :  large integer to test risk of overflow on wflg           **/
/** maxmem :    amount of memory needed for no compressions              **/
/** me :        current supervariable being eliminated, and the          **/
/**                     current element created by eliminating that      **/
/**                     supervariable                                    **/
/** mem :       memory in use assuming no compressions have occurred     **/
/** mindeg :    current minimum degree                                   **/
/** nel :       number of pivots selected so far                         **/
/** newmem :    amount of new memory needed for current pivot element    **/
/** nleft :     n - nel, the number of nonpivotal rows/columns remaining **/
/** nvi :       the number of variables in a supervariable i (= nv (i))  **/
/** nvj :       the number of variables in a supervariable j (= nv (j))  **/
/** nvpiv :     number of pivots in current element                      **/
/** slenme :    number of variables in variable list of pivotal variable **/
/** we :        w (e)                                                    **/
/** wflg :      used for flagging the w array.  See description of iw.   **/
/** wnvi :      wflg - nv (i)                                            **/
/** x :         either a supervariable or an element                     **/
/** wf3 :       off diagoanl block area                                  **/
/** wf4 :       diagonal block area                                      **/
/** mf :        minimum fill                                             **/
/** nbflag :    number of flagged entries in the initial gragh.          **/
/** nreal :     number of entries on which ordering must be perfomed     **/
/**             (nreal = n - nbflag)                                     **/
/** nelme :     number of pivots selected when reaching the root         **/
/** lastd :     index of the last row in the list of dense rows          **/

  Gnum                p, p1, p2, p3, pdst, pend, pj, pme, pme1, pme2,
                      pn, psrc;
  Gnum                ncmpa, totel;
/**             Any parameter (pe (...) or pfree) or local variable      **/
/**             starting with "p" (for Pointer) is an index into iw,     **/
/**             and all indices into iw use variables starting with      **/
/**             "p."  The only exception to this rule is the iwlen       **/
/**             input argument.                                          **/
/** p :         pointer into lots of things                              **/
/** p1 :        pe (i) for some variable i (start of element list)       **/
/** p2 :        pe (i) + elen (i) -  1 for some var. i (end of el. list) **/
/** p3 :        index of first supervariable in clean list               **/
/** pdst :      destination pointer, for compression                     **/
/** pend :      end of memory to compress                                **/
/** pj :        pointer into an element or variable                      **/
/** pme :       pointer into the current element (pme1...pme2)           **/
/** pme1 :      the current element, me, is stored in iw (pme1...pme2)   **/
/** pme2 :      the end of the current element                           **/
/** pn :        pointer into a "clean" variable, also used to compress   **/
/** psrc :      source pointer, for compression                          **/

  -- pe;                                          /* Array index adjustments */
  -- len;
  -- iw;
  -- nv;
  -- elen;
  -- last;
  -- degree;
  -- wf;
  -- next;
  -- w;
/* -- head;                                          Array head not updated since starts from 0 */

  idummy   = GNUMMAX - 1;
  dummy    = (float) idummy;
  n2       = - (nbbuck + 1);                      /* Variable with degree N2 are in halo; bucket NBBUCK+1 used for halo variables */
  pas      = MAX ((norig / 8), 1);                /* Distance betweeen elements of the N, ..., NBBUCK entries of HEAD             */
  wflg     = 2;
  maxint_n = GNUMMAX - norig;
  ncmpa    = 0;
  nel      = 0;
  totel    = 0;
  hmod     = MAX (1, nbbuck - 1);
  dmax     = 0;
  mem      = pfree - 1;
  maxmem   = mem;
  mindeg   = 0;
  nbflag   = 0;
  lastd    = 0;

  memSet (head, 0, (nbbuck + 2) * sizeof (Gnum));
  memSet (last + 1, 0, n * sizeof (Gnum));

  if (nbelts == 0) {                              /* If graph has no elements and only variables */
    for (i = 1; i <= n; i ++) {
/*    elen[i] = 0;                                   Already done before calling */
      w[i]    = 1;
      if (len[i] < 0) {
        degree[i] = n2;
        nbflag ++;
        if (len[i] == (-1 - norig)) {             /* Variable in V1 with empty adj. list                                     */
          len[i] = 0;                             /* Because of compress, we force skipping this entry which is anyway empty */
          pe[i]  = 0;
        }
        else
          len[i] = - len[i];
      }
      else {
        totel += nv[i];
        if (n == norig)                           /* If graph not compressed */
          degree[i] = len[i];
        else {
          degree[i] = 0;
          for (p = pe[i]; p < pe[i] + len[i]; p ++)
            degree[i] += nv[iw[p]];
        }
      }
    }
  }
  else {                                          /* Graph has elements */
    for (i = 1; i <= n; i ++) {
      w[i] = 1;
      if (len[i] < 0) {
        degree[i] = n2;
        nbflag ++;
        if (len[i] == (-1 - norig)) {
          len[i]  = 0;                            /* Because of compress, we force skipping this entry (which is anyway empty) */
          pe[i]   = 0;
/*        elen[i] = 0;                               Already done before calling */
        }
        else {
          len[i]  = - len[i];
          elen[i] = len[i];
        }
      }
      else {                                      /* Non-halo vertex or element */
        if (elen[i] < 0) {                        /* If i \in Ve                */
          nel    += nv[i];
          elen[i] = - nel;
          if (n == norig)
            degree[i] = len[i];
          else {
            degree[i] = 0;
            for (p = pe[i]; p < pe[i] + len[i]; p ++)
              degree[i] += nv[iw[p]];
          }
          if (degree[i] > dmax)
            dmax = degree[i];
        }
        else {                                    /* i \in V0 */
          totel    += nv[i];
          degree[i] = elen[i];
          elen[i]   = len[i];
        }
      }
    }
  }

#ifdef SCOTCH_DEBUG_ORDER2
  if (nbelts != nel) {
    errorPrint ("hallOrderHfR3Hamdf4: internal error (1)");
    *ncmpaptr = - norig;
    return;
  }
#endif /* SCOTCH_DEBUG_ORDER2 */

  nreal = n - nbflag;                             /* Number of entries to be ordered */

  for (i = 1; i <= n; i ++) {                     /* Initialize degree lists and eliminate rows with no off-diag. nz. */
    if (elen[i] < 0)                              /* Skip element vertices (Ve)                                       */
      continue;

    deg = degree[i];
    if (deg == n2) {                              /* V1 variables (deg == n2): flagged variables are stored in the degree list of NBBUCK+1 */
      deg = nbbuck + 1;
      if (lastd == 0) {                           /* Degree list is empty */
        lastd     = i;
        head[deg] = i;
        next[i]   = 0;
        last[i]   = 0;
      }
      else {
        next[lastd] = i;
        last[i]     = lastd;
        lastd       = i;
        next[i]     = 0;
      }
    }
    else if (deg > 0) {
      wf[i] = deg;                                /* Version 1 */
      if (deg > norig)
        deg = MIN ((deg - norig) / pas + norig, nbbuck); /* Note that if (deg==0), no fill-in will occur but one variable adjacent to i */
      inext = head[deg];
      if (inext != 0)
        last[inext] = i;
      next[i] = inext;
      head[deg] = i;
    }
    else {                                        /* Variable can be eliminated at once because no off-diagonal non-zero in its row */
      nel += nv[i];
      elen[i] = - nel;
      pe[i]   = 0;
      w[i]    = 0;
    }
  }

  nleft = totel - nel;                            /* If elements provided (nbelts > 0), they are eliminated */

  while (nel < totel) {
    for (deg = mindeg; deg <= nbbuck; deg ++) {
      me = head[deg];
      if (me > 0)
        break;
    }

    mindeg = deg;
    if (me <= 0) {
      errorPrint ("hallOrderHfR3Hamdf4: internal error (2)");
      *ncmpaptr = -1 - norig;
      return;
    }

    if (deg > norig) {                            /* Linear search to find variable with best score in the list */
      j = next[me];
      k = wf[me];
      while (j > 0) {                             /* L55: CONTINUE */
        if (wf[j] < k) {
          me = j;
          k  = wf[me];
        }
        j = next[j];
      }
      ilast = last[me];
      inext = next[me];
      if (inext != 0)
        last[inext] = ilast;
      if (ilast != 0)
        next[ilast] = inext;
      else
        head[deg] = inext;                        /* me is at the head of the degree list */
    }
    else {                                        /* Remove chosen variable from link list */
      inext = next[me];
      if (inext != 0)
        last[inext] = 0;
      head[deg] = inext;
    }

    elenme   = elen[me];                          /* me represents the elimination of pivots nel+1 to nel+nv(me) */
    elen[me] = - (nel + 1);                       /* Place me itself as the first in this set. It will be moved  */
    nvpiv    = nv[me];                            /* to the nel+nv(me) position when the permutation vectors are */
    nel     += nvpiv;                             /* computed                                                    */

    nv[me] = - nvpiv;                             /* At this point, me is the pivotal supervariable */
    degme  = 0;                                   /* It will be converted into the current element  */
    if (elenme == 0) {                            /* Construct the new element in place             */
      pme1 = pe[me];
      pme2 = pme1 - 1;

      for (p = pme1; p < pme1 + len[me]; p ++) {  /* Scan list of the pivotal supervariable me */
        i   = iw[p];
        nvi = nv[i];
        if (nvi > 0) {                            /* i is a principal variable not yet placed in Lme */
          degme +=   nvi;                         /* Store i in new list                             */
          nv[i]  = - nvi;                         /* Flag i as being in Lme by negating nv (i)       */
          pme2 ++;
          iw[pme2] = i;

          if (degree[i] != n2) {                  /* Remove variable i from degree list. (only if i \in V0) */
            ilast = last[i];
            inext = next[i];
            if (inext != 0)
              last[inext] = ilast;
            if (ilast != 0)
              next[ilast] = inext;
            else {
              if (wf[i] > norig)
                deg = MIN ((wf[i] - norig) / pas + norig, nbbuck);
              else
                deg = wf[i];
              head[deg] = inext;
            }
          }
        }
      }                                           /* L60: */

      newmem = 0;
    }
    else {                                        /* Construct the new element in empty space, iw[pfree ...] */
      p      = pe[me];
      pme1   = pfree;
      slenme = len[me] - elenme;
      knt1_updated = 0;
      for (knt1 = 1; knt1 <= elenme + 1; knt1 ++) {
        knt1_updated ++;
        if (knt1 > elenme) {                      /* Search the supervariables in me */
          e  = me;
          pj = p;
          ln = slenme;
        }
        else {                                    /* Search the elements in me */
          e  = iw[p ++];
          pj = pe[e];
          ln = len[e];
        }

        knt2_updated = 0;
        for (knt2 = 1; knt2 <= ln; knt2 ++) {     /* Search for different supervariables and add them to the new list */
          knt2_updated ++;
          i   = iw[pj ++];
          nvi = nv[i];
          if (nvi > 0) {
            if (pfree > iwlen) {                  /* Compress iw if necessary                                                       */
              pe[me]   = p;                       /* Prepare for compressing iw by adjusting pointers and lengths so that the lists */
              len[me] -= knt1_updated;            /* being searched in the inner and outer loops contain only the remaining entries */
              knt1_updated = 0;                   /* Reset knt1_updated in case of recompress at same iteration of the loop 120     */
              if (len[me] == 0)                   /* Check if anything left in supervariable me                                     */
                pe[me] = 0;
              pe[e]  = pj;
              len[e] = ln - knt2_updated;
              knt2_updated = 0;                   /* Reset KNT2_UPDATED in case of recompress at same iteration of the loop 110 */
              if (len[e] == 0)
                pe[e] = 0;
              ncmpa ++;

              for (j = 1; j <= n; j ++) {         /* Store first item in pe; set first entry to -item */
                pn = pe[j];
                if (pn > 0) {
                  pe[j]  = iw[pn];
                  iw[pn] = - j;
                }
              }                                   /* L70: */

              pdst = 1;
              psrc = 1;
              pend = pme1 - 1;

              while (psrc <= pend) {              /* L80: */
                j = - iw[psrc ++];
                if (j > 0) {
                  iw[pdst] = pe[j];
                  pe[j]    = pdst ++;
                  lenj     = len[j];
                  for (knt3 = 0; knt3 <= lenj - 2; knt3 ++) /* L90: */
                    iw[pdst + knt3] = iw[psrc + knt3];
                  pdst += (lenj - 1);
                  psrc += (lenj - 1);
                }
              }

              p1 = pdst;                          /* Move the new partially-constructed element */
              for (psrc = pme1; psrc <= pfree - 1; psrc ++, pdst ++) /* L100:                   */
                iw[pdst] = iw[psrc];
              pme1 = p1;
              pfree = pdst;
              pj = pe[e];
              p  = pe[me];
            }

            degme +=   nvi;                       /* i is a principal variable not yet placed in Lme; store i in new list */
            nv[i]  = - nvi;
            iw[pfree] = i;
            pfree ++;

            if (degree[i] != n2) {                /* Remove variable i from degree link list (only if i in V0) */
              ilast = last[i];
              inext = next[i];
              if (inext != 0)
                last[inext] = ilast;
              if (ilast != 0)
                next[ilast] = inext;
              else {
                if (wf[i] > norig)
                  deg = MIN ((wf[i] - norig) / pas + norig, nbbuck);
                else
                  deg = wf[i];
                head[deg] = inext;
              }
            }
          }
        }                                         /* L110: */

        if (e != me) {                            /* Set tree pointer and flag to indicate element e is absorbed into new element me (the parent of e is me) */
          pe[e] = - me;
          w[e]  = 0;
        }
      }                                           /* L120: */
      pme2 = pfree - 1;

      newmem = pfree - pme1;                      /* This element takes newmem new memory in iw (possibly zero) */
      mem   += newmem;
      maxmem = MAX (maxmem, mem);
    }

    degree[me] = degme;                           /* me has now been converted into an element in iw (pme1..pme2) */
    pe[me]     = pme1;                            /* degme holds the external degree of new element               */
    len[me]    = pme2 - pme1 + 1;

    if (wflg > maxint_n) {                        /* Make sure that wflg is not too large; wflg+n must not cause integer overflow */
      for (x = 1; x <= n; x ++) {
        if (w[x] != 0)
          w[x] = 1;
      }                                           /* L130: */
      wflg = 2;
    }

    for (pme = pme1; pme <= pme2; pme ++) {       /* Compute (w[e] - wflg) = |Le\Lme| for all elements */
      i   = iw[pme];
      eln = elen[i];
      if (eln > 0) {                              /* Note that nv[i] has been negated to denote i in Lme */
        nvi  = - nv[i];
        wnvi = wflg - nvi;
        for (p = pe[i]; p < pe[i] + eln; p ++) {
          e  = iw[p];
          we = w[e];
          if (we >= wflg)                         /* Unabsorbed element e has been seen in this loop */
            we -= nvi;
          else if (we != 0) {                     /* e is an unabsorbed element; this is the first time we have seen e in all of Scan 1 */
            we    = degree[e] + wnvi;
            wf[e] = 0;
          }
          w[e] = we;
        }                                         /* L140: */
      }
    }                                             /* L150: */

    for (pme = pme1; pme <= pme2; pme ++) {       /* Degree update and element absorption */
      i  = iw[pme];
      p1 = pe[i];
      p2 = p1 + elen[i] - 1;
      pn = p1;
      hash = 0;
      deg  = 0;
      wf3  = 0;
      wf4  = 0;
      nvi  = - nv[i];

      for (p = p1; p <= p2; p ++) {               /* Scan the element list associated with supervariable i */
        e    = iw[p];
        dext = w[e] - wflg;                       /* dext = |Le\Lme| */
        if (dext > 0) {
          if (wf[e] == 0)                         /* First time we meet e: compute wf[e] which is the surface associated to element e */
            wf[e] = dext * ((2 * degree[e]) - dext - 1);
          wf4      += wf[e];
          deg      += dext;
          iw[pn ++] = e;
          hash     += e;
        }
        else if (dext == 0) {                     /* Aggressive absorption: e is not adjacent to me, but the |Le \ Lme| is 0, so absorb it into me */
          pe[e] = - me;
          w[e]  = 0;
        }
      }                                           /* L160:                                            */
      elen[i] = pn - p1 + 1;                      /* Count the number of elements in i (including me) */

      p3 = pn;
      for (p = p2 + 1; p < p1 + len[i]; p ++) {   /* Scan the supervariables in the list associated with i */
        j   = iw[p];
        nvj = nv[j];
        if (nvj > 0) {                            /* j is unabsorbed, and not in Lme; add to degree and add to new list */
          deg += nvj;
          wf3 += nvj;
          iw[pn ++] = j;
          hash += j;
        }
      }                                           /* L170: */

      if (degree[i] == n2)
        deg = n2;
      if (deg == 0) {                             /* Mass elimination */
        pe[i]   = - me;
        nvi     = - nv[i];
        degme  -= nvi;
        nvpiv  += nvi;
        nel    += nvi;
        nv[i]   = 0;
        elen[i] = 0;
      }
      else {                                      /* Update the upper-bound degree of i                  */
        if (degree[i] != n2) {                    /* i does not belong to halo                           */
          if (degree[i] < deg) {                  /* Our appox degree is loose; we cannot subtract wf[i] */
            wf4 = 0;
            wf3 = 0;
          }
          else
            degree[i] = deg;
        }
        wf[i]  = wf4 + 2 * nvi * wf3;
        iw[pn] = iw[p3];                          /* Add me to the list for i */
        iw[p3] = iw[p1];
        iw[p1] = me;                              /* Add new element to front of list           */
        len[i] = pn - p1 + 1;                     /* Store the new length of the list in len[i] */

        if (deg != n2) {                          /* Place in hash bucket; save hash key of i in last[i] */
          hash = (hash % hmod) + 1;
          j = head[hash];
          if (j <= 0) {                           /* The degree list is empty, hash head is -j */
            next[i]    = - j;
            head[hash] = - i;
          }
          else {                                  /* Degree list is not empty; use last (head (hash)) as hash head */
            next[i] = last[j];
            last[j] = i;
          }
          last[i] = hash;
        }
      }
    }                                             /* L180: */
    degree[me] = degme;

    if (degme > dmax)                             /* Clear the counter array w[...] by incrementing wflg */
      dmax = degme;
    wflg += dmax;

    if (wflg > maxint_n) {                        /* Make sure that wflg+n does not cause integer overflow */
      for (x = 1; x <= n; x ++) {
        if (w[x] != 0)
          w[x] = 1;
      }
      wflg = 2;
    }

    for (pme = pme1; pme <= pme2; pme ++) {       /* Supervariable detection */
      i = iw[pme];
      if ((nv[i] < 0) && (degree[i] != n2)) {     /* i is a principal variable in Lme */
        hash = last[i];
        j    = head[hash];
        if (j == 0)
          continue;                               /* GOTO L250:           */
        if (j < 0) {                              /* Degree list is empty */
          i = - j;
          head[hash] = 0;
        }
        else {                                    /* Degree list is not empty: restore last[] of head */
          i       = last[j];
          last[j] = 0;
        }
        if (i == 0)
          continue;

L200:                                             /* WHILE LOOP L200: */
        if (next[i] != 0) {
          ln  = len[i];
          eln = elen[i];
          for (p = pe[i] + 1; p < pe[i] + ln; p ++)
            w[iw[p]] = wflg;

          jlast = i;
          j = next[i];

L220:                                             /* WHILE LOOP: */
          if (j != 0) {
            if (len[j] != ln)                     /* Jump if i and j do not have same size data structure */
              goto L240;
            if (elen[j] != eln)                   /* Jump if i and j do not have same number adj elts */
              goto L240;

            for (p = pe[j] + 1; p < pe[j] + ln; p ++) {
              if (w[iw[p]] != wflg)               /* Jump if an entry [iw[p]] is in j but not in i */
                goto L240;
            }                                     /* L230: */

            pe[j] = -i;                           /* Found it! j can be absorbed into i */
            if (wf[j] > wf[i])
              wf[i] = wf[j];
            nv[i]  += nv[j];                      /* Both nv[i] and nv[j] are negated since they are in Lme */
            nv[j]   = 0;
            elen[j] = 0;

            j           = next[j];                /* Delete j from hash bucket */
            next[jlast] = j;
            goto L220;

L240:
            jlast = j;                            /* j cannot be absorbed into i */
            j     = next[j];
            goto L220;
          }

          wflg ++;
          i = next[i];
          if (i != 0)
            goto L200;
        }
      }
    }

    p     = pme1;                                 /* Restore degree lists and remove nonprincipal supervariable from element */
    nleft = totel - nel;
    for (pme = pme1; pme <= pme2; pme ++) {
      i   = iw[pme];
      nvi = - nv[i];
      if (nvi > 0) {                              /* i is a principal variable in Lme; restore nv[i] to signify that i is principal */
        nv[i] = nvi;
        if (degree[i] != n2) {                    /* Compute the external degree (add size of current elem) */
          deg = degree[i];
          if (degree[i] + degme > nleft) {
            rmf1      = (float) deg * (float) ((deg - 1) + (2 * degme)) - (float) wf[i];
            degree[i] = nleft - nvi;
            deg       = degree[i];
            rmf = (float) deg * (float) (deg - 1) - (float) (degme - nvi) * (float) (degme - nvi - 1);
            rmf = MIN (rmf, rmf1);
          }
          else {
            degree[i] += degme - nvi;
            rmf = (float) deg * (float) ((deg - 1) + 2 * degme) - (float) wf[i];
          }

          rmf /= (float) (nvi + 1);
          if (rmf < dummy)
            wf[i] = (Gnum) anint (rmf);
          else if ((rmf / (float) n) < dummy)
            wf[i] = (Gnum) anint (rmf / (float) n);
          else
            wf[i] = idummy;
          if (wf[i] < 1)
            wf[i] = 1;

          deg = wf[i];
          if (deg > norig)
            deg = MIN ((deg - norig) / pas + norig, nbbuck);
          inext = head[deg];
          if (inext != 0)
            last[inext] = i;
          next[i]   = inext;
          last[i]   = 0;
          head[deg] = i;

          if (mindeg > deg)                       /* Save the new degree, and find the minimum degree */
            mindeg = deg;
        }

        iw[p ++] = i;                             /* Place the supervariable in the element pattern */
      }
    } /* L260: */

    nv[me]  = nvpiv + degme;                      /* Finalize the new element */
    len[me] = p - pme1;
    if (len[me] == 0) {                           /* There is nothing left of the current pivot element */
      pe[me] = 0;
      w[me]  = 0;
    }
    if (newmem != 0) {                            /* Element was not constructed in place: deallocate part of it */
      pfree = p;
      mem   = mem - newmem + len[me];
    }
  }                                               /* END WHILE (selecting pivots) */

  if (nbflag > 0) {                               /* Begin halo v2 */
    for (deg = mindeg; deg <= (nbbuck + 1); deg ++) {
      me = head[deg];
      if (me > 0)
        break;
    }

    mindeg = deg;
    nelme  = - (nel + 1);
    for (x = 1; x <= n; x ++) {
      if ((pe[x] > 0) && (elen[x] < 0))           /* x is an unabsorbed element */
        pe[x] = - me;
      else if (degree[x] == n2) {                 /* x is a dense row, absorb it in ME (mass elimination) */
        nel    += nv[x];
        pe[x]   = - me;
        elen[x] = 0;
        nv[x]   = 0;                              /* Patch 12/12/98 <PA+FP> (old: n + 1) */
      }
    }

    elen[me] = nelme;                             /* me is the root node                                  */
    nv[me]   = n - nreal;                         /* Correct value of NV is (principal variable) = nbflag */
    pe[me]   = 0;

#ifdef SCOTCH_DEBUG_ORDER2
    if (nel != norig) {                           /* Error 2 */
      errorPrint ("hallOrderHfR3Hamdf4: internal error (3)");
      *ncmpaptr = -2 - norig;
      return;
    }
#endif /* SCOTCH_DEBUG_ORDER2 */
  }

  for (i = 1; i <= n; i ++) {
    if (elen[i] == 0) {
      for (j = - pe[i]; elen[j] >= 0; j = - pe[j]) ; /* L270: */
      e = j;
      k = - elen[e];                              /* Get the current pivot ordering of e */
      j = i;

      while (elen[j] >= 0) {                      /* L280: */
        jnext = - pe[j];
        pe[j] = - e;
        if (elen[j] == 0)
          elen[j] = k ++;
        j = jnext;
      }
      elen[e] = - k;
    }
  }                                               /* L290: */

  *ncmpaptr = ncmpa;
}
