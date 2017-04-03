/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_merge_faces_.h"

void
neut_tess_merge_faces (struct TESS *pTess, int *faces, int faceqty)
{
  int newface;
  int delfaceqty, *delfaces = NULL;
  int keepedgeqty, *keepedges = NULL;
  int deledgeqty, *deledges = NULL;
  int keepverqty, *keepvers = NULL;
  int delverqty, *delvers = NULL;

  if (faceqty == 1)
    return;

  neut_tess_merge_faces_facelists (pTess, faces, faceqty,
				   &newface, &delfaces, &delfaceqty);

  neut_tess_merge_faces_edgelists (pTess, faces, faceqty,
				   &keepedges, &keepedgeqty,
				   &deledges, &deledgeqty);

  neut_tess_merge_faces_verlists (pTess,
				  deledges, deledgeqty,
				  &keepvers, &keepverqty,
				  &delvers, &delverqty);

  neut_tess_merge_faces_mergefaces (pTess, newface,
				    delfaces, delfaceqty,
				    keepedges, keepedgeqty);

  neut_tess_merge_faces_updateedges (pTess, newface,
				     delfaces, delfaceqty,
				     keepedges, keepedgeqty,
				     deledges, deledgeqty);

  neut_tess_merge_faces_updatevers (pTess,
				    deledges, deledgeqty,
				    keepvers, keepverqty, delvers, delverqty);

  ut_free_1d_int_ (&delfaces);
  ut_free_1d_int_ (&keepedges);
  ut_free_1d_int_ (&deledges);
  ut_free_1d_int_ (&keepvers);
  ut_free_1d_int_ (&delvers);

  return;
}
