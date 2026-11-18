# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 10 -dim 2 -domain "file(domain.tess):cell(2)" -o test)
include(../../test.cmake)
