# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Format <= 3.5 records sqrt (w): same geometry as morphooptini_weight (ref written by hand).
set (test_command -T -loadtess old.tess -statcell vol,w -o test)
include(../../test.cmake)
