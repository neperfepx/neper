# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M n2-id1.tess,n2-id1.msh -clratio 2:1:0.5 -for tess,msh -order 1 -o test)
include(../../test.cmake)
