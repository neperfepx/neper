/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2021, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"

#include"net_tess_file.h"
#include"../net_tess3d/net_tess3d_domain/net_tess3d_domain.h"

extern void net_tess_file_tess (int level, char *morpho, struct TESS *Tess,
                                int TessId, struct SEEDSET *SSet);

extern void net_tess_file_updatedomain (struct MTESS *MTess, struct TESS *Tess,
                                        int dtess, int dcell, int TessId);

extern int net_tess_file_mtess (struct MTESS *pMTess, struct TESS *Tess,
                                int dtess, int dcell, int TessId);

extern int net_tess_file_mtess_vers (struct MTESS *pMTess, struct TESS *Tess,
                                     int dtess, int dcell, int TessId);

extern int net_tess_file_mtess_edges (struct MTESS *pMTess, struct TESS *Tess,
                                      int dtess, int dcell, int TessId);

extern int net_tess_file_mtess_faces (struct MTESS *pMTess, struct TESS *Tess,
                                      int dtess, int dcell, int TessId);
