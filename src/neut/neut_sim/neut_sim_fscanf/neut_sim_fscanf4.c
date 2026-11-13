/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#include"neut_sim_fscanf_.h"

// setting a value of a section of the definition file of a result (section is NULL
// out of any section and empty in an unknown section)
void
neut_sim_fscanf_resdef_value (char *name, char *section, char *value, char **pexpr, char **ptype)
{
  int colqty;

  if (!section)
    ut_print_message (1, 3, "File `%s': `%s' is not in a section.  Ignoring it...\n", name, value);

  else if (!strcmp (section, "*expr"))
    ut_string_string (value, pexpr);

  else if (!strcmp (section, "*type"))
  {
    if (!neut_sim_simres_type_colqty (value, &colqty))
      ut_string_string (value, ptype);
    else
      ut_print_message (1, 3, "File `%s': invalid type `%s' (expected int, real, vector, skewtensor, symtensor, tensor or ori).  Ignoring it...\n",
                        name, value);
  }

  return;
}

// printing an error on reading legacy .sim, with the line of the current
// position
void
neut_sim_fscanf_error (FILE *file, const char *format, ...)
{
  int c, line = 1;
  long pos = ftell (file);
  char *message = NULL;
  va_list args, args2;

  va_start (args, format);
  va_copy (args2, args);
  message = ut_alloc_1d_char (vsnprintf (NULL, 0, format, args) + 1);
  va_end (args);
  vsprintf (message, format, args2);
  va_end (args2);

  rewind (file);
  while (ftell (file) < pos && (c = fgetc (file)) != EOF)
    if (c == '\n')
      line++;

  ut_print_message (2, 2, "Failed to read .sim, line %d: %s\n", line, message);

  ut_free_1d_char (&message);

  return;
}
