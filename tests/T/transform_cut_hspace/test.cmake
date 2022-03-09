# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 20 -transform "cut(hspace(0.5,0,0,1))" -o test)
include(../../test.cmake)
