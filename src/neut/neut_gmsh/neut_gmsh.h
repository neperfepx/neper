/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_gmsh.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_GMSH_H
#define NEUT_GMSH_H

/// \brief Rename Gmsh's default configuration file
/// \param action action to apply: "bak" to create a backup, "unbak" to
/// revert a backup
  extern void neut_gmsh_rc (char *action);

/// \brief Get the Gmsh number of a type of element
/// \param type type of element: "tri" for triangle / tetrahedron, "quad"
/// for quadrangle / hexahedron
/// \param dim dimension (0 to 3)
/// \param order order (1 or 2)
/// \retval pnb pointer to the number
/// \return 0 on success, -1 on error
  extern int neut_gmsh_elt_nb (char *type, int dim, int order, int *pnb);

/// \brief Get the Gmsh id of a 2D meshing algorithm
  extern int neut_gmsh_meshalgo2d_nb (char *, int *);

/// \brief Get the Gmsh id of a 3D meshing algorithm
  extern int neut_gmsh_meshalgo3d_nb (char *, int *);

/// \brief Get the Gmsh id of an optimization algorithm
  extern int neut_gmsh_optialgo_nb (char *, int *);

/// \brief Print out a sphere at the Gmsh .geo format
  extern int neut_gmsh_sphere_tess_fprintf (FILE *, double, double);

#endif				/* NEUT_GMSH_H */

#ifdef __cplusplus
}
#endif
