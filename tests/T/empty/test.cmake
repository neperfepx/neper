# This file is part of the Neper software package.
# Copyright(C) 2003-2022, Romain Quey.
# See the COPYING file in the top-level directory.

set (test_command -T -n 3 -dim 2 -domain "square(200,200)" -morphooptiini "coo:file(coo)"  -o test)
include (../../test.cmake)
