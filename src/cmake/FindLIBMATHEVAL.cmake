# Copyright (c) 2010 Vladimir Chalupecky - Modified 2012 Romain Quey
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

# use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
  find_path(Matheval_INCLUDEDIR
    NAMES matheval.h HINTS
	   )
  set (Matheval_INCLUDE_DIRS ${Matheval_INCLUDEDIR})

  find_library( Matheval_LIBRARIES
    NAMES matheval HINTS
    ${Matheval_LIBDIR}
    ${Matheval_LIBRARY_DIRS}
	      )

# handle the QUIETLY and REQUIRED arguments and set Matheval_FOUND to
# TRUE if 
# all listed variables are TRUE
  include( FindPackageHandleStandardArgs )
  find_package_handle_standard_args( Matheval DEFAULT_MSG
          Matheval_LIBRARIES Matheval_INCLUDE_DIRS)

# mark_as_advanced(Matheval_INCLUDE_DIRS Matheval_LIBRARIES)
