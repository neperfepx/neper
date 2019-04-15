/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTMTESS_H
#define STRUCTMTESS_H

/// \file neut_mtess_struct.h
/// \brief MTESS structure declaration
/// \details MTESS contains the description of a multilevel tessellation.
/// \author Romain Quey
/// \bug No known bugs
/// \todo Comment

  struct MTESS
  {
    /// \brief Number of levels
    /// \details 1 for a regular tessellation
    int LevelQty;

    /// \brief Number of tesses at each level
    /// \details indexed 1
    int *LevelTessQty;

    /// \brief Identifiers of the tessellations at each level
    /// \details indexed 1 1
    int **LevelTess;

    /// \brief Grand number of tessellations
    /// \details 1 for a regular tessellation
    int TessQty;

    /// \brief Domain info of each tessellation
    /// \details The size of the array is MTess.TessQty + 1 by 2.
    /// Reminding that the domain is a polyhedron of a lower-level tessellation,
    /// for tessellation number i, TessDom[i][0] and TessDom[i][1] correspond
    /// to the id of the parent tessellation and the id of the poly in
    /// the parent tessellation, respectively.
    int **TessDom;

    /// \brief Tessellation info of a domain
    /// \details For a domain defined by a poly `polyid' of a tessellation
    /// `tessid', DomTess[tessid][polyid] is the id of the tessellation
    /// built in the domain.
    /// defined only for a multitess
    int **DomTess;

    /// \brief Grand number of TessD
    /// \details 0 for a regular tessellation
    int TessDQty;

    /// \brief Numbers of the domain vertices in the domain tessellation
    /// (ver number in the domain tessellation)
    int **TessDomVerNb;

    /// \brief Numbers of the domain edges in the domain tessellation
    /// (edge number in the domain tessellation)
    int **TessDomEdgeNb;

    /// \brief Numbers of the domain faces in the domain tessellation
    /// (face number in the domain tessellation)
    int **TessDomFaceNb;

    /// \brief For a given tessellation, for a given face, numbers of the
    /// domain faces in the 2 higher-level tessellations.
    /// Ordered as in FacePoly.
    /// defined only for a multitess
    int ***DomTessFaceNb;

  };
  typedef struct MTESS MTESS;

#endif				/* STRUCTMTESS_H */

#ifdef __cplusplus
}
#endif
