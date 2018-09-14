/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_faces_merge_.h"

void
neut_tess_faces_merge (struct TESS *pTess, int *faces, int faceqty)
{
  int newface;
  int delfaceqty, *delfaces = NULL;
  int keepedgeqty, *keepedges = NULL;
  int deledgeqty, *deledges = NULL;
  int keepverqty, *keepvers = NULL;
  int delverqty, *delvers = NULL;

  if (faceqty == 1)
    return;

  neut_tess_faces_merge_facelists (pTess, faces, faceqty,
				   &newface, &delfaces, &delfaceqty);

  neut_tess_faces_merge_edgelists (pTess, faces, faceqty,
				   &keepedges, &keepedgeqty,
				   &deledges, &deledgeqty);

  neut_tess_faces_merge_verlists (pTess,
				  deledges, deledgeqty,
				  &keepvers, &keepverqty,
				  &delvers, &delverqty);

  neut_tess_faces_merge_mergefaces (pTess, newface,
				    delfaces, delfaceqty,
				    keepedges, keepedgeqty);

  neut_tess_faces_merge_updateedges (pTess, newface,
				     delfaces, delfaceqty,
				     keepedges, keepedgeqty,
				     deledges, deledgeqty);

  neut_tess_faces_merge_updatevers (pTess,
				    deledges, deledgeqty,
				    keepvers, keepverqty, delvers, delverqty);

  ut_free_1d_int_ (&delfaces);
  ut_free_1d_int_ (&keepedges);
  ut_free_1d_int_ (&deledges);
  ut_free_1d_int_ (&keepvers);
  ut_free_1d_int_ (&delvers);

  return;
}
