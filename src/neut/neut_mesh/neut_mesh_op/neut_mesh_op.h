/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MESH_OP_H
#define NEUT_MESH_OP_H

/// \brief Allocate a MESH structure.
///
///
///
  extern struct MESH neut_mesh_alloc (int Dimension, char *EltType,
				      int EltOrder, int EltQty, int ElsetQty);

/// \brief Free a MESH structure.
///
///
///
  extern void neut_mesh_free (struct MESH *pMesh);

/// \brief Allocate an array of MESH structures.
///
///
///
  extern struct MESH *neut_mesh_alloc_1d (int size);

/// \brief Free an array of MESH structures.
///
///
///
  extern void neut_mesh_free_1d (struct MESH *pMesh, int size);

/// \brief Set a MESH structure to zero.
///
///
///
  extern void neut_mesh_set_zero (struct MESH *pMesh);

/// \brief Copy a mesh structure.
///
///
///
  extern void neut_mesh_memcpy (struct MESH Old, struct MESH *pNew);

/// \brief Compare two mesh structures.
///
///
///
  extern int neut_mesh_cmp (struct NODES N1, struct MESH M1, struct NODES N2,
			    struct MESH M2);

#ifdef HAVE_LIBSCOTCH
#include <scotch.h>
/// \brief Convert a mesh into a mesh at the Scotch format.
///
///
///
  extern void neut_mesh_scotchmesh (struct MESH, int, SCOTCH_Mesh *);
#endif

/// \brief Initialize NodeElts of a MESH structure.
///
///
///
  extern void neut_mesh_init_nodeelts (struct MESH *pMesh, int NodeQty);
  extern void neut_mesh_array_init_nodeelts (struct MESH *Mesh, int dim, int NodeQty);

/// \brief Initialize EltElset of a MESH structure.
///
/// Input: Elsets
///
  extern void neut_mesh_init_eltelset (struct MESH *pMesh, int *elset_nbs);

/// \brief Initialize Elsets of a MESH structure.
///
/// Input: EltElset
///
  extern void neut_mesh_init_elsets (struct MESH *pMesh);

/// \brief Initialize Elsets labels
///
/// Input: Dimension, ElsetQty
///
  extern void neut_mesh_init_elsetlabels (struct MESH *pMesh);

/// \brief Merge the elsets of a mesh.
///
///
///
  extern void neut_mesh_mergeelsets (struct MESH *pEMesh);

/// \brief Order the elsets of a 2D mesh.
///
/// This function enable to get all elts pointing towards the same side
/// of an elset.
///
  extern void neut_mesh_orderelsets (struct MESH *pMesh);

/// \brief Reverse the nodes of an elt of a mesh.
///
/// The functions works for a 2D mesh only. It is useful to get a
/// properly oriented normal.
///
  extern void neut_mesh_elt_reversenodes (struct MESH *pMesh, int elt);

  extern void neut_mesh_elset_reversenodes (struct MESH *pMesh, int elset);

  extern void neut_mesh_reversenodes (struct MESH *pMesh);

  extern void neut_mesh_elset_reverseelts (struct MESH *pMesh, int elset);

/// \brief Get a tri/tet mesh from a quad/hex mesh.
///
///
///
  extern void neut_mesh_quad_tri (struct NODES Nodes, struct MESH Mesh,
				  struct MESH *pTMesh);

/// \brief Get a tri mesh from a quad mesh, by inserting a node at the centre of the elements.
///
///
///
  extern void neut_mesh_quad_tri_insertnode (struct NODES *pNodes,
					     struct MESH Mesh,
					     struct MESH *pTMesh);

/// \brief Get a 3D prism mesh from a 2D tri mesh
///
/// \author Loïc Renversade
///
///
  extern void neut_mesh_tri_prism (struct NODES *pNodes, struct MESH Mesh,
				   struct MESH *pPMesh, double thickness);

/// \brief Get a 3D tet mesh from a 2D tri mesh
///
/// \author Loïc Renversade
///
/// Reference: "How to Subdivide Pyramids, Prisms and Hexahedra into
/// Tetrahedra, Dompierre et al., 8th International Meshing Roundtable,
/// Lake Tahoe, Californie, 10–13 octobre 1999."
///
  extern void neut_mesh_tri_tet (struct NODES *Nodes, struct MESH Mesh,
				 struct MESH *pTMesh, double thickness);

/// \brief Add an elt to a mesh.
///
///
///
  extern void neut_mesh_addelt (struct MESH *pMesh, int *EltNodes);

/// \brief Add an elset to a mesh.
///
///
///
  extern void neut_mesh_addelset (struct MESH *pMesh, int *ElsetElts, int
				  EltQty);

/// \brief Add an elt to an elset of a mesh.
///
///
///
  extern void neut_mesh_elset_addelt (struct MESH *pMesh, int elset_nb,
				      int elt);

/// \brief Remove an elt from a mesh.
///
///
///
  extern int neut_mesh_rmelt (struct MESH *pMesh, int elt);

/// \brief Remove elts from a mesh.
///
///
///
  extern int neut_mesh_rmelts (struct MESH *pMesh, struct NODES Nodes,
			       int *rmelt, int rmeltqty);

/// \brief Remove an elset from a mesh.
///
///
///
  extern int neut_mesh_rmelset (struct MESH *pMesh, struct NODES Nodes,
				int elset);

/// \brief Switch two elts of a mesh.
///
///
///
  extern void neut_mesh_elts_switch_pair (struct MESH *pMesh, int n1, int n2);

/// \brief Switch elts of a mesh.
///
///
///
  extern void neut_mesh_elts_switch (struct MESH *pMesh, int *elt_nbs);

/// \brief Switch nodes of a mesh.
///
///
///
  extern void neut_mesh_nodes_switch (struct MESH *pMesh, int *node_nbs);

/// \brief Switch nodes, elts and elsets of a mesh.
///
///
///
  extern void neut_mesh_switch (struct MESH *pMesh, int *nodes_old_new,
				int *elts_old_new, int *elsets_old_new);

/// \brief Renumber nodes, elts and elsets of a mesh so that they are numbered contiguously from 1.
///
///
///
  extern void neut_mesh_renumber_continuous (struct MESH *pMesh,
					     int *node_nbs, int *elt_nbs,
					     int *elset_nbs);

/// \brief Split an elt of a mesh into 3 elts.
///
/// This function works to a 2D mesh only.
///
  extern void neut_mesh_elt_split (struct NODES *pNodes, struct MESH *pMesh,
				   int elt);

/// \brief Flip two elts of a mesh.
///
///
///
  extern void neut_mesh_eltpair_flip (struct MESH *pMesh, int elt1, int elt2);

/// \brief Split a pair of adjacent elts into 4 elts.
///
/// The new elts are based on the 4 existing nodes and a node located at
/// the centre of the elt edge shared by the elts.
///
  extern void neut_mesh_eltpair_split (struct NODES *pNodes,
				       struct MESH *pMesh, int elt1,
				       int elt2);

  extern void neut_mesh_2d_laplaciansmooth (struct NODES *pN, struct MESH M,
                                            int *bnodes, int bnodeqty);

#endif				/* NEUT_MESH_OP_H */

#ifdef __cplusplus
}
#endif
