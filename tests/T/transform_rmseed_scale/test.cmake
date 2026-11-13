# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 30 -id 1 -transform "scale(2,2,2),rmseed(id==1)" -o test)
include(../../test.cmake)
