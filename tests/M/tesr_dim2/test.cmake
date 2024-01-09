# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -M n2-id1-dim2.tesr -rcl 0.7 -for msh,geof,inp -order 1 -o test)
include(../../test.cmake)
