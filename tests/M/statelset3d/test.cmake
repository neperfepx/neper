# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M -loadmesh n2-id1.msh -statelset3d id,x,y,z,vol,eltnb,elts,nodenb,nodes,rrav,rrmin,rrmax,Osize,true,body -o test)
include(../../test.cmake)
