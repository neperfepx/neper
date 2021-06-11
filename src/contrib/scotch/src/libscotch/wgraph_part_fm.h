/* Copyright 2007-2010,2012,2018,2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : wgraph_part_fm.h                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Jun-Ho HER (v6.0)                       **/
/**                Charles-Edmond BICHOT (v5.1b)           **/
/**                                                        **/
/**   FUNCTION   : These lines are the data declarations   **/
/**                for the improved Fiduccia-Mattheyses    **/
/**                refinement routine for the vertex over- **/
/**                lapped graph partitioning.              **/
/**                                                        **/
/**   DATES      : # Version 5.1  : from : 01 dec 2007     **/
/**                                 to   : 01 jul 2008     **/
/**                # Version 6.0  : from : 05 nov 2009     **/
/**                                 to   : 31 may 2018     **/
/**                # Version 6.1  : from : 30 jul 2020     **/
/**                                 to   : 20 aug 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

/*+ Gain table subbits. +*/

#define WGRAPHPARTFMGAINBITS        4

/*+ Prime number for hashing vertex numbers. +*/

#define WGRAPHPARTFMHASHPRIME       17            /*+ Prime number for hashing +*/

/*
**  The type and structure definitions.
*/

/*+ This structure holds the method parameters. +*/

typedef struct WgraphPartFmParam_ {
  INT                       movenbr;              /*+ Maximum number of uneffective moves that can be done +*/
  INT                       passnbr;              /*+ Number of passes to be performed (-1: infinite)      +*/
  double                    deltrat;              /*+ Maximum weight imbalance ratio                       +*/
} WgraphPartFmParam;

/*+ The part list structure, to record used neighboring parts. +*/

typedef struct WgraphPartFmPartList_ {
  Anum                      nextidx;              /*+ Pointer to next cell in neighbor linked list      +*/
  Gnum                      loadgainval;          /*+ Load gain value                                   +*/
  Gnum                      sizegainval;          /*+ Size gain value                                   +*/
  Gnum                      vechnum;              /*+ End vertex that will change load of this part     +*/
  Gnum                      vencnum;              /*+ End vertex that will not change load of this part +*/
} WgraphPartFmPartList;

/*+ The vertex hash slot. +*/

typedef struct WgraphPartFmVertex_ {
  Gnum                          vertnum;          /*+ Number of vertex in hash table               +*/
  Anum                          partnum;          /*+ Current vertex part                          +*/
  Gnum                          linkidx;          /*+ Start index of list of links to target parts +*/
  struct WgraphPartFmVertex_ *  nlokptr;          /*+ Pointer to next vertex in locked list        +*/
  struct WgraphPartFmVertex_ *  nlstptr;          /*+ Pointer to next vertex in working list       +*/
} WgraphPartFmVertex;

/*+ The vertex hash management structure. +*/

typedef struct WgraphPartFmHashData_ {
  WgraphPartFmVertex *      hashtab;              /*+ Pointer to hash array          +*/
  Gnum                      hashsiz;              /*+ Size of hash array             +*/
  Gnum                      hashmax;              /*+ Maximum capacity of hash table +*/
  Gnum                      hashmsk;              /*+ Mask for hash accesses         +*/
  Gnum                      hashnbr;              /*+ Number of used hash slots      +*/
  WgraphPartFmVertex *      lockptr;              /*+ List of locked vertices        +*/
} WgraphPartFmHashData;

/*+ The vertex link structure. +*/

typedef struct WgraphPartFmLink_ {
  GainLink                  gainlink;             /*+ Gain link: FIRST                                   +*/
  Gnum                      nextidx;              /*+ Index of next link for vertex                      +*/
  Gnum                      partnum;              /*+ Part to whick the linked vertex should move        +*/
  Gnum                      vertnum;              /*+ Number of vertex to which link belongs             +*/
  Gnum                      gainval;              /*+ Gain value for frontier                            +*/
  Anum                      npmipartnum;          /*+ Number of lightest neighboring part to be impacted +*/
} WgraphPartFmLink;

/*+ The vertex link management structure. +*/

typedef struct WgraphPartFmLinkData_ {
  WgraphPartFmLink * restrict linktab;            /*+ Pointer to link array                +*/
  Gnum                        linksiz;            /*+ Size of link array                   +*/
  Gnum                        lfrenbr;            /*+ Current number of free links         +*/
  Gnum                        lfreidx;            /*+ Index of first free cell; -1 if none +*/
} WgraphPartFmLinkData;

/*+ The save type identifier. +*/

typedef enum WgraphPartFmSaveType_ {
  WGRAPHPARTFMSAVEMOVE,                           /*+ Vertex part change +*/
  WGRAPHPARTFMSAVELOAD                            /*+ Part load change   +*/
} WgraphPartFmSaveType;

/*+ The save type: records either vertex moves or part loads updates. +*/

typedef struct WgraphPartFmSave_ {
  WgraphPartFmSaveType      typeval;              /*+ Type of save slot     +*/
  union {
    struct {
      Gnum                  vertnum;              /*+ Vertex number         +*/
      Gnum                  partnum;              /*+ Old vertex part value +*/
    } movedat;
    struct {
      Gnum                  partnum;              /*+ Part number           +*/
      Gnum                  loadval;              /*+ Old load value        +*/
      Gnum                  sizeval;              /*+ Old size value        +*/
    } loaddat;
  } u;
} WgraphPartFmSave;

/*+ The save array management structure. +*/

typedef struct WgraphPartFmSaveData_ {
  WgraphPartFmSave * restrict savetab;            /*+ Pointer to save array        +*/
  Gnum                        savesiz;            /*+ Size of save array           +*/
  Gnum                        savenbr;            /*+ Current number of free cells +*/
} WgraphPartFmSaveData;

/*
**  The function prototypes.
*/

#ifdef WGRAPH_PART_FM
static int                  wgraphPartFmHashResize (WgraphPartFmHashData * const);

static void                 wgraphPartFmLinkInit (WgraphPartFmLinkData * const);
static Gnum                 wgraphPartFmLinkGet (WgraphPartFmLinkData * const);
static void                 wgraphPartFmLinkPut (WgraphPartFmLinkData * const, const Gnum);
static int                  wgraphPartFmLinkResize (WgraphPartFmLinkData * const, GainTabl * const);

static int                  wgraphPartFmSaveResize (WgraphPartFmSaveData * const);

#ifdef SCOTCH_DEBUG_WGRAPH3
static int                  wgraphPartFmCheck   (const Wgraph * restrict const, const WgraphPartFmHashData * restrict const, const Gnum);
#endif /* SCOTCH_DEBUG_WGRAPH3 */
#endif /* WGRAPH_PART_FM */

int                         wgraphPartFm        (Wgraph * restrict const, const WgraphPartFmParam * restrict const);

/*
**  The macro definitions.
*/

#define wgraphPartFmHashClaim(hdatptr,vertnbr) ((hdatptr)->hashmax < ((hdatptr)->hashnbr + (vertnbr)))

#define wgraphPartFmLinkClaim(ldatptr,linknbr) ((ldatptr)->lfrenbr < (linknbr))

#define wgraphPartFmSaveInit(sdatptr) (sdatptr)->savenbr = (sdatptr)->savesiz
#define wgraphPartFmSaveClaim(sdatptr,savvnbr) (((sdatptr)->savesiz - (sdatptr)->savenbr) < (savvnbr))
