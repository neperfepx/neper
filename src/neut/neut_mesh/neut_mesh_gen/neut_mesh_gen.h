/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_GEN_H
#define NEUT_MESH_GEN_H

extern int neut_mesh_isvoid (struct MESH Mesh);

/// \brief Create a mesh containing an element of a mesh.
///
///
///
  extern int neut_mesh_elt_mesh (struct NODES Nodes, struct MESH Mesh,
				 int elt, struct NODES *pENodes,
				 struct MESH *pEMesh);

/// \brief Create a mesh containing an elset of a mesh.
///
///
///
  extern void neut_mesh_elset_mesh (struct NODES Nodes, struct MESH Mesh,
				    int elset, struct NODES *pENodes,
				    struct MESH *pEMesh, int **pnode_nbs);
/// \brief Get the mesh of the boundary of a TESS face.
///
///
///
  extern void neut_mesh_face_boundmesh (struct MESH Mesh1D, struct TESS Tess,
					int face, struct MESH *pBMesh);

/// \brief Get the nodes of the boundary of a Tess face.
///
///
///
  extern void neut_mesh_face_boundnodes (struct MESH Mesh1D, struct TESS
					 Tess, int face, int **pnodes,
					 int *pnodeqty);

/// \brief Get the coordinates of the nodes of the boundary of a Tess face.
///
///
///
extern void neut_mesh_face_boundnodecoos (struct NODES Nodes, struct MESH
                                          Mesh1D, struct TESS Tess, int face,
                                          double ***pnodecoos, int *pnodeqty);

/// \brief Get the mesh of the boundary of a TESS poly.
///
///
///
  extern void neut_mesh_poly_boundmesh (struct TESS Tess, int poly, struct
					MESH Mesh2D, struct MESH *pBMesh);

/// \brief Create a 2D mesh from a slice of a 3D mesh.
///
///
///
  extern void neut_mesh3d_slice (struct NODES Nodes, struct MESH Mesh,
				 double *eq, struct NODES *pSNodes,
				 struct MESH *pSMesh, int **pelt_newold,
				 int ***pnode_newold, double **pnode_fact);

/// \brief Test if an elset id is valid.
///
///
///
  extern int neut_mesh_elset_valid (struct MESH Mesh, int elset);

/// \brief Get the distance between points and a 2D mesh.
///
///
///
  extern void neut_mesh_points_mesh2ddist (struct TESS Tess,
					   struct NODES Nodes,
					   struct MESH Mesh2D,
					   struct MESH Mesh3D, double **coo,
					   int qty, double **p, double *d,
					   double **v, double **n);

/// \brief Get the distance between the nodes of a 3D mesh and a 2D mesh.
///
///
///
  extern void neut_mesh_nodes_mesh2ddist (struct TESS Tess,
					  struct NODES Nodes,
					  struct MESH Mesh2D,
					  struct MESH Mesh3D, double **p,
					  double *d, double **v, double **n);

/// \brief Get the distance between the elts of a 3D mesh and a 2D mesh.
///
///
///
  extern void neut_mesh_elts_mesh2ddist (struct TESS Tess, struct NODES Nodes,
					 struct MESH Mesh2D,
					 struct MESH Mesh3D, double **p,
					 double *d, double **v, double **n);

  extern void neut_mesh_var_list (char *entity, char ***pvar, int *pvarqty);
  extern int neut_mesh_var_val (struct NODES Nodes, struct MESH Mesh0D,
				struct MESH Mesh1D, struct MESH Mesh2D,
				struct MESH Mesh3D, struct PART Part,
				struct TESS Tess, int *showelt0d,
				int *showelt1d, int *showelt2d,
				int *showelt3d, double cl, char *entity,
				int id, char *var, double **pvals,
                                int *pvalqty, char **ptype);
  extern int neut_mesh_var_val_one (struct NODES Nodes, struct MESH Mesh0D,
                                    struct MESH Mesh1D, struct MESH Mesh2D,
                                    struct MESH Mesh3D, struct PART Part,
                                    struct TESS Tess, int *showelt0d,
                                    int *showelt1d, int *showelt2d,
                                    int *showelt3d, double cl, char *entity,
                                    int id, char *var, double *pval,
                                    char **ptype);

  extern void neut_mesh_entity_expr_val (struct NODES Nodes,
					 struct MESH Mesh0D,
					 struct MESH Mesh1D,
					 struct MESH Mesh2D,
					 struct MESH Mesh3D, struct PART Part,
					 struct TESS Tess, int *showelt0d,
					 int *showelt1d, int *showelt2d,
					 int *showelt3d, char *entity,
					 char *expr, double *val);

  extern int neut_mesh_array_dim (struct MESH *Mesh);

extern void neut_mesh_elset1d_bodynodes (struct TESS Tess, struct MESH
    *Mesh, int elset, int **pnodes, int *pnodeqty);
extern void neut_mesh_elset2d_bodynodes (struct TESS Tess, struct MESH
    *Mesh, int elset, int **pnodes, int *pnodeqty);

extern void neut_mesh2d_mesh3d (struct NODES *pNodes, struct MESH
				Mesh2D, struct MESH *pMesh3D);
extern void neut_mesh1d_mesh2d (struct NODES *pNodes, struct MESH
				Mesh1D, struct MESH *pMesh2D);

#endif /* NEUT_MESH_GEN_H */

#ifdef __cplusplus
}
#endif
