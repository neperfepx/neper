/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_TESS_GEN_H
#define NEUT_TESS_GEN_H

#include<orilib.h>

  extern void neut_tess_var_dim (int dim, char *entity, char *var, int *pdim);
  extern int neut_tess_var_val (struct TESS *pTess,
				int *showedge, int *showface, int *showpoly,
				char *entity, int id, char *var, double **pvals,
				int *pvalqty, char **type);
  extern int neut_tess_var_val_one (struct TESS *pTess,
				    int *showedge, int *showface, int *showpoly,
				    char *entity, int id, char *var, double *pvals,
				    char **type);
  extern int neut_tess_var_val_string (struct TESS Tess, char *entity, int id,
				       char *var, char **pval, char **type);
  extern void neut_tess_entity_dim (struct TESS Tess, char *entity, int *pdim);
  extern void neut_tess_dim_entity (int dim, char **pentity);
  extern void neut_tess_entity_qty (struct TESS Tess, char *entity, int
				    *pqty);
  extern int neut_tess_expr_cells (struct TESS *pTess, char *, int **,
				      int *);
  extern int neut_tess_expr_polys (struct TESS *pTess, char *, int **,
				      int *);
  extern int neut_tess_expr_faces (struct TESS *pTess, char *, int **,
				      int *);
  extern int neut_tess_expr_edges (struct TESS *pTess, char *, int **,
				      int *);
  extern void neut_tess_cell (struct TESS Tess, char **pcell);
  extern void neut_tess_entity_expr_val (struct TESS *pTess, char *entity,
					 char *expr, double *val, char **ptype);
  extern void neut_tess_entity_expr_val_int (struct TESS *pTess,
                                         char *entity, char *expr, int *val);
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

  extern int neut_tess_expr_vars_vals (struct TESS *pTess, char* expr, int
      *showedge, int *showface, int *showpoly, char *entity, int id, char
      ***pvars, double **pvals, char ***ptypes, int *pvarqty);

  extern int neut_tess_isvoid (struct TESS Tess);

  extern int neut_tess_cell_isvoid (struct TESS Tess, int cell);

  extern int neut_tess_hascellorispread (struct TESS Tess);

  extern int neut_tess_isreg (struct TESS Tess);

  extern int neut_tess_cell_id (struct TESS Tess, int cell);

  extern void neut_tess_olset (struct TESS Tess, struct OL_SET *pOSet);

  extern int neut_tess_cellori (struct TESS Tess, double **cellori);

  extern void neut_tess_cells_olset (struct TESS Tess, double **CellOri, char *crysym,
                       int *cells, int cellqty, struct OL_SET *pOSet);

  extern int neut_tess_expr_val (struct TESS *pTess, int *showedge, int *showface, int *showpoly,
                    char *entity, int id, char *expr,
                    double **pvals, int *pvalqty, char **ptype);

  extern int neut_tess_expr_val_one (struct TESS *pTess, int *showedge, int *showface, int *showpoly,
                        char *entity, int id, char *expr, double *pval, char **ptype);

#endif				/* NEUT_TESS_GEN_H */

#ifdef __cplusplus
}
#endif
