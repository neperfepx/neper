/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_input (struct IN_M *, int, char **, int, char **);

extern void nem_input_init_dim_tess (struct IN_M *pIn, struct TESS Tess);
extern void nem_input_init_dim_tesr (struct IN_M *pIn, struct TESR Tesr);
extern int nem_input_init_dim_mesh (struct IN_M *pIn, struct MESH *Mesh);

extern int nem_input_outputismesh (struct IN_M In);
