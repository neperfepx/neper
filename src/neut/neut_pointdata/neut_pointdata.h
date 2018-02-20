/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2018, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/// \file neut_pointdata.h
/// \brief Manipulate data associated to tessellations or meshes (TESSDATA, POINTDATA)
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_POINTDATA_H
#define NEUT_POINTDATA_H

extern void neut_pointdata_set_default (struct POINTDATA *);
extern void neut_pointdata_free (struct POINTDATA *);
extern void neut_pointdata_coldatatype_size (struct POINTDATA PointData,
					     int *psize);

#endif /* NEUT_POINTDATA_H */
