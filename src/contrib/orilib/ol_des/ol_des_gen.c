/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_des_gen.h"

int
ol_des_size (char* des0)
{
  char *des = NULL;

  ut_string_string (des0, &des);
  ut_string_fnr (des, ':', '\0');

  if (! strcmp (des, "label") || !strcmp (des, "name"))
    return 1;
  if (! strcmp (des, "g") || !strcmp (des, "rotmat"))
    return 9;
  else if (!strcmp (des, "e")  || !strcmp (des, "eb")
	|| !strcmp (des, "er") || !strcmp (des, "ek")
	|| !strncmp (des, "euler", 5))
    return 3;
  else if (!strcmp (des, "m") || !strcmp (des, "miller"))
    return 6;
  else if (!strcmp (des, "rtheta") || !strcmp (des, "axis-angle"))
    return 4;
  else if (!strcmp (des, "R") || !strcmp (des, "rodrigues"))
    return 3;
  else if (!strcmp (des, "q") || !strcmp (des, "quaternion"))
    return 4;
  else
    return -1;
}

int
ol_q_des (double *q, char *des, double *vect)
{
  int status = 0;

  if (! strcmp (des, "g") || !strcmp (des, "rotmat"))
    ol_q_gv (q, vect);
  else if (!strcmp (des, "e") || !strcmp (des, "eb") || !strcmp (des, "euler"))
    ol_q_e (q, vect);
  else if (!strcmp (des, "er"))
  {
    ol_q_e (q, vect);
    ol_e_er (vect, vect);
  }
  else if (!strcmp (des, "ek"))
  {
    ol_q_e (q, vect);
    ol_e_ek (vect, vect);
  }
  else if (!strcmp (des, "rtheta") || !strcmp (des, "axis-angle"))
    ol_q_rthetav (q, vect);
  else if (!strcmp (des, "R") || !strcmp (des, "rodrigues"))
    ol_q_R (q, vect);
  else if (!strcmp (des, "q") || !strcmp (des, "quaternion"))
    ol_q_memcpy (q, vect);
  else
    status = -1;

  return status;
}
