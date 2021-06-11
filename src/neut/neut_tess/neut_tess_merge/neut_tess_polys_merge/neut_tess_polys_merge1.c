/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_polys_merge_.h"

int
neut_tess_polys_merge (struct TESS *pTess, int *polys, int polyqty)
{
  int newpoly;
  int delpolyqty, *delpolys = NULL;
  int keepfaceqty, *keepfaces = NULL;
  int delfaceqty, *delfaces = NULL;
  int keepedgeqty, *keepedges = NULL;
  int deledgeqty, *deledges = NULL;
  int keepverqty, *keepvers = NULL;
  int delverqty, *delvers = NULL;

  if (polyqty <= 1)
    return (polyqty == 1) ? polys[0] : -1;

  neut_tess_polys_merge_polylists (pTess, polys, polyqty, &newpoly, &delpolys,
                                   &delpolyqty);

  neut_tess_polys_merge_facelists (pTess, polys, polyqty, &keepfaces,
                                   &keepfaceqty, &delfaces, &delfaceqty);

  neut_tess_polys_merge_edgelists (pTess, delfaces, delfaceqty, &keepedges,
                                   &keepedgeqty, &deledges, &deledgeqty);

  neut_tess_faces_merge_verlists (pTess, deledges, deledgeqty, &keepvers,
                                  &keepverqty, &delvers, &delverqty);

  neut_tess_polys_merge_mergepolys (pTess, newpoly, delpolys, delpolyqty,
                                    keepfaces, keepfaceqty);

  neut_tess_polys_merge_updatefaces (pTess, delfaces, delfaceqty);

  neut_tess_polys_merge_updateedges (pTess, delfaces, delfaceqty, keepedges,
                                     keepedgeqty, deledges, deledgeqty);

  neut_tess_faces_merge_updatevers (pTess, deledges, deledgeqty, keepvers,
                                    keepverqty, delvers, delverqty);

  ut_free_1d_int (&delfaces);
  ut_free_1d_int (&keepfaces);
  ut_free_1d_int (&delfaces);
  ut_free_1d_int (&keepedges);
  ut_free_1d_int (&deledges);
  ut_free_1d_int (&keepvers);
  ut_free_1d_int (&delvers);

  return newpoly;
}
