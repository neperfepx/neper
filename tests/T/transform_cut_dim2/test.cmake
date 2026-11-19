# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 20 -dim 2 -transform "cut(hspace(0.1,1,0,0),cylinder(0.5,0.5,0,0,0,1,0.2))" -o test)
include(../../test.cmake)
