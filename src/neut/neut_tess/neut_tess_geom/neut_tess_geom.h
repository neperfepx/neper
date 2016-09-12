/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_GEOM_H
#define NEUT_TESS_GEOM_H

/// \brief Get the centre of an edge
///
///
///
  extern int neut_tess_edge_centre (struct TESS Tess, int, double *);
  extern int neut_tess_edge_centre_x (struct TESS Tess, int, double *);
  extern int neut_tess_edge_centre_y (struct TESS Tess, int, double *);
  extern int neut_tess_edge_centre_z (struct TESS Tess, int, double *);

  extern int neut_tess_cell_size (struct TESS Tess, int cell, double *psize);

/// \brief Get the equation of an edge (for a 2D tess)
///
///
///
  extern int neut_tess_edge_eq (struct TESS Tess, int, double *);

/// \brief Get the equation of a domain edge (for a 2D tess)
///
///
///
  extern int neut_tess_domedge_eq (struct TESS Tess, int, double *);

  extern int neut_tess_domedge_length (struct TESS, int, double *pval);

/// \brief Get the area of a face
///
///
///
  extern int neut_tess_face_area (struct TESS Tess, int, double *);

/// \brief Get the area of a set of faces
///
///
///
  extern int neut_tess_faces_area (struct TESS Tess, int *, int, double *);

  extern void neut_tess_face_centre_x (struct TESS Tess, int, double *);
  extern void neut_tess_face_centre_y (struct TESS Tess, int, double *);
  extern void neut_tess_face_centre_z (struct TESS Tess, int, double *);

/// \brief Get the centre of a face
///
///
///
  extern void neut_tess_face_centre (struct TESS Tess, int, double *);

/// \brief Get the volume of a polyhedron
///
///
///
  extern int neut_tess_poly_volume (struct TESS Tess, int, double *);

/// \brief Get the centroid of a polyhedron
///
///
///
  extern int neut_tess_poly_centroid (struct TESS Tess, int, double *);

/// \brief Get the centroid of the vertices of a polyhedron
///
///
///
  extern int neut_tess_poly_vercentroid (struct TESS Tess, int, double *);

/// \brief Get the centroid of a cell
///
///
///
  extern int neut_tess_cell_centroid (struct TESS Tess, int cell, double
				      *coo);

/// \brief Get the size of a TESS
///
///
///
  extern int neut_tess_size (struct TESS Tess, double *);

/// \brief Get the average cell equivalent diameter of a TESS
///
///
///
  extern void neut_tess_cellavdiameq (struct TESS Tess,
				      int CellQty, double *pdiameq);
  extern void neut_tess_cellavradeq  (struct TESS Tess,
				      int CellQty, double *pradeq);

/// \brief Get the average cell size of a TESS
///
///
///
  extern void neut_tess_cellavsize (struct TESS Tess,
				    int CellQty, double *psize);

/// \brief Get the volume of a TESS
///
///
///
  extern int neut_tess_volume (struct TESS Tess, double *);

/// \brief Get the area of a TESS
///
///
///
  extern int neut_tess_area (struct TESS Tess, double *);

/// \brief Get the length of a TESS
///
///
///
  extern int neut_tess_length (struct TESS Tess, double *);

/// \brief Get the area of a domain face of a TESS
///
///
///
  extern int neut_tess_domface_area (struct TESS Tess, int domface, double *);

  extern int neut_tess_domface_label_area (struct TESS Tess, char *domface,
					   double *parea);

/// \brief Get the centre of a TESS
///
///
///
  extern void neut_tess_centre (struct TESS Tess, double *);

/// \brief Get the bounding box of a TESS
///
///
///
  extern void neut_tess_bbox (struct TESS Tess, double **);

/// \brief Get the bounding box of a TESS
///
///
///
  extern void neut_tess_edge_bbox (struct TESS Tess, int edge, double **size);

/// \brief Determine whether a point is within a cell
///
///
///
  extern int neut_tess_point_incell (struct TESS Tess, double *, int);

/// \brief Determine whether a point is within a polyhedron
///
///
///
  extern int neut_tess_point_inpoly (struct TESS Tess, double *, int);

/// \brief Determine whether a point is within a face
///
///
///
  extern int neut_tess_point_inface (struct TESS Tess, double *, int);

  extern int neut_tess_point_inedge (struct TESS Tess, double *, int);

  extern void rcl2cl (double, int, double, int, char *, double *);
  extern void rsel2sel (double, int, double, int, double *);

  extern void neut_tess_face_normal_fromver (struct TESS Tess, int face,
					     double *n);

  extern void neut_tess_face_dihangle (struct TESS, int face, int id,
				       double *pangle);
  extern void neut_tess_face_dihangles (struct TESS Tess, int face,
					double **pangle, int *pqty);

  extern void neut_tess_poly_dihangles (struct TESS Tess, int poly,
					double **pangle, int *pqty);

  extern void neut_tess_face_radeq (struct TESS Tess, int face,
				     double *pval);
  extern void neut_tess_poly_radeq (struct TESS Tess, int poly,
				     double *pval);
  extern void neut_tess_cell_radeq (struct TESS Tess, int cell,
				     double *pval);
  extern void neut_tess_face_diameq (struct TESS Tess, int face,
				     double *pval);
  extern void neut_tess_poly_diameq (struct TESS Tess, int poly,
				     double *pval);
  extern void neut_tess_cell_diameq (struct TESS Tess, int cell,
				     double *pval);
  extern void neut_tess_face_perimeter (struct TESS Tess, int face,
					double *pval);
  extern void neut_tess_poly_area (struct TESS Tess, int poly, double *pval);
  extern int neut_tess_face_circularity (struct TESS, int face, double *pval);
  extern int neut_tess_poly_sphericity (struct TESS, int poly, double *pval);
  extern int neut_tess_cell_sphericity (struct TESS, int cell, double *pval);

  extern int neut_tess_cell_planeside (struct TESS Tess, int cell,
				       double *plane, int *pside);

  extern void neut_tess_face_convexity (struct TESS Tess, int face,
					double *pval);
  extern void neut_tess_poly_convexity (struct TESS Tess, int poly,
					double *pval);
  extern void neut_tess_cell_convexity (struct TESS Tess, int cell,
					double *pval);

#endif				/* NEUT_TESS_GEOM_H */

#ifdef __cplusplus
}
#endif
