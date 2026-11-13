# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -M n2-id1.tess -cl 0.5 -clface 0.3 -clver 0.1 -pledge 1.5 -plface 1.5 -plpoly "(id==1)?3:default" -order 1 -o test)
include(../../test.cmake)
