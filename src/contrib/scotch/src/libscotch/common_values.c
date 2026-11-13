/* Copyright 2021,2023,2025 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : common_values.c                         **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module handles the context values  **/
/**                management routines.                    **/
/**                                                        **/
/**   DATES      : # Version 7.0  : from : 09 sep 2021     **/
/**                                 to   : 30 aug 2025     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#include "module.h"
#include "common.h"
#include "common_values.h"

/************************************/
/*                                  */
/* These routines handle the values */
/* features of contexts.            */
/*                                  */
/************************************/

/* This routine initializes a values context
** in the given context.
** It returns:
** - 0   : if the values context has been created.
** - !0  : else.
*/

int
contextValuesInit (
Context * const             contptr,
void * const                dataptr,
const size_t                datasiz,
const int                   vintnbr,
const size_t                ointval,
const int                   vdblnbr,
const size_t                odblval)
{
  ValuesContext * restrict  valuptr;

  if (contptr->valuptr == NULL) {
    if ((contptr->valuptr = memAlloc (sizeof (ValuesContext))) == NULL) {
      errorPrint ("contextValuesInit: out of memory");
      return (1);
    }
  }

  valuptr = contptr->valuptr;

  valuptr->dainptr =                              /* Initialize values context area, pointing to default values array */
  valuptr->dataptr = dataptr;
  valuptr->datasiz = datasiz;
  valuptr->vintnbr = vintnbr;
  valuptr->ointval = ointval;
  valuptr->vdblnbr = vdblnbr;
  valuptr->odblval = odblval;

  return (0);
}

/* These routines get a value of the said
** type from the given context.
** They return:
** - 0   : if the value was obtained.
** - !0  : invalid value number.
*/

int
contextValuesGetDbl (
Context * const             contptr,
const int                   valunum,
double * const              vdblptr)
{
  const ValuesContext * restrict const  valuptr = contptr->valuptr;

  if ((valunum <  0) ||                           /* If invalid value number */
      (valunum >= valuptr->vdblnbr))
    return (1);

  *vdblptr = ((double *) ((byte *) valuptr->dataptr + valuptr->odblval))[valunum];

  return (0);
}

int
contextValuesGetInt (
Context * const             contptr,
const int                   valunum,
INT * const                 vintptr)
{
  const ValuesContext * restrict const  valuptr = contptr->valuptr;

  if ((valunum <  0) ||                           /* If invalid value number */
      (valunum >= valuptr->vintnbr))
    return (1);

  *vintptr = ((INT *) ((byte *) valuptr->dataptr + valuptr->ointval))[valunum];

  return (0);
}

/* This routine sets an interger option value
** in the given context.
** It returns:
** - 0   : if the value was set.
** - !0  : on error.
*/

static
int
contextValuesAllocate (
Context * const             contptr)
{
  ValuesContext * restrict const    valuptr = contptr->valuptr;

  if (valuptr->dataptr == valuptr->dainptr) {     /* If current value array is default array, allocate a new one */
    void *              dataptr;

    if ((dataptr = (void *) memAlloc (valuptr->datasiz)) == NULL) /* If cannot allocate new array */
      return (1);

    memCpy (dataptr, valuptr->dainptr, valuptr->datasiz); /* Initialize new values array */
    valuptr->dataptr = dataptr;
  }

  return (0);
}

int
contextValuesSetDbl (
Context * const             contptr,
const int                   valunum,
const double                vdblval)
{
  if ((valunum <  0) ||                           /* If invalid value number */
      (valunum >= contptr->valuptr->vdblnbr))
    return (1);

  if (((double *) ((byte *) contptr->valuptr->dataptr + contptr->valuptr->odblval))[valunum] == vdblval) /* If nothing to do */
    return (0);

  if (contextValuesAllocate (contptr) != 0)       /* If current value array is default array, allocate a new one */
    return (1);

  ((double *) ((byte *) contptr->valuptr->dataptr + contptr->valuptr->odblval))[valunum] = vdblval;

  return (0);
}

int
contextValuesSetInt (
Context * const             contptr,
const int                   valunum,
const INT                   vintval)
{
  if ((valunum <  0) ||                           /* If invalid value number */
      (valunum >= contptr->valuptr->vintnbr))
    return (1);

  if (((INT *) ((byte *) contptr->valuptr->dataptr + contptr->valuptr->ointval))[valunum] == vintval) /* If nothing to do */
    return (0);

  if (contextValuesAllocate (contptr) != 0)       /* If current value array is default array, allocate a new one */
    return (1);

  ((INT *) ((byte *) contptr->valuptr->dataptr + contptr->valuptr->ointval))[valunum] = vintval;

  return (0);
}
