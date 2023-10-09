/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_TOPO_H
#define NEUT_TESS_TOPO_H

/// \brief Get the edges of a vertex
///
///
///
  extern void neut_tess_ver_edges_all (struct TESS Tess, int ver, int **pedge,
				       int *pedgeqty);

/// \brief Get the edges of a vertex
///
///
///
  extern void neut_tess_ver_edges (struct TESS Tess, int ver, int **pedge,
				   int *pedgeqty);

/// \brief Get the faces of a vertex
///
///
///
  extern void neut_tess_ver_faces (struct TESS Tess, int ver, int **pface,
				   int *pfaceqty);

/// \brief Get the faces of vertices
///
///
///
  extern void neut_tess_vers_faces (struct TESS Tess, int* vers, int verqty,
                                    int **pfaces, int *pfaceqty);

/// \brief Get the faces of vertices, faces not on the domain
///
///
///
  extern void neut_tess_vers_faces_notondomain (struct TESS Tess, int* vers, int verqty,
                                                int **pfaces, int *pfaceqty);

/// \brief Get the faces of a vertex, among the faces of a poly
///
///
///
  extern void neut_tess_ver_poly_faces (struct TESS Tess, int ver, int
					poly, int **pface, int *pfaceqty);

/// \brief Get the polyhedra of a vertex
///
///
///
  extern void neut_tess_ver_polys (struct TESS Tess, int ver, int **ppoly,
				   int *ppolyqty);

/// \brief Get the seeds of a vertex
///
///
///
  extern void neut_tess_ver_seeds (struct TESS Tess, int ver, int **pseed,
				   int *pseedqty);

  extern void neut_tess_ver_masterseeds (struct TESS Tess, int ver, int **pseed,
					 int *pseedqty);
  extern void neut_tess_edge_masterseeds (struct TESS Tess, int edge, int **pseed,
					 int *pseedqty);
  extern void neut_tess_face_masterseeds (struct TESS Tess, int face, int **pseed,
					 int *pseedqty);

/// \brief Get the positive seeds of a vertex
///
///
///
  extern void neut_tess_ver_seeds_positive (struct TESS Tess, int ver,
					    int **pseed, int *pseedqty);

/// \brief Get the domain edges of a vertex
///
///
///
  extern void neut_tess_ver_domedges (struct TESS Tess, int ver, int **pdedge,
				      int *pdedgeqty);

  extern void neut_tess_vers_alldomedges (struct TESS Tess, int *vers, int
					  verqty, int **pdedges,
					  int *pdedgeqty);

/// \brief Get the domain faces of a vertex
///
///
///
  extern void neut_tess_ver_domfaces (struct TESS Tess, int ver, int **pdface,
				      int *pdfaceqty);

/// \brief Get the domain faces of an edge
///
///
///
  extern void neut_tess_edge_domfaces (struct TESS Tess, int edge,
				       int **pdface, int *pdfaceqty);

  extern void neut_tess_edge_cells (struct TESS Tess, int edge,
				    int **pcell, int *pcellqty);

/// \brief Get the seeds of an edge
///
///
///
  extern void neut_tess_edge_seeds (struct TESS Tess, int edge, int **pseed,
				    int *pseedqty);

/// \brief Get the positive seeds of an edge
///
///
///
  extern void neut_tess_edge_seeds_positive (struct TESS Tess, int edge,
					     int **pseed, int *pseedqty);

/// \brief Get the polyhedra of an edge
///
///
///
  extern void neut_tess_edge_polys (struct TESS Tess, int edge, int **ppoly,
				    int *ppolyqty);

/// \brief Get the polyhedra of a face
///
///
///
  extern void neut_tess_face_polys (struct TESS Tess, int face, int **ppoly,
				    int *ppolyqty);

/// \brief Get the seeds of a face
///
///
///
  extern void neut_tess_face_seeds (struct TESS Tess, int face, int
      **pseeds, int *pseedqty);

  extern void neut_tess_face_seeds_positive (struct TESS Tess, int face, int
      **pseeds, int *pseedqty);

/// \brief Get the cells of a face
///
///
///
  extern void neut_tess_face_cells (struct TESS Tess, int face, int **pcell,
				    int *pcellqty);

/// \brief Get the vertices of a set of edges
///
///
///
  extern void neut_tess_edges_vers (struct TESS Tess, int *edge, int edgeqty,
				    int **pver, int *pverqty);

/// \brief Get the faces of a set of edges
///
///
///
  extern void neut_tess_faces_polys (struct TESS Tess, int *face, int faceqty,
				     int **ppoly, int *ppolyqty);

/// \brief Get the faces of a set of edges
///
///
///
  extern void neut_tess_edges_faces (struct TESS Tess, int *edge, int edgeqty,
				     int **pface, int *pfaceqty);

/// \brief Get the edges of a ver, with the edges belonging to a given domface
///
///
///
  extern int neut_tess_ver_domface_edges (struct TESS Tess, int ver,
					  int dface, int **pedge,
					  int *pedgeqty);

/// \brief Get the faces of an edge, with the faces belonging to a given domface
///
///
///
  extern int neut_tess_edge_domface_faces (struct TESS Tess, int edge,
					   int dface, int **pface,
					   int *pfaceqty);

/// \brief Tell if an edge is a fake edge (not a triple line + on the circular part of a cylinder)
///
///
///
  extern int neut_tess_edge_fake (struct TESS Tess, int edge);

/// \brief Get the edges of a set of faces
///
///
///
  extern void neut_tess_faces_vers (struct TESS Tess, int *face, int faceqty,
				    int **pver, int *pverqty);

/// \brief Get the edges of a set of faces
///
///
///
  extern void neut_tess_faces_edges (struct TESS Tess, int *face, int faceqty,
				     int **pedge, int *pedgeqty);

/// \brief Get the com edge of a set of faces of a TESS
///
///
///
  extern int neut_tess_faces_comedges (struct TESS Tess, int *face,
				       int faceqty, int **pedges,
				       int *pedgeqty);

/// \brief Get the com ver of a set of faces of a TESS
///
///
///
  extern int neut_tess_faces_comvers (struct TESS Tess, int *face,
				       int faceqty, int **pvers,
				       int *pverqty);

/// \brief Get the com edge of a set of faces of a TESS
///
///
///
  extern int neut_tess_faces_comedge (struct TESS Tess, int *face,
				      int faceqty, int *pedge);

/// \brief Get the com ver of a set of edges of a TESS
///
///
///
  extern int neut_tess_edges_comver (struct TESS Tess, int *edge,
				     int edgeqty, int *pver);

  extern int neut_tess_edgepair_comver (struct TESS Tess, int edge1, int edge2, int *pver);

/// \brief Get the vertices of a edge
///
///
///
  extern void neut_tess_edge_vers (struct TESS Tess, int edge, int **pver,
				   int *pverqty);

/// \brief Get the vertices of a face
///
///
///
  extern void neut_tess_face_vers (struct TESS Tess, int face, int **pver,
				   int *pverqty);

/// \brief Get the vertices of a polyhedron
///
///
///
  extern void neut_tess_poly_vers (struct TESS Tess, int poly, int **pver,
				   int *pverqty);
  extern void neut_tess_poly_vercoos (struct TESS Tess, int poly, int **pvers,
                                      double ***pvercoos, int *pverqty);

/// \brief Get the edges of a polyhedron
///
///
///
  extern void neut_tess_poly_edges (struct TESS Tess, int poly, int **pedge,
				    int *pedgeqty);
  extern void neut_tess_poly_edgevers (struct TESS Tess, int poly, int **pedge,
				       int ***pedgevers, int *pedgeqty);

  extern void neut_tess_polys_faces (struct TESS Tess, int *polys, int
				     polyqty, int **pfaces, int
				     *pfaceqty);
  extern void neut_tess_poly_faceedges (struct TESS Tess, int poly,
                                        int **pfaces, int ***pfaceedges,
                                        int **pfaceedgeqty, int *pfaceqty);

  extern void neut_tess_poly_faces (struct TESS Tess, int poly, int **pface,
				    int *pfaceqty);

/// \brief Get the skin faces of a set of polyhedra
///
///
///
  extern void neut_tess_polys_skinfaces (struct TESS Tess, int *poly,
					 int polyqty, int **pface,
					 int **pfacepoly, int *pfaceqty);

/// \brief Get the neighboring polyhedra of a polyhedron
///
///
///
  extern void neut_tess_poly_neighpolys_unsort (struct TESS Tess, int poly,
                                                int **pnpoly, int *pnpolyqty);
  extern void neut_tess_poly_neighpolys (struct TESS Tess, int poly,
					 int **pnpoly, int *pnpolyqty);
  extern void neut_tess_poly_neighpolys_samedomain (struct TESS Tess, int poly,
					int **pnpoly, int *pnpolyqty);
  extern void neut_tess_poly_neighseeds (struct TESS Tess, int poly,
                                         int **pnseeds, int *pnseedqty);

/// \brief Get the neighboring faces of a face
///
///
///
  extern void neut_tess_face_neighfaces (struct TESS Tess, int face,
					int **pnface, int *pnfaceqty);
  extern void neut_tess_face_neighfaces_samedomain (struct TESS Tess, int face,
					int **pnface, int *pnfaceqty);

/// \brief Get the neighboring faces of faces (may contain some of the input faces);
///
///
///
  extern void neut_tess_faces_neighfaces (struct TESS Tess, int *faces,
                                          int faceqty,
					  int **pnfaces, int *pnfaceqty);

  extern int neut_tess_facepair_neigh (struct TESS Tess, int face1, int
      face2);

  extern int neut_tess_edgepair_neigh (struct TESS Tess, int edge1, int
      edge2);

/// \brief Get the neighboring edges of a edge
///
///
///
  extern void neut_tess_edge_neighedges (struct TESS Tess, int edge,
					int **pnedge, int *pnedgeqty);

/// \brief Get the neighboring cells of a cell
///
///
///
  extern void neut_tess_cell_neighcell (struct TESS Tess, int cell,
					int **pncell, int *pncellqty);

/// \brief Get the two faces of an edge of a polyhedron
///
///
///
  extern void neut_tess_poly_edge_faces (struct TESS Tess, int poly, int edge,
					 int **pface);

/// \brief Get the orientation of a face of a polyhedron
///
///
///
  extern void neut_tess_poly_face_ori (struct TESS Tess, int poly, int face,
				       int *pfaceori);

/// \brief Get the orientation of an edge of a face
///
///
///
  extern void neut_tess_face_edge_ori (struct TESS Tess, int face, int edge,
				       int *pedgeori);

/// \brief Get the vertices of a set of domain vertices
///
///
///
  extern void neut_tess_domvers_ver (struct TESS Tess, int *dver, int dverqty,
				     int **pver, int *pverqty);

/// \brief Get the com domain edge of a set of domain vertices
///
///
///
  extern int neut_tess_domvers_comdomedge (struct TESS Tess, int *dver,
					   int dverqty, int *pdedge);

/// \brief Get the domain faces of a domain vertex
///
///
///
  extern void neut_tess_domver_domface (struct TESS Tess, int dver,
					int **pdface, int *pdfaceqty);

/// \brief Get the edges of a domain edge, in sorted order, i.e. going
/// from one vertex of the domain edge to the next.
///
///
  extern void neut_tess_domedge_edges_sorted (struct TESS Tess, int dedge,
					      int **pedge, int **pedgeori,
					      int *pedgeqty);

/// \brief Get the com domain face of a set of domain edges
///
///
///
  extern int neut_tess_domedges_comdomface (struct TESS Tess, int *dedge,
					    int dedgeqty, int *pdface);

/// \brief Get the domain vertices of a set of domain edges
///
///
///
  extern int neut_tess_domedges_domvers (struct TESS Tess, int *dedge,
					 int dedgeqty, int **pdver,
					 int *pdverqty);

/// \brief Get the com domain vertex of a set of domain edges
///
///
///
  extern int neut_tess_domedges_comdomver (struct TESS Tess, int *dedge,
					   int dedgeqty, int *pdver);

/// \brief Get the vertices of a domain edge
///
///
///
  extern void neut_tess_domedge_ver (struct TESS Tess, int dedge, int **pver,
				     int *pverqty);

/// \brief Get the vertices of the body of a domain edge
///
///
///
  extern void neut_tess_domedge_body_ver (struct TESS Tess, int dedge,
					  int **pver, int *pverqty);

/// \brief Get the vertices of a domain edge
///
///
///
  extern void neut_tess_domedges_ver (struct TESS Tess, int *dedge,
				      int dedgeqty, int **pver, int *pverqty);

/// \brief Get the vertices of the bodies of domain edges
///
/// dfaceqty = -1 means take all dom edges
///
  extern void neut_tess_domedges_body_ver (struct TESS Tess, int *dedge,
					   int dedgeqty, int **pver,
					   int *pverqty);

/// \brief Get the com domain edge of a set of domain faces
///
///
///
  extern int neut_tess_domfaces_comdomedge (struct TESS Tess, int *dface,
					    int dfaceqty, int *pdedge);

/// \brief Get the com domain ver of a set of domain faces
///
///
///
  extern int neut_tess_domfaces_comdomver (struct TESS Tess, int *face,
					   int faceqty, int *pver);

/// \brief Get the vertices of a domain face
///
///
///
  extern void neut_tess_domface_vers (struct TESS Tess, int dface, int **pver,
				     int *pverqty);

/// \brief Get the vertices of domain faces
///
///
///
  extern void neut_tess_domfaces_vers (struct TESS Tess, int *domfaces,
                                       int domfaceqty, int **pvers, int *pverqty);

/// \brief Get the vertices of the body of a domain face
///
///
///
  extern void neut_tess_domface_body_ver (struct TESS Tess, int dface,
					  int **pver, int *pverqty);

/// \brief Get the vertices of the bodies of domain faces
///
/// dfaceqty = -1 means take all dom faces
///
  extern void neut_tess_domfaces_body_ver (struct TESS Tess, int *dface,
					   int dfaceqty, int **pver,
					   int *pverqty);

/// \brief Get the vers of a domain edge
///
///
///
  extern void neut_tess_domedge_vers (struct TESS Tess, int dedge, int **pver,
				      int *pverqty);

/// \brief Get the edges of a domain face
///
///
///
  extern void neut_tess_domface_edges (struct TESS Tess, int dface,
				       int **pedge, int *pedgeqty);

/// \brief Get the edges of the body of a domain face
///
///
///
  extern void neut_tess_domface_body_edges (struct TESS Tess, int dface,
					    int **pedge, int *pedgeqty);

/// \brief Get the body level of a polyhedron
///
///
///
  extern int neut_tess_cell_body (struct TESS *pTess, char *expr, int);

/// \brief Get the body level of a face
///
///
///
  extern int neut_tess_face_body (struct TESS *pTess, char *expr, int);

/// \brief Get the body level of a edge
///
///
///
  extern int neut_tess_edge_body (struct TESS *pTess, char *expr, int);

/// \brief Get the body level of a ver
///
///
///
  extern int neut_tess_ver_body (struct TESS *pTess, char *expr, int);

/// \brief Determine if a polyhedron is standard
///
///
///
  extern int neut_tess_poly_std (struct TESS Tess, int);

/// \brief Determine if a polyhedron has been regularized
///
///
///
  extern int neut_tess_poly_reg (struct TESS Tess, int);

/// \brief Get a mesh from a poly of a TESS
///
///
///
  extern int
    neut_tess_poly_mesh (struct TESS Tess, int nb,
			 struct NODES *pNodes, struct MESH *pMesh);

  extern int neut_tess_face_interpolmesh (struct TESS Tess, int,
					  struct NODES *, struct MESH *);

  extern int neut_tess_domface_label_id (struct TESS Tess, char *label,
					 int *pid);

  extern void neut_tess_cell_vers (struct TESS Tess, int cell, int **pvers,
				   int *pverqty);

  extern int neut_tess_seed_master_slave (struct TESS Tess, int master,
					  int *pos, int fact);

  extern int neut_tess_seed_perseed (struct TESS Tess, int seed, int *shift);

  extern void neut_tess_edge_perfaces (struct TESS Tess, int edge,
				       int **pfaces, int *pfaceqty);
  extern void neut_tess_perfaces_perseeds (struct TESS Tess,
					   int *faces, int faceqty,
					   int **pseeds, int *pseedqty);
  extern int neut_tess_face_perface (struct TESS Tess, int face, int *shift,
				     int *pperface);

  extern int neut_tess_seeds_comface (struct TESS Tess, int *seeds,
				      int seedqty, int *pface);

  extern int neut_tess_seeds_edge (struct TESS Tess, int *seeds,
				   int seedqty, int *pedge);

  extern int neut_tess_seeds_ver (struct TESS Tess, int *seeds,
				  int seedqty, int *pver);

  extern void neut_tess_cells_neigh (struct TESS Tess, int* cells,
				     int cellqty, int *pneigh);

  extern void neut_tess_domedge_faces (struct TESS, int domedge, int
      **pfaces, int *pfaceqty);

  extern void neut_tess_domface_polys (struct TESS, int domface, int
      **ppolys, int *ppolyqty);

  extern void neut_tess_face_domedge_edges (struct TESS Tess, int face,
      int domedge, int **pedges, int *pedgeqty);
  extern void neut_tess_poly_domface_faces (struct TESS Tess, int poly,
      int domface, int **pfaces, int *pfaceqty);

  extern void neut_tess_faces_contiguousfaces (struct TESS Tess, double coplanar, int
      *faces, int faceqty, int *pqty, int ***pfaces, int **pfaceqty);

  extern void neut_tess_edges_contiguousedges (struct TESS Tess, double colinear, int
      *edges, int edgeqty, int *pqty, int ***pedges, int **pedgeqty);

  extern void neut_tess_ver_cells (struct TESS Tess, int ver, int
      **pcell, int *pcellqty);

  extern int neut_tess_seed_masterseed (struct TESS Tess, int seed, int *pmaster);

  extern int neut_tess_seed_isper (struct TESS Tess, int seed);
  extern int neut_tess_seed_iscell (struct TESS Tess, int seed);
  extern int neut_tess_ver_isper (struct TESS Tess, int ver);
  extern int neut_tess_edge_isper (struct TESS Tess, int edge);
  extern int neut_tess_face_isper (struct TESS Tess, int face);

  extern int neut_tess_ver_ispermaster (struct TESS Tess, int ver);
  extern int neut_tess_edge_ispermaster (struct TESS Tess, int edge);
  extern int neut_tess_face_ispermaster (struct TESS Tess, int face);

  extern int neut_tess_ver_isperslave (struct TESS Tess, int ver);
  extern int neut_tess_edge_isperslave (struct TESS Tess, int edge);
  extern int neut_tess_face_isperslave (struct TESS Tess, int face);

  extern void neut_tess_ver_inters (struct TESS Tess, int ver, int
				    **pinters, int *pinterqty);

  extern void neut_tess_inter_seeds (struct TESS Tess, int inter, int **pseed,
				     int *pseedqty);
  extern void neut_tess_inter_seeds_positive (struct TESS Tess, int inter, int **pseed,
				     int *pseedqty);
  extern int neut_tess_inter_isperslave (struct TESS Tess, int inter);

  extern int neut_tess_face_scale (struct TESS Tess, int face, int *pscale);
  extern int neut_tess_face_scale_polys (struct TESS Tess, int face, int scale, int *poly);

  extern void neut_tess_edge_scale (struct TESS Tess, int edge, int *pscale);
  extern void neut_tess_ver_scale (struct TESS Tess, int ver, int *pscale);

  extern void neut_tess_polypair_commonfaces (struct TESS Tess, int poly1, int poly2,
                                int **pfaces, int *pfaceqty);
  extern void neut_tess_seedpair_commonfaces (struct TESS Tess, int seed1, int seed2,
                                int **pfaces, int *pfaceqty);

  extern void neut_tess_vers_allfaces (struct TESS Tess, int *vers, int verqty,
                                       int **pfaces, int *pfaceqty);

  extern void neut_tess_face_vercoos (struct TESS Tess, int face, double ***pvercoos,  int *pverqty);

  extern void neut_tess_poly_edge_faces (struct TESS Tess, int poly, int edge, int **pface);

  extern void neut_tess_domfaces_faces (struct TESS Tess, int *domfaces, int domfaceqty,
                                        int **pfaces, int *pfaceqty);

  extern int neut_tess_edge_iscurved (struct TESS Tess, int edge);
  extern int neut_tess_face_iscurved (struct TESS Tess, int face);
  extern int neut_tess_face_hascurvededge (struct TESS Tess, int face);

  extern void neut_tess_edge_scale (struct TESS Tess, int edge, int *pscale);
  extern void neut_tess_ver_scale (struct TESS Tess, int ver, int *pscale);
  extern void neut_tess_edge_faces (struct TESS Tess, int edge, int **pfaces, int *pfaceqty);

  extern int neut_tess_cellbody_pos (struct TESS Tess, char *expr, int *ppos);

  extern int neut_tess_cell_scale (struct TESS Tess, int id, int *pscale);

#endif				/* NEUT_TESS_TOPO_H */

#ifdef __cplusplus
}
#endif
