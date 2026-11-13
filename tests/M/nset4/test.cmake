# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -M n2-id1.tess -nset "x0,bottom:z<1e-6,top:max(z),corner:min(x+y+z),pt:closest(0.5,0.5,1),vertices" -order 1 -o test)
include(../../test.cmake)
