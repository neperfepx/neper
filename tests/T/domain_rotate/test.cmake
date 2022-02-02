# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -domain "cube(1,1,1):rotate(1,1,1,10)" -o test)
include(../../test.cmake)
