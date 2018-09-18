/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_OP_H
#define NEUT_TESS_OP_H

/// \brief Set a TESS structure to zero
///
///
///
  extern void neut_tess_set_zero (struct TESS *pTess);
  extern void neut_tess_set_zero_domain (struct TESS *pTess);

/// \brief Free a TESS structure
///
///
///
  extern void neut_tess_free (struct TESS *pTess);
  extern void neut_tess_free_domain (struct TESS *pTess);

/// \brief Alloc a TESS structure
///
///
///
  extern void neut_tess_alloc (struct TESS *pTess, int VerQty,
			       int EdgeQty, int FaceQty, int PolyQty);

/// \brief Scale a TESS in the 3 directions of space
///
///
  extern void neut_tess_scale (struct TESS *pTess, double x, double y,
			       double z);

  extern void neut_tess_shift (struct TESS *pTess, double sx, double sy,
			       double sz);
  extern void neut_tess_rotate (struct TESS *pTess, double **g);

/// \brief Switch two polyhedra of a TESS
///
///
  extern void neut_tess_poly_switch (struct TESS *, int poly1, int poly2);

/// \brief Initialize the true level of the polyhedra of a TESS
///
///
///
  extern void neut_tess_init_celltrue (struct TESS *pTess);

/// \brief Initialize the body level of the polyhedra of a TESS
///
///
///
  extern void neut_tess_init_cellbody (struct TESS *pTess);

/// \brief Initialize the lengths of the edges of a TESS
///
///
///
  extern void neut_tess_init_edgelength (struct TESS *pTess);

/// \brief Initialize the length of an edge of a TESS
///
///
///
  extern void neut_tess_init_edgelength_edge (struct TESS *pTess, int edge);

/// \brief Initialize PolyQty, PolyFaceQty and PolyFaceNb from FacePoly
///
///
///
  extern void neut_tess_init_polytopo_fromfacepoly (struct TESS *pTess);

/// \brief Initialize EdgeVerNb and EdgeLength from VerQty, VerEdgeQty and VerEdgeNb
///
///
///
  extern void neut_tess_init_edgetopo_fromver (struct TESS *pTess);

/// \brief Initialize EdgeVerNb and EdgeLength from VerQty, VerEdgeQty and VerEdgeNb
///
///
///
  extern void neut_tess_init_facetopo_fromedge (struct TESS *pTess);

/// \brief Initialize FaceEdgeOri and FaceVerNb; permuting FaceEdgeNb; initialize FacePt, FacePtCoo and FaceFF
///
///
///
  extern void neut_tess_init_facetopo_fromver (struct TESS *pTess);

/// \brief Initialize BoundingBox
///
///
///
  extern void neut_tess_init_bbox (struct TESS *pTess);

  extern void neut_tess_init_verdom (struct TESS *pTess);
  extern void neut_tess_init_edgedom (struct TESS *pTess);
  extern void neut_tess_init_facedom (struct TESS *pTess);

/// \brief Initialize FaceDom
///
/// Requirements: uses FacePoly
///
/// Returned value: amount of domain faces
///
  extern int neut_tess_init_facedom_fromfacepoly (struct TESS *pTess);

/// \brief Initialize FaceDom of a face using VerDom
///
/// Requirements: uses VerDom (and the TESS non-domain variables)
///
  extern void neut_tess_init_facedom_face_v (struct TESS *pTess, int face);

  extern int neut_tess_init_edgedom_fromfacedom (struct TESS *pTess);
  extern int neut_tess_init_edgedom_fromedgeface (struct TESS *pTess);

/// \brief Initialize VerDom
///
/// Requirements: uses the Tess (incl. Domain) definition and FaceDom
///
///
  extern int neut_tess_init_verdom_fromfacedom (struct TESS *pTess);
  extern int neut_tess_init_verdom_fromedgedom (struct TESS *pTess);
  extern int neut_tess_init_verdom_fromveredge (struct TESS *pTess);

/// \brief Initialize the face flatness fault of a face
///
///
///
  extern double neut_tess_face_ff (struct TESS Tess, int face);

/// \brief Initialize DomTessFaceQty and DomTessFaceNb
///
/// Uses FaceDom
///
  extern void neut_tess_init_domtessface (struct TESS *pTess);

/// \brief Initialize DomTessEdgeQty and DomTessEdgeNb
///
/// Uses EdgeDom
///
  extern void neut_tess_init_domtessedge (struct TESS *pTess);

/// \brief Initialize DomTessVerNb
///
/// Uses VerDom
///
  extern void neut_tess_init_domtessver (struct TESS *pTess);

/// \brief Initialize the domain of a TESS
///
/// Input requirements: pointer to a TESS structure whose all non-domain
/// variables have been initialized.
///
/// Output: pointer to a TESS structure whose all *Dom* parameters are initialized.
///
  extern void neut_tess_init_domain (struct TESS *pTess);
  extern void neut_tess_init_domain_3d (struct TESS *pTess);
  extern void neut_tess_init_domain_2d (struct TESS *pTess);
  extern void neut_tess_init_domain_1d (struct TESS *pTess);

/// \brief Initialize the domain of a TESS using a POLY
///
///
///
  extern void neut_tess_init_domain_poly (struct TESS *pTess, struct TESS
					  Tess, int poly, int **,
					  int **, int **);

/// \brief Initialize the domain of a TESS using a POLY: simple copy, no initialization of the Tess to Domain variables (*Dom and DomTess*)
///
///
///
  extern void neut_tess_init_domain_memcpy (struct TESS *pTess,
					    struct TESS Domain);

  extern void neut_tess_domain_tess (struct TESS Tess, struct TESS *pDomTess);

/// \brief Initialize the face labels of the domain of a TESS.
///
///
///
  extern void neut_tess_init_domain_label (struct TESS *pTess);
  extern void neut_tess_init_domain_label_3d (struct TESS *pTess);
  extern void neut_tess_init_domain_label_2d (struct TESS *pTess);
  extern void neut_tess_init_domain_label_1d (struct TESS *pTess);

  extern void neut_tess_cat (struct TESS *pTessA, struct TESS TessB);

  extern void neut_tess_poly_tess (struct TESS Tess, int poly, struct
				   TESS *pPoly);

  extern int neut_tess_addver (struct TESS *pTess, double *coo);
  extern int neut_tess_addedge (struct TESS *pTess, int ver1, int ver2);
  extern int neut_tess_addface_alloc (struct TESS *pTess);
  extern int neut_tess_addpoly_alloc (struct TESS *pTess);
  extern int neut_tess_addcell_alloc (struct TESS *pTess);
  extern int neut_tess_adddomver (struct TESS *pTess, double *coo);
  extern int neut_tess_adddomedge (struct TESS *pTess, int, int);
  extern int neut_tess_adddomface_alloc (struct TESS *pTess);
  extern void neut_tess_face_addeq (struct TESS *pTess, int face, double *eq);
  extern void neut_tess_face_addedge (struct TESS *pTess, int face, int
				      edge, int ori);
  extern void neut_tess_face_addedges (struct TESS *pTess, int face,
				       int *edges, int edgeqty);
  extern void neut_tess_face_setedges (struct TESS *pTess, int face,
				       int *edges, int edgeqty,
                                       int firstedgeori);
  extern void neut_tess_cell_addcentre (struct TESS *pTess, int cell,
					double *centre, double weight);
  extern void neut_tess_poly_addface (struct TESS *pTess, int poly, int
				      face, int ori);
  extern void neut_tess_poly_rmface (struct TESS *pTess, int poly, int face);
  extern void neut_tess_face_changepoly (struct TESS *pTess, int face, int poly1,
					 int poly2);
  extern void neut_tess_init_veredge (struct TESS *pTess);
  extern void neut_tess_init_edgeface (struct TESS *pTess);
  extern void neut_tess_init_facepoly (struct TESS *pTess);
  extern int neut_tess_init_alldom_fromdomtess (struct TESS *pTess);

  extern void neut_tess_face_fixorifromedges (struct TESS *pTess, int face);

  extern void neut_tess_init_domain_domveredge (struct TESS *pTess);
  extern void neut_tess_init_domain_domedgeface (struct TESS *pTess);

  extern void neut_tess_domface_tess (struct TESS Tess, int domface,
				      struct TESS *pT);
  extern void neut_tess_3dcolumnar_2d (struct TESS Tess, struct TESS *pT);
  extern void neut_tess_init_domfaceeq (struct TESS *pTess);

  extern void neut_tess_init_seedslave (struct TESS *pTess);
  extern void neut_tess_init_faceslave (struct TESS *pTess);
  extern void neut_tess_init_edgeslave (struct TESS *pTess);
  extern void neut_tess_init_verslave (struct TESS *pTess);

  extern void neut_tess_init_facepoly_per (struct TESS *pTess);
  extern void neut_tess_init_edgefacenb_per (struct TESS *pTess);

  extern void neut_tess_init_seeds_fromcell (struct TESS *pTess);

#endif				/* NEUT_TESS_OP_H */

#ifdef __cplusplus
}
#endif
