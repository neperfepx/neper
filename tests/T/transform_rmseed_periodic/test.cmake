# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -periodic 1 -transform "rmseed(cell_size<0.006)" -o test)
include(../../test.cmake)
