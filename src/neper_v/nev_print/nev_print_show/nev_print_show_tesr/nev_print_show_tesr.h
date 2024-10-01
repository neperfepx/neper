/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nev_print_show_tesr (char *expr, struct PRINT *pPrint);
extern void nev_print_show_tesr_cell (char *expr, struct TESR Tesr,
				struct PRINT *pPrint);
extern void nev_print_show_tesr_vox (char *option, char *expr, struct TESR Tesr,
                               struct PRINT *pPrint);

extern void nev_print_show_tesr_edge (char *expr, struct PRINT *pPrint);
