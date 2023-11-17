/* Copyright (C) 2003-2022, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>
#include<limits.h>
#include"ut.h"

int
ut_string_fnr (char *string, char find, char replace)
{
  int i, length, res;

  res = 0;
  length = strlen (string);
  for (i = 0; i <= length - 1; i++)
    if (string[i] == find)
    {
      string[i] = replace;
      res = 1;
    }

  return res;
}

int
ut_string_fnrs (char *string, const char *find, const char *replace,
                int repeat)
{
  int i, j, p, equal, test = 0;
  int *pos = ut_alloc_1d_int (strlen (string) + 1);
  int length = strlen (string);
  int lengthf = strlen (find);
  int lengthr;
  char *replace2 = NULL;

  if (replace)
  {
    replace2 = ut_alloc_1d_char (strlen (replace) + 1);
    lengthr = strlen (replace);
    strcpy (replace2, replace);
  }
  else
  {
    lengthr = 1;
    replace2 = ut_alloc_1d_char (2);
    replace2[0] = '\0';
  }

  char *string2 = ut_alloc_1d_char (ceil (length * lengthr / lengthf) + 1000);

  while (repeat-- > 0)
  {
    length = strlen (string);

    pos[0] = 0;

    /* looking for the pos of 'find' in 'string' */
    for (i = 0; i < length - lengthf + 1; i++)
    {
      equal = 1;
      for (j = 0; j < lengthf; j++)
      {
        if (string[i + j] != find[j])
        {
          equal = 0;
          break;
        }
      }

      if (equal == 1)
      {
        test = 1;
        pos[++pos[0]] = i;
        i += lengthf - 1;
      }
    }

    if (pos[0] == 0)
      break;
    else
    {
      p = 0;
      for (i = 0; i < length; i++)
      {
        if (ut_array_1d_int_find (pos + 1, pos[0], i) == -1)
          string2[p++] = string[i];
        else
        {
          for (j = 0; j < lengthr; j++)
            string2[p++] = replace2[j];

          i += lengthf - 1;
        }
      }
      string2[p] = '\0';
    }
    sprintf (string, "%s", string2);
  }

  ut_free_1d_int (&pos);
  ut_free_1d_char (&string2);
  ut_free_1d_char (&replace2);

  return test;
}

int
ut_string_completion (char *in, char **list, int qty, char **pout)
{
  int i, j, res = 0;
  char ***list2 = ut_alloc_1d_ppchar (qty + 1);
  int *qty2 = ut_alloc_1d_int (qty + 1);

  ut_free_1d_char (pout);

  for (i = 1; i <= qty; i++)
    ut_list_break (list[i], ",", list2 + i, qty2 + i);

  // Checking if it matches exactly
  for (i = 1; i <= qty; i++)
  {
    for (j = 0; j < qty2[i]; j++)
      if (!strcmp (in, list2[i][j]))
      {
        res = i;
        break;
      }

    if (res != 0)
      break;
  }

  if (res == 0)                 // not found earlier
    for (i = 1; i <= qty; i++)
    {
      for (j = 0; j < qty2[i]; j++)
        if (!strncmp (in, list2[i][j], strlen (in)))    // found
        {
          if (res == 0)         // not found earlier
          {
            res = i;
            break;
          }
          else                  // found multiple times
            res = -2;
        }

      if (res == -2)
        break;
    }

  if (res > 0)
    ut_string_string (list2[res][0], pout);     // this is the good possibility
  else
    ut_string_string (in, pout); // copying input

  for (i = 1; i <= qty; i++)
    ut_free_2d_char (&list2[i], qty2[i]);
  free (list2);
  ut_free_1d_int (&qty2);

  return res > 0 ? 0 : res == -2 ? 1 : -1;
}

char *
ut_string_addextension (const char *in, const char *ext, ...)
{
  char *out = NULL;

  unsigned int NoCin;           /* nb of characters of in */
  unsigned int NoCext;          /* nb of characters of extin */
  int skip = 0;

  va_list args;
  char *fullext = ut_alloc_1d_char (1000);

  va_start (args, ext);
  vsprintf (fullext, ext, args);
  va_end (args);

  NoCin = strlen (in);
  NoCext = strlen (fullext);

  if (fullext[0] == '.')
  {
    NoCext--;
    skip++;
  }

  out = ut_alloc_1d_char (NoCin + NoCext + 2);

  sprintf (out, "%s.%s", in, fullext + skip);

  ut_free_1d_char (&fullext);

  return out;
}

/* RAM */
void
ut_string_body (char *in, char *stops, char **pbody)
{
  int i, pos;
  char *in2 = NULL;
  ut_string_string (in, &in2);

  pos = strlen (in2);
  for (i = strlen (in2) - strlen (stops); i >= 0; i--)
    if (!strncmp (in2 + i, stops, strlen (stops)))
    {
      pos = i;
      break;
    }

  (*pbody) = ut_realloc_1d_char (*pbody, pos + 1);
  strncpy (*pbody, in2, pos);
  (*pbody)[pos] = '\0';

  ut_free_1d_char (&in2);

  return;
}

int
ut_string_untilchar (char *string, char c, int *ppos, char *res)
{
  char *copy;
  int pos;

  pos = ppos ? (*ppos) : 0;

  if ((unsigned int) pos < strlen (string))
  {
    copy = ut_alloc_1d_char (strlen (string) + 1);

    strcpy (copy, string + pos);

    ut_string_fnr (copy, c, '\0');

    strcpy (res, copy);

    pos += strlen (copy);

    ut_free_1d_char (&copy);
  }
  else
    res[0] = '\0';

  if (ppos)
    (*ppos) = pos;

  if (strlen (res) > 0)
    return 1;
  else
    return 0;
}

int
ut_string_untilstring (const char *string, const char *s, int *ppos,
                       char *res)
{
  int pos, status;
  char *copy = NULL;

  pos = ppos ? (*ppos) : 0;

  if ((unsigned int) pos < strlen (string))
  {
    status = 1;

    copy = ut_alloc_1d_char (strlen (string) + 1);

    strcpy (copy, string + pos);

    ut_string_fnrs (copy, s, NULL, 1);

    strcpy (res, copy);

    pos += strlen (copy);
  }
  else
  {
    status = 0;
    res[0] = '\0';
  }

  if (ppos)
    (*ppos) = pos;

  ut_free_1d_char (&copy);

  return status;
}

int
ut_string_thischarqty (char *string, char c)
{
  int i, res = 0;

  for (i = 0; i < (int) strlen (string); i++)
    if (string[i] == c)
      res++;

  return res;
}

int
ut_string_thischarpos (char *string, char c, int nb)
{
  int pos, tmp, res;

  /* by default, i.e. if nb == 0 or occurrence not found */
  res = -1;

  if (nb != 0)
  {
    tmp = 0;
    for (pos = 0; pos < (int) strlen (string); pos++)
      if (string[pos] == c)
      {
        tmp++;
        if (tmp == nb)          /* the good occurence is found */
        {
          res = pos;
          break;
        }
      }
  }

  return res;
}

void
ut_string_function (char *string, char **pfct, char ***pvars, char ***pvals,
                    int *pqty)
{
  int i, pos = 0, qty;
  char *string2 = NULL;
  char ***parts = NULL;
  int *qty1 = NULL;
  char *string3 = NULL;
  char *tmp = NULL;

  if (!string)
  {
    if (pqty)
      *pqty = 0;
    return;
  }

  if (!strstr (string, "("))
  {
    if (pfct)
      ut_string_string (string, pfct);
    if (pqty)
      *pqty = 0;
    return;
  }

  string3 = ut_alloc_1d_char (strlen (string) + 1);
  tmp = ut_alloc_1d_char (strlen (string) + 1);

  ut_string_untilchar (string, '(', &pos, tmp);

  if (pfct)
    ut_string_string (tmp, pfct);

  ut_string_string (string + pos + 1, &string2);
  string2[strlen (string2) - 1] = '\0';

  ut_list_break2 (string2, ",", "=", &parts, &qty1, &qty);

  if (pqty)
    (*pqty) = qty;
  if (pvars)
    (*pvars) = ut_alloc_1d_pchar (qty);
  if (pvals)
    (*pvals) = ut_alloc_1d_pchar (qty);

  for (i = 0; i < qty; i++)
  {
    if (qty1[i] == 2)
    {
      if (pvars)
        ut_string_string (parts[i][0], (*pvars) + i);
      if (pvals)
        ut_string_string (parts[i][1], (*pvals) + i);
    }
    else if (qty1[i] == 1)
    {
      if (pvals)
        ut_string_string (parts[i][0], (*pvals) + i);
    }
    else
      abort ();
  }

  ut_free_1d_char (&string2);
  ut_free_1d_char (&string3);
  for (i = qty - 1; i >= 0; i--)
    ut_free_2d_char (&parts[i], qty1[i]);
  free (parts);
  ut_free_1d_int (&qty1);
  ut_free_1d_char (&tmp);

  return;
}

int
ut_string_functionargument (char *string, char **pargument)
{
  int qty, status;
  char *fct = NULL, **vals = NULL, **vars = NULL;

  ut_string_function (string, &fct, &vars, &vals, &qty);

  if (qty == 1)
  {
    ut_string_string (vals[0], pargument);
    status = 0;
  }
  else
  {
    ut_free_1d_char (pargument);
    status = -1;
  }

  ut_free_1d_char (&fct);
  ut_free_2d_char (&vals, qty);
  ut_free_2d_char (&vars, qty);

  return status;
}

void
ut_string_function_expr (char *string, char **pfct, char ***pexprs,
                         int *pexprqty)
{
  int pos = 0;
  char *tmp = NULL, *string2 = NULL;

  if (!string)
  {
    if (pexprqty)
      (*pexprqty) = 0;
    return;
  }

  tmp = ut_alloc_1d_char (strlen (string) + 1);

  ut_string_untilchar (string, '(', &pos, tmp);

  if (pfct)
    ut_string_string (tmp, pfct);

  ut_string_string (string + pos + 1, &string2);
  string2[strlen (string2) - 1] = '\0';

  ut_list_break (string2, ",", pexprs, pexprqty);

  ut_free_1d_char (&string2);
  ut_free_1d_char (&tmp);

  return;
}

int
ut_string_nbwords (char *string)
{
  unsigned int i;
  int bef, nbw;

  nbw = 0;
  bef = 0;
  for (i = 0; i < strlen (string); i++)
  {
    if (string[i] != ' ' && string[i] != '\t' && string[i] != '\n')
    {
      if (bef == 0)
      {
        nbw++;
        bef = 1;
      }
    }
    else
      bef = 0;
  }

  return nbw;
}

int
ut_string_substrings (char *string, char ***psubstrings, int *psubstringqty)
{
  int i, shift;
  char *tmp = NULL;

  (*psubstringqty) = 0;

  if (!string)
    return 0;

  if (strstr (string, "#"))
    abort ();

  shift = 0;
  for (i = 0; i < (int) strlen (string); i++)
    if (string[i] == ' ' || string[i] == '\t' || string[i] == '\n')
      shift++;
    else
      break;

  ut_string_string (string + shift, &tmp);
  ut_string_fnr (tmp, ' ', '#');
  ut_string_fnr (tmp, '\t', '#');
  ut_string_fnr (tmp, '\n', '#');
  ut_string_fnrs (tmp, "##", "#", INT_MAX);

  ut_list_break (tmp, "#", psubstrings, psubstringqty);

  ut_free_1d_char (&tmp);

  return 0;
}

char *
ut_string_paste (char *string1, char *string2)
{
  char *res;

  res = ut_alloc_1d_char (strlen (string1) + strlen (string2) + 1);

  sprintf (res, "%s%s", string1, string2);

  return res;
}

char *
ut_string_paste3 (char *string1, char *string2, char *string3)
{
  char *res;

  res = ut_alloc_1d_char (strlen (string1) + strlen (string2) + strlen (string3) + 1);

  sprintf (res, "%s%s%s", string1, string2, string3);

  return res;
}

char *
ut_string_paste4 (char *string1, char *string2, char *string3, char *string4)
{
  char *res;

  res = ut_alloc_1d_char (strlen (string1) + strlen (string2)
                        + strlen (string3) + strlen (string4) + 1);

  sprintf (res, "%s%s%s%s", string1, string2, string3, string4);

  return res;
}

char *
ut_string_array_paste (char **string, int qty, char *sep)
{
  int i;
  char *res;

  int length = 1;
  for (i = 0; i < qty; i++)
    length += strlen (string[i]);

  int seplength = sep ? (qty - 1) * strlen (sep) : 0;

  res = ut_alloc_1d_char (length + seplength + 1);
  for (i = 0; i < qty; i++)
  {
    res = strcat (res, string[i]);
    if (i < qty - 1 && sep && strlen (sep) > 0)
      res = strcat (res, sep);
  }

  return res;
}

int
ut_string_format (const char *string, char *format)
{
  int i;
  int length = strlen (string);
  int dotqty, dotpos, nbqty, isstring;

  nbqty = 0;
  dotqty = 0;
  dotpos = 0;
  isstring = 0;
  for (i = 0; i < length; i++)
  {
    if ((i == 0 && string[i] == '-')
        || (string[i] >= '0' && string[i] <= '9'))
      nbqty++;
    else if (string[i] == '.')
    {
      dotqty++;
      dotpos = i;
    }
    else
    {
      isstring = 1;
      break;
    }
  }

  if (dotqty > 1)
    isstring = 1;

  // string
  if (isstring == 1)
    sprintf (format, "%%s");
  // int
  else if (dotqty == 0)
  {
    if (string[0] != '0')
      sprintf (format, "%%d");
    else
      sprintf (format, "%%0%dd", length);
  }
  else if (dotqty == 1)
  {
    sprintf (format, "%%0%d.%df", length, length - dotpos - 1);
  }
  else
    abort ();

  return 0;
}

int
ut_string_format_test (char *in, char *format)
{
  int status;
  char *format2 = ut_alloc_1d_char (10);

  ut_string_format (in, format2);

  status = !strcmp (format, format2);

  ut_free_1d_char (&format2);

  return status;
}

int
ut_string_isdigits (char *string)
{
  int status = 1;
  int i;

  for (i = 0; status && (i < (int) strlen (string)); i++)
    status = isdigit (string[i]);

  return status;
}

void
ut_string_removetrailingdigits (char *string, char **pstring2)
{
  int i, qty = 0, length;

  ut_string_string (string, pstring2);
  for (i = strlen (string) - 1; i >= 0; i--)
  {
    if (isdigit (string[i]))
      qty++;
    else
      break;
  }

  length = strlen (string) - qty;
  *pstring2 = ut_alloc_1d_char (strlen (string) - qty + 1);
  *pstring2 = strncpy (*pstring2, string, length);

  return;
}

void
ut_string_trailingdigits (char *string, char **pstring2)
{
  int i, qty = 0;

  ut_string_string (string, pstring2);
  for (i = strlen (string) - 1; i >= 0; i--)
  {
    if (isdigit (string[i]))
      qty++;
    else
      break;
  }

  *pstring2 = ut_alloc_1d_char (qty + 1);
  *pstring2 = strcpy (*pstring2, string + strlen (string) - qty);

  return;
}

void
ut_string_real (char *string, double *pres)
{
  char *format = ut_alloc_1d_char (10);

  ut_string_format (string, format);

  if (format[strlen (format) - 1] == 'd'
      || format[strlen (format) - 1] == 'f')
    sscanf (string, "%lf", pres);

  else if (format[strlen (format) - 1] == 's')
    ut_math_eval (string, 0, NULL, NULL, pres);
  else
    abort ();

  ut_free_1d_char (&format);

  return;
}

void
ut_string_int (char *string, int *pres)
{
  double val;

  ut_string_real (string, &val);
  (*pres) = ut_num_d2ri (val);

  return;
}

void
ut_string_string (const char *input, char **poutput)
{
  if (input)
  {
    char *input2 = ut_alloc_1d_char (strlen (input) + 1);
    input2 = strcpy (input2, input);

    (*poutput) = ut_realloc_1d_char (*poutput, strlen (input2) + 1);
    strcpy (*poutput, input2);

    ut_free_1d_char (&input2);
  }
  else
    ut_free_1d_char (poutput);

  return;
}

int
ut_string_strcmp (const char *string1, const char *string2)
{
  if (!string1 && !string2)
    return 0;
  else if (!string1 || !string2)
    return -1;
  else
    return strcmp (string1, string2);
}

int
ut_string_strncmp (const char *string1, const char *string2, int length)
{
  if (!string1 && !string2)
    return 0;
  else if (!string1 || !string2)
    return -1;
  else
    return strncmp (string1, string2, length);
}

int
ut_string_isfilename (char *string)
{
  return ((string && strlen (string) > 0)
          && (!strncmp (string, "file(", 5) || !strncmp (string, "msfile(", 7)
              || string[0] == '@'));
}

int
ut_string_section_level (char *string, int *plevel)
{
  unsigned int i;

  (*plevel) = 0;

  for (i = 0; i < strlen (string); i++)
  {
    if (string[i] == '*')
      (*plevel)++;
    else
      break;
  }

  return 0;
}

int
ut_string_isnumber (char *s)
{
  if (s == NULL || *s == '\0' || isspace (*s))
    return 0;

  char *p;
  strtod (s, &p);
  return *p == '\0';
}

void
ut_string_random (int length, int rand, char **pstring)
{
  int i, key;
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

  srand48 (rand);

  (*pstring) = ut_alloc_1d_char (length + 1);

  if (length)
  {
    --length;
    for (i = 0; i < length; i++)
    {
      key = (int) (drand48 () * (sizeof charset - 1));
      (*pstring)[i] = charset[key];
    }
    (*pstring)[length] = '\0';
  }

  return;
}
