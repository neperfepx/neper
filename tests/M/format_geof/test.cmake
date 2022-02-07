# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M n2-id1.tess -loadmesh n2-id1.msh -for geof -o test)
include(../../test.cmake)
