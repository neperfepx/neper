/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_DATA_GEN_H
#define NEUT_DATA_GEN_H

  extern void neut_data_coldatatype_size (struct DATA MeshData,
					      int *psize);

  extern int neut_data_value_type (struct SIM Sim, char *entity,
                                   char *attribute, char *value, char **ptype);

  extern void neut_data_datastring_type_value (char *entity, char *attribute,
      char *datastring, char **pdatatype, char **pdatavalue);

  extern void neut_data_disp_coo (double **ref, double **data, double fact, int
                                  size, double **coo);

  extern void neut_data_coo_coo (double **ref, double **data, double fact, int size,
                  double **coo);

  extern void neut_data_real_color (double **data, int *datadef, int size, char *scale,
                      char *scheme, int **Col, char **pscale);

  extern void neut_data_ori_color (double **data, int size, char *scheme, int **Col);
  extern void neut_data_ori_color_rodrigues (double **data, int size, char *scheme, int **Col);
  extern void neut_data_ori_color_axis (double **data, int size, char *scheme, int **Col);
  extern void neut_data_ori_color_angle (double **data, int size, char *scheme, int **Col);
  extern void neut_data_ori_color_axisangle (double **data, int size, char *scheme, int **Col);
  extern void neut_data_ori_color_ipf (double **data, int size, char *scheme, int **Col);

  extern void neut_data_rad_radius (double **data, int size, double *Rad);

  extern void neut_data_tr_tr (double **data, int size, double *trs);

  extern void neut_data_col_color (double **data, int size, int **Col);

  extern void neut_data_int_color (double **data, int size, int *UnDefCol, int **Col);

  extern int neut_data_type_size (char *type, int *psize);

  extern int neut_data_string_entity_attribute (char *string, char *entity,
                                                char **inputs, int inputqty, char *attribute);

  extern void neut_data_colscheme_asygradient (char *colscheme, int stepqty, char **pasygradient);

#endif				/* NEUT_DATA_GEN_H */

#ifdef __cplusplus
}
#endif
