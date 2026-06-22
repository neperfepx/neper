# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -V "ori(type=ori):file(n2-id1.ori)" -space ipf -imageformat asy -print test)
include(../../test.cmake)
