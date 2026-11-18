# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 10 -dim 2 -domain "planes(planes)" -morpho "size:dirac(1)" -morphooptistop itermax=200 -o test)
include(../../test.cmake)
