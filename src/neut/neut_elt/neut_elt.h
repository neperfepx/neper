/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_elt.h
/// \brief Get properties of mesh elements
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_ELT_H
#define NEUT_ELT_H

extern void neut_elt_name_prop (char *, char *, char *, int *, int *);
extern int neut_elt_nodeqty (char *, int, int);
extern int neut_elt_boundqty (char *, int);
extern int neut_elt_dim_gtype_order (int, int);
extern int neut_elt_order (char *, int, int);
extern int neut_elt_gtype_prop (int, char *, int *, int *);
extern double neut_elt_rr_3d (double *, double *, double *, double *);
extern double neut_elt_rr_2d (double *, double *, double *);
extern double neut_elt_3d_rho (double *, double *, double *, double *);
extern int neut_elt_boundlist (char *type, int dim, int ***pfaces,
			       int *pfaceqty, int *pnodeqty);
extern void neut_elt_nodes_facet (int dim, int* nodes, int* facetnodes, int* pfacet, int *pfacetori);
extern void neut_elt_facet_nodes (char *type, int dim, int order, int* nodes, int facet, int facetori, int*
    surfnodes);

extern void neut_elt_tri_shapefct (double *a, double *b, double *c, double *p, double *val);

extern void neut_elt_orderarrays_3d (char *elttype, int ***pfir3, int *pnodeqty_3d, int ***psec3, int *pnodeqty_3d1);
extern void neut_elt_orderarrays_2d (char *elttype, int ***pfir2, int *pnodeqty_2d, int ***psec2, int *pnodeqty_2d1);
extern void neut_elt_orderarrays_1d (char *elttype, int ***pfir1, int *pnodeqty_1d, int ***psec1, int *pnodeqty_1d1);
extern void neut_elt_orderarrays_0d (char *elttype, int ***pfir0, int *pnodeqty_0d, int ***psec0, int *pnodeqty_0d1);

extern void neut_elt_orderarrays (char *elttype, int dim, int ***pfir,  int *pnodeqty, int ***psec,
                                  int *pnodeqty1);

#endif				/* NEUT_ELT_H */

#ifdef __cplusplus
}
#endif
