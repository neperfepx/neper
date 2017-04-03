/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_GEN_H
#define NEUT_TESS_GEN_H

  extern void neut_tess_var_list (struct TESS Tess, char *entity,
				  char ***pvar, int *pvarqty);
  extern void neut_tess_var_dim (int dim, char *entity, char *var, int *pdim);
  extern int neut_tess_var_val (struct TESS Tess,
				int *showedge, int *showface, int *showpoly,
				char *entity, int id, char *var, double *pval,
				char **type);
  extern int neut_tess_var_val_all (struct TESS Tess, int *showedge,
				    int *showface, int *showpoly,
				    char *entity, char *var, double *val,
				    char **type);
  extern int neut_tess_var_val_string (struct TESS Tess, char *entity, int id,
				       char *var, char **pval, char **type);
  extern void neut_tess_entity_dim (char *entity, int *pdim);
  extern void neut_tess_dim_entity (int dim, char **pentity);
  extern void neut_tess_entity_qty (struct TESS Tess, char *entity, int
				    *pqty);
  extern int neut_tess_expr_polylist (struct TESS Tess, char *, int **,
				      int *);
  extern int neut_tess_expr_facelist (struct TESS Tess, char *, int **,
				      int *);
  extern void neut_tess_cell (struct TESS Tess, char **pcell);
  extern void neut_tess_entity_expr_val (struct TESS Tess, char *entity,
					 char *expr, double *val);
  extern int neut_tess_dim_entityqty (struct TESS Tess, int dim);

  extern int neut_tess_val_poly2face (struct TESS, double *polyval,
				      double *faceval);
  extern int neut_tess_val_face2edge (struct TESS, double *faceval,
				      double *edgeval);
  extern int neut_tess_val_edge2ver (struct TESS, double *edgeval,
				     double *verval);

  extern int neut_tess_poly_projection (struct TESS Tess, int cell,
					double *eq, double ***pcoo,
					int *pqty);
  extern int neut_tess_poly_projection_ontoline (struct TESS Tess, int cell,
						 double *eq, double ***pcoo,
						 int *pqty);

  extern int neut_tess_delaunay (struct TESS Tess, struct NODES *pNodes,
				 struct MESH *pMesh);

  extern int neut_tess_dom_pt_randpt (struct TESS Dom, struct POINT
				      Point, gsl_rng *r, double rad,
				      double penetration, double
				      *coo, double *pdist);
  extern int neut_tess_dom_pt_randpt_pick (struct TESS Dom, struct POINT
      Point, gsl_rng *r, double *coo);
  extern int neut_tess_dom_pt_randpt_test (struct TESS Dom, struct POINT
      Point, double rad, double penetration, double *coo, double
      *pdist);
  extern int neut_tess_dom_pt_randpt_cluster (struct TESS Dom, struct
      POINT Point, gsl_rng *r, struct POINT Point2, double penetration,
      double *coo, double *pdist);

#endif				/* NEUT_TESS_GEN_H */

#ifdef __cplusplus
}
#endif
