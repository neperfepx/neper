# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# POV-Ray is not needed for writing a pov file
set (test_command -V ../povray/n2-id1.tess -povray nosuchpovray -imageformat pov -print test)
set (absent_files test.png)
include(../../test.cmake)
