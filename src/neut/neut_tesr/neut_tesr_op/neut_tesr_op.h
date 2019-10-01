/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef  NEUT_VOX_OP_H
#define  NEUT_VOX_OP_H

/// \brief Set a TESR structure to zero.
///
///
///
  extern void neut_tesr_set_zero (struct TESR *pTesr);

/// \brief Allocate a TESR structure to a given size.
///
///
///
  extern void neut_tesr_alloc (struct TESR *pTesr, int dim, int *size,
			       double *vsize);

/// \brief Free a TESR structure.
///
///
///
  extern void neut_tesr_free (struct TESR *pTesr);

/// \brief Copy a TESR structure into another TESR structure.
///
/// The new structure does not need to be preallocated.
///
  extern void neut_tesr_memcpy (struct TESR Vox1, struct TESR *pTesr2);

/// \brief Copy the parameters of a TESR structure into another TESR structure.
///
/// The new structure does not need to be preallocated.
///
  extern void neut_tesr_memcpy_parms (struct TESR Vox1, struct TESR *pTesr2);

/// \brief Scale (= stretch) a TESR structure.
///
/// The size of the raster points is scaled.
///
  extern void neut_tesr_scale (struct TESR *pTesr, double scale1, double
			       scale2, double scale3);

/// \brief Translate a TESR structure.
///
///
///
  extern void neut_tesr_translate (struct TESR *pTesr, double s1, double s2,
                                   double s3);

/// \brief Scale the grid of a TESR structure.
///
/// The number of points are scaled.
///
  extern void neut_tesr_rasterscale (struct TESR *pTesr, double scale1,
				     double scale2, double scale3);

/// \brief Return the neighbouring points of a given point, within a certain cell.
///
///
///
  extern void neut_tesr_cell_pos_neighpos (struct TESR, int cell, int *pos,
					   int type, int ***ppos, int *pqty);

/// \brief Return the neighbouring points of a given point, within certain cells.
///
/// If cell == NULL, then all cells (id different from 0) are considered.
/// type = 1: 26 neighbours, type = 2: 6 neighbours
///
  extern void neut_tesr_cells_pos_neighpos (struct TESR Tesr, int *cell,
					    int cellqty, int *pos, int type,
					    int ***ppos, int *pqty);
  extern void neut_tesr_cells_pos_neighpos_3d (struct TESR Tesr, int *cell,
					       int cellqty, int *pos,
					       int type, int ***ppos,
					       int *pqty);
  extern void neut_tesr_cells_pos_neighpos_2d (struct TESR Tesr, int *cell,
					       int cellqty, int *pos,
					       int type, int ***ppos,
					       int *pqty);

/// \brief Get the point quantity of the cells of a TESR structure.
///
///
///
  extern void neut_tesr_cells_voxqty (struct TESR, int **);

/// \brief Copy the cell of a TESR structure into another TESR structure
///
///
///
  extern void neut_tesr_cell_tesr (struct TESR Tesr, int cell,
				   struct TESR *CellTesr);

  extern int neut_tesr_expr_cells (struct TESR Tesr, char *expr, int **pcell,
				   int *pcellqty);

/// \brief Renumber the cells of a TESR structure to get contiguous numbering.
///
///
///
  extern int neut_tesr_renumber_continuous (struct TESR *pTesr);

  extern int neut_tesr_cell_switch (struct TESR *pTesr, int cell1, int cell2);

  extern void neut_tesr_init_tesrsize (struct TESR *pTesr, struct TESS
				       Domain, int dim, char *voxstring);

  extern void neut_tesr_crop (struct TESR *pTesr, char *shape);
  extern int  neut_tesr_2d (struct TESR *pTesr);

  extern void neut_tesr_autocrop (struct TESR *pTesr);

  extern void neut_tesr_init_cellbbox (struct TESR *pTesr);
  extern void neut_tesr_init_cellcoo  (struct TESR *pTesr);
  extern void neut_tesr_init_cellvol  (struct TESR *pTesr);
  extern void neut_tesr_init_cellconvexity  (struct TESR *pTesr);
  extern void neut_tesr_init_hasvoid  (struct TESR *pTesr);

  extern void net_tesr_cell_segment (struct TESR Tesr, int cell, int
      dim, struct TESR *pSeg);
  extern void net_tesr_segment (struct TESR Tesr, int dim, struct TESR
      *pSeg);

  extern void neut_tesr_rmsat (struct TESR *pTesr, char *rmsat, int verbosity);
  extern void neut_tesr_grow (struct TESR *pTesr, char *grow, int verbosity);
  extern int neut_tesr_grow_neigh (struct TESR *pTesr, int poly, int qty,
				    int itermax, int *piterqty);

  extern void neut_tesr_rmsat_cell_find (struct TESR Tesr, int dim, int *pvoxqty,
					 int ***pvoxpos);
  extern void neut_tesr_rmsat_cell_remove (struct TESR *pTesr, int dim, int voxqty,
					   int **voxpos);
  extern void neut_tesr_tessinter (struct TESR *pTesr, char *crop, int
      verbosity);

  extern void neut_tesr_addbuffer (struct TESR *pTesr, char *addbuffer);

  extern void neut_tesr_rotate (struct TESR *pTesr, double **g);

  extern void neut_tesr_init_voxori (struct TESR *pTesr);

  extern void neut_tesr_oriaverage (struct TESR *pTesr);

#endif				/* NEUT_VOX_OP_H */

#ifdef __cplusplus
}
#endif
