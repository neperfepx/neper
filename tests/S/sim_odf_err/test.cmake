# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# ODF-based results that must be rejected, with no directory left behind:
# missing orientation space (fr99), invalid argument (failing while computing
# the result) and triclinic crystal symmetry
execute_process(COMMAND rm -rf test.sim)
execute_process(COMMAND cp -r n2-id1.sim test.sim)

# invalid -orispace, whatever the results
foreach (orispace "foo" "fr5" "rodrigues(my.msh)" "rodrigues(fred.msh)" "rodrigues(fr)" "rodrigues(fr5x)"
                  "rodrigues(x=fr5)" "rodrigues(fr5,fr5)" "euler-bunge(fr5)")
  execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -orispace ${orispace} -rescell x
                  RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
  if (NOT RESVAR)
    message(FATAL_ERROR "Test failed - -orispace ${orispace} accepted")
  endif()
endforeach()

execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -orispace "rodrigues(fr99)" -resmesh odf
                RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (NOT RESVAR)
  message(FATAL_ERROR "Test failed - fr99 accepted")
endif()
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.sim/results/mesh)
  message(FATAL_ERROR "Test failed - fr99 left mesh")
endif()

execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -orispace "rodrigues(fr5)" -resmesh foo:odf(input=bar)
                RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (NOT RESVAR)
  message(FATAL_ERROR "Test failed - odf(input=bar) accepted")
endif()
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.sim/results/mesh)
  message(FATAL_ERROR "Test failed - odf(input=bar) left mesh")
endif()

# an existing result is kept when a following one fails
execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -orispace "rodrigues(fr5)" -resmesh vol)
execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -orispace "rodrigues(fr5)" -resmesh vol,foo:odf(input=bar)
                RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (NOT RESVAR)
  message(FATAL_ERROR "Test failed - odf(input=bar) accepted")
endif()
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.sim/results/mesh/vol)
  message(FATAL_ERROR "Test failed - mesh/vol removed")
endif()
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.sim/results/mesh/foo)
  message(FATAL_ERROR "Test failed - mesh/foo exists")
endif()
execute_process(COMMAND rm -rf test.sim)
execute_process(COMMAND cp -r n2-id1.sim test.sim)

execute_process(COMMAND sed -i "s/cubic/triclinic/" test.sim/inputs/simulation.tess test.sim/inputs/simulation.msh)

foreach (opt "-restess;odf" "-restess;odfnindex" "-rescell;odf" "-resmesh;odf" "-resmesh;foo:odfn(theta=5)"
             "-resmesh;orifield(var=x)" "-reselset;odf" "-reselt;odf")
  execute_process(COMMAND ${test_prog} --rcfile none -S test.sim ${opt}
                  RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
  if (NOT RESVAR)
    message(FATAL_ERROR "Test failed - ${opt} accepted")
  endif()
endforeach()

foreach (absent_dir tess/odf tess/odfnindex cells/odf mesh/odf mesh/foo mesh/orifield elsets/odf elts/odf)
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test.sim/results/${absent_dir})
    message(FATAL_ERROR "Test failed - ${absent_dir} exists")
  endif()
endforeach()

# results other than ODF-based ones do not need the orientation space
execute_process(COMMAND ${test_prog} --rcfile none -S test.sim -rescell x
                RESULT_VARIABLE RESVAR OUTPUT_QUIET ERROR_QUIET)
if (RESVAR)
  message(FATAL_ERROR "Test failed - triclinic -rescell x rejected")
endif()

execute_process(COMMAND rm -r test.sim)
