/* Copyright 2004,2007,2009-2012,2016,2017,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : bgraph_bipart_st.c                      **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module contains the strategy and   **/
/**                method tables for graph bipartitioning  **/
/**                methods.                                **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 08 oct 1996     **/
/**                                 to   : 13 sep 1998     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 12 mar 1999     **/
/**                # Version 3.4  : from : 01 jun 2001     **/
/**                                 to   : 01 jun 2001     **/
/**                # Version 4.0  : from : 12 jan 2004     **/
/**                                 to   : 20 aug 2004     **/
/**                # Version 5.0  : from : 27 nov 2006     **/
/**                                 to   : 29 may 2007     **/
/**                # Version 5.1  : from : 26 oct 2009     **/
/**                                 to   : 15 apr 2011     **/
/**                # Version 6.0  : from : 23 feb 2011     **/
/**                                 to   : 02 jan 2017     **/
/**                # Version 7.0  : from : 17 jan 2023     **/
/**                                 to   : 08 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "fibo.h"
#include "gain.h"
#include "parser.h"
#include "graph.h"
#include "arch.h"
#include "mapping.h"
#include "graph_coarsen.h"
#include "bgraph.h"
#include "bgraph_bipart_bd.h"
#include "bgraph_bipart_df.h"
#include "bgraph_bipart_ex.h"
#include "bgraph_bipart_fm.h"
#include "bgraph_bipart_ga.h"
#include "bgraph_bipart_gg.h"
#include "bgraph_bipart_gp.h"
#include "bgraph_bipart_ml.h"
#include "bgraph_bipart_zr.h"
#include "bgraph_bipart_st.h"

/*
**  The static and global variables.
*/

static Bgraph               bgraphdummy;      /*+ Dummy active graph for offset computations +*/

static union {
  BgraphBipartBdParam       param;
  StratNodeMethodData       padding;
} bgraphbipartstdefaultbd = { { 3, &stratdummy } };

static union {
  BgraphBipartDfParam       param;
  StratNodeMethodData       padding;
} bgraphbipartstdefaultdf = { { 40, BGRAPHBIPARTDFTYPEBAL } };

static union {                                /* Default parameters for bipartitioning methods */
  BgraphBipartFmParam       param;            /* Parameter zone                                */
  StratNodeMethodData       padding;          /* To avoid reading out of structure             */
} bgraphbipartstdefaultfm = { { 80, ~0, 0.01L, BGRAPHBIPARTFMTYPEBOUNDARY } };

static union {
  BgraphBipartGaParam       param;
  StratNodeMethodData       padding;
} bgraphbipartstdefaultga = { { 40, 100 } };

static union {
  BgraphBipartGgParam       param;
  StratNodeMethodData       padding;
} bgraphbipartstdefaultgg = { { 5 } };

static union {
  BgraphBipartGpParam       param;
  StratNodeMethodData       padding;
} bgraphbipartstdefaultgp = { { 5 } };

static union {
  BgraphBipartMlParam       param;
  StratNodeMethodData       padding;
} bgraphbipartstdefaultml = { { 100, 0.8L, &stratdummy, &stratdummy } };

static StratMethodTab       bgraphbipartstmethtab[] = { /* Bipartitioning methods array */
                              { BGRAPHBIPARTSTMETHBD, "b",  (StratMethodFunc) bgraphBipartBd, &bgraphbipartstdefaultbd },
                              { BGRAPHBIPARTSTMETHDF, "d",  (StratMethodFunc) bgraphBipartDf, &bgraphbipartstdefaultdf },
                              { BGRAPHBIPARTSTMETHEX, "x",  (StratMethodFunc) bgraphBipartEx, NULL },
                              { BGRAPHBIPARTSTMETHFM, "f",  (StratMethodFunc) bgraphBipartFm, &bgraphbipartstdefaultfm },
                              { BGRAPHBIPARTSTMETHGA, "a",  (StratMethodFunc) bgraphBipartGa, &bgraphbipartstdefaultga },
                              { BGRAPHBIPARTSTMETHGG, "h",  (StratMethodFunc) bgraphBipartGg, &bgraphbipartstdefaultgg },
                              { BGRAPHBIPARTSTMETHGP, "g",  (StratMethodFunc) bgraphBipartGp, &bgraphbipartstdefaultgp },
                              { BGRAPHBIPARTSTMETHML, "m",  (StratMethodFunc) bgraphBipartMl, &bgraphbipartstdefaultml },
                              { BGRAPHBIPARTSTMETHZR, "z",  (StratMethodFunc) bgraphBipartZr, NULL },
                              { -1,                   NULL, (StratMethodFunc) NULL,           NULL } };

static StratParamTab        bgraphbipartstparatab[] = { /* Method parameter list */
                              { BGRAPHBIPARTSTMETHBD,  STRATPARAMSTRAT,  "bnd",
                                (byte *) &bgraphbipartstdefaultbd.param,
                                (byte *) &bgraphbipartstdefaultbd.param.stratbnd,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTSTMETHBD,  STRATPARAMSTRAT,  "org",
                                (byte *) &bgraphbipartstdefaultbd.param,
                                (byte *) &bgraphbipartstdefaultbd.param.stratorg,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTSTMETHBD,  STRATPARAMINT,    "width",
                                (byte *) &bgraphbipartstdefaultbd.param,
                                (byte *) &bgraphbipartstdefaultbd.param.distmax,
                                NULL },
                              { BGRAPHBIPARTSTMETHDF,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartstdefaultdf.param,
                                (byte *) &bgraphbipartstdefaultdf.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHDF,  STRATPARAMCASE,   "type",
                                (byte *) &bgraphbipartstdefaultdf.param,
                                (byte *) &bgraphbipartstdefaultdf.param.typeval,
                                (void *) "bk" },
                              { BGRAPHBIPARTSTMETHFM,  STRATPARAMINT,    "move",
                                (byte *) &bgraphbipartstdefaultfm.param,
                                (byte *) &bgraphbipartstdefaultfm.param.movenbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHFM,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartstdefaultfm.param,
                                (byte *) &bgraphbipartstdefaultfm.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHFM,  STRATPARAMDOUBLE, "bal",
                                (byte *) &bgraphbipartstdefaultfm.param,
                                (byte *) &bgraphbipartstdefaultfm.param.deltval,
                                NULL },
                              { BGRAPHBIPARTSTMETHFM,  STRATPARAMCASE,   "type",
                                (byte *) &bgraphbipartstdefaultfm.param,
                                (byte *) &bgraphbipartstdefaultfm.param.typeval,
                                (void *) "ab" },
                              { BGRAPHBIPARTSTMETHGA,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartstdefaultga.param,
                                (byte *) &bgraphbipartstdefaultga.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHGA,  STRATPARAMINT,    "pop",
                                (byte *) &bgraphbipartstdefaultga.param,
                                (byte *) &bgraphbipartstdefaultga.param.popunbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHGG,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartstdefaultgg.param,
                                (byte *) &bgraphbipartstdefaultgg.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHGP,  STRATPARAMINT,    "pass",
                                (byte *) &bgraphbipartstdefaultgp.param,
                                (byte *) &bgraphbipartstdefaultgp.param.passnbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHML,  STRATPARAMSTRAT,  "asc",
                                (byte *) &bgraphbipartstdefaultml.param,
                                (byte *) &bgraphbipartstdefaultml.param.stratasc,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTSTMETHML,  STRATPARAMSTRAT,  "low",
                                (byte *) &bgraphbipartstdefaultml.param,
                                (byte *) &bgraphbipartstdefaultml.param.stratlow,
                                (void *) &bgraphbipartststratab },
                              { BGRAPHBIPARTSTMETHML,  STRATPARAMINT,    "vert",
                                (byte *) &bgraphbipartstdefaultml.param,
                                (byte *) &bgraphbipartstdefaultml.param.coarnbr,
                                NULL },
                              { BGRAPHBIPARTSTMETHML,  STRATPARAMDOUBLE, "rat",
                                (byte *) &bgraphbipartstdefaultml.param,
                                (byte *) &bgraphbipartstdefaultml.param.coarrat,
                                NULL },
                              { BGRAPHBIPARTSTMETHNBR, STRATPARAMINT,    NULL,
                                NULL, NULL, NULL } };

static StratParamTab        bgraphbipartstcondtab[] = { /* Active graph condition parameter table */
                              { STRATNODECOND,       STRATPARAMDOUBLE, "bal",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.bbalval,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "edge",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.s.edgenbr,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "levl",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.levlnum,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "lmin0",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.compload0min,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "lmax0",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.compload0max,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "load",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.s.velosum,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "load0",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.compload0,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "vert",
                                (byte *) &bgraphdummy,
                                (byte *) &bgraphdummy.s.vertnbr,
                                NULL },
                              { STRATNODENBR,        STRATPARAMINT,    NULL,
                                NULL, NULL, NULL } };

StratTab                    bgraphbipartststratab = { /* Strategy tables for graph bipartitioning methods */
                              bgraphbipartstmethtab,
                              bgraphbipartstparatab,
                              bgraphbipartstcondtab };

/***********************************************/
/*                                             */
/* This is the generic bipartitioning routine. */
/*                                             */
/***********************************************/

/* This routine performs the bipartitioning of
** the given active graph according to the
** given strategy.
** It returns:
** - 0 : if bipartitioning could be computed.
** - 1 : on error.
*/

int
bgraphBipartSt (
Bgraph * restrict const       grafptr,            /*+ Active graph to bipartition +*/
const Strat * restrict const  straptr)            /*+ Bipartitioning strategy     +*/
{
  StratTest           testdat;                    /* Result of condition evaluation */
  BgraphStore         savetab[2];                 /* Results of the two strategies  */
  Gnum                compload0;
  int                 o;
  int                 o0;
  int                 o1;

#ifdef SCOTCH_DEBUG_BGRAPH2
  if (sizeof (Gnum) != sizeof (INT)) {
    errorPrint ("bgraphBipartSt: invalid type specification for parser variables");
    return (1);
  }
  if ((sizeof (BgraphBipartFmParam) > sizeof (StratNodeMethodData)) ||
      (sizeof (BgraphBipartGgParam) > sizeof (StratNodeMethodData)) ||
      (sizeof (BgraphBipartMlParam) > sizeof (StratNodeMethodData))) {
    errorPrint ("bgraphBipartSt: invalid type specification");
    return (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH2 */
#ifdef SCOTCH_DEBUG_BGRAPH1
  if (straptr->tablptr != &bgraphbipartststratab) {
    errorPrint ("bgraphBipartSt: invalid parameter (1)");
    return (1);
  }
#endif /* SCOTCH_DEBUG_BGRAPH1 */

  o = 0;
  switch (straptr->typeval) {
    case STRATNODECONCAT :
      o = bgraphBipartSt (grafptr, straptr->data.concdat.stratab[0]); /* Apply the first strategy      */
      if (o == 0)                                 /* If it worked all right                            */
        o |= bgraphBipartSt (grafptr, straptr->data.concdat.stratab[1]); /* Then apply second strategy */
      break;
    case STRATNODECOND :
      o = stratTestEval (straptr->data.conddat.testptr, &testdat, (void *) grafptr); /* Evaluate expression */
      if (o == 0) {                               /* If evaluation was correct */
#ifdef SCOTCH_DEBUG_VGRAPH2
        if ((testdat.testval != STRATTESTVAL) ||
            (testdat.nodeval != STRATPARAMLOG)) {
          errorPrint ("bgraphBipartSt: invalid test result");
          o = 1;
          break;
        }
#endif /* SCOTCH_DEBUG_VGRAPH2 */
        if (testdat.data.val.vallog == 1)         /* If expression is true                           */
          o = bgraphBipartSt (grafptr, straptr->data.conddat.stratab[0]); /* Apply first strategy    */
        else {                                    /* Else if expression is false                     */
          if (straptr->data.conddat.stratab[1] != NULL)  /* And if there is an else statement        */
            o = bgraphBipartSt (grafptr, straptr->data.conddat.stratab[1]); /* Apply second strategy */
        }
      }
      break;
    case STRATNODEEMPTY :
      break;
    case STRATNODESELECT :
      if (((bgraphStoreInit (grafptr, &savetab[0])) != 0) || /* Allocate save areas */
          ((bgraphStoreInit (grafptr, &savetab[1])) != 0)) {
        errorPrint      ("bgraphBipartSt: out of memory");
        bgraphStoreExit (&savetab[0]);
        return (1);
      }

      bgraphStoreSave     (grafptr, &savetab[1]); /* Save initial bipartition                   */
      o0 = bgraphBipartSt (grafptr, straptr->data.seledat.stratab[0]); /* Apply first strategy  */
      bgraphStoreSave     (grafptr, &savetab[0]); /* Save its result                            */
      bgraphStoreUpdt     (grafptr, &savetab[1]); /* Restore initial bipartition                */
      o1 = bgraphBipartSt (grafptr, straptr->data.seledat.stratab[1]); /* Apply second strategy */

      if ((o0 | o1) != 0) {                       /* If at least one method failed */
        if (o0 == 0)                              /* If first succeeded, take it   */
          goto take0;
        if (o1 != 0) {                            /* If none succeeded           */
          bgraphStoreUpdt (grafptr, &savetab[1]); /* Restore initial bipartition */
          o = 1;                                  /* Indicate error              */
        }
        goto take1;                               /* If second succeeded, keep it; anyway, go freeing data structures */
      }

      compload0 = grafptr->compload0avg + savetab[0].compload0dlt;
      o0 = ((compload0 < grafptr->compload0min) ||
            (compload0 > grafptr->compload0max)) ? 1 : 0;
      compload0 = grafptr->compload0avg + grafptr->compload0dlt;
      o1 = ((compload0 < grafptr->compload0min) ||
            (compload0 > grafptr->compload0max)) ? 1 : 0;

      if (o1 > o0)                                /* If first is balanced and second is not, take first */
        goto take0;
      if (o1 < o0)                                /* If second is balanced and first is not, take second */
        goto take1;
      if (o0 == 0) {                              /* If both are balanced    */
        if ( (savetab[0].commload >  grafptr->commload) || /* Compare on cut */
            ((savetab[0].commload == grafptr->commload) &&
             (ABS (savetab[0].compload0dlt) > ABS (grafptr->compload0dlt))))
          goto take1;
        else
          goto take0;
      }
      else {                                      /* If both are imbalanced */
        if ( (ABS (savetab[0].compload0dlt) >  ABS (grafptr->compload0dlt)) || /* Compare on imbalance */
            ((ABS (savetab[0].compload0dlt) == ABS (grafptr->compload0dlt)) &&
             (savetab[0].commload > grafptr->commload)))
          goto take1;
      }

take0:
      bgraphStoreUpdt (grafptr, &savetab[0]);     /* Restore first partition          */
take1:                                            /* Keep second partition by default */
      bgraphStoreExit (&savetab[1]);              /* Free both save areas             */
      bgraphStoreExit (&savetab[0]);
      break;
    case STRATNODEMETHOD :
      return (((BgraphBipartFunc) (straptr->tablptr->methtab[straptr->data.methdat.methnum].funcptr))
              (grafptr, (const void * const) &straptr->data.methdat.datadat));
    default :
      errorPrint ("bgraphBipartSt: invalid parameter (2)");
      return (1);
  }

  return (o);
}
