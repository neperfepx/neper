#!/bin/bash
set -euxo pipefail

# GSL and Scotch: conda packages (found by CMake instead of the built-in
# copies of src/contrib).
cmake -S src -B build ${CMAKE_ARGS:-} \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX="${PREFIX}" \
  -DBUILD_TESTING=OFF

cmake --build build --parallel "${CPU_COUNT:-2}"
cmake --install build
