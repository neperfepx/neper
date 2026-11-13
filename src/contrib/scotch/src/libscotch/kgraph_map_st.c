/* Copyright 2004,2007,2009-2011,2014,2018,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : kgraph_map_st.c                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                Sebastien FOURESTIER (v6.0)             **/
/**                                                        **/
/**   FUNCTION   : This module contains the strategy and   **/
/**                method tables for the multi-way static  **/
/**                mapping routines.                       **/
/**                                                        **/
/**   DATES      : # Version 3.2  : from : 15 oct 1996     **/
/**                                 to   : 26 may 1998     **/
/**                # Version 3.3  : from : 19 oct 1998     **/
/**                                 to   : 17 may 1999     **/
/**                # Version 3.4  : from : 12 sep 2001     **/
/**                                 to   : 12 sep 2001     **/
/**                # Version 4.0  : from : 12 jan 2004     **/
/**                                 to   : 05 jan 2005     **/
/**                # Version 5.1  : from : 04 oct 2009     **/
/**                                 to   : 29 mar 2011     **/
/**                # Version 6.0  : from : 03 mar 2011     **/
/**                                 to   : 04 aug 2018     **/
/**                # Version 6.1  : from : 18 jul 2021     **/
/**                                 to   : 18 jul 2021     **/
/**                # Version 7.0  : from : 20 jan 2023     **/
/**                                 to   : 08 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "parser.h"
#include "gain.h"
#include "fibo.h"
#include "graph.h"
#include "arch.h"
#include "graph_coarsen.h"
#include "mapping.h"
#include "bgraph.h"
#include "bgraph_bipart_st.h"
#include "kgraph.h"
#include "kgraph_map_bd.h"
#include "kgraph_map_cp.h"
#include "kgraph_map_df.h"
#include "kgraph_map_ex.h"
#include "kgraph_map_fm.h"
#include "kgraph_map_ml.h"
#include "kgraph_map_rb.h"
#include "kgraph_map_st.h"

/*
**  The static and global variables.
*/

static Kgraph               kgraphdummy;          /*+ Dummy active graph for offset computations +*/

static union {
  KgraphMapBdParam          param;
  StratNodeMethodData       padding;
} kgraphmapstdefaultbd = { { 3, &stratdummy, &stratdummy } };

static union {
  StratNodeMethodData       padding;
} kgraphmapstdefaultcp;

static union {
  KgraphMapDfParam          param;
  StratNodeMethodData       padding;
} kgraphmapstdefaultdf = { { 40, 1.0F, 0.0F } };

static union {
  KgraphMapExParam          param;
  StratNodeMethodData       padding;
} kgraphmapstdefaultex = { { 0.05 } };

static union {
  KgraphMapFmParam          param;
  StratNodeMethodData       padding;
} kgraphmapstdefaultfm = { { 200, ~0, 0.05 } };

static union {
  KgraphMapMlParam          param;
  StratNodeMethodData       padding;
} kgraphmapstdefaultml = { { 100, 0.8, &stratdummy, &stratdummy, 0 } };

static union {
  KgraphMapRbParam          param;
  StratNodeMethodData       padding;
} kgraphmapstdefaultrb = { { 1, 1, KGRAPHMAPRBPOLINGSIZE, &stratdummy, 0.05 } };

static StratMethodTab       kgraphmapstmethtab[] = { /* Mapping methods array */
                              { KGRAPHMAPSTMETHBD, "b",  (StratMethodFunc) kgraphMapBd, &kgraphmapstdefaultbd },
                              { KGRAPHMAPSTMETHCP, "c",  (StratMethodFunc) kgraphMapCp, &kgraphmapstdefaultcp },
                              { KGRAPHMAPSTMETHDF, "d",  (StratMethodFunc) kgraphMapDf, &kgraphmapstdefaultdf },
                              { KGRAPHMAPSTMETHEX, "x",  (StratMethodFunc) kgraphMapEx, &kgraphmapstdefaultex },
                              { KGRAPHMAPSTMETHFM, "f",  (StratMethodFunc) kgraphMapFm, &kgraphmapstdefaultfm },
                              { KGRAPHMAPSTMETHML, "m",  (StratMethodFunc) kgraphMapMl, &kgraphmapstdefaultml },
                              { KGRAPHMAPSTMETHRB, "r",  (StratMethodFunc) kgraphMapRb, &kgraphmapstdefaultrb },
                              { -1,                NULL, (StratMethodFunc) NULL,        NULL } };

static StratParamTab        kgraphmapstparatab[] = { /* Method parameter list */
                              { KGRAPHMAPSTMETHBD,  STRATPARAMINT,    "width",
                                (byte *) &kgraphmapstdefaultbd.param,
                                (byte *) &kgraphmapstdefaultbd.param.distmax,
                                NULL },
                              { KGRAPHMAPSTMETHBD,  STRATPARAMSTRAT,  "bnd",
                                (byte *) &kgraphmapstdefaultbd.param,
                                (byte *) &kgraphmapstdefaultbd.param.stratbnd,
                                (void *) &kgraphmapststratab },
                              { KGRAPHMAPSTMETHBD,  STRATPARAMSTRAT,  "org",
                                (byte *) &kgraphmapstdefaultbd.param,
                                (byte *) &kgraphmapstdefaultbd.param.stratorg,
                                (void *) &kgraphmapststratab },
                              { KGRAPHMAPSTMETHDF,  STRATPARAMINT,    "pass",
                                (byte *) &kgraphmapstdefaultdf.param,
                                (byte *) &kgraphmapstdefaultdf.param.passnbr,
                                NULL },
                              { KGRAPHMAPSTMETHDF,  STRATPARAMDOUBLE, "dif",
                                (byte *) &kgraphmapstdefaultdf.param,
                                (byte *) &kgraphmapstdefaultdf.param.cdifval,
                                NULL },
                              { KGRAPHMAPSTMETHDF,  STRATPARAMDOUBLE, "rem",
                                (byte *) &kgraphmapstdefaultdf.param,
                                (byte *) &kgraphmapstdefaultdf.param.cremval,
                                NULL },
                              { KGRAPHMAPSTMETHEX,  STRATPARAMDOUBLE, "bal",
                                (byte *) &kgraphmapstdefaultex.param,
                                (byte *) &kgraphmapstdefaultex.param.kbalval,
                                (void *) &kgraphmapststratab },
                              { KGRAPHMAPSTMETHFM,  STRATPARAMINT,    "move",
                                (byte *) &kgraphmapstdefaultfm.param,
                                (byte *) &kgraphmapstdefaultfm.param.movenbr,
                                NULL },
                              { KGRAPHMAPSTMETHFM,  STRATPARAMINT,    "pass",
                                (byte *) &kgraphmapstdefaultfm.param,
                                (byte *) &kgraphmapstdefaultfm.param.passnbr,
                                NULL },
                              { KGRAPHMAPSTMETHFM,  STRATPARAMDOUBLE, "bal",
                                (byte *) &kgraphmapstdefaultfm.param,
                                (byte *) &kgraphmapstdefaultfm.param.deltval,
                                NULL },
                              { KGRAPHMAPSTMETHML,  STRATPARAMSTRAT,  "asc",
                                (byte *) &kgraphmapstdefaultml.param,
                                (byte *) &kgraphmapstdefaultml.param.stratasc,
                                (void *) &kgraphmapststratab },
                              { KGRAPHMAPSTMETHML,  STRATPARAMSTRAT,  "low",
                                (byte *) &kgraphmapstdefaultml.param,
                                (byte *) &kgraphmapstdefaultml.param.stratlow,
                                (void *) &kgraphmapststratab },
                              { KGRAPHMAPSTMETHML,  STRATPARAMINT,    "vert",
                                (byte *) &kgraphmapstdefaultml.param,
                                (byte *) &kgraphmapstdefaultml.param.coarnbr,
                                NULL },
                              { KGRAPHMAPSTMETHML,  STRATPARAMDOUBLE, "rat",
                                (byte *) &kgraphmapstdefaultml.param,
                                (byte *) &kgraphmapstdefaultml.param.coarval,
                                NULL },
                              { KGRAPHMAPSTMETHML,  STRATPARAMCASE,   "type",
                                (byte *) &kgraphmapstdefaultml.param,
                                (byte *) &kgraphmapstdefaultml.param.typeval,
                                (void *) "hscd" },
                              { KGRAPHMAPSTMETHRB,  STRATPARAMCASE,   "job",
                                (byte *) &kgraphmapstdefaultrb.param,
                                (byte *) &kgraphmapstdefaultrb.param.flagjobtie,
                                (void *) "ut" },
                              { KGRAPHMAPSTMETHRB,  STRATPARAMDOUBLE, "bal",
                                (byte *) &kgraphmapstdefaultrb.param,
                                (byte *) &kgraphmapstdefaultrb.param.kbalval,
                                NULL },
                              { KGRAPHMAPSTMETHRB,  STRATPARAMCASE,   "map",
                                (byte *) &kgraphmapstdefaultrb.param,
                                (byte *) &kgraphmapstdefaultrb.param.flagmaptie,
                                (void *) "ut" },
                              { KGRAPHMAPSTMETHRB,  STRATPARAMCASE,   "poli",
                                (byte *) &kgraphmapstdefaultrb.param,
                                (byte *) &kgraphmapstdefaultrb.param.polival,
                                (void *) "rlsLS" },
                              { KGRAPHMAPSTMETHRB,  STRATPARAMSTRAT,  "sep",
                                (byte *) &kgraphmapstdefaultrb.param,
                                (byte *) &kgraphmapstdefaultrb.param.strat,
                                (void *) &bgraphbipartststratab },
                              { KGRAPHMAPSTMETHNBR, STRATPARAMINT,    NULL,
                                NULL, NULL, NULL } };

static StratParamTab        kgraphmapstcondtab[] = { /* Graph condition parameter table */
                              { STRATNODECOND,       STRATPARAMINT,    "load",
                                (byte *) &kgraphdummy,
                                (byte *) &kgraphdummy.s.velosum,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "levl",
                                (byte *) &kgraphdummy,
                                (byte *) &kgraphdummy.levlnum,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "edge",
                                (byte *) &kgraphdummy,
                                (byte *) &kgraphdummy.s.edgenbr,
                                NULL },
                              { STRATNODECOND,       STRATPARAMINT,    "vert",
                                (byte *) &kgraphdummy,
                                (byte *) &kgraphdummy.s.vertnbr,
                                NULL },
                              { STRATNODENBR,        STRATPARAMINT,    NULL,
                                NULL, NULL, NULL } };

StratTab                    kgraphmapststratab = { /* Strategy tables for graph mapping methods */
                              kgraphmapstmethtab,
                              kgraphmapstparatab,
                              kgraphmapstcondtab };

/****************************************/
/*                                      */
/* This is the generic mapping routine. */
/*                                      */
/****************************************/

/* This routine computes the given
** mapping according to the given
** strategy.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
kgraphMapSt (
Kgraph * restrict const       grafptr,            /*+ Mapping graph    +*/
const Strat * restrict const  straptr)            /*+ Mapping strategy +*/
{
  StratTest           testdat;                    /* Result of condition evaluation         */
  KgraphStore         savetab[2];                 /* Results of the two strategies          */
  Gnum                comploaddltasu[2];          /* Absolute sum of computation load delta */
  Anum                partnum;
  int                 o;
  int                 o0;
  int                 o1;

#ifdef SCOTCH_DEBUG_KGRAPH2
  if (sizeof (Gnum) != sizeof (INT)) {
    errorPrint ("kgraphMapSt: invalid type specification for parser variables");
    return (1);
  }
  if ((sizeof (KgraphMapRbParam) > sizeof (StratNodeMethodData))) {
    errorPrint ("kgraphMapSt: invalid type specification");
    return (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
#ifdef SCOTCH_DEBUG_KGRAPH1
  if ((straptr->tablptr != &kgraphmapststratab) &&
      (straptr          != &stratdummy)) {
    errorPrint ("kgraphMapSt: invalid parameter (1)");
    return (1);
  }
#endif /* SCOTCH_DEBUG_KGRAPH1 */

  o = 0;
  switch (straptr->typeval) {
    case STRATNODECONCAT :
      o = kgraphMapSt (grafptr, straptr->data.concdat.stratab[0]); /* Apply first strategy          */
      if (o == 0)                                 /* If it worked all right                         */
        o |= kgraphMapSt (grafptr, straptr->data.concdat.stratab[1]); /* Then apply second strategy */
      break;
    case STRATNODECOND :
      o = stratTestEval (straptr->data.conddat.testptr, &testdat, (void *) grafptr); /* Evaluate expression */
      if (o == 0) {                               /* If evaluation was correct */
#ifdef SCOTCH_DEBUG_KGRAPH2
        if ((testdat.testval != STRATTESTVAL) ||
            (testdat.nodeval != STRATPARAMLOG)) {
          errorPrint ("kgraphMapSt: invalid test result");
          o = 1;
          break;
        }
#endif /* SCOTCH_DEBUG_KGRAPH2 */
        if (testdat.data.val.vallog == 1)         /* If expression is true                        */
          o = kgraphMapSt (grafptr, straptr->data.conddat.stratab[0]); /* Apply first strategy    */
        else {                                    /* Else if expression is false                  */
          if (straptr->data.conddat.stratab[1] != NULL) /* And if there is an else statement      */
            o = kgraphMapSt (grafptr, straptr->data.conddat.stratab[1]); /* Apply second strategy */
        }
      }
      break;
    case STRATNODEEMPTY :
      break;
    case STRATNODESELECT :
      if (((kgraphStoreInit (grafptr, &savetab[0])) != 0) || /* Allocate save areas */
          ((kgraphStoreInit (grafptr, &savetab[1])) != 0)) {
        errorPrint ("kgraphMapSt: out of memory");
        kgraphStoreExit (&savetab[0]);
        return (1);
      }

      kgraphStoreSave  (grafptr, &savetab[1]);    /* Save initial mapping                    */
      o0 = kgraphMapSt (grafptr, straptr->data.seledat.stratab[0]); /* Apply first strategy  */
      kgraphStoreSave  (grafptr, &savetab[0]);    /* Save its result                         */
      kgraphStoreUpdt  (grafptr, &savetab[1]);    /* Restore initial mapping                 */
      o1 = kgraphMapSt (grafptr, straptr->data.seledat.stratab[1]); /* Apply second strategy */

      if ((o0 | o1) != 0) {                       /* If at least one method failed */
        if (o0 == 0)                              /* If first succeeded, take it   */
          goto take0;
        if (o1 != 0) {                            /* If none succeeded       */
          kgraphStoreUpdt (grafptr, &savetab[1]); /* Restore initial mapping */
          o = 1;                                  /* Indicate error          */
        }
        goto take1;                               /* If second succeeded, keep it; anyway, go freeing data structures */
      }

      comploaddltasu[0] = 0;
      for (partnum = 0; partnum < savetab[0].domnnbr; partnum ++) {
        Gnum                comploadadlt;

        comploadadlt = ABS (savetab[0].comploaddlt[partnum]);
        if (comploadadlt > ((Gnum) ((double) savetab[0].comploadavg[partnum] * savetab[0].kbalval)))
          o0 = 1;
        comploaddltasu[0] += comploadadlt;
      }

      comploaddltasu[1] = 0;
      for (partnum = 0; partnum < grafptr->m.domnnbr; partnum ++) {
        Gnum                comploadadlt;

        comploadadlt = ABS (grafptr->comploaddlt[partnum]);
        if (comploadadlt > ((Gnum) ((double) grafptr->comploadavg[partnum] * grafptr->kbalval)))
          o1 = 1;
        comploaddltasu[1] += comploadadlt;
      }

      if (o0 > o1)                                /* If first mapping is imbalanced  */
        goto take1;                               /* Take the second one             */
      if (o0 < o1)                                /* If second mapping is imbalanced */
        goto take0;                               /* Take the first one              */

      if (o0 == 0) {                              /* If both are valid       */
        if ( (savetab[0].commload >  grafptr->commload) || /* Compare on cut */
            ((savetab[0].commload == grafptr->commload) &&
             (comploaddltasu[0] > comploaddltasu[1])))
          goto take1;
      }
      else {                                      /* If both are invalid         */
        if ( (comploaddltasu[0] >  comploaddltasu[1]) || /* Compare on imbalance */
            ((comploaddltasu[0] == comploaddltasu[1]) &&
             (savetab[0].commload > grafptr->commload)))
          goto take1;
      }

take0:
      kgraphStoreUpdt (grafptr, &savetab[0]);     /* Restore first mapping          */
take1:                                            /* Keep second mapping by default */
      kgraphStoreExit (&savetab[1]);              /* Free both save areas           */
      kgraphStoreExit (&savetab[0]);
      break;
#ifdef SCOTCH_DEBUG_KGRAPH1
    case STRATNODEMETHOD :
#else /* SCOTCH_DEBUG_KGRAPH1 */
    default :
#endif /* SCOTCH_DEBUG_KGRAPH1 */
      return (((KgraphMapFunc) (straptr->tablptr->methtab[straptr->data.methdat.methnum].funcptr))
              (grafptr, (void *) &straptr->data.methdat.datadat));
#ifdef SCOTCH_DEBUG_KGRAPH1
    default :
      errorPrint ("kgraphMapSt: invalid parameter (2)");
      return (1);
#endif /* SCOTCH_DEBUG_KGRAPH1 */
  }
  return (o);
}
