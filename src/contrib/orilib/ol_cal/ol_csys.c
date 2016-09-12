/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_csys.h"

void
ol_g_refcsys (double **g, double **gs, double **gnew)
{
  double **gsinv = ol_g_alloc ();

  ol_g_inverse (gs, gsinv);
  ol_g_g_g (gsinv, g, gnew);

  ol_g_free (gsinv);

  return;
}


void
ol_q_refcsys (double *q, double *qs, double *qnew)
{
  double *qsinv = ol_q_alloc ();

  ol_q_inverse (qs, qsinv);
  ol_q_q_q (qsinv, q, qnew);

  ol_q_free (qsinv);

  return;
}

void
ol_g_csys (double **g, double **gs, double **gnew)
{
  double **gsinv = ol_g_alloc ();

  ol_g_inverse (gs, gsinv);
  ol_g_g_g (gsinv, g, gnew);
  ol_g_g_g (gnew, gs, gnew);

  ol_g_free (gsinv);

  return;
}


void
ol_q_csys (double *q, double *qs, double *qnew)
{
  double *qsinv = ol_q_alloc ();

  ol_q_inverse (qs, qsinv);
  ol_q_q_q (qsinv, q, qnew);
  ol_q_q_q (qnew, qs, qnew);

  ol_q_free (qsinv);

  return;
}
