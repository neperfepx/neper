/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2020, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polymod_poly_.h"

/* Determination of CompVer and CompVerInv
 */
void
Pm2PCompVerNVerInv (struct POLYMOD Polymod, int *CompVer, int *CompVerInv)
{
  int i;                        /* mute variable */
  int RealVerQty = 0;           /* Real quantity of vertices (i.e. amount of useful */
  /* vertices of the polyhedron. */

  /* sum i in poly    == sum CompVer[i]    in polymod
   * sum i in polymod == sum CompVerInv[i] in poly
   */
  for (i = 1; i <= Polymod.VerQty; i++)
    if (Polymod.VerUse[i] != 0)
    {
      RealVerQty++;
      CompVer[RealVerQty] = i;
      CompVerInv[i] = RealVerQty;
    }

  CompVer[0] = RealVerQty;

  return;
}

/* Determination of CompFace and CompFaceInv
 */
void
Pm2PCompFaceNFaceInv (struct POLYMOD Polymod, int *CompFace, int *CompFaceInv)
{
  int i;                        /* mute variable */
  int RealFaceQty = 0;          /* Real quantity of faces (i.e. amount of useful */
  /* faces of the polyhedron. */

  /* face i in poly    == face CompFace[i]    in polymod
   * face i in polymod == face CompFaceInv[i] in poly
   */
  for (i = 1; i <= Polymod.FaceQty; i++)
    if (Polymod.FaceUse[i] != 0)
    {
      RealFaceQty++;
      CompFace[RealFaceQty] = i;
      CompFaceInv[i] = RealFaceQty;
    }

  CompFace[0] = RealFaceQty;

  return;
}
