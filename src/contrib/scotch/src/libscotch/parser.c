/* Copyright 2004,2007,2008,2010,2012,2014,2016,2018,2021,2023,2024,2026 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : parser.c                                **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : Part of a static mapper.                **/
/**                This module is the strategy lexical and **/
/**                syntactic analyzer.                     **/
/**                                                        **/
/**   DATES      : # Version 3.1  : from : 07 nov 1995     **/
/**                                 to   : 02 may 1996     **/
/**                # Version 3.2  : from : 07 oct 1996     **/
/**                                 to   : 19 oct 1996     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 10 sep 2001     **/
/**                # Version 4.0  : from : 20 dec 2001     **/
/**                                 to   : 02 feb 2004     **/
/**                # Version 5.0  : from : 20 feb 2008     **/
/**                                 to   : 20 feb 2008     **/
/**                # Version 5.1  : from : 22 oct 2008     **/
/**                                 to   : 11 aug 2010     **/
/**                # Version 6.0  : from : 01 jun 2012     **/
/**                                 to   : 30 dec 2016     **/
/**                # Version 7.0  : from : 02 mar 2018     **/
/**                                 to   : 07 oct 2026     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define SCOTCH_PARSER

#include "module.h"
#include "common.h"

#undef INTEGER                                    /* In case someone defined them */
#undef DOUBLE

#include "parser.h"
#include "parser_yy.h"
#include "parser_ly.h"

/*
**  The static and global variables.
*/

static StratTab             stratdummytab = { NULL, NULL, NULL }; /* Dummy strategy table for the dummy empty object       */
Strat                       stratdummy = { &stratdummytab, STRATNODEEMPTY }; /* Dummy empty object for offset computations */

/**************************/
/*                        */
/* The strategy routines. */
/*                        */
/**************************/

/* This routine parses the given strategy
** string and builds the corresponding
** strategy tree.
** It returns:
** - !NULL  : pointer to the strategy.
** - NULL   : on error.
*/

Strat *
stratInit (
const StratTab * const      strattab,             /*+ Pointer to strategy parsing table +*/
const char * const          string)               /*+ Strategy string to parse          +*/
{
  Strat *             o;

#ifdef SCOTCH_DEBUG_PARSER1
  if ((strattab == NULL) || (string == NULL)) {
    errorPrint ("stratInit: invalid parameter");
    return     (NULL);
  }
#endif /* SCOTCH_DEBUG_PARSER1 */

  o = stratParserParse (strattab, string);        /* Parse strategy string */

  return (o);
}

/* This routine frees a strategy structure.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
stratExit (
Strat * const               straptr)
{
  StratParamTab *   paratab;                      /* Table of method parameters                  */
  byte *            poffptr;                      /* Offset of parameter within method structure */
  unsigned int      i;
  int               o;

  if (straptr == NULL)                            /* If node does not exist, abort */
    return (0);

  o = 0;                                          /* Assume everything will be all right */
  switch (straptr->typeval) {                     /* Recursively free sub-strategies     */
    case STRATNODECONCAT :
      o  = stratExit (straptr->data.concdat.stratab[0]);
      o |= stratExit (straptr->data.concdat.stratab[1]);
      break;
    case STRATNODECOND :
      o  = stratTestExit (straptr->data.conddat.testptr);
      o |= stratExit     (straptr->data.conddat.stratab[0]);
      if (straptr->data.conddat.stratab[1] != NULL)
        o |= stratExit (straptr->data.conddat.stratab[1]);
      break;
    case STRATNODESELECT :
      o  = stratExit (straptr->data.seledat.stratab[0]);
      o |= stratExit (straptr->data.seledat.stratab[1]);
      break;
    case STRATNODEEMPTY :                         /* Empty strategy node         */
      if (straptr == &stratdummy)                 /* If node is empty dummy node */
        return (0);                               /* Return without freeing it   */
      break;
    case STRATNODEMETHOD :                        /* Method strategy node       */
      paratab = straptr->tablptr->paratab;        /* Free the method parameters */
      for (i = 0; paratab[i].nameptr != NULL; i ++) {
        if ((paratab[i].methnum == straptr->data.methdat.methnum) && /* For all parameters of that method */
            (paratab[i].typeval == STRATPARAMSTRAT)) { /* Which are non-deprecated strategy parameters    */
          poffptr = (byte *) &straptr->data.methdat.datadat + /* Compute parameter offset within method   */
                    (paratab[i].doffptr - paratab[i].dbasptr);
          o |= stratExit (*((Strat **) poffptr)); /* Perform recursion */
        }
      }
      break;
    default :
      errorPrint ("stratExit: invalid strategy node");
      o = 1;
      break;
  }

  memFree (straptr);                              /* Free strategy structure itself */
  return  (o);                                    /* Return output code             */
}

/* This routine displays the given
** strategy structure.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
stratSave (
const Strat * const         straptr,
FILE * const                stream)
{
  unsigned int      paraflag;                     /* Flag set if method has parameters           */
  StratParamTab *   paratab;                      /* Pointer to method parameter table           */
  byte *            paraofft;                     /* Offset of parameter within method structure */
  unsigned int      i;
  int               o;

  o = 0;
  switch (straptr->typeval) {                     /* Recursively view sub-strategies */
    case STRATNODECOND :
      if ((fprintf (stream, "(/(") == EOF) ||
          (stratTestSave (straptr->data.conddat.testptr, stream) != 0) ||
          (fprintf (stream, ")?(") == EOF) ||
          (stratSave (straptr->data.conddat.stratab[0], stream) != 0))
        o = 1;
      if ((o == 0) && (straptr->data.conddat.stratab[1] != NULL)) {
        if ((fprintf (stream, "):(") == EOF) ||
            (stratSave (straptr->data.conddat.stratab[1], stream) != 0))
          o = 1;
      }
      if (o == 0)
        o = (fprintf (stream, ");)") == EOF);
      break;
    case STRATNODECONCAT :
      if ((stratSave (straptr->data.concdat.stratab[0], stream) != 0) ||
          (stratSave (straptr->data.concdat.stratab[1], stream) != 0))
        o = 1;
      break;
    case STRATNODESELECT :
      if ((fprintf (stream, "(") == EOF) ||
          (stratSave (straptr->data.seledat.stratab[0], stream) != 0) ||
          (fprintf (stream, "|") == EOF) ||
          (stratSave (straptr->data.seledat.stratab[1], stream) != 0) ||
          (fprintf (stream, ")") == EOF))
        o = 1;
    case STRATNODEEMPTY :
      break;
    case STRATNODEMETHOD :
      if (fprintf (stream, "%s", straptr->tablptr->methtab[straptr->data.methdat.methnum].nameptr) == EOF) { /* Print method name */
        o = 1;
        break;
      }
      paraflag = 0;                               /* No method parameters seen yet */
      paratab  = straptr->tablptr->paratab;
      for (i = 0; paratab[i].nameptr != NULL; i ++) {
        if ((paratab[i].methnum == straptr->data.methdat.methnum) && /* For all parameters of that method */
            ((paratab[i].typeval & STRATPARAMDEPRECATED) == 0)) { /* Which are not deprecated             */
          paraofft = (byte*) &straptr->data.methdat.datadat + /* Compute parameter offset within method   */
                     (paratab[i].doffptr - paratab[i].dbasptr);
          if (fprintf (stream, "%c%s=",           /* Open or continue parameter list */
                       ((paraflag ++ == 0) ? '{' : ','),
                       paratab[i].nameptr) == EOF) {
            o = 1;
            break;
          }
          switch (paratab[i].typeval) {           /* Print parameter value         */
            case STRATPARAMCASE :                 /* Case value                    */
              o = (fprintf (stream, "%c",         /* Print corresponding character */
                            ((char *) paratab[i].dselptr)[*((unsigned int *) paraofft)]) == EOF);
              break;
            case STRATPARAMINT :                  /* Integer value */
              o = (fprintf (stream, INTSTRING, *((INT *) paraofft)) == EOF);
              break;
            case STRATPARAMDOUBLE :               /* Double value */
              o = (fprintf (stream, "%g", *((double *) paraofft)) == EOF);
              break;
            case STRATPARAMSTRAT :                /* Strategy                      */
              o = stratSave (*((Strat **) paraofft), stream); /* Perform recursion */
              break;
            case STRATPARAMSTRING :               /* String value */
              o = (fprintf (stream, "%s", (char *) paraofft) == EOF);
              break;
            default :
              errorPrint ("stratSave: invalid parameter type");
              return (1);
          }
        }
        if (o != 0)                               /* If an error has occured */
          break;                                  /* Abort the loop          */
      }
      if ((o == 0) && (paraflag != 0))            /* If there is a parameter list */
        o |= (fprintf (stream, "}") == EOF);      /* Close it                     */
      break;
    default :
      errorPrint ("stratSave: invalid strategy node");
      return (1);
  }
  if (o != 0) {
    errorPrint ("stratSave: bad output");
  }

  return (o);
}

/*****************************************/
/*                                       */
/* These routines handle strategy tests. */
/*                                       */
/*****************************************/

/* This routine evaluates the
** given condition.
** It returns:
** - 0   : on success; eval updated.
** - !0  : on error.
*/

int
stratTestEval (
const StratTest * restrict const  testptr,
StratTest * restrict const        evalptr,        /*+ Place where to return final value                      +*/
const void * restrict const       dataptr)        /*+ Pointer to data structure where to read variables from +*/
{
  StratTest         testtab[2];                   /* Temporary evaluation variables */
  StratTestType     sign;                         /* Sign of comparison             */
  int               o;

#ifdef SCOTCH_DEBUG_PARSER1
  if ((testptr == NULL) || (evalptr == NULL) || (dataptr == NULL)) {
    errorPrint ("stratTestEval: invalid parameter");
    return (1);
  }
#endif /* SCOTCH_DEBUG_PARSER1 */

  o = 0;                                          /* Assume no error */
  switch (testptr->testval) {
    case STRATTESTNOT :                           /* Not operator */
      o = stratTestEval (testptr->data.testtab[0], evalptr, dataptr);
#ifdef SCOTCH_DEBUG_PARSER2
      if ((o == 0) && (evalptr->nodeval != STRATPARAMLOG)) {
        errorPrint ("stratTestEval: internal error (1)");
        return (1);
      }
#endif /* SCOTCH_DEBUG_PARSER2 */
      evalptr->data.val.vallog = 1 - evalptr->data.val.vallog;
      break;
    case STRATTESTAND :                           /* And operator */
      o = stratTestEval (testptr->data.testtab[0], evalptr, dataptr);
#ifdef SCOTCH_DEBUG_PARSER2
      if ((o == 0) && (evalptr->nodeval != STRATPARAMLOG)) {
        errorPrint ("stratTestEval: internal error (2)");
        return (1);
      }
#endif /* SCOTCH_DEBUG_PARSER2 */
      if ((o == 0) && (evalptr->data.val.vallog == 1)) {
        o = stratTestEval (testptr->data.testtab[1], evalptr, dataptr);
#ifdef SCOTCH_DEBUG_PARSER2
        if (evalptr->nodeval != STRATPARAMLOG) {
          errorPrint ("stratTestEval: internal error (3)");
          return (1);
        }
#endif /* SCOTCH_DEBUG_PARSER2 */
      }
      break;
    case STRATTESTOR :                            /* Or operator */
      o = stratTestEval (testptr->data.testtab[0], evalptr, dataptr);
#ifdef SCOTCH_DEBUG_PARSER2
      if ((o == 0) && (evalptr->nodeval != STRATPARAMLOG)) {
        errorPrint ("stratTestEval: internal error (4)");
        return (1);
      }
#endif /* SCOTCH_DEBUG_PARSER2 */
      if ((o == 0) && (evalptr->data.val.vallog == 0)) {
        o = stratTestEval (testptr->data.testtab[1], evalptr, dataptr);
#ifdef SCOTCH_DEBUG_PARSER2
        if (evalptr->nodeval != STRATPARAMLOG) {
          errorPrint ("stratTestEval: internal error (5)");
          return (1);
        }
#endif /* SCOTCH_DEBUG_PARSER2 */
      }
      break;
    case STRATTESTLT :                            /* Less-than operator    */
    case STRATTESTEQ :                            /* Equal-to operator     */
    case STRATTESTGT :                            /* Greater-than operator */
      o  = stratTestEval (testptr->data.testtab[0], &testtab[0], dataptr);
      o |= stratTestEval (testptr->data.testtab[1], &testtab[1], dataptr);
      o |= stratTestEvalCast (&testtab[0], &testtab[1]);
      if (o != 0)
        break;
      sign = STRATTESTNBR;                        /* In case of error */
      switch (testtab[0].nodeval) {
        case STRATPARAMDOUBLE :
          sign = (testtab[0].data.val.valdbl < testtab[1].data.val.valdbl) ? STRATTESTLT : ((testtab[0].data.val.valdbl > testtab[1].data.val.valdbl) ? STRATTESTGT : STRATTESTEQ);
          break;
        case STRATPARAMINT :
          sign = (testtab[0].data.val.valint < testtab[1].data.val.valint) ? STRATTESTLT : ((testtab[0].data.val.valint > testtab[1].data.val.valint) ? STRATTESTGT : STRATTESTEQ);
          break;
        default :
          errorPrint ("stratTestEval: internal error (6)");
          o = 1;
          break;
      }
      evalptr->nodeval         = STRATPARAMLOG;   /* Build test result */
      evalptr->data.val.vallog = (sign == testptr->testval);
      break;
    case STRATTESTADD :                           /* Addition operator */
      o  = stratTestEval (testptr->data.testtab[0], &testtab[0], dataptr);
      o |= stratTestEval (testptr->data.testtab[1], &testtab[1], dataptr);
      o |= stratTestEvalCast (&testtab[0], &testtab[1]);
      if (o != 0)
        break;
      if (testtab[0].nodeval == STRATPARAMDOUBLE)
        evalptr->data.val.valdbl = testtab[0].data.val.valdbl + testtab[1].data.val.valdbl;
      else
        evalptr->data.val.valint = testtab[0].data.val.valint + testtab[1].data.val.valint;
      evalptr->nodeval = testtab[0].nodeval;
      break;
    case STRATTESTSUB :                           /* Subtraction operator */
      o  = stratTestEval (testptr->data.testtab[0], &testtab[0], dataptr);
      o |= stratTestEval (testptr->data.testtab[1], &testtab[1], dataptr);
      o |= stratTestEvalCast (&testtab[0], &testtab[1]);
      if (o != 0)
        break;
      if (testtab[0].nodeval == STRATPARAMDOUBLE)
        evalptr->data.val.valdbl = testtab[0].data.val.valdbl - testtab[1].data.val.valdbl;
      else
        evalptr->data.val.valint = testtab[0].data.val.valint - testtab[1].data.val.valint;
      evalptr->nodeval = testtab[0].nodeval;
      break;
    case STRATTESTMUL :                           /* Multiplication operator */
      o  = stratTestEval (testptr->data.testtab[0], &testtab[0], dataptr);
      o |= stratTestEval (testptr->data.testtab[1], &testtab[1], dataptr);
      o |= stratTestEvalCast (&testtab[0], &testtab[1]);
      if (o != 0)
        break;
      if (testtab[0].nodeval == STRATPARAMDOUBLE)
        evalptr->data.val.valdbl = testtab[0].data.val.valdbl * testtab[1].data.val.valdbl;
      else
        evalptr->data.val.valint = testtab[0].data.val.valint * testtab[1].data.val.valint;
      evalptr->nodeval = testtab[0].nodeval;
      break;
    case STRATTESTMOD :                           /* Modulus operator */
      o  = stratTestEval (testptr->data.testtab[0], &testtab[0], dataptr);
      o |= stratTestEval (testptr->data.testtab[1], &testtab[1], dataptr);
      o |= stratTestEvalCast (&testtab[0], &testtab[1]);
      if (o != 0)
        break;
      if (testtab[0].nodeval == STRATPARAMDOUBLE)
        evalptr->data.val.valdbl = fmod (testtab[0].data.val.valdbl, testtab[1].data.val.valdbl);
      else
        evalptr->data.val.valint = testtab[0].data.val.valint % testtab[1].data.val.valint;
      evalptr->nodeval = testtab[0].nodeval;
      break;
    case STRATTESTVAL :                           /* Constant value */
      *evalptr = *testptr;                        /* Copy value     */
      break;
    case STRATTESTVAR :                           /* Variable */
      switch (testptr->nodeval) {
        case STRATPARAMDOUBLE :
          evalptr->data.val.valdbl = *((double *) ((byte *) dataptr + testptr->data.var.dataoft));
          break;
        case STRATPARAMINT :
          evalptr->data.val.valint = *((INT *) ((byte *) dataptr + testptr->data.var.dataoft));
          break;
        default :
          errorPrint ("stratTestEval: internal error (7)");
          o = 1;
          break;
      }
      evalptr->nodeval = testptr->nodeval;
      break;
    default :
      errorPrint ("stratTestEval: invalid condition type (%u)", testptr->testval);
      o = 1;
      break;
  }
  evalptr->testval = STRATTESTVAL;

  return (o);
}

/* This routine casts the type of one
** of the two input values so as to
** get the same type for both values.
** It returns:
** - void  : in all cases;
*/

static
int
stratTestEvalCast (
StratTest * const           tes0ptr,
StratTest * const           tes1ptr)
{
#ifdef SCOTCH_DEBUG_PARSER2
  if (((tes0ptr->nodeval != STRATPARAMINT) && (tes0ptr->nodeval != STRATPARAMDOUBLE)) ||
      ((tes1ptr->nodeval != STRATPARAMINT) && (tes1ptr->nodeval != STRATPARAMDOUBLE))) {
    errorPrint ("stratTestEvalCast: internal error");
    return (1);
  }
#endif /* SCOTCH_DEBUG_PARSER2 */

  if (tes0ptr->nodeval != tes1ptr->nodeval) {     /* If value types differ */
    if (tes0ptr->nodeval == STRATPARAMDOUBLE) {
      tes1ptr->nodeval         = STRATPARAMDOUBLE;
      tes1ptr->data.val.valdbl = (double) tes1ptr->data.val.valint;
    }
    else {
      tes0ptr->nodeval         = STRATPARAMDOUBLE;
      tes0ptr->data.val.valdbl = (double) tes0ptr->data.val.valint;
    }
  }

  return (0);
}

/* This routine fres the given
** strategy condition.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

int
stratTestExit (
StratTest * const           testptr)
{
  int               o;                            /* Output condition flag */

#ifdef SCOTCH_DEBUG_PARSER1
  if (testptr == NULL) {
    errorPrint ("stratTestExit: invalid parameter");
    return (1);
  }
#endif /* SCOTCH_DEBUG_PARSER1 */

  o = 0;
  switch (testptr->testval) {
    case STRATTESTNOT :                           /* Not operator   */
      o = stratTestExit (testptr->data.testtab[0]); /* Free the son */
      break;
    case STRATTESTAND :                           /* And operator            */
    case STRATTESTOR  :                           /* Or operator             */
    case STRATTESTLT  :                           /* Less-than operator      */
    case STRATTESTEQ  :                           /* Equal-to operator       */
    case STRATTESTGT  :                           /* Greater-than operator   */
    case STRATTESTMOD :                           /* Modulus operator        */
    case STRATTESTMUL :                           /* Multiplication operator */
    case STRATTESTADD :                           /* Addition operator       */
    case STRATTESTSUB :                           /* Subtraction operator    */
      o  = stratTestExit (testptr->data.testtab[0]); /* Free the sons        */
      o |= stratTestExit (testptr->data.testtab[1]);
      break;
    case STRATTESTVAL :                           /* Constant value */
    case STRATTESTVAR :                           /* Variable       */
      break;
    default :
      errorPrint ("stratTestExit: invalid condition type (%u)", testptr->testval);
      o = 1;
      break;
  }

  memFree (testptr);                              /* Free the structure */
  return  (o);
}

/* This routine displays the
** given strategy condition.
** It returns:
** - 0   : on success.
** - !0  : on error.
*/

static char                 strattestsaveop[STRATTESTNBR] = "|&!=><+-*%##";
static char *               strattestsavepa[2][2] = { { "(", ")" }, { "", "" } };

int
stratTestSave (
const StratTest * const     testptr,
FILE * const                stream)
{
  int               i;
  int               o;

#ifdef SCOTCH_DEBUG_PARSER1
  if ((testptr == NULL) || (stream == NULL)) {
    errorPrint ("stratTestSave: invalid parameter");
    return (1);
  }
#endif /* SCOTCH_DEBUG_PARSER1 */

  o = 0;                                          /* Assume no error */
  switch (testptr->testval) {
    case STRATTESTNOT :                           /* Not operator */
      if ((fprintf (stream, "!(") == EOF) ||
          (stratTestSave (testptr->data.testtab[0], stream) != 0) ||
          (fprintf (stream, ")") == EOF))
        o = 1;
      break;
    case STRATTESTAND :                           /* And operator            */
    case STRATTESTOR :                            /* Or operator             */
    case STRATTESTEQ :                            /* Equal-to operator       */
    case STRATTESTGT :                            /* Greater-than operator   */
    case STRATTESTLT :                            /* Less-than operator      */
    case STRATTESTADD :                           /* Addition operator       */
    case STRATTESTSUB :                           /* Subtraction operator    */
    case STRATTESTMUL :                           /* Multiplication operator */
    case STRATTESTMOD :                           /* Modulus operator        */
      i = (testptr->data.testtab[0]->testval < testptr->testval) ? 1 : 0;
      fprintf (stream, "%s", strattestsavepa[i][0]);
      o = stratTestSave (testptr->data.testtab[0], stream);
      fprintf (stream, "%s", strattestsavepa[i][1]);
      if (o == 0) {
        fprintf (stream, "%c", strattestsaveop[testptr->testval]);
        i = (testptr->data.testtab[1]->testval < testptr->testval) ? 1 : 0;
        fprintf (stream, "%s", strattestsavepa[i][0]);
        stratTestSave (testptr->data.testtab[1], stream);
        fprintf (stream, "%s", strattestsavepa[i][1]);
      }
      break;
    case STRATTESTVAL :                           /* Constant value */
      switch (testptr->nodeval) {
        case STRATPARAMDOUBLE :
          o = (fprintf (stream, "%lf", testptr->data.val.valdbl) == EOF);
          break;
        case STRATPARAMINT :
          o = (fprintf (stream, INTSTRING, (INT) testptr->data.val.valint) == EOF);
          break;
        default :
          errorPrint ("stratTestSave: invalid value type");
          o = 1;
      }
      break;
    case STRATTESTVAR :                           /* Variable */
      for (i = 0; testptr->data.var.datatab->condtab[i].nameptr != NULL; i ++) {
        if ((testptr->data.var.datatab->condtab[i].doffptr -
             testptr->data.var.datatab->condtab[i].dbasptr) == testptr->data.var.dataoft)
          break;
      }
      if (testptr->data.var.datatab->condtab[i].nameptr == NULL) {
        errorPrint ("stratTestSave: invalid variable displacement");
        return (1);
      }
      o = (fprintf (stream, "%s", testptr->data.var.datatab->condtab[i].nameptr) == EOF);
      break;
    default :
      errorPrint ("stratTestSave: invalid condition type (%u)", testptr->testval);
      o = 1;
  }

  return (o);
}

/*********************************/
/*                               */
/* The parser location routines. */
/*                               */
/*********************************/

void
parserLocationUpdate (
ParserLocation * const      locaptr,
const char * const          textptr)
{
  int                 textidx;

  locaptr->cobenum = locaptr->coennum;
  locaptr->libenum = locaptr->liennum;
  locaptr->tebeptr = locaptr->teenptr;

  for (textidx = 0; textptr[textidx] != '\0'; textidx ++) {
    if (textptr[textidx] == '\n') {
      locaptr->coennum = 0;
      locaptr->liennum ++;
    }
    else
      locaptr->coennum ++;
  }

  locaptr->teenptr += textidx;
}
