# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -n from_morpho -morpho "tocta(1,2,3)" -domain "cube(1,2,3)" -o test)
set (test_mode_force_minimal 0)
include(../../test.cmake)
