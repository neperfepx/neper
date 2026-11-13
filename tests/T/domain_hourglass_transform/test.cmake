# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 100 -domain "hourglass(1,1,0.9):translate(0.5,0.5,0.5):rotate(0,1,0,90)" -o test)
include(../../test.cmake)
