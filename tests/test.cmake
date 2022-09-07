# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

execute_process(COMMAND ${test_prog} --rcfile none ${test_command} RESULT_VARIABLE RESVAR)

if(RESVAR)
  message(FATAL_ERROR "Test failed")
endif()

if ("${test_mode}" MATCHES "Normal" AND NOT "${test_mode_force_minimal}" EQUAL 1)
  file(GLOB bak_files *.bak *~)
  foreach(bak_file ${bak_files})
    file (REMOVE ${bak_file})
  endforeach()

  file(GLOB ref_files ref.* ref-*)
  foreach(ref_file ${ref_files})
    string(REPLACE "ref" "test" test_file ${ref_file})
    if (NOT "${test_file}" MATCHES ".png")
      if ("${test_mode_diff}" MATCHES "cmake")
        execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files
                        ${test_file} ${ref_file}
                        RESULT_VARIABLE RESVAR)
      else()
        execute_process(COMMAND ${test_prog} --diff
                        ${test_file} ${ref_file}
                        RESULT_VARIABLE RESVAR)
      endif()
    else()
      execute_process(COMMAND compare -metric AE ${test_file} ${ref_file} NULL:
                      RESULT_VARIABLE RESVAR)
    endif()

    if(RESVAR)
      file(RENAME ${test_file} ${test_file}.bak)
      message(FATAL_ERROR "Test failed - files differ")
      file (REMOVE ${test_file})
    endif()
  endforeach()

elseif ("${test_mode}" MATCHES "Writing")
  file(GLOB test_files test*)
  foreach(test_file ${test_files})
    if (NOT "${test_file}" MATCHES "test.cmake")
      string(REPLACE "test." "ref." tmp ${test_file})
      string(REPLACE "test-" "ref-" ref_file ${tmp})
      file(RENAME ${test_file} ${ref_file})
    endif()
  endforeach()
endif()

file(GLOB test_files test*)
foreach(test_file ${test_files})
  if (NOT "${test_file}" MATCHES "test.cmake")
    string (FIND ${test_file} ".bak" RESVAR)
    if (RESVAR EQUAL -1)
      file (REMOVE ${test_file})
    endif()
  endif()
endforeach()
