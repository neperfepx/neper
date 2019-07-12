/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
*                                    #####        # #    #                *
*        ####  #####  ###### #    # #     #       # #   #                 *
*       #    # #    # #      ##   # #             # #  #                  *
*       #    # #    # #####  # #  # #  ####       # ###                   *
*       #    # #####  #      #  # # #     # #     # #  #                  *
*       #    # #      #      #   ## #     # #     # #   #                 *
*        ####  #      ###### #    #  #####   #####  #    #                *
*                                                                         *
*                                                                         *
*           Mattia Montanari    |   University of Oxford 2019             *
* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *
*                                                                         *
* This file implements the GJK algorithm and the Signed Volumes method as *
* presented in:                                                           *
*   M. Montanari, N. Petrinic, E. Barbieri, "Improving the GJK Algorithm  *
*   for Faster and More Reliable Distance Queries Between Convex Objects" *
*   ACM Transactions on Graphics, vol. 36, no. 3, Jun. 2017.              *
*                                                                         *
* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

/**
 * @file openGJK.c
 * @author Mattia Montanari
 * @date April 2018
 * @brief File containing entire implementation of the GJK algorithm.
 *
 */

#define _CRT_HAS_CXX17 0
#include <stdio.h>
#include "openGJK/openGJK.h"

/* If instricuted, uses adaptive floating point arithmetics. */
#ifdef ADAPTIVEFP
#include "predicates.h"
#endif

/* If instricuted, compile a mex function for Matlab.  */
#ifdef MATLABDOESMEXSTUFF
#include "mex.h"
#else
#define mexPrintf  printf
#endif

#ifdef unix
#define _isnan(x) isnan(x)
#endif

#define dotProduct(a, b) (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])

/**
 * @brief Returns squared vector norm
 */
double norm2(double *v) {
  double n2 = 0;
  for (int i = 0; i < 3; ++i)
    n2 += v[i] * v[i];
  return n2;
}

/**
 * @brief Finds point of minimum norm of 1-simplex. Robust, but slower, version of algorithm presented in paper.
 */
inline static void S1D(struct simplex * s, double *vv) {
  int    i = 0;
  int    indexI = 1;
  int    FacetsTest[2];
  double det_ap = 0;
  double det_pb = 0;
  double pt = 0;
  double leng = 0;
  double a[3];
  double b[3];
  double t[3];
  double nu_fabs[3];

  for (i = 0; i < 3; ++i) {
    b[i] = s->vrtx[0][i];
    a[i] = s->vrtx[1][i];
    t[i] = b[i] - a[i];
    leng += t[i];
    nu_fabs[i] = fabs(t[i]);
  }

  if (nu_fabs[0] > nu_fabs[1]) {
    if (nu_fabs[0] > nu_fabs[2])
      indexI = 0;
    else
      indexI = 2;
  }
  else if (nu_fabs[0] < nu_fabs[1]) {
    if (nu_fabs[1] > nu_fabs[2])
      indexI = 1;
    else
      indexI = 2;
  }
  else if (nu_fabs[0] < nu_fabs[2]) {
    indexI = 2;
  }
  else if (nu_fabs[1] < nu_fabs[2]) {
    indexI = 2;
  }

  /* Project origin onto the 1D simplex - line */
  pt = dotProduct(b, t) / (dotProduct(t, t)) * (a[indexI] - b[indexI]) + b[indexI];

  /* Compute signed determinants */
  det_ap = a[indexI] - pt;
  det_pb = pt - b[indexI];

  /* Compare signs of AB and auxiliary simplices */
  FacetsTest[0] = SAMESIGN(t[indexI], -1 * det_ap);
  FacetsTest[1] = SAMESIGN(t[indexI], -1 * det_pb);

  if (FacetsTest[0] + FacetsTest[1] == 2) {
    /* The origin is between A and B */
    s->lambdas[0] = det_ap * -1.0 / t[indexI];
    s->lambdas[1] = 1 - s->lambdas[0];
    s->wids[0] = 0;
    s->wids[1] = 1;
    s->nvrtx = 2;
  }
  else if (FacetsTest[0] == 0) {
    /* The origin is beyond A */
    s->lambdas[0] = 1;
    s->wids[0] = 0;
    s->nvrtx = 1;
    for (i = 0; i < 3; ++i) {
      s->vrtx[0][i] = s->vrtx[1][i];
    }
  }
  else {
    /* The origin is behind B */
    s->lambdas[0] = 1;
    s->wids[0] = 1;
    s->nvrtx = 1;
  }

  for (int j = 0; j < 3; ++j) {
    vv[j] = 0;
    for (i = 0; i < s->nvrtx; ++i) {
      vv[j] += (s->lambdas[i] * s->vrtx[i][j]);
    }
  }
}

/**
 * @brief Finds point of minimum norm of 2-simplex. Robust, but slower, version of algorithm presented in paper.
 */
inline static void S2D(struct simplex * s, double *vv) {
  int       i;
  int       k;
  int       l;
  int       j;
  int       indexI = 1;
  //int       stemp[3];
  int       FacetsTest[3];
  int       indexJ[2] = { -1 };
  double    nu_max = 0;
  double    inv_detM = 0;
  double    nnorm_sqrd = 0;
  double    nnnorm = 0;
  double    dotNA;
  double    a[3];
  double    b[3];
  double    c[3];
  double    s21[3];
  double    s31[3];
  double    nu_test[3];
  double    nu_fabs[3];
  double    B[3];
  double    n[3];
  double    v[3];
  double    vtmp[3];
  double    pp[3 - 1];
  double    sa[3 - 1];
  double    sb[3 - 1];
  double    sc[3 - 1];


  for (i = 0; i < 3; ++i) {
    c[i] = s->vrtx[0][i];
    b[i] = s->vrtx[1][i];
    a[i] = s->vrtx[2][i];
    s21[i] = b[i] - a[i];
    s31[i] = c[i] - a[i];
  }

  /* Find best axis for projection */
  k = 1; l = 2;
  for (i = 0; i < 3; ++i) {
    nu_test[i] = pow(-1.0, i) * (b[k] * c[l] + a[k] * b[l] + c[k] * a[l] - b[k] * a[l] - c[k] * b[l] - a[k] * c[l]);
    k = l; l = i;
  }

  for (i = 0; i < 3; ++i) {
    nu_fabs[i] = fabs(nu_test[i]);
  }

  if (nu_fabs[0] > nu_fabs[1]) {
    if (nu_fabs[0] > nu_fabs[2]) {
      indexI = 0;
      indexJ[0] = 1;
      indexJ[1] = 2;
    }
    else {
      indexJ[0] = 0;
      indexJ[1] = 1;
      indexI = 2;
    }
  }
  else if (nu_fabs[0] < nu_fabs[1]) {
    if (nu_fabs[1] > nu_fabs[2]) {
      indexJ[0] = 0;
      indexI = 1;
      indexJ[1] = 2;
    }
    else {
      indexJ[0] = 0;
      indexJ[1] = 1;
      indexI = 2;
    }
  }
  else if (nu_fabs[0] < nu_fabs[2]) {
    indexJ[0] = 0;
    indexJ[1] = 1;
    indexI = 2;
  }

  nu_max = nu_test[indexI];

  k = 1;      l = 2;
  for (i = 0; i < 3; ++i) {
    n[i] = s21[k] * s31[l] - s21[l] * s31[k];
    nnorm_sqrd += n[i] * n[i];
    k = l;     l = i;
  }
  nnnorm = 1 / sqrt(nnorm_sqrd);
  for (i = 0; i < 3; ++i) {
    n[i] = n[i] * nnnorm;
  }

  dotNA = dotProduct(n, a);

  pp[0] = dotNA * n[indexJ[0]];
  pp[1] = dotNA * n[indexJ[1]];

  /* Compute signed determinants */
#ifndef ADAPTIVEFP
  double    ss[3][3 - 1];

  ss[0][0] = sa[0] = a[indexJ[0]];
  ss[0][1] = sa[1] = a[indexJ[1]];
  ss[1][0] = sb[0] = b[indexJ[0]];
  ss[1][1] = sb[1] = b[indexJ[1]];
  ss[2][0] = sc[0] = c[indexJ[0]];
  ss[2][1] = sc[1] = c[indexJ[1]];

  k = 1;   l = 2;
  for (i = 0; i < 3; ++i) {
    B[i] = pp[0] * ss[k][1] + pp[1] * ss[l][0] + ss[k][0] * ss[l][1] - pp[0] * ss[l][1] - pp[1] * ss[k][0] -
           ss[l][0] * ss[k][1];
    k = l;
    l = i;
  }
#else

  sa[0] = a[indexJ[0]];
  sa[1] = a[indexJ[1]];
  sb[0] = b[indexJ[0]];
  sb[1] = b[indexJ[1]];
  sc[0] = c[indexJ[0]];
  sc[1] = c[indexJ[1]];

  B[0] = orient2d(sa, pp, sc);
  B[1] = orient2d(sa, pp, sc);
  B[2] = orient2d(sa, sb, pp);
#endif

  /* Test if sign of ABC is equal to the signes of the auxiliary simplices */
  for (i = 0; i < 3; ++i) {
    FacetsTest[i] = SAMESIGN(nu_max, B[i]);
  }

  // The nan check was not included originally and will be removed in v2.0
  if (FacetsTest[1] + FacetsTest[2] == 0 || isnan(n[0])) {
    struct simplex  sTmp;

    sTmp.nvrtx = 2;
    s->nvrtx = 2;
    for (i = 0; i < 3; ++i) {

      sTmp.vrtx[0][i] = s->vrtx[1][i];
      sTmp.vrtx[1][i] = s->vrtx[2][i];

      s->vrtx[0][i] = s->vrtx[0][i];
      s->vrtx[1][i] = s->vrtx[2][i];
    }

    S1D(&sTmp, v);
    S1D(s, v);

    for (j = 0; j < 3; ++j) {
      vtmp[j] = 0;
      v[j] = 0;
      for (i = 0; i < sTmp.nvrtx; ++i) {
        vtmp[j] += (sTmp.lambdas[i] * sTmp.vrtx[i][j]);
        v[j] += (s->lambdas[i] * s->vrtx[i][j]);
      }
    }

    if (dotProduct(v, v) < dotProduct(vtmp, vtmp)) {
      /* Keep simplex. Need to update sID only*/
      for (i = 1; i < s->nvrtx; ++i) {
        s->wids[i] = s->wids[i] + 1;
      }
    }
    else {
      s->nvrtx = sTmp.nvrtx;
      for (j = 0; j < 3; ++j) {
        for (i = 0; i < s->nvrtx; ++i) {
          s->vrtx[i][j] = s->vrtx[i][j];
          s->lambdas[i] = sTmp.lambdas[i];
          /* No need to convert sID here since sTmp deal with the vertices A and B. ;*/
          s->wids[i] = sTmp.wids[i];
        }
      }
    }
  }
  else if ((FacetsTest[0] + FacetsTest[1] + FacetsTest[2]) == 3) {
    /* The origin projections lays onto the triangle */
    inv_detM = 1 / nu_max;
    s->lambdas[0] = B[2] * inv_detM;
    s->lambdas[1] = B[1] * inv_detM;
    s->lambdas[2] = 1 - s->lambdas[0] - s->lambdas[1];
    s->wids[0] = 0;
    s->wids[1] = 1;
    s->wids[2] = 2;
    s->nvrtx = 3;
  }
  else if (FacetsTest[2] == 0) {
    /* The origin projection P faces the segment AB */
    s->nvrtx = 2;
    for (i = 0; i < 3; ++i) {
      s->vrtx[0][i] = s->vrtx[1][i];
      s->vrtx[1][i] = s->vrtx[2][i];
    }
    S1D(s, v);
  }
  else if (FacetsTest[1] == 0) {
    /* The origin projection P faces the segment AC */
    s->nvrtx = 2;
    for (i = 0; i < 3; ++i) {
      s->vrtx[0][i] = s->vrtx[0][i];
      s->vrtx[1][i] = s->vrtx[2][i];
    }
    S1D(s, v);
    for (i = 1; i < s->nvrtx; ++i) {
      s->wids[i] = s->wids[i] + 1;
    }
  }
  else {
    /* The origin projection P faces the segment BC */
    s->nvrtx = 2;
    S1D(s, v);

  }

  for ( j = 0; j < 3; ++j) {
    vv[j] = 0;
    for (i = 0; i < s->nvrtx; ++i) {
      vv[j] += (s->lambdas[i] * s->vrtx[i][j]);
    }
  }
}

/**
 * @brief Finds point of minimum norm of 3-simplex. Robust, but slower, version of algorithm presented in paper.
 */
inline static void S3D(struct simplex * s,  double *vv) {
  int      FacetsTest[4] = { 1,1,1,1 };
  int      sID[4] = { 0, 0, 0, 0 };
  int      TrianglesToTest[9] = { 3, 3, 3, 1, 2, 2, 0, 0, 1 };
  int      i;
  int      j;
  int      k;
  int      l;
  int      vtxid;
  int      ndoubts = 0;
  int      nclosestS = 0;
  int      firstaux = 0;
  int      secondaux = 0;
  int      Flag_sAuxused = 0;
  double   a[3];
  double   b[3];
  double   c[3];
  double   d[3];
  double   vtmp[3];
  double   v[3];
  double   B[4];
  double   tmplamda[4] = { 0, 0, 0, 0 };
  double   tmpscoo1[4][3] = { 0 };
  double   sqdist_tmp = 0;
  double   detM;
  double   inv_detM;
#ifdef ADAPTIVEFP
  double  o[3] = { 0 };
#endif
  for (i = 0; i < 3; ++i) {
    d[i] = s->vrtx[0][i];
    c[i] = s->vrtx[1][i];
    b[i] = s->vrtx[2][i];
    a[i] = s->vrtx[3][i];
  }

#ifndef ADAPTIVEFP
  B[0] = -1 * (b[0] * c[1] * d[2] + b[1] * c[2] * d[0] + b[2] * c[0] * d[1] - b[2] * c[1] * d[0] - b[1] * c[0] * d[2] - b[0] * c[2] * d[1]);
  B[1] = +1 * (a[0] * c[1] * d[2] + a[1] * c[2] * d[0] + a[2] * c[0] * d[1] - a[2] * c[1] * d[0] - a[1] * c[0] * d[2] - a[0] * c[2] * d[1]);
  B[2] = -1 * (a[0] * b[1] * d[2] + a[1] * b[2] * d[0] + a[2] * b[0] * d[1] - a[2] * b[1] * d[0] - a[1] * b[0] * d[2] - a[0] * b[2] * d[1]);
  B[3] = +1 * (a[0] * b[1] * c[2] + a[1] * b[2] * c[0] + a[2] * b[0] * c[1] - a[2] * b[1] * c[0] - a[1] * b[0] * c[2] - a[0] * b[2] * c[1]);
  detM = B[0] + B[1] + B[2] + B[3];
#else
  B[0] = orient3d(o, b, c, d);
  B[1] = orient3d(a, o, c, d);
  B[2] = orient3d(a, b, o, d);
  B[3] = orient3d(a, b, c, o);
  detM = orient3d(a, b, c, d);
#endif

  /* Test if sign of ABCD is equal to the signes of the auxiliary simplices */
  double eps = 1e-13;
  if (fabs(detM) < eps)
  {
    if (fabs(B[2]) < eps && fabs(B[3]) < eps)
      FacetsTest[1] = 0; /* A = B. Test only ACD */
    else if (fabs(B[1]) < eps && fabs(B[3]) < eps)
      FacetsTest[2] = 0; /* A = C. Test only ABD */
    else if (fabs(B[1]) < eps && fabs(B[2]) < eps)
      FacetsTest[3] = 0; /* A = D. Test only ABC */
    else if (fabs(B[0]) < eps && fabs(B[3]) < eps)
      FacetsTest[1] = 0; /* B = C. Test only ACD */
    else if (fabs(B[0]) < eps && fabs(B[2]) < eps)
      FacetsTest[1] = 0; /* B = D. Test only ABD */
    else if (fabs(B[0]) < eps && fabs(B[1]) < eps)
      FacetsTest[2] = 0; /* C = D. Test only ABC */
    else {
      for (i = 0; i < 4; i++)
        FacetsTest[i] = 0; /* Any other case. Test ABC, ABD, ACD */
    }
  }
  else
  {
    for (i = 0; i < 4; ++i)
      FacetsTest[i] = SAMESIGN(detM, B[i]);
  }

  /* Compare signed volumes and compute barycentric coordinates */
  if (FacetsTest[0] + FacetsTest[1] + FacetsTest[2] + FacetsTest[3] == 4) {
    /* All signs are equal, therefore the origin is inside the simplex */
    inv_detM = 1 / detM;
    s->lambdas[3] = B[0] * inv_detM;
    s->lambdas[2] = B[1] * inv_detM;
    s->lambdas[1] = B[2] * inv_detM;
    s->lambdas[0] = 1 - s->lambdas[1] - s->lambdas[2] - s->lambdas[3];
    s->wids[0] = 0;
    s->wids[1] = 1;
    s->wids[2] = 2;
    s->wids[3] = 3;
    s->nvrtx = 4;
  }
  else if (FacetsTest[1] + FacetsTest[2] + FacetsTest[3] == 0) {
    /* There are three facets facing the origin  */
    ndoubts = 3;

    struct simplex  sTmp;

    for (i = 0; i < ndoubts; ++i) {
      sTmp.nvrtx = 3;
      /* Assign coordinates to simplex */
      for (k = 0; k < sTmp.nvrtx; ++k) {
        vtxid = TrianglesToTest[i + (k * 3)];
        for (j = 0; j < 3; ++j) {
          sTmp.vrtx[2 - k][j] = s->vrtx[vtxid][j];
        }
      }
      /* ... and call S2D  */
      S2D(&sTmp, v);
      /* ... compute aux squared distance */
      for (j = 0; j < 3; ++j) {
        vtmp[j] = 0;
        for (l = 0; l < sTmp.nvrtx; ++l) {
          vtmp[j] += sTmp.lambdas[l] * (sTmp.vrtx[l][j]);
        }
      }

      if (i == 0) {
        sqdist_tmp = dotProduct(vtmp, vtmp);
        nclosestS = sTmp.nvrtx;
        for (l = 0; l < nclosestS; ++l) {
          sID[l] = TrianglesToTest[i + (sTmp.wids[l] * 3)];
          tmplamda[l] = sTmp.lambdas[l];
        }
      }
      else if (dotProduct(vtmp, vtmp) < sqdist_tmp) {
        sqdist_tmp = dotProduct(vtmp, vtmp);
        nclosestS = sTmp.nvrtx;
        for (l = 0; l < nclosestS; ++l) {
          sID[l] = TrianglesToTest[i + (sTmp.wids[l] * 3)];
          tmplamda[l] = sTmp.lambdas[l];
        }
      }
    }

    for (i = 0; i < 4; ++i) {
      for (j = 0; j < 3; ++j) {
        tmpscoo1[i][j] = s->vrtx[i][j];
      }
    }

    /* Store closest simplex */
    s->nvrtx = nclosestS;
    for (i = 0; i < s->nvrtx; ++i) {
      for (j = 0; j < 3; ++j) {
        s->vrtx[nclosestS - 1 - i][j] = tmpscoo1[sID[i]][j];
      }
      s->lambdas[i] = tmplamda[i];
      s->wids[nclosestS - 1 - i] = sID[i];
    }
  }
  else if (FacetsTest[1] + FacetsTest[2] + FacetsTest[3] == 1) {
    /* There are two   facets facing the origin, need to find the closest.   */
    struct simplex  sTmp;
    sTmp.nvrtx = 3;

    if (FacetsTest[1] == 0) {
      /* ... Test facet ACD  */
      for (i = 0; i < 3; ++i) {
        sTmp.vrtx[0][i] = s->vrtx[0][i];
        sTmp.vrtx[1][i] = s->vrtx[1][i];
        sTmp.vrtx[2][i] = s->vrtx[3][i];
      }
      /* ... and call S2D  */
      S2D(&sTmp, v);
      /* ... compute aux squared distance */
      for (j = 0; j < 3; ++j) {
        vtmp[j] = 0;
        for (i = 0; i < sTmp.nvrtx; ++i) {
          vtmp[j] += sTmp.lambdas[i] * (sTmp.vrtx[i][j]);
        }
      }
      sqdist_tmp = dotProduct(vtmp, vtmp);
      Flag_sAuxused = 1;
      firstaux = 0;
    }
    if (FacetsTest[2] == 0) {
      /* ... Test facet ABD  */
      if (Flag_sAuxused == 0) {

        for (i = 0; i < 3; ++i) {
          sTmp.vrtx[0][i] = s->vrtx[0][i];
          sTmp.vrtx[1][i] = s->vrtx[2][i];
          sTmp.vrtx[2][i] = s->vrtx[3][i];
        }
        /* ... and call S2D  */
        S2D(&sTmp, v);
        /* ... compute aux squared distance */
        for (j = 0; j < 3; ++j) {
          vtmp[j] = 0;
          for (i = 0; i < sTmp.nvrtx; ++i) {
            vtmp[j] += sTmp.lambdas[i] * (sTmp.vrtx[i][j]);
          }
        }
        sqdist_tmp = dotProduct(vtmp, vtmp);
        firstaux = 1;
      }
      else {
        s->nvrtx = 3;
        for (i = 0; i < 3; ++i) {
          s->vrtx[0][i] = s->vrtx[0][i];
          s->vrtx[1][i] = s->vrtx[2][i];
          s->vrtx[2][i] = s->vrtx[3][i];
        }
        /* ... and call S2D itself */
        S2D(s, v);
        secondaux = 1;
      }
    }

    if (FacetsTest[3] == 0) {
      /* ... Test facet ABC  */
      s->nvrtx = 3;
      for (i = 0; i < 3; ++i) {
        s->vrtx[0][i] = s->vrtx[1][i];
        s->vrtx[1][i] = s->vrtx[2][i];
        s->vrtx[2][i] = s->vrtx[3][i];
      }
      /* ... and call S2D  */
      S2D(s, v);
      secondaux = 2;
    }
    /* Compare outcomes */
    for (j = 0; j < 3; ++j) {
      v[j] = 0;
      for (i = 0; i < s->nvrtx; ++i) {
        v[j] += s->lambdas[i] * (s->vrtx[i][j]);
      }
    }
    if (dotProduct(v, v) < sqdist_tmp) {
      /* Keep simplex. Need to update sID only*/
      for (i = 0; i < s->nvrtx; ++i) {
        /* Assume that vertex a is always included in sID. */
        s->wids[s->nvrtx - 1 - i] = TrianglesToTest[secondaux + (s->wids[i] * 3)];
      }
    }
    else {
      s->nvrtx = sTmp.nvrtx;
      for (i = 0; i < s->nvrtx; ++i) {
        for (j = 0; j < 3; ++j) {
          s->vrtx[i][j] = sTmp.vrtx[i][j];
        }
        s->lambdas[i] = sTmp.lambdas[i];
        s->wids[sTmp.nvrtx - 1 - i] = TrianglesToTest[firstaux + (sTmp.wids[i] * 3)];
      }
    }
  }
  else if (FacetsTest[1] + FacetsTest[2] + FacetsTest[3] == 2) {
    /* Only one facet is facing the origin */
    if (FacetsTest[1] == 0) {
      /* The origin projection P faces the facet ACD */
      s->nvrtx = 3;
      for (i = 0; i < 3; ++i) {
        s->vrtx[0][i] = s->vrtx[0][i];
        s->vrtx[1][i] = s->vrtx[1][i];
        s->vrtx[2][i] = s->vrtx[3][i];
      }
      S2D(s, v);

      /* Keep simplex. Need to update sID only*/
      for (i = 0; i < s->nvrtx; ++i) {
        s->wids[i] = s->wids[i];
      }
    }
    else if (FacetsTest[2] == 0) {
      /* The origin projection P faces the facet ABD */
      s->nvrtx = 3;
      for (i = 0; i < 3; ++i) {
        s->vrtx[0][i] = s->vrtx[0][i];
        s->vrtx[1][i] = s->vrtx[2][i];
        s->vrtx[2][i] = s->vrtx[3][i];
      }
      S2D(s, v);
      /* Keep simplex. Need to update sID only*/
      for (i = 2; i < s->nvrtx; ++i) {
        /* Assume that vertex a is always included in sID. */
        s->wids[i] = s->wids[i] + 1;
      }
    }
    else if (FacetsTest[3] == 0) {
      /* The origin projection P faces the facet ABC */
      s->nvrtx = 3;
      for (i = 0; i < 3; ++i) {
        s->vrtx[0][i] = s->vrtx[1][i];
        s->vrtx[1][i] = s->vrtx[2][i];
        s->vrtx[2][i] = s->vrtx[3][i];
      }
      S2D(s, v);
    }
  }
  else {
    /* The origin projection P faces the facet BCD */
    s->nvrtx = 3;
    for (i = 0; i < 3; ++i) {
      s->vrtx[0][i] = s->vrtx[0][i];
      s->vrtx[1][i] = s->vrtx[1][i];
      s->vrtx[2][i] = s->vrtx[2][i];
    }
    /* ... and call S2D   */
    S2D(s, v);
    /* Keep simplex. Need to update sID only*/
    for (i = 0; i < s->nvrtx; ++i) {
      /* Assume that vertex a is always included in sID. */
      s->wids[i] = s->wids[i] + 1;
    }
  }


  for ( j = 0; j < 3; ++j) {
    vv[j] = 0;
    for (i = 0; i < s->nvrtx; ++i) {
      vv[j] += (s->lambdas[i] * s->vrtx[i][j]);
    }
  }
}


inline static void support( struct bd *body, double *v ) {
  int    better = -1;
  int    i;
  double s;
  double maxs;
  double *vrt;

  maxs = dotProduct(body->s, v);

  for (i = 1; i < body->numpoints; ++i) {
    vrt = body->coord[i];
    s = dotProduct (vrt, v);
    if ( s > maxs ){
      maxs = s;
      better = i;
    }
  }

  if (better != -1 ){
    body->s[0] = body->coord[better][0];
    body->s[1] = body->coord[better][1];
    body->s[2] = body->coord[better][2];
  }
}

inline static void subalgorithm( struct simplex *s, double *v ) {

  switch ( s->nvrtx ){
    case 4:
      S3D( s, v );
      break;
    case 3:
      S2D( s, v );
      break;
    case 2:
      S1D( s, v );
      break;
  }
}

double gjk( struct bd bd1, struct bd bd2, struct simplex *s) {

  int    i;
  int    k = 0;                /**< Iteration counter            */
  int    mk = 50;              /**< Maximum number of iterations of the GJK algorithm */
  int    exeedtol_rel = 0;     /**< Flag for 1st exit condition  */
  double v[3];                 /**< Search direction             */
  double vminus[3];            /**< Search direction * -1        */
  double w[3];                 /**< Vertex on CSO boundary given by the difference of support functions on both bodies */
  double eps_rel = 1e-3;       /**< Tolerance on relative distance */
  double eps_tot = 1e-13;      /**< Tolerance on absolute distance */
  double norm2Wmax = 0;
  double tesnorm = 0;

#ifdef ADAPTIVEFP
  exactinit();
#endif

  double eps_rel2 = eps_rel * eps_rel  ;

  /* Initialise  */
  s->nvrtx = 1;
  for ( i = 0; i < 3; i++)
  {
    v[i] = bd1.coord[0][i] - bd2.coord[0][i];
    bd1.s[i] = bd1.coord[0][i];
    bd2.s[i] = bd2.coord[0][i];
    s->vrtx[0][i] = v[i];
  }

  /* Begin GJK iteration */
  do {

    k++;

    /* Update negative search direction */
    vminus[0] = -v[0];
    vminus[1] = -v[1];
    vminus[2] = -v[2];

    /* Support function */
    support( &bd1 , vminus );
    support( &bd2 , v );
    w[0] = bd1.s[0] - bd2.s[0];
    w[1] = bd1.s[1] - bd2.s[1];
    w[2] = bd1.s[2] - bd2.s[2];

    /* 1st exit condition */
    exeedtol_rel = (norm2(v) - dotProduct (v,w) ) <= eps_rel2 * norm2(v);
    if ( exeedtol_rel ){
      break;
    }

    /* 2nd exit condition */
    if (norm2(v) < eps_rel2)
      break;

    /* Add new vertex to simplex */
    i = s->nvrtx;
    s->vrtx[i][0] = w[0];
    s->vrtx[i][1] = w[1];
    s->vrtx[i][2] = w[2];
    s->nvrtx++;

    /* Invoke distance sub-algorithm */
    subalgorithm ( s, v );

    /* 3rd exit condition */
    for (i = 0; i < s->nvrtx; i++)
    {
      tesnorm = norm2(s->vrtx[i]);
      if (tesnorm > norm2Wmax) {
        norm2Wmax = tesnorm;
      }
    }
    if (norm2(v) <= (eps_tot * eps_tot * norm2Wmax))
      break;

    /* 4th and 5th exit conditions */
  } while((s->nvrtx != 4) && (k != mk));

  /* Compute and return distance */
  return sqrt(norm2(v));
}

#ifdef MATLABDOESMEXSTUFF
/**
 * @brief Mex function for Matlab.
 */
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

  double *inCoordsA;
  double *inCoordsB;
  size_t  nCoordsA;
  size_t  nCoordsB;
  int     i;
  double *distance;
  int     c = 3;
  int     count = 0;
  double**arr1;
  double**arr2;

  /**************** PARSE INPUTS AND OUTPUTS **********************/
  /*----------------------------------------------------------------*/
  /* Examine input (right-hand-side) arguments. */
  if(nrhs!=2) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:nrhs","Two inputs required.");
  }
  /* Examine output (left-hand-side) arguments. */
  if(nlhs!=1) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:nlhs","One output required.");
  }

  /* make sure the two input arguments are any numerical type */
  /* .. first input */
  if( !mxIsNumeric(prhs[0])) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notNumeric","Input matrix must be type numeric.");
  }
  /* .. second input */
  if( !mxIsNumeric(prhs[1])) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notNumeric","Input matrix must be type numeric.");
  }

  /* make sure the two input arguments have 3 columns */
  /* .. first input */
  if(mxGetM(prhs[0])!=3) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notColumnVector","First input must have 3 columns.");
  }
  /* .. second input */
  if(mxGetM(prhs[1])!=3) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notColumnVector","Second input must have 3 columns.");
  }

  /*----------------------------------------------------------------*/
  /* CREATE DATA COMPATIBLE WITH MATALB  */

  /* create a pointer to the real data in the input matrix  */
  inCoordsA = mxGetPr(prhs[0]);
  inCoordsB = mxGetPr(prhs[1]);

  /* get the length of each input vector */
  nCoordsA = mxGetN(prhs[0]);
  nCoordsB = mxGetN(prhs[1]);

  /* Create output */
  plhs[0]=mxCreateDoubleMatrix(1,1,mxREAL);

  /* get a pointer to the real data in the output matrix */
  distance = mxGetPr(plhs[0]);

   /* Copy data from Matlab's vectors into two new arrays */
  arr1 = (double **)mxMalloc( sizeof(double *) * (int)nCoordsA );
  arr2 = (double **)mxMalloc( sizeof(double *) * (int)nCoordsB );

  for (i = 0; i < nCoordsA; i++)
    arr1[i] = &inCoordsA[i  * 3];

  for (i = 0; i < nCoordsB; i++)
    arr2[i] = &inCoordsB[i  * 3];

  /*----------------------------------------------------------------*/
  /* POPULATE BODIES' STRUCTURES  */

  struct bd       bd1; /* Structure of body A */
  struct bd       bd2; /* Structure of body B */

  /* Assign number of vertices to each body */
  bd1.numpoints = (int) nCoordsA;
  bd2.numpoints = (int) nCoordsB;

  bd1.coord = arr1;
  bd2.coord = arr2;

  /*----------------------------------------------------------------*/
  /*CALL COMPUTATIONAL ROUTINE  */

  struct simplex s;
  s.nvrtx = 0;

  /* Compute squared distance using GJK algorithm */
  distance[0] = gjk (bd1, bd2, &s);

  mxFree(arr1);
  mxFree(arr2);

}
#endif

/**
 * @brief Invoke this function from C# applications
 */
double csFunction( int nCoordsA, double *inCoordsA, int nCoordsB, double *inCoordsB )
{
  double distance = 0;
  int i, j;

  /*----------------------------------------------------------------*/
  /* POPULATE BODIES' STRUCTURES  */

  struct bd       bd1; /* Structure of body A */
  struct bd       bd2; /* Structure of body B */

  /* Assign number of vertices to each body */
  bd1.numpoints = (int) nCoordsA;
  bd2.numpoints = (int) nCoordsB;

  double **pinCoordsA = (double **)malloc(bd1.numpoints * sizeof(double *));
  for (i=0; i< bd1.numpoints ; i++)
    pinCoordsA[i] = (double *)malloc(3 * sizeof(double));

  for (i = 0; i <  3; i++)
    for (j = 0; j < bd1.numpoints; j++)
      pinCoordsA[j][i] = inCoordsA[ i*bd1.numpoints + j] ;

  double **pinCoordsB = (double **)malloc(bd2.numpoints * sizeof(double *));
  for (i=0; i< bd2.numpoints ; i++)
    pinCoordsB[i] = (double *)malloc(3 * sizeof(double));

  for (i = 0; i <  3; i++)
    for (j = 0; j < bd2.numpoints; j++)
      pinCoordsB[j][i] = inCoordsB[ i*bd2.numpoints + j] ;

  bd1.coord = pinCoordsA;
  bd2.coord = pinCoordsB;


  /*----------------------------------------------------------------*/
  /*CALL COMPUTATIONAL ROUTINE  */
  struct simplex s;

  /* Initialise simplex as empty */
  s.nvrtx = 0;

  /* Compute squared distance using GJK algorithm */
  distance = gjk (bd1, bd2, &s);

  for (i=0; i< bd1.numpoints ; i++)
    free(pinCoordsA[i]);
  free( pinCoordsA );

  for (i=0; i< bd2.numpoints ; i++)
    free(pinCoordsB[i]);
  free( pinCoordsB );

  return distance;
}
