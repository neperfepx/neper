# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

# Taken from discussion 447
set (test_command -M n2.tess -rcl 1.7 -elttype quad -order 1 -o test)
include(../../test.cmake)
