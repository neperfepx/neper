# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -domain "cylinder(1,2)" -transform "renamedomface(cyl,lateral),renamedomface(z0,bottom)" -o test)
include(../../test.cmake)
