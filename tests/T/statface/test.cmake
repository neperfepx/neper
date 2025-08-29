# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -group id -statface id,x,y,z,body,state,domtype,area,diameq,radeq,circularity,dihangleav,dihanglemin,dihanglemax,dihangles,ff,vernb,edgenb,polynb,neighnb,vers,edges,groupnb -o test)
include(../../test.cmake)
