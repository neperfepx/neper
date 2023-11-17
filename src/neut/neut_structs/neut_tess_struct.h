/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2022, Romain Quey. */
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
    // GENERAL INFORMATION ---------------------------------------------

    // Dimension of the tessellation (2 or 3)
    int Dim;

    // Dimension the tessellation aims to have (-1 by default, can be 2)
    int PseudoDim;

    // If PseudoDim == 2, size in the 3rd direction
    double PseudoSize;

    // Level of the tessellation (1 for a regular tessellation)
    int Level;

    // Identifier of the tessellation (1 for a regular tessellation)
    int TessId;

    // Type of tessellation ("standard" or "periodic")
    char *Type;

    // Periodicity along x, y and z [0...2] (0: no, 1: yes)
    int *Periodic;

    // Periodicity distance along x, y and z [0...2]
    double *PeriodicDist;

    // CELL INFORMATION --------------------------------------------------

    // Number of cells, i.e. value of option -n.
    // Equal to FaceQty in 2D and PolyQty in 3D.
    int CellQty;

    // Identifiers of the cells [1...CellQty]
    // defined only if necessary
    int *CellId;

    // Crystal orientations of the cells, in quaternions
    // [1...CellQty][0...3]
    double **CellOri;

    double *CellWeight;

    // Crystal orientation spreads of the cells
    // [1...CellQty], NULL if uninitialized
    char **CellOriDistrib;

    // Orientation descriptor used in the tess file
    char *CellOriDes;

    // For a lamellar tessellation, lamella ids of the cells, in terms
    // of imposed width [1...CellQty] (starts at 1)
    // defined only if necessary
    int *CellLamId;

    // For a multimodal tessellation, mode ids of the cells, in terms of
    // imposed size [1...CellQty] (starts at 1)
    // set to 1 by default (no multimodal)
    int *CellModeId;

    // Group of the cells [1...CellQty]
    // defined only if necessary
    int *CellGroup;

    // Crystal symmetry of the cells ("triclinic", "cubic" or
    // "hexagonal").  Defined once for all cells, but this could be
    // improved
    char *CellCrySym;

    // "Body" levels of the cells. body>0 means that the cell does not
    // intersect the domain boundary.
    // 0: not body;
    // 1: body;
    // 2: all neighs are body >= 1;
    // 3: all neighs are body >= 2, etc.
    int **CellBody;
    int CellBodyQty;
    char **CellBodyExpr;

    // SEED INFORMATION --------------------------------------------------

    // Number of seeds.  Equal to CellQty for a standard tessellation
    // and to the grand-number of seeds for a periodic tessellation.
    int SeedQty;

    // Coordinates of the seeds [1...SeedQty][0...2]
    double **SeedCoo;

    // Laguerre weights of the seeds [1...SeedQty]
    double *SeedWeight;

    // VERTEX INFORMATION ------------------------------------------------

    // Number of vertices. In the case of a periodic tessellations, all
    // vertices are listed, master/slave relationships are defined in
    // PerVer* members.
    int VerQty;

    // Vertex coordinates [1...VerQty][0...2]
    double **VerCoo;

    // Vertex amount of edges [1...VerQty]
    int *VerEdgeQty;

    // Numbers of the vertex edges [1...VerQty][0...]
    int **VerEdgeNb;

    // Vertex domain information [1...VerQty][0...1]
    // [i][0] = -1: not on domain
    // [i][0] =  0: on domain vertex
    // [i][0] =  1: on domain edge
    // [i][0] =  2: on domain face
    // [i][1]: corresponding domain vertex / edge / face (w/a)
    int **VerDom;

    // Vertex state information [1...VerQty]
    // -1: deleted
    //  0: unchanged
    // >=1: modified, corresponds to the number of vertices the vertex
    // represents.
    // Note that state -1 occurs only **during** regularization.  It
    // will never happen outside the regularization function.
    int *VerState;

    // EDGE INFORMATION --------------------------------------------------

    // Number of edges. In the case of a periodic tessellations, all
    // edges are listed, master/slave relationships are defined in
    // PerEdge* members.
    int EdgeQty;

    // Vertices of the edges [1...EdgeQty][0...1]
    int **EdgeVerNb;

    // Number of faces of the edges [1...EdgeQty]
    int *EdgeFaceQty;

    // Numbers of the faces of the edges [1...EdgeQty][0...]
    int **EdgeFaceNb;

    // Lengths of the edges [1...EdgeQty]
    double *EdgeLength;

    // States of the edges [1...EdgeQty]
    // -1: deleted
    //  0: unchanged
    // Note that state -1 occurs only **during** regularization.  It
    // will never happen outside the regularization function.
    int *EdgeState;

    // Deletion information of the edges [1...VerQty]
    //  0: can be deleted
    // -1: cannot be deleted (too large)
    // Note that this is used only **during** regularization.
    int *EdgeDel;

    // Domains of the edges [1...EdgeQty][0...1]
    // [i][0] = -1: not on domain
    // [i][0] =  1: on domain edge
    // [i][0] =  2: on domain face
    // [i][1]: corresponding domain edge / face (w/a)
    int **EdgeDom;

    // FACE INFORMATION --------------------------------------------------

    // Number of faces. In the case of a periodic tessellations, all
    // faces are listed, master/slave relationships are defined in
    // PerFace* members.
    int FaceQty;

    // Polys of the faces [1...FaceQty][0...1].  In the standard
    // case, the ids of the two polys are recorded. In the case of a
    // boundary face, the id of the domain face (a negative number) is
    // recorded as second poly.  In the case of a periodic face,
    // the id of the slave seed is recorded as second poly.
    int **FacePoly;

    // Equation parameters of the faces [1...FaceQty][0...3]:
    // FaceEq[i][1]*X1+FaceEq[i][2]*X2+FaceEq[i][3]*X3=FaceEq[i][0]
    double **FaceEq;

    // Number of vertices of the faces [1...FaceQty]
    int *FaceVerQty;

    // Numbers of the vertices of the faces [1...FaceQty][1...FaceVerQty[i]]
    int **FaceVerNb;

    // Numbers of the edges of the faces [1...FaceQty][1...FaceVerQty[i]]
    int **FaceEdgeNb;

    // Orientations of the edges of the faces [1...FaceQty][1...FaceVerQty[i]],
    // -1 or 1
    int **FaceEdgeOri;

    // States of the faces [1...FaceQty]
    // -1: deleted
    //  0: unchanged
    //  1: modified
    // Note that state = -1 occurs only **during** regularization.  It
    // will never happen outside the net_reg function.
    int *FaceState;

    // Points used to interpolate the faces:
    // -1: undefined (i.e. for remeshing)
    //  0: barycenter
    // >0: number of the face vertex
    // always -1 in the case of a 2D tessellation
    int *FacePt;

    // Coordinates of the points used to interpolate the faces.
    // undefined if FacePt != 0
    double **FacePtCoo;

    // Domains of the faces [1...EdgeQty][0...1]
    // [i][0] = -1: not on domain
    // [i][0] =  2: on domain face
    // [i][1]: corresponding domain face (w/a)
    int **FaceDom;

    // POLYHEDRON INFORMATION --------------------------------------------

    // Number of polys.
    int PolyQty;

    // Number of faces of the polys [1...PolyQty]
    int *PolyFaceQty;

    // Numbers of the faces of the polys
    // [1...PolyQty][1...PolyFaceQty[i]]
    int **PolyFaceNb;

    // Orientations of the faces of the polys
    // [1...PolyQty][1...PolyFaceQty[i]].
    //  1: the face normal is outgoing
    // -1: the face normal points otherwise.
    int **PolyFaceOri;

    // States of the polys [1...PolyQty]:
    // seems to be always 0 (FIXME)
    int *PolyState;

    // DOMAIN INFORMATION ------------------------------------------------

    // Domain type ("cube", "cylinder", "sphere", "planes" or "cut")
    char *DomType;

    // Number of vertices of the domain
    int DomVerQty;

    // Coordinates of the domain vertices [1...DomVerQty][0...2]
    double **DomVerCoo;

    // Labels of the domain vertices [1...DomVerQty]
    char **DomVerLabel;

    // Number of domain edges of the domain vertices [1...DomVerQty]
    int *DomVerEdgeQty;

    // Numbers of the domain edges of the domain vertices
    // [1...DomVerQty][0...DomVerEdgeQty[i]-1]
    int **DomVerEdgeNb;

    // Numbers of the tessellation vertices of the domain vertices
    // [1...DomVerQty]
    int *DomTessVerNb;

    // Number of domain edges
    int DomEdgeQty;

    // Labels of the domain edges
    char **DomEdgeLabel;

    // Numbers of domain vertices of the domain edges
    // [1...DomVerQty]
    int *DomEdgeVerQty;

    // Numbers of the domain vertices of the domain edges
    // [1...DomVerQty][0...DomEdgeVerQty-1]
    int **DomEdgeVerNb;		// 0 indexed

    // Numbers of the domain faces of the domain edges
    // [1...DomEdgeQty][0...1]
    int **DomEdgeFaceNb;

    // Number of tessellation edges of the domain edges
    // [1...DomEdgeQty][0...1]
    int *DomTessEdgeQty;

    // Numbers of the tessellation edges of the domain edges
    // [1...DomEdgeQty][1...DomTessEdgeQty[i]]
    int **DomTessEdgeNb;

    // Number of domain faces
    int DomFaceQty;

    // Labels of the domain faces [1...DomFaceQty]
    char **DomFaceLabel;

    // Equation parameters of the domain faces [1...DomFaceQty][0...3]
    // DomFaceEq[i][1]*X1+DomFaceEq[i][2]*X2+DomFaceEq[i][3]*X3=DomFaceEq[i][0]
    double **DomFaceEq;

    /// Type of domain face: NULL, plane, cylinder, ecylinder or sphere
    /// The parameters are recorded in DomFaceParms
    char **DomFaceType;

    int* DomFaceParmQty;

    // ecylinder: Cx, Cy, Cz, axisx, axisy, axisz, ell1x, ell1y, ell1z, ell2x,
    // ell2y, ell2z, rady, radz
    double **DomFaceParms;

    // Number of domain vertices of the domain faces [1...DomFaceQty]
    int *DomFaceVerQty;

    // Numbers of the domain vertices of the domain faces
    // [1...DomFaceQty][1...DomFaceVerQty[i]]
    int **DomFaceVerNb;

    // Number of domain edge of the domain faces [1...DomFaceQty]
    int *DomFaceEdgeQty;

    // Numbers of the domain edges of the domain faces
    // [1...DomFaceQty][1...DomFaceEdgeQty[i]]
    int **DomFaceEdgeNb;

    // Amount of tessellation faces of the domain faces [1...DomFaceQty]
    int *DomTessFaceQty;

    // Numbers of the tessellation faces of the domain faces
    // [1...DomFaceQty][1...DomTessFaceQty[i]]
    int **DomTessFaceNb;

    // SCALE INFORMATION -------------------------------------------------

    // Number of scales
    int ScaleQty;

    // Identifiers of the cells that cells belong to throughout scales
    // [1...CellQty][0...ScaleQty]
    int **ScaleCellId;

    // PERIODICITY INFORMATION -------------------------------------------

    // Number of periodic (meaning slave) seeds
    // PerSeedQty = SeedQty - CellQty
    // Note that below, "Per" is used as a prefix to periodicity-related
    // variables rather than "slave".  Also, PerSeed* members go from 1
    // to SeedQty, i.e. cover all seeds.  Variables from 1 to CellQty
    // refer to the master seeds while variables from CellQty + 1 to
    // SeedQty refer to the slave seeds.
    int PerSeedQty;

    // Master seeds of the periodic seeds [CellQty+1...SeedQty]
    int *PerSeedMaster;

    // Periodicity shifts of the periodic seeds wrt their master seeds
    // [CellQty+1...SeedQty][0...2] Values can be: -1, 0, 1.
    int **PerSeedShift;

    // Slaves of the periodic (master) seeds [1...SeedQty][0...26]
    // Note that master seeds range between 1 and CellQty
    int **PerSeedSlave;

    // Number of periodic (slave) vertices
    int PerVerQty;

    // Numbers of the periodic (slave) vertices [1...PerVerQty]
    int *PerVerNb;

    // Masters of the periodic (slave) vertices [1...VerQty]
    // 0 if the vertex is a master, >0 otherwise
    int *PerVerMaster;

    // Periodicity shifts of the slave vertices wrt the corresponding
    // master vertices [1...VerQty][0...2].
    // For a given direction:
    // -1: slave vertex has a coordinate equal to the coordinate
    //     of its master minus the periodicity distance,
    //  0: slave vertex has a coordinate equal to the coordinate
    //     of its master,
    //  1: slave vertex has a coordinate equal to the coordinate
    //     of its master plus the periodicity distance,
    // 0,0,0 if the vertex is a master
    int **PerVerShift;

    // Number of slave vertices of the master vertices [1...VerQty]
    // >0 for all master vertices, 0 for all slave vertices
    int *PerVerSlaveQty;

    // Numbers of the slave vertices of the master vertices
    // [1...VerQty][1...PerVerSlaveQty[i]]
    int **PerVerSlaveNb;

    // Number of periodic (slave) edges
    int PerEdgeQty;

    // Numbers of the periodic (slave) edges [1...PerEdgeQty]
    int *PerEdgeNb;

    // Masters of the periodic (slave) edges [1...EdgeQty]
    // 0 if the edge is a master, >0 otherwise
    int *PerEdgeMaster;

    // Periodicity shifts of the slave edges wrt the corresponding
    // master edges [1...EdgeQty][0...2]
    // Values can be: -1, 0, 1 (see PerVerShift).
    // Equal to 0,0,0 for master edges
    int **PerEdgeShift;

    // Periodicity orientations of the slave edges wrt the corresponding
    // master edges [1...EdgeQty]:
    // -1: opposite sense
    //  1: same sense
    int *PerEdgeOri;

    // Number of slave edges of the periodic edges [1...PerEdgeQty]
    // >0 for all master edges, 0 for all slave edges
    int *PerEdgeSlaveQty;

    // Numbers of the slave edges of the periodic edges
    // [1...EdgeQty][1...PerEdgeSlaveQty[i]]
    int **PerEdgeSlaveNb;

    // Number of periodic (slave) faces
    int PerFaceQty;

    // Numbers of the periodic (slave) faces [1...PerFaceQty]
    int *PerFaceNb;

    // Masters of the periodic (slave) faces [1...FaceQty]
    // 0 if the face is a master, >0 otherwise
    int *PerFaceMaster;

    // Periodicity shifts of the slave faces wrt the corresponding
    // master faces [1...FaceQty][0...2]
    // Values can be: -1, 0, 1 (see PerVerShift)
    // Equal to 0,0,0 for master faces
    int **PerFaceShift;

    // Periodicity orientations of the slave faces wrt the corresponding
    // master faces [1...FaceQty]:
    // -1: opposite normal
    //  1: same normal
    int *PerFaceOri;

    // Numbers of the slave faces of the periodic faces
    // [1...FaceQty][1...PerFaceSlaveQty[i]]
    int *PerFaceSlaveNb;

    // pointer to SIM structure
    struct SIM *pSim;
  };
  typedef struct TESS TESS;

#endif				/* STRUCTTESS_H */

#ifdef __cplusplus
}
#endif
