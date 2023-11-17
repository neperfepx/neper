/* This file is part of the Neper software sizeage. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void net_tess_opt_init_sset_coo (struct MTESS MTess, struct TESS
    *Tess, int dtess, int dcell, struct TOPT *pTOpt, char *var, int pos,
    char *cooexpr, double *rad, int *qty);

extern int net_tess_opt_init_sset_coo_bcc (struct TESS Dom, int n, double ***pcoo);
