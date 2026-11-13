# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# only the extension of the asy file must be replaced in the output file name
execute_process (COMMAND ${test_prog} --rcfile none -V ../asymptote/n2-id1.tess -space pf -print fantasy
                 RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (RESVAR OR NOT EXISTS fantasy.png)
  message (FATAL_ERROR "Test failed")
endif()
file (REMOVE fantasy.png)
