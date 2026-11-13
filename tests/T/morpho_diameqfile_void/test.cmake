# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -dim 2 -n 30 -morpho "diameq:file(diameq30)" -morphooptistop itermax=300 -morphooptilogval iter,val,valmin -o test)
include(../../test.cmake)
