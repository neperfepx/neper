/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<limits.h>
#include"ut.h"

int
ut_file_nblines (const char *filename)
{
  int nblines;
  FILE *file;
  char trash[10000];

  file = fopen (filename, "r");

  nblines = 0;
  while (fgets (trash, 10000, file))
    nblines++;

  fclose (file);

  return nblines;
}

int
ut_file_nblines_pointer (FILE * file)
{
  int nblines;
  char trash[10000];

  nblines = 0;
  while (fgets (trash, 10000, file))
    nblines++;

  fseek (file, 0, 0);

  return nblines;
}

int
ut_file_nbwords (const char *filename)
{
  int nbwords;
  FILE *file;
  char trash[10000];

  file = ut_file_open (filename, "R");

  nbwords = 0;
  while (fscanf (file, "%s", trash) == 1)
    nbwords++;

  ut_file_close (file, filename, "R");

  return nbwords;
}

int
ut_file_nbwords_test (const char *filename, int qty)
{
  return ut_file_nbwords (filename) - qty;
}

int
ut_file_nbwords_testwmessage (const char *filename, int qty)
{
  if (ut_file_nbwords (filename) != qty)
    ut_print_message (2, 0,
                      "Wrong number of data in input file %s (%d instead of %d).\n",
                      filename, ut_file_nbwords (filename), qty);

  return 0;
}

int
ut_file_nbwords_pointer (FILE * file)
{
  int nbwords;
  char trash[10000];

  nbwords = 0;
  while (fscanf (file, "%s", trash) == 1)
    nbwords++;

  fseek (file, 0, 0);

  return nbwords;
}

int
ut_file_nbcolumns (const char *filename)
{
  if (!ut_file_exist (filename))
    return -1;
  else if (!ut_file_nbwords (filename))
    return -1;
  else
    return ut_file_nbwords (filename) / ut_file_nblines (filename);
}

int
ut_file_exist (const char *name, ...)
{
  int res;
  va_list args;
  char *fullname = NULL;
  char *fullname2 = NULL;
  FILE *file = NULL;

  if (!name)
    return 0;

  fullname = ut_alloc_1d_char (1000);
  fullname2 = ut_alloc_1d_char (1000);

  va_start (args, name);
  vsprintf (fullname, name, args);

  if (!strncmp (fullname, "file(", 5)
      && fullname[strlen (fullname) - 1] == ')')
    fullname2 = strncpy (fullname2, fullname + 5, strlen (fullname) - 6);
  else if (!strncmp (fullname, "msfile(", 7)
           && fullname[strlen (fullname) - 1] == ')')
    fullname2 = strncpy (fullname2, fullname + 7, strlen (fullname) - 8);
  else
    fullname2 = strcpy (fullname2, fullname);

  int qty;
  char **vals = NULL;

  ut_list_break (fullname2, ",", &vals, &qty);

  if (qty > 0 && vals[0])
  {
    file = fopen (vals[0], "r");
    if (file == NULL)
      res = 0;
    else
    {
      fclose (file);
      res = 1;
    }
  }
  else
    res = 0;

  ut_free_2d_char (&vals, qty);
  ut_free_1d_char (&fullname);
  ut_free_1d_char (&fullname2);
  va_end (args);

  return res;
}

FILE *
ut_file_open (const char *name, const char *mode)
{
  FILE *file = NULL;
  char *name2 = NULL;

  ut_file_squashname (name, &name2);
  ut_file_openmessage (name2, mode);

  if (mode[0] == 'r' || mode[0] == 'R')
  {
    file = fopen (name2, "r");
    if (file == NULL)
    {
      ut_print_lineheader (2);
      printf ("%s: no such file.\n", name2);
      fflush (stdout);
      abort ();
    }
  }
  else if (mode[0] == 'w' || mode[0] == 'W')
  {
    // if file is to be in a directory and the directory does not exist,
    // create it

    if (strstr (name2, "/"))
    {
      int i;
      char *dir = NULL;

      ut_string_string (name2, &dir);
      for (i = strlen (dir) - 1; i >= 0; i--)
        if (dir[i] == '/')
        {
          dir[i] = '\0';
          break;
        }

      ut_sys_mkdir (dir);
      ut_free_1d_char (&dir);
    }

    file = fopen (name2, "w");
    if (file == NULL)
    {
      ut_print_lineheader (2);
      printf ("%s cannot be opened for writing.\n", name2);
      fflush (stdout);
      abort ();
    }
  }
  else if (mode[0] == 'a' || mode[0] == 'A')
  {
    file = fopen (name2, "a");
    if (file == NULL)
    {
      ut_print_lineheader (2);
      printf ("%s cannot be opened for writing.\n", name2);
      fflush (stdout);
      abort ();
    }
  }
  else
    abort ();

  ut_free_1d_char (&name2);

  return file;
}

void
ut_file_openmessage (const char *name, const char *mode)
{
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsing file `%s'...\n", name);
  }
  else if (mode[0] == 'w')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s'...\n", name);
  }
  else if (mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s'...\n", name);
  }

  return;
}

void
ut_dir_openmessage (const char *name, const char *mode)
{
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsing directory `%s'...\n", name);
  }
  else if (mode[0] == 'w')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing directory `%s'...\n", name);
  }
  else if (mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing directory `%s'...\n", name);
  }

  return;
}

void
ut_file_createmessage (const char *name, const char *mode)
{
  if (mode[0] == 'c')
  {
    ut_print_lineheader (0);
    printf ("    [o] Creating directory `%s'...\n", name);
  }

  return;
}

void
ut_file_testpath (const char *name, const char *mode)
{
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsing file `%s'...\n", name);
  }
  else if (mode[0] == 'w')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s'...\n", name);
  }
  else if (mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s'...\n", name);
  }

  return;
}


void
ut_file_close (FILE * file, const char *name, const char *mode)
{
  ut_file_closemessage (name, mode);

  fclose (file);

  return;
}

void
ut_file_closemessage (const char *name, const char *mode)
{
  char *name2 = NULL;

  ut_file_squashname (name, &name2);
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsed file `%s'.", name2);
  }
  else if (mode[0] == 'w' || mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Wrote file `%s'.", name2);
  }

  ut_free_1d_char (&name2);

  if ((mode[0] == 'r' || mode[0] == 'w' || mode[0] == 'a')
      && !strstr (mode, "nonl"))
    printf ("\n");

  return;
}

void
ut_dir_closemessage (const char *name, const char *mode)
{
  char *name2 = NULL;

  ut_file_squashname (name, &name2);
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsed directory `%s'.", name2);
  }
  else if (mode[0] == 'w' || mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Wrote directory `%s'.", name2);
  }

  ut_free_1d_char (&name2);

  if ((mode[0] == 'r' || mode[0] == 'w' || mode[0] == 'a')
      && !strstr (mode, "nonl"))
    printf ("\n");

  return;
}

int
ut_file_skip (FILE * file, int qty)
{
  int i, status;
  char trash[1000];

  if (qty == 0)
    return 0;
  else if (qty > 0)
    for (i = 0; i < qty; i++)
    {
      if (fscanf (file, "%s", trash) != 1)
        return -1;
    }
  else
    for (i = 0; i > qty; i--)
    {
      do
      {
        fseek (file, -1, SEEK_CUR);
        status = ut_file_testpos (file, "wb");
      }
      while (status != 1);
    }

  return 0;
}

int
ut_file_skip_char (FILE * file, int qty)
{
  int i;
  char trash;

  if (qty == 0)
    return 0;
  else if (qty > 0)
  {
    for (i = 0; i < qty; i++)
      if (fscanf (file, "%c", &trash) != 1)
        return -1;
      else
        abort ();
  }

  return 0;
}

int
ut_file_skip_line (FILE * file, int qty)
{
  int i, c;

  // Read and print characters until a newline is found
  for (i = 0; i < qty; i++)
    while ((c = fgetc(file)) != EOF && c != '\n') {}

  return 0;
}

int
ut_file_cp_qty (FILE * in, int qty, FILE * out, const char *a, const char *b)
{
  int i;
  char s[1000];

  for (i = 0; i < qty; i++)
  {
    if (fscanf (in, "%s", s) != 1)
      return -1;
    fprintf (out, "%s%s", s, a);
  }
  fprintf (out, "%s", b);

  return 0;
}


int
ut_file_readwcomma (FILE * file, double *pres)
{
  char *tmp = ut_alloc_1d_char (1000);

  int status = 0;
  if (fscanf (file, "%s", tmp) == 1)
    (*pres) = ut_num_readwcomma (tmp);
  else
    status = -1;

  ut_free_1d_char (&tmp);

  return status;
}

int
ut_file_testformat (const char *filename, const char *format)
{
  int res;
  char *format2 = NULL;

  ut_file_format (filename, &format2);

  if (strcmp (format, format2) == 0)
    res = 1;
  else
    res = 0;

  ut_free_1d_char (&format2);

  return res;
}

int
ut_file_format (const char *filename, char **pformat)
{
  int res, status;
  FILE *file = ut_file_open (filename, "R");
  char *string = ut_alloc_1d_char (1000);

  status = fscanf (file, "%s", string);

  if (status != 1)
  {
    (*pformat) = NULL;
    res = -1;
  }
  else if (strncmp (string, "***tess", 7) == 0)
  {
    (*pformat) = ut_alloc_1d_char (5);
    sprintf ((*pformat), "tess");
    res = 0;
  }
  else if (strcmp (string, "***tesr") == 0)
  {
    (*pformat) = ut_alloc_1d_char (5);
    sprintf ((*pformat), "tesr");
    res = 0;
  }
  else if (strcmp (string, "$MeshFormat") == 0)
  {
    (*pformat) = ut_alloc_1d_char (9);
    sprintf ((*pformat), "gmsh:msh");
    res = 0;
  }
  else if (strcmp (string, "***geometry") == 0)
  {
    (*pformat) = ut_alloc_1d_char (12);
    sprintf ((*pformat), "zset:geof");
    res = 0;
  }
  else if (strstr (filename, ".obj"))
  {
    (*pformat) = ut_alloc_1d_char (12);
    sprintf ((*pformat), "obj");
    res = 0;
  }
  else if (strcmp (string, "OVM") == 0)
  {
    (*pformat) = ut_alloc_1d_char (12);
    sprintf ((*pformat), "ovm");
    res = 0;
  }
  else
  {
    (*pformat) = ut_alloc_1d_char (1);
    (*pformat)[0] = '\0';
    res = -1;
  }

  ut_file_close (file, filename, "R");

  ut_free_1d_char (&string);

  return res;
}

// does not take the blank characters into account (because fscanf skip
// them)
int
ut_file_string_goto (FILE * file, const char *string)
{
  int status;
  unsigned int j;
  char c;
  fpos_t pos;
  int match;

  do
  {
    fgetpos (file, &pos);
    match = 1;
    for (j = 0; j < strlen (string); j++)
    {
      status = fscanf (file, "%c", &c);
      if (status != 1 || c != string[j])
      {
        match = 0;
        break;
      }
    }
    fsetpos (file, &pos);
  }
  while (match == 0 && fscanf (file, "%c", &c) != EOF);

  return match ? 0 : -1;
}

int
ut_file_goto_nextline (FILE * file)
{
  int status;
  char c = 'a';

  do
    status = fscanf (file, "%c", &c);
  while (status == 1 &&  (c != '\n' && c != EOF));

  return c == '\n' ? 0 : -1;
}

int
ut_file_string_scanandtest (FILE * file, const char *string)
{
  int status;
  char *scan = ut_alloc_1d_char (1000);

  status = fscanf (file, "%s", scan);

  if (status == 1)
    status = strcmp (string, scan);
  else
    status = -1;

  ut_free_1d_char (&scan);

  return !status ? 1 : 0;
}


int
ut_file_string_nextpos (FILE * file, const char *string)
{
  int p, ok;
  char *tmp = ut_alloc_1d_char (1000);
  fpos_t pos;

  fgetpos (file, &pos);

  ok = 0;
  p = 0;
  while (fscanf (file, "%s", tmp) != EOF)
  {
    p++;
    if (!strcmp (tmp, string))
    {
      p--;
      ok = 1;
      break;
    }
  }
  fprintf (file, "\n");

  fsetpos (file, &pos);

  ut_free_1d_char (&tmp);

  return ok ? p : -1;
}

int
ut_file_string_number (FILE * file, const char *string)
{
  int nb;
  char *tmp = ut_alloc_1d_char (1000);
  fpos_t pos;

  fgetpos (file, &pos);

  nb = 0;
  while (fscanf (file, "%s", tmp) != EOF)
    if (strcmp (tmp, string) == 0)
      nb++;

  fsetpos (file, &pos);

  ut_free_1d_char (&tmp);

  return nb;
}

int
ut_file_testpos (FILE * file, const char *p)
{
  int fsstatus, status, res, qty;
  char c, c2;
  fpos_t pos;

  fgetpos (file, &pos);

  res = 0;
  if (strcmp (p, "le") == 0)
  {
    do
      status = fscanf (file, "%c", &c);
    while (c == ' ' || c == '\t');

    if (status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "lE") == 0)
  {
    status = fscanf (file, "%c", &c);

    if (status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "lB") == 0)
  {
    fsstatus = fseek (file, -1, SEEK_CUR);
    status = fscanf (file, "%c", &c);

    if (fsstatus == -1 || status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "lb") == 0)
  {
    qty = 0;
    do
    {
      fsstatus = fseek (file, --qty, SEEK_CUR);
      status = fscanf (file, "%c", &c);
    }
    while (c == ' ' || c == '\t');

    if (fsstatus == -1 || status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "wb") == 0)
  {
    if (ftell (file) == 0)
      c = ' ';
    else
    {
      fseek (file, -1, SEEK_CUR);
      status = fscanf (file, "%c", &c);
    }
    status = fscanf (file, "%c", &c2);

    if (status == 1 && (c == ' ' || c == '\t' || c == '\n')
        && !(c2 == ' ' || c2 == '\t' || c2 == '\n'))
      res = 1;
  }
  else if (strcmp (p, "we") == 0)
  {
    status = fscanf (file, "%c%c", &c, &c2);

    if (status == 2 && !(c == ' ' || c == '\t' || c == '\n')
        && (c2 == ' ' || c2 == '\t' || c2 == '\n'))
      res = 1;
  }
  else if (strcmp (p, "fe") == 0)
  {
    status = fscanf (file, "%c", &c);

    if (status != 1)
      res = 1;
  }
  else
    abort ();

  fsetpos (file, &pos);

  return res;
}

int
ut_file_nextstring (FILE * file, char *string)
{
  fpos_t pos;

  fgetpos (file, &pos);
  if (string)
  {
    if (fscanf (file, "%s", string) != 1)
      return -1;
    else
    {
      fsetpos (file, &pos);
      return 1;
    }
  }
  else
  {
    int status;
    char *tmp = ut_alloc_1d_char (1000);

    if (fscanf (file, "%s", tmp) != 1)
      status = -1;
    else
    {
      fsetpos (file, &pos);
      status = 1;
    }

    ut_free_1d_char (&tmp);

    return status;
  }
}

int
ut_file_nextstring_test (FILE * file, char *string)
{
  int status;
  char *string0 = ut_alloc_1d_char (10000);

  ut_file_nextstring (file, string0);
  status = !strcmp (string0, string);

  ut_free_1d_char (&string0);

  return status;
}

void
ut_file_nextline (FILE * file, char **pline)
{
  int length = 10000;
  int multiplier = 2;
  int beg = 0, end = length - 2;

  fpos_t pos;

  fgetpos (file, &pos);

  (*pline) = ut_alloc_1d_char (length);
  (*pline) = fgets (*pline, length, file);

  if (*pline == NULL)
    abort ();

  while (1)
  {
    (*pline) = ut_realloc_1d_char ((*pline), length);

    //((*pline) + beg) =
    if (fgets ((*pline) + beg, length - beg, file) == NULL)
    break;

    if ((*pline)[end] == '\0' || (*pline)[end] == '\n'
    || (*pline)[end] == EOF)
    break;

    beg = end + 1;
    length *= multiplier;
    end = length - 2;
  }

  (*pline) = ut_realloc_1d_char ((*pline), strlen (*pline) + 1);

  fsetpos (file, &pos);

  return;
}

void
ut_file_nextlinenbwords (FILE * file, int *pqty)
{
  char *line = NULL;

  ut_file_nextline (file, &line);

  (*pqty) = ut_string_nbwords (line);

  ut_free_1d_char (&line);

  return;
}

void
ut_file_cp (const char *frompath, const char *topath)
{
  int size;
  char buffer[BUFSIZ];

  FILE *source = NULL;
  FILE *dest = NULL;

  source = ut_file_open (frompath, "R");
  dest = ut_file_open (topath, "W");

  while ((size = fread (buffer, 1, BUFSIZ, source)))
    fwrite (buffer, 1, size, dest);

  ut_file_close (source, frompath, "R");
  ut_file_close (dest, topath, "W");

  return;
}

int
ut_file_cp_line (FILE * src, FILE * dest)
{
  return ut_file_cp_untilchar (src, dest, '\n');
}

int
ut_file_cp_untilchar (FILE * src, FILE * dest, char c)
{
  char d = (c == 'a') ? 'b' : 'a';

  while (d != c && d != EOF)
  {
    d = fgetc (src);
    putc (d, dest);
  }

  return (d == c) ? 0 : -1;
}

void
ut_file_cp_lines (FILE * src, FILE * dest, int *lines, int lineqty)
{
  int i, line;

  // checking that the lines are ordered
  for (i = 0; i < lineqty - 1; i++)
    if (lines[i] > lines[i + 1])
      abort ();

  line = 1;
  for (i = 0; i < lineqty; i++)
  {
    ut_file_skip_line (src, lines[i] - line);
    if (ut_file_cp_line (src, dest) != 0)
      abort ();
    line = lines[i] + 1;
  }

  return;
}

void
ut_file_squashname (const char *name, char **pname)
{
  (*pname) = ut_alloc_1d_char (strlen (name) + 1);

  if (!strncmp (name, "file(", 5))
  {
    ut_string_string (name + 5, pname);
    (*pname)[strlen ((*pname)) - 1] = '\0';
  }
  else if (!strncmp (name, "msfile(", 7))
  {
    ut_string_string (name + 7, pname);
    (*pname)[strlen ((*pname)) - 1] = '\0';
  }
  else if (sscanf (name, "@%s", *pname) == 1)
  {
  }
  else if (sscanf (name, "%s", *pname) == 1)
  {
  }

  int qty;
  char **tmp = NULL;

  ut_list_break (*pname, ",", &tmp, &qty);

  ut_string_string (tmp[0], pname);

  ut_free_2d_char (&tmp, qty);

  return;
}

int
ut_file_lineheader_goto (FILE * file, const char *header)
{
  int status;
  char *tmp = ut_alloc_1d_char (10000);
  char *lineflag = ut_alloc_1d_char (10000);
  fpos_t pos;

  do
  {
    status = -1;
    fgetpos (file, &pos);
    if (fgets (tmp, 10000, file))
      status = sscanf (tmp, "%s", lineflag);
  }
  while (status == 1 && strcmp (lineflag, header));

  if (status == 1 && strcmp (lineflag, header))
  {
    fsetpos (file, &pos);
    status = 0;
  }
  else
    status = -1;

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&lineflag);

  return status;
}

int
ut_file_nextstring_sectionlevel (FILE * file, int *plevel)
{
  int val, status;
  char *tmp = ut_alloc_1d_char (1000);

  status = ut_file_nextstring (file, tmp);

  if (status == 1)
  {
    ut_string_section_level (tmp, &val);

    if (plevel)
      (*plevel) = val;

    status = 0;
  }

  ut_free_1d_char (&tmp);

  return status;
}

int
ut_file_isendoffile (FILE * file)
{
  int status;
  char c;
  fpos_t pos;

  fgetpos (file, &pos);

  status = 1;
  while (fscanf (file, "%c", &c) != EOF)
    if (c == ' ' || c == '\n' || c == '\t')
      continue;
    else
    {
      status = 0;
      break;
    }

  fsetpos (file, &pos);

  return status;
}

void
ut_file_scan_file (FILE *file, char *dirname, FILE **pfile, char **pfilename)
{
  char c;
  fpos_t pos;
  char *filename = NULL;
  char *tmp = ut_alloc_1d_char (1000);
  char *tmp2 = ut_alloc_1d_char (1000);

  if (pfilename)
    ut_free_1d_char (pfilename);

  do
  {
    fgetpos (file, &pos);
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  }
  while (c == ' ' || c == '\n' || c == '\t');

  fsetpos (file, &pos);

  if (c == '*')
    if (fscanf (file, "%s", tmp) != 1)
      abort ();

  if (!strcmp (tmp, "*file"))
  {
    filename = ut_alloc_1d_char (1000);
    if (fscanf (file, "%s", tmp2) != 1)
      abort ();
    sprintf (filename, "%s/%s", dirname, tmp2);
    *pfile = ut_file_open (filename, "r");
  }
  else
    *pfile = file;

  if (pfilename)
    ut_string_string (filename, pfilename);

  ut_free_1d_char (&filename);
  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);

  return;
}

void
ut_file_dir_basename_extension_filename (char *dir, char *basename,
                                         char *extension, char **pfilename)
{
  char *tmp = NULL;
  char *filename = ut_alloc_1d_char (strlen (dir) + strlen (basename)
                                     + strlen (extension) + 10);

  tmp = ut_string_addextension (basename, extension);
  filename = ut_string_paste3 (dir, "/", tmp);

  while (!strncmp (filename, "./", 2))
    ut_string_fnrs (filename, "./", "", 1);

  ut_string_fnrs (filename, "//", "/", INT_MAX);

  ut_string_string (filename, pfilename);

  ut_free_1d_char (&filename);
  ut_free_1d_char (&tmp);

  return;
}

void
ut_file_removepath (char *filename1, char **pfilename2)
{
  int qty;
  char **tmp = NULL;

  ut_list_break (filename1, "/", &tmp, &qty);

  ut_string_string (tmp[qty - 1], pfilename2);

  ut_free_2d_char (&tmp, qty);

  return;
}
