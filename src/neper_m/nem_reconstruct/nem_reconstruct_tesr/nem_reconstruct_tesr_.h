/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neut.h"
#include "nem_reconstruct_tesr.h"
#include "../nem_reconstruct.h"
#include "../../nem/nem.h"

extern void nem_reconstruct_tesr_domain (struct TESR *pTesr);

extern void nem_reconstruct_tesr_tesr (struct TESR Tesr, struct TESS *pTess,
				       struct NODES *pNodes,
				       struct MESH *Mesh, struct NSET *NSet);
