/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_png_segment (FILE *, double *, double *, double, char *);
extern void nev_print_png_segment_wsph (FILE *, double *, double *, double,
				    char *);
extern void nev_print_png_cube (FILE *, double *, double *, char *, double, char *);
extern void nev_print_png_cyl (FILE *, double *, double *, char *);
extern void nev_print_png_arr (FILE *, double *, double *, char *);
extern void nev_print_png_tor (FILE *, double *, double *, char *);
extern void nev_print_png_disc (FILE *, double *, double *, char *);
extern void nev_print_png_sphere (FILE *, double *, double, char *);
extern void nev_print_png_ellipsoid (FILE *, double *, double *, char *);
extern void nev_print_png_triangle_edge (FILE *, double *, double *, double *,
				     char *, char *);
extern void nev_print_png_triangle (FILE *, double *, double *, double *, char *,
				char *, char *);
extern void nev_print_png_polygon (FILE * file, double *eq, int qty, double **coo,
                   char *texture, double edge_rad, char *edge_texture, int pt,
                   double *p, double iedge_rad, char *iedge_texture);
extern void nev_print_png_polygon_edge (FILE *, int, double **, double, char *);

extern void nev_print_png_tet_edge (FILE *, double *, double *, double *,
				double *, char *, char *);
extern void nev_print_png_tet (FILE *, double *, double *, double *, double *,
			   char *, char *, char *);

extern void nev_print_png_mesh2d (FILE *, struct NODES, struct MESH, int *,
			      int **, double *, char *, struct PRINT);
extern void nev_print_png_mesh1d (FILE *file, struct NODES Nodes, struct MESH Mesh, int *showelt,
                      int **Col, double *Rad, struct PRINT Print);

extern void nev_print_png_hcp (FILE * file, double *coo, double *rad, char *texture,
                              double brad, char *bordertexture);
extern void nev_print_png_cone (FILE * file, double *coo1, double *coo2, double rad,
                char *edge_texture);
