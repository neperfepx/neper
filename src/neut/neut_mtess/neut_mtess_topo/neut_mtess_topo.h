/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef NEUT_MTESS_TOPO_H
#define NEUT_MTESS_TOPO_H

/// \brief Get the domain at a given level of the poly of a tess
///
///
///
  extern int neut_mtess_tess_level_dom (struct MTESS MTess, struct TESS
					Tess, int level, int *dom);
  extern int neut_mtess_tess_doms (struct MTESS MTess, struct TESS Tess,
				   int **doms);
  extern int neut_mtess_tess_domtesses (struct MTESS MTess, struct TESS Tess,
				   int *domtesses);
  extern int neut_mtess_tess_dompolys (struct MTESS MTess, struct TESS Tess,
				   int *dompolys);

  extern int neut_mtess_tess_poly_childtesses (struct MTESS MTess, struct TESS *Tess,
                                  int tessid, int **ptess, int*
				  ptessqty);

#endif				/* NEUT_MTESS_TOPO_H */

#ifdef __cplusplus
}
#endif
