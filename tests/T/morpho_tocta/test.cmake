# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n from_morpho -morpho "tocta(2)" -o test)
set (test_mode_force_minimal 1)
include(../../test.cmake)
