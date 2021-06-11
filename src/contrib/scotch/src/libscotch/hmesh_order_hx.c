/* Copyright 2004,2007,2018-2020 IPB, Universite de Bordeaux, INRIA & CNRS
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
/**   NAME       : hmesh_order_hx.c                        **/
/**                                                        **/
/**   AUTHOR     : Francois PELLEGRINI                     **/
/**                                                        **/
/**   FUNCTION   : This module contains service routines   **/
/**                for the hmeshOrderH{d|f} ordering       **/
/**                routines.                               **/
/**                                                        **/
/**   DATES      : # Version 4.0  : from : 09 dec 2003     **/
/**                                 to   : 24 jan 2004     **/
/**                # Version 6.0  : from : 30 apr 2018     **/
/**                                 to   : 25 jun 2018     **/
/**                # Version 6.1  : from : 29 oct 2019     **/
/**                                 to   : 26 jan 2020     **/
/**                                                        **/
/************************************************************/

/*
**  The defines and includes.
*/

#define HMESH_ORDER_HX

#include "module.h"
#include "common.h"
#include "graph.h"
#include "mesh.h"
#include "hmesh.h"
#include "hmesh_order_hx.h"

/***********************************/
/*                                 */
/* These are the service routines. */
/*                                 */
/***********************************/

/* This routine fills the input arrays for
** the mesh ordering routines.
** It returns:
** - void  : in all cases.
*/

int
hmeshOrderHxFill (
const Hmesh * restrict const  meshptr,
Gnum * restrict const         petab,
Gnum * restrict const         lentab,
Gnum * restrict const         iwtab,
Gnum * restrict const         nvtab,
Gnum * restrict const         elentab,
Gnum * restrict const         pfreptr)
{
  HmeshOrderHxHash * restrict hashtab;            /* Neighbor hash table */
  Gnum                        hashsiz;
  Gnum                        hashmsk;
  Gnum                        n;                  /* Number of nodes to order             */
  Gnum                        norig;              /* Sum of degrees                       */
  Gnum                        velmadj;            /* Index adjustment for element indices */
  Gnum                        vnodadj;            /* Index adjustment for node indices    */
  Gnum                        velmnum;
  Gnum                        vnodnum;
  Gnum                        degrval;
  Gnum                        vertnum;
  Gnum                        edgenum;

  Gnum * restrict const       petax   = petab   - 1; /* Base HAMF arrays at base 1 */
  Gnum * restrict const       iwtax   = iwtab   - 1;
  Gnum * restrict const       lentax  = lentab  - 1;
  Gnum * restrict const       nvtax   = nvtab   - 1;
  Gnum * restrict const       elentax = elentab - 1;
  const Gnum * restrict const verttax = meshptr->m.verttax;
  const Gnum * restrict const vendtax = meshptr->m.vendtax;
  const Gnum * restrict const velotax = meshptr->m.velotax;
  const Gnum * restrict const edgetax = meshptr->m.edgetax;

  n     = meshptr->m.velmnbr + meshptr->m.vnodnbr;
  norig = meshptr->m.velmnbr + meshptr->m.vnodnbr; /* Elements always have load 1                        */
  for (hashsiz = 16, degrval = meshptr->m.degrmax * (meshptr->m.degrmax - 1); /* Compute hash table size */
       hashsiz < degrval; hashsiz <<= 1) ;
  hashsiz <<= 1;
  hashmsk   = hashsiz - 1;

  if ((hashtab = memAlloc (hashsiz * sizeof (HmeshOrderHxHash))) == NULL) {
    errorPrint  ("hmeshOrderHxFill: out of memory");
    return      (1);
  }
  memSet (hashtab, ~0, hashsiz * sizeof (HmeshOrderHxHash));

  velmadj = 1 + meshptr->m.vnodnbr - meshptr->m.velmbas;
  for (vnodnum = meshptr->m.vnodbas, vertnum = edgenum = 1; /* Copy non-halo node data with base 1 */
       vnodnum < meshptr->vnohnnd; vertnum ++, vnodnum ++) {
    Gnum                      enodnum;
    Gnum                      nghbnbr;

    petax[vertnum]  = edgenum;
    lentax[vertnum] = vendtax[vnodnum] - verttax[vnodnum];
    nvtax[vertnum]  = (velotax != NULL) ? velotax[vnodnum] : 1;

    for (enodnum = verttax[vnodnum], nghbnbr = -1; /* -1 since loop edge will be processed in the main loop */
         enodnum < vendtax[vnodnum]; enodnum ++) {
      Gnum                      velmnum;
      Gnum                      eelmnum;

      velmnum = edgetax[enodnum];

      iwtax[edgenum ++] = velmnum + velmadj;      /* Adjust end element index */
      for (eelmnum = verttax[velmnum]; eelmnum < vendtax[velmnum]; eelmnum ++) {
        Gnum                      vnodend;
        Gnum                      hnodend;

        vnodend = edgetax[eelmnum];

        for (hnodend = (vnodend * HMESHORDERHXHASHPRIME) & hashmsk; ; hnodend = (hnodend + 1) & hashmsk) {
          if (hashtab[hnodend].vertnum != vnodnum) {
            hashtab[hnodend].vertnum = vnodnum;
            hashtab[hnodend].vertend = vnodend;
            nghbnbr ++;
          }
          if (hashtab[hnodend].vertend == vnodend) /* If end vertex already present */
            break;                                /* Skip to next end vertex        */
        }
      }
      elentax[vertnum] = nghbnbr;
    }
  }

  for ( ; vnodnum < meshptr->m.vnodnnd; vnodnum ++, vertnum ++) { /* Copy halo vertices with base 1 */
    Gnum                      degrval;
    Gnum                      enodnum;

    degrval = verttax[vnodnum] - vendtax[vnodnum]; /* Compute opposite of degree */
    petax[vertnum]   = edgenum;
    lentax[vertnum]  = (degrval != 0) ? degrval : (-1 - norig);
    elentax[vertnum] = 0;
    nvtax[vertnum]   = (velotax != NULL) ? velotax[vnodnum] : 1;

    for (enodnum = verttax[vnodnum];
         enodnum < vendtax[vnodnum]; enodnum ++)
      iwtax[edgenum ++] = edgetax[enodnum] + velmadj; /* Adjust end element index */
  }

  vnodadj = 1 - meshptr->m.vnodbas;               /* Base nodes at 1 */
  for (velmnum = meshptr->m.velmbas; velmnum < meshptr->m.velmnnd; velmnum ++, vertnum ++) {
    Gnum                      eelmnum;

    petax[vertnum]   = edgenum;
    lentax[vertnum]  = meshptr->m.vendtax[velmnum] - meshptr->m.verttax[velmnum];
    elentax[vertnum] = - (n + 1);
    nvtax[vertnum]   = 1;                         /* Elements always have weight 1 */

    for (eelmnum = verttax[velmnum];
         eelmnum < vendtax[velmnum]; eelmnum ++)
      iwtax[edgenum ++] = edgetax[eelmnum] + vnodadj; /* Adjust end node index */
  }

  *pfreptr = edgenum;                             /* Set index to first free area */

  memFree (hashtab);

  return (0);
}
