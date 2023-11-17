/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"
#include"net_tess/net_tess3d/net_tesl/net_tesl.h"
#include"net_tess/net_tess3d/net_tess3d_domain/net_tess3d_domain.h"
#include"net_tess/net_tess3d/net_tess3d_periodic/net_tess3d_periodic.h"

#include"neper_config.h"

#include"net_tess_opt_post.h"

extern void net_tess_opt_post_tess (struct MTESS *pMTess, struct TESS *Tess,
		   int dtess, int dcell, int tessid,
		   struct TOPT TOpt, struct SEEDSET *SSet);

extern void net_tess_opt_post_modes (struct TOPT TOpt, struct TESS
    *Tess, int tessid);
