/* Copyright (C) 2003-2019, Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<unistd.h>
#include<math.h>
#include"ut.h"

void ut_print_1lwnc (FILE *, const char *, unsigned int *, unsigned int);

void
ut_print_wnc_int (FILE * file, int nb, int *pnbcol, int nbcolmax)
{
  int length = ut_num_tenlen_int (nb);

  if (length > nbcolmax)
    ut_print_message (2, 0,
                      "The max. number of columns of the output file is too low!\n");

  if ((*pnbcol) == 0)
  {
    fprintf (file, "%d", nb);
    (*pnbcol) = length;
  }
  else if ((*pnbcol) + 1 + length <= nbcolmax)
  {
    fprintf (file, " %d", nb);
    (*pnbcol) += length + 1;
  }
  else
  {
    fprintf (file, "\n%d", nb);
    (*pnbcol) = length;
  }

  return;
}

int
ut_print_wnc_string (FILE * file, const char *string, int *pnbcol,
                     int nbcolmax)
{
  int status;
  int length = strlen (string);

  if (length > nbcolmax)
    ut_print_message (2, 0,
                      "The max. number of columns of the output file is too low!\n");

  status = 0;
  if ((*pnbcol) == 0)
  {
    fprintf (file, "%s", string);
    (*pnbcol) = length;
  }
  else if ((*pnbcol) + 1 + length <= nbcolmax)
  {
    fprintf (file, " %s", string);
    (*pnbcol) += length + 1;
  }
  else
  {
    fprintf (file, "\n%s", string);
    (*pnbcol) = length;
    status = 1;
  }

  return status;
}

void
ut_print_wnc_int_header (FILE * file, int nb, int *pnbcol, int nbcolmax,
                         const char *head)
{
  int length = ut_num_tenlen_int (nb);

  if (length > nbcolmax)
    ut_print_message (2, 0,
                      "The max. number of columns of the output file is too low!\n");

  if ((*pnbcol) == 0)
  {
    fprintf (file, "%s %d", head, nb);
    (*pnbcol) = strlen (head) + 1 + length;
  }
  else if ((*pnbcol) + 1 + length <= nbcolmax)
  {
    fprintf (file, " %d", nb);
    (*pnbcol) += length + 1;
  }
  else
  {
    fprintf (file, "\n%s %d", head, nb);
    (*pnbcol) = strlen (head) + 1 + length;
  }

  return;
}

void
ut_print_wnc (FILE * file, int *pnbcol, int nbcolmax, const char *s, ...)
{
  va_list args;
  char *string = ut_alloc_1d_char (1000);

  va_start (args, s);
  vsprintf (string, s, args);
  va_end (args);

  ut_print_wnc_string (file, string, pnbcol, nbcolmax);

  ut_free_1d_char (&string);

  return;
}

void
ut_print_wnc_wnelts (FILE * file, int *pnbcol, int nbcolmax, int *pnbelts,
                     int nbeltsmax, const char *s, ...)
{
  int status;
  va_list args;
  char *string = ut_alloc_1d_char (1000);

  va_start (args, s);
  vsprintf (string, s, args);
  va_end (args);

  if (++(*pnbelts) <= nbeltsmax)
  {
    status = ut_print_wnc_string (file, string, pnbcol, nbcolmax);
    if (status == 1)
      (*pnbelts) = 1;
  }
  else
  {
    fprintf (file, "\n");
    (*pnbcol) = 0;
    (*pnbelts) = 1;
    ut_print_wnc_string (file, string, pnbcol, nbcolmax);
  }

  ut_free_1d_char (&string);

  return;
}

void
ut_print_1lwnc (FILE * file, const char *string, unsigned int *pnbcol,
                unsigned int nbcolmax)
{
  int i;
  int print_qty = 0;
  const char *c = NULL;

  while (string[(*pnbcol)] == ' ')
    (*pnbcol)++;

  c = string + (*pnbcol);

  if (strlen (c) <= nbcolmax)
    print_qty = strlen (c);
  else
    for (i = nbcolmax + 1; i >= 0; i--)
      if (c[i] == ' ')
      {
        print_qty = i;
        break;
      }

  if (!print_qty)
  {
    for (i = nbcolmax; i < (int) strlen (c); i++)
      if (c[i] == ' ' || c[i] == '\0')
      {
        print_qty = i;
        break;
      }

    if (i == (int) strlen (c))
      print_qty = strlen (c);
  }

  for (i = 0; i < print_qty; i++)
    fprintf (file, "%c", c[i]);
  fprintf (file, "\n");

  (*pnbcol) += print_qty;

  return;
}

void
ut_print_lineheader (int head)
{
  switch (head)
  {
  case -1:
    printf ("*DEBUG*: ");
    break;
  case 0:
    printf ("Info   : ");
    break;
  case 1:
    printf ("Warning: ");
    break;
  case 2:
    printf ("Error  : ");
    break;
  default:
    printf ("\t  ");
    break;
  }

  return;
}

void
ut_print_level (int level)
{
  if (level == 2)
    printf ("  - ");
  else if (level == 3)
    printf ("    > ");
  else if (level == 4)
    printf ("      . ");

  return;
}

void
ut_print_increment (int incr, int incrf)
{
  if (incrf > 0)
    printf ("[%d/%d] ", incr, incrf);
  else
    printf ("[%d] ", incr);

  return;
}

void
ut_print_message (int head, int level, const char *message, ...)
{
  va_list args;

  ut_print_lineheader (head);

  ut_print_level (level);

  va_start (args, message);
  vfprintf (stdout, message, args);
  va_end (args);

  fflush (stdout);

  if (head == 2)
    abort ();

  return;
}

void
ut_print_message_incr (int head, int level, int incr, int incrf,
                       const char *message, ...)
{
  va_list args;

  ut_print_lineheader (head);

  ut_print_level (level);

  ut_print_increment (incr, incrf);

  va_start (args, message);
  vfprintf (stdout, message, args);
  va_end (args);

  fflush (stdout);

  if (head == 2)
    abort ();

  return;
}

void
ut_print_messagewnc (int head, int nbcolmax, const char *message)
{
  char *Header = ut_alloc_1d_char (10);

  int first = 1;
  int length;
  int line;
  unsigned int m;

  switch (head)
  {
  case 0:
    sprintf (Header, "Info   : ");
    length = strlen ("Info   : ");
    break;
  case 1:
    sprintf (Header, "Warning: ");
    length = strlen ("Warning: ");
    break;
  case 2:
    sprintf (Header, "Error  : ");
    length = strlen ("Error  : ");
    break;
  case -1:
    sprintf (Header, "*DEBUG*: ");
    length = strlen ("*DEBUG*: ");
    break;
  default:
    abort ();
  }

  line = 0;
  m = 0;
  length = 9;                   // 10 changed to 9
  while (m != strlen (message))
  {
    line++;

    if (first == 1)
    {
      printf ("%s", Header);
      first = 0;
    }
    else
      printf ("         ");

    ut_print_1lwnc (stdout, message, &m, nbcolmax - length);

    if (line > (int) strlen (message))
    {
      printf ("         ");
      printf ("%s\n", message + m);
      break;
    }
  }

  ut_free_1d_char (&Header);

  if (head == 2)
    abort ();

  fflush (stdout);

  return;
}

void
ut_print_missingfile (const char *filename)
{
  ut_print_message (2, 0, "File '%s' not found.\n", filename);

  return;
}

void
ut_print_moduleheader (const char *module, int fargv, char **fargc, int argv,
                       char **argc)
{
  int i, qty, fqty;

  char *fcommand1 = NULL;
  char *fcommand2 = NULL;
  char *command1 = NULL;
  char *command2 = NULL;

  qty = 0;
  for (i = 1; i < argv; i++)
    qty += strlen (argc[i]) + 1;

  fqty = 0;
  for (i = 1; i < fargv; i++)
    fqty += strlen (fargc[i]) + 1;

  fcommand1 = ut_alloc_1d_char (fqty + 100);
  fcommand2 = ut_alloc_1d_char (fqty + 100);
  command1 = ut_alloc_1d_char (qty + 100);
  command2 = ut_alloc_1d_char (qty + 100);

  if (argv == 1)
    sprintf (command1, "[com line] (none)");
  else
    sprintf (command1, "[com line]");
  sprintf (command2, "%s", command1);

  for (i = 1; i < argv; i++)
  {
    sprintf (command2, "%s %s", command1, argc[i]);
    sprintf (command1, "%s", command2);
  }

  if (fargv == 0 || fargv == 1)
    sprintf (fcommand1, "[ini file] (none)");
  else
    sprintf (fcommand1, "[ini file]");
  sprintf (fcommand2, "%s", fcommand1);
  for (i = 1; i < fargv; i++)
  {
    sprintf (fcommand2, "%s %s", fcommand1, fargc[i]);
    sprintf (fcommand1, "%s", fcommand2);
  }

  ut_print_lineheader (0);
  printf
    ("---------------------------------------------------------------\n");
  ut_print_lineheader (0);
  printf ("MODULE %3s loaded with arguments:\n", module);
  ut_print_messagewnc (0, 72, fcommand2);
  ut_print_messagewnc (0, 72, command2);
  ut_print_lineheader (0);
  printf
    ("---------------------------------------------------------------\n");

  ut_free_1d_char (&fcommand1);
  ut_free_1d_char (&fcommand2);
  ut_free_1d_char (&command1);
  ut_free_1d_char (&command2);

  return;
}

// print progress with no newline at the end (100%)
void
ut_print_progress_nonl (FILE * file, long current, long total,
                        const char *format, char *prevprogress)
{
  unsigned int i, beg;
  char *string = ut_alloc_1d_char (strlen (format) + 100);

  sprintf (string, format, 100 * ((double) current / (double) total));

  beg = 0;
  for (i = 0; i < ut_num_min (strlen (string), strlen (prevprogress)); i++)
    if (string[i] == prevprogress[i])
      beg = i;
    else
      break;

  if (strcmp (string, prevprogress))
  {
    if (isatty (1))
    {
      for (i = strlen (string); i < strlen (prevprogress); i++)
        fprintf (file, "\b");
      for (i = strlen (string); i < strlen (prevprogress); i++)
        fprintf (file, " ");
      for (i = beg; i < strlen (prevprogress); i++)
        fprintf (file, "\b");
      fprintf (file, "%s", string + beg);
    }
    else
      fprintf (file, "\n%s", string);

    fflush (file);

    strcpy (prevprogress, string);
  }

  ut_free_1d_char (&string);

  return;
}

void
ut_print_progress (FILE * file, long nb, long tot, const char *format,
                   char *prev)
{
  ut_print_progress_nonl (file, nb, tot, format, prev);

  if (nb == tot && isatty (1))
    fprintf (file, "\n");

  return;
}

void
ut_print_clearline (FILE * file, int qty)
{
  int i;

  for (i = 0; i < qty; i++)
    fprintf (file, "\b");
  for (i = 0; i < qty; i++)
    fprintf (file, " ");
  for (i = 0; i < qty; i++)
    fprintf (file, "\b");

  return;
}

void
ut_print_gplv3 (FILE * file)
{
  fprintf (file, "                    GNU GENERAL PUBLIC LICENSE\n");
  fprintf (file, "                       Version 3, 29 June 2007\n");
  fprintf (file, "\n");
  fprintf (file,
           " Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>\n");
  fprintf (file,
           " Everyone is permitted to copy and distribute verbatim copies\n");
  fprintf (file,
           " of this license document, but changing it is not allowed.\n");
  fprintf (file, "\n");
  fprintf (file, "                            Preamble\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The GNU General Public License is a free, copyleft license for\n");
  fprintf (file, "software and other kinds of works.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The licenses for most software and other practical works are designed\n");
  fprintf (file,
           "to take away your freedom to share and change the works.  By contrast,\n");
  fprintf (file,
           "the GNU General Public License is intended to guarantee your freedom to\n");
  fprintf (file,
           "share and change all versions of a program--to make sure it remains free\n");
  fprintf (file,
           "software for all its users.  We, the Free Software Foundation, use the\n");
  fprintf (file,
           "GNU General Public License for most of our software; it applies also to\n");
  fprintf (file,
           "any other work released this way by its authors.  You can apply it to\n");
  fprintf (file, "your programs, too.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  When we speak of free software, we are referring to freedom, not\n");
  fprintf (file,
           "price.  Our General Public Licenses are designed to make sure that you\n");
  fprintf (file,
           "have the freedom to distribute copies of free software (and charge for\n");
  fprintf (file,
           "them if you wish), that you receive source code or can get it if you\n");
  fprintf (file,
           "want it, that you can change the software or use pieces of it in new\n");
  fprintf (file,
           "free programs, and that you know you can do these things.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  To protect your rights, we need to prevent others from denying you\n");
  fprintf (file,
           "these rights or asking you to surrender the rights.  Therefore, you have\n");
  fprintf (file,
           "certain responsibilities if you distribute copies of the software, or if\n");
  fprintf (file,
           "you modify it: responsibilities to respect the freedom of others.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  For example, if you distribute copies of such a program, whether\n");
  fprintf (file,
           "gratis or for a fee, you must pass on to the recipients the same\n");
  fprintf (file,
           "freedoms that you received.  You must make sure that they, too, receive\n");
  fprintf (file,
           "or can get the source code.  And you must show them these terms so they\n");
  fprintf (file, "know their rights.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Developers that use the GNU GPL protect your rights with two steps:\n");
  fprintf (file,
           "(1) assert copyright on the software, and (2) offer you this License\n");
  fprintf (file,
           "giving you legal permission to copy, distribute and/or modify it.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  For the developers' and authors' protection, the GPL clearly explains\n");
  fprintf (file,
           "that there is no warranty for this free software.  For both users' and\n");
  fprintf (file,
           "authors' sake, the GPL requires that modified versions be marked as\n");
  fprintf (file,
           "changed, so that their problems will not be attributed erroneously to\n");
  fprintf (file, "authors of previous versions.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Some devices are designed to deny users access to install or run\n");
  fprintf (file,
           "modified versions of the software inside them, although the manufacturer\n");
  fprintf (file,
           "can do so.  This is fundamentally incompatible with the aim of\n");
  fprintf (file,
           "protecting users' freedom to change the software.  The systematic\n");
  fprintf (file,
           "pattern of such abuse occurs in the area of products for individuals to\n");
  fprintf (file,
           "use, which is precisely where it is most unacceptable.  Therefore, we\n");
  fprintf (file,
           "have designed this version of the GPL to prohibit the practice for those\n");
  fprintf (file,
           "products.  If such problems arise substantially in other domains, we\n");
  fprintf (file,
           "stand ready to extend this provision to those domains in future versions\n");
  fprintf (file, "of the GPL, as needed to protect the freedom of users.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Finally, every program is threatened constantly by software patents.\n");
  fprintf (file,
           "States should not allow patents to restrict development and use of\n");
  fprintf (file,
           "software on general-purpose computers, but in those that do, we wish to\n");
  fprintf (file,
           "avoid the special danger that patents applied to a free program could\n");
  fprintf (file,
           "make it effectively proprietary.  To prevent this, the GPL assures that\n");
  fprintf (file, "patents cannot be used to render the program non-free.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The precise terms and conditions for copying, distribution and\n");
  fprintf (file, "modification follow.\n");
  fprintf (file, "\n");
  fprintf (file, "                       TERMS AND CONDITIONS\n");
  fprintf (file, "\n");
  fprintf (file, "  0. Definitions.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  \"This License\" refers to version 3 of the GNU General Public License.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  \"Copyright\" also means copyright-like laws that apply to other kinds of\n");
  fprintf (file, "works, such as semiconductor masks.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  \"The Program\" refers to any copyrightable work licensed under this\n");
  fprintf (file,
           "License.  Each licensee is addressed as \"you\".  \"Licensees\" and\n");
  fprintf (file, "\"recipients\" may be individuals or organizations.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  To \"modify\" a work means to copy from or adapt all or part of the work\n");
  fprintf (file,
           "in a fashion requiring copyright permission, other than the making of an\n");
  fprintf (file,
           "exact copy.  The resulting work is called a \"modified version\" of the\n");
  fprintf (file, "earlier work or a work \"based on\" the earlier work.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A \"covered work\" means either the unmodified Program or a work based\n");
  fprintf (file, "on the Program.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  To \"propagate\" a work means to do anything with it that, without\n");
  fprintf (file,
           "permission, would make you directly or secondarily liable for\n");
  fprintf (file,
           "infringement under applicable copyright law, except executing it on a\n");
  fprintf (file,
           "computer or modifying a private copy.  Propagation includes copying,\n");
  fprintf (file,
           "distribution (with or without modification), making available to the\n");
  fprintf (file, "public, and in some countries other activities as well.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  To \"convey\" a work means any kind of propagation that enables other\n");
  fprintf (file,
           "parties to make or receive copies.  Mere interaction with a user through\n");
  fprintf (file,
           "a computer network, with no transfer of a copy, is not conveying.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  An interactive user interface displays \"Appropriate Legal Notices\"\n");
  fprintf (file,
           "to the extent that it includes a convenient and prominently visible\n");
  fprintf (file,
           "feature that (1) displays an appropriate copyright notice, and (2)\n");
  fprintf (file,
           "tells the user that there is no warranty for the work (except to the\n");
  fprintf (file,
           "extent that warranties are provided), that licensees may convey the\n");
  fprintf (file,
           "work under this License, and how to view a copy of this License.  If\n");
  fprintf (file,
           "the interface presents a list of user commands or options, such as a\n");
  fprintf (file,
           "menu, a prominent item in the list meets this criterion.\n");
  fprintf (file, "\n");
  fprintf (file, "  1. Source Code.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The \"source code\" for a work means the preferred form of the work\n");
  fprintf (file,
           "for making modifications to it.  \"Object code\" means any non-source\n");
  fprintf (file, "form of a work.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A \"Standard Interface\" means an interface that either is an official\n");
  fprintf (file,
           "standard defined by a recognized standards body, or, in the case of\n");
  fprintf (file,
           "interfaces specified for a particular programming language, one that\n");
  fprintf (file,
           "is widely used among developers working in that language.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The \"System Libraries\" of an executable work include anything, other\n");
  fprintf (file,
           "than the work as a whole, that (a) is included in the normal form of\n");
  fprintf (file,
           "packaging a Major Component, but which is not part of that Major\n");
  fprintf (file,
           "Component, and (b) serves only to enable use of the work with that\n");
  fprintf (file,
           "Major Component, or to implement a Standard Interface for which an\n");
  fprintf (file,
           "implementation is available to the public in source code form.  A\n");
  fprintf (file,
           "\"Major Component\", in this context, means a major essential component\n");
  fprintf (file,
           "(kernel, window system, and so on) of the specific operating system\n");
  fprintf (file,
           "(if any) on which the executable work runs, or a compiler used to\n");
  fprintf (file,
           "produce the work, or an object code interpreter used to run it.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The \"Corresponding Source\" for a work in object code form means all\n");
  fprintf (file,
           "the source code needed to generate, install, and (for an executable\n");
  fprintf (file,
           "work) run the object code and to modify the work, including scripts to\n");
  fprintf (file,
           "control those activities.  However, it does not include the work's\n");
  fprintf (file,
           "System Libraries, or general-purpose tools or generally available free\n");
  fprintf (file,
           "programs which are used unmodified in performing those activities but\n");
  fprintf (file,
           "which are not part of the work.  For example, Corresponding Source\n");
  fprintf (file,
           "includes interface definition files associated with source files for\n");
  fprintf (file,
           "the work, and the source code for shared libraries and dynamically\n");
  fprintf (file,
           "linked subprograms that the work is specifically designed to require,\n");
  fprintf (file,
           "such as by intimate data communication or control flow between those\n");
  fprintf (file, "subprograms and other parts of the work.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The Corresponding Source need not include anything that users\n");
  fprintf (file,
           "can regenerate automatically from other parts of the Corresponding\n");
  fprintf (file, "Source.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The Corresponding Source for a work in source code form is that\n");
  fprintf (file, "same work.\n");
  fprintf (file, "\n");
  fprintf (file, "  2. Basic Permissions.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  All rights granted under this License are granted for the term of\n");
  fprintf (file,
           "copyright on the Program, and are irrevocable provided the stated\n");
  fprintf (file,
           "conditions are met.  This License explicitly affirms your unlimited\n");
  fprintf (file,
           "permission to run the unmodified Program.  The output from running a\n");
  fprintf (file,
           "covered work is covered by this License only if the output, given its\n");
  fprintf (file,
           "content, constitutes a covered work.  This License acknowledges your\n");
  fprintf (file,
           "rights of fair use or other equivalent, as provided by copyright law.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may make, run and propagate covered works that you do not\n");
  fprintf (file,
           "convey, without conditions so long as your license otherwise remains\n");
  fprintf (file,
           "in force.  You may convey covered works to others for the sole purpose\n");
  fprintf (file,
           "of having them make modifications exclusively for you, or provide you\n");
  fprintf (file,
           "with facilities for running those works, provided that you comply with\n");
  fprintf (file,
           "the terms of this License in conveying all material for which you do\n");
  fprintf (file,
           "not control copyright.  Those thus making or running the covered works\n");
  fprintf (file,
           "for you must do so exclusively on your behalf, under your direction\n");
  fprintf (file,
           "and control, on terms that prohibit them from making any copies of\n");
  fprintf (file,
           "your copyrighted material outside their relationship with you.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Conveying under any other circumstances is permitted solely under\n");
  fprintf (file,
           "the conditions stated below.  Sublicensing is not allowed; section 10\n");
  fprintf (file, "makes it unnecessary.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  3. Protecting Users' Legal Rights From Anti-Circumvention Law.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  No covered work shall be deemed part of an effective technological\n");
  fprintf (file,
           "measure under any applicable law fulfilling obligations under article\n");
  fprintf (file,
           "11 of the WIPO copyright treaty adopted on 20 December 1996, or\n");
  fprintf (file,
           "similar laws prohibiting or restricting circumvention of such\n");
  fprintf (file, "measures.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  When you convey a covered work, you waive any legal power to forbid\n");
  fprintf (file,
           "circumvention of technological measures to the extent such circumvention\n");
  fprintf (file,
           "is effected by exercising rights under this License with respect to\n");
  fprintf (file,
           "the covered work, and you disclaim any intention to limit operation or\n");
  fprintf (file,
           "modification of the work as a means of enforcing, against the work's\n");
  fprintf (file,
           "users, your or third parties' legal rights to forbid circumvention of\n");
  fprintf (file, "technological measures.\n");
  fprintf (file, "\n");
  fprintf (file, "  4. Conveying Verbatim Copies.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may convey verbatim copies of the Program's source code as you\n");
  fprintf (file,
           "receive it, in any medium, provided that you conspicuously and\n");
  fprintf (file,
           "appropriately publish on each copy an appropriate copyright notice;\n");
  fprintf (file,
           "keep intact all notices stating that this License and any\n");
  fprintf (file,
           "non-permissive terms added in accord with section 7 apply to the code;\n");
  fprintf (file,
           "keep intact all notices of the absence of any warranty; and give all\n");
  fprintf (file,
           "recipients a copy of this License along with the Program.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may charge any price or no price for each copy that you convey,\n");
  fprintf (file,
           "and you may offer support or warranty protection for a fee.\n");
  fprintf (file, "\n");
  fprintf (file, "  5. Conveying Modified Source Versions.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may convey a work based on the Program, or the modifications to\n");
  fprintf (file,
           "produce it from the Program, in the form of source code under the\n");
  fprintf (file,
           "terms of section 4, provided that you also meet all of these conditions:\n");
  fprintf (file, "\n");
  fprintf (file,
           "    a) The work must carry prominent notices stating that you modified\n");
  fprintf (file, "    it, and giving a relevant date.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    b) The work must carry prominent notices stating that it is\n");
  fprintf (file,
           "    released under this License and any conditions added under section\n");
  fprintf (file,
           "    7.  This requirement modifies the requirement in section 4 to\n");
  fprintf (file, "    \"keep intact all notices\".\n");
  fprintf (file, "\n");
  fprintf (file,
           "    c) You must license the entire work, as a whole, under this\n");
  fprintf (file,
           "    License to anyone who comes into possession of a copy.  This\n");
  fprintf (file,
           "    License will therefore apply, along with any applicable section 7\n");
  fprintf (file,
           "    additional terms, to the whole of the work, and all its parts,\n");
  fprintf (file,
           "    regardless of how they are packaged.  This License gives no\n");
  fprintf (file,
           "    permission to license the work in any other way, but it does not\n");
  fprintf (file,
           "    invalidate such permission if you have separately received it.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    d) If the work has interactive user interfaces, each must display\n");
  fprintf (file,
           "    Appropriate Legal Notices; however, if the Program has interactive\n");
  fprintf (file,
           "    interfaces that do not display Appropriate Legal Notices, your\n");
  fprintf (file, "    work need not make them do so.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A compilation of a covered work with other separate and independent\n");
  fprintf (file,
           "works, which are not by their nature extensions of the covered work,\n");
  fprintf (file,
           "and which are not combined with it such as to form a larger program,\n");
  fprintf (file,
           "in or on a volume of a storage or distribution medium, is called an\n");
  fprintf (file,
           "\"aggregate\" if the compilation and its resulting copyright are not\n");
  fprintf (file,
           "used to limit the access or legal rights of the compilation's users\n");
  fprintf (file,
           "beyond what the individual works permit.  Inclusion of a covered work\n");
  fprintf (file,
           "in an aggregate does not cause this License to apply to the other\n");
  fprintf (file, "parts of the aggregate.\n");
  fprintf (file, "\n");
  fprintf (file, "  6. Conveying Non-Source Forms.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may convey a covered work in object code form under the terms\n");
  fprintf (file, "of sections 4 and 5, provided that you also convey the\n");
  fprintf (file,
           "machine-readable Corresponding Source under the terms of this License,\n");
  fprintf (file, "in one of these ways:\n");
  fprintf (file, "\n");
  fprintf (file,
           "    a) Convey the object code in, or embodied in, a physical product\n");
  fprintf (file,
           "    (including a physical distribution medium), accompanied by the\n");
  fprintf (file,
           "    Corresponding Source fixed on a durable physical medium\n");
  fprintf (file, "    customarily used for software interchange.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    b) Convey the object code in, or embodied in, a physical product\n");
  fprintf (file,
           "    (including a physical distribution medium), accompanied by a\n");
  fprintf (file,
           "    written offer, valid for at least three years and valid for as\n");
  fprintf (file,
           "    long as you offer spare parts or customer support for that product\n");
  fprintf (file,
           "    model, to give anyone who possesses the object code either (1) a\n");
  fprintf (file,
           "    copy of the Corresponding Source for all the software in the\n");
  fprintf (file,
           "    product that is covered by this License, on a durable physical\n");
  fprintf (file,
           "    medium customarily used for software interchange, for a price no\n");
  fprintf (file,
           "    more than your reasonable cost of physically performing this\n");
  fprintf (file, "    conveying of source, or (2) access to copy the\n");
  fprintf (file,
           "    Corresponding Source from a network server at no charge.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    c) Convey individual copies of the object code with a copy of the\n");
  fprintf (file,
           "    written offer to provide the Corresponding Source.  This\n");
  fprintf (file,
           "    alternative is allowed only occasionally and noncommercially, and\n");
  fprintf (file,
           "    only if you received the object code with such an offer, in accord\n");
  fprintf (file, "    with subsection 6b.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    d) Convey the object code by offering access from a designated\n");
  fprintf (file,
           "    place (gratis or for a charge), and offer equivalent access to the\n");
  fprintf (file,
           "    Corresponding Source in the same way through the same place at no\n");
  fprintf (file,
           "    further charge.  You need not require recipients to copy the\n");
  fprintf (file,
           "    Corresponding Source along with the object code.  If the place to\n");
  fprintf (file,
           "    copy the object code is a network server, the Corresponding Source\n");
  fprintf (file,
           "    may be on a different server (operated by you or a third party)\n");
  fprintf (file,
           "    that supports equivalent copying facilities, provided you maintain\n");
  fprintf (file,
           "    clear directions next to the object code saying where to find the\n");
  fprintf (file,
           "    Corresponding Source.  Regardless of what server hosts the\n");
  fprintf (file,
           "    Corresponding Source, you remain obligated to ensure that it is\n");
  fprintf (file,
           "    available for as long as needed to satisfy these requirements.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    e) Convey the object code using peer-to-peer transmission, provided\n");
  fprintf (file,
           "    you inform other peers where the object code and Corresponding\n");
  fprintf (file,
           "    Source of the work are being offered to the general public at no\n");
  fprintf (file, "    charge under subsection 6d.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A separable portion of the object code, whose source code is excluded\n");
  fprintf (file,
           "from the Corresponding Source as a System Library, need not be\n");
  fprintf (file, "included in conveying the object code work.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A \"User Product\" is either (1) a \"consumer product\", which means any\n");
  fprintf (file,
           "tangible personal property which is normally used for personal, family,\n");
  fprintf (file,
           "or household purposes, or (2) anything designed or sold for incorporation\n");
  fprintf (file,
           "into a dwelling.  In determining whether a product is a consumer product,\n");
  fprintf (file,
           "doubtful cases shall be resolved in favor of coverage.  For a particular\n");
  fprintf (file,
           "product received by a particular user, \"normally used\" refers to a\n");
  fprintf (file,
           "typical or common use of that class of product, regardless of the status\n");
  fprintf (file,
           "of the particular user or of the way in which the particular user\n");
  fprintf (file,
           "actually uses, or expects or is expected to use, the product.  A product\n");
  fprintf (file,
           "is a consumer product regardless of whether the product has substantial\n");
  fprintf (file,
           "commercial, industrial or non-consumer uses, unless such uses represent\n");
  fprintf (file, "the only significant mode of use of the product.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  \"Installation Information\" for a User Product means any methods,\n");
  fprintf (file,
           "procedures, authorization keys, or other information required to install\n");
  fprintf (file,
           "and execute modified versions of a covered work in that User Product from\n");
  fprintf (file,
           "a modified version of its Corresponding Source.  The information must\n");
  fprintf (file,
           "suffice to ensure that the continued functioning of the modified object\n");
  fprintf (file,
           "code is in no case prevented or interfered with solely because\n");
  fprintf (file, "modification has been made.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If you convey an object code work under this section in, or with, or\n");
  fprintf (file,
           "specifically for use in, a User Product, and the conveying occurs as\n");
  fprintf (file,
           "part of a transaction in which the right of possession and use of the\n");
  fprintf (file,
           "User Product is transferred to the recipient in perpetuity or for a\n");
  fprintf (file,
           "fixed term (regardless of how the transaction is characterized), the\n");
  fprintf (file,
           "Corresponding Source conveyed under this section must be accompanied\n");
  fprintf (file,
           "by the Installation Information.  But this requirement does not apply\n");
  fprintf (file,
           "if neither you nor any third party retains the ability to install\n");
  fprintf (file,
           "modified object code on the User Product (for example, the work has\n");
  fprintf (file, "been installed in ROM).\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The requirement to provide Installation Information does not include a\n");
  fprintf (file,
           "requirement to continue to provide support service, warranty, or updates\n");
  fprintf (file,
           "for a work that has been modified or installed by the recipient, or for\n");
  fprintf (file,
           "the User Product in which it has been modified or installed.  Access to a\n");
  fprintf (file,
           "network may be denied when the modification itself materially and\n");
  fprintf (file,
           "adversely affects the operation of the network or violates the rules and\n");
  fprintf (file, "protocols for communication across the network.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Corresponding Source conveyed, and Installation Information provided,\n");
  fprintf (file,
           "in accord with this section must be in a format that is publicly\n");
  fprintf (file,
           "documented (and with an implementation available to the public in\n");
  fprintf (file,
           "source code form), and must require no special password or key for\n");
  fprintf (file, "unpacking, reading or copying.\n");
  fprintf (file, "\n");
  fprintf (file, "  7. Additional Terms.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  \"Additional permissions\" are terms that supplement the terms of this\n");
  fprintf (file,
           "License by making exceptions from one or more of its conditions.\n");
  fprintf (file,
           "Additional permissions that are applicable to the entire Program shall\n");
  fprintf (file,
           "be treated as though they were included in this License, to the extent\n");
  fprintf (file,
           "that they are valid under applicable law.  If additional permissions\n");
  fprintf (file,
           "apply only to part of the Program, that part may be used separately\n");
  fprintf (file,
           "under those permissions, but the entire Program remains governed by\n");
  fprintf (file,
           "this License without regard to the additional permissions.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  When you convey a copy of a covered work, you may at your option\n");
  fprintf (file,
           "remove any additional permissions from that copy, or from any part of\n");
  fprintf (file,
           "it.  (Additional permissions may be written to require their own\n");
  fprintf (file,
           "removal in certain cases when you modify the work.)  You may place\n");
  fprintf (file,
           "additional permissions on material, added by you to a covered work,\n");
  fprintf (file,
           "for which you have or can give appropriate copyright permission.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Notwithstanding any other provision of this License, for material you\n");
  fprintf (file,
           "add to a covered work, you may (if authorized by the copyright holders of\n");
  fprintf (file,
           "that material) supplement the terms of this License with terms:\n");
  fprintf (file, "\n");
  fprintf (file,
           "    a) Disclaiming warranty or limiting liability differently from the\n");
  fprintf (file, "    terms of sections 15 and 16 of this License; or\n");
  fprintf (file, "\n");
  fprintf (file,
           "    b) Requiring preservation of specified reasonable legal notices or\n");
  fprintf (file,
           "    author attributions in that material or in the Appropriate Legal\n");
  fprintf (file, "    Notices displayed by works containing it; or\n");
  fprintf (file, "\n");
  fprintf (file,
           "    c) Prohibiting misrepresentation of the origin of that material, or\n");
  fprintf (file,
           "    requiring that modified versions of such material be marked in\n");
  fprintf (file,
           "    reasonable ways as different from the original version; or\n");
  fprintf (file, "\n");
  fprintf (file,
           "    d) Limiting the use for publicity purposes of names of licensors or\n");
  fprintf (file, "    authors of the material; or\n");
  fprintf (file, "\n");
  fprintf (file,
           "    e) Declining to grant rights under trademark law for use of some\n");
  fprintf (file, "    trade names, trademarks, or service marks; or\n");
  fprintf (file, "\n");
  fprintf (file,
           "    f) Requiring indemnification of licensors and authors of that\n");
  fprintf (file,
           "    material by anyone who conveys the material (or modified versions of\n");
  fprintf (file,
           "    it) with contractual assumptions of liability to the recipient, for\n");
  fprintf (file,
           "    any liability that these contractual assumptions directly impose on\n");
  fprintf (file, "    those licensors and authors.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  All other non-permissive additional terms are considered \"further\n");
  fprintf (file,
           "restrictions\" within the meaning of section 10.  If the Program as you\n");
  fprintf (file,
           "received it, or any part of it, contains a notice stating that it is\n");
  fprintf (file,
           "governed by this License along with a term that is a further\n");
  fprintf (file,
           "restriction, you may remove that term.  If a license document contains\n");
  fprintf (file,
           "a further restriction but permits relicensing or conveying under this\n");
  fprintf (file,
           "License, you may add to a covered work material governed by the terms\n");
  fprintf (file,
           "of that license document, provided that the further restriction does\n");
  fprintf (file, "not survive such relicensing or conveying.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If you add terms to a covered work in accord with this section, you\n");
  fprintf (file,
           "must place, in the relevant source files, a statement of the\n");
  fprintf (file,
           "additional terms that apply to those files, or a notice indicating\n");
  fprintf (file, "where to find the applicable terms.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Additional terms, permissive or non-permissive, may be stated in the\n");
  fprintf (file,
           "form of a separately written license, or stated as exceptions;\n");
  fprintf (file, "the above requirements apply either way.\n");
  fprintf (file, "\n");
  fprintf (file, "  8. Termination.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may not propagate or modify a covered work except as expressly\n");
  fprintf (file,
           "provided under this License.  Any attempt otherwise to propagate or\n");
  fprintf (file,
           "modify it is void, and will automatically terminate your rights under\n");
  fprintf (file,
           "this License (including any patent licenses granted under the third\n");
  fprintf (file, "paragraph of section 11).\n");
  fprintf (file, "\n");
  fprintf (file,
           "  However, if you cease all violation of this License, then your\n");
  fprintf (file,
           "license from a particular copyright holder is reinstated (a)\n");
  fprintf (file,
           "provisionally, unless and until the copyright holder explicitly and\n");
  fprintf (file,
           "finally terminates your license, and (b) permanently, if the copyright\n");
  fprintf (file,
           "holder fails to notify you of the violation by some reasonable means\n");
  fprintf (file, "prior to 60 days after the cessation.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Moreover, your license from a particular copyright holder is\n");
  fprintf (file,
           "reinstated permanently if the copyright holder notifies you of the\n");
  fprintf (file,
           "violation by some reasonable means, this is the first time you have\n");
  fprintf (file,
           "received notice of violation of this License (for any work) from that\n");
  fprintf (file,
           "copyright holder, and you cure the violation prior to 30 days after\n");
  fprintf (file, "your receipt of the notice.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Termination of your rights under this section does not terminate the\n");
  fprintf (file,
           "licenses of parties who have received copies or rights from you under\n");
  fprintf (file,
           "this License.  If your rights have been terminated and not permanently\n");
  fprintf (file,
           "reinstated, you do not qualify to receive new licenses for the same\n");
  fprintf (file, "material under section 10.\n");
  fprintf (file, "\n");
  fprintf (file, "  9. Acceptance Not Required for Having Copies.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You are not required to accept this License in order to receive or\n");
  fprintf (file,
           "run a copy of the Program.  Ancillary propagation of a covered work\n");
  fprintf (file,
           "occurring solely as a consequence of using peer-to-peer transmission\n");
  fprintf (file,
           "to receive a copy likewise does not require acceptance.  However,\n");
  fprintf (file,
           "nothing other than this License grants you permission to propagate or\n");
  fprintf (file,
           "modify any covered work.  These actions infringe copyright if you do\n");
  fprintf (file,
           "not accept this License.  Therefore, by modifying or propagating a\n");
  fprintf (file,
           "covered work, you indicate your acceptance of this License to do so.\n");
  fprintf (file, "\n");
  fprintf (file, "  10. Automatic Licensing of Downstream Recipients.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Each time you convey a covered work, the recipient automatically\n");
  fprintf (file,
           "receives a license from the original licensors, to run, modify and\n");
  fprintf (file,
           "propagate that work, subject to this License.  You are not responsible\n");
  fprintf (file,
           "for enforcing compliance by third parties with this License.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  An \"entity transaction\" is a transaction transferring control of an\n");
  fprintf (file,
           "organization, or substantially all assets of one, or subdividing an\n");
  fprintf (file,
           "organization, or merging organizations.  If propagation of a covered\n");
  fprintf (file,
           "work results from an entity transaction, each party to that\n");
  fprintf (file,
           "transaction who receives a copy of the work also receives whatever\n");
  fprintf (file,
           "licenses to the work the party's predecessor in interest had or could\n");
  fprintf (file,
           "give under the previous paragraph, plus a right to possession of the\n");
  fprintf (file,
           "Corresponding Source of the work from the predecessor in interest, if\n");
  fprintf (file,
           "the predecessor has it or can get it with reasonable efforts.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You may not impose any further restrictions on the exercise of the\n");
  fprintf (file,
           "rights granted or affirmed under this License.  For example, you may\n");
  fprintf (file,
           "not impose a license fee, royalty, or other charge for exercise of\n");
  fprintf (file,
           "rights granted under this License, and you may not initiate litigation\n");
  fprintf (file,
           "(including a cross-claim or counterclaim in a lawsuit) alleging that\n");
  fprintf (file,
           "any patent claim is infringed by making, using, selling, offering for\n");
  fprintf (file, "sale, or importing the Program or any portion of it.\n");
  fprintf (file, "\n");
  fprintf (file, "  11. Patents.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A \"contributor\" is a copyright holder who authorizes use under this\n");
  fprintf (file,
           "License of the Program or a work on which the Program is based.  The\n");
  fprintf (file,
           "work thus licensed is called the contributor's \"contributor version\".\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A contributor's \"essential patent claims\" are all patent claims\n");
  fprintf (file,
           "owned or controlled by the contributor, whether already acquired or\n");
  fprintf (file,
           "hereafter acquired, that would be infringed by some manner, permitted\n");
  fprintf (file,
           "by this License, of making, using, or selling its contributor version,\n");
  fprintf (file,
           "but do not include claims that would be infringed only as a\n");
  fprintf (file,
           "consequence of further modification of the contributor version.  For\n");
  fprintf (file,
           "purposes of this definition, \"control\" includes the right to grant\n");
  fprintf (file,
           "patent sublicenses in a manner consistent with the requirements of\n");
  fprintf (file, "this License.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Each contributor grants you a non-exclusive, worldwide, royalty-free\n");
  fprintf (file,
           "patent license under the contributor's essential patent claims, to\n");
  fprintf (file,
           "make, use, sell, offer for sale, import and otherwise run, modify and\n");
  fprintf (file, "propagate the contents of its contributor version.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  In the following three paragraphs, a \"patent license\" is any express\n");
  fprintf (file,
           "agreement or commitment, however denominated, not to enforce a patent\n");
  fprintf (file,
           "(such as an express permission to practice a patent or covenant not to\n");
  fprintf (file,
           "sue for patent infringement).  To \"grant\" such a patent license to a\n");
  fprintf (file,
           "party means to make such an agreement or commitment not to enforce a\n");
  fprintf (file, "patent against the party.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If you convey a covered work, knowingly relying on a patent license,\n");
  fprintf (file,
           "and the Corresponding Source of the work is not available for anyone\n");
  fprintf (file,
           "to copy, free of charge and under the terms of this License, through a\n");
  fprintf (file,
           "publicly available network server or other readily accessible means,\n");
  fprintf (file,
           "then you must either (1) cause the Corresponding Source to be so\n");
  fprintf (file,
           "available, or (2) arrange to deprive yourself of the benefit of the\n");
  fprintf (file,
           "patent license for this particular work, or (3) arrange, in a manner\n");
  fprintf (file,
           "consistent with the requirements of this License, to extend the patent\n");
  fprintf (file,
           "license to downstream recipients.  \"Knowingly relying\" means you have\n");
  fprintf (file,
           "actual knowledge that, but for the patent license, your conveying the\n");
  fprintf (file,
           "covered work in a country, or your recipient's use of the covered work\n");
  fprintf (file,
           "in a country, would infringe one or more identifiable patents in that\n");
  fprintf (file, "country that you have reason to believe are valid.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If, pursuant to or in connection with a single transaction or\n");
  fprintf (file,
           "arrangement, you convey, or propagate by procuring conveyance of, a\n");
  fprintf (file,
           "covered work, and grant a patent license to some of the parties\n");
  fprintf (file,
           "receiving the covered work authorizing them to use, propagate, modify\n");
  fprintf (file,
           "or convey a specific copy of the covered work, then the patent license\n");
  fprintf (file,
           "you grant is automatically extended to all recipients of the covered\n");
  fprintf (file, "work and works based on it.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  A patent license is \"discriminatory\" if it does not include within\n");
  fprintf (file,
           "the scope of its coverage, prohibits the exercise of, or is\n");
  fprintf (file,
           "conditioned on the non-exercise of one or more of the rights that are\n");
  fprintf (file,
           "specifically granted under this License.  You may not convey a covered\n");
  fprintf (file,
           "work if you are a party to an arrangement with a third party that is\n");
  fprintf (file,
           "in the business of distributing software, under which you make payment\n");
  fprintf (file,
           "to the third party based on the extent of your activity of conveying\n");
  fprintf (file,
           "the work, and under which the third party grants, to any of the\n");
  fprintf (file,
           "parties who would receive the covered work from you, a discriminatory\n");
  fprintf (file,
           "patent license (a) in connection with copies of the covered work\n");
  fprintf (file,
           "conveyed by you (or copies made from those copies), or (b) primarily\n");
  fprintf (file,
           "for and in connection with specific products or compilations that\n");
  fprintf (file,
           "contain the covered work, unless you entered into that arrangement,\n");
  fprintf (file,
           "or that patent license was granted, prior to 28 March 2007.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Nothing in this License shall be construed as excluding or limiting\n");
  fprintf (file,
           "any implied license or other defenses to infringement that may\n");
  fprintf (file,
           "otherwise be available to you under applicable patent law.\n");
  fprintf (file, "\n");
  fprintf (file, "  12. No Surrender of Others' Freedom.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If conditions are imposed on you (whether by court order, agreement or\n");
  fprintf (file,
           "otherwise) that contradict the conditions of this License, they do not\n");
  fprintf (file,
           "excuse you from the conditions of this License.  If you cannot convey a\n");
  fprintf (file,
           "covered work so as to satisfy simultaneously your obligations under this\n");
  fprintf (file,
           "License and any other pertinent obligations, then as a consequence you may\n");
  fprintf (file,
           "not convey it at all.  For example, if you agree to terms that obligate you\n");
  fprintf (file,
           "to collect a royalty for further conveying from those to whom you convey\n");
  fprintf (file,
           "the Program, the only way you could satisfy both those terms and this\n");
  fprintf (file,
           "License would be to refrain entirely from conveying the Program.\n");
  fprintf (file, "\n");
  fprintf (file, "  13. Use with the GNU Affero General Public License.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Notwithstanding any other provision of this License, you have\n");
  fprintf (file,
           "permission to link or combine any covered work with a work licensed\n");
  fprintf (file,
           "under version 3 of the GNU Affero General Public License into a single\n");
  fprintf (file,
           "combined work, and to convey the resulting work.  The terms of this\n");
  fprintf (file,
           "License will continue to apply to the part which is the covered work,\n");
  fprintf (file,
           "but the special requirements of the GNU Affero General Public License,\n");
  fprintf (file,
           "section 13, concerning interaction through a network will apply to the\n");
  fprintf (file, "combination as such.\n");
  fprintf (file, "\n");
  fprintf (file, "  14. Revised Versions of this License.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The Free Software Foundation may publish revised and/or new versions of\n");
  fprintf (file,
           "the GNU General Public License from time to time.  Such new versions will\n");
  fprintf (file,
           "be similar in spirit to the present version, but may differ in detail to\n");
  fprintf (file, "address new problems or concerns.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Each version is given a distinguishing version number.  If the\n");
  fprintf (file,
           "Program specifies that a certain numbered version of the GNU General\n");
  fprintf (file,
           "Public License \"or any later version\" applies to it, you have the\n");
  fprintf (file,
           "option of following the terms and conditions either of that numbered\n");
  fprintf (file,
           "version or of any later version published by the Free Software\n");
  fprintf (file,
           "Foundation.  If the Program does not specify a version number of the\n");
  fprintf (file,
           "GNU General Public License, you may choose any version ever published\n");
  fprintf (file, "by the Free Software Foundation.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If the Program specifies that a proxy can decide which future\n");
  fprintf (file,
           "versions of the GNU General Public License can be used, that proxy's\n");
  fprintf (file,
           "public statement of acceptance of a version permanently authorizes you\n");
  fprintf (file, "to choose that version for the Program.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  Later license versions may give you additional or different\n");
  fprintf (file,
           "permissions.  However, no additional obligations are imposed on any\n");
  fprintf (file,
           "author or copyright holder as a result of your choosing to follow a\n");
  fprintf (file, "later version.\n");
  fprintf (file, "\n");
  fprintf (file, "  15. Disclaimer of Warranty.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n");
  fprintf (file,
           "APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n");
  fprintf (file,
           "HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n");
  fprintf (file,
           "OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n");
  fprintf (file,
           "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n");
  fprintf (file,
           "PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n");
  fprintf (file,
           "IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n");
  fprintf (file, "ALL NECESSARY SERVICING, REPAIR OR CORRECTION.\n");
  fprintf (file, "\n");
  fprintf (file, "  16. Limitation of Liability.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING\n");
  fprintf (file,
           "WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS\n");
  fprintf (file,
           "THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY\n");
  fprintf (file,
           "GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE\n");
  fprintf (file,
           "USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF\n");
  fprintf (file,
           "DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD\n");
  fprintf (file,
           "PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),\n");
  fprintf (file,
           "EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF\n");
  fprintf (file, "SUCH DAMAGES.\n");
  fprintf (file, "\n");
  fprintf (file, "  17. Interpretation of Sections 15 and 16.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If the disclaimer of warranty and limitation of liability provided\n");
  fprintf (file,
           "above cannot be given local legal effect according to their terms,\n");
  fprintf (file,
           "reviewing courts shall apply local law that most closely approximates\n");
  fprintf (file,
           "an absolute waiver of all civil liability in connection with the\n");
  fprintf (file,
           "Program, unless a warranty or assumption of liability accompanies a\n");
  fprintf (file, "copy of the Program in return for a fee.\n");
  fprintf (file, "\n");
  fprintf (file, "                     END OF TERMS AND CONDITIONS\n");
  fprintf (file, "\n");
  fprintf (file,
           "            How to Apply These Terms to Your New Programs\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If you develop a new program, and you want it to be of the greatest\n");
  fprintf (file,
           "possible use to the public, the best way to achieve this is to make it\n");
  fprintf (file,
           "free software which everyone can redistribute and change under these terms.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  To do so, attach the following notices to the program.  It is safest\n");
  fprintf (file,
           "to attach them to the start of each source file to most effectively\n");
  fprintf (file,
           "state the exclusion of warranty; and each file should have at least\n");
  fprintf (file,
           "the \"copyright\" line and a pointer to where the full notice is found.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    <one line to give the program's name and a brief idea of what it does.>\n");
  fprintf (file, "    Copyright (C) <year>  <name of author>\n");
  fprintf (file, "\n");
  fprintf (file,
           "    This program is free software: you can redistribute it and/or modify\n");
  fprintf (file,
           "    it under the terms of the GNU General Public License as published by\n");
  fprintf (file,
           "    the Free Software Foundation, either version 3 of the License, or\n");
  fprintf (file, "    (at your option) any later version.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    This program is distributed in the hope that it will be useful,\n");
  fprintf (file,
           "    but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
  fprintf (file,
           "    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
  fprintf (file, "    GNU General Public License for more details.\n");
  fprintf (file, "\n");
  fprintf (file,
           "    You should have received a copy of the GNU General Public License\n");
  fprintf (file,
           "    along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
  fprintf (file, "\n");
  fprintf (file,
           "Also add information on how to contact you by electronic and paper mail.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  If the program does terminal interaction, make it output a short\n");
  fprintf (file, "notice like this when it starts in an interactive mode:\n");
  fprintf (file, "\n");
  fprintf (file, "    <program>  Copyright (C) <year>  <name of author>\n");
  fprintf (file,
           "    This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n");
  fprintf (file,
           "    This is free software, and you are welcome to redistribute it\n");
  fprintf (file,
           "    under certain conditions; type `show c' for details.\n");
  fprintf (file, "\n");
  fprintf (file,
           "The hypothetical commands `show w' and `show c' should show the appropriate\n");
  fprintf (file,
           "parts of the General Public License.  Of course, your program's commands\n");
  fprintf (file,
           "might be different; for a GUI interface, you would use an \"about box\".\n");
  fprintf (file, "\n");
  fprintf (file,
           "  You should also get your employer (if you work as a programmer) or school,\n");
  fprintf (file,
           "if any, to sign a \"copyright disclaimer\" for the program, if necessary.\n");
  fprintf (file,
           "For more information on this, and how to apply and follow the GNU GPL, see\n");
  fprintf (file, "<http://www.gnu.org/licenses/>.\n");
  fprintf (file, "\n");
  fprintf (file,
           "  The GNU General Public License does not permit incorporating your program\n");
  fprintf (file,
           "into proprietary programs.  If your program is a subroutine library, you\n");
  fprintf (file,
           "may consider it more useful to permit linking proprietary applications with\n");
  fprintf (file,
           "the library.  If this is what you want to do, use the GNU Lesser General\n");
  fprintf (file,
           "Public License instead of this License.  But first, please read\n");
  fprintf (file, "<http://www.gnu.org/philosophy/why-not-lgpl.html>.\n");

  return;
}

void
ut_print_bug ()
{
  ut_print_messagewnc (2, 72, "You have discovered a bug!\n");

  return;
}

void
ut_print_neperbug ()
{
  ut_print_messagewnc (2, 72,
                       "You have discovered a bug in Neper!  Please file an issue at https://github.com/rquey/neper/issues.  Thank you.\n");

  return;
}

void
ut_print_exprbug (char *expr)
{
  ut_print_message (2, 0, "Expression `%s' could not be processed.\n", expr);

  return;
}

void
ut_print_elapsedtime (struct timeval beg_time, struct timeval end_time)
{
  double t = ut_time_subtract (&beg_time, &end_time);
  ut_print_message (0, 1, "Elapsed time: %.3f secs.\n", t);

  return;
}
