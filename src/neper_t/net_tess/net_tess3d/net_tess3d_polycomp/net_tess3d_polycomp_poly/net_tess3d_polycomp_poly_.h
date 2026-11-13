/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>
#include<limits.h>

#include"neut_t.h"

#include"net_tess3d_polycomp_poly.h"

  extern int net_tess3d_polycomp_poly_test (struct SEEDSET SSet, int id, int nid, double nweight,
                                            double maxdist);
  extern double net_tess3d_polycomp_poly_maxdist (struct POLY Poly, double *coo);
  extern void net_tess3d_polycomp_poly_addchangedneigh (struct TDYN *pTD, int cell);

  extern void net_tess3d_polycomp_poly_cut_badver (double *plane, struct PRIM *pPrim,
                                                   struct POLY Poly, int **pBadVer);
  extern int net_tess3d_polycomp_poly_cut_mod (int Nei, double *plane, struct PRIM *pPrim,
                                               struct POLY *pPoly, int *BadVer);

  extern int net_tess3d_polycomp_poly_cut_mod_veruse (int *VerUse, int VerQty, int *BadVer);
  extern void net_tess3d_polycomp_poly_cut_mod_faceuse (struct POLY Poly, int *VerUse,
                                                        int *FaceUse);
  extern void net_tess3d_polycomp_poly_cut_mod_newface (int Nei, struct POLY *pPoly, double *plane);
  extern void net_tess3d_polycomp_poly_cut_mod_fnlfaces (struct POLY Poly, int *VerUse, int *BadVer,
                                                         int *FnLFaces);
  extern int net_tess3d_polycomp_poly_cut_mod_face (int pFace, int Face, struct POLY *pPoly,
                                                    int **FaceVerNbCpy, int *FaceVerQtyCpy,
                                                    struct PRIM *pPrim, int *VerUse, int *FaceUse,
                                                    int last);
  extern int net_tess3d_polycomp_poly_cut_mod_newver (struct POLY *pPoly, int first, int second,
                                                      int third, double *coo);
  extern void net_tess3d_polycomp_poly_cut_mod_newfaceaddver (struct POLY *pPoly);
  extern void net_tess3d_polycomp_poly_cut_mod_curvedface (struct POLY *pPoly);
  extern void net_tess3d_polycomp_poly_cut_mod_inter (struct POLY Poly, struct PRIM *pPrim,
                                                      int **FaceVerNbCpy, int *FaceVerQtyCpy,
                                                      int p1, int p2, int p3, double *inter);
  extern void net_tess3d_polycomp_poly_cut_mod_compact (struct POLY *pPoly, int *VerUse,
                                                        int *FaceUse);

  extern int net_tess3d_polycomp_poly_cut_mod_faceuse_check (struct POLY Poly, int *VerUse,
                                                             int face);
  extern void net_tess3d_polycomp_poly_cut_mod_newface_qty (struct POLY *pPoly);
  extern void net_tess3d_polycomp_poly_cut_mod_newface_eq (struct POLY *pPoly, double *plane);
  extern void net_tess3d_polycomp_poly_cut_mod_newface_poly (struct POLY *pPoly, int Nei);
  extern void net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns (struct POLY Poly, int ns, int nf,
                                                              int *Ver, int *BadVer);
  extern int net_tess3d_polycomp_poly_cut_mod_fnlfaces_commonfaces (struct POLY Poly, int first,
                                                                    int second, int *res);
  extern int net_tess3d_polycomp_poly_cut_mod_face_vercouple (struct POLY *pPoly, int *VerUse,
                                                              int *FaceUse, int Face, int pFace,
                                                              int *couple1, int *couple2,
                                                              int *faces1, int *faces2,
                                                              int *pNbBadVer);
  extern void net_tess3d_polycomp_poly_cut_mod_face_genpara (struct POLY *pPoly, int Face,
                                                             int NbBadVer);
  extern void net_tess3d_polycomp_poly_cut_mod_face_newver1 (struct POLY *pPoly, int Face);
  extern void net_tess3d_polycomp_poly_cut_mod_face_newver2 (struct POLY *pPoly, int Face, int bel2,
                                                             int next, double *coo);
  extern void net_tess3d_polycomp_poly_cut_mod_face_addbegver (struct POLY *pPoly, int Face,
                                                               int last, int bel2);
  extern void net_tess3d_polycomp_poly_cut_mod_newver_qty (struct POLY *pPoly);
  extern void net_tess3d_polycomp_poly_cut_mod_newver_face (struct POLY *pPoly, int first,
                                                            int second, int third);
  extern void net_tess3d_polycomp_poly_cut_mod_newver_coo (struct POLY *pPoly, double *coo);
  extern void net_tess3d_polycomp_poly_cut_mod_newfaceaddver_qty (struct POLY *pPoly);
  extern void net_tess3d_polycomp_poly_cut_mod_newfaceaddver_nb (struct POLY *pPoly);
  extern void net_tess3d_polycomp_poly_cut_mod_inter_planes (struct POLY Poly, int p1, int p2,
                                                             int p3, double *inter);
  extern void net_tess3d_polycomp_poly_cut_mod_inter_prim (struct POLY Poly, struct PRIM *pPrim,
                                                           int **FaceVerNbCpy, int *FaceVerQtyCpy,
                                                           int p1, int p2, int p3, double *inter);

  extern int net_tess3d_polycomp_poly_cut_mod_fnlfaces_snns_neiver (struct POLY Poly, int face,
                                                                    int sum, int near);
  extern int net_tess3d_polycomp_poly_cut_mod_face_vercouple_rot (struct POLY *pPoly, int *VerUse,
                                                                  int *FaceUse, int Face);

#ifdef __cplusplus
}
#endif
