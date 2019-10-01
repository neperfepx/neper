/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_H
#define STRUCTIN_H

struct IN_M
{
  // prerequisites

  char *gmsh;			// absolute path of Gmsh
  char *tmp;			// absolute path of temporary directory

  // input data

  char *tess;			// input tess file
  char *tesr;			// input tesr  file
  char *mesh;			// input mesh file

  char *loadmesh;		// load an output mesh file
  char *loadpoint;		// load a set of points

  // meshing options
  char *elttype;		// type of elements (tri or quad)
  int order;			// order of the mesh (1 or 2)
  char *dimstring;		// meshing dimension string
  int dim;			// dimension of the mesh (0, 1, 2 or 3)
  char *dimout;			// dimensions of the output meshes
  char *dimout_msh;		// dimout, but specific to msh format.

  char *cltype;			// abs or rel
  char *clstring;		// value of -cl or -rcl
  char *clratiostring;		// ratio between the x, y and z direction

  char *clfacetype;		// abs or rel, face
  char *clfacestring;		// value of -cl or -rcl, face
  char *cledgetype;		// abs or rel, edge
  char *cledgestring;		// value of -cl or -rcl, edge
  char *clvertype;		// abs or rel, ver
  char *clverstring;		// value of -cl or -rcl, ver

  double clmin;
  double pl;

  char *meshpoly;		// polys to mesh (expression)
  char *meshface;		// faces to mesh (expression)
  char* interface;		// type of interface meshing
                                // (continuous or discontinuous)

  char *tesrsmooth;		// smoothing method for raster tessellation
  // (laplacian)
  double tesrsmoothfact;	// factor for smoothing
  int tesrsmoothitermax;	// itermax for smoothing

  char *mesh2dalgo;		// 2d meshing algos
  int mesh2dpinchfix;		// 2d mesh pinches fixing
  char *mesh3dalgo;		// 3d meshing algos
  char *meshqualexpr;		// expression of O (see the Neper paper)
  double meshqualmin;		// min, target value of O (see the Neper paper)
  char *meshqualdisexpr;	// expression of Odis (see the Neper paper)
  double mesh2dmaxtime;		// 2D mesher max run time
  double mesh2drmaxtime;	// 2D mesher max relative run time
  int mesh2diter;		// 2D mesher max number of iterations
  double mesh3dmaxtime;		// 3D mesher max run time
  double mesh3drmaxtime;	// 3D mesher max relative run time
  int mesh3diter;		// 3D mesher max number of iterations
  char* mesh3dclrepsstring;     // 3D mesh cl convergence criterion
  int mesh3dreport;		// report statistics on the 3D mesh (logical)

  char *transportstring;	// Data to transport
  char *transportfepxstring;	// Data to transport / specific to FEpX
  char *transporteltmethodstring; // Method to transport elemental data

  int singnodedup;		// duplicate singular nodes (logical)
  double dupnodemerge;		// merge duplicate nodes (tolerancy)

  char *nset;			// list of nsets
  char *elset;			// list of elsets
  char *faset;			// list of fasets

  char *partstring;		// partitioning string
  char *partmethod;		// partitioning method (in Scotch jargon)
  double partbalancing;		// partitioning balancing factor

  // transformation
  char *transform;        // transform the mesh once generated (or loaded)

  // output details

  char *format;			// format of the output mesh
  char *performat;	        // format of the output per file

  char *body;			// output file name, without extension
  char *msh;			// mesh file at the msh format
  char *vtk;			// mesh file at the vtk format
  char *abq;			// mesh file at the Abaqus format
  char *geof;			// mesh file at the Zset/Zebulon format
  char *fepx1;			// mesh file at the fepx format, parms file
  char *fepx2;			// mesh file at the fepx format, mesh  file
  char *fepx3;			// mesh file at the fepx format, surf  file
  char *fepx4;			// mesh file at the fepx format, opt   file
  char *fepx5;			// mesh file at the fepx format, bcs   file
  char *outtess;		// output tessellation
  char *per;			// periodicity file
  char *intf;	        	// interface file

  char *stn;			// statistics on the nodes
  char **stelt;			// statistics on the 0D, 1D, 2D and 3D elts
  char **stelset;		// statistics on the 0D, 1D, 2D and 3D elsets
  char *stpt;			// statistics on points
};
typedef struct IN_M IN_M;

#endif /* STRUCTIN_H */
