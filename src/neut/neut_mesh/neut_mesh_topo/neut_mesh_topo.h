/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_TOPO_H
#define NEUT_MESH_TOPO_H

/// \brief Get the 1D elts of a 0D elt.
///
///
///
  extern int neut_mesh_elt0d_elts1d (struct MESH Mesh0d, int elt0d,
				     struct MESH MESH1d, int **pelt1d,
				     int *pelt1dqty);

/// \brief Get the 1D elsets of a 0D elt.
///
///
///
  extern int neut_mesh_elt0d_elsets1d (struct MESH Mesh0D, int elt0d,
				       struct MESH Mesh1D, int **pelset1d,
				       int *pelset1dqty);

/// \brief Get the 2D elts of a 0D elt.
///
///
///
  extern int neut_mesh_elt0d_elts2d (struct MESH Mesh0D, int elt0d,
				     struct MESH Mesh1D, struct MESH Mesh2D,
				     int **pelt3d, int *pelt3dqty);

/// \brief Get the 3D elts of a 0D elt.
///
///
///
  extern int neut_mesh_elt0d_elts3d (struct MESH Mesh0D, int elt0d,
				     struct MESH Mesh1D, struct MESH Mesh2D,
				     struct MESH Mesh3D, int **pelt3d,
				     int *pelt3dqty);

/// \brief Get the 2D elts of a 1D elt.
///
///
///
  extern int neut_mesh_elt1d_elts2d (struct MESH Mesh1D, int elt1d,
				     struct MESH Mesh2D, int **pelt2d,
				     int *pelt2dqty);

/// \brief Get the 2D elsets of a 1D elt.
///
///
///
  extern int neut_mesh_elt1d_elsets2d (struct MESH Mesh1D, int elt1d,
				       struct MESH Mesh2D, int **pelset2d,
				       int *pelset2dqty);

/// \brief Get the 3D elts of a 1D elt.
///
///
///
  extern int neut_mesh_elt1d_elts3d (struct MESH Mesh1D, int elt1d,
				     struct MESH Mesh2D, struct MESH Mesh3D,
				     int **pelt3d, int *pelt3dqty);

/// \brief Get the 3D elts of a 2D elt.
///
///
///
  extern int neut_mesh_elt2d_elts3d (struct MESH Mesh2D, int elt2d,
				     struct MESH Mesh3D, int **pelt3d,
				     int *pelt3dqty);

/// \brief Get the 3D elsets of a 2D elt.
///
///
///
  extern int neut_mesh_elt2d_elsets3d (struct MESH Mesh2D, int elt2d,
				       struct MESH Mesh3D, int **pelset3d,
				       int *pelset3dqty);

/// \brief Get the 2D elts of a 3D elt.
///
///
///
  extern void neut_mesh_elt3d_elts2d (struct MESH Mesh3D, int elt3d,
				      struct MESH Mesh2D, int **pelts2d,
				      int *pelt2dqty);

/// \brief Get the elsets of elts.
///
///
///
  extern void neut_mesh_elts_elsets (struct MESH Mesh, int *elts,
				     int eltqty, int **pelsets,
				     int *pelsetqty);

/// \brief Get the neighbouring elts of an elt.
///
///
///
  extern int neut_mesh_elt_neighelts (struct MESH Mesh, int elt, int **pnelts,
				      int *pneltqty);

/// \brief Get the neighbouring elts of an elt, which are in a given elset.
///
///
///
  extern int neut_mesh_elt_elset_neighelts (struct MESH Mesh, int elt, int
					    elset, int **pnelts,
					    int *pneltqty);

/// \brief Get the neighbouring elts of an elt, which are in a given elset and share at least maxnodeqty nodes with the elt.
///
///
///
  extern void neut_mesh_elt_elset_neighelts_maxnodeqty (struct MESH Mesh,
							int elt, int elset,
							int maxnodeqty,
							int **pnelts,
							int *pneltqty);

/// \brief Get the elts which are at the boundary of a set of elts.
///
/// The output elements are part of the input set of elts.
///
  extern void neut_mesh_elts_boundelts (struct MESH Mesh, int *elts,
					int eltqty, int **pboundelts,
					int *pboundeltqty);

/// \brief Get all elts of nodes.
///
///
///
  extern void neut_mesh_nodes_allelts (struct MESH Mesh, int *nodes,
				       int nodeqty, int **pelts,
				       int *peltqty);

/// \brief Get the common elts of nodes.
///
///
///
  extern void neut_mesh_nodes_comelts (struct MESH Mesh, int *nodes,
				       int nodeqty, int **pelts,
				       int *peltqty);

/// \brief Get the common elt of nodes.
///
///
///
  extern int neut_mesh_nodes_comelt (struct MESH Mesh, int *nodes, int
				     nodeqty, int *pelt);

/// \brief Get the common elts of nodes, with the elts belonging to
/// particular elsets.
///
///
///
  extern int neut_mesh_nodes_elsets_comelts (struct MESH Mesh, int *nodes,
					     int nodeqty, int *elsets,
					     int elsetqty, int **pelts,
					     int *peltqty);

/// \brief Get the common nodes of elts.
///
///
///
  extern void neut_mesh_elts_comnodes (struct MESH Mesh, int *elts,
				       int eltqty, int **pnodes,
				       int *pnodeqty);

/// \brief Get the common nodes of a pair of elts.
///
///
///
  extern void neut_mesh_eltpair_comnodes (struct MESH Mesh, int elt1, int
					  elt2, int **pnodes, int *pnodeqty);

/// \brief Get all nodes of elts.
///
///
///
  extern void neut_mesh_elts_allnodes (struct MESH Mesh, int *elts, int
				       eltqty, int **pnodes, int *pnodeqty);

/// \brief Get the nodes of an elset.
///
///
///
  extern void neut_mesh_elset_nodes (struct MESH Mesh, int id, int
				     **pnodes, int *pqty);

/// \brief Get the nodes of elsets.
///
///
///
  extern void neut_mesh_elsets_nodes (struct MESH Mesh, int *elsets,
				      int elsetqty, int **pnodes,
				      int *pnodeqty);

/// \brief Get the orientation a pair of nodes of an elt of a mesh.
///
/// The result is 1 if the 2nd node follows the 1st in the elt node
/// list, -1 in the opposite case and 0 otherwise.
///
  extern void neut_mesh_elt_nodes_ori (struct MESH Mesh, int elt, int *nodes,
				       int *pori);

/// \brief Get the topology relations between a 3D and a 2D mesh.
///
///
///
  extern void neut_mesh_facepoly (struct NODES Nodes, struct MESH Mesh2D,
				  struct MESH Mesh3D, int ***pfacepoly);

/// \brief Get the topology relations between a 2D and a 1D mesh.
///
///
///
  extern void neut_mesh_edgeface (struct MESH Mesh1D, struct MESH Mesh2D,
				  int ***pedgefacenb, int **pedgefaceqty);

/// \brief Get the topology relations between a 1D and a 0D mesh.
///
///
///
  extern void neut_mesh_veredge (struct MESH Mesh0D, struct MESH Mesh1D,
				 int ***pveredgenb, int **pveredgeqty);

/// \brief Create a mesh from the boundary of an elt of a mesh.
///
/// This function works for 1D, 2D and 3D input meshes.
///
  extern void neut_mesh_elt_boundmesh (struct NODES Nodes, struct MESH Mesh,
				       int elt, struct MESH *pBMesh);

/// \brief Get a 2D mesh from a 3D mesh.
///
///
///
  extern void neut_mesh3d_mesh2d (struct NODES Nodes, struct MESH Mesh3D,
				  struct MESH *pMesh2D, int ***pFacePoly, int
				  *pFaceQty, int verbosity);

/// \brief Get a 1D mesh from a 2D mesh.
///
///
///
  extern void neut_mesh2d_mesh1d (struct NODES Nodes, struct MESH Mesh2D,
				  struct MESH *pMesh1D, int ***pEdgeFaceNb,
				  int **pEdgeFaceQty, int *pEdgeQty,
				  int verbosity);

/// \brief Get a 0D mesh from a 1D mesh.
///
///
///
  extern void neut_mesh1d_mesh0d (struct MESH Mesh1D,
				  struct MESH *pMesh0D, int ***pVerEdgeNb,
				  int **pVerEdgeQty, int *pVerQty,
				  int verbosity);

/// \brief Get the dimension of a node of a mesh.
///
///
///
  extern int neut_mesh_node_dim (struct MESH Mesh0D, struct MESH Mesh1D,
                                 struct MESH Mesh2D, struct MESH Mesh3D,
                                 int node);

/// \brief Get the min dimension of a node of a mesh.
///
///
///
  extern int neut_mesh_node_dim_min (struct MESH Mesh0D, struct MESH Mesh1D,
                                     struct MESH Mesh2D, struct MESH Mesh3D,
                                     int node);

/// \brief Get the max dimension of a node of a mesh.
///
///
///
  extern int neut_mesh_node_dim_max (struct MESH Mesh0D, struct MESH Mesh1D,
				     struct MESH Mesh2D, struct MESH Mesh3D,
				     int node);

/// \brief Get the neighbouring nodes of a node of a mesh.
///
///
///
  extern void neut_mesh_node_neighnodes (struct MESH Mesh, int node,
					 int **pnodes, int *pnodeqty);

/// \brief Get the elsets of a node of a mesh.
///
///
///
  extern void neut_mesh_node_elsets (struct MESH Mesh, int node,
				     int **pelsets, int *pelsetqty);

  extern int  neut_mesh_node_elset (struct MESH Mesh, int node,
				    int *pelset);

extern void neut_mesh_node_elset_elts (struct MESH Mesh, int node, int
				       elset, int **pelts, int* peltqty);

/// \brief Get the elsets and the elset elts of a node of a mesh.
///
///
///
  extern void neut_mesh_node_elsets_elsetelts (struct MESH Mesh, int node,
					       int **pelsets, int *pelsetqty,
					       int ***pelsetelts,
					       int **pelseteltqty);

/// \brief Get the nodes of a mesh.
///
///
///
  extern void neut_mesh_nodes (struct MESH Mesh, int **pnodes, int *pnodeqty);

/// \brief Get the node with maximum id of a mesh.
///
///
///
  extern void neut_mesh_nodemin (struct MESH Mesh, int *pmin);

/// \brief Get the node with maximum id of a mesh.
///
///
///
  extern void neut_mesh_nodemax (struct MESH Mesh, int *pmax);

/// \brief
  extern void neut_mesh_elt_elset_neighelts_minnodeqty (struct MESH Mesh,
							int elt, int elset,
							int minnodeqty,
							int **pnelts,
							int *pneltqty);

extern int neut_mesh_elt2d_elset3d_elt3d (struct MESH Mesh2D, int elt2d,
				   struct MESH Mesh3D, int elset3d,
				   int *pelt3d);

extern void neut_mesh_elset2d_elset3d_elts3d (struct MESH Mesh2D, int
    elset2d, struct MESH Mesh3D, int elset3d, int **pelt3ds, int*
    pelt3dqty);

extern void neut_mesh_elsets_elts (struct MESH Mesh, int* elsets,
				   int elsetqty, int **pelts, int *peltqty);

extern int neut_mesh_elt1d_elsets2d_elts2d (struct MESH Mesh1D, int elt1d,
				   struct MESH Mesh2D, int *elsets2d,
				   int elset2dqty, int **pelt2d,
				   int *pelt2dqty);

extern int neut_mesh_elt2d_nodes_ordering (struct MESH Mesh, int elt2d,
    int* nodes, int nodeqty);

extern void neut_mesh_fasets_bound (struct TESS Tess, struct NODES
    Nodes, struct MESH Mesh2D, struct MESH Mesh3D, char *fasetlist,
    struct BOUNDARY *pBound);

extern void neut_mesh_domface_elts3d (struct TESS Tess, struct MESH
    Mesh2D, struct MESH Mesh3D, struct NODES Nodes, char *faset, int
    **pelts, int **peltfasets, int *peltqty);

extern int neut_mesh_elset_elt_insidefacets (struct MESH Mesh1D, struct MESH
    Mesh2D, int elset, int elt, int **pinsidefacets, int *pinsidefacetqty);
extern void neut_mesh_nodeqty (struct MESH Mesh, int *pnodeqty);

extern int neut_mesh_elt1d_isembedded (struct MESH Mesh3D, struct MESH Mesh1D,
                                       int elt1d);

extern int neut_mesh_node_domtype (struct TESS Tess, struct MESH Mesh0D,
                                   struct MESH Mesh1D, struct MESH Mesh2D,
                                   struct MESH Mesh3D, int node,
                                   int *pdomtype);

#endif /* NEUT_MESH_TOPO_H */

#ifdef __cplusplus
}
#endif
