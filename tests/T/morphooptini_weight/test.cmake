# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Two seeds with w1 - w2 = 0.1: the interface is at x = 0.6 (ref written by hand).
set (test_command -T -n 2 -morpho voronoi -morphooptiini "coo:file(coo),weight:file(weight)" -statcell vol,w -o test)
include(../../test.cmake)
