# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -V n2-id1.tess,n2-id1.msh -slicemesh "x=0.2,y=0.4,z=0.6" -showtess 1 -showmesh 0 -showmeshslice 0 -imageformat pov -print test)
include(../../test.cmake)
