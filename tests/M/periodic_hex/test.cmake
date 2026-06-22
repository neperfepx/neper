# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -M n2.tess -elt hex -cl 0.5 -order 2 -for msh,per -o test)
include(../../test.cmake)
