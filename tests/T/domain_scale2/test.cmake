# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 2 -dim 2 -domain "square(1,1):scale(1,2)" -o test)
include(../../test.cmake)
