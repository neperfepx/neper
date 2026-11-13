/* Copyright 2004,2007,2008,2014,2018,2021,2023,2024 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : parser.h                                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : These lines are the declarations for    **/
/**                the strategy lexical and syntactic      **/
/**                analyzer.                               **/
/**                                                        **/
/**   DATES      : # Version 3.1  : from : 07 nov 1995     **/
/**                                 to   : 02 may 1996     **/
/**                # Version 3.2  : from : 07 oct 1996     **/
/**                                 to   : 19 oct 1996     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 4.0  : from : 20 dec 2001     **/
/**                                 to   : 11 jun 2004     **/
/**                # Version 5.1  : from : 20 feb 2008     **/
/**                                 to   : 20 feb 2008     **/
/**                # Version 6.0  : from : 30 sep 2014     **/
/**                                 to   : 30 sep 2014     **/
/**                # Version 7.0  : from : 02 mar 2018     **/
/**                                 to   : 07 nov 2024     **/
/**                                                        **/
/************************************************************/

/*
**  The type definitions.
*/

/*+ Strategy node types. +*/

typedef enum StratNodeType_ {
  STRATNODECONCAT,                                /*+ Concatenation node       +*/
  STRATNODECOND,                                  /*+ Condition node           +*/
  STRATNODEEMPTY,                                 /*+ Empty strategy           +*/
  STRATNODEMETHOD,                                /*+ Method                   +*/
  STRATNODESELECT,                                /*+ Selection node           +*/
  STRATNODENBR                                    /*+ Number of strategy nodes +*/
} StratNodeType;

/*+ Method and graph parameter types. +*/

typedef int StratParamType;                       /*+ Same type as enum +*/

#define STRATPARAMCASE              0             /*+ Character; TRICK: FIRST +*/
#define STRATPARAMDOUBLE            1             /*+ Double floating-point   +*/
#define STRATPARAMINT               2             /*+ Integer                 +*/
#define STRATPARAMLOG               3             /*+ Logical value           +*/
#define STRATPARAMSTRAT             4             /*+ Strategy                +*/
#define STRATPARAMSTRING            5             /*+ String of characters    +*/

#define STRATPARAMDEPRECATED        8             /*+ Indicates deprecated parameter; can be merged with the above +*/

/*+ Test types, ordered by ascending priority,
    for proper writing of parentheses. Initial
    value should be zero for proper indexing.  +*/

typedef enum StratTestType_ {
  STRATTESTOR = 0,                                /*+ Or operator             +*/
  STRATTESTAND,                                   /*+ And operator            +*/
  STRATTESTNOT,                                   /*+ Not operator            +*/
  STRATTESTEQ,                                    /*+ Equal-to operator       +*/
  STRATTESTGT,                                    /*+ Greater-than operator   +*/
  STRATTESTLT,                                    /*+ Less-than operator      +*/
  STRATTESTADD,                                   /*+ Addition operator       +*/
  STRATTESTSUB,                                   /*+ Subtraction operator    +*/
  STRATTESTMUL,                                   /*+ Multiplication operator +*/
  STRATTESTMOD,                                   /*+ Modulus operator        +*/
  STRATTESTVAL,                                   /*+ Constant value          +*/
  STRATTESTVAR,                                   /*+ Variable                +*/
  STRATTESTNBR                                    /*+ Number of test nodes    +*/
} StratTestType;

/*+ Method characteristics. +*/

typedef int (* StratMethodFunc) (void);

typedef struct StratMethodTab_ {
  int                       methnum;              /*+ Method number in method table      +*/
  char *                    nameptr;              /*+ Method name                        +*/
  StratMethodFunc           funcptr;              /*+ Pointer to graph processing method +*/
  void *                    dataptr;              /*+ Pointer to default parameters      +*/
} StratMethodTab;

/*+ Method parameter characteristics. +*/

typedef struct StratParamTab_ {
  int                       methnum;              /*+ Method number in method table    +*/
  StratParamType            typeval;              /*+ Parameter type                   +*/
  char *                    nameptr;              /*+ Parameter name                   +*/
  byte *                    dbasptr;              /*+ Pointer to data base in method   +*/
  byte *                    doffptr;              /*+ Pointer to data offset in method +*/
  void *                    dselptr;              /*+ Pointer to data selector         +*/
} StratParamTab;

/*+ Strategy characteristics. +*/

typedef struct StratTab_ {
  StratMethodTab *          methtab;              /*+ Pointer to method table    +*/
  StratParamTab *           paratab;              /*+ Pointer to parameter table +*/
  StratParamTab *           condtab;              /*+ Pointer to condition table +*/
} StratTab;

/*+ Concatenation strategy node. +*/

typedef struct StratNodeConcat_ {                 /*+ Concatenation node                        +*/
  struct Strat_ *           stratab[2];           /*+ Pointers to the two strategies to combine +*/
} StratNodeConcat;

/*+ Condition and test strategy nodes. +*/

typedef union StratTestVal_ {                     /*+ Constant value +*/
  double                    valdbl;               /*+ Double value   +*/
  INT                       valint;               /*+ Integer value  +*/
  int                       vallog;               /*+ Logical value  +*/
} StratTestVal;

typedef struct StratTestVar_ {                    /*+ Condition variable                     +*/
  const StratTab *          datatab;              /*+ Pointer to data parameter table        +*/
  ptrdiff_t                 dataoft;              /*+ Displacement with respect to beginning +*/
} StratTestVar;

typedef struct StratTest_ {                       /*+ Test node +*/
  StratTestType             testval;              /*+ Test type +*/
  StratParamType            nodeval;              /*+ Node type +*/
  union {
    struct StratTest_ *     testtab[2];           /*+ Logical/relational branches +*/
    StratTestVal            val;                  /*+ Value                       +*/
    StratTestVar            var;                  /*+ Variable                    +*/
  } data;
} StratTest;

typedef struct StratNodeCond_ {                   /*+ Test node            +*/
  StratTest *               testptr;              /*+ Test condition       +*/
  struct Strat_ *           stratab[2];           /*+ Then/else strategies +*/
} StratNodeCond;

/*+ Data structure of the empty strategy operator node. +*/

typedef struct StratNodeEmpty_ {                  /*+ Empty node +*/
  byte                      dummy;                /*+ Dummy data +*/
} StratNodeEmpty;

/*+ Data structure of the empty strategy operator node. +*/

typedef double StratNodeMethodData[10];           /*+ Reserved padded space for method data */

typedef struct StratNodeMethod_ {                 /*+ Method node           +*/
  int                       methnum;              /*+ Index in method table +*/
  StratNodeMethodData       datadat;              /*+ Method data           +*/
} StratNodeMethod;

/*+ Data structure of the selection strategy operator node. +*/

typedef struct StratNodeSelect_ {                 /*+ Selection node                         +*/
  struct Strat_ *           stratab[2];           /*+ Pointers to the two strategies to test +*/
} StratNodeSelect;

/*+ The strategy node data structure. +*/

typedef struct Strat_ {
  const StratTab *          tablptr;              /*+ Pointer to parsing strategy table +*/
  StratNodeType             typeval;              /*+ Method type                       +*/
  union {                                         /*+ Method data                       +*/
    double                  padding;              /*+ Padding for double alignment      +*/
    StratNodeConcat         concdat;              /*+ Concatenation node data           +*/
    StratNodeCond           conddat;              /*+ Condition node data               +*/
    StratNodeEmpty          emptdat;              /*+ Empty node data                   +*/
    StratNodeMethod         methdat;              /*+ Method node data                  +*/
    StratNodeSelect         seledat;              /*+ Selection node data               +*/
  } data;
} Strat;

/*+ The parser environment structure. +*/

typedef struct ParserEnv_ {
  const StratTab *          stratab;              /*+ Pointer to parsing tables        +*/
  Strat *                   straptr;              /*+ Pointer to current strategy node +*/
  const StratParamTab *     paraptr;              /*+ Pointer to current parameter     +*/
  const char *              textptr;              /*+ Pointer to strategy string       +*/
} ParserEnv;

/*+ The text parsing location structure. +*/

typedef struct ParserLocation_ {
  int                       cobenum;              /*+ Column of beginning of area  +*/
  int                       libenum;              /*+ Line of beginning of area    +*/
  const char *              tebeptr;              /*+ Pointer to beginning of area +*/
  int                       coennum;              /*+ Column of end of area        +*/
  int                       liennum;              /*+ Line of end of area          +*/
  const char *              teenptr;              /*+ Pointer to end of area       +*/
} ParserLocation;

#define YYLTYPE                     ParserLocation /* Trigger use of locations in parser */

#define YYLLOC_DEFAULT(Current, Rhs, N)                                  \
  do {                                                                   \
    if (N) {                                                             \
      (Current).cobenum = YYRHSLOC (Rhs, 1).cobenum;                     \
      (Current).libenum = YYRHSLOC (Rhs, 1).libenum;                     \
      (Current).tebeptr = YYRHSLOC (Rhs, 1).tebeptr;                     \
      (Current).coennum = YYRHSLOC (Rhs, N).coennum;                     \
      (Current).liennum = YYRHSLOC (Rhs, N).liennum;                     \
      (Current).teenptr = YYRHSLOC (Rhs, N).teenptr;                     \
    }                                                                    \
    else {                                                               \
      (Current).cobenum = (Current).coennum = YYRHSLOC (Rhs, 0).coennum; \
      (Current).libenum = (Current).liennum = YYRHSLOC (Rhs, 0).liennum; \
      (Current).tebeptr = (Current).teenptr = YYRHSLOC (Rhs, 0).teenptr; \
    }                                                                    \
  } while (0)

/*
**  The external declarations.
*/

extern Strat                stratdummy;           /*+ Dummy empty strategy node +*/

/*
**  The function prototypes.
*/

#ifdef SCOTCH_PARSER
static int                  stratTestEvalCast   (StratTest * const, StratTest * const);
#endif /* SCOTCH_PARSER */

Strat *                     stratInit           (const StratTab * const , const char * const);
int                         stratExit           (Strat * const);
int                         stratSave           (const Strat * const, FILE * const);

int                         stratTestEval       (const StratTest * const, StratTest * const, const void * const);
int                         stratTestExit       (StratTest * const);
int                         stratTestSave       (const StratTest * const, FILE * const);

void                        parserLocationUpdate (ParserLocation * const, const char * const);
