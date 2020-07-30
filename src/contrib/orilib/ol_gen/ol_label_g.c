/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_label_g.h"

int
ol_label_g (char *label, double **g)
{
  int status = 0;
  double *r = ol_r_alloc ();
  double *des = ut_alloc_1d (4);
  double *des2 = ut_alloc_1d (4);
  int *m = ol_m_alloc ();

  if (!strcmp (label, "cube") || !strcmp (label, "Cube"))
    ol_g_set_id (g);
  else if (!strcmp (label, "goss") || !strcmp (label, "Goss"))
  {
    ol_r_set_this (r, 1, 0, 0);
    ol_rtheta_g (r, 45, g);
  }
  else if (!strcmp (label, "u") || !strcmp (label, "U"))
  {
    ol_r_set_this (r, 0, 1, 0);
    ol_rtheta_g (r, 45, g);
  }
  else if (!strcmp (label, "45NDcube") || !strcmp (label, "45NDCube"))
  {
    ol_r_set_this (r, 0, 0, 1);
    ol_rtheta_g (r, 45, g);
  }
  else if (!strcmp (label, "45NDcube") || !strcmp (label, "45NDCube"))
  {
    ol_r_set_this (r, 0, 0, 1);
    ol_rtheta_g (r, 45, g);
  }
  else if (!strcmp (label, "s1") || !strcmp (label, "S1"))
  {
    ol_m_set_this (m, 1, 2, 3, 6, 3, -4);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "s2") || !strcmp (label, "S2"))
  {
    ol_m_set_this (m, -1, 2, 3, 6, -3, 4);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "s3") || !strcmp (label, "S3"))
  {
    ol_m_set_this (m, 1, -2, 3, 6, -3, -4);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "s4") || !strcmp (label, "S4"))
  {
    ol_m_set_this (m, -1, -2, 3, 6, 3, 4);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "brass1") || !strcmp (label, "Brass1"))
  {
    ol_m_set_this (m, 1, 1, 0, 1, -1, 2);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "brass2") || !strcmp (label, "Brass2"))
  {
    ol_m_set_this (m, -1, 1, 0, 1, 1, -2);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "copper1") || !strcmp (label, "Copper1"))
  {
    ol_m_set_this (m, 1, 1, 2, 1, 1, -1);
    ol_m_g (m, g);
  }
  else if (!strcmp (label, "copper2") || !strcmp (label, "Copper2"))
  {
    ol_m_set_this (m, -1, 1, 2, 1, -1, 1);
    ol_m_g (m, g);
  }
  else if (!strncmp (label, "g(", 2))
  {
    sscanf (label, "g(%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf)",
	    g[0], g[0] + 1, g[0] + 2,
	    g[1], g[1] + 1, g[1] + 2,
	    g[2], g[2] + 1, g[2] + 2);
  }
  else if (!strncmp (label, "e(", 2))
  {
    sscanf (label, "e(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_e_g (des, g);
  }
  else if (!strncmp (label, "ek(", 3))
  {
    sscanf (label, "ek(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_ek_e (des, des);
    ol_e_g (des, g);
  }
  else if (!strncmp (label, "er(", 3))
  {
    sscanf (label, "er(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_er_e (des, des);
    ol_e_g (des, g);
  }
  else if (!strncmp (label, "m(", 2))
  {
    sscanf (label, "m(%d,%d,%d,%d,%d,%d)",
	    m, m + 1, m + 2, m + 3, m + 4, m + 5);
    ol_m_g (m, g);
  }
  else if (!strncmp (label, "rtheta(", 7))
  {
    sscanf (label, "rtheta(%lf,%lf,%lf,%lf)", des, des + 1, des + 2, des + 3);
    ol_rtheta_g (des, des[3], g);
  }
  else if (!strncmp (label, "R(", 2))
  {
    sscanf (label, "R(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_R_g (des, g);
  }
  else if (!strncmp (label, "q(", 2))
  {
    sscanf (label, "q(%lf,%lf,%lf,%lf)", des, des + 1, des + 2, des + 3);
    ol_q_g (des, g);
  }
  else if (!strncmp (label, "rotmat(", 7))
  {
    sscanf (label, "rotmat(%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf)",
	    g[0], g[0] + 1, g[0] + 2,
	    g[1], g[1] + 1, g[1] + 2,
	    g[2], g[2] + 1, g[2] + 2);
  }
  else if (!strncmp (label, "euler-bunge(", 12))
  {
    sscanf (label, "euler-bunge(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_e_g (des, g);
  }
  else if (!strncmp (label, "euler-kocks(", 12))
  {
    sscanf (label, "euler-kocks(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_ek_e (des, des);
    ol_e_g (des, g);
  }
  else if (!strncmp (label, "euler-roe(", 10))
  {
    sscanf (label, "euler-roe(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_er_e (des, des);
    ol_e_g (des, g);
  }
  else if (!strncmp (label, "miller(", 7))
  {
    sscanf (label, "miller(%d,%d,%d,%d,%d,%d)",
	    m, m + 1, m + 2, m + 3, m + 4, m + 5);
    ol_m_g (m, g);
  }
  else if (!strncmp (label, "axis-angle(", 11))
  {
    sscanf (label, "axis-angle(%lf,%lf,%lf,%lf)", des, des + 1, des + 2, des + 3);
    ol_rtheta_g (des, des[3], g);
  }
  else if (!strncmp (label, "rodrigues(", 10))
  {
    sscanf (label, "rodrigues(%lf,%lf,%lf)", des, des + 1, des + 2);
    ol_R_g (des, g);
  }
  else if (!strncmp (label, "quaternion(", 11))
  {
    sscanf (label, "quaternion(%lf,%lf,%lf,%lf)", des, des + 1, des + 2, des + 3);
    ol_q_g (des, g);
  }
  else
    status = -1;

  ol_r_free (r);
  ol_m_free (m);
  ut_free_1d (&des);
  ut_free_1d (&des2);

  return status;
}
