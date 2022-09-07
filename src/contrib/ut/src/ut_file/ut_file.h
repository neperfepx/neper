/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef UT_FILE_H
#define UT_FILE_H

/// \file ut_file.h
/// \brief File-related functions
/// \author Romain Quey
/// \bug No known bugs

/// \brief Open file
/// \param filename: file name
/// \param mode: opening mode, among "r", "w", "a", "R", "W" and "A"
/// \note \c lowercased modes generate a message, uppercased do not
/// \return file pointer
  extern FILE *ut_file_open (const char *filename, const char *mode);

/// \brief Close file
/// \param file: file pointer
/// \param filename: file name
/// \param mode: opening mode, among "r", "w", "a", "R", "W" and "A"; "nonl" can be added to generate a message without newline at the end
/// \note \c lowercased modes generate a message, uppercased do not
  extern void ut_file_close (FILE * file, const char *filename,
                             const char *mode);

/// \brief Print a file opening message
/// \param filename: file name
/// \param mode: opening mode, among "r", "w", "a", "R", "W" and "A"; "nonl" can be added to generate a message without newline at the end
/// \note \c lowercased modes generate a message, uppercased do not
  extern void ut_file_openmessage (const char *filename, const char *mode);

/// \brief Print a directory opening message
/// \param dirname: dir name
/// \param mode: opening mode, among "r", "w", "a", "R", "W" and "A"; "nonl" can be added to generate a message without newline at the end
/// \note \c lowercased modes generate a message, uppercased do not
  extern void ut_dir_openmessage (const char *dirname, const char *mode);

/// \brief Print a dir closing message
/// \param dirname: dir name
/// \param mode: opening mode, among "r", "w", "a", "R", "W" and "A"; "nonl" can be added to generate a message without newline at the end
/// \note \c lowercased modes generate a message, uppercased do not
  extern void ut_dir_closemessage (const char *dirname, const char *mode);

/// \brief Print a file closing message
/// \param filename: file name
/// \param mode: opening mode, among "r", "w", "a", "R", "W" and "A"; "nonl" can be added to generate a message without newline at the end
/// \note \c lowercased modes generate a message, uppercased do not
  extern void ut_file_closemessage (const char *filename, const char *mode);

/// \brief Print a directory creating message
/// \param dirname: directory name
/// \param mode: creation mode, among "c"
/// \note \c lowercased modes generate a message, uppercased do not
  extern void ut_file_createmessage (const char *dirname, const char *mode);

/// \brief Copy a file
/// \param filename1: input file name
/// \param filename2: output file name
  extern void ut_file_cp (const char *filename1, const char *filename2);

/// \brief Copy a file, only one line
/// \param file1: input file
/// \param file2: output file
/// \return 0 on success or -1 on failure
  extern int ut_file_cp_line (FILE * file1, FILE * file2);

/// \brief Copy a file, specify lines
/// \param file1: input file
/// \param file2: output file
/// \param lines: lines
/// \param lineqty: number of lines
  extern void ut_file_cp_lines (FILE * file1, FILE * file2, int *lines,
                                int lineqty);

/// \brief Copy a file, until char
/// \param file1: input file
/// \param file2: output file
/// \param c: stopping character
/// \return 0 on success (stopping at \c char) and -1 on failure (\c EOF reached)
  extern int ut_file_cp_untilchar (FILE * file1, FILE * file2, char c);

/// \brief Copy a file, specify number of words
/// \param file1: input file
/// \param file2: output file
/// \param qty: number of words
/// \param sep: separator
/// \param endsep: end separator
/// \return 0 on success and -1 on failure
  extern int ut_file_cp_qty (FILE * file1, int qty, FILE * file2,
                             const char *sep, const char *endsep);

/// \brief Skip words in file
/// \param file: file pointer
/// \param qty: number of words to skip
/// \param filename2: output file name
/// \return 0 on success and -1 on failure
/// \note \c qty can be negative
  extern int ut_file_skip (FILE * file, int qty);

/// \brief Skip characters in file
/// \param file: file pointer
/// \param qty: number of characters to skip
/// \return 0 on success and -1 on failure
/// \note \c qty cannot be negative
  extern int ut_file_skip_char (FILE * file, int qty);

/// \brief Skip lines in file
/// \param file: file pointer
/// \param qty: number of lines to skip
/// \return 0 on success and -1 on failure
/// \note \c qty cannot be negative
  extern int ut_file_skip_line (FILE * file, int qty);

/// \brief Get the number of lines of a file
/// \param filename: file name
/// \return number of lines
  extern int ut_file_nblines (const char *filename);

/// \brief Get the number of lines of a file, provide the file pointer
/// \param filename: file name
/// \return number of lines
  extern int ut_file_nblines_pointer (FILE * file);

/// \brief Get the number of words of a file
/// \param filename: file name
/// \return number of words
  extern int ut_file_nbwords (const char *filename);

/// \brief Get the number of words of a file, provide the file pointer
/// \param filename: file name
/// \return number of words
  extern int ut_file_nbwords_pointer (FILE * file);

/// \brief Get the number of columns of a file
/// \param filename: file name
/// \return number of columns
/// \note The number of columns is computed as the number of words divided by the number of lines
  extern int ut_file_nbcolumns (const char *filename);

/// \brief Test the number of words of a file
/// \param filename: file name
/// \param qty: reference number of words
/// \return 1 if the number of line is equal to \c qty and 0 otherwise
  extern int ut_file_nbwords_test (const char *filename, int qty);

/// \brief Test the number of words of a file and print message
/// \param filename: file name
/// \param qty: reference number of words
/// \return 0 on success (aborts otherwise)
  extern int ut_file_nbwords_testwmessage (const char *filename, int qty);

/// \brief Test is a file exists
/// \param filename: file name
/// \return 1 if file exists and 0 otherwise
  extern int ut_file_exist (const char *filename, ...);

/// \brief Get the next entry of a file as a string
/// \param file: file pointer
/// \param string: string (must be preallocated)
/// \return 1 on success and -1 on failure
/// \note string can be \c NULL
  extern int ut_file_nextstring (FILE * file, char *string);

/// \brief Test if the next entry of a file is a specified string
/// \param file: file pointer
/// \param string: string
/// \return 1 if string matches and 0 otherwise
  extern int ut_file_nextstring_test (FILE * file, char *string);

/// \brief Get the section level of the next string entry of a file
/// \param file: file pointer
/// \param *plevel: level
/// \return 0 on success and -1 on failure
  extern int ut_file_nextstring_sectionlevel (FILE * file, int *plevel);

/// \brief Determine if we are at the end of the file (excluding spaces, newlines and tabs)
/// \param file: file pointer
/// \return 1 on success and 0 otherwise
/// \note The position in the file is not changed
  extern int ut_file_isendoffile (FILE * file);

/// @cond INTERNAL
  extern void ut_file_nextline (FILE * file, char **pline);
/// @endcond

/// \brief Get the number of words on the next line of a file
/// \param file: file pointer
/// \param *pqty: number of words
  extern void ut_file_nextlinenbwords (FILE * file, int *pqty);

/// \brief Get the next position of a string in a file
/// \param file: file pointer
/// \param string: string
/// \return position (non-negative integer) on success and -1 on failure
  extern int ut_file_string_nextpos (FILE * file, const char *string);

/// \brief Get the number of occurrence of a string in a file
/// \param file: file pointer
/// \param string: string
/// \return number of occurrence
/// \note The position in the file is not changed.
  extern int ut_file_string_number (FILE * file, const char *string);

/// \brief Read the next entry of a file as a \c double written with a comma as decimal point
/// \param file: file pointer
/// \param *pval: value
  extern int ut_file_readwcomma (FILE * file, double *pval);

/// \brief Go to the next occurrence of a string in a file
/// \param file: file pointer
/// \param string: string
/// \return 0 on success and -1 on failure
/// \note In case of failure, the file pointer is likely to be located at the end of the file
  extern int ut_file_string_goto (FILE * file, const char *string);

/// \brief Go to the next line of a file
/// \param file: file pointer
/// \return 0 on success and -1 on failure
/// \note In case of failure, the file pointer is likely to be located at the end of the file
  extern int ut_file_goto_nextline (FILE * file);

/// \brief Go to the next occurrence of a line header in a file
/// \brief Read a file until string is matched
/// \param file: file pointer
/// \param header: line header
/// \return 0 on success and -1 on failure
/// \note In case of failure, the file pointer is likely to be located at the end of the file
  extern int ut_file_lineheader_goto (FILE * fp, const char *header);

/// \brief Squash the name of a file
/// \param filename1: input file name
/// \param *pfilename2: output file name
/// \note Remove \c file(), \c msfile() or \c @ to get only the file name and optional arguments (following the file name after \c ,)
  extern void ut_file_squashname (const char *filename1, char **pfilename2);

/// \brief Get the format of a file
/// \param filename: file name
/// \param *pformat: format, among \c tess, \c tesr, \c gmsh:msh or \c zset:geof
/// \return 0 on success and -1 on failure
  extern int ut_file_format (const char *filename, char **pformat);

/// \brief Test the format of a file
/// \param filename: file name
/// \param format: format, among \c tess, \c tesr, \c gmsh:msh or \c zset:geof
/// \return 1 if format matches and 0 otherwise
  extern int ut_file_testformat (const char *filename, const char *format);

/// \brief Scan and test the next string of a file
/// \param file: file pointer
/// \param string: string
/// \return 1 on success and 0 on failure
  extern int ut_file_string_scanandtest (FILE * file, const char *string);

/// @cond INTERNAL
  extern int ut_file_testpos (FILE *, const char *);
/// @endcond

/// \brief Scan and test if the next strings of a file is *file <file_name>, and return a pointer to the file
/// \param file: file pointer
/// \param dirname: name of the directory of the opened file
/// \param pfile: new file pointer
extern void ut_file_scan_file (FILE *file, char *dirname, FILE **pfile, char **pfilename);

extern void ut_file_dir_basename_extension_filename (char *dir, char *basename,
                                                     char *extension, char **pfilename);

#endif                          /* UT_FILE_H */

#ifdef __cplusplus
}
#endif
