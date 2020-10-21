/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<orilib.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef  NEUT_VOX_GEN_H
#define  NEUT_VOX_GEN_H

  extern void neut_tesr_cell (struct TESR Tesr, char **pcell);
  extern void neut_tesr_entity_qty (struct TESR Tesr, char *entity,
				    int *pqty);

  extern void neut_tesr_var_list (char *entity, char ***pvar, int *pvarqty);
  extern int neut_tesr_expr_val (struct TESR Tesr, char *entity,
				     int id, char *expr, double **pvals, int *pvalqty, char **ptype);
  extern int neut_tesr_expr_val_one (struct TESR Tesr, char *entity,
				     int id, char *expr, double *pval, char **ptype);
  extern int neut_tesr_expr_val_int (struct TESR Tesr, char *entity,
				     int id, char *expr, int **pvals, int *pvalqty, char **ptype);
  extern int neut_tesr_expr_val_int_one (struct TESR Tesr, char *entity,
                                         int id, char *expr, int *pval, char **ptype);
  extern int neut_tesr_var_val (struct TESR Tesr,
				char *entity, int id, char *var, double **vals, int* pvalqty,
				char **ptype);
  extern int neut_tesr_var_val_one (struct TESR Tesr,
                                    char *entity, int id, char *var, double *pval, char **ptype);
  extern int neut_tesr_var_val_int (struct TESR Tesr, char *entity, int id,
                                    char *var, int **pvals, int *pvalqty, char **ptype);

  extern void neut_tesr_entity_expr_val (struct TESR Tesr, char *entity,
					 char *expr, double *val, char **ptype);
  extern void neut_tesr_entity_expr_val_int (struct TESR Tesr, char *entity,
                                             char *expr, int *val);

  extern void neut_tesr_var_dim (int dim, char *entity, char *var, int *pdim);

  extern int neut_tesr_pos_valid (struct TESR Tesr, int *pos);
  extern void neut_tesr_sizestring (struct TESR Tesr, char **psizestring);

  extern void neut_tesr_cell_olset (struct TESR Tesr, int cell, struct OL_SET *pOSet);

  extern int neut_tesr_isvoid (struct TESR Tesr);

#endif				/* NEUT_VOX_GEN_H */

#ifdef __cplusplus
}
#endif
