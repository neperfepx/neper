/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include "../structIn_s.h"

  extern void nes_in_set_zero (struct IN_S *pIn);
  extern void nes_in_free (struct IN_S In);

  extern void nes_loadmesh (struct SIM Sim, struct NODES *pNodes, struct MESH *pMesh);

#ifdef __cplusplus
}
#endif
