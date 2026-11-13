/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2026, Romain Quey, CNRS. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SIM_SIMRES_H
#define NEUT_SIM_SIMRES_H

#include"neut.h"

extern void neut_sim_simres (struct SIM Sim, char *entity, char *res, struct SIMRES *pSimRes);

/// \brief Get the number of columns of a result type
/// \param type: int, real, vector, skewtensor, symtensor, tensor or ori
/// \param pcolqty: number of columns, 0 if not imposed by the type
/// \return 0 on success and -1 if the type is not valid
extern int neut_sim_simres_type_colqty (char *type, int *pcolqty);

/// \brief Get the type of a result of known name
/// \param res: result
/// \param ptype: type (can be NULL)
/// \return 0 on success and -1 if the name is not known
extern int neut_sim_simres_knowntype (char *res, char **ptype);

#endif /* NEUT_SIM_SIMRES_H */
