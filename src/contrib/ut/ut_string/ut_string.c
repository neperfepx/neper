/* Copyright (C) 2003-2019, Romain Quey */
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
ut_string_find (const char *string, char find)
{
  int i, length, res;

  res = -1;
  length = strlen (string);
  for (i = 0; i <= length - 1; i++)
    if (string[i] == find)
    {
      res = i;
      break;
    }

  return res;
}

int
ut_string_finds (const char *string, const char *find)
{
  int i, j, equal, res;
  int length = strlen (string);
  int lengthf = strlen (find);

  res = -1;
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
      res = i;
      break;
    }
  }

  return res;
}

int
ut_string_fnrs (char *string, const char *find, const char *replace, int repeat)
{
  int i, j, p, equal, test = 0;
  int *pos = ut_alloc_1d_int (strlen (string) + 1);
  int length = strlen (string);
  int lengthf = strlen (find);
  int lengthr;
  char* replace2 = NULL;

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

  ut_free_1d_int (pos);
  ut_free_1d_char (string2);
  ut_free_1d_char (replace2);

  return test;
}

char *
ut_string_array_paste_separator(char **string, int qty, char separator)
{
  int i;
  char *res = NULL;
  char *sep = ut_alloc_1d_char(2);

  sprintf(sep, "%c", separator);

  int length = 1;
  for (i = 0; i < qty; i++)
    length += strlen (string[i]) + 1;

  res = ut_alloc_1d_char (length);
  for (i = 0; i < qty - 1; i++)
  {
    res = strcat (res, string[i]);
    res = strcat (res, sep);
  }
  res = strcat (res, string[i]);

  ut_free_1d_char(sep);
  return res;
}

int
ut_string_comp (char *in, char **list, int qty, char **pout)
{
  int i, j;
  int res = 0;
  char ***list2 = ut_alloc_1d_ppchar (qty + 1);
  int *qty2 = ut_alloc_1d_int (qty + 1);

  ut_free_1d_char (*pout);
  (*pout) = NULL;

  for (i = 1; i <= qty; i++)
    ut_string_separate (list[i], ",", &(list2[i]), &(qty2[i]));

  /* Checking if it matches exactly */
  for (i = 1; i <= qty; i++)	/* for every list parameter */
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

  if (res == 0)
    for (i = 1; i <= qty; i++)	/* for every list parameter */
    {
      for (j = 0; j < qty2[i]; j++)
	if (strncmp (in, list2[i][j], strlen (in)) == 0)
	{
	  if (res == 0)
	  {
	    res = i;
	    break;
	  }
	  else
	    res = -2;
	}

      if (res == -2)
	break;
    }

  if (res > 0)
  {
    (*pout) = ut_alloc_1d_char (strlen (list2[res][0]) + 1);
    strcpy (*pout, list2[res][0]);	/* this is the good possibility. */
  }

  for (i = 1; i <= qty; i++)
    ut_free_2d_char (list2[i], qty2[i]);
  free (list2);
  ut_free_1d_int (qty2);

  if (res > 0)
    return 0;
  else
    return (res == -2) ? 1 : -1;
}

char *
ut_string_extension (char *in)
{
  char *ext;

  int i;
  int NoCin, NoCext;
  int DashPos = -1;

  NoCin = strlen (in);

  for (i = NoCin - 1; i >= 0; i--)
  {
    if (in[i] == '.')
    {
      DashPos = i;
      break;
    }
  }

  if (DashPos == -1)
    ext = NULL;
  else
  {
    NoCext = NoCin - DashPos;

    ext = ut_alloc_1d_char (NoCext + 1);

    for (i = 0; i <= NoCext - 1; i++)
      ext[i] = in[i + DashPos];
    ext[NoCext] = '\0';
  }

  return ext;
}

char *
ut_string_addextension (const char *in, const char *ext, ...)
{
  char *out = NULL;

  unsigned int NoCin;		/* nb of characters of in */
  unsigned int NoCext;		/* nb of characters of extin */
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

  /* out = strncpy (out, in, NoCin);
     out = strcat (out, fullext + 1); */
  sprintf (out, "%s.%s", in, fullext + skip);
  /* out[NoCin + NoCext + skip] = '\0'; */

  ut_free_1d_char (fullext);

  return out;
}

/* RAM */
void
ut_string_body (char *in, char* stops, char **pbody)
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

  ut_free_1d_char (in2);

  return;
}

char *
ut_string_changeextension (char *string, char *newext)
{
  char *body = NULL;
  char *res = NULL;

  ut_string_body (string, ".", &body);
  res = ut_string_addextension (body, newext);

  ut_free_1d_char (body);

  return res;
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

    ut_free_1d_char (copy);
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
ut_string_untilstring (const char *string, const char* s, int *ppos, char *res)
{
  int pos, status;
  char* copy = NULL;

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

  ut_free_1d_char (copy);

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
	if (tmp == nb)		/* the good occurence is found */
	{
	  res = pos;
	  break;
	}
      }
  }

  return res;
}

int
ut_string_stringpos (char *string, char *string2, int nb)
{
  int pos, tmp, res;
  int l1, l2, i, status;

  l1 = (int) strlen (string);
  l2 = (int) strlen (string2);

  /* by default, i.e. if nb == 0 or occurrence not found */
  res = -1;

  if (nb != 0)
  {
    tmp = 0;
    for (pos = 0; pos < l1 - l2 + 1; pos++)
    {
      status = 1;
      for (i = 0; i < l2; i++)
	if (string[pos + i] != string2[i])
	{
	  status = 0;
	  break;
	}
      if (status == 1)
	tmp++;
      if (tmp == nb)		/* the good occurence is found */
      {
	res = pos;
	break;
      }
    }
  }

  return res;
}

void
ut_string_separate (const char *string, const char* c, char ***parts, int *pqty)
{
  int i, pos, bracket;
  char **tmp = NULL;
  char *val = NULL;

  if (!string)
  {
    (*pqty) = 0;
    return;
  }

  tmp = ut_alloc_2d_char (strlen (string) + 1, strlen (string) + 1);
  val = ut_alloc_1d_char (strlen (string) + 1);

  (*pqty) = 0;
  pos = 0;
  bracket = 0;
  while (ut_string_untilstring (string, c, &pos, val))
  {
    bracket += ut_string_thischarqty (val, '(')
             - ut_string_thischarqty (val, ')');
    if (bracket >= 0)
    {
      tmp[*pqty] = strcat (tmp[*pqty], val);
      if (bracket > 0)
	tmp[*pqty] = strcat (tmp[*pqty], c);
      else
	(*pqty)++;
      pos += strlen (c);
    }
    else
      abort ();
  }

  if (parts)
  {
    (*parts) = ut_alloc_1d_pchar (*pqty);
    for (i = 0; i < (*pqty); i++)
    {
      (*parts)[i] = ut_alloc_1d_char (strlen (tmp[i]) + 1);
      strcpy ((*parts)[i], tmp[i]);
    }
  }

  ut_free_2d_char (tmp, strlen (string) + 1);
  ut_free_1d_char (val);

  return;
}

void
ut_string_function_separate (char *string, char** pfct,
                             char ***pvars, char ***pvals, int *pqty)
{
  int i, pos = 0, qty;
  char* string2 = NULL;
  char*** parts = NULL;
  int *qty1 = NULL;
  char* string3 = NULL;
  char* tmp = NULL;

  if (!string)
  {
    if (pqty)
      (*pqty) = 0;
    return;
  }

  string3 = ut_alloc_1d_char (strlen (string) + 1);
  tmp = ut_alloc_1d_char (strlen (string) + 1);

  ut_string_untilchar (string, '(', &pos, tmp);

  if (pfct)
    ut_string_string (tmp, pfct);

  ut_string_string (string + pos + 1, &string2);
  string2[strlen (string2) - 1] = '\0';

  ut_string_separate2 (string2, ",", "=", &parts, &qty1, &qty);

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

  ut_free_1d_char (string2);
  ut_free_1d_char (string3);
  for (i = qty - 1; i >= 0; i--)
    ut_free_2d_char (parts[i], qty1[i]);
  free (parts);
  ut_free_1d_int (qty1);
  ut_free_1d_char (tmp);

  return;
}

void
ut_string_function_separate_exprs (char *string, char** pfct,
                                   char ***pexprs, int *pexprqty)
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

  ut_string_separate (string2, ",", pexprs, pexprqty);

  ut_free_1d_char (string2);
  ut_free_1d_char (tmp);

  return;
}

void
ut_string_catfiles_separate (const char *string, char ***parts, int *pqty)
{
  int i, *pos = ut_alloc_1d_int (strlen (string));

  (*pqty) = 0;
  for (i = 0; i < (int) strlen (string); i++)
    if (!strncmp (string + i, "file(", 5))
    {
      pos[(*pqty)] = i;
      (*pqty)++;
    }
  pos[(*pqty)] = strlen (string);

  (*parts) = ut_alloc_2d_char (*pqty, strlen (string) + 1);
  for (i = 0; i < *pqty; i++)
    strncpy ((*parts)[i], string + pos[i], pos[i + 1] - pos[i]);

  ut_free_1d_int (pos);

  return;
}

void
ut_string_partqty (char *string, char c, int *pqty)
{
  int pos;
  char *val = ut_alloc_1d_char (strlen (string));

  (*pqty) = 0;
  pos = 0;
  while (ut_string_untilchar (string, c, &pos, val) == 1)
  {
    (*pqty)++;
    pos++;
  }

  ut_free_1d_char (val);

  return;
}

int
ut_string_part (char *string, char c, int id, char *part)
{
  int pos, nb, res;
  char *val = ut_alloc_1d_char (strlen (string) + 1);

  res = 0;
  nb = 0;
  pos = 0;
  while (ut_string_untilchar (string, c, &pos, val) == 1)
  {
    pos++;
    nb++;
    if (nb == id)
    {
      res = 1;
      break;
    }
  }

  sprintf (part, "%s", val);

  ut_free_1d_char (val);

  return res;
}

/* could be optimized: do not break the list */
int
ut_string_inlist (const char *string, const char* c, const char *part)
{
  int res;
  char *tmp = ut_alloc_1d_char (strlen (string) + 3);
  char *tmp2 = ut_alloc_1d_char (strlen (part) + 3);

  sprintf (tmp, "%s%s%s", c, string, c);
  sprintf (tmp2, "%s%s%s", c, part, c);

  res = (strstr (tmp, tmp2) != NULL) ? 1 : 0;

  ut_free_1d_char (tmp);
  ut_free_1d_char (tmp2);

  return res;
}

int
ut_string_inlist_int (char *string, char* c, int a)
{
  int status;
  char* part = ut_alloc_1d_char (20);

  sprintf (part, "%d", a);

  status = ut_string_inlist (string, c, part);

  ut_free_1d_char (part);

  return status;
}

int
ut_string_addtolist (char **pstring, char *c, char *new)
{
  int length;

  if (!(*pstring))
    ut_string_string (new, pstring);
  else
  {
    length = (*pstring) ? strlen (*pstring) : 0;
    length += 1 + strlen (new);

    (*pstring) = ut_realloc_1d_char (*pstring, length + 1);
    sprintf (*pstring + strlen (*pstring), "%s%s", c, new);
  }

  return 0;
}

int
ut_string_rmfromlist (char **pstring, char *c, char *rm)
{
  int i, partqty;
  char **parts = NULL, *string2 = NULL;

  ut_string_separate (*pstring, c, &parts, &partqty);

  for (i = 0; i < partqty; i++)
    if (strcmp (parts[i], rm))
      ut_string_addtolist (&string2, c, parts[i]);

  ut_string_string (string2, pstring);

  ut_free_1d_char (string2);
  ut_free_2d_char (parts, partqty);

  return 0;
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

  if (strstr (string, "#"))
    abort ();

  shift = 0;
  for (i = 0; i < (int) strlen (string); i++)
    if (string[i] == ' ' || string[i] == '\t' || string[i] == '\n')
      shift++;
    else
      break;

  ut_string_string (string + shift, &tmp);
  ut_string_fnr  (tmp, ' ', '#');
  ut_string_fnr  (tmp, '\t', '#');
  ut_string_fnr  (tmp, '\n', '#');
  ut_string_fnrs (tmp, "##", "#", INT_MAX);

  ut_string_separate (tmp, "#", psubstrings, psubstringqty);

  ut_free_1d_char (tmp);

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
ut_string_array_paste (char **string, int qty)
{
  int i;
  char *res;

  int length = 1;
  for (i = 0; i < qty; i++)
    length += strlen (string[i]);

  res = ut_alloc_1d_char (length);
  for (i = 0; i < qty; i++)
    res = strcat (res, string[i]);

  return res;
}

char *
ut_string_array_paste_cmp (char **string, int qty)
{
  char *res;
  char **stringcpy = ut_alloc_1d_pchar (qty);

  ut_array_1d_pchar_memcpy (stringcpy, qty, string);
  ut_array_1d_pchar_sort (stringcpy, qty);
  res = ut_string_array_paste (stringcpy, qty);

  ut_free_2d_char (stringcpy, qty);

  return res;
}

int
ut_string_format (const char *in, char *format)
{
  int i;
  int length = strlen (in);
  int dotqty, dotpos, nbqty, isstring;

  nbqty = 0;
  dotqty = 0;
  dotpos = 0;
  isstring = 0;
  for (i = 0; i < length; i++)
  {
    if ((i == 0 && in[i] == '-') || (in[i] >= '0' && in[i] <= '9'))
      nbqty++;
    else if (in[i] == '.')
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
    if (in[0] != '0')
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
ut_string_format_strcmp (char *in, char *format)
{
  int status;
  char *format2 = ut_alloc_1d_char (10);

  ut_string_format (in, format2);

  status = strcmp (format, format2);

  ut_free_1d_char (format2);

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

int
ut_string_real (char* string, double* pres)
{
  char* format = ut_alloc_1d_char (10);

  ut_string_format (string, format);

  if (format[strlen (format) - 1] == 'd' || format[strlen (format) - 1] == 'f')
    sscanf (string, "%lf", pres);

  else if (format[strlen (format) - 1] == 's')
    ut_math_eval (string, 0, NULL, NULL, pres);
  else
    abort ();

  ut_free_1d_char (format);

  return 0;
}

int
ut_string_int (char* string, int* pres)
{
  double val;

  ut_string_real (string, &val);
  (*pres) = ut_num_d2ri (val);

  return 0;
}

void
ut_string_string (const char* input, char** poutput)
{
  if (input)
  {
    (*poutput) = ut_realloc_1d_char (*poutput, strlen (input) + 1);
    strcpy (*poutput, input);
  }
  else
  {
    ut_free_1d_char (*poutput);
    (*poutput) = NULL;
  }

  return;
}

void
ut_string_separate2 (const char *string, const char* c, const char* c2, char ****parts, int** pqty2, int *pqty)
{
  int i, j;
  char** list = NULL;
  char** list2 = NULL;

  ut_string_separate (string, c, &list, pqty);

  (*parts) = ut_alloc_1d_ppchar (*pqty);
  (*pqty2) = ut_alloc_1d_int (*pqty);

  for (i = 0; i < *pqty; i++)
  {
    ut_string_separate (list[i], c2, &list2, &((*pqty2)[i]));

    (*parts)[i] = ut_alloc_1d_pchar ((*pqty2)[i]);

    for (j = 0; j < (*pqty2)[i]; j++)
      ut_string_string (list2[j], &((*parts)[i][j]));

    ut_free_2d_char (list2, (*pqty2)[i]);
    list2 = NULL;
  }

  ut_free_2d_char (list, *pqty);

  return;
}

int
ut_string_filename (char* string)
{
  return ((string && strlen (string) > 0)
	 && (!strncmp (string, "file(", 5)
          || !strncmp (string, "msfile(", 7)
          || string[0] == '@'));
}

int
ut_string_iter_test (char* string, char* sep, int iter)
{
  int i, status, iterstep, varqty;
  char **vars = NULL;

  ut_string_separate (string, sep, &vars, &varqty);

  iterstep = 1;
  for (i = 0; i < varqty; i++)
    if (!strncmp (vars[i], "iter(", 5))
      sscanf (vars[i], "iter(%d)", &iterstep);

  status = (iterstep == 1 || (iter - 1) % iterstep == 0);

  ut_free_2d_char (vars, varqty);

  return status;
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

void
ut_string_uppercase (char *string)
{
  int i = 0;

  while(string[i])
  {
    string[i] = toupper(string[i]);
    i++;
  }

  return;
}

int
ut_string_isanumber (char *s)
{
  if (s == NULL || *s == '\0' || isspace(*s))
    return 0;

  char * p;
  strtod (s, &p);
  return *p == '\0';
}
