# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n from_morpho -periodic 1 -morpho "cube(1,2,3)" -o test)
include(../../test.cmake)
