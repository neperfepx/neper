# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -transform "cut(torus(-0.5,0,1.0,0,0,1,1.1,0.5))" -o test)
include(../../test.cmake)
