/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef STRUCTTESS_H
#define STRUCTTESS_H

/// \brief Tessellation structure
  struct TESS
  {
    // MULTILEVEL TESSELLATION INFORMATION -------------------------------

    int Dim;			// dimension of the tessellation
    int PseudoDim;		// dimension the tessellation aims to
    // have (-1 by default, can be 2)
    double PseudoSize;		// if PseudoDim, size in the extruded
    // direction

    int Level;			// level of the tessellation
    // (1 for a regular tessellation)

    int TessId;			// identifier of the tessellation
    // (1 for a regular tessellation)

    char *Type;			// type of tessellation (standard/periodic)
    int *Periodic;		// [0...2] for x, y and z periodicity
    // (0: no, 1: yes)
    double *PeriodicDist;	// [0...2] periodicity size along x, y and z

    // CELL INFORMATION --------------------------------------------------

    int CellQty;		// value of option -n.
    int *CellId;		// identifiers of the cells
    double **CellOri;		// crystal orientations of the cells
    int *CellLamId;		// in the case of a lamellar
                                // tessellation, id of the lamella (width)
    int *CellModeId;		// in the case of a multimodal
                                // tessellation, id of the mode
    char *CellCrySym;		// crystal symmetry (triclinic, cubic or hexagonal)

    int *CellTrue;		// true level of the cell (0 if not true, 1
    // is true, 2 if all neighs are true>=1, 3 if all
    // neighs are true>=2, etc.

    int *CellBody;		// same than CellTrue, but for body.

    // SEED INFORMATION --------------------------------------------------

    int SeedQty;		// number of seeds (!= number of cells
    // for periodic)
    double **SeedCoo;		// SeedCoo[i][j] (j=0...2) are the 3 coordinates
    // of the center.
    double *SeedWeight;		// SeedWeight[i] is the Laguerre weight

    // VERTEX INFORMATION ------------------------------------------------

    int VerQty;			// number of vertices

    // For vertex i (i=1...VerQty):
    double **VerCoo;		// VerCoo[i][j] (j=0...2) are the 3 coordinates.

    int *VerEdgeQty;		// VerEdgeQty[i] is the quantity of parent edges.
    int **VerEdgeNb;		// VerEdge[i][j] (j=0...) are the parent edges.

    int **VerDom;		// VerDom[i][0] is the boundary type:
    //  0: ver = domain ver
    //  1: on a domain edge
    //  2: on a domain face
    // -1: not on domain
    // VerDom[i][1] is the id of the boundary entity (0 if [0]=-1)

    int *VerState;		// VerState[i] is the state of vertex i:
    // 0: unchanged, -1: deleted, >0: modified =
    // vertex weight (i.e. nb of vertices it
    // represents).

    // EDGE INFORMATION --------------------------------------------------

    int EdgeQty;		// number of edges

    // For edge i (i=1...EdgeQty):
    int **EdgeVerNb;		// VerNb[i][j] (j=0,1) are the numbers of the two
    // vertices.

    int *EdgeFaceQty;		// EdgeFaceQty[i] is the quantity of parent faces.
    int **EdgeFaceNb;		// EdgeFaceNb[i][j] (j=0...) are the numbers of the
    // parent faces.

    double *EdgeLength;		// EdgeLength[i] is the length.

    int *EdgeState;		// EdgeState[i]=0 means edge i still exists (=-1 else)

    int *EdgeDel;		// EdgeDel[i]=0 means edge i can be deleted;
    // -1 -> to large.

    int **EdgeDom;		// EdgeDom[i][0] is the boundary type:
    //  1: on a domain edge
    //  2: on a domain face
    // -1: not on domain
    // EdgeDom[i][1] is the id of the boundary entity

    // FACE INFORMATION --------------------------------------------------

    int FaceQty;		// number of faces

    // For face i (i=1...FaceQty):
    int **FacePoly;		// FacePoly[i][j] (j=0,1) are the numbers of the two
    // parent polys.

    double **FaceEq;		// FaceEq[i][j] (j=0...3) are the four equation
    // parameters:
    // FaceEq[i][1]*X1+FaceEq[i][2]*X2+FaceEq[i][3]*X3=FaceEq[i][0]

    int *FaceVerQty;		// FaceVerQty[i] is the quantity of face vertices
    int **FaceVerNb;		// FaceVerNb[i][j] (j=1...FaceVerQty[i]) are the
    // numbers of the vertices.

    int **FaceEdgeNb;		// FaceEdgeNb[i][j] (j=1...FaceVerQty[i]) are the
    // numbers of the edges.

    int **FaceEdgeOri;		// FaceEdgeOri[i][j] (j=1...FaceVerQty[i]) are the
    // orientations of the edges.

    int *FaceState;		// FaceState[i] = 0 means face i is unmodified,
    // >0 means face i is modified, == -1 means face

    int *FacePt;		// point for the interpolation of the face.
    // if facept>0, facept=nb of the face ver.
    // if facept=0, the barycenter is considered.
    // if facept=-1, undefined (e.g. for remeshing).
    // for a 2D tess, must be -1.

    double **FacePtCoo;		// coo of the pt used for interpolation.
    int **FaceDom;		// FaceDom[i][0] is the boundary type:
    //  2: on a domain face
    // -1: not on domain
    // FaceDom[i][1] is the id of the boundary entity

    // POLYHEDRON INFORMATION --------------------------------------------

    int PolyQty;		// number of polyhedra

    // For polyhedron i (i=1...PolyQty):

    int *PolyFaceQty;		// PolyFaceQty[i] is the quantity of faces.
    int **PolyFaceNb;		// PolyFaceNb[i][j] (j=1...PolyFaceQty[i]) are the
    // numbers of the faces.
    int **PolyFaceOri;		// PolyFaceOri[i][j] (j=1...PolyFaceQty[i]) are the
    // orientations of the faces: 1 if the face normal
    // is outgoing, -1 otherwise.

    int *PolyState;		// PolyState[i] = 0 means poly i is unmodified,
				// -1 means deleted

    // DOMAIN INFORMATION ------------------------------------------------
    char *DomType;

    int DomVerQty;
    double **DomVerCoo;
    char **DomVerLabel;
    int *DomVerEdgeQty;
    int **DomVerEdgeNb;		// 0 indexed
    int *DomTessVerNb;

    int DomEdgeQty;
    char **DomEdgeLabel;
    int **DomEdgeVerNb;		// 0 indexed
    int **DomEdgeFaceNb;	// 0 indexed
    int *DomTessEdgeQty;
    int **DomTessEdgeNb;	// 1 indexed

    int DomFaceQty;
    char **DomFaceLabel;
    double **DomFaceEq;
    int *DomFaceVerQty;
    int **DomFaceVerNb;		// 1 indexed
    int **DomFaceEdgeNb;	// 1 indexed
    int *DomTessFaceQty;
    int **DomTessFaceNb;	// 1 indexed

    // SCALE INFORMATION -------------------------------------------------

    int ScaleQty;		// number of scales
    int **ScaleCellId;		// id of the poly a cell
				// belongs to at a given scale
				// [cell][scale=0...ScaleQty]

    // PERIODICITY INFORMATION -------------------------------------------

    int PerSeedQty;		// number of periodic (slave) seeds
    int *PerSeedMaster;		// master seed
    int **PerSeedShift;		// shift wrt master (j=0...3)
    int **PerSeedSlave;		// PerSeedSlave[i][j] (j=0...26) are the
    // slave seeds

    int PerVerQty;		// number of periodic (slave) vers
    int *PerVerNb;		// ids of periodic (slave) vers [1...]
    int *PerVerMaster;		// master ver
    int **PerVerShift;		// shift wrt master (j=0...3)
    int *PerVerSlaveQty;	// number of slaves of a master
    int **PerVerSlaveNb;	// slaves of a master [1...]

    int PerEdgeQty;		// number of periodic (slave) edges
    int *PerEdgeNb;		// ids of periodic (slave) edges [1...]
    int *PerEdgeMaster;		// master edge
    int **PerEdgeShift;		// shift wrt master (j=0...3)
    int *PerEdgeOri;		// orientation of a slave wrt master
    int *PerEdgeSlaveQty;	// number of slaves of a master
    int **PerEdgeSlaveNb;	// slaves of a master [1...]

    int PerFaceQty;		// number of periodic (slave) faces
    int *PerFaceNb;		// ids of periodic (master or slave) faces [1...]
    int *PerFaceMaster;		// master face
    int **PerFaceShift;		// shift wrt master (j=0...3)
    int *PerFaceOri;		// orientation of a slave wrt master
    int *PerFaceSlaveNb;	// slave of a master
  };
  typedef struct TESS TESS;

#endif				/* STRUCTTESS_H */

#ifdef __cplusplus
}
#endif
