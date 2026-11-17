@echo on

:: MinGW-w64 GCC; GSL built in and no Scotch (their conda packages are
:: built with MSVC); Gmsh through its C interface.  Forward slashes for
:: CMake.
set "PREFIX_F=%LIBRARY_PREFIX:\=/%"
set "INC_F=%LIBRARY_INC:\=/%"
set "LIB_F=%LIBRARY_LIB:\=/%"

cmake -S src -B build -G Ninja %CMAKE_ARGS% ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_INSTALL_PREFIX="%PREFIX_F%" ^
  -DBUILD_TESTING=OFF ^
  -DHAVE_LIBSCOTCH=OFF ^
  -DFORCE_BUILTIN_GSL=ON ^
  -DGMSH_INCLUDE_DIR="%INC_F%" ^
  -DGMSH_LIBRARY="%LIB_F%/gmsh.dll.lib"
if errorlevel 1 exit 1

cmake --build build --parallel %CPU_COUNT%
if errorlevel 1 exit 1

cmake --install build
if errorlevel 1 exit 1
