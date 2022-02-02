# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n from_morpho::2::2 -morpho "cube(2)::voronoi::voronoi" -o test)
include(../../test.cmake)
