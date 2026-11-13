# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# results that must be rejected, with no directory left behind or written:
# component out of range (vec: 3-column vector), component of a scalar (sca),
# result of known name of invalid number of columns (strain: 9 columns),
# operation on a result of undefined type (und), invalid type, declared type
# inconsistent with the file, unknown argument, steps not in the directory,
# number of lines different from the number of elements, missing file
execute_process(COMMAND rm -rf test.sim)
execute_process(COMMAND cp -r n2-id1.sim test.sim)

foreach (res "vec12" "sca1" "strain12" "strain_eq" "und1" "und12" "und_eq"
             "foo:file(six,type=matrix)" "foo:file(six,type=tensor)" "foo:file(six,foo=1)" "foo:file(ext)" "foo:file(short)" "foo:file(nofile)")
  execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -reselt ${res}
                  RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
  if (NOT RESVAR)
    message(FATAL_ERROR "Test failed - ${res} accepted")
  endif()
endforeach()

execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -resmesh und_prval
                RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (NOT RESVAR)
  message(FATAL_ERROR "Test failed - und_prval accepted")
endif()

foreach (absent_dir elts/vec12 elts/sca1 elts/strain12 elts/strain_eq elts/und1 elts/und12
                    elts/und_eq elts/foo mesh/und_prval)
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.sim/results/${absent_dir})
    message(FATAL_ERROR "Test failed - ${absent_dir} exists")
  endif()
endforeach()

execute_process(COMMAND rm -r test.sim)
