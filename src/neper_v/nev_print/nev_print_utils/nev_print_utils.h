/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_segment (FILE *, double *, double *, char *, char *);
extern void nev_print_segment_wsph (FILE *, double *, double *, char *,
				    char *);
extern void nev_print_arrow (FILE *, double *, double *, char *, char *);

extern void nev_print_cube (FILE *, double *, double *, char *);
extern void nev_print_cyl (FILE *, double *, double *, char *);
extern void nev_print_arr (FILE *, double *, double *, char *);
extern void nev_print_tor (FILE *, double *, double *, char *);
extern void nev_print_disc (FILE *, double *, double *, char *);
extern void nev_print_sphere (FILE *, double *, char *, char *);
extern void nev_print_ellipsoid (FILE *, double *, double *, char *);
extern void nev_print_triangle_edge (FILE *, double *, double *, double *,
				     char *, char *);
extern void nev_print_triangle (FILE *, double *, double *, double *, char *,
				char *, char *);
extern void nev_print_polygon (FILE *, double*, int, double **, char *, char *, char *,
			       int, double *, char *, char *);
extern void nev_print_polygon_edge (FILE *, int, double **, char *, char *);

extern void nev_print_tet_edge (FILE *, double *, double *, double *,
				double *, char *, char *);
extern void nev_print_tet (FILE *, double *, double *, double *, double *,
			   char *, char *, char *);

extern void nev_print_mesh2d (FILE *, struct NODES, struct MESH, int *,
			      int **, char *, int);

extern void nev_print_scale (FILE *, char *, char *, char *);
