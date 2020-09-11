/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

  extern void neut_ori_des_des (char *des, char **pdes);
  extern void neut_ori_des_expand (char *des, char **pdes2);

  extern void neut_ori_volthetafct (struct FCT *pvolfct);
  extern void neut_ori_vol_theta (struct FCT *pvolfct, double vol,
                              double *ptheta);
  extern void neut_ori_n_avthetaeq (struct FCT *pvolfct, int n, char *crysym,
                                double *ptheta);
  extern void neut_ori_n_avdiameq (struct FCT *pvolfct, int n, char *crysym,
                               double *pdiameq);
  extern void neut_ori_n_avradeq (struct FCT *pvolfct, int n, char *crysym,
                              double *pradeq);

  extern void neut_ori_f_qdotnorm (double *f, double *pqdotnorm);
  extern void neut_ori_f_r (double *f, double *r);
  extern void neut_ori_f_qdot (double *f, double *qdot);

  extern void neut_ori_f_norm (double **f, int N, double *pnorm);

  extern int neut_ori_expr_desconv (char *expr, char **pdes, char **pconv);

  extern int neut_ori_fscanf (FILE *file, char *desconv, double **q, int qty, char *prefix);
  extern int neut_ori_fnscanf (char *filename, char *desconv, double **q, int qty, char *prefix);
  extern void neut_ori_fprintf (FILE *file, char *desconv, double **q, int qty, char *prefix);

  extern void neut_ori_desconv_expr (char *des, char *conv, char **pexpr);
  extern void neut_ori_desconv_expr_fepx (char *des, char *conv, char **pexpr);

  extern void neut_ori_rodriguescol (double *q, char *crysym, double *col);
  extern void neut_ori_rodriguescol_R (double *R, char *crysym, double *col);
  extern void neut_ori_rodriguescol_int (double *q, char *crysym, int *col);

#ifdef __cplusplus
}
#endif
