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

double Tm3m[24][3][3] = {
  {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}},
  {{1, 0, 0}, {0, -1, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, 1, 0}, {0, 0, -1}},
  {{-1, 0, 0}, {0, -1, 0}, {0, 0, 1}},
  {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
  {{0, 0, -1}, {1, 0, 0}, {0, -1, 0}},
  {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
  {{0, 0, 1}, {-1, 0, 0}, {0, -1, 0}},
  {{0, 1, 0}, {0, 0, 1}, {1, 0, 0}},
  {{0, 1, 0}, {0, 0, -1}, {-1, 0, 0}},
  {{0, -1, 0}, {0, 0, 1}, {-1, 0, 0}},
  {{0, -1, 0}, {0, 0, -1}, {1, 0, 0}},
  {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
  {{0, 0, 1}, {0, 1, 0}, {-1, 0, 0}},
  {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}},
  {{1, 0, 0}, {0, 0, 1}, {0, -1, 0}},
  {{0, 0, -1}, {0, 1, 0}, {1, 0, 0}},
  {{0, 1, 0}, {-1, 0, 0}, {0, 0, 1}},
  {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
  {{0, -1, 0}, {-1, 0, 0}, {0, 0, -1}},
  {{0, 0, 1}, {0, -1, 0}, {1, 0, 0}},
  {{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
  {{0, 0, -1}, {0, -1, 0}, {-1, 0, 0}},
  {{-1, 0, 0}, {0, 0, -1}, {0, -1, 0}}
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
  return ol_lauegroup_qty (lauegroup);
}

int
ol_lauegroup_qty (char *lauegroup)
{
  if (strcmp (lauegroup, "-1") == 0)
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

int
ol_g_crysym (double **g, char *crysym, int nb, double **g2)
{
  /* note: illegal character for variable names, e.g. '-' or '/', are
   * replaced by '_'. */
  int i, j, qty;
  double **T = ol_g_alloc ();

  if ((qty = ol_crysym_qty (crysym) == -1))
    return -1;

  nb--;                         /* 0-indexed local vs 1-indexed input; this modif on nb is *local* */
  if (strcmp (crysym, "-1") == 0 || strcmp (crysym, "triclinic") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T_1[nb][i][j];
  else if (strcmp (crysym, "2/m") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T2_m[nb][i][j];
  else if (strcmp (crysym, "mmm") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = Tmmm[nb][i][j];
  else if (strcmp (crysym, "4/m") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T4_m[nb][i][j];
  else if (strcmp (crysym, "4/mmm") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T4_mmm[nb][i][j];
  else if (strcmp (crysym, "-3") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T_3[nb][i][j];
  else if (strcmp (crysym, "-3m") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T_3m[nb][i][j];
  else if (strcmp (crysym, "6/m") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T6_m[nb][i][j];
  else if (strcmp (crysym, "6/mmm") == 0 || strcmp (crysym, "hexagonal") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = T6_mmm[nb][i][j];
  else if (strcmp (crysym, "m-3") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = Tm_3[nb][i][j];
  else if (strcmp (crysym, "m-3m") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = Tm3m[nb][i][j];
  else if (strcmp (crysym, "cubic") == 0)
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++)
        T[i][j] = Tcubic[nb][i][j];
  else
    abort ();

  ol_g_g_g (T, g, g2);

  ol_g_free (T);

  return 0;
}

void
ol_q_crysym (double *q, char *crysym, int nb, double *q2)
{
  double *U = NULL;

  nb--;                         /* 0-indexed local vs 1-indexed input; this modif on nb is *local* */
  if (strcmp (crysym, "cubic") == 0 || strcmp (crysym, "m-3m") == 0)
    U = Um3m[nb];
  else if (strcmp (crysym, "hexagonal") == 0 || strcmp (crysym, "6/mmm") == 0)
    U = U6_mmm[nb];
  else if (strcmp (crysym, "triclinic") == 0 || strcmp (crysym, "-1") == 0)
    U = U_1[nb];
  else if (strcmp (crysym, "2/m") == 0)
    U = U2_m[nb];
  else if (strcmp (crysym, "mmm") == 0)
    U = Ummm[nb];
  else if (strcmp (crysym, "4/m") == 0)
    U = U4_m[nb];
  else if (strcmp (crysym, "4/mmm") == 0)
    U = U4_mmm[nb];
  else if (strcmp (crysym, "-3") == 0)
    U = U_3[nb];
  else if (strcmp (crysym, "-3m") == 0)
    U = U_3m[nb];
  else if (strcmp (crysym, "6/m") == 0)
    U = U6_m[nb];
  else if (strcmp (crysym, "m-3") == 0)
    U = Um_3[nb];
  else
    abort ();

  ol_q_q_q (q, U, q2);
  ol_q_q (q2, q2);

  return;
}

void
ol_q_crysym_nopos (double *q, char *crysym, int nb, double *q2)
{
  double *U = NULL;

  nb--;                         /* 0-indexed local vs 1-indexed input; this modif on nb is *local* */
  if (strcmp (crysym, "cubic") == 0 || strcmp (crysym, "m-3m") == 0)
    U = Um3m[nb];
  else if (strcmp (crysym, "hexagonal") == 0 || strcmp (crysym, "6/mmm") == 0)
    U = U6_mmm[nb];
  else if (strcmp (crysym, "triclinic") == 0 || strcmp (crysym, "-1") == 0)
    U = U_1[nb];
  else if (strcmp (crysym, "2/m") == 0)
    U = U2_m[nb];
  else if (strcmp (crysym, "mmm") == 0)
    U = Ummm[nb];
  else if (strcmp (crysym, "4/m") == 0)
    U = U4_m[nb];
  else if (strcmp (crysym, "4/mmm") == 0)
    U = U4_mmm[nb];
  else if (strcmp (crysym, "-3") == 0)
    U = U_3[nb];
  else if (strcmp (crysym, "-3m") == 0)
    U = U_3m[nb];
  else if (strcmp (crysym, "6/m") == 0)
    U = U6_m[nb];
  else if (strcmp (crysym, "m-3") == 0)
    U = Um_3[nb];
  else
    abort ();

  ol_q_q_q (q, U, q2);

  return;
}

void
ol_m_crysym (int *m1, char *crysym, int nb, int *m2)
{
  int i;

  /* TODO */
  if (strcmp (crysym, "cubic") == 0)
  {
    if (nb < 1 || nb > 24)
      abort ();

    for (i = 0; i < 6; i++)
      m2[i] = signcubic[nb][i] * m1[permcubic[nb][i]];
  }
  else
    abort ();

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

  ol_g_free (tmp);

  return mintheta;
}

double
ol_rtheta_rthetacrysym (double *r, double theta, char *crysym, double *r2,
                        double *ptheta2)
{
  double mintheta;
  double *q = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_rtheta_q (r, theta, q);
  mintheta = ol_q_qcrysym (q, crysym, q2);
  ol_q_rtheta (q2, r2, ptheta2);

  ol_q_free (q);
  ol_q_free (q2);

  return mintheta;
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

  ol_q_free (q);
  ol_q_free (q2);

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

  ol_q_free (tmp);

  return mintheta * 180 / OL_PI;
}

void
ol_m_mcrysym (int *m, char *crysym, int *m2)
{
  int i, j, id;
  int qty = ol_crysym_qty (crysym);
  int **allm = ut_alloc_2d_int (qty, 6);
  int *tmp = ut_alloc_1d_int (qty);
  int *nb = ut_alloc_1d_int (qty);

  /* recording all sets of miller indices from cubic symmetry */
  for (i = 0; i < qty; i++)
    ol_m_crysym (m, crysym, i + 1, allm[i]);

  /* sorting according to the amount of negative indices */
  for (i = 0; i < qty; i++)
  {
    tmp[i] = 0;
    for (j = 0; j < 3; j++)
      if (allm[i][j] < 0)
        tmp[i] += 10;           /* "redhibitory" */
    for (j = 3; j < 6; j++)
      if (allm[i][j] < 0)
        tmp[i]++;
  }
  ut_array_1d_int_sort_index (tmp, qty, nb);

  /* searching the quantity of acceptable solutions, and recording */
  for (i = 0; i < qty; i++)
    if (tmp[nb[i]] != tmp[nb[i + 1]])
    {
      qty = i + 1;
      break;
    }
  for (i = 0; i < qty; i++)
    ol_m_memcpy (allm[nb[i]], allm[i]);

  /* choosing the sets of Miller indices with increasing hkl */
  for (id = 0; id < 3; id++)
    if (qty > 1)
    {
      for (i = 0; i < qty; i++)
        tmp[i] = allm[i][id];
      ut_array_1d_int_sort_index (tmp, qty, nb);
      /* searching the quantity of acceptable solutions, and recording */
      for (i = 0; i < qty; i++)
        if (tmp[nb[i]] != tmp[nb[i + 1]])
        {
          qty = i + 1;
          break;
        }
      for (i = 0; i < qty; i++)
        ol_m_memcpy (allm[nb[i]], allm[i]);
    }

  /* choosing those with decreasing uvw */
  for (id = 3; id < 6; id++)
    if (qty > 1)
    {
      for (i = 0; i < qty; i++)
        tmp[i] = allm[i][id];
      ut_array_1d_int_abssort_index (tmp, qty, nb);
      ut_array_1d_int_reverseelts (nb, qty);
      /* searching the quantity of acceptable solutions, and recording */
      for (i = 0; i < qty; i++)
        if (tmp[nb[i]] != tmp[nb[i + 1]])
        {
          qty = i + 1;
          break;
        }
      for (i = 0; i < qty; i++)
        ol_m_memcpy (allm[nb[i]], allm[i]);
    }

  /* choosing the one with the rightest minus sign */
  if (qty != 1)
  {
    for (i = 0; i < qty; i++)
    {
      tmp[i] = 7;
      for (j = 0; j <= 6; j++)
        if (allm[i][j] < 0)
        {
          tmp[i] = j;
          break;
        }
    }
    ut_array_1d_int_sort_index (tmp, qty, nb);
    ut_array_1d_int_reverseelts (nb, qty);

    /* recording solutions */
    ol_m_memcpy (allm[nb[0]], allm[0]);
  }

  ol_m_memcpy (allm[0], m2);

  ut_free_2d_int (&allm, qty);
  ut_free_1d_int (&tmp);
  ut_free_1d_int (&nb);

  return;
}

void
ol_e_ecrysym_rad (double *e, char *crysym, double *e2)
{
  int i, id;
  double *q = ol_q_alloc ();
  double *tmp = ol_q_alloc ();
  double min;
  int qty = ol_crysym_qty (crysym);

  ol_e_e_rad (e, e2);
  ol_e_q_rad (e2, q);

  id = -1;
  min = OL_PI;
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

  ol_q_crysym (q, crysym, id, tmp);
  ol_q_e_rad (tmp, e2);

  ol_q_free (q);
  ol_q_free (tmp);

  return;
}

void
ol_e_ecrysym (double *e, char *crysym, double *e2)
{
  int i, id;
  double *q = ol_q_alloc ();
  double *tmp = ol_q_alloc ();
  double min;
  int qty = ol_crysym_qty (crysym);

  ol_e_e (e, e2);
  ol_e_q (e2, q);

  id = -1;
  min = 180;
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

  ol_q_crysym (q, crysym, id, tmp);
  ol_q_e (tmp, e2);

  ol_q_free (q);
  ol_q_free (tmp);

  return;
}

void
ol_g_refsym (double **g, int nb, double **g2)
{
  int i;

  nb--;

  ol_g_memcpy (g, g2);

  for (i = 0; i < 3; i++)
    g2[i][nb] *= -1;

  for (i = 0; i < 3; i++)
    g2[nb][i] *= -1;

  return;
}

void
ol_q_refsym (double *q, int nb, double *q2)
{
  int i;

  ol_q_memcpy (q, q2);

  for (i = 1; i < 4; i++)
    if (i != nb)
      q2[i] *= -1;

  return;
}

void
ol_R_refsym (double *R, int nb, double *R2)
{
  int i;

  nb--;

  ol_R_memcpy (R, R2);

  for (i = 0; i < 3; i++)
    if (i != nb)
      R2[i] *= -1;

  return;
}

void
ol_vect_refsym (double *vect, int nb, double *vect2)
{
  ol_vect_memcpy (vect, vect2);

  vect2[nb - 1] *= -1;

  return;
}

void
ol_e_refsym_rad (double *e, int nb, double *e2)
{
  double *q = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q_rad (e, q);
  ol_q_refsym (q, nb, q2);
  ol_q_e_rad (q2, e2);

  ol_q_free (q);
  ol_q_free (q2);

  return;
}

void
ol_e_refsym (double *e, int nb, double *e2)
{
  double *q = ol_q_alloc ();
  double *q2 = ol_q_alloc ();

  ol_e_q (e, q);
  ol_q_refsym (q, nb, q2);
  ol_q_e (q2, e2);

  ol_q_free (q);
  ol_q_free (q2);

  return;
}

void
ol_m_refsym (int *m, int nb, int *m2)
{
  ol_m_memcpy (m, m2);

  if (nb == 1)
  {
    m2[0] *= -1;
    m2[4] *= -1;
    m2[5] *= -1;
  }
  else if (nb == 2)
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
ol_R_refsym_orthotropic (double *R, double **R2, int *pqty)
{
  int i, j;
  double **Rtmp = ut_alloc_2d (3, 3);
  double theta, thetamin;

  ol_R_refsym (R, 1, Rtmp[0]);
  ol_R_refsym (R, 2, Rtmp[1]);
  ol_R_refsym (Rtmp[0], 2, Rtmp[2]);

  (*pqty) = 1;
  ol_R_memcpy (R, R2[0]);

  for (i = 0; i < 3; i++)
  {
    thetamin = 1e32;
    for (j = 0; j < (*pqty); j++)
    {
      ol_R_R_misori (Rtmp[i], R2[j], &theta);
      thetamin = ut_num_min (thetamin, theta);
    }

    if (thetamin > OL_EPS_DEG)
    {
      ol_R_memcpy (Rtmp[i], R2[*pqty]);
      (*pqty)++;
    }
  }

  ut_free_2d (&Rtmp, 3);

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
ol_R_refsym_orthotropic_all (double *R, double **R2)
{
  ol_R_memcpy (R, R2[0]);
  ol_R_refsym (R, 1, R2[1]);
  ol_R_refsym (R, 2, R2[2]);
  ol_R_refsym (R2[1], 2, R2[3]);

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
  ut_free_3d (&g2, 3, 3);
  ol_g_free (g);
  ol_m_free (mtmp);

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

  ol_g_free (gtmp);

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

  ol_q_free (qtmp);

  return;
}

void
ol_R_refsym_monoclinic (double *R, double **R2, int *pqty)
{
  double *Rtmp = ol_R_alloc ();
  double theta;

  ol_R_refsym (R, 2, Rtmp);

  (*pqty) = 1;
  ol_R_memcpy (R, R2[0]);

  ol_R_R_misori (Rtmp, R, &theta);

  if (theta > OL_EPS_DEG)
  {
    ol_R_memcpy (Rtmp, R2[1]);
    (*pqty) = 2;
  }

  ol_R_free (Rtmp);

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

  ol_g_free (g);

  return;
}

void
ol_g_refsym_g (double **g, int nb, double **gs)
{
  double *r = ol_r_alloc ();
  double theta;
  double **gcpy = ol_g_alloc ();

  ol_g_memcpy (g, gcpy);

  ol_g_refsym (gcpy, nb, gs);

  ol_g_g_gdisori (gcpy, gs, "cubic", gs);

  ol_g_rtheta (gs, r, &theta);
  theta /= 2;
  ol_rtheta_g (r, theta, gs);

  ol_r_free (r);
  ol_g_free (gcpy);

  return;
}

void
ol_q_refsym_q (double *q, int nb, double *qs)
{
  /* 29 june 2008:
   * Warning: qtmp is necessary, although it should not be...
   * there must be something somewhere... */

  double *qtmp = ol_q_alloc ();
  double *r = ol_r_alloc ();
  double theta;

  ol_q_refsym (q, nb, qtmp);
  ol_q_q_qdisori (q, qtmp, "cubic", qs);
  ol_q_rtheta (qs, r, &theta);
  theta *= 0.5;
  ol_rtheta_q (r, theta, qs);

  ol_r_free (r);

  return;
}

void
ol_m_refsym_g (int *m, int nb, double **gs)
{
  int *m2 = ol_m_alloc ();
  double **g1 = ol_g_alloc ();
  double **g2 = ol_g_alloc ();
  double *r = ol_r_alloc ();
  double theta;

  ol_m_refsym (m, nb, m2);

  ol_m_g (m, g1);
  ol_m_g (m2, g2);

  ol_g_g_gdisori (g1, g2, "cubic", gs);
  ol_g_rtheta (gs, r, &theta);
  theta /= 2;
  ol_rtheta_g (r, theta, gs);

  ol_m_free (m2);
  ol_g_free (g1);
  ol_g_free (g2);

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

  ol_q_free (q);
  ol_q_free (q2);
  ol_e_free (etmp);

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

  ol_e_free (ecpy);

  return;
}

void
ol_pole_crysym (int *pole1, char *crysym, int nb, int *pole2)
{
  int i;

  int permcubic[24][6] = {
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

  if (strcmp (crysym, "cubic") != 0 || nb < 1 || nb > 24)
    abort ();

  nb--;
  for (i = 0; i < 3; i++)
    pole2[i] = signcubic[nb][i] * pole1[permcubic[nb][i]];

  return;
}

void
ol_pole_polecrysym (int *pole, char *crysym, int *pole2)
{
  int tmp;
  int qty = ol_crysym_qty (crysym);
  int **polelist = ut_alloc_2d_int (qty, 3);

  ol_polef_polecrysym (pole, crysym, polelist, &tmp);
  ol_pole_memcpy (polelist[0], pole2);

  ut_free_2d_int (&polelist, qty);

  return;
}

void
ol_polef_polecrysym (int *polef, char *crysym, int **pole, int *pqty)
{
  int *tmp = ol_pole_alloc ();
  int i, j;
  int record, qty1, qty2;
  int *sort;
  int *perm;
  int qty = ol_crysym_qty (crysym);

  (*pqty) = 0;
  for (i = 1; i <= qty; i++)
  {
    ol_pole_crysym (polef, crysym, i, tmp);

    record = 1;
    for (j = 0; j < *pqty; j++) /* is it already in pole? */
      if ((tmp[0] == pole[j][0]
           && tmp[1] == pole[j][1]
           && tmp[2] == pole[j][2])
          || (tmp[0] == -pole[j][0]
              && tmp[1] == -pole[j][1] && tmp[2] == -pole[j][2]))
      {
        record = 0;
        break;
      }

    if (record)
      ol_pole_memcpy (tmp, pole[(*pqty)++]);
  }

  /* ordering: positive first */
  for (i = 0; i < *pqty; i++)
  {
    qty1 = 0;
    qty2 = 0;
    for (j = 0; j < 3; j++)
      tmp[j] = -pole[i][j];

    for (j = 0; j < 3; j++)
    {
      if (pole[i][j] < 0)
        qty1++;
      if (tmp[j] < 0)
        qty2++;
    }

    if (qty2 < qty1)
      ol_pole_memcpy (tmp, pole[i]);
  }

  sort = ut_alloc_1d_int (*pqty);
  for (i = 0; i < *pqty; i++)
  {
    sort[i] =
      100 * fabs (pole[i][0]) + 10 * fabs (pole[i][1]) + fabs (pole[i][2]);
    for (j = 0; j < 3; j++)
      if (pole[i][j] < 0)
        sort[i] -= 1000;
  }

  perm = ut_alloc_1d_int (*pqty);
  ut_array_1d_int_sort_des_index (sort, *pqty, perm);
  ut_array_2d_int_permute (pole, *pqty, 3, perm);

  ol_pole_free (tmp);
  ut_free_1d_int (&sort);
  ut_free_1d_int (&perm);

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

void
ol_e_refsym_orthotropic (double *e, double **e2, int *pqty)
{
  int i;
  double *q = ol_q_alloc ();
  double **q2 = ut_alloc_2d (4, 4);

  ol_e_q (e, q);
  ol_q_refsym_orthotropic (q, q2, pqty);
  for (i = 0; i < *pqty; i++)
    ol_q_e (q2[i], e2[i]);

  ol_q_free (q);
  ut_free_2d (&q2, 4);

  return;
}

void
ol_e_refsym_orthotropic_all (double *e, double **e2)
{
  int i;
  double *q = ol_q_alloc ();
  double **q2 = ut_alloc_2d (4, 4);

  ol_e_q (e, q);
  ol_q_refsym_orthotropic_all (q, q2);
  for (i = 0; i < 4; i++)
    ol_q_e (q2[i], e2[i]);

  ol_q_free (q);
  ut_free_2d (&q2, 4);

  return;
}
