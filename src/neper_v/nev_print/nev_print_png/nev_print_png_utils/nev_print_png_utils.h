/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_png_segment (FILE *, double *, double *, char *, char *);
extern void nev_print_png_segment_wsph (FILE *, double *, double *, char *,
				    char *);
extern void nev_print_png_arrow (FILE *, double *, double *, char *, char *);

extern void nev_print_png_cube (FILE *, double *, double *, char *, double, char *);
extern void nev_print_png_cyl (FILE *, double *, double *, char *);
extern void nev_print_png_arr (FILE *, double *, double *, char *);
extern void nev_print_png_tor (FILE *, double *, double *, char *);
extern void nev_print_png_disc (FILE *, double *, double *, char *);
extern void nev_print_png_sphere (FILE *, double *, char *, char *);
extern void nev_print_png_ellipsoid (FILE *, double *, double *, char *);
extern void nev_print_png_triangle_edge (FILE *, double *, double *, double *,
				     char *, char *);
extern void nev_print_png_triangle (FILE *, double *, double *, double *, char *,
				char *, char *);
extern void nev_print_png_polygon (FILE *, double*, int, double **, char *, char *, char *,
			       int, double *, char *, char *);
extern void nev_print_png_polygon_edge (FILE *, int, double **, char *, char *);

extern void nev_print_png_tet_edge (FILE *, double *, double *, double *,
				double *, char *, char *);
extern void nev_print_png_tet (FILE *, double *, double *, double *, double *,
			   char *, char *, char *);

extern void nev_print_png_mesh2d (FILE *, struct NODES, struct MESH, int *,
			      int **, char *, int);

extern void nev_print_png_scale (FILE *, char *, char *, char *);
