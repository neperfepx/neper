# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# the POV-Ray binary path can contain spaces, and file names shell characters
find_program (povray_binary povray)
file (MAKE_DIRECTORY "my dir")
execute_process (COMMAND ${CMAKE_COMMAND} -E create_symlink ${povray_binary} "my dir/povray")
execute_process (COMMAND ${test_prog} --rcfile none -V ../povray/n2-id1.tess -povray "my dir/povray" -print "test(1)"
                 RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (RESVAR OR NOT EXISTS "test(1).png")
  message (FATAL_ERROR "Test failed")
endif()
file (REMOVE_RECURSE "my dir" "test(1).png")
