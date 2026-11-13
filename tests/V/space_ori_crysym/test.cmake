# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# triclinic (the default of -T) means not defined: -crysym, then cubic, apply
foreach (crysym "" "-crysym;cubic" "-crysym;hexagonal")
  execute_process (COMMAND ${test_prog} --rcfile none -V ../povray/n2-id1.tess ${crysym} -space ori -print test
                   RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
  if (RESVAR OR NOT EXISTS test.png)
    message (FATAL_ERROR "Test failed (${crysym})")
  endif()
  file (REMOVE test.png)
endforeach()
