/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2013, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_S_H
#define STRUCTIN_S_H

struct IN_S
{
  char *fepxdir;                // FEPX result directory
  char *simdir;                 // simulation directory

  char *noderes;                // nodal results
  char *eltres;                 // elemental results
  char *elsetres;               // element set results
};
typedef struct IN_S IN_S;

#endif /* STRUCTIN_S_H */
