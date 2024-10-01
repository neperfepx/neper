/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_S_H
#define STRUCTIN_S_H

struct IN_S
{
  char *fepxdir;                // FEPX result directory
  char *simdir;                 // simulation directory
  char *mergedir;               // simulation directories to merge

  char *orispace;               // orientation space

  char *entity;               // new entity/entities based on elements

  int entityqty;                // number of different -res* options
  char **entities;              // -res* entities
  char **entityresexpr;         // -res* results

  int stepqty;                  // number of steps
};
typedef struct IN_S IN_S;

#endif /* STRUCTIN_S_H */
