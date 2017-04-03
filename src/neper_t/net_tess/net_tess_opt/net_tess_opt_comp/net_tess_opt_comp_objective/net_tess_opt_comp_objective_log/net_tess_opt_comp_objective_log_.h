/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut_t.h"

#include"neper_config.h"

#include"net_tess_opt_comp_objective_log.h"

extern void net_tess_opt_comp_objective_log_time (struct TOPT TOpt);
extern void net_tess_opt_comp_objective_log_dis (struct TOPT TOpt, int id);
extern void net_tess_opt_comp_objective_log_var (struct TOPT TOpt);
extern void net_tess_opt_comp_objective_log_val (struct TOPT TOpt);
extern int  net_tess_opt_comp_objective_log_testiter (struct TOPT TOpt, char*);

#ifdef __cplusplus
}
#endif
