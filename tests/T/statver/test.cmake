# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -group id -statver id,x,y,z,body,state,domtype,edgenb,facenb,polynb,groupnb -o test)
include(../../test.cmake)
