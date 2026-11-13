# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -dim 2 -reg 1 -regrsel 5 -regdelcell 1 -o test)
include(../../test.cmake)
