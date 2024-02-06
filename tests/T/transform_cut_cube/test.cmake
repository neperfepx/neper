# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -transform "cut(cube(-0.3,0.7,-0.3,0.7,0.3,0.7,0.001))" -o test)
include(../../test.cmake)
