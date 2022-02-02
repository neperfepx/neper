# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M -loadmesh n2-id1.msh -statelset3d id,x,y,z,vol,eltnb,rrav,rrmin,rrmax,Osize,eltnb,true,body -o test)
include(../../test.cmake)
