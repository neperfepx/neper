/* Copyright 2010,2011,2016,2018 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : fibo.h                                  **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains the definitions of **/
/**                the generic Fibonacci trees.            **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 01 may 2010     **/
/**                                 to   : 12 may 2010     **/
/**                # Version 6.0  : from : 22 oct 2011     **/
/**                                 to   : 07 jun 2018     **/
/**                                                        **/
/**   NOTES      : # Since this module is to be used as    **/
/**                  the gain keeping data structure for   **/
/**                  local optimization algorithms, the    **/
/**                  computation of the best node needs    **/
/**                  only to be done when actually picking **/
/**                  the vertex, while many insertions and **/
/**                  deletions (neighbor vertices) can     **/
/**                  take place in the mean time. This is  **/
/**                  why this data structure does not keep **/
/**                  track of the best node, as most       **/
/**                  implementations do.                   **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

/*+ Degree range. +*/

#ifndef FIBO_DEGRMAX
#define FIBO_DEGRMAX                (sizeof (INT) << 3) /* Assume maximum degree derives from an integer range */
#endif /* FIBO_DEGRMAX */

/*
**  The type and structure definitions.
*/

/* The doubly linked list structure. */

typedef struct FiboLink_ {
  struct FiboNode_ *        prevptr;              /*+ Pointer to previous sibling element +*/
  struct FiboNode_ *        nextptr;              /*+ Pointer to next sibling element     +*/
} FiboLink;

/* The tree node data structure. The deflval
   variable merges degree and flag variables.
   The degree of a node is smaller than
   "bitsizeof (INT)", so it can hold on an
   "int". The flag value is stored in the
   lowest bit of the value.                   */
  

typedef struct FiboNode_ {
  struct FiboNode_ *        pareptr;              /*+ Pointer to parent element, if any                +*/
  struct FiboNode_ *        chldptr;              /*+ Pointer to first child element, if any           +*/
  FiboLink                  linkdat;              /*+ Pointers to sibling elements                     +*/
  int                       deflval;              /*+ Lowest bit: flag value; other bits: degree value +*/
} FiboNode;

/* The tree data structure. The fake dummy node aims
   at handling root node insertion without any test.
   This is important as many insertions have to be
   performed.                                        */

typedef struct FiboHeap_ {
  FiboNode                  rootdat;              /*+ Dummy node for fast root insertion                      +*/
  FiboNode ** restrict      degrtab;              /*+ Consolidation array of size "bitsizeof (INT)"           +*/
  int                    (* cmpfptr) (const FiboNode * const, const FiboNode * const); /*+ Comparison routine +*/
} FiboHeap;

/*
**  The marco definitions.
*/

#define fiboHeapLinkAfter(o,n)      do {                              \
                                      FiboNode *          nextptr;    \
                                      nextptr = (o)->linkdat.nextptr; \
                                      (n)->linkdat.nextptr = nextptr; \
                                      (n)->linkdat.prevptr = (o);     \
                                      nextptr->linkdat.prevptr = (n); \
                                      (o)->linkdat.nextptr = (n);     \
                                    } while (0)

#define fiboHeapUnlink(n)           do {                                                            \
                                      (n)->linkdat.prevptr->linkdat.nextptr = (n)->linkdat.nextptr; \
                                      (n)->linkdat.nextptr->linkdat.prevptr = (n)->linkdat.prevptr; \
                                    } while (0)

#define fiboHeapAddMacro(t,n)       do {                                        \
                                      (n)->pareptr = NULL;                      \
                                      (n)->chldptr = NULL;                      \
                                      (n)->deflval = 0;                         \
                                      fiboHeapLinkAfter (&((t)->rootdat), (n)); \
                                    } while (0)

#define fiboHeapMinMacro(t)         (fiboHeapConsolidate (t))

#define fiboHeapMoveToRoot(t,n,p)   do {                                              \
                                      FiboNode *          rghtptr;                    \
                                      int                 deflval;                    \
                                      rghtptr = (n)->linkdat.nextptr;                 \
                                      fiboHeapUnlink (n);                             \
                                      (n)->pareptr  = NULL;                           \
                                      (n)->deflval &= ~1;                             \
                                      deflval = (p)->deflval - 2;                     \
                                      (p)->deflval = deflval;                         \
                                      (p)->chldptr = (deflval <= 1) ? NULL : rghtptr; \
                                      fiboHeapLinkAfter (&((t)->rootdat), (n));       \
                                    } while (0)

#define fiboHeapDecreaseMacro(t,n)  do {                                                  \
                                      FiboNode *          pareptr;                        \
                                      pareptr = (n)->pareptr;                             \
                                      if ((pareptr != NULL) &&                            \
                                          (treeptr->cmpfptr (nodeptr, pareptr) < 0)) {    \
                                        FiboNode *          gdpaptr;                      \
                                        fiboHeapMoveToRoot (treeptr, nodeptr, pareptr);   \
                                        gdpaptr = pareptr->pareptr;                       \
                                        while (gdpaptr != NULL) {                         \
                                          if ((pareptr->deflval & 1) == 0) {              \
                                            pareptr->deflval |= 1;                        \
                                            break;                                        \
                                          }                                               \
                                          fiboHeapMoveToRoot (treeptr, pareptr, gdpaptr); \
                                          pareptr = gdpaptr;                              \
                                          gdpaptr = gdpaptr->pareptr;                     \
                                        }                                                 \
                                      }                                                   \
                                    } while (0)

#define fiboHeapCutChildren(t,n)    do {                                                \
                                      FiboNode *          chldptr;                      \
                                      chldptr = (n)->chldptr;                           \
                                      if (chldptr != NULL) {                            \
                                        FiboNode *          cendptr;                    \
                                        cendptr = chldptr;                              \
                                        do {                                            \
                                          FiboNode *          nextptr;                  \
                                          nextptr = chldptr->linkdat.nextptr;           \
                                          chldptr->pareptr = NULL;                      \
                                          fiboHeapLinkAfter (&((t)->rootdat), chldptr); \
                                          chldptr = nextptr;                            \
                                        } while (chldptr != cendptr);                   \
                                      }                                                 \
                                    } while (0)

#define fiboHeapDelMacro(t,n)       do {                                                    \
                                      FiboNode *          pareptr;                          \
                                      FiboNode *          rghtptr;                          \
                                      pareptr = (n)->pareptr;                               \
                                      fiboHeapUnlink (n);                                   \
                                      fiboHeapCutChildren ((t), (n));                       \
                                      if (pareptr == NULL)                                  \
                                        break;                                              \
                                      rghtptr = (n)->linkdat.nextptr;                       \
                                      while (1) {                                           \
                                        FiboNode *          gdpaptr;                        \
                                        int                 deflval;                        \
                                        deflval = pareptr->deflval - 2;                     \
                                        pareptr->deflval = deflval | 1;                     \
                                        gdpaptr = pareptr->pareptr;                         \
                                        pareptr->chldptr = (deflval <= 1) ? NULL : rghtptr; \
                                        if (((deflval & 1) == 0) || (gdpaptr == NULL))      \
                                          break;                                            \
                                        rghtptr = pareptr->linkdat.nextptr;                 \
                                        fiboHeapUnlink (pareptr);                           \
                                        pareptr->pareptr = NULL;                            \
                                        fiboHeapLinkAfter (&((t)->rootdat), pareptr);       \
                                        pareptr = gdpaptr;                                  \
                                      }                                                     \
                                    } while (0)

/*
**  The function prototypes.
*/

#define fiboHeapAddIsMacro
#define fiboHeapMinIsMacro

int                         fiboHeapInit        (FiboHeap * const, int (*) (const FiboNode * const, const FiboNode * const));
void                        fiboHeapExit        (FiboHeap * const);
void                        fiboHeapFree        (FiboHeap * const);
FiboNode *                  fiboHeapConsolidate (FiboHeap * const);
#ifndef fiboHeapAddIsMacro
void                        fiboHeapAdd         (FiboHeap * const, FiboNode * const);
#else /* fiboHeapAddIsMacro */
#define fiboHeapAdd              fiboHeapAddMacro
#endif /* fiboHeapAddIsMacro */
#ifndef fiboHeapDecreaseIsMacro
void                        fiboHeapDecrease    (FiboHeap * const, FiboNode * const);
#else /* fiboHeapDecreaseIsMacro */
#define fiboHeapDecrease         fiboHeapDecreaseMacro
#endif /* fiboHeapDecreaseIsMacro */
#ifndef fiboHeapDelIsMacro
void                        fiboHeapDel         (FiboHeap * const, FiboNode * const);
#else /* fiboHeapDelIsMacro */
#define fiboHeapDel              fiboHeapDelMacro
#endif /* fiboHeapDelIsMacro */
#ifndef fiboHeapMinIsMacro
FiboNode *                  fiboHeapMin         (FiboHeap * const);
#else /* fiboHeapMinIsMacro */
#define fiboHeapMin              fiboHeapMinMacro
#endif /* fiboHeapMinIsMacro */
#ifdef SCOTCH_DEBUG_FIBO3
#ifdef FIBO
static int                  fiboHeapCheck2      (const FiboNode * const);
#endif /* FIBO */
int                         fiboHeapCheck       (const FiboHeap * const);
#endif /* SCOTCH_DEBUG_FIBO3 */
