# Copyright (c) 2012 Romain Quey
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

find_path (NLOPT_INCLUDE_DIR NAMES nlopt.h)

set (NLOPT_INCLUDE_DIRS ${NLOPT_INCLUDE_DIR})

find_library (NLOPT_LIBRARY NAMES nlopt nlopt_cxx)

set (NLOPT_LIBRARIES ${NLOPT_LIBRARY})

set (NLOPT_NAMES nlopt nlopt_cxx)

# handle the QUIETLY and REQUIRED arguments and set NLOPT_FOUND to TRUE if 
# all listed variables are TRUE
include ( FindPackageHandleStandardArgs )
find_package_handle_standard_args (NLOPT DEFAULT_MSG NLOPT_LIBRARY NLOPT_INCLUDE_DIRS)

# mark_as_advanced (NLOPT_LIBRARY NLOPT_INCLUDE_DIRS)
