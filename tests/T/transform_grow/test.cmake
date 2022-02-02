# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -for tesr -domain "cylinder(1,1,100)" -transform grow -o test)
set (test_mode_diff "cmake")
include(../../test.cmake)
