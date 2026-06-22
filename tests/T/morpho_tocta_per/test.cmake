# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

set (test_command -T -morpho "tocta(2)" -per 1 -o test)
set (test_mode_force_minimal 0)
include(../../test.cmake)
