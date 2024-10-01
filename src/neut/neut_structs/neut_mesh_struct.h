/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2024, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTMESH_H
#define STRUCTMESH_H

  struct MESH
  {
    char *Domain;

    // mesh properties (all elements are of the same type / dimension / order)

    char *EltType;		// type of elements (tri or quad)
    int Dimension;		// Dimension (0, 1, 2 or 3)
    int EltOrder;		// Elt order (1 or 2)

    // information on the elements and element sets

    int EltQty;			// total number of elements
    // arrays are indexed [1...EltQty]

    int **EltNodes;		// for each elt: nodes, indexed [0...]

    int ElsetQty;		// total number of elsets, including custom elsets
    // arrays are indexed [1...ElsetQty]

    int CustomElsetQty;		// total number of custom elsets

    int *ElsetId;		// if != NULL, actual ids of the elsets

    int **Elsets;		// for each elset:
    // [0]: total number of elts
    // [1...Elsets[][0]]: ids of the elts

    int *EltElset;		// for each elt: elset

    char **ElsetLabels;         // for each elset: label

    // information on the nodes

    int NodeQty;		// total number of nodes

    int **NodeElts;		// for each node:
    // [0]: total number of elts
    // [1...NodeElts[][0]]: ids of the elts

    int PartQty;                 // total number of partitions
    int **Parts;                 // partitions (same format as Elsets)
    int *EltPart;                // partition of an elt

    double **ElsetOri;           // orientations of the elsets [1...ElsetQty][0...3]
    char *ElsetOriDes;           // orientation descriptor

    double **EltOri;             // orientations of the elts [1...EltQty][0...3]
    char *EltOriDes;             // orientation descriptor

    int *ElsetGroup;             // groups of the elsets [1...ElsetQty][0...3]

    char *ElsetCrySym;           // crystal symmetry

    // pointer to SIM structure
    struct SIM *pSim;
    double **SimEltOri;          // optional, element orientations from sim

    int *EltBody;
  };
  typedef struct MESH MESH;

#endif				/* STRUCTMESH_H */

#ifdef __cplusplus
}
#endif
