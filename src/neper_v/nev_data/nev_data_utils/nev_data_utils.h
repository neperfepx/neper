/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern int nev_data_string_entity_type (char *, char *, char *);
extern void nev_data_entity_type_dataid (struct DATA **, int *, char *,
					 char *, int *);
extern void nev_data_id_colour (double **, int, int **);
extern void nev_data_col_colour (double **, int, int **);
extern void nev_data_ori_colour (double **, int, char *, int **);
extern void nev_data_rad_radius (double **, int, double *);
extern void nev_data_tr_tr (double **, int, double *);
extern void nev_data_scal_colour (double **, int *, int, char *, char *,
				  int **, char **);
extern int nev_data_type_size (char *, int *);

extern void nev_data_coo_coo (double **, double **, double, int, double **);
extern void nev_data_disp_coo (double **, double **, double, int, double **);

extern void nev_data_typearg_args (char *, char *, char **, char **, char **);

extern void nev_data_fscanf_ori (char *value, int qty, double **dataembed,
                                 double ***pColData, char **pColDataType);
extern void nev_data_fscanf_ori_tesr (struct TESR Tesr, char *value, int qty,
                                      double ****dataembedvox,
                                      double **dataembedcell,
                                      double ***pColData, char **pColDataType);
extern void nev_data_fscanf_ori_file (char *value, int qty, double ***pColData,
                                      char **pColDataType);
