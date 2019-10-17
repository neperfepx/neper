# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #
#                                    #####        # #    #                #
#        ####  #####  ###### #    # #     #       # #   #                 #
#       #    # #    # #      ##   # #             # #  #                  #
#       #    # #    # #####  # #  # #  ####       # ###                   #
#       #    # #####  #      #  # # #     # #     # #  #                  #
#       #    # #      #      #   ## #     # #     # #   #                 #
#        ####  #      ###### #    #  #####   #####  #    #                #
#                                                                         #
#   This file is part of openGJK.                                         #
#                                                                         #
#   openGJK is free software: you can redistribute it and/or modify       #
#    it under the terms of the GNU General Public License as published by #
#    the Free Software Foundation, either version 3 of the License, or    #
#    any later version.                                                   #
#                                                                         #
#   openGJK is distributed in the hope that it will be useful,            #
#    but WITHOUT ANY WARRANTY; without even the implied warranty of       #
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See The        #
#    GNU General Public License for more details.                         #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#    along with openGJK. If not, see <https://www.gnu.org/licenses/>.     #
#                                                                         #
#        openGJK: open-source Gilbert-Johnson-Keerthi algorithm           #
#         Copyright (C) Mattia Montanari, University of Oxford            #
#                        http://iel.eng.ox.ac.uk                         #
#                                                                         #
# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - #


project (openGJKlib)

set(CMAKE_C_STANDARD 11)

# SELECT USER OPTIONS
option(VERSION_ACCURATE  "Reduce speed to maximise accuracy (OFF)" OFF )

# APPLY USER OPTIONS
IF(VERSION_ACCURATE)
    set(USE_PREDICATES         ON)
    set(openGJK_VERSION "Accurate")
ELSE()
    set(USE_PREDICATES         OFF)
    set(openGJK_VERSION     "Fast")
ENDIF()

# COMPILE
message( "[${CMAKE_PROJECT_NAME}] Compiling ..")
message(STATUS "Version     (Accurate,Fast): " ${openGJK_VERSION}  )
message(STATUS "Build type  (Debug,Release): " ${CMAKE_BUILD_TYPE} )

# Select source files
set( SOURCE_FILES src/openGJK.c )
set( SOURCE_HEADS include/openGJK/openGJK.h)

IF(USE_PREDICATES)
    # for adpative floating-point artim.
    set( SOURCE_FILES ${SOURCE_FILES} ext/predicates.c)
    set( SOURCE_HEADS ${SOURCE_HEADS} ext/predicates.h)
    # Add flag for adpative floating-point artim.
    add_definitions(-DADAPTIVEFP)
ENDIF()

# Create the (dynamic) library
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${SOURCE_HEADS})
add_definitions(-DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=TRUE -DBUILD_SHARED_LIBS=TRUE)


target_link_libraries (openGJKlib  ${EXTRA_LIBS})

# Link include file
target_include_directories( ${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)

IF(USE_PREDICATES)
    # for adpative floating-point artim.
    target_include_directories( ${PROJECT_NAME}
            PUBLIC ${PROJECT_SOURCE_DIR}/ext
            )
ENDIF()

# Report
message( ".. DONE!")
