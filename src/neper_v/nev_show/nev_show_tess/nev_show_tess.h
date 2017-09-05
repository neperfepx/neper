/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_show_tess (char **argv, int *pi, struct PRINT *pPrint);
extern void nev_show_tess_seed (char **argv, int *pi, struct TESS Tess,
				struct PRINT *pPrint);
extern void nev_show_tess_ver (char **argv, int *pi, struct TESS Tess,
			       struct PRINT *pPrint);
extern void nev_show_tess_edge (char **argv, int *pi, struct TESS Tess,
				struct PRINT *pPrint);
extern void nev_show_tess_face (char **argv, int *pi, struct TESS Tess,
				struct PRINT *pPrint);
extern void nev_show_tess_faceinter (char **argv, int *pi,
				     struct PRINT *pPrint);
extern void nev_show_tess_poly (char **argv, int *pi, struct TESS Tess,
				struct PRINT *pPrint);
extern void nev_show_tess_cell (char **argv, int *pi, struct TESS Tess,
				struct PRINT *pPrint);
