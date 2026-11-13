# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -morpho gg -morphooptismooth diameq:0.1,1-sphericity:0.01 -morphooptistop itermax=10 -o test)
include(../../test.cmake)
