/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTSIMRES_H
#define STRUCTSIMRES_H

struct SIMRES
{
  // result, subresult, file, unknown
  // if unknown, we attempt to process as a built-in result and than as an
  // expression
  char *status;

  // step number
  int step;

  // entity
  char *entity;

  // res (name)
  char *res;

  // expression (if any)
  char *expr;

  // real, vector or tensor
  char *type;

  // access file; for a subresult, of the form parenfile,col=...
  char *file;

  // directory
  char *dir;

  // for a subresult, new file name
  char *newfile;

  // if subresult, parent result, column and file
  char *parentres;
  int parentcol;
  char *parentfile;
  char *parentdir;

  // number of columns
  int colqty;
};
typedef struct SIMRES SIMRES;
#endif /* STRUCTSIMRES_H */
