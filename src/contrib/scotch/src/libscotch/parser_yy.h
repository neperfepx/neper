/* Copyright 2004,2007,2010,2018,2019 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : parser_yy.h                             **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : Part of a static mapper.                **/
/**                These lines are the declarations for    **/
/**                the strategy strings syntactic parser.  **/
/**                                                        **/
/**   DATES      : # Version 3.1  : from : 07 nov 1995     **/
/**                                 to   : 30 may 1996     **/
/**                # Version 3.2  : from : 03 oct 1996     **/
/**                                 to   : 19 oct 1996     **/
/**                # Version 3.3  : from : 01 oct 1998     **/
/**                                 to   : 01 oct 1998     **/
/**                # Version 4.0  : from : 20 dec 2001     **/
/**                                 to   : 21 dec 2001     **/
/**                # Version 5.1  : from : 09 jun 2009     **/
/**                                 to   : 07 aug 2010     **/
/**                # Version 6.0  : from : 27 apr 2018     **/
/**                                 to   : 26 oct 2019     **/
/**                                                        **/
/************************************************************/

/*
**  The defines.
*/

/* Change some function names. */

#if ((defined SCOTCH_RENAME_PARSER) || (defined yylex)) /* If prefix renaming    */
#define scotchyyparse               stratParserParse2 /* Parser function name    */
#ifndef yylval
#define yylval                      SCOTCH_NAME_MACRO3 (scotchyy, SCOTCH_NAME_SUFFIXC, lval) /* It should be Yacc/Bison's job to redefine it! */
#endif /* yylval              */
#else /* SCOTCH_RENAME_PARSER */
#define yylex                       stratParserLex /* Lexical analyzer           */
#define yyparse                     stratParserParse2 /* Parser function name    */
#endif /* SCOTCH_RENAME_PARSER */

/*
**  The function prototypes.
*/

Strat *                     stratParserParse    (const StratTab * const, const char * const);

int                         yylex               (void);
int                         yyparse             (void);

#ifdef PARSER_YY
static int                  yyerror             (const char * const);
#endif /* PARSER_YY */
