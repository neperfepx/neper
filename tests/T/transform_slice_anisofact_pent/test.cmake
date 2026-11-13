# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 1 -id 1 -transform "slice(2.5,1,2,3)" -statcell anisofact,area -o test)
include(../../test.cmake)
