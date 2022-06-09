# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 10 -per 1 -statpoly "id,x,y,z,true,body,state,area,vol,size,diameq,radeq,sphericity,convexity,dihangleav,dihanglemin,dihanglemax,dihangles,vernb,edgenb,facenb,neighnb,vers,edges,faces,npolys,faceareas,faceeqs,vercoos,scaleid(1)" -o test)
include(../../test.cmake)
