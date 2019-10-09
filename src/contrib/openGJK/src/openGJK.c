/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  *
 *                                   #####        # #    #                *
 *       ####  #####  ###### #    # #     #       # #   #                 *
 *      #    # #    # #      ##   # #             # #  #                  *
 *      #    # #    # #####  # #  # #  ####       # ###                   *
 *      #    # #####  #      #  # # #     # #     # #  #                  *
 *      #    # #      #      #   ## #     # #     # #   #                 *
 *       ####  #      ###### #    #  #####   #####  #    #                *
 *                                                                        *
 *  This file is part of openGJK.                                         *
 *                                                                        *
 *  openGJK is free software: you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by *
 *   the Free Software Foundation, either version 3 of the License, or    *
 *   any later version.                                                   *
 *                                                                        *
 *   openGJK is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See The        *
 *   GNU General Public License for more details.                         *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *   along with Foobar.  If not, see <https://www.gnu.org/licenses/>.     *
 *                                                                        *
 *       openGJK: open-source Gilbert-Johnson-Keerthi algorithm           *
 *            Copyright (C) Mattia Montanari 2018 - 2019                  *
 *              http://iel.eng.ox.ac.uk/?page_id=504                      *
 *                                                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

#include "openGJK/openGJK.h"

/* If instricuted, compile a mex function for Matlab.*/
#ifdef MATLABDOESMEXSTUFF
#include "mex.h"
#else
#define mexPrintf  printf
#endif

#define eps_rel22  5e-4
#define eps_tot22  1e-12

#define norm2(a) (a[0]*a[0]+a[1]*a[1]+a[2]*a[2])

#define dotProduct(a, b) (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])

#define S3Dregion1234()                 v[0] = 0;\
                                        v[1] = 0;\
                                        v[2] = 0;\
                                        s->nvrtx = 4;

#define select_1ik()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s->vrtx[3][t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = si[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

#define select_1ij()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s->vrtx[3][t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = si[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sj[t];

#define select_1jk()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s->vrtx[3][t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = sj[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

#define select_1i()   s->nvrtx = 2;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = s->vrtx[3][t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = si[t];

#define select_1j()   s->nvrtx = 2;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = s->vrtx[3][t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sj[t];

#define select_1k()   s->nvrtx = 2;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = s->vrtx[3][t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

#define getvrtx(point, location)        point[0] = s->vrtx[location][0];\
                                        point[1] = s->vrtx[location][1];\
                                        point[2] = s->vrtx[location][2];

#define calculateEdgeVector(p1p2, p2)   p1p2[0] = p2[0] - s->vrtx[3][0];\
                                        p1p2[1] = p2[1] - s->vrtx[3][1];\
                                        p1p2[2] = p2[2] - s->vrtx[3][2];

#define S1Dregion1()                    v[0] = s->vrtx[1][0];\
                                        v[1] = s->vrtx[1][1];\
                                        v[2] = s->vrtx[1][2];\
                                        s->nvrtx = 1;\
                                        s->vrtx[0][0] = s->vrtx[1][0];\
                                        s->vrtx[0][1] = s->vrtx[1][1];\
                                        s->vrtx[0][2] = s->vrtx[1][2];

#define S2Dregion1()                    v[0] = s->vrtx[2][0];\
                                        v[1] = s->vrtx[2][1];\
                                        v[2] = s->vrtx[2][2];\
                                        s->nvrtx = 1;\
                                        s->vrtx[0][0] = s->vrtx[2][0];\
                                        s->vrtx[0][1] = s->vrtx[2][1];\
                                        s->vrtx[0][2] = s->vrtx[2][2];

#define S2Dregion12()                   s->nvrtx = 2;\
                                        s->vrtx[0][0] = s->vrtx[2][0];\
                                        s->vrtx[0][1] = s->vrtx[2][1];\
                                        s->vrtx[0][2] = s->vrtx[2][2];

#define S2Dregion13()                   s->nvrtx = 2;\
                                        s->vrtx[1][0] = s->vrtx[2][0];\
                                        s->vrtx[1][1] = s->vrtx[2][1];\
                                        s->vrtx[1][2] = s->vrtx[2][2];

#define S3Dregion1()                    v[0] = s1[0];\
                                        v[1] = s1[1];\
                                        v[2] = s1[2];\
                                        s->nvrtx = 1;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];
                                        
inline static double determinant(const double *p, const double *q, const double *r) {
  return p[0] * ((q[1] * r[2]) - (r[1] * q[2])) - p[1] * (q[0] * r[2] - r[0] * q[2]) + p[2] * (q[0] * r[1] - r[0] * q[1]);
}

inline static void crossProduct(const double *a, const double *b, double *c)
{
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
}

inline static void projectOnLine(const double *p, const double *q, double *v)
{
  double pq[3];
  double tmp;
  pq[0] = p[0] - q[0];
  pq[1] = p[1] - q[1];
  pq[2] = p[2] - q[2];

  tmp = dotProduct(p, pq) / dotProduct(pq, pq);

  for (int i = 0; i < 3; i++)
    v[i] = p[i] - pq[i] * tmp;
}

inline static void projectOnPlane(const double *p, const double *q, const double *r, double *v)
{
  double n[3], pq[3], pr[3];
  double tmp;

  for (int i = 0; i < 3; i++)
    pq[i] = p[i] - q[i];

  for (int i = 0; i < 3; i++)
    pr[i] = p[i] - r[i];

  crossProduct(pq, pr, n);
  tmp = dotProduct(n, p) / dotProduct(n, n);

  for (int i = 0; i < 3; i++)
    v[i] = n[i] * tmp;
}

inline static int hff1(const double *p, const double *q)
{
  double tmp = 0;

#pragma omp simd reduction(+:tmp)
  for (int i = 0; i < 3; i++)
    tmp += (p[i] * p[i] - p[i] * q[i]);

  if (tmp > 0)
    return 1; // keep q

  return 0;
}

inline static int hff2(const double *p, const double *q, const double *r)
{
  double ntmp[3];
  double n[3], pq[3], pr[3];
  double tmp = 0;

  for (int i = 0; i < 3; i++)
    pq[i] = q[i] - p[i];

  for (int i = 0; i < 3; i++)
    pr[i] = r[i] - p[i];

  crossProduct(pq, pr, ntmp);
  crossProduct(pq, ntmp, n);

#pragma omp simd reduction(+:tmp)
  for (int i = 0; i < 3; i++)
    tmp = tmp + (p[i] * n[i]);

  if (tmp < 0)
    return 1; // Discard r

  return 0;
}

inline static int hff3(const double *p, const double *q, const double *r)
{
  double n[3], pq[3], pr[3];
  double tmp = 0;

  for (int i = 0; i < 3; i++)
    pq[i] = q[i] - p[i];

  for (int i = 0; i < 3; i++)
    pr[i] = r[i] - p[i];

  crossProduct(pq, pr, n);

#pragma omp simd reduction(+:tmp)
  for (int i = 0; i < 3; i++)
    tmp = tmp + (p[i] * n[i]);

  if (tmp > 0)
    return 0; // discard s

  return 1;
}

inline static void S1D(struct simplex * s, double *v)
{
  double *s1p = s->vrtx[1];
  double *s2p = s->vrtx[0];

  if (hff1(s1p, s2p)) {
    projectOnLine(s1p, s2p, v); // Update v, no need to update s
    return; 
  }
  else {
    S1Dregion1();
    return;
  }
}


inline static void S2D(struct simplex * s, double *v)
{
  double *s1p = s->vrtx[2];
  double *s2p = s->vrtx[1];
  double *s3p = s->vrtx[0];
  int     hff1f_s12 = hff1(s1p, s2p);
  int     hff1f_s13 = hff1(s1p, s3p);
  int     hff2f_23 = !hff2(s1p, s2p, s3p);
  int     hff2f_32 = !hff2(s1p, s3p, s2p);

  if (hff1f_s12) {
    if (hff2f_23) {
      if (hff1f_s13) {
        if (hff2f_32) {
          projectOnPlane(s1p, s2p, s3p, v); // Update s, no need to update c
          return;
        }
        else
        {
          projectOnLine(s1p, s3p, v); // Update v
          S2Dregion13();
          return;
        }
      }
      else
      {
        projectOnPlane(s1p, s2p, s3p, v);
        return;
      }
    }
    else
    {
      projectOnLine(s1p, s2p, v); // Update v
      S2Dregion12(); // Update s
      return; // Return V{1,2}
    }
  }
  else if (hff1f_s13) {
    if (hff2f_32) {
      projectOnPlane(s1p, s2p, s3p, v); // Update s, no need to update c
      return; // Return V{1,2,3}
    }
    else
    {
      projectOnLine(s1p, s3p, v);
      S2Dregion13();
      return;
    }
  }
  else {
    S2Dregion1(); 
    return;
  }

}

inline static void S3D(struct simplex * s, double *v) {

  double s1[3], s2[3], s3[3], s4[3], s1s2[3], s1s3[3], s1s4[3];
  double si[3], sj[3], sk[3];
  int testLineThree, testLineFour, testPlaneTwo, testPlaneThree, testPlaneFour, dotTotal;
  int i, j, k, t;

  getvrtx(s1, 3);
  getvrtx(s2, 2);
  getvrtx(s3, 1);
  getvrtx(s4, 0);
  calculateEdgeVector(s1s2, s2);
  calculateEdgeVector(s1s3, s3);
  calculateEdgeVector(s1s4, s4);

  int hff1_tests[3];
  hff1_tests[2] = hff1(s1, s2);
  hff1_tests[1] = hff1(s1, s3);
  hff1_tests[0] = hff1(s1, s4);
  testLineThree = hff1(s1, s3);
  testLineFour = hff1(s1, s4);

  dotTotal = hff1(s1, s2) + testLineThree + testLineFour;
  if (dotTotal == 0) {
    S3Dregion1();
    return;
  }

  double det134 = determinant(s1s3, s1s4, s1s2);
  int sss;
  if (det134 > 0) {
    sss = 0;
  }
  else {
    sss = 1;
  }

  testPlaneTwo = hff3(s1, s3, s4) - sss;
  testPlaneTwo = testPlaneTwo * testPlaneTwo;
  testPlaneThree = hff3(s1, s4, s2) - sss;
  testPlaneThree = testPlaneThree * testPlaneThree;
  testPlaneFour = hff3(s1, s2, s3) - sss;
  testPlaneFour = testPlaneFour * testPlaneFour;

  switch (testPlaneTwo + testPlaneThree + testPlaneFour) {
    case 3:
    S3Dregion1234();
      break;

    case 2:
      // Only one facing the oring
      s->nvrtx = 3;
      if (!testPlaneTwo) { 
        for (i = 0; i < 3; i++)
          s->vrtx[2][i] = s->vrtx[3][i];
      }
      else if (!testPlaneThree) {
        for (i = 0; i < 3; i++)
          s->vrtx[1][i] = s2[i];
        for (i = 0; i < 3; i++)
          s->vrtx[2][i] = s->vrtx[3][i];
      }
      else if (!testPlaneFour) { 
        for (i = 0; i < 3; i++)
          s->vrtx[0][i] = s3[i];
        for (i = 0; i < 3; i++)
          s->vrtx[1][i] = s2[i];
        for (i = 0; i < 3; i++)
          s->vrtx[2][i] = s->vrtx[3][i];
      }
      // Call S2D
      S2D(s, v);
      break;
    case 1:
      // Two triangles face the origins:
      // 1,i,j, are the indices of the points on the triangle and remove k from simplex
      s->nvrtx = 3;
      if (testPlaneTwo) {
        k = 2;  i = 1; j = 0;
      }
      else if (testPlaneThree) {
        k = 1;  i = 0; j = 2;
      }
      else {
        k = 0;  i = 2; j = 1;
      }

      getvrtx(si, i);
      getvrtx(sj, j);
      getvrtx(sk, k);

      if (dotTotal == 1) {
        if (hff1_tests[k]) {
          if (!hff2(s1, sk, si)) {
            select_1ik();
            projectOnPlane(s1, si, sk, v);
          }
          else if (!hff2(s1, sk, sj)) {
            select_1jk();
            projectOnPlane(s1, sj, sk, v);
          }
          else {
            select_1k();
            projectOnLine(s1, sk, v);
          }
        }
        else if (hff1_tests[i]) {
          if (!hff2(s1, si, sk)) {
            select_1ik();
            projectOnPlane(s1, si, sk, v);
          }
          else {
            select_1i(); // select region 1i
            projectOnLine(s1, si, v);
          }
        }
        else {
          if (!hff2(s1, sj, sk)) {
            select_1jk();
            projectOnPlane(s1, sj, sk, v);
          }
          else {
            select_1j(); // select region 1i
            projectOnLine(s1, sj, v);
          }
        }
      }
      else if (dotTotal == 2) {
        // Two edges have positive hff1

        if (hff1_tests[i]) {
          if (!hff2(s1, sk, si))
            if (!hff2(s1, si, sk)) {
              select_1ik();     // select region 1ik
              projectOnPlane(s1, si, sk, v);
            }
            else {
              select_1k(); // select region 1k
              projectOnLine(s1, sk, v);
            }
          else {
            if (!hff2(s1, sk, sj)) {
              select_1jk();  // select region 1jk
              projectOnPlane(s1, sj, sk, v);
            }
            else {
              select_1k();   // select region 1k
              projectOnLine(s1, sk, v);
            }
          }
        }
        else if (hff1_tests[j]) {
          if (!hff2(s1, sk, sj))
            if (!hff2(s1, sj, sk)) {
              select_1jk();       // select region 1jk
              projectOnPlane(s1, sj, sk, v);
            }
            else {
              select_1j();        // select region 1j
              projectOnLine(s1, sj, v);
            }
          else {
            if (!hff2(s1, sk, si)) {
              select_1ik();       // select region 1ik
              projectOnPlane(s1, si, sk, v);
            }
            else {
              select_1k();        // select region 1k
              projectOnLine(s1, sk, v);
            }
          }
        }
        else {
          // ERROR;
        }

      }
      else if (dotTotal == 3) {
        // sk is s.t. hff3 for sk < 0

        int hff2_ik = hff2(s1,si,sk);
        int hff2_jk = hff2(s1,sj,sk);
        int hff2_ki = hff2(s1,sk,si);
        int hff2_kj = hff2(s1,sk,sj);

        if (hff2_ki == 0 && hff2_kj == 0){
          mexPrintf("\n\n UNEXPECTED VALUES!!! \n\n");
        }
        if (hff2_ki == 1 && hff2_kj == 1){
          select_1k();
          projectOnLine(s1, sk, v);
        }
        else if (hff2_ki) {
          if (hff2_jk){
            // discard k
            select_1j();
            projectOnLine(s1, sj, v);
          }
          else{
            select_1jk();
            projectOnPlane(s1, sk, sj, v);
          }
        }
        else {
          if (hff2_ik){
            // discard k
            select_1i();
            projectOnLine(s1, si, v);
          }
          else{
            select_1ik();
            projectOnPlane(s1, sk, si, v);
          }
        }
      }
      break;

    case 0:
      if (dotTotal == 1) {
        if (testLineThree) {
          k = 2;  i = 1; j = 0;
        }
        else if (testLineFour) {
          k = 1;  i = 0; j = 2;
        }
        else {
          k = 0;  i = 2; j = 1;
        }
        getvrtx(si, i);
        getvrtx(sj, j);
        getvrtx(sk, k);

        if (!hff2(s1, si, sj)) {
          select_1ij();
          projectOnPlane(s1, si, sj, v);
        }
        else if (!hff2(s1, si, sk)) {
          select_1ik();
          projectOnPlane(s1, si, sk, v);
        }
        else {
          select_1i();
          projectOnLine(s1, si, v);
        }
      }
      else if (dotTotal == 2) {
        // Here si is set such that hff(s1,si) < 0
        s->nvrtx = 3;
        if (!testLineThree) {
          k = 2;  i = 1;  j = 0;
        }
        else if (!testLineFour) {
          k = 1; i = 0;   j = 2;
        }
        else {
          k = 0; i = 2;  j = 1;
        }
        getvrtx(si, i);
        getvrtx(sj, j);
        getvrtx(sk, k);

        if (!hff2(s1, sj, sk)) {
          if (!hff2(s1, sk, sj)) {
            select_1jk();  // select region 1jk
            projectOnPlane(s1, sj, sk, v);
          }
          else if (!hff2(s1, sk, si)) {
            select_1ik();
            projectOnPlane(s1, sk, si, v);
          }
          else {
            select_1k();
            projectOnLine(s1, sk, v);
          }
        }
        else if (!hff2(s1, sj, si)) {
          select_1ij();
          projectOnPlane(s1, si, sj, v);
        }
        else {
          select_1j();
          projectOnLine(s1, sj, v);
        }
      }
      break;
    default:
      mexPrintf("\nERROR:\tunhandled");
  }

}

inline static void support(struct bd *body, const double *v) {

  double s, maxs;
  double *vrt;
  int better = -1;

  maxs = dotProduct(body->s, v);

  for (int i = 0; i < body->numpoints; ++i) {
    vrt = body->coord[i];
    s = dotProduct(vrt, v);
    if (s > maxs) {
      maxs = s;
      better = i;
    }
  }

  if (better != -1) {
    body->s[0] = body->coord[better][0];
    body->s[1] = body->coord[better][1];
    body->s[2] = body->coord[better][2];
  }
}

inline static void subalgorithm(struct simplex *s, double *v) {

  switch (s->nvrtx) {
    case 4:
      S3D(s, v);
      break;
    case 3:
      S2D(s, v);
      break;
    case 2:
      S1D(s, v);
      break;
    default:
      mexPrintf("\nERROR:\t invalid simplex\n");
  }
}

double gjk(struct bd bd1, struct bd bd2, struct simplex *s) {

  int k = 0;                /**< Iteration counter            */
  int i;                    /**< General purpose counter      */
  int mk = 5000;            /**< Maximum number of iterations of the GJK algorithm */
  int absTestin;
  double norm2Wmax = 0;
  double tesnorm;
  double v[3];              /**< Search direction             */
  double vminus[3];         /**< Search direction * -1        */
  double w[3];              /**< Vertex on CSO boundary given by the difference of support functions on both bodies */
  double eps_rel = eps_rel22;   /**< Tolerance on relative        */
  double eps_rel2 = eps_rel * eps_rel;
  double eps_tot = eps_tot22;
  int exeedtol_rel = 0;     /**< Flag for 1st exit condition  */
  int nullV = 0;

  /* Initialise search direction */
  v[0] = bd1.coord[0][0] - bd2.coord[0][0];
  v[1] = bd1.coord[0][1] - bd2.coord[0][1];
  v[2] = bd1.coord[0][2] - bd2.coord[0][2];

  /* Inialise simplex */
  s->nvrtx = 1;
  for (int t = 0; t < 3; ++t)
    s->vrtx[0][t] = v[t];

  for (int t = 0; t < 3; ++t)
    bd1.s[t] = bd1.coord[0][t];

  for (int t = 0; t < 3; ++t)
    bd2.s[t] = bd2.coord[0][t];

  /* Begin GJK iteration */
  do {

    k++;

    /* Update negative search direction */
    for (int t = 0; t < 3; ++t)
      vminus[t] = -v[t];

    /* Support function */
    support(&bd1, vminus);
    support(&bd2, v);
    for (int t = 0; t < 3; ++t)
      w[t] = bd1.s[t] - bd2.s[t];

    /* Test first exit condition (new point already in simplex/can't move further) */
    exeedtol_rel = (norm2(v) - dotProduct(v, w)) <= eps_rel2 * norm2(v);
    if (exeedtol_rel) {
      break;
    }

    nullV = norm2(v) < eps_rel2;
    if (nullV) {
      break;
    }

    /* Add new vertex to simplex */
    i = s->nvrtx;
    for (int t = 0; t < 3; ++t)
      s->vrtx[i][t] = w[t];
    s->nvrtx++;

    /* Invoke distance sub-algorithm */
    subalgorithm(s, v);

    for (int jj = 0; jj < s->nvrtx; jj++) {
      tesnorm = norm2(s->vrtx[jj]);
      if (tesnorm > norm2Wmax) {
        norm2Wmax = tesnorm;
      }
    }

    absTestin = (norm2(v) <= (eps_tot * eps_tot * norm2Wmax));
    if (absTestin)
      break;

  } while ((s->nvrtx != 4) && (k != mk));

  if (k == mk) {
    mexPrintf("\n * * OPENGJK REACHED MAXIMUM ITERATION NUMBER ! * * * * * * * * \n");
  }

  return sqrt(norm2(v));
}



#ifdef MATLABDOESMEXSTUFF
/**
 * @brief Mex function for Matlab.
 */
void mexFunction(int nlhs, mxArray *plhs[],
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
  if (nrhs != 2) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:nrhs", "Two inputs required.");
  }
  /* Examine output (left-hand-side) arguments. */
  if (nlhs != 1) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:nlhs", "One output required.");
  }

  /* make sure the two input arguments are any numerical type */
  /* .. first input */
  if (!mxIsNumeric(prhs[0])) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notNumeric", "Input matrix must be type numeric.");
  }
  /* .. second input */
  if (!mxIsNumeric(prhs[1])) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notNumeric", "Input matrix must be type numeric.");
  }

  /* make sure the two input arguments have 3 columns */
  /* .. first input */
  if (mxGetM(prhs[0]) != 3) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notColumnVector", "First input must have 3 columns.");
  }
  /* .. second input */
  if (mxGetM(prhs[1]) != 3) {
    mexErrMsgIdAndTxt("MyToolbox:gjk:notColumnVector", "Second input must have 3 columns.");
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
  plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);

  /* get a pointer to the real data in the output matrix */
  distance = mxGetPr(plhs[0]);

  /* Copy data from Matlab's vectors into two new arrays */
  arr1 = (double **)mxMalloc(sizeof(double *) * (int)nCoordsA);
  arr2 = (double **)mxMalloc(sizeof(double *) * (int)nCoordsB);

  for (i = 0; i < nCoordsA; i++)
    arr1[i] = &inCoordsA[i * 3];

  for (i = 0; i < nCoordsB; i++)
    arr2[i] = &inCoordsB[i * 3];

  /*----------------------------------------------------------------*/
  /* POPULATE BODIES' STRUCTURES  */

  struct bd       bd1; /* Structure of body A */
  struct bd       bd2; /* Structure of body B */

  /* Assign number of vertices to each body */
  bd1.numpoints = (int)nCoordsA;
  bd2.numpoints = (int)nCoordsB;

  bd1.coord = arr1;
  bd2.coord = arr2;

  /*----------------------------------------------------------------*/
  /*CALL COMPUTATIONAL ROUTINE  */

  struct simplex s;
  s.nvrtx = 0;

  /* Compute squared distance using GJK algorithm */
  distance[0] = gjk(bd1, bd2, &s);

  mxFree(arr1);
  mxFree(arr2);

}
#endif

/**
 * @brief Invoke this function from C# applications
 */
double csFunction(int nCoordsA, double *inCoordsA, int nCoordsB, double *inCoordsB)
{
  double distance = 0;
  int i, j;

  /*----------------------------------------------------------------*/
  /* POPULATE BODIES' STRUCTURES  */

  struct bd       bd1; /* Structure of body A */
  struct bd       bd2; /* Structure of body B */

  /* Assign number of vertices to each body */
  bd1.numpoints = (int)nCoordsA;
  bd2.numpoints = (int)nCoordsB;

  double **pinCoordsA = (double **)malloc(bd1.numpoints * sizeof(double *));
  for (i = 0; i < bd1.numpoints; i++)
    pinCoordsA[i] = (double *)malloc(3 * sizeof(double));

  for (i = 0; i < 3; i++)
    for (j = 0; j < bd1.numpoints; j++)
      pinCoordsA[j][i] = inCoordsA[i*bd1.numpoints + j];

  double **pinCoordsB = (double **)malloc(bd2.numpoints * sizeof(double *));
  for (i = 0; i < bd2.numpoints; i++)
    pinCoordsB[i] = (double *)malloc(3 * sizeof(double));

  for (i = 0; i < 3; i++)
    for (j = 0; j < bd2.numpoints; j++)
      pinCoordsB[j][i] = inCoordsB[i*bd2.numpoints + j];

  bd1.coord = pinCoordsA;
  bd2.coord = pinCoordsB;


  /*----------------------------------------------------------------*/
  /*CALL COMPUTATIONAL ROUTINE  */
  struct simplex s;

  /* Initialise simplex as empty */
  s.nvrtx = 0;

  /* Compute squared distance using GJK algorithm */
  distance = gjk(bd1, bd2, &s);

  for (i = 0; i < bd1.numpoints; i++)
    free(pinCoordsA[i]);
  free(pinCoordsA);

  for (i = 0; i < bd2.numpoints; i++)
    free(pinCoordsB[i]);
  free(pinCoordsB);

  return distance;
}
