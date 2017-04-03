/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_merge_polys_.h"

void
neut_tess_merge_polys (struct TESS *pTess, int *polys, int polyqty)
{
  int newpoly;
  int delpolyqty, *delpolys = NULL;
  int keepfaceqty, *keepfaces = NULL;
  int delfaceqty, *delfaces = NULL;
  int keepedgeqty, *keepedges = NULL;
  int deledgeqty, *deledges = NULL;
  int keepverqty, *keepvers = NULL;
  int delverqty, *delvers = NULL;

  if (polyqty == 1)
    return;

  neut_tess_merge_polys_polylists (pTess, polys, polyqty,
				   &newpoly, &delpolys, &delpolyqty);

  neut_tess_merge_polys_facelists (pTess, polys, polyqty,
				   &keepfaces, &keepfaceqty,
				   &delfaces, &delfaceqty);

  neut_tess_merge_polys_edgelists (pTess, delfaces, delfaceqty,
				   &keepedges, &keepedgeqty,
				   &deledges, &deledgeqty);

  neut_tess_merge_faces_verlists (pTess,
				  deledges, deledgeqty,
				  &keepvers, &keepverqty,
				  &delvers, &delverqty);

  neut_tess_merge_polys_mergepolys (pTess, newpoly,
				    delpolys, delpolyqty,
				    keepfaces, keepfaceqty);

  neut_tess_merge_polys_updatefaces (pTess, delfaces, delfaceqty);

  neut_tess_merge_polys_updateedges (pTess, delfaces, delfaceqty,
				     keepedges, keepedgeqty,
				     deledges, deledgeqty);

  neut_tess_merge_faces_updatevers (pTess,
				    deledges, deledgeqty,
				    keepvers, keepverqty, delvers, delverqty);

  ut_free_1d_int_ (&delfaces);
  ut_free_1d_int_ (&keepfaces);
  ut_free_1d_int_ (&delfaces);
  ut_free_1d_int_ (&keepedges);
  ut_free_1d_int_ (&deledges);
  ut_free_1d_int_ (&keepvers);
  ut_free_1d_int_ (&delvers);

  return;
}
