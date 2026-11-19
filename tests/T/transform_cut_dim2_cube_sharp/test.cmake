# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 50 -dim 2 -transform "cut(cylinder(0.3,0.3,0,0,0,1,0.1),cube(0.2,1,0,0.3,0,1,0))" -o test)
include(../../test.cmake)
