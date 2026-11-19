# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 20 -dim 2 -transform "cut(cube(0.25,1,0,0.3,0,1,0.05))" -o test)
include(../../test.cmake)
