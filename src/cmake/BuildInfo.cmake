# This file is part of the Neper software package.
# Copyright (C) 2003-2026, Romain Quey, CNRS.
# See the COPYING file in the top-level directory.

# Write the build information of Neper, neper_build_info.h (CMake script, run with cmake -P at
# every build by the target neper_build_info, and once at configure time; as FEPX's).
#
# Variables (-D):
#   SOURCE_DIR  the source tree (its git repository gives the hash, the date and the dirty flag)
#   OUTPUT      the header written (in the build tree)
#   VERSION     the version (project version), e.g. 5.0.1
#   SUFFIX      the version suffix: "-dev" between releases (devel), empty on main
#
# The version shown (neper --version, header):
#   - a release (main at the tag vVERSION, without uncommitted changes, or its tarball): the
#     version alone, e.g. 5.0.1;
#   - main, n commits after the release: VERSION+n with the git information, e.g.
#     5.0.1+3 (git 9f8e7d6c, 2026-11-20);
#   - devel: VERSION-dev with the git information, e.g. 5.0.2-dev (git 364c76cb, 2026-11-13).
# The git information is that of the commit built: its short hash (8 characters), the commit
# date as YYYY-MM-DD (ISO 8601), and "-dirty" after the hash when tracked files have uncommitted
# changes. Outside a git repository, it is that written by git archive into
# src/cmake/git_archive_info.txt (a source tarball: the file's export-subst attribute,
# .gitattributes, replaces its placeholder by the full hash, the date and the git describe of
# the commit archived); without it (a copy of the tree, or no git), the hash and the date are
# empty and neper shows the version only. The header is rewritten only when its text changes,
# so that an unchanged build compiles nothing.

set(git_hash "")
set(git_date "")
set(git_post "")   # the number of commits since the release (main only; empty if unknown)
string(REPLACE "." "\\." version_regex "${VERSION}")
find_program(GIT_EXECUTABLE git)
if(GIT_EXECUTABLE)
  # Only the source tree's own repository (not one that merely contains it).
  execute_process(COMMAND ${GIT_EXECUTABLE} -C ${SOURCE_DIR} rev-parse --show-toplevel
                  OUTPUT_VARIABLE git_top RESULT_VARIABLE git_status
                  OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
  if(git_status EQUAL 0)
    get_filename_component(git_top "${git_top}" REALPATH)
    get_filename_component(source_real "${SOURCE_DIR}" REALPATH)
  endif()
  if(git_status EQUAL 0 AND git_top STREQUAL source_real)
    # Short hash and commit date of HEAD.
    execute_process(COMMAND ${GIT_EXECUTABLE} -C ${SOURCE_DIR} log -1 --abbrev=8
                            --format=%h\ %cd --date=format:%Y-%m-%d
                    OUTPUT_VARIABLE git_log RESULT_VARIABLE git_status
                    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
    if(git_status EQUAL 0 AND git_log MATCHES "^([0-9a-f]+) ([0-9-]+)$")
      set(git_hash "${CMAKE_MATCH_1}")
      set(git_date "${CMAKE_MATCH_2}")
      # Uncommitted changes of tracked files.
      execute_process(COMMAND ${GIT_EXECUTABLE} -C ${SOURCE_DIR} status --porcelain
                              --untracked-files=no
                      OUTPUT_VARIABLE git_changes RESULT_VARIABLE git_status
                      OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
      if(git_status EQUAL 0 AND NOT git_changes STREQUAL "")
        string(APPEND git_hash "-dirty")
      endif()
      # main: the commits since the release (its tag vVERSION).
      if(SUFFIX STREQUAL "")
        execute_process(COMMAND ${GIT_EXECUTABLE} -C ${SOURCE_DIR} rev-list --count
                                v${VERSION}..HEAD
                        OUTPUT_VARIABLE git_count RESULT_VARIABLE git_status
                        OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        if(git_status EQUAL 0 AND git_count MATCHES "^[0-9]+$")
          set(git_post "${git_count}")
        endif()
      endif()
    endif()
  endif()
endif()

# A source tarball: the commit archived, from src/cmake/git_archive_info.txt, "<hash> <date>
# <describe>" (in a git repository the file still holds its placeholder, "$Format:...$", and is
# not used).
if(git_hash STREQUAL "" AND EXISTS ${SOURCE_DIR}/src/cmake/git_archive_info.txt)
  file(STRINGS ${SOURCE_DIR}/src/cmake/git_archive_info.txt archive_info LIMIT_COUNT 1)
  if(archive_info MATCHES "^([0-9a-f]+) ([0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9])( (.*))?$")
    string(SUBSTRING "${CMAKE_MATCH_1}" 0 8 git_hash)
    set(git_date "${CMAKE_MATCH_2}")
    set(git_describe "${CMAKE_MATCH_4}")
    if(SUFFIX STREQUAL "" AND git_describe MATCHES "^v${version_regex}(-([0-9]+)-g[0-9a-f]+)?$")
      if(CMAKE_MATCH_2)
        set(git_post "${CMAKE_MATCH_2}")
      else()
        set(git_post "0")
      endif()
    endif()
  endif()
endif()

# The version shown: a release shows the version alone (no git information), main after it
# VERSION+n.
set(build_version "${VERSION}${SUFFIX}")
if(SUFFIX STREQUAL "" AND git_post STREQUAL "0" AND NOT git_hash MATCHES "-dirty$")
  set(git_hash "")
  set(git_date "")
elseif(SUFFIX STREQUAL "" AND git_post MATCHES "^[1-9][0-9]*$")
  set(build_version "${VERSION}+${git_post}")
endif()

# The header, rewritten only when it changes.
set(text "/* Generated by src/cmake/BuildInfo.cmake: version and git information of the sources. */\n")
string(APPEND text "#define NEPER_BUILD_VERSION \"${build_version}\"\n")
string(APPEND text "#define NEPER_GIT_HASH \"${git_hash}\"\n")
string(APPEND text "#define NEPER_GIT_DATE \"${git_date}\"\n")
file(WRITE ${OUTPUT}.tmp "${text}")
configure_file(${OUTPUT}.tmp ${OUTPUT} COPYONLY)
file(REMOVE ${OUTPUT}.tmp)
