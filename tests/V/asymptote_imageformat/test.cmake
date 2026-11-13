# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Asymptote is not needed for writing an asy file
set (test_command -V ../asymptote/n2-id1.tess -space pf -asymptote nosuchasy -imageformat asy -print test)
set (absent_files test.png)
include(../../test.cmake)
