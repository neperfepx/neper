/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_SIM_GEN_H
#define NEUT_SIM_GEN_H

#include"neut.h"

/// tells if a simulation is void
extern int neut_sim_isvoid (struct SIM Sim);

/// read a simulation report file
extern void neut_sim_fscanf (char *dir, struct SIM *pSim, char *mode);

/// writes a simulation report file
extern void neut_sim_fprintf (char *dir, struct SIM Sim, char *mode);

/// returns the result of a filename
extern void neut_sim_file_res (char *file, char *res);

/// returns the type of simulation, fepx, merge or sim
extern int neut_sim_name_type (char *name, char **ptype, char **pname, int *prestart);

// get the proper name of an entity (node, element or elset)
extern void neut_sim_entity_entity (char *entity,  char **pentity2);

extern void neut_sim_entity_entitydir (char *entity,  char **pentitydir);

/// is entity node?
extern int neut_sim_entityisnode (char *entity);

/// is entity elt?
extern int neut_sim_entityiselt (char *entity);

/// is entity elset?
extern int neut_sim_entityiselset (char *entity);

/// is entity mesh?
extern int neut_sim_entityismesh (char *entity);

/// does entity exist?
extern int neut_sim_entity_exist (struct SIM Sim, char *entity);

extern void neut_sim_entity_res (struct SIM Sim, char *entity, char ***pres, int *presqty);

/// returns the type of a result: int, real, ori, vector, tensor
extern int neut_sim_res_type (struct SIM Sim, char *entity, char *res, char **ptype, int *pcolqty);

// get the input file names
extern void neut_sim_input_files (struct SIM Sim, char ***pfullfiles, char ***pfiles, int *pfileqty);

extern void neut_sim_orispace (struct SIM Sim, char **porispace);

extern int neut_sim_entity_pos (struct SIM Sim, char *entity, int *ppos);

extern int neut_sim_entity_type (struct SIM Sim, char *entity, char **ptype);

extern int neut_sim_entity_qty (struct SIM Sim, char *entity, int *pqty);

extern void neut_sim_entity_name (char *entity, char **pname);

extern int neut_sim_entity_hasexpr (struct SIM Sim, char *entity);

extern int neut_sim_entity_internal (char *entity);

extern int neut_sim_entity_parent (struct SIM Sim, char *entity, char **pparent);

extern int neut_sim_entity_id_res_val (struct SIM Sim, char *entity, int id, char *res, double *pval);

#endif /* NEUT_SIM_GEN_H */
