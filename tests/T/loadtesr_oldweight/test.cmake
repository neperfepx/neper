# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Format <= 2.2 records sqrt (w): the weights must be 0.1 and 0 (ref checked by hand).
set (test_command -T -loadtesr old.tesr -format tesr -o test)
include(../../test.cmake)
