/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_GEOM_H
#define NEUT_MESH_GEOM_H

/// \brief Get the centre of an elt of a mesh.
///
///
///
  extern void neut_mesh_elt_centre (struct NODES Nodes, struct MESH Mesh,
				    int elt, double *coo);
  extern void neut_mesh_elt_centre_x (struct NODES Nodes, struct MESH Mesh,
				      int elt, double *coo);
  extern void neut_mesh_elt_centre_y (struct NODES Nodes, struct MESH Mesh,
				      int elt, double *coo);
  extern void neut_mesh_elt_centre_z (struct NODES Nodes, struct MESH Mesh,
				      int elt, double *coo);

/// \brief Get the length of an elt of a 1D mesh.
///
///
///
  extern int neut_mesh_elt_length (struct NODES Nodes, struct MESH Mesh,
				   int elt, double *plength);

/// \brief Get the average, min and max lengths of an elt of a 2D or 3D mesh.
///
///
///
  extern void neut_mesh_elt_lengths (struct NODES Nodes, struct MESH Mesh,
				     int elt, double *pavlength,
				     double *pminlength, double *pmaxlength);

/// \brief Get the area of an elt of a 2D mesh.
///
///
///
  extern int neut_mesh_elt_area (struct NODES Nodes, struct MESH Mesh,
				 int elt, double *parea);

/// \brief Get the volume of an elt of a 3D mesh.
///
///
///
  extern int neut_mesh_elt_volume (struct NODES Nodes, struct MESH Mesh,
				   int elt, double *pvol);

/// \brief Get the normal of an elt of a 2D mesh.
///
///
///
  extern void neut_mesh_elt_normal (struct MESH Mesh, struct NODES Nodes,
				    int elt, double *n);

/// \brief Get the normal at a node of a 2D mesh.
///
///
///
  extern void neut_mesh_node_normal (struct MESH Mesh, struct NODES Nodes,
				     int node, double *n);

/// \brief Get the equation of an elt of a 2D mesh.
///
///
///
  extern void neut_mesh_elt_eq (struct MESH Mesh, struct NODES Nodes, int elt,
				double *eq);

/// \brief Get the equation of an elset of a 2D mesh.
///
/// The equation is the average of the equations of the elts, weighted
/// by their respective areas.
///
  extern void neut_mesh_elset_eq (struct MESH Mesh, struct NODES Nodes,
				  int elset, double *eq);

/// \brief Get the radius ratio of an elt of a 3D mesh.
///
///
///
  extern int neut_mesh_elt_rr (struct NODES Nodes, struct MESH Mesh, int elt,
			       double *prr);

/// \brief Get the centre of an elset of a 3D mesh.
///
///
///
  extern int neut_mesh_elset_centre (struct NODES Nodes, struct MESH Mesh,
				     int elset, double *centre);
  extern int neut_mesh_elset_centre_x (struct NODES Nodes, struct MESH Mesh,
				       int elset, double *px);
  extern int neut_mesh_elset_centre_y (struct NODES Nodes, struct MESH Mesh,
				       int elset, double *py);
  extern int neut_mesh_elset_centre_z (struct NODES Nodes, struct MESH Mesh,
				       int elset, double *pz);

  extern int neut_mesh_elsets_centre (struct NODES Nodes, struct MESH Mesh,
				      int *elsets, int elsetqty,
				      double *centre);

/// \brief Get the volume of an elset of a 3D mesh.
///
///
///
  extern int neut_mesh_elset_volume (struct NODES Nodes, struct MESH Mesh,
				     int elset, double *pvol);

/// \brief Get the area of an elset of a 2D mesh.
///
///
///
  extern int neut_mesh_elset_area (struct NODES Nodes, struct MESH Mesh,
				   int elset, double *parea);

/// \brief Get the area of a 2D mesh.
///
///
///
  extern int neut_mesh_area (struct NODES Nodes, struct MESH Mesh,
			     double *parea);

/// \brief Get the length of an elset of a 1D mesh.
///
///
///
  extern int neut_mesh_elset_length (struct NODES Nodes, struct MESH Mesh,
				     int elset, double *plength);

/// \brief Get the length of a 1D mesh.
///
///
///
  extern int neut_mesh_length (struct NODES Nodes, struct MESH Mesh,
			       double *plength);

/// \brief Get the size of a mesh (3D: volume, 2D: area, 1D: length,
/// 0D: max distance bw two elements)
  extern int neut_mesh_size (struct NODES Nodes, struct MESH Mesh,
			     double *psize);

/// \brief Get the min, average and max radius ratios of the elts of an
/// elset of a mesh.
///
/// This function works for a 3D mesh.
///
  extern int neut_mesh_elset_rr (struct NODES Nodes, struct MESH Mesh, int
				 elset, double *prrmean, double *prrmin,
				 double *prrmax);

/// \brief Get the Osize value of an elset of a mesh.
///
/// See (CMAME, 2011) for the definition of Osize.
///
  extern int neut_mesh_elset_Osize (struct NODES Nodes, struct MESH Mesh,
				    int elset, double cl, double *pOsize);

/// \brief Get the centre of a 3D mesh.
///
///
///
  extern void neut_mesh_centre (struct NODES Nodes, struct MESH Mesh,
				double *centre);

/// \brief Get the volume of a 3D mesh.
///
///
///
  extern int neut_mesh_volume (struct NODES Nodes, struct MESH Mesh,
			       double *pvol);

/// \brief Get the average length of the elts of a mesh.
///
///
///
  extern int neut_mesh_eltlength (struct NODES Nodes, struct MESH Mesh,
				  double *pavlength);

/// \brief Get the min, average and max radius ratios of the elts of a mesh.
///
/// This function works for 2D and 3D meshes.
///
  extern int neut_mesh_rr (struct NODES Nodes, struct MESH Mesh,
			   double *prrav, double *prrmin, double *prrmax);

/// \brief Get the Odis value of a mesh.
///
/// See (CMAME, 2011) for the definition of Odis.
///
  extern int neut_mesh_Odis (struct NODES Nodes, struct MESH Mesh,
			     char *Odisexpr, double *pOdis);

  extern int neut_mesh_elset_Odis (struct NODES Nodes, struct MESH Mesh,
				   int elset, char *Odisexpr, double *pOdis);

/// \brief Get the Osize value of a mesh.
///
/// See (CMAME, 2011) for the definition of Osize.
///
  extern int neut_mesh_Osize (struct NODES Nodes, struct MESH Mesh, double cl,
			      double *pOsize);

/// \brief Get the angle between two sides of an elt at a particular node.
///
///
///
  extern void neut_mesh_elt_node_angle (struct NODES Nodes, struct MESH
					Mesh, int elt, int node,
					double *pangle);

/// \brief Determine the elt of a mesh to which a point belongs.
///
///
///
  extern int neut_mesh_point_elt (struct MESH Mesh, struct NODES Nodes,
				  double *coo, int *pelt);

/// \brief Determine the elset of a mesh to which a point belongs.
///
///
///
  extern int neut_mesh_point_elset (struct MESH Mesh, struct NODES Nodes,
				    double *coo, int *elsetlist,
				    int elsetqty, int *pelt);

/// \brief Determine the elt of an elset of a mesh to which a point belongs.
///
///
///
  extern int neut_mesh_elset_point_elt (struct MESH Mesh, struct NODES
					Nodes, int elset, double *coo,
					int *pelt);

/// \brief Test if a point belongs to an elt of a mesh.
///
///
///
  extern int neut_mesh_point_elt_in (struct MESH Mesh, struct NODES
				      Nodes, double *coo, int elt);

/// \brief Determine the clsest elt of an elset of a mesh to which a point is.
///
///
///
  extern int neut_mesh_elset_point_closestelt (struct MESH Mesh, struct
					       NODES Nodes, int elset,
					       double *coo, int *pelt);

  extern int neut_mesh_elset_points_closestelts (struct MESH Mesh, struct
					         NODES Nodes, int elset,
					         double **coos, int qty,
                                                 char *method, int *elts);

/// \brief Determine the closest position from a point to the elements
/// of a 2D mesh.
///
///
///
  extern int neut_mesh_point_closestpoint (struct MESH Mesh,
					   struct NODES Nodes, double *coo,
					   double *pdist, double *ccoo,
					   double *v, double *eq);

/// \brief Project a point along a given direction onto a mesh.
///
///
///
  extern int neut_mesh_point_proj_alongonto (double *Coo, double *n, struct
					     NODES N, struct MESH M,
					     int elset);

/// \brief Get the distance from a point to the closest node of a mesh.
///
///
///
  extern int neut_mesh_point_closestnode (struct MESH Mesh,
					  struct NODES Nodes, double *coo,
					  int *pnode);

  extern int neut_mesh_elt2dpair_angle (struct NODES Nodes, struct MESH
      Mesh, int elt1, int elt2, double *pangle);

#endif				/* NEUT_MESH_GEOM_H */

#ifdef __cplusplus
}
#endif
