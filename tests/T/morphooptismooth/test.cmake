# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -morpho gg -morphooptismooth 0.1 -morphooptistop itermax=10 -o test)
include(../../test.cmake)
