/* Copyright (C) 2003-2024, Romain Quey.*/
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stdarg.h>
#include<limits.h>
#include"ut.h"

void
ut_list_break_2 (const char *string, const char *sep, char **ppart1, char **ppart2)
{
  int qty;
  char **tmp = NULL;

  ut_free_1d_char (ppart1);
  ut_free_1d_char (ppart2);

  ut_list_break (string, sep, &tmp, &qty);

  ut_string_string (tmp[0], ppart1);

  if (qty == 2)
    ut_string_string (tmp[1], ppart2);

  ut_free_2d_char (&tmp, qty);

  return;
}

void
ut_list_break (const char *string0, const char *sep, char ***pparts, int *pqty)
{
  int i, pos, bracket, len;
  char **tmp = NULL, *val = NULL;
  char *string = NULL;

  ut_string_string (string0, &string);

  if (!string)
  {
    (*pqty) = 0;
    return;
  }

  len = strlen (string);
  for (i = len; i >= 0; i--)
  {
    if (!isgraph (string[i]))
      string[i] = '\0';
    else
      break;
  }

  tmp = ut_alloc_2d_char (strlen (string) + 1, strlen (string) + 1);
  val = ut_alloc_1d_char (strlen (string) + 1);

  (*pqty) = 0;
  pos = 0;
  bracket = 0;
  while (ut_string_untilstring (string, sep, &pos, val))
  {
    bracket +=
      ut_string_thischarqty (val, '(') - ut_string_thischarqty (val, ')');
    if (bracket >= 0)
    {
      tmp[*pqty] = strcat (tmp[*pqty], val);
      if (bracket > 0)
        tmp[*pqty] = strcat (tmp[*pqty], sep);
      else
        (*pqty)++;
      pos += strlen (sep);
    }
    else
      abort ();
  }

  if ((unsigned int) pos == strlen (string) && strlen (string) > 0)
    (*pqty)++;

  if (pparts)
  {
    (*pparts) = ut_alloc_1d_pchar (*pqty);
    for (i = 0; i < (*pqty); i++)
      ut_string_string (tmp[i], *pparts + i);
  }

  ut_free_2d_char (&tmp, strlen (string) + 1);
  ut_free_1d_char (&val);
  ut_free_1d_char (&string);

  return;
}

void
ut_list_break2 (const char *string, const char *c, const char *c2,
                char ****parts, int **pqty2, int *pqty)
{
  int i, j;
  char **list = NULL;
  char **list2 = NULL;

  ut_list_break (string, c, &list, pqty);

  (*parts) = ut_alloc_1d_ppchar (*pqty);
  (*pqty2) = ut_alloc_1d_int (*pqty);

  for (i = 0; i < *pqty; i++)
  {
    ut_list_break (list[i], c2, &list2, &((*pqty2)[i]));

    (*parts)[i] = ut_alloc_1d_pchar ((*pqty2)[i]);

    for (j = 0; j < (*pqty2)[i]; j++)
      ut_string_string (list2[j], &((*parts)[i][j]));

    ut_free_2d_char (&list2, (*pqty2)[i]);
    list2 = NULL;
  }

  ut_free_2d_char (&list, *pqty);

  return;
}

void
ut_list_break_double (const char *string, const char *sep, double **pparts, int *pqty)
{
  int i, qty;
  char **tmp = NULL;

  ut_list_break (string, sep, &tmp, &qty);

  if (pparts)
  {
    (*pparts) = ut_alloc_1d (qty);
    for (i = 0; i < qty; i++)
      (*pparts)[i] = atof (tmp[i]);
  }

  if (pqty)
    (*pqty) = qty;

  ut_free_2d_char (&tmp, qty);

  return;
}

void
ut_list_break_int (const char *string, const char *sep, int **pparts, int *pqty)
{
  int i, qty;
  char **tmp = NULL;

  ut_list_break (string, sep, &tmp, &qty);

  if (pparts)
  {
    (*pparts) = ut_alloc_1d_int (qty);
    for (i = 0; i < qty; i++)
      (*pparts)[i] = atoi (tmp[i]);
  }

  if (pqty)
    (*pqty) = qty;

  ut_free_2d_char (&tmp, qty);

  return;
}

void
ut_list_break_int_range (const char *string, const char *sep, int **pparts, int *pqty)
{
  int i, j, beg, end, qty, *qty2 = NULL;
  char ***tmp = NULL;

  ut_list_break2 (string, sep, "-", &tmp, &qty2, &qty);

  if (pparts)
  {
    *pqty = 0;
    for (i = 0; i < qty; i++)
    {
      if (qty2[i] == 1)
      {
        beg = atoi (tmp[i][0]);
        end = atoi (tmp[i][0]);
      }

      else if (qty2[i] == 2)
      {
        beg = atoi (tmp[i][0]);
        end = atoi (tmp[i][1]);
      }

      else
        abort ();

      for (j = beg; j <= end; j++)
        ut_array_1d_int_list_addval_nocheck (pparts, pqty, j);
    }
  }

  for (i = 0; i < qty; i++)
    ut_free_2d_char (tmp + i, qty2[i]);
  ut_free_1d_ppchar (&tmp);
  ut_free_1d_int (&qty2);

  return;
}

/* could be optimized: do not break the list */
int
ut_list_testelt (const char *string, const char *c, const char *part)
{
  int res;
  char *tmp = NULL, *tmp2 = NULL;

  if (!string)
    return 0;

  tmp = ut_alloc_1d_char (strlen (string) + 3);
  tmp2 = ut_alloc_1d_char (strlen (part) + 3);

  sprintf (tmp, "%s%s%s", c, string, c);
  sprintf (tmp2, "%s%s%s", c, part, c);

  res = (strstr (tmp, tmp2) != NULL) ? 1 : 0;

  ut_free_1d_char (&tmp);
  ut_free_1d_char (&tmp2);

  return res;
}

int
ut_list_testelt_int (char *string, char *c, int a)
{
  int status;
  char *part = ut_alloc_1d_char (20);

  sprintf (part, "%d", a);

  status = ut_list_testelt (string, c, part);

  ut_free_1d_char (&part);

  return status;
}

void
ut_list_addelt (char **pstring, char *c, char *new)
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

  return;
}

int
ut_list_rmelt (char **pstring, char *c, char *rm)
{
  int i, partqty;
  char **parts = NULL, *string2 = NULL;

  ut_list_break (*pstring, c, &parts, &partqty);

  for (i = 0; i < partqty; i++)
    if (strcmp (parts[i], rm))
      ut_list_addelt (&string2, c, parts[i]);

  ut_string_string (string2, pstring);

  ut_free_1d_char (&string2);
  ut_free_2d_char (&parts, partqty);

  return 0;
}

int
ut_list_iter_test (char *string, char *sep, int iter)
{
  int i, iterstep, varqty;
  char **vars = NULL;

  ut_list_break (string, sep, &vars, &varqty);

  iterstep = 1;
  for (i = 0; i < varqty; i++)
    if (!strncmp (vars[i], "iter(", 5))
      sscanf (vars[i], "iter(%d)", &iterstep);

  ut_free_2d_char (&vars, varqty);

  return (iterstep == 1 || (iter - 1) % iterstep == 0);
}

void
ut_list_create (char **parts, int size, const char *sep, char **plist)
{
  int i, length = 0;

  for (i = 0; i < size; i++)
    length += strlen (parts[i]);
  length += (size - 1) * strlen (sep);

  (*plist) = ut_alloc_1d_char (length + 1);

  for (i = 0; i < size; i++)
    sprintf ((*plist) + strlen (*plist), "%s%s", parts[i], i < size - 1? sep : "");

  return;
}

int
ut_list_eltnb (const char *list, const char *sep)
{
  int qty;
  char **parts = NULL;

  ut_list_break (list, sep, &parts, &qty);

  ut_free_2d_char (&parts, qty);

  return qty;
}
