//
// Created by mmontanari on 29/06/16.
//

#include <stdio.h>
#include <stdlib.h>

#ifndef CGJK_PREDICATES_H
#define CGJK_PREDICATES_H

#endif //CGJK_PREDICATES_H

extern double orient3d(
  double *pa,
  double *pb,
  double *pc,
  double *pd
);

extern double orient2d(
  double *pa,
  double *pb,
  double *pc
);
 
extern void exactinit();
