# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M n20-id1.tess -mesh3dclreps 1 -pl 10000 -meshqualmin 0 -order 1 -o test)
include(../../test.cmake)
