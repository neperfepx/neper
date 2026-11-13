# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# a missing POV-Ray must be reported, as must a POV-Ray that generates no file
foreach (case "nosuchpovray;POV-Ray .command .nosuchpovray.. not found" "true;could not be generated")
  list (GET case 0 binary)
  list (GET case 1 message)
  execute_process (COMMAND ${test_prog} --rcfile none -V ../povray/n2-id1.tess -povray ${binary} -print test
                   RESULT_VARIABLE RESVAR OUTPUT_VARIABLE OUTVAR ERROR_QUIET)
  if (NOT RESVAR OR NOT "${OUTVAR}" MATCHES "${message}")
    message (FATAL_ERROR "Test failed (${binary})")
  endif()
endforeach()
file (REMOVE test.pov test.png)
