# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M -loadmesh n2-id1.msh -statelt1d id,x,y,z,elset1d,length,body,domtype -order 1 -o test)
include(../../test.cmake)
