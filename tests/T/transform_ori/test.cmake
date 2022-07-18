# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -loadtess n2-id1.tess -transform "ori(cell-oris)" -o test)
include(../../test.cmake)
