/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTSEEDSET_H
#define STRUCTSEEDSET_H

  struct SEEDSET
  {
    /// \brief Dimensionality
    int Dim;

    /// \brief Number of seeds
    int N;

    /// \brief Identifier of the distribution
    int Id;

    /// \brief size of the seed set (= bounding box of the domain)
    double **Size;

    /// \brief weight
    /// \details can be: custom or a statistics law or values
    char *weight;

    /// \brief Number used for the seed generation process.
    long Random;
    long RandomO;

    /// \brief Equation for lamella tessellations
    /// \details (equation of the lamella interfaces)
    ///          LamEq[i][1]*X1+LamEq[i][2]*X2+LamEq[i][3]*X3=LamEq[i][0]
    /// i = 1 to N
    double **LamEq;

    /// \brief Width for lamella tessellations
    /// \details width of the lamellae, i = 1 to N
    double *LamWidth;

    /// \brief Width id for lamella tessellations
    /// \details width id of the lamellae, i = 1 to N, starts at 1
    int *LamWidthId;

    // SEED DISTRIBUTION -------------------------------------------------

    // For seed i (i=1...N):
    /// Input seed coordinates ([0,2])
    double **SeedCoo0;

    /// Seed coordinates (same as SeedCoo0 if standard, periodized if
    //periodic)
    double **SeedCoo;

    /// Seed weights
    double *SeedWeight;

    /// Orientations (quaternion)
    double **SeedOri;
    /// Orientations (Rodrigues vector)
    double **SeedOriR;

    double *SeedOriWeight; // radian

    char **SeedOriDistrib;
    double *SeedOriTheta; // radian

    // Crystal symmetry
    char *crysym;

    // periodic
    int Nall;

    char *Type;			// standard or periodic...

    int *Periodic;		// [0...2] for x, y and z periodicity
    // (0: no, 1: yes)
    double *PeriodicDist;	// [0...2] periodicity size along x, y and z

    int *PerSeedMaster;		// master seed
    int **PerSeedShift;		// shift wrt master (j=0...3)

    int *PerSeedSlaveQty;	// number of slaves of a master
    int **PerSeedSlaveNb;	// slaves of a master [1...]
  };
  typedef struct SEEDSET SEEDSET;

#endif				/* STRUCTSEEDSET_H */

#ifdef __cplusplus
}
#endif
