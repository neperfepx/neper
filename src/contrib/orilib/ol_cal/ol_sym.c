/* This file is part of the 'orilib' software. */
/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_sym.h"

// symmetry operators, made global variables for computational efficiency

double T_1[1][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}
};

double T2_m[2][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}}
};

double Tmmm[4][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
  {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}}
};

double T4_m[5][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
  {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
  {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}}
};

double T4_mmm[8][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
  {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
  {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}},
  {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
  {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
  {{0, -1, 0}, {-1, 0, 0}, {0, 0, -1}}
};

double T_3[3][3][3] = {
  {{1.0, 0, 0}, {0, 1.0, 0}, {0, 0, 1}},
  {{-0.5, OL_S3O2, 0}, {-OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{-0.5, -OL_S3O2, 0}, {OL_S3O2, -0.5, 0}, {0, 0, 1}}
};

double T_3m[6][3][3] = {
  {{1.0, 0, 0}, {0, 1.0, 0}, {0, 0, 1}},
  {{-0.5, OL_S3O2, 0}, {-OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{-0.5, -OL_S3O2, 0}, {OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{1.0, 0, 0}, {0, -1.0, 0}, {0, 0, -1}},
  {{-0.5, OL_S3O2, 0}, {OL_S3O2, 0.5, 0}, {0, 0, -1}},
  {{-0.5, -OL_S3O2, 0}, {-OL_S3O2, 0.5, 0}, {0, 0, -1}}
};

double T6_m[6][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{0.5, OL_S3O2, 0}, {-OL_S3O2, 0.5, 0}, {0, 0, 1}},
  {{-0.5, OL_S3O2, 0}, {-OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
  {{-0.5, -OL_S3O2, 0}, {OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{0.5, -OL_S3O2, 0}, {OL_S3O2, 0.5, 0}, {0, 0, 1}},
};

double T6_mmm[12][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{0.5, OL_S3O2, 0}, {-OL_S3O2, 0.5, 0}, {0, 0, 1}},
  {{-0.5, OL_S3O2, 0}, {-OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
  {{-0.5, -OL_S3O2, 0}, {OL_S3O2, -0.5, 0}, {0, 0, 1}},
  {{0.5, -OL_S3O2, 0}, {OL_S3O2, 0.5, 0}, {0, 0, 1}},
  {{0.5, -OL_S3O2, 0}, {-OL_S3O2, -0.5, 0}, {0, 0, -1}},
  {{-0.5, -OL_S3O2, 0}, {-OL_S3O2, 0.5, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
  {{-0.5, OL_S3O2, 0}, {OL_S3O2, 0.5, 0}, {0, 0, -1}},
  {{0.5, OL_S3O2, 0}, {OL_S3O2, -0.5, 0}, {0, 0, -1}},
  {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}}
};

double Tm_3[12][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},
  {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
  {{0, -1, 0}, {0, 0, -1}, {1, 0, 0}},
  {{0, 0, 1}, {-1, 0, 0}, {0, -1, 0}},
  {{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}},
  {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
  {{0, 1, 0}, {0, 0, -1}, {-1, 0, 0}},
  {{0, 0, -1}, {1, 0, 0}, {0, -1, 0}},
  {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}}
};

double Tcubic[24][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},    /*  1:   0/[100]  */
  {{1, 0, 0}, {0, 0, 1}, {0, -1, 0}},   /*  2:  90/[100]  */
  {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},  /*  3: 180/[100]  */
  {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},   /*  4: 270/[100]  */
  {{0, 0, -1}, {0, 1, 0}, {1, 0, 0}},   /*  5:  90/[010]  */
  {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},  /*  6: 180/[010]  */
  {{0, 0, 1}, {0, 1, 0}, {-1, 0, 0}},   /*  7: 270/[010]  */
  {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}},   /*  8:  90/[001]  */
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},  /*  9: 180/[001]  */
  {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},   /* 10: 270/[001]  */
  {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},   /* 11: 180/[110]  */
  {{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},   /* 13: 180/[011]  */
  {{0, 0, 1}, {0, -1, 0}, {1, 0, 0}},   /* 15: 180/[101]  */
  {{0, -1, 0}, {-1, 0, 0}, {0, 0, -1}}, /* 12: 180/[-110] */
  {{-1, 0, 0}, {0, 0, -1}, {0, -1, 0}}, /* 14: 180/[0-11] */
  {{0, 0, -1}, {0, -1, 0}, {-1, 0, 0}}, /* 16: 180/[10-1] */
  {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},    /* 17: 120/[111]  */
  {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},    /* 18: 240/[111]  */
  {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},  /* 19: 120/[-111] */
  {{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}},  /* 20: 240/[-111] */
  {{0, 0, 1}, {-1, 0, 0}, {0, -1, 0}},  /* 21: 120/[1-11] */
  {{0, -1, 0}, {0, 0, -1}, {1, 0, 0}},  /* 22: 240/[1-11] */
  {{0, 0, -1}, {1, 0, 0}, {0, -1, 0}},  /* 23: 240/[11-1] */
  {{0, 1, 0}, {0, 0, -1}, {-1, 0, 0}}   /* 24: 240/[11-1] */
};

double U_1[1][4] = {
  {1, 0, 0, 0}
};

double U2_m[2][4] = {
  {1, 0, 0, 0},
  {0, 0, 0, 1}
};

double Ummm[4][4] = {
  {1, 0, 0, 0},
  {0, 0, 0, 1},
  {0, 1, 0, 0},
  {0, 0, 1, 0}
};

double U4_m[4][4] = {
  {1, 0, 0, 0},
  {0, 0, 0, 1},
  {OL_IS2, 0, 0, -OL_IS2},
  {OL_IS2, 0, 0, OL_IS2}
};

double U4_mmm[8][4] = {
  {1, 0, 0, 0},
  {0, 0, 0, 1},
  {OL_IS2, 0, 0, -OL_IS2},
  {OL_IS2, 0, 0, OL_IS2},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, OL_IS2, OL_IS2, 0},
  {0, OL_IS2, -OL_IS2, 0}
};

double U_3[3][4] = {
  {1, 0, 0, 0},
  {0.5, 0, 0, OL_S3O2},
  {0.5, 0, 0, -OL_S3O2}
};

double U_3m[6][4] = {
  {1, 0, 0, 0},
  {0.5, 0, 0, OL_S3O2},
  {0.5, 0, 0, -OL_S3O2},
  {0, 1, 0, 0},
  {0, -0.5, -OL_S3O2, 0},
  {0, -0.5, OL_S3O2, 0}
};

double U6_m[6][4] = {
  {1, 0, 0, 0},
  {OL_S3O2, 0, 0, 0.5},
  {0.5, 0, 0, OL_S3O2},
  {0, 0, 0, 1},
  {0.5, 0, 0, -OL_S3O2},
  {OL_S3O2, 0, 0, -0.5},
};

double U6_mmm[12][4] = {
  {1, 0, 0, 0},
  {OL_S3O2, 0, 0, 0.5},
  {0.5, 0, 0, OL_S3O2},
  {0, 0, 0, 1},
  {0.5, 0, 0, -OL_S3O2},
  {OL_S3O2, 0, 0, -0.5},
  {0, OL_S3O2, -0.5, 0},
  {0, -0.5, OL_S3O2, 0},
  {0, 0, 1, 0},
  {0, 0.5, OL_S3O2, 0},
  {0, OL_S3O2, 0.5, 0},
  {0, 1, 0, 0}
};

double Um_3[12][4] = {
  {1, 0, 0, 0},
  {0.5, 0.5, 0.5, 0.5},
  {0.5, -0.5, -0.5, -0.5},
  {0.5, -0.5, 0.5, -0.5},
  {0.5, 0.5, -0.5, 0.5},
  {0.5, 0.5, -0.5, -0.5},
  {0.5, -0.5, 0.5, 0.5},
  {0.5, -0.5, -0.5, 0.5},
  {0.5, 0.5, 0.5, -0.5},
  {0, 1, 0, 0},
  {0, 0, 1, 0},
  {0, 0, 0, 1}
};

double Um3m[24][4] = {
  {1, 0, 0, 0},
  {OL_IS2, OL_IS2, 0, 0},
  {0, 1, 0, 0},
  {-OL_IS2, OL_IS2, 0, 0},
  {OL_IS2, 0, OL_IS2, 0},
  {0, 0, 1, 0},
  {-OL_IS2, 0, OL_IS2, 0},
  {OL_IS2, 0, 0, OL_IS2},
  {0, 0, 0, 1},
  {-OL_IS2, 0, 0, OL_IS2},
  {0, OL_IS2, OL_IS2, 0},
  {0, 0, OL_IS2, OL_IS2},
  {0, OL_IS2, 0, OL_IS2},
  {0, -OL_IS2, OL_IS2, 0},
  {0, 0, -OL_IS2, OL_IS2},
  {0, OL_IS2, 0, -OL_IS2},
  {.5, .5, .5, .5},
  {-.5, .5, .5, .5},
  {.5, -.5, .5, .5},
  {-.5, -.5, .5, .5},
  {.5, .5, -.5, .5},
  {-.5, .5, -.5, .5},
  {.5, .5, .5, -.5},
  {-.5, .5, .5, -.5}
};

int permcubic[25][6] = {
  {0, 0, 0, 0, 0, 0},
  {0, 1, 2, 3, 4, 5},
  {0, 2, 1, 3, 5, 4},
  {0, 1, 2, 3, 4, 5},
  {0, 2, 1, 3, 5, 4},
  {2, 1, 0, 5, 4, 3},
  {0, 1, 2, 3, 4, 5},
  {2, 1, 0, 5, 4, 3},
  {1, 0, 2, 4, 3, 5},
  {0, 1, 2, 3, 4, 5},
  {1, 0, 2, 4, 3, 5},
  {1, 0, 2, 4, 3, 5},
  {0, 2, 1, 3, 5, 4},
  {2, 1, 0, 5, 4, 3},
  {1, 0, 2, 4, 3, 5},
  {0, 2, 1, 3, 5, 4},
  {2, 1, 0, 5, 4, 3},
  {1, 2, 0, 4, 5, 3},
  {2, 0, 1, 5, 3, 4},
  {2, 0, 1, 5, 3, 4},
  {1, 2, 0, 4, 5, 3},
  {2, 0, 1, 5, 3, 4},
  {1, 2, 0, 4, 5, 3},
  {2, 0, 1, 5, 3, 4},
  {1, 2, 0, 4, 5, 3}
};

int signcubic[25][6] = {
  {0, 0, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1},
  {1, 1, -1, 1, 1, -1},
  {1, -1, -1, 1, -1, -1},
  {1, -1, 1, 1, -1, 1},
  {-1, 1, 1, -1, 1, 1},
  {-1, 1, -1, -1, 1, -1},
  {1, 1, -1, 1, 1, -1},
  {1, -1, 1, 1, -1, 1},
  {-1, -1, 1, -1, -1, 1},
  {-1, 1, 1, -1, 1, 1},
  {1, 1, -1, 1, 1, -1},
  {-1, 1, 1, -1, 1, 1},
  {1, -1, 1, 1, -1, 1},
  {-1, -1, -1, -1, -1, -1},
  {-1, -1, -1, -1, -1, -1},
  {-1, -1, -1, -1, -1, -1},
  {1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 1, 1},
  {-1, -1, 1, -1, -1, 1},
  {-1, 1, -1, -1, 1, -1},
  {1, -1, -1, 1, -1, -1},
  {-1, -1, 1, -1, -1, 1},
  {-1, 1, -1, -1, 1, -1},
  {1, -1, -1, 1, -1, -1}
};

// end of symmetry operators

int
ol_crysym_qty (char *lauegroup)
{
  if (!lauegroup || !strcmp (lauegroup, "none"))
    return 1;
  else if (strcmp (lauegroup, "-1") == 0)
    return 1;
  else if (strcmp (lauegroup, "2/m") == 0)
    return 2;
  else if (strcmp (lauegroup, "mmm") == 0)
    return 4;
  else if (strcmp (lauegroup, "4/m") == 0)
    return 4;
  else if (strcmp (lauegroup, "4/mmm") == 0)
    return 8;
  else if (strcmp (lauegroup, "-3") == 0)
    return 3;
  else if (strcmp (lauegroup, "-3m") == 0)
    return 6;
  else if (strcmp (lauegroup, "6/m") == 0)
    return 6;
  else if (strcmp (lauegroup, "6/mmm") == 0)
    return 12;
  else if (strcmp (lauegroup, "m-3") == 0)
    return 12;
  else if (strcmp (lauegroup, "m-3m") == 0)
    return 24;
  else if (strcmp (lauegroup, "triclinic") == 0)
    return 1;
  else if (strcmp (lauegroup, "cubic") == 0)
    return 24;
  else if (strcmp (lauegroup, "hexagonal") == 0)
    return 12;
  else
    return -1;
}

void
ol_g_crysym (double **g, char *crysym, int i, double **g2)
{
  /* note: illegal character for variable names, e.g. '-' or '/', are
   * replaced by '_'. */
  int k, j, qty;
  double **T = ol_g_alloc ();

  if ((qty = ol_crysym_qty (crysym)) == -1)
    ut_print_message (2, 0, "Unknown crystal symmetry `%s'.\n", crysym);

  i--;                         /* 0-indexed local vs 1-indexed input; this modif on i is *local* */
  if (!crysym || strcmp (crysym, "none") == 0 || strcmp (crysym, "-1") == 0
      || strcmp (crysym, "triclinic") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T_1[i][k][j];
  else if (strcmp (crysym, "2/m") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T2_m[i][k][j];
  else if (strcmp (crysym, "mmm") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = Tmmm[i][k][j];
  else if (strcmp (crysym, "4/m") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T4_m[i][k][j];
  else if (strcmp (crysym, "4/mmm") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T4_mmm[i][k][j];
  else if (strcmp (crysym, "-3") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T_3[i][k][j];
  else if (strcmp (crysym, "-3m") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T_3m[i][k][j];
  else if (strcmp (crysym, "6/m") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T6_m[i][k][j];
  else if (strcmp (crysym, "6/mmm") == 0 || strcmp (crysym, "hexagonal") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = T6_mmm[i][k][j];
  else if (strcmp (crysym, "m-3") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = Tm_3[i][k][j];
  else if (strcmp (crysym, "m-3m") == 0 || strcmp (crysym, "cubic") == 0)
    for (k = 0; k < 3; k++)
      for (j = 0; j < 3; j++)
        T[k][j] = Tcubic[i][k][j];
  else
    ut_print_message (2, 0, "Unknown crystal symmetry `%s'.\n", crysym);

  if (g)
    ol_g_g_g (g, T, g2);
  else
    ol_g_memcpy (T, g2);

  ol_g_free (&T);

  return;
}

void
ol_q_crysym (double *q, char *crysym, int i, double *q2)
{
  double *U = NULL;

  i--;                         /* 0-indexed local vs 1-indexed input; this modif on i is *local* */
  if (!crysym || strcmp (crysym, "none") == 0 || strcmp (crysym, "triclinic") == 0
      || strcmp (crysym, "-1") == 0)
    U = U_1[i];
  else if (strcmp (crysym, "cubic") == 0 || strcmp (crysym, "m-3m") == 0)
    U = Um3m[i];
  else if (strcmp (crysym, "hexagonal") == 0 || strcmp (crysym, "6/mmm") == 0)
    U = U6_mmm[i];
  else if (strcmp (crysym, "2/m") == 0)
    U = U2_m[i];
  else if (strcmp (crysym, "mmm") == 0)
    U = Ummm[i];
  else if (strcmp (crysym, "4/m") == 0)
    U = U4_m[i];
  else if (strcmp (crysym, "4/mmm") == 0)
    U = U4_mmm[i];
  else if (strcmp (crysym, "-3") == 0)
    U = U_3[i];
  else if (strcmp (crysym, "-3m") == 0)
    U = U_3m[i];
  else if (strcmp (crysym, "6/m") == 0)
    U = U6_m[i];
  else if (strcmp (crysym, "m-3") == 0)
    U = Um_3[i];
  else
    ut_print_message (2, 0, "Unknown crystal symmetry `%s'.\n", crysym);

  ol_q_q_q (q, U, q2);
  ol_q_q (q2, q2);

  return;
}

void
ol_q_crysym_nopos (double *q, char *crysym, int i, double *q2)
{
  double *U = NULL;

  i--;                         /* 0-indexed local vs 1-indexed input; this modif on i is *local* */
  if (strcmp (crysym, "cubic") == 0 || strcmp (crysym, "m-3m") == 0)
    U = Um3m[i];
  else if (strcmp (crysym, "hexagonal") == 0 || strcmp (crysym, "6/mmm") == 0)
    U = U6_mmm[i];
  else if (strcmp (crysym, "triclinic") == 0 || strcmp (crysym, "-1") == 0)
    U = U_1[i];
  else if (strcmp (crysym, "2/m") == 0)
    U = U2_m[i];
  else if (strcmp (crysym, "mmm") == 0)
    U = Ummm[i];
  else if (strcmp (crysym, "4/m") == 0)
    U = U4_m[i];
  else if (strcmp (crysym, "4/mmm") == 0)
    U = U4_mmm[i];
  else if (strcmp (crysym, "-3") == 0)
    U = U_3[i];
  else if (strcmp (crysym, "-3m") == 0)
    U = U_3m[i];
  else if (strcmp (crysym, "6/m") == 0)
    U = U6_m[i];
  else if (strcmp (crysym, "m-3") == 0)
    U = Um_3[i];
  else
    ut_print_message (2, 0, "Unknown crystal symmetry `%s'.\n", crysym);

  ol_q_q_q (q, U, q2);

  return;
}

void
ol_m_crysym (int *m1, char *crysym, int i, int *m2)
{
  int k;

  if (!strcmp (crysym, "cubic") || !strcmp (crysym, "m-3m"))
  {
    if (i < 1 || i > 24)
      ut_print_message (2, 0, "Invalid symmetry operator index (%d).\n", i);

    for (k = 0; k < 6; k++)
      m2[k] = signcubic[i][k] * m1[permcubic[i][k]];
  }
  else
    ut_print_message (2, 0, "Miller index symmetry is only available for cubic symmetry.\n");

  return;
}

double
ol_g_gcrysym (double **g, char *crysym, double **g2)
{
  int i;
  int minid = 1;
  double theta, mintheta;
  double **tmp;
  int qty = ol_crysym_qty (crysym);
  double crysym_mintheta;

  tmp = ol_g_alloc ();

  if (strcmp (crysym, "cubic") == 0)
    crysym_mintheta = .25 * OL_PI;
  else
    crysym_mintheta = 0;

  mintheta = 1e3;
  for (i = 1; i <= qty; i++)
  {
    ol_g_crysym (g, crysym, i, tmp);
    ol_g_theta_rad (tmp, &theta);
    if (theta < mintheta)
    {
      mintheta = theta;
      minid = i;
      if (mintheta < crysym_mintheta)
        break;
    }
  }

  ol_g_crysym (g, crysym, minid, g2);

  ol_g_free (&tmp);

  return mintheta * 180 / OL_PI;
}

double
ol_R_Rcrysym (double *R, char *crysym, double *R2)
{
  double mintheta;
  double *q = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_R_q (R, q);
  mintheta = ol_q_qcrysym (q, crysym, q2);
  ol_q_R (q2, R2);

  ol_q_free (&q);
  ol_q_free (&q2);

  return mintheta;
}

double
ol_q_qcrysym (double *q, char *crysym, double *q2)
{
  int i;
  int minid = 1;
  double theta, mintheta;
  double *tmp;
  int qty = ol_crysym_qty (crysym);
  double crysym_mintheta;

  if (strcmp (crysym, "cubic") == 0)
    crysym_mintheta = .25 * OL_PI;
  else
    crysym_mintheta = 0;

  tmp = ol_q_alloc ();

  mintheta = 2 * OL_PI;
  for (i = 1; i <= qty; i++)
  {
    ol_q_crysym (q, crysym, i, tmp);
    ol_q_theta_rad (tmp, &theta);
    if (theta < mintheta)
    {
      mintheta = theta;
      minid = i;
      if (mintheta < crysym_mintheta)
        break;
    }
  }

  ol_q_crysym (q, crysym, minid, q2);

  ol_q_free (&tmp);

  return mintheta * 180 / OL_PI;
}

/* keeps, among the candidates cand[0..*pqty-1], those of minimal key */
static void
ol_m_mcrysym_keepmin (int *key, int *cand, int *pqty)
{
  int i, min = INT_MAX, qty = 0;

  for (i = 0; i < *pqty; i++)
    if (key[cand[i]] < min)
      min = key[cand[i]];

  for (i = 0; i < *pqty; i++)
    if (key[cand[i]] == min)
      cand[qty++] = cand[i];

  (*pqty) = qty;

  return;
}

double
ol_m_mcrysym (int *m, char *crysym, int *m2)
{
  int i, j, id;
  int allqty = ol_crysym_qty (crysym);
  int qty = allqty;
  int **allm = ut_alloc_2d_int (allqty, 6);
  int *key = ut_alloc_1d_int (allqty);
  int *cand = ut_alloc_1d_int (allqty);

  /* recording all sets of miller indices */
  for (i = 0; i < allqty; i++)
  {
    ol_m_crysym (m, crysym, i + 1, allm[i]);
    cand[i] = i;
  }

  /* (i) (hkl) with positive values, (ii) [uvw] with as less negative values as possible */
  for (i = 0; i < allqty; i++)
  {
    key[i] = 0;
    for (j = 0; j < 3; j++)
      if (allm[i][j] < 0)
        key[i] += 10;           /* "redhibitory" */
    for (j = 3; j < 6; j++)
      if (allm[i][j] < 0)
        key[i]++;
  }
  ol_m_mcrysym_keepmin (key, cand, &qty);

  /* (iii) (hkl) with ascending values */
  for (id = 0; id < 3; id++)
  {
    for (i = 0; i < allqty; i++)
      key[i] = allm[i][id];
    ol_m_mcrysym_keepmin (key, cand, &qty);
  }

  /* (iv) [uvw] with descending absolute values */
  for (id = 3; id < 6; id++)
  {
    for (i = 0; i < allqty; i++)
      key[i] = -abs (allm[i][id]);
    ol_m_mcrysym_keepmin (key, cand, &qty);
  }

  /* (v) [uvw] with the negative values as at right as possible */
  for (i = 0; i < allqty; i++)
  {
    key[i] = -6;
    for (j = 0; j < 6; j++)
      if (allm[i][j] < 0)
      {
        key[i] = -j;
        break;
      }
  }
  ol_m_mcrysym_keepmin (key, cand, &qty);

  ol_m_memcpy (allm[cand[0]], m2);

  double theta, **g = ol_g_alloc ();
  ol_m_g (m2, g);
  ol_g_theta (g, &theta);
  ol_g_free (&g);

  ut_free_2d_int (&allm, allqty);
  ut_free_1d_int (&key);
  ut_free_1d_int (&cand);

  return theta;
}

double
ol_e_ecrysym_rad (double *e, char *crysym, double *e2)
{
  int i, id;
  double *q = ol_q_alloc ();
  double *tmp = ol_q_alloc ();
  double min;
  int qty = ol_crysym_qty (crysym);

  ol_e_e_rad (e, e2);
  ol_e_q_rad (e2, q);

  /* preferred: in the [0, OL_PI] x [0, OL_PI / 2] x [0, OL_PI / 2] domain (always possible for cubic
     symmetry), with the smallest phi1; otherwise, the smallest phi1 */
  id = -1;
  min = DBL_MAX;
  for (i = 1; i <= qty; i++)
  {
    ol_q_crysym (q, crysym, i, tmp);
    ol_q_e_rad (tmp, e2);

    if (e2[1] >= 0 && e2[1] <= OL_PI / 2
        && e2[2] >= 0 && e2[2] <= OL_PI / 2 && e2[0] >= 0 && e2[0] < min)
    {
      min = e2[0];
      id = i;
    }
  }

  if (id == -1)
  {
    min = DBL_MAX;
    for (i = 1; i <= qty; i++)
    {
      ol_q_crysym (q, crysym, i, tmp);
      ol_q_e_rad (tmp, e2);

      if (e2[0] < min)
      {
        min = e2[0];
        id = i;
      }
    }
  }

  ol_q_crysym (q, crysym, id, tmp);
  ol_q_e_rad (tmp, e2);

  double theta;
  ol_q_theta_rad (tmp, &theta);

  ol_q_free (&q);
  ol_q_free (&tmp);

  return theta;
}

double
ol_e_ecrysym (double *e, char *crysym, double *e2)
{
  int i, id;
  double *q = ol_q_alloc ();
  double *tmp = ol_q_alloc ();
  double min;
  int qty = ol_crysym_qty (crysym);

  ol_e_e (e, e2);
  ol_e_q (e2, q);

  /* preferred: in the [0, 180] x [0, 90] x [0, 90] domain (always possible for cubic
     symmetry), with the smallest phi1; otherwise, the smallest phi1 */
  id = -1;
  min = DBL_MAX;
  for (i = 1; i <= qty; i++)
  {
    ol_q_crysym (q, crysym, i, tmp);
    ol_q_e (tmp, e2);

    if (e2[1] >= 0 && e2[1] <= 90
        && e2[2] >= 0 && e2[2] <= 90 && e2[0] >= 0 && e2[0] < min)
    {
      min = e2[0];
      id = i;
    }
  }

  if (id == -1)
  {
    min = DBL_MAX;
    for (i = 1; i <= qty; i++)
    {
      ol_q_crysym (q, crysym, i, tmp);
      ol_q_e (tmp, e2);

      if (e2[0] < min)
      {
        min = e2[0];
        id = i;
      }
    }
  }

  ol_q_crysym (q, crysym, id, tmp);
  ol_q_e (tmp, e2);

  double theta;
  ol_q_theta (tmp, &theta);

  ol_q_free (&q);
  ol_q_free (&tmp);

  return theta;
}

void
ol_g_refsym (double **g, int i, double **g2)
{
  int k;

  i--;

  ol_g_memcpy (g, g2);

  for (k = 0; k < 3; k++)
    g2[k][i] *= -1;

  for (k = 0; k < 3; k++)
    g2[i][k] *= -1;

  return;
}

void
ol_q_refsym (double *q, int i, double *q2)
{
  int k;

  ol_q_memcpy (q, q2);

  for (k = 1; k < 4; k++)
    if (k != i)
      q2[k] *= -1;

  return;
}

void
ol_vect_refsym (double *vect, int i, double *vect2)
{
  ol_vect_memcpy (vect, vect2);

  vect2[i - 1] *= -1;

  return;
}

void
ol_m_refsym (int *m, int i, int *m2)
{
  ol_m_memcpy (m, m2);

  if (i == 1)
  {
    m2[0] *= -1;
    m2[4] *= -1;
    m2[5] *= -1;
  }
  else if (i == 2)
  {
    m2[1] *= -1;
    m2[4] *= -1;
  }
  else
  {
    m2[0] *= -1;
    m2[1] *= -1;
    m2[5] *= -1;
  }

  return;
}

void
ol_g_refsym_orthotropic (double **g, double ***g2, int *pqty)
{
  int i, j;
  double ***gtmp = ut_alloc_3d (3, 3, 3);
  double theta, thetamin;

  ol_g_refsym (g, 1, gtmp[0]);
  ol_g_refsym (g, 2, gtmp[1]);
  ol_g_refsym (gtmp[0], 2, gtmp[2]);

  (*pqty) = 1;
  ol_g_memcpy (g, g2[0]);

  for (i = 0; i < 3; i++)
  {
    thetamin = 1e32;
    for (j = 0; j < (*pqty); j++)
    {
      ol_g_g_misori (gtmp[i], g2[j], &theta);
      thetamin = ut_num_min (thetamin, theta);
    }

    if (thetamin > OL_EPS_DEG)
    {
      ol_g_memcpy (gtmp[i], g2[*pqty]);
      (*pqty)++;
    }
  }

  ut_free_3d (&gtmp, 3, 3);

  return;
}

void
ol_g_refsym_orthotropic_all (double **g, double ***g2)
{
  ol_g_memcpy (g, g2[0]);
  ol_g_refsym (g, 1, g2[1]);
  ol_g_refsym (g, 2, g2[2]);
  ol_g_refsym (g2[1], 2, g2[3]);

  return;
}

void
ol_q_refsym_orthotropic (double *q, double **q2, int *pqty)
{
  int i, j;
  double **qtmp = ut_alloc_2d (3, 4);
  double theta, thetamin;

  ol_q_refsym (q, 1, qtmp[0]);
  ol_q_refsym (q, 2, qtmp[1]);
  ol_q_refsym (qtmp[0], 2, qtmp[2]);

  (*pqty) = 1;
  ol_q_memcpy (q, q2[0]);

  for (i = 0; i < 3; i++)
  {
    thetamin = 1e32;
    for (j = 0; j < (*pqty); j++)
    {
      ol_q_q_misori (qtmp[i], q2[j], &theta);
      thetamin = ut_num_min (thetamin, theta);
    }

    if (thetamin > OL_EPS_DEG)
    {
      ol_q_memcpy (qtmp[i], q2[*pqty]);
      (*pqty)++;
    }
  }

  ut_free_2d (&qtmp, 3);

  return;
}

void
ol_q_refsym_orthotropic_all (double *q, double **q2)
{
  ol_q_memcpy (q, q2[0]);
  ol_q_refsym (q, 1, q2[1]);
  ol_q_refsym (q, 2, q2[2]);
  ol_q_refsym (q2[1], 2, q2[3]);

  return;
}

void
ol_m_refsym_orthotropic (int *m, int **m2, int *pqty)
{
  int i, j;
  double ***gtmp = ut_alloc_3d (3, 3, 3);
  double theta, thetamin;
  double **g = ol_g_alloc ();
  int *mtmp = ol_m_alloc ();
  double ***g2 = ut_alloc_3d (4, 3, 3);

  ol_m_g (m, g);
  ol_g_refsym (g, 1, gtmp[0]);
  ol_g_refsym (g, 2, gtmp[1]);
  ol_g_refsym (gtmp[0], 2, gtmp[2]);

  (*pqty) = 1;
  ol_g_memcpy (g, g2[0]);
  ol_m_memcpy (m, m2[0]);

  for (i = 0; i < 3; i++)
  {
    thetamin = 1e32;
    for (j = 0; j < (*pqty); j++)
    {
      ol_g_g_misori (gtmp[i], g2[j], &theta);
      thetamin = ut_num_min (thetamin, theta);
    }

    if (thetamin > OL_EPS_DEG)
    {
      ol_g_memcpy (gtmp[i], g2[*pqty]);
      if (i == 0)
        ol_m_refsym (m, 1, m2[*pqty]);
      else if (i == 1)
        ol_m_refsym (m, 2, m2[*pqty]);
      else
      {
        ol_m_refsym (m, 1, m2[*pqty]);
        ol_m_refsym (m2[*pqty], 2, m2[*pqty]);
      }
      (*pqty)++;
    }
  }

  ut_free_3d (&gtmp, 3, 3);
  ut_free_3d (&g2, 4, 3);
  ol_g_free (&g);
  ol_m_free (&mtmp);

  return;
}

void
ol_g_refsym_monoclinic (double **g, double ***g2, int *pqty)
{
  double **gtmp = ol_g_alloc ();
  double theta;

  ol_g_refsym (g, 2, gtmp);

  (*pqty) = 1;
  ol_g_memcpy (g, g2[0]);

  ol_g_g_misori (gtmp, g, &theta);

  if (theta > OL_EPS_DEG)
  {
    ol_g_memcpy (gtmp, g2[1]);
    (*pqty) = 2;
  }

  ol_g_free (&gtmp);

  return;
}

void
ol_q_refsym_monoclinic (double *q, double **q2, int *pqty)
{
  double *qtmp = ol_q_alloc ();
  double theta;

  ol_q_refsym (q, 2, qtmp);

  (*pqty) = 1;
  ol_q_memcpy (q, q2[0]);

  ol_q_q_misori (qtmp, q, &theta);

  if (theta > OL_EPS_DEG)
  {
    ol_q_memcpy (qtmp, q2[1]);
    (*pqty) = 2;
  }

  ol_q_free (&qtmp);

  return;
}

void
ol_m_refsym_monoclinic (int *m, int **m2, int *pqty)
{
  double **g = ol_g_alloc ();
  double ***g2 = ut_alloc_3d (2, 3, 3);

  ol_m_g (m, g);
  ol_g_refsym_monoclinic (g, g2, pqty);

  ol_m_memcpy (m, m2[0]);

  if ((*pqty) == 2)
    ol_m_refsym (m, 2, m2[1]);

  ol_g_free (&g);

  return;
}

void
ol_g_refsym_g (double **g, int nb, double **gs)
{
  double *rtheta = ol_rtheta_alloc ();
  double **gcpy = ol_g_alloc ();

  ol_g_memcpy (g, gcpy);

  ol_g_refsym (gcpy, nb, gs);

  ol_g_g_gdisori (gcpy, gs, "cubic", gs);

  ol_g_rtheta (gs, rtheta);
  rtheta[3] /= 2;
  ol_rtheta_g (rtheta, gs);

  ol_rtheta_free (&rtheta);
  ol_g_free (&gcpy);

  return;
}

void
ol_q_refsym_q (double *q, int nb, double *qs)
{
  /* 29 june 2008:
   * Warning: qtmp is necessary, although it should not be...
   * there must be something somewhere... */

  double *qtmp = ol_q_alloc ();
  double *rtheta = ol_rtheta_alloc ();

  ol_q_refsym (q, nb, qtmp);
  ol_q_q_qdisori (q, qtmp, "cubic", qs);
  ol_q_rtheta (qs, rtheta);
  rtheta[3] *= 0.5;
  ol_rtheta_q (rtheta, qs);

  ol_q_free (&qtmp);
  ol_rtheta_free (&rtheta);

  return;
}

void
ol_e_ecrysym_orthotropic_rad (double *e, char *crysym, double **e2, int *pqty)
{
  int i, j, qty2;
  double *q = ol_q_alloc ();
  double **qsym = ut_alloc_2d (8, 4);
  double *q2 = ol_q_alloc ();
  double *etmp = ol_e_alloc ();
  int qty = ol_crysym_qty (crysym);

  ol_e_q_rad (e, q);
  ol_q_refsym_orthotropic (q, qsym, &qty2);

  (*pqty) = 0;
  for (j = 0; j < qty2; j++)
    for (i = 1; i <= qty; i++)
    {
      ol_q_crysym (qsym[j], crysym, i, q2);
      ol_q_e_rad (q2, etmp);
      ol_e_e_rad (etmp, etmp);
      if (etmp[0] <= .5 * OL_PI && etmp[1] <= .5 * OL_PI
          && etmp[2] <= .5 * OL_PI)
      {
        ol_e_memcpy (etmp, e2[*pqty]);
        (*pqty)++;
      }
    }

  if (*pqty != 3)
  {
    ol_e_memcpy (e, etmp);
    for (i = 0; i < 3; i++)
      etmp[i] -= 1e-6;
    ol_e_ecrysym_orthotropic_rad (etmp, crysym, e2, pqty);
  }
  if (*pqty != 3)
  {
    printf ("qty2 = %d != 3 !\n", *pqty);
    abort ();
  }

  ol_q_free (&q);
  ol_q_free (&q2);
  ol_e_free (&etmp);

  return;
}

void
ol_e_ecrysym_orthotropic (double *e, char *crysym, double **e2, int *pqty)
{
  int i;
  double *ecpy = ol_e_alloc ();

  ol_e_deg2rad (e, ecpy);
  ol_e_ecrysym_orthotropic_rad (ecpy, crysym, e2, pqty);
  for (i = 0; i < *pqty; i++)
    ol_e_rad2deg (e2[i], e2[i]);

  ol_e_free (&ecpy);

  return;
}

void
ol_pole_crysym (int *pole1, char *crysym, int nb, int *pole2)
{
  int i;

  if (!strcmp (crysym, "cubic") || !strcmp (crysym, "m-3m"))
  {
    if (nb < 1 || nb > 24)
      ut_print_message (2, 0, "Invalid symmetry operator %d (1 to 24).\n", nb);

    for (i = 0; i < 3; i++)
      pole2[i] = signcubic[nb][i] * pole1[permcubic[nb][i]];
  }

  else
    ut_print_message (2, 0, "Crystal symmetry `%s' not supported.\n", crysym);

  return;
}

void
ol_pole_polecrysym (int *pole, char *crysym, int *pole2)
{
  int poleqty;
  int **poles = NULL;

  ol_polef_polecrysym (pole, crysym, &poles, &poleqty);
  ol_pole_memcpy (poles[0], pole2);

  ut_free_2d_int (&poles, poleqty);

  return;
}

void
ol_polef_polecrysym (int *polef, char *crysym, int ***ppoles, int *ppoleqty)
{
  int *tmp = ol_pole_alloc ();
  int i, j;
  int record, qty1, qty2;
  int *sort;
  int *perm;
  int qty = ol_crysym_qty (crysym);
  int **poles = ut_alloc_2d_int (100, 3);

  (*ppoleqty) = 0;
  for (i = 1; i <= qty; i++)
  {
    ol_pole_crysym (polef, crysym, i, tmp);

    record = 1;
    for (j = 0; j < *ppoleqty; j++) /* is it already in pole? */
      if ((tmp[0] == poles[j][0]
           && tmp[1] == poles[j][1]
           && tmp[2] == poles[j][2])
          || (tmp[0] == -poles[j][0]
              && tmp[1] == -poles[j][1] && tmp[2] == -poles[j][2]))
      {
        record = 0;
        break;
      }

    if (record)
      ol_pole_memcpy (tmp, poles[(*ppoleqty)++]);
  }

  /* ordering: positive first */
  for (i = 0; i < *ppoleqty; i++)
  {
    qty1 = 0;
    qty2 = 0;
    for (j = 0; j < 3; j++)
      tmp[j] = -poles[i][j];

    for (j = 0; j < 3; j++)
    {
      if (poles[i][j] < 0)
        qty1++;
      if (tmp[j] < 0)
        qty2++;
    }

    if (qty2 < qty1)
      ol_pole_memcpy (tmp, poles[i]);
  }

  sort = ut_alloc_1d_int (*ppoleqty);
  for (i = 0; i < *ppoleqty; i++)
  {
    sort[i] =
      100 * abs (poles[i][0]) + 10 * abs (poles[i][1]) + abs (poles[i][2]);
    for (j = 0; j < 3; j++)
      if (poles[i][j] < 0)
        sort[i] -= 1000;
  }

  perm = ut_alloc_1d_int (*ppoleqty);
  ut_array_1d_int_sort_des_index (sort, *ppoleqty, perm);
  ut_array_2d_int_permute (poles, *ppoleqty, 3, perm);

  for (i = 0; i < *ppoleqty; i++)
    if (ut_array_1d_int_equal (polef, 3, poles[i], 3))
    {
      ut_array_2d_int_switchlines (poles, 3, 0, i);
      break;
    }

  if (ppoles)
  {
    *ppoles = ut_alloc_2d_int (*ppoleqty, 3);
    ut_array_2d_int_memcpy (poles, *ppoleqty, 3, *ppoles);
  }

  ol_pole_free (&tmp);
  ut_free_1d_int (&sort);
  ut_free_1d_int (&perm);
  ut_free_2d_int (&poles, 100);

  return;
}

void
ol_vect_crysym (double *v, char *crysym, int nb, double *v2)
{
  double **g = ol_g_alloc ();

  ol_g_set_id (g);

  ol_g_crysym (g, crysym, nb, g);

  ol_g_vect_vect (g, v, v2);

  ol_g_free (&g);

  return;
}

void
ol_vect_crysym_all (double *v, char *crysym, double ***pv2, int *pqty)
{
  int i, j, found, qty;
  double **gs = ol_g_alloc ();
  double *tmp = ut_alloc_1d (3);
  double *tmp2 = ut_alloc_1d (3);
  double **v2 = NULL;

  qty = ol_crysym_qty (crysym);

  // (at most qty vectors)
  v2 = ut_alloc_2d (qty, 3);

  (*pqty) = 0;
  for (i = 1; i <= qty; i++)
  {
    ol_g_crysym (NULL, crysym, i, gs);
    ol_g_vect_vect (gs, v, tmp);
    ut_array_1d_memcpy (tmp, 3, tmp2);
    ut_array_1d_scale (tmp2, 3, -1);

    found = 0;
    for (j = 0; j < (*pqty); j++)
      if (ut_array_1d_equal (tmp, v2[j], 3, 1e-6) || ut_array_1d_equal (tmp2, v2[j], 3, 1e-6))
      {
        found = 1;
        break;
      }

    if (!found)
      ut_array_1d_memcpy (tmp, 3, v2[(*pqty)++]);
  }

  (*pv2) = ut_alloc_2d (*pqty, 3);
  ut_array_2d_memcpy (v2, *pqty, 3, *pv2);

  ut_free_2d (&v2, qty);
  ol_g_free (&gs);
  ut_free_1d (&tmp);
  ut_free_1d (&tmp2);

  return;
}

int
ol_q_qrefsym_orthotropic (double *q, double *q2)
{
  int i, qty, test;
  double **qsym = ut_alloc_2d (4, 4);

  ol_q_refsym_orthotropic (q, qsym, &qty);

  test = -1;
  for (i = 0; i < qty; i++)
    if (qsym[i][1] > -1e-12 && qsym[i][2] > -1e-12)
    {
      ol_q_memcpy (qsym[i], q2);
      test = i;
      break;
    }

  if (test == -1)
    abort ();

  ut_free_2d (&qsym, 4);

  return test;
}
