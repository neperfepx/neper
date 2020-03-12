/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_des_gen.h"

int
ol_des_size (char *des)
{
  if (!strcmp (des, "g"))
    return 9;
  else if (!strcmp (des, "e") || !strcmp (des, "eb")
           || !strcmp (des, "er") || !strcmp (des, "ek"))
    return 3;
  else if (!strcmp (des, "m"))
    return 6;
  else if (!strcmp (des, "rtheta"))
    return 4;
  else if (!strcmp (des, "R"))
    return 3;
  else if (!strcmp (des, "q"))
    return 4;
  else
    return -1;
}
