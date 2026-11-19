# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n 800 -dim 2 -domain "circle(18):translate(-9,-9)" -transform "cut(cylinder(0,0,0,0,0,1,8.5),hspace(0,0,-1,0),hspace(0,-1,0,0))" -reg 1 -o test)
include(../../test.cmake)
