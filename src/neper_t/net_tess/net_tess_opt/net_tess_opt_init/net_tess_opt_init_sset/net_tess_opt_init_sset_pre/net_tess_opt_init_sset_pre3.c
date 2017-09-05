/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2017, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess_opt_init_sset_pre_.h"

int
net_tess_opt_init_sset_pre_randseed_rand (int *N, int *id, int *poly,
					  int levelqty)
{
  int i;
  time_t t;
  double tmp;
  int Rand;

  gsl_rng *r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, 1);

  /* Rand: long integer number calculated from (N,id,level,poly):
   * Rand is 8 numbers long ; N is put at the left side, id at the
   * right side. If N and id numbers cross, they are added.
   * N=123  and id=12     --> Rand=12300012
   * N=3000 and id=65433  --> Rand=30065433
   * N=3333 and id=88888  --> Rand=33418888
   * To this value is added the product level * poly.  It is put at the
   * left hand side, but with a length of 7.
   */

  // levelqty = 1 and = 2 are kept for backward compatibility
  if (levelqty == 1)
  {
    if (id[1] >= 0)
      Rand = N[1] * pow (10, 7 - ut_num_tenlen (N[1])) + id[1];
    else
    {
      time (&t);
      Rand = t;
    }
  }
  /* this version would provide backward compatibility with v2.0, but
   * different inputs can provide same outputs:
   N = 10 poly =  1 level = 1 id = 1 Rand = 11000001
   N = 10 poly = 10 level = 1 id = 1 Rand = 11000001
   Better to avoid it.

   else if (levelqty == 2)
   {
   if (id[2] >= 0)
   {
   Rand = N[1] * pow (10, 7 - ut_num_tenlen (N[1]))
   + poly[2] * 1 * pow (10, 6 - ut_num_tenlen (poly[2] * 1)) +
   id[2];
   printf ("printf N = %d poly = %d level = %d id = %d Rand = %d\n",
   N[1], poly[2], 1, id[2], Rand);
   }
   else
   {
   time (&t);
   Rand = t;
   }
   }
   */
  else
  {
    tmp = 1;
    for (i = 1; i <= levelqty; i++)
    {
      tmp += 100 * gsl_rng_uniform (r) * N[i]
	+ 100 * gsl_rng_uniform (r) * id[i]
	+ 100 * gsl_rng_uniform (r) * poly[i];
    }
    while (tmp > INT_MAX)
      tmp *= 0.5;

    Rand = (int) tmp;
  }

  if (Rand < 0)
    ut_error_reportbug ();

  gsl_rng_free (r);

  return Rand;
}
