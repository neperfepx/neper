# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M n50.tess -mesh3dclreps 1 -cl 0.5 -order 1 -o test)
include(../../test.cmake)
