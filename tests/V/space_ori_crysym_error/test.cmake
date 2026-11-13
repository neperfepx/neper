# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# the orientation space is only available for the cubic and hexagonal symmetries
execute_process (COMMAND ${test_prog} --rcfile none -V ../povray/n2-id1.tess -crysym triclinic -space ori -print test
                 RESULT_VARIABLE RESVAR OUTPUT_VARIABLE OUTVAR ERROR_QUIET)
if (NOT RESVAR OR NOT "${OUTVAR}" MATCHES "Orientation space not available")
  message (FATAL_ERROR "Test failed")
endif()
