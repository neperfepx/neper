# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -morpho gg -morphooptistop time=0.1 -o test)
set (test_mode_force_minimal 1)
include(../../test.cmake)
