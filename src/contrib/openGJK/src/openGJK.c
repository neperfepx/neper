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

#ifdef FAST
#define eps_rel22  1e-3
#define eps_tot22  1e-10
#else
#define eps_rel22  2e-5
#define eps_tot22  1e-13
#endif

/* Select distance sub-algorithm */
#define USE_EG 
#define NEWMAT

#ifdef USE_EG
#define INCREMENTAL 1
#else
#define INCREMENTAL 0 // use Signed Volumes method
#endif

#define norm2(a) (a[0]*a[0]+a[1]*a[1]+a[2]*a[2])

#define dotProduct(a, b) (a[0]*b[0]+a[1]*b[1]+a[2]*b[2])

#define differentSign( a, b ) ( (a>0) != (b>0) )

#if INCREMENTAL
#ifdef NEWMAT

// This divides!
#define calculateFace(N)                scalingFactor = 1 / norm2(N)*dotProduct(N, s1);\
                                        v[0] = N[0] * scalingFactor;\
                                        v[1] = N[1] * scalingFactor;\
                                        v[2] = N[2] * scalingFactor;

// This divides!
#define S3Dregion123()                  calculateFace(N4);\
                                        s->nvrtx = 3;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

// This divides!
#define S3Dregion124()                  calculateFace(N3);\
                                        s->nvrtx = 3;\
                                        s->vrtx[1][0] = s1[0];\
                                        s->vrtx[1][1] = s1[1];\
                                        s->vrtx[1][2] = s1[2];

// This divides!
#define S3Dregion134()                  calculateFace(N2);\
                                        s->nvrtx = 3;\
                                        s->vrtx[2][0] = s1[0];\
                                        s->vrtx[2][1] = s1[1];\
                                        s->vrtx[2][2] = s1[2];

#define S3Dregion1234()                 v[0] = 0;\
                                        v[1] = 0;\
                                        v[2] = 0;\
                                        s->nvrtx = 4;

#define select_1ik()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = si[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

#define select_1ik()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = si[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

#define select_1ij()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = si[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sj[t];

#define select_1jk()  s->nvrtx = 3;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[2][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = sj[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

#define select_1i()   s->nvrtx = 2;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = si[t];

#define select_1j()   s->nvrtx = 2;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sj[t];

#define select_1k()   s->nvrtx = 2;\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[1][t] = s1[t];\
                      for (t = 0; t < 3; t++)\
                        s->vrtx[0][t] = sk[t];

inline static int sign(double x){
  if (x > 0) return 1;
  if (x < 0) return -1;
  return 0;
}

inline static double determinant(double *p, double *q, double *r) {
  return p[0] * ((q[1] * r[2]) - (r[1] * q[2])) - p[1] * (q[0] * r[2] - r[0] * q[2]) + p[2] * (q[0] * r[1] - r[0] * q[1]);
}
    
inline static void crossProduct(double *a, double *b, double *c)
{
  c[0] = a[1] * b[2] - a[2] * b[1];
  c[1] = a[2] * b[0] - a[0] * b[2];
  c[2] = a[0] * b[1] - a[1] * b[0];
  return;
}

inline static void projectOnLine(double *p, double *q, double *v)
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

inline static void projectOnPlane(double *p, double *q, double *r, double *v)
{
  double n[3], pq[3], pr[3];
  double tmp;

  for (int i = 0; i < 3; i++)
    pq[i] = p[i] - q[i];

  for (int i = 0; i < 3; i++)
    pr[i] = p[i] - r[i];

  crossProduct(pq, pr, n);
  tmp = dotProduct(n, p) / dotProduct(n,n);

  for (int i = 0; i < 3; i++)
    v[i] = n[i] * tmp;
}



inline static int hff1(double *p, double *q)
{
  double tmp = 0;

  for (int i = 0; i < 3; i++)
    tmp = tmp + (p[i] * (q[i] - p[i]));
  //tmp = p[i] * q[i] - p[i] * p[i]; // More robust than p[i] * (q[i] - p[i])

  if (tmp < 0)
    return 1;
  else
    return 0;
}

inline static int hff2(double *p, double *q, double *r)
{
  double ntmp[3] ;
  double n[3], pq[3], pr[3];
  double tmp = 0;

  for (int i = 0; i < 3; i++)
    pq[i] = p[i] - q[i];

  for (int i = 0; i < 3; i++)
    pr[i] = p[i] - r[i];

  crossProduct(pq, pr, ntmp);
  crossProduct(ntmp, pq, n);

  for (int i = 0; i < 3; i++)
    tmp = tmp + (n[i] * q[i]);

  if (tmp < 0)
    return 1;
  else
    return 0;
}

inline static int hff3(double *p, double *q, double *r, double *s)
{
  double n[3], pq[3], pr[3];
  double tmp = 0;

  for (int i = 0; i < 3; i++)
    pq[i] = q[i] - p[i];

  for (int i = 0; i < 3; i++)
    pr[i] = r[i] - p[i];

  crossProduct(pr, pq, n);

  if (dotProduct(n,p) > 0)
    return 1; // The origin is facing the outer side of the face
  else
    return 0; // The origin is facing the inner side of the face (sort of "inside"). 
}



#define edgeTestPrep(x, edg12, pnt3)      calculateEdge(x, edg12);\
                                          calculateEdgeVector(edg13, pnt3);\
										                      calculateNormalVector(Ntmp,edg12,edg13);\
										                      calculateNormalVector(tempVect,edg12,Ntmp);
#define edgeTest()   					            dotProduct(s1,tempVect) > 0
#define edgeTest_OLD()                      dotProduct(v, tempVect) > 0


#else
// This divides!
#define calculateFace(N)                scalingFactor = 1 / norm2(N)*dotProduct(N, s1);\
                                        v[0] = N[0] * scalingFactor;\
                                        v[1] = N[1] * scalingFactor;\
                                        v[2] = N[2] * scalingFactor;

#define edgeTestPrep(x, edge, point)    calculateEdge(x, edge);\
                                        tempVect[0] = v[0] - point[0];\
                                        tempVect[1] = v[1] - point[1];\
                                        tempVect[2] = v[2] - point[2];
#define edgeTest()                      dotProduct(v, tempVect) > 0

#define S3Dregion123()                  calculateFace(N4);\
                                        s->nvrtx = 3;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

#define S3Dregion124()                  calculateFace(N3);\
                                        s->nvrtx = 3;\
                                        s->vrtx[1][0] = s1[0];\
                                        s->vrtx[1][1] = s1[1];\
                                        s->vrtx[1][2] = s1[2];

#define S3Dregion134()                  calculateFace(N2);\
                                        s->nvrtx = 3;\
                                        s->vrtx[2][0] = s1[0];\
                                        s->vrtx[2][1] = s1[1];\
                                        s->vrtx[2][2] = s1[2];

#define S3Dregion1234()                 v[0] = 0;\
                                        v[1] = 0;\
                                        v[2] = 0;\
                                        s->nvrtx = 4;\

#endif
#define calculateEdge(x, edge)          scalingFactor = -x/norm2(edge);\
                                        v[0] = s1[0] + scalingFactor * edge[0];\
                                        v[1] = s1[1] + scalingFactor * edge[1];\
                                        v[2] = s1[2] + scalingFactor * edge[2];

#define calculateNormalVector(N, x, y)  N[0] = x[1]*y[2] - x[2]*y[1];\
                                        N[1] = x[2]*y[0] - x[0]*y[2];\
                                        N[2] = x[0]*y[1] - x[1]*y[0];


#define download(point, location)       point[0] = s->vrtx[location][0];\
                                        point[1] = s->vrtx[location][1];\
                                        point[2] = s->vrtx[location][2];

#define calculateEdgeVector(p1p2, p2)   p1p2[0] = p2[0] - s1[0];\
                                        p1p2[1] = p2[1] - s1[1];\
                                        p1p2[2] = p2[2] - s1[2];

#define S1Dregion1()                    v[0] = s1[0];\
                                        v[1] = s1[1];\
                                        v[2] = s1[2];\
                                        s->nvrtx = 1;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

#define S1Dregion12()                   calculateEdge(x2, s1s2);

#define S2Dregion1()                    v[0] = s1[0];\
                                        v[1] = s1[1];\
                                        v[2] = s1[2];\
                                        s->nvrtx = 1;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

#define S2Dregion12()                   s->nvrtx = 2;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

#define S2Dregion13()                   s->nvrtx = 2;\
                                        s->vrtx[1][0] = s1[0];\
                                        s->vrtx[1][1] = s1[1];\
                                        s->vrtx[1][2] = s1[2];

#define S2Dregion123()                  calculateNormalVector(N, s1s2, s1s3);\
                                        calculateFace(N);

#define S3Dregion1()                    v[0] = s1[0];\
                                        v[1] = s1[1];\
                                        v[2] = s1[2];\
                                        s->nvrtx = 1;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

#define S3Dregion12()                   s->nvrtx = 2;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];\
                                        s->vrtx[1][0] = s2[0];\
                                        s->vrtx[1][1] = s2[1];\
                                        s->vrtx[1][2] = s2[2];

#define S3Dregion13()                   s->nvrtx = 2;\
                                        s->vrtx[0][0] = s1[0];\
                                        s->vrtx[0][1] = s1[1];\
                                        s->vrtx[0][2] = s1[2];

#define S3Dregion14()                   s->nvrtx = 2;\
                                        s->vrtx[1][0] = s1[0];\
                                        s->vrtx[1][1] = s1[1];\
                                        s->vrtx[1][2] = s1[2];


/**
* @brief S1D
*/


#ifdef NEWMAT
inline static void S1D(struct simplex * s, double *v)
{
  double s1[3], s2[3], s1s2[3] ;

  download(s1, 1);
  download(s2, 0);
  calculateEdgeVector(s1s2, s2);

  if (hff1(s1, s2)) {
    projectOnLine(s1, s2, v); // Update v, no need to update s
    return; // Return V{1,2}
  }
  else {
    S1Dregion1(); // Update v and s
    return;  // Return V{1}
  }
}

#else
inline static void S1D( struct simplex * s, double *v ) {

  double s1[3], s2[3], s1s2[3], x2, scalingFactor;

  download(s1, 1);
  download(s2, 0);
  calculateEdgeVector(s1s2, s2);

  x2 = dotProduct(s1, s1s2);

  if (x2 < 0) {
    S1Dregion12();
  }
  else {
    S1Dregion1();
  }
}
#endif


/**
* @brief S2D
*/
#ifdef NEWMAT
inline static void S2D(struct simplex * s, double *v)
{
  double s1[3], s2[3], s3[3];

  download(s1, 2);
  download(s2, 1);
  download(s3, 0);

  int    hff1f_s12 = hff1(s1, s2);
  int    hff1f_s13 = hff1(s1, s3);

  if (hff1f_s12){
    if (hff2(s1, s2, s3)){
      projectOnLine(s1, s2, v); // Update v
      S2Dregion12(); // Update s
      return; // Return V{1,2}
    }
  }

  if (hff1f_s13) {
    if (hff2(s1, s3, s2)) {
      projectOnLine(s1, s3, v); // Update v
      S2Dregion13(); // Update s
      return ; // Return V{1,3}
    }
  }

  if ((hff1f_s12 + hff1f_s13) > 0) {
    projectOnPlane(s1, s2, s3, v); // Update s, no need to update c
    return; // Return V{1,2,3}
  }

  S2Dregion1(); // Update s and v
  return; // Return V{1}
}

#else
inline static void S2D( struct simplex * s, double *v ) {

  double s1[3], s2[3], s3[3], s1s2[3], s1s3[3], x2, x3, scalingFactor, N[3], tempVect[3];
  int testTwo, testThree; 

  download(s1, 2);
  download(s2, 1);
  download(s3, 0);
  calculateEdgeVector(s1s2, s2);
  calculateEdgeVector(s1s3, s3);

  x2 = dotProduct(s1, s1s2);
  x3 = dotProduct(s1, s1s3);
  testTwo = x2 < 0;
  testThree = x3 < 0;

  if (testTwo) {

    edgeTestPrep(x2, s1s2, s3);
    if (edgeTest()) {

      if (testThree) {
        edgeTestPrep(x3, s1s3, s2);
        if (edgeTest()) {
          S2Dregion123()
        }
        else {
          S2Dregion13(); 
        }
      }
      else {
        S2Dregion123();
      }

    }
    else {

      S2Dregion12(); 

    }

  }
  else if (testThree) {

    edgeTestPrep(x3, s1s3, s2);
    if (edgeTest()) {

      S2Dregion123();

    }
    else {

      S2Dregion13(); 

    }

  }
  else {

    S2Dregion1();

  }

}
#endif
/**
* @brief S3D
*/

#ifdef NEWMAT
inline static void S3D( struct simplex * s, double *v ) {

  double s1[3], s2[3], s3[3], s4[3], s1s2[3], s1s3[3], s1s4[3], N2[3], N3[3], N4[3], tempVect[3];
  double si[3], sj[3], sk[3];
  double scalingFactor, x2, x3, x4, y2, y3, y4;
  int testLineTwo, testLineThree, testLineFour, testPlaneTwo, testPlaneThree, testPlaneFour, dotTotal, planeTotal;
  int i, j, k, t;
  double edg13[3], Ntmp[3];

  download(s1, 3);
  download(s2, 2);
  download(s3, 1);
  download(s4, 0);
  calculateEdgeVector(s1s2, s2);
  calculateEdgeVector(s1s3, s3);
  calculateEdgeVector(s1s4, s4);

  //x2 = dotProduct(s1, s1s2);
  //x3 = dotProduct(s1, s1s3);
  //x4 = dotProduct(s1, s1s4);
  int hff1_tests[3];
  hff1_tests[2] = hff1(s1, s2);
  hff1_tests[1] = hff1(s1, s3);
  hff1_tests[0] = hff1(s1, s4);
  testLineTwo = hff1(s1, s2);
  testLineThree = hff1(s1, s3);
  testLineFour = hff1(s1, s4);

  dotTotal = testLineTwo+testLineThree+testLineFour;
  if (dotTotal == 0) { /* case 0.0 -------------------------------------- */
    S3Dregion1();
    return;
  }

   double det134 = determinant(s1s3, s1s4, s1s2);
   int sss = -1;
   if (det134>0){
     //printf("\n\ns3d    tet is positive %.4f", det134);
     sss = 0;
   }
   else{
     //printf("\n\ns3d    tet is negative %.4f", det134);
     sss = 1;
   }
  //double  ttn[3];
  //crossProduct(s1s3, s1s4, ttn);
  //double det134 = norm2(ttn);
  //crossProduct(s1s4, s1s3, ttn);
  //double det142 = norm2(ttn);
  //crossProduct(s1s2, s1s3, ttn);
  //double det123 = norm2(ttn);

 /* double det134 = determinant(s1, s3, s4);
  double det142 = determinant(s1, s4, s2);
  double det123 = determinant(s1, s2, s3);*/

  //if (differentSign(det134, det142) || differentSign(det134, det123) || differentSign(det123, det142) )
  //  printf("\n\ns3d    triangles have different sign: %.4f %.4f %.4f", det134, det142, det123);
  //else
  //  printf("\n\ns3d    triangles have SAME sign: %.4f %.4f %.4f", det134, det142, det123);

  testPlaneTwo =   hff3(s1, s3, s4, s2) - sss;
  testPlaneTwo = testPlaneTwo * testPlaneTwo;
  testPlaneThree = hff3(s1, s4, s2, s3) - sss;
  testPlaneThree = testPlaneThree * testPlaneThree;
  testPlaneFour =  hff3(s1, s2, s3, s4) - sss;
  testPlaneFour = testPlaneFour * testPlaneFour;

  // IF NEGATIVE MEANS THAT FACING ORIGIN
  planeTotal = testPlaneTwo  + testPlaneThree  + testPlaneFour ;

  switch (planeTotal) {

    case 3:
      /* case 3.0 -------------------------------------- */
      S3Dregion1234();
      break;

    case 2:
      // Only one facing the oring
      // 1,i,j, are the indices of the points on the triangle and remove k from simplex
      s->nvrtx = 3;
      if (!testPlaneTwo) { // k = 2;   removes s2
        for (i = 0; i < 3; i++)
          s->vrtx[2][i] = s1[i];
      }
      else if (!testPlaneThree) {// k = 1; // removes s3
        for (i = 0; i < 3; i++)
          s->vrtx[1][i] = s2[i];
        for (i = 0; i < 3; i++)
          s->vrtx[2][i] = s1[i];
      }
      else if (!testPlaneFour) { // k = 0; // removes s4  and no need to reorder
        for (i = 0; i < 3; i++)
          s->vrtx[0][i] = s3[i];
        for (i = 0; i < 3; i++)
          s->vrtx[1][i] = s2[i];
        for (i = 0; i < 3; i++)
          s->vrtx[2][i] = s1[i];
      }
      // Call S2D
      S2D(s, v);
      break;
    case 1: 
      // Two triangles face the origins:
      //    The only positive hff3 is for triangle 1,i,j, therefore k must be in the solution as it supports the the point of minimum norm.

      // 1,i,j, are the indices of the points on the triangle and remove k from simplex
      s->nvrtx = 3;
      if (testPlaneTwo) {
        k = 2; // s2
        i = 1; 
        j = 0;
      }
      else if (testPlaneThree) {
        k = 1; // s3 
        i = 0;
        j = 2;
      }
      else if (testPlaneFour) {
        k = 0; // s4
        i = 2;
        j = 1;
      }

      download(si, i);
      download(sj, j);
      download(sk, k);

      if (dotTotal == 1) {
        if (hff1_tests[k]) {
          if (!hff2(s1, sk, si)) {
            select_1ik();
            projectOnPlane(s1, si, sk, v);
          }
          else if (!hff2(s1,sk,sj)) {
            select_1jk();
            projectOnPlane(s1, sj, sk, v);
          }
          else {
            select_1k(); // select region 1i 
            projectOnLine(s1, sk, v);
          }
        }
        else if (hff1_tests[i]) {
          if (hff2(s1,si,sk)) {
            select_1ik();
            projectOnPlane(s1, si, sk, v);
          }
          else {
            select_1i(); // select region 1i 
            projectOnLine(s1, si, v);
          }
        }
        else {
          if (hff2(s1, sj, sk)) {
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
        // Two edges have positive hff1, meaning that for two edges the origin's project fall on the segement. 
        //  Certainly the edge 1,k supports the the point of minimum norm, and so hff1_1k is positive

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
        else if (hff1_tests[j] ) {//  there is no other choice
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
        // sk is s.t. hff3 for sk < 0. So, sk must support the origin because there are 2 triangles facing the origin.
        if (!hff2(s1, sk, si)) {
          select_1ik();
          projectOnPlane(s1, si, sk, v);
        }
        else if (!hff2(s1, sk, sj)) {
          select_1jk();
          projectOnPlane(s1, sk, sj, v);
        } 
        else {
          select_1k();
          projectOnLine(s1, sk, v);
        }
      }
      break;

    case 0:
      // The origin is outside all 3 triangles 
      if (dotTotal == 1) {
        // Here si is set such that hff(s1,si) > 0 
        if (testLineThree) {
          k = 2;
          i = 1; // s3
          j = 0;
        }
        else if (testLineFour) {
          k = 1; // s3 
          i = 0;
          j = 2;
        }
        else if (testLineTwo) {
          k = 0;
          i = 2; // s2
          j = 1;
        }
        download(si, i);
        download(sj, j);
        download(sk, k);

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
          k = 2;
          i = 1; // s3
          j = 0;
        }
        else if (!testLineFour) {
          k = 1; 
          i = 0; // s4
          j = 2;
        }
        else if (!testLineTwo) {
          k = 0;
          i = 2; // s2
          j = 1;
        }
        download(si, i);
        download(sj, j);
        download(sk, k);        

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
            select_1j();
            projectOnLine(s1, sj, v);
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
      else {
        printf("\n\n\n\n\n  ERROR S3D case 0 unhandled case");
      }
      break;
  }

}
#else

inline static void S3D(struct simplex * s, double *v) {

  double s1[3], s2[3], s3[3], s4[3], s1s2[3], s1s3[3], s1s4[3], N2[3], N3[3], N4[3], tempVect[3], scalingFactor, x2, x3, x4, y2, y3, y4;
  int testLineTwo, testLineThree, testLineFour, testPlaneTwo, testPlaneThree, testPlaneFour, dotTotal, planeTotal;

  download(s1, 3);
  download(s2, 2);
  download(s3, 1);
  download(s4, 0);
  calculateEdgeVector(s1s2, s2);
  calculateEdgeVector(s1s3, s3);
  calculateEdgeVector(s1s4, s4);

  x2 = dotProduct(s1, s1s2);
  x3 = dotProduct(s1, s1s3);
  x4 = dotProduct(s1, s1s4);
  testLineTwo = x2 < 0;
  testLineThree = x3 < 0;
  testLineFour = x4 < 0;

  dotTotal = testLineTwo + testLineThree + testLineFour;
  if (dotTotal == 0) {
    S3Dregion1();
    return;
  }

  calculateNormalVector(N2, s1s3, s1s4);
  calculateNormalVector(N3, s1s2, s1s4);
  calculateNormalVector(N4, s1s2, s1s3);

  testPlaneTwo = differentSign(dotProduct(N2, s1), dotProduct(N2, s1s2));
  testPlaneThree = differentSign(dotProduct(N3, s1), dotProduct(N3, s1s3));
  testPlaneFour = differentSign(dotProduct(N4, s1), dotProduct(N4, s1s4));

  planeTotal = testPlaneTwo + testPlaneThree + testPlaneFour;

  switch (planeTotal) {

  case 3:

    S3Dregion1234();
    break;

  case 2:

    if (!testPlaneTwo) {

      if (testLineThree) {
        edgeTestPrep(x3, s1s3, s4);
        if (edgeTest()) {
          if (testLineFour) {
            edgeTestPrep(x4, s1s4, s3);
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion14();
            }
          }
          else {
            S3Dregion134();
          }
        }
        else {
          S3Dregion13();
        }
      }
      else if (testLineFour) {
        edgeTestPrep(x4, s1s4, s3);
        if (edgeTest()) {
          S3Dregion134();
        }
        else {
          S3Dregion14();
        }
      }

    }
    else if (!testPlaneThree) {

      if (testLineTwo) {
        edgeTestPrep(x2, s1s2, s4);
        if (edgeTest()) {
          if (testLineFour) {
            edgeTestPrep(x4, s1s4, s2);
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion14();
            }
          }
          else {
            S3Dregion124();
          }
        }
        else {
          S3Dregion12();
        }
      }
      else if (testLineFour) {
        edgeTestPrep(x4, s1s4, s2);
        if (edgeTest()) {
          S3Dregion124();
        }
        else {
          S3Dregion14();
        }
      }

    }
    else if (!testPlaneFour) {

      if (testLineTwo) {
        edgeTestPrep(x2, s1s2, s3);
        if (edgeTest()) {
          if (testLineThree) {
            edgeTestPrep(x3, s1s3, s2);
            if (edgeTest()) {
              S3Dregion123();
            }
            else {
              S3Dregion13();
            }
          }
          else {
            S3Dregion123();
          }
        }
        else {
          S3Dregion12();
        }
      }
      else if (testLineThree) {
        edgeTestPrep(x3, s1s3, s2);
        if (edgeTest()) {
          S3Dregion123();
        }
        else {
          S3Dregion13();
        }
      }

    }
    break;

  case 1:

    if (testPlaneTwo) {

      if (dotTotal == 1) {

        if (testLineTwo) {

          calculateEdge(x2, s1s2);
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            tempVect[0] = v[0] - s4[0];
            tempVect[1] = v[1] - s4[1];
            tempVect[2] = v[2] - s4[2];
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion12();
            }
          }

        }
        else if (testLineThree) {

          edgeTestPrep(x3, s1s3, s2);
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            S3Dregion13();
          }

        }
        else if (testLineFour) {

          edgeTestPrep(x4, s1s4, s2);
          if (edgeTest()) {
            S3Dregion124();
          }
          else {
            S3Dregion14();
          }

        }

      }
      else if (dotTotal == 2) {

        if (testLineThree) {

          calculateEdge(x2, s1s2);
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x3, s1s3, s2);
            if (edgeTest()) {
              S3Dregion123();
            }
            else {
              S3Dregion13()
            }
          }
          else {
            tempVect[0] = v[0] - s4[0];
            tempVect[1] = v[1] - s4[1];
            tempVect[2] = v[2] - s4[2];
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion12();
            }
          }

        }
        else if (testLineFour) {

          calculateEdge(x2, s1s2);
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x4, s1s4, s2);
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion14()
            }
          }
          else {
            tempVect[0] = v[0] - s3[0];
            tempVect[1] = v[1] - s3[1];
            tempVect[2] = v[2] - s3[2];
            if (edgeTest()) {
              S3Dregion123();
            }
            else {
              S3Dregion12();
            }
          }

        }

      }
      else if (dotTotal == 3) {

        calculateEdge(x2, s1s2);
        tempVect[0] = v[0] - s3[0];
        tempVect[1] = v[1] - s3[1];
        tempVect[2] = v[2] - s3[2];
        if (dotProduct(v, tempVect) > 0) {
          edgeTestPrep(x3, s1s3, s2);
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            S3Dregion13()
          }
        }
        else {
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x4, s1s4, s2);
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion14()
            }
          }
          else {
            S3Dregion12();
          }
        }

      }

    }
    else if (testPlaneThree) {

      if (dotTotal == 1) {

        if (testLineThree) {

          calculateEdge(x3, s1s3);
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            tempVect[0] = v[0] - s4[0];
            tempVect[1] = v[1] - s4[1];
            tempVect[2] = v[2] - s4[2];
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion13();
#ifdef NEWEdgePrep
              calculateEdge(x3, s1s3);
#endif
            }
          }

        }
        else if (testLineTwo) {

          edgeTestPrep(x2, s1s2, s3);
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            S3Dregion12();
#ifdef NEWEdgePrep
            calculateEdge(x2, s1s2);
#endif
          }

        }
        else if (testLineFour) {

          edgeTestPrep(x4, s1s4, s3);
          if (edgeTest()) {
            S3Dregion134();
          }
          else {
            S3Dregion14();
#ifdef NEWEdgePrep
            calculateEdge(x4, s1s4);
#endif
          }

        }

      }
      else if (dotTotal == 2) {

        if (testLineTwo) {

          calculateEdge(x3, s1s3);
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x2, s1s2, s3);
            if (edgeTest()) {
              S3Dregion123();
            }
            else {
              S3Dregion12();
#ifdef NEWEdgePrep
              calculateEdge(x2, s1s2);
#endif
            }
          }
          else {
            tempVect[0] = v[0] - s4[0];
            tempVect[1] = v[1] - s4[1];
            tempVect[2] = v[2] - s4[2];
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion13();
#ifdef NEWEdgePrep
              calculateEdge(x3, s1s3);
#endif
            }
          }

        }
        else if (testLineFour) {

          calculateEdge(x3, s1s3);
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x4, s1s4, s3);
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion14();
#ifdef NEWEdgePrep
              calculateEdge(x4, s1s4);
#endif
            }
          }
          else {
            tempVect[0] = v[0] - s2[0];
            tempVect[1] = v[1] - s2[1];
            tempVect[2] = v[2] - s2[2];
            if (edgeTest()) {
              S3Dregion123();
            }
            else {
              S3Dregion13();
#ifdef NEWEdgePrep
              calculateEdge(x3, s1s3);
#endif
            }
          }

        }

      }
      else if (dotTotal == 3) {

        calculateEdge(x3, s1s3);
        tempVect[0] = v[0] - s2[0];
        tempVect[1] = v[1] - s2[1];
        tempVect[2] = v[2] - s2[2];
        if (dotProduct(v, tempVect) > 0) {
          edgeTestPrep(x2, s1s2, s3);
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            S3Dregion12()
#ifdef NEWEdgePrep
              calculateEdge(x2, s1s2);
#endif
          }
        }
        else {
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x4, s1s4, s3);
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion14()
#ifdef NEWEdgePrep
                calculateEdge(x4, s1s4);
#endif
            }
          }
          else {
            S3Dregion13();
#ifdef NEWEdgePrep
            calculateEdge(x3, s1s3);
#endif
          }
        }

      }

    }
    else if (testPlaneFour) {

      if (dotTotal == 1) {

        if (testLineFour) {

          calculateEdge(x4, s1s4);
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (dotProduct(v, tempVect) > 0) {
            S3Dregion124();
          }
          else {
            tempVect[0] = v[0] - s3[0];
            tempVect[1] = v[1] - s3[1];
            tempVect[2] = v[2] - s3[2];
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion14();
#ifdef NEWEdgePrep
              calculateEdge(x4, s1s4);
#endif
            }
          }

        }
        else if (testLineTwo) {

          edgeTestPrep(x2, s1s2, s4);
          if (edgeTest()) {
            S3Dregion124();
          }
          else {
            S3Dregion12();
#ifdef NEWEdgePrep
            calculateEdge(x2, s1s2);
#endif
          }

        }
        else if (testLineThree) {

          edgeTestPrep(x3, s1s3, s4);
          if (edgeTest()) {
            S3Dregion134();
          }
          else {
            S3Dregion14();
#ifdef NEWEdgePrep
            calculateEdge(x4, s1s4);
#endif
          }

        }

      }
      else if (dotTotal == 2) {

        if (testLineTwo) {

          calculateEdge(x4, s1s4);
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x2, s1s2, s4);
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion12()
#ifdef NEWEdgePrep
                calculateEdge(x2, s1s2);
#endif
            }
          }
          else {
            tempVect[0] = v[0] - s3[0];
            tempVect[1] = v[1] - s3[1];
            tempVect[2] = v[2] - s3[2];
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion14();
#ifdef NEWEdgePrep
              calculateEdge(x4, s1s4);
#endif
            }
          }

        }
        else if (testLineThree) {

          calculateEdge(x4, s1s4);
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x3, s1s3, s4);
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion13()
#ifdef NEWEdgePrep
                calculateEdge(x3, s1s3);
#endif
            }
          }
          else {
            tempVect[0] = v[0] - s2[0];
            tempVect[1] = v[1] - s2[1];
            tempVect[2] = v[2] - s2[2];
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion14();
#ifdef NEWEdgePrep
              calculateEdge(x4, s1s4);
#endif
            }
          }

        }

      }
      else if (dotTotal == 3) {

        calculateEdge(x4, s1s4);
        tempVect[0] = v[0] - s2[0];
        tempVect[1] = v[1] - s2[1];
        tempVect[2] = v[2] - s2[2];
        if (dotProduct(v, tempVect) > 0) {
          edgeTestPrep(x2, s1s2, s4);
          if (edgeTest()) {
            S3Dregion124();
          }
          else {
            S3Dregion12()
#ifdef NEWEdgePrep
              calculateEdge(x2, s1s2);
#endif
          }
        }
        else {
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (dotProduct(v, tempVect) > 0) {
            edgeTestPrep(x3, s1s3, s4);
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion13()
#ifdef NEWEdgePrep
                calculateEdge(x3, s1s3);
#endif
            }
          }
          else {
            S3Dregion14();
#ifdef NEWEdgePrep
            calculateEdge(x4, s1s4);
#endif
          }
        }

      }

    }
    break;

  case 0:

    if (dotTotal == 1) {

      if (testLineTwo) {

        calculateEdge(x2, s1s2);
        tempVect[0] = v[0] - s3[0];
        tempVect[1] = v[1] - s3[1];
        tempVect[2] = v[2] - s3[2];
        if (edgeTest()) {
          S3Dregion123();
        }
        else {
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (edgeTest()) {
            S3Dregion124();
          }
          else {
            S3Dregion12();
#ifdef NEWEdgePrep
            calculateEdge(x2, s1s2);
#endif
          }
        }

      }
      else if (testLineThree) {

        calculateEdge(x3, s1s3);
        tempVect[0] = v[0] - s2[0];
        tempVect[1] = v[1] - s2[1];
        tempVect[2] = v[2] - s2[2];
        if (edgeTest()) {
          S3Dregion123();
        }
        else {
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (edgeTest()) {
            S3Dregion134();
          }
          else {
            S3Dregion13();
#ifdef NEWEdgePrep
            calculateEdge(x3, s1s3);
#endif
          }
        }

      }
      else if (testLineFour) {

        calculateEdge(x4, s1s4);
        tempVect[0] = v[0] - s2[0];
        tempVect[1] = v[1] - s2[1];
        tempVect[2] = v[2] - s2[2];
        if (edgeTest()) {
          S3Dregion124();
        }
        else {
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (edgeTest()) {
            S3Dregion134();
          }
          else {
            S3Dregion14();
#ifdef NEWEdgePrep
            calculateEdge(x4, s1s4);
#endif
          }
        }

      }

    }
    else if (dotTotal == 2) {

      if (!testLineTwo) {

        calculateEdge(x3, s1s3);
        tempVect[0] = v[0] - s4[0];
        tempVect[1] = v[1] - s4[1];
        tempVect[2] = v[2] - s4[2];
        if (dotProduct(v, tempVect) > 0) {
          calculateEdge(x4, s1s4);
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (edgeTest()) {
            S3Dregion134();
          }
          else {
            tempVect[0] = v[0] - s2[0];
            tempVect[1] = v[1] - s2[1];
            tempVect[2] = v[2] - s2[2];
            if (edgeTest()) {
              S3Dregion124();
            }
            else {
              S3Dregion14();
#ifdef NEWEdgePrep
              calculateEdge(x4, s1s4);
#endif
            }
          }
        }
        else {
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            S3Dregion13();
#ifdef NEWEdgePrep
            calculateEdge(x3, s1s3);
#endif
          }
        }

      }
      else if (!testLineThree) {

        calculateEdge(x2, s1s2);
        tempVect[0] = v[0] - s4[0];
        tempVect[1] = v[1] - s4[1];
        tempVect[2] = v[2] - s4[2];
        if (dotProduct(v, tempVect) > 0) {
          calculateEdge(x4, s1s4);
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (edgeTest()) {
            S3Dregion124();
          }
          else {
            tempVect[0] = v[0] - s3[0];
            tempVect[1] = v[1] - s3[1];
            tempVect[2] = v[2] - s3[2];
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion14();
#ifdef NEWEdgePrep
              calculateEdge(x4, s1s4);
#endif
            }
          }
        }
        else {
          tempVect[0] = v[0] - s3[0];
          tempVect[1] = v[1] - s3[1];
          tempVect[2] = v[2] - s3[2];
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            S3Dregion12();
#ifdef NEWEdgePrep
            calculateEdge(x2, s1s2);
#endif
          }
        }

      }
      else if (!testLineFour) {

        calculateEdge(x2, s1s2);
        tempVect[0] = v[0] - s3[0];
        tempVect[1] = v[1] - s3[1];
        tempVect[2] = v[2] - s3[2];
        if (edgeTest()) {
          calculateEdge(x3, s1s3);
          tempVect[0] = v[0] - s2[0];
          tempVect[1] = v[1] - s2[1];
          tempVect[2] = v[2] - s2[2];
          if (edgeTest()) {
            S3Dregion123();
          }
          else {
            tempVect[0] = v[0] - s4[0];
            tempVect[1] = v[1] - s4[1];
            tempVect[2] = v[2] - s4[2];
            if (edgeTest()) {
              S3Dregion134();
            }
            else {
              S3Dregion13();
#ifdef NEWEdgePrep
              calculateEdge(x3, s1s3);
#endif
            }
          }
        }
        else {
          tempVect[0] = v[0] - s4[0];
          tempVect[1] = v[1] - s4[1];
          tempVect[2] = v[2] - s4[2];
          if (edgeTest()) {
            S3Dregion124();
          }
          else {
            S3Dregion12();
#ifdef NEWEdgePrep
            calculateEdge(x2, s1s2);
#endif
          }
        }

      }

    }
    break;

  }

}
#endif
#else

#define SAMESIGN( a, b ) ( (a>0) == (b>0) )

inline static void S1D(struct simplex * s, double *vv) {

  double a[3],
      b[3],
      t[3],
      nu_fabs[3],
      det_ap = 0,
      det_pb = 0,
      pt = 0,
      leng = 0;

  int    i = 0,
      indexI = 1,
      FacetsTest[2];

  for (i = 0; i < 3; ++i) {
    b[i] = s->vrtx[0][i];
    a[i] = s->vrtx[1][i];
    t[i] = b[i] - a[i];
    leng += t[i];
    nu_fabs[i] = fabs(t[i]);
  }

  if (nu_fabs[0] > nu_fabs[1]) {
    if (nu_fabs[0] > nu_fabs[2]) {
      indexI = 0;
    }
    else {
      indexI = 2;
    }
  }
  else if (nu_fabs[0] < nu_fabs[1]) {
    if (nu_fabs[1] > nu_fabs[2]) {
      indexI = 1;
    }
    else {
      indexI = 2;
    }
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

inline static void S2D(struct simplex * s, double *vv) {
  double    a[3],
      b[3],
      c[3],
      s21[3],
      s31[3],
      nu_test[3],
      nu_fabs[3],
      B[3],
      n[3],
      v[3],
      vtmp[3],
      pp[3 - 1],
      sa[3 - 1],
      sb[3 - 1],
      sc[3 - 1],
      nu_max = 0,
      inv_detM = 0,
      nnorm_sqrd = 0,
      nnnorm = 0,
      dotNA;

  int       i = 0,
      FacetsTest[3],
      k,
      l,
      j,
      indexI = 1,
      indexJ[2] = { -1 },
      stemp[3];

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
  for (int m = 0; m < 3; ++m) {
    /* i = 0 -> OBC
     * i = 1 -> AOC
     * i = 2 -> ABO */
    FacetsTest[m] = SAMESIGN(nu_max, B[m]);
  }

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
    /* ... test segment AC */
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

inline static void S3D(struct simplex * s,  double *vv) {
  // vv[0] = vv[0];    ? delete this?
  double   a[3],
      b[3],
      c[3],
      d[3],
      vtmp[3],
      v[3],
      B[4],
      sqdist_tmp = 0,
      detM,
      inv_detM,
      tmplamda[4] = { 0, 0, 0, 0 },
      tmpscoo1[4][3] = { 0 };
#ifdef ADAPTIVEFP
  double  o[3] = { 0 };
#endif
  int      i,
      j,
      k,
      l,
      Flag_sAuxused = 0,
      firstaux = 0,
      secondaux = 0,
      FacetsTest[4] = { 1,1,1,1 },
      sID[4] = { 0, 0, 0, 0 },
      nclosestS = 0,
      TrianglesToTest[9] = { 3, 3, 3, 1, 2, 2, 0, 0, 1 },
      ndoubts = 0,
      vtxid;

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
    {
      FacetsTest[1] = 0; /* A = B. Test only ACD */
    }
    else if (fabs(B[1]) < eps && fabs(B[3]) < eps)
    {
      FacetsTest[2] = 0; /* A = C. Test only ABD */
    }
    else if (fabs(B[1]) < eps && fabs(B[2]) < eps)
    {
      FacetsTest[3] = 0; /* A = D. Test only ABC */
    }
    else if (fabs(B[0]) < eps && fabs(B[3]) < eps)
    {
      FacetsTest[1] = 0; /* B = C. Test only ACD */
    }
    else if (fabs(B[0]) < eps && fabs(B[2]) < eps)
    {
      FacetsTest[1] = 0; /* B = D. Test only ABD */
    }
    else if (fabs(B[0]) < eps && fabs(B[1]) < eps)
    {
      FacetsTest[2] = 0; /* C = D. Test only ABC */
    }
    else
    {
      for (i = 0; i < 4; i++)
      {
        FacetsTest[i] = 0; /* Any other case. Test ABC, ABD, ACD */
      }
    }

  }
  else
  {
    for (i = 0; i < 4; ++i) {
      FacetsTest[i] = SAMESIGN(detM, B[i]);
    }
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
      /* ... and call S2D itself */
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
      /* ... and call S2D itself */
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
        /* ... and call S2D itself */
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
      /* ... and call S2D itself */
      S2D(s, v);
      secondaux = 2;
    }
    /* Do a loop and compare current outcomes */
    /* ... computer aux squared distance */
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
      /* ... thus, remove the vertex B from the simplex. */
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
      /* ... thus, remove the vertex C from the simplex. */
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
    /* ... and call S2D itself */
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
#endif

inline static void support( struct bd *body, double *v ) {

  int i;
  double s;
  double maxs;
  double *vrt;
  int better = -1;

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

  int k = 0;                /**< Iteration counter            */
  int i;                    /**< General purpose counter      */
  int mk = 100;             /**< Maximum number of iterations of the GJK algorithm */
  double v[3];              /**< Search direction             */
  double vminus[3];         /**< Search direction * -1        */
  double w[3];              /**< Vertex on CSO boundary given by the difference of support functions on both bodies */
  double eps_rel = eps_rel22;   /**< Tolerance on relative        */
  double eps_rel2 = eps_rel * eps_rel  ;
  double eps_tot = eps_tot22;
  double dd = -1;           /**< Squared distance             */
  int exeedtol_rel = 0;     /**< Flag for 1st exit condition  */
  int nullV = 0;

#ifdef DEBUG
  mexPrintf ("Num points A = %i \n",bd1.numpoints );
  mexPrintf ("Num points B = %i \n",bd2.numpoints );
  for ( i = 0; i < bd1.numpoints; ++i) {
    for (int j = 0; j < 3; j++){
      mexPrintf ("%.4f ", bd1.coord[ i][j ]);
    }
    mexPrintf ("\n");
  }

  for ( i = 0; i < bd2.numpoints; ++i) {
    for (int j = 0; j < 3; j++){
      mexPrintf ("%.4f ", bd2.coord[ i][j ]);
    }
    mexPrintf ("\n");
  }
#endif

  /* Initialise search direction */
  v[0] = bd1.coord[0][0] - bd2.coord[0][0];
  v[1] = bd1.coord[0][1] - bd2.coord[0][1];
  v[2] = bd1.coord[0][2] - bd2.coord[0][2];

  /* Inialise simplex */
  s->nvrtx = 1;
  s->vrtx[0][0] = v[0];
  s->vrtx[0][1] = v[1];
  s->vrtx[0][2] = v[2];

  bd1.s[0] = bd1.coord[0][0];
  bd1.s[1] = bd1.coord[0][1];
  bd1.s[2] = bd1.coord[0][2];

  bd2.s[0] = bd2.coord[0][0];
  bd2.s[1] = bd2.coord[0][1];
  bd2.s[2] = bd2.coord[0][2];

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

    /* Test first exit condition (new point already in simplex/can't move further) */
    exeedtol_rel = (norm2(v) - dotProduct (v,w) ) <= eps_rel2 * norm2(v);
    if ( exeedtol_rel ){
      break;
    }

    nullV = norm2(v) < eps_rel2;
    if (nullV) {
      break;
    }

    /* Add new vertex to simplex */
    i = s->nvrtx;
    s->vrtx[i][0] = w[0];
    s->vrtx[i][1] = w[1];
    s->vrtx[i][2] = w[2];
    s->nvrtx++;

    /* Invoke distance sub-algorithm */
    subalgorithm ( s, v );

    /* Test */
    int absTestin;
    double norm2Wmax = 0;
    for (int jj = 0; jj < s->nvrtx; jj++)
    {
      double tesnorm = norm2(s->vrtx[jj]);
      if (tesnorm > norm2Wmax) {
        norm2Wmax = tesnorm;
      }
    }

    absTestin = (norm2(v) <= (eps_tot * eps_tot * norm2Wmax));
    if (absTestin)
    {
      break;
    }

  } while((s->nvrtx != 4) && (k != mk));


  if (k == mk) {

#if INCREMENTAL
    #ifdef NEWMAT
#ifdef FAST
#else
#endif
#else
#endif
#else
#endif
  }

  /* Compute and return distance */
  dd = sqrt(norm2(v));

  return dd;
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
