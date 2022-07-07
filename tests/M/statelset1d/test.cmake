# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M -loadmesh n2-id1.msh -statelset1d id,x,y,z,length,eltnb,elts,nodenb,nodes,true,body,domtype -o test)
include(../../test.cmake)
