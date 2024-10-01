/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_PRINT_H
#define UT_PRINT_H

/// \file ut_print.h
/// \brief Printing functions
/// \author Romain Quey
/// \bug No known bugs
/// \todo Improve \c ut_print_messagewnc
/// \todo change \c wnc to something more explicit

/// \brief Print a 9-character line header
/// \param head: type of header, 0: info, 1: warning, 2: error, -1; debug
  extern void ut_print_lineheader (int head);

/// \brief Print level header
/// \param indentation level: 1: none, 2: "  -", 3, "    >", 4: "      ."
  extern void ut_print_level (int level);

/// \brief Print increment header
/// \param incr: current increment
/// \param incrf: final increment
/// \note if \c incrf <= 0, then only the increment number is printed
  extern void ut_print_incr (int incr, int incrf);

/// \brief Print a message
/// \param head: type of header
/// \param level: indentation level
/// \param message: message (can be multiple arguments)
/// \see ut_print_lineheader, ut_print_level
  extern void ut_print_message (int head, int level, const char *message,
                                ...);

/// \brief Print a message including increments in header
/// \param head: type of header
/// \param level: indentation level
/// \param incr: current increment
/// \param incrf: final increment
/// \param message: message (can be multiple arguments)
/// \see ut_print_lineheader, ut_print_level, ut_print_incr
  extern void ut_print_message_incr (int head, int level, int incr, int
                                     incrf, const char *message, ...);

/// \brief Print a message including increments in header
/// \param head: type of header
/// \param nbcolmax: indentation level
/// \param message: message
  extern void ut_print_messagewnc (int head, int nbcolmax,
                                   const char *message);

/// \brief Print a bug message
  extern void ut_print_bug (void);

/// \brief Print a Neper bug message
  extern void ut_print_neperbug (void);

/// \brief Print an expression bug message
/// \note Print that the expression cannot be processed
  extern void ut_print_exprbug (char *expr);

/// \brief Print elapsed time message
/// \param beg_time: beginning time
/// \param end_time: end time
  extern void ut_print_elapsedtime (struct timeval beg_time,
                                    struct timeval end_time);

/// \brief Print a \c string within a certain number of columns
/// \param file: file pointer
/// \param *pnbcol: current column position
/// \param nbcolmax: maximal number of columns
/// \param string: character string (can be multiple arguments)
  extern void ut_print_wnc (FILE * file, int *pnbcol, int nbcolmax,
                            const char *string, ...);

/// \brief Print an \c int within a certain number of columns
/// \param file: file pointer
/// \param nb: number
/// \param *pnbcol: current column position
/// \param nbcolmax: maximal number of columns
  extern void ut_print_wnc_int (FILE * file, int nb, int *pnbcol,
                                int nbcolmax);

/// \brief Print a character string within a certain number of columns
/// \param file: file pointer
/// \param string: character string
/// \param *pnbcol: current column position
/// \param nbcolmax: maximal number of columns
  extern int ut_print_wnc_string (FILE * file, const char *string,
                                  int *pnbcol, int nbcolmax);

/// \brief Print an \c int within a certain number of columns, including a line header
/// \param file: file pointer
/// \param nb: number
/// \param *pnbcol: current column position
/// \param nbcolmax: maximal number of columns
/// \param head: line header
  extern void ut_print_wnc_int_header (FILE * file, int nb, int *pnbcol,
                                       int nbcolmax, const char *head);

/// \brief Print a \c string within a certain number of columns and a maximum number of elements per line
/// \param file: file pointer
/// \param *pnbcol: current column position
/// \param nbcolmax: maximal number of columns
/// \param *pnbelts: current number of elements on the line
/// \param nbeltmax: maximal number of elements on the line
/// \param string: character string (can be multiple arguments)
  extern void ut_print_wnc_wnelts (FILE * file, int *pnbcol, int nbcolmax,
                                   int *pnbelts, int nbeltmax,
                                   const char *string, ...);

/// \brief Print a missing-file error message
/// \param filename: file name
  extern void ut_print_missingfile (const char *filename);

/// \brief Print a module header
/// \param module: module
/// \param fargv: argv from the initialization file
/// \param fargc: argc from the initialization file
/// \param argv: argv from the command line
/// \param argc: argc from the command line
  extern void ut_print_moduleheader (const char *module, int fargv,
                                     char **fargc, int argv, char **argc);

/// \brief Print progress information (percentage) and linebreak
/// \param file: output stream (usually \c stdout)
/// \param current: current value
/// \param total: final value
/// \param format: format, e.g. "%.8f" or "foo %.8f" (the percentage is written)
/// \param prevprogress: previous printed progress (does not need to be set)
/// \note A linebreak is made when 100% is reached
  extern void ut_print_progress (FILE * file, long current, long total,
                                 const char *format, char *prevprogress);

/// \brief Print progress information (percentage)
/// \param file: output stream (usually \c stdout)
/// \param current: current value
/// \param total: final value
/// \param format: format, e.g. "%.8f" or "foo %.8f" (the percentage is written)
/// \param prevprogress: previous printed progress (does not need to be set)
  extern void ut_print_progress_nonl (FILE * file, long current, long total,
                                      const char *format, char *prevprogress);

/// \brief Print the GPL3 license
/// \param file: output file pointer
  extern void ut_print_gplv3 (FILE * file);

/// \brief Clear last characters on the current line
/// \param file: file pointer
/// \param qty: number of characters
  extern void ut_print_clearline (FILE * file, int qty);

/// \brief Print whitespaces on the line
/// \param file: file pointer
/// \param qty: number of whitespaces
  extern void ut_print_whitespaces (FILE * file, int qty);

#endif                          /* UT_PRINT_H */

#ifdef __cplusplus
}
#endif
