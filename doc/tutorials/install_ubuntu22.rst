.. _installation_ubuntu_22:

Installing Neper on Ubuntu 22
=============================

.. important:: Read first: :ref:`introduction`, especially :ref:`installing_neper` and :ref:`testing_neper`.

This tutotial describes how to install Neper step by step on a *fresh* Ubuntu 22 distribution.  It may apply to other Ubuntu distributions too, and may be adapted to other distributions quite easily.  General installation instructions are provided in :ref:`introduction` (:ref:`installing_neper`), and this tutorial does not aim to substitute to it. However, we adopt here a different approach by managing (dependency) issues as they occur during the installation on a fresh Ubuntu 22 distribution (which misses packages that would typically already be present on older installations).

Downloading Neper
-----------------

We first download the last official version from the website (https://neper.info), :code:`neper-4.6.1.tar.gz` at the time of writing.
It is then a good idea to move the archive to a dedicated directory, for example :file:`~/Progs`, and work from there.

We create directory :file:`~/Progs` and move the archive into it:

.. code-block:: console

  $ mkdir ~/Progs
  $ mv neper-4.6.1.tar.gz ~/Progs

We change directory to :file:`Progs` and unarchive the code:

.. code-block:: console

  $ cd ~/Progs
  $ tar -zxf neper-4.6.1.tar.gz

Compiling Neper
---------------

We first compile Neper using the commands provided in :ref:`installing_neper` (namely, we should follow the :command:`cmake`, :command:`make`, :command:`make install` sequence):

.. code-block:: console

  $ cd neper-4.6.1
  $ cd src
  $ mkdir build
  $ cd build
  $ cmake ..

  Command 'cmake' not found, but can be installed with:
  sudo snap install cmake  # version 3.26.4, or
  sudo apt  install cmake  # version 3.22.1-1ubuntu1.22.04.1
  See 'snap info cmake' for additional versions.

We follow the terminal output and install :command:`cmake`:

.. code-block:: console

  $ sudo apt install cmake

We re-run Cmake:

.. code-block:: console

  $ cmake ..

  -- The C compiler identification is GNU 11.3.0
  -- The CXX compiler identification is unknown
  -- Detecting C compiler ABI info
  -- Detecting C compiler ABI info - done
  -- Check for working C compiler: /usr/bin/cc - skipped
  -- Detecting C compile features
  -- Detecting C compile features - done
  CMake Error at CMakeLists.txt:11 (project):
    No CMAKE_CXX_COMPILER could be found.

  Tell CMake where to find the compiler by setting either the environment
  variable "CXX" or the CMake cache entry CMAKE_CXX_COMPILER to the full path
  to the compiler, or to the compiler name if it is in the PATH.

  -- Configuring incomplete, errors occurred!
  See also "/home/rquey/Progs/neper-4.6.1/src/build/CMakeFiles/CMakeOutput.log".
  See also "/home/rquey/Progs/neper-4.6.1/src/build/CMakeFiles/CMakeError.log".

A CXX (C++) compiler is missing.  The GNU C++ compiler is :command:`g++`; so, we install it:

.. code-block:: console

  $ sudo apt install g++

We re-run Cmake:

.. code-block:: console

  $ cmake ..

  -- WARNING: you are using the obsolete 'PKGCONFIG' macro, use FindPkgConfig
  -- WARNING: PKGCONFIG() indicates that the tool pkg-config has not been found on your system. You should install it.
  FindGSL.cmake: gsl-config/pkg-config gsl not found. Please set
  it manually. GSL_CONFIG=GSL_CONFIG-NOTFOUND
  CMake Error at CMakeLists.txt:46 (_message):
    GSL required, please specify it's location.
  Call Stack (most recent call first):
    cmake/FindGSL.cmake:178 (MESSAGE)
    CMakeLists.txt:103 (find_package)

  -- Configuring incomplete, errors occurred!
  See also "/home/rquey/Progs/neper-4.6.1/src/build/CMakeFiles/CMakeOutput.log".
  See also "/home/rquey/Progs/neper-4.6.1/src/build/CMakeFiles/CMakeError.log".

We follow the terminal output and install :command:`pkg-config`:

.. code-block:: console

  $ sudo apt install pkg-config

We re-run Cmake:

.. code-block:: console

  $ cmake ..

  -- WARNING: you are using the obsolete 'PKGCONFIG' macro, use FindPkgConfig
  -- PKGCONFIG() indicates that gsl is not installed (install the package which contains gsl.pc if you want to support this feature)
  FindGSL.cmake: gsl-config/pkg-config gsl not found. Please set
  it manually. GSL_CONFIG=GSL_CONFIG-NOTFOUND
  CMake Error at CMakeLists.txt:46 (_message):
    GSL required, please specify it's location.
  Call Stack (most recent call first):
    cmake/FindGSL.cmake:178 (MESSAGE)
    CMakeLists.txt:103 (find_package)

  -- Configuring incomplete, errors occurred!
  See also "/home/rquey/Progs/neper-4.6.1/src/build/CMakeFiles/CMakeOutput.log".
  See also "/home/rquey/Progs/neper-4.6.1/src/build/CMakeFiles/CMakeError.log".

We have to install :command:`gsl`. We search for it as follows (it is a library, so, we prefix it with :command:`lib` in the search):

.. code-block:: console

  $ sudo apt search libgsl

  Sorting... Done
  Full Text Search... Done
  libgsl-dbg/jammy 2.7.1+dfsg-3 amd64
    GNU Scientific Library (GSL) -- debug symbols package

  libgsl-dev/jammy 2.7.1+dfsg-3 amd64
    GNU Scientific Library (GSL) -- development package

  libgsl27/jammy 2.7.1+dfsg-3 amd64
    GNU Scientific Library (GSL) -- library package

  libgslcblas0/jammy 2.7.1+dfsg-3 amd64
    GNU Scientific Library (GSL) -- blas library package

  libocamlgsl-ocaml/jammy 1.24.3-1build3 amd64
    GNU scientific library for OCaml

  libocamlgsl-ocaml-dev/jammy 1.24.3-1build3 amd64
    GNU scientific library for OCaml

:command:`libgsl-dev` is the one we need; so, we install it:

.. code-block:: console

  $ sudo apt install libgsl-dev

We re-run Cmake:

.. code-block:: console

  $ cmake ..
  -- Found OpenMP_C: -fopenmp (found version "4.5")
  -- Found OpenMP_CXX: -fopenmp (found version "4.5")
  -- Found OpenMP: TRUE (found version "4.5")
  -- Could NOT find system Scotch - using built-in
  -- Could NOT find system NLopt - using built-in
  -- Neper is set to be installed in: /usr/local/bin and /usr/share
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /home/rquey/Progs/neper-4.6.1/src/build

It now seems good to go.  We can compile:

.. code-block:: console

  $ make
  [  0%] Building C object contrib/nlopt/CMakeFiles/nlopt.dir/src/algs/direct/DIRect.c.o
  ...
  [100%] Built target neper

We can already install Neper (with root permissions):

.. code-block:: console

  $ sudo make install

which copies the :file:`neper` binary to a system location and makes the :command:`neper` command available system-wide.

.. note:: It is also possible to simply copy the :file:`neper` binary to a user location, e.g. :file:`~/bin/`, and make it visible to the system by including :file:`~/bin` to your :code:`$PATH`, by adding :code:`PATH=$PATH:~/bin` to your :file:`~/.bashrc` file (assuming you are using the Bash shell (:command:`echo $SHELL`)).

We can now test the installation:

.. code-block:: console

   $ ctest

  Test project /home/rquey/Progs/neper/neper/src/build
          Start   1: T/dim
    1/461 Test   #1: T/dim ...................................   Passed    0.02 sec
  [...]

  90% tests passed, 44 tests failed out of 461

  Total Test time (real) =  48.33 sec

  The following tests FAILED:
          129 - M/cl (Failed)
          130 - M/cl_expr (Failed)
          133 - M/clmin (Failed)
          134 - M/clratio (Failed)
          135 - M/cut (Failed)
          136 - M/dim (Failed)
          137 - M/dim2 (Failed)
          138 - M/dim_expr (Failed)
          139 - M/faset (Failed)
          142 - M/interface (Failed)
          143 - M/interface2 (Failed)
          144 - M/interface3 (Failed)
          145 - M/mesh2dalgo_dela (Failed)
          146 - M/mesh2dalgo_fron (Failed)
          147 - M/mesh2dalgo_mead (Failed)
          148 - M/mesh2dalgo_netg (Failed)
          149 - M/mesh3dalgo_netggmne (Failed)
          150 - M/mesh3dalgo_netggmsh (Failed)
          151 - M/mesh3dalgo_netgnetg (Failed)
          152 - M/meshing (Failed)
          154 - M/meshqualdisexpr (Failed)
          155 - M/meshqualexpr (Failed)
          156 - M/meshqualmin (Failed)
          157 - M/morpho_cube (Failed)
          158 - M/nset1 (Failed)
          159 - M/nset2 (Failed)
          160 - M/nset3 (Failed)
          161 - M/order (Failed)
          162 - M/order_dim2 (Failed)
          166 - M/part2 (Failed)
          167 - M/part_dim2 (Failed)
          168 - M/pl (Failed)
          169 - M/rcl (Failed)
          170 - M/rcl_expr (Failed)
          173 - M/remesh1 (Failed)
          174 - M/remesh2 (Failed)
          175 - M/remesh3 (Failed)
          176 - M/remesh4 (Failed)
          182 - M/statelsetco (Failed)
          188 - M/stateltco (Failed)
          190 - M/tesr_dim2 (Failed)
          193 - M/tess2ddim3 (Failed)
          211 - V/asymptote (Failed)
          326 - V/povray (Failed)

Some tests failed because we have not install the run-time dependencies: Gmsh, Asymptote and POV-Ray (see :ref:`installing_neper`).

Installing Gmsh
---------------

Gmsh is available from the package manager (meaning that we could run :command:`sudo apt install gmsh` to install it); however, it is worth installing it from source since it will run much faster.

We download the source code from the website (https://gmsh.info), latest version.  It is then a good idea to move it to :file:`~/Progs`, where Neper already is:

.. code-block:: console

  $ mv gmsh-4.11.1-source.tgz ~/Progs/
  $ tar -zxf gmsh-4.11.1-source.tgz
  $ cd ~/Progs/gmsh-4.11.1-source

We can then compile it.  As Gmsh is also managed with CMake, installation is no much different from that of Neper itself:

.. code-block:: console

  $ mkdir build
  $ cd build
  $ cmake ..

  -- The CXX compiler identification is GNU 11.3.0
  -- The C compiler identification is GNU 11.3.0
  -- Detecting CXX compiler ABI info
  -- Detecting CXX compiler ABI info - done
  -- Check for working CXX compiler: /usr/bin/c++ - skipped
  -- Detecting CXX compile features
  -- Detecting CXX compile features - done
  -- Detecting C compiler ABI info
  -- Detecting C compiler ABI info - done
  -- Check for working C compiler: /usr/bin/cc - skipped
  -- Detecting C compile features
  -- Detecting C compile features - done
  -- Could NOT find Git (missing: GIT_EXECUTABLE)
  -- Looking for sys/types.h
  -- Looking for sys/types.h - found
  -- Looking for stdint.h
  -- Looking for stdint.h - found
  -- Looking for stddef.h
  -- Looking for stddef.h - found
  -- Check size of size_t
  -- Check size of size_t - done
  -- Found 64Bit
  -- Found OpenMP_C: -fopenmp (found version "4.5")
  -- Found OpenMP_CXX: -fopenmp (found version "4.5")
  -- Found OpenMP: TRUE (found version "4.5")
  -- Found OpenMP
  -- Found Eigen[contrib]
  -- Found Mesh
  -- Found Solver
  -- Found Post
  -- Found Plugins
  -- Found Parser
  -- Could NOT find OpenGL (missing: OPENGL_opengl_LIBRARY OPENGL_glx_LIBRARY OPENGL_INCLUDE_DIR)
  -- Could NOT find X11 (missing: X11_X11_INCLUDE_PATH X11_X11_LIB)
  -- Could NOT find FLTK (missing: FLTK_LIBRARIES FLTK_INCLUDE_DIR)
  -- Found ONELAB
  -- Found ONELABMetamodel
  -- Found ANN[contrib]
  -- Found ALGLIB[contrib]
  -- Found DIntegration
  -- Found Kbipack
  -- GMP not found: Kbipack uses long int
  -- Found MathEx[contrib]
  -- Found Metis[contrib]
  -- Found TetGen/BR
  -- Found Voro++[contrib]
  -- Found OptHom
  -- Found DomHex
  -- Found QuadTri
  -- Found Blossom
  -- Found Netgen
  -- Found Bamg
  -- Found Hxt
  -- Found QuadMeshingTools
  -- Found WinslowUntangler
  -- Could NOT find HDF5 (missing: HDF5_LIBRARIES HDF5_INCLUDE_DIRS) (found version "")
  -- HDF5 not found
  -- Found Gmm[contrib]
  -- Could NOT find Freetype (missing: FREETYPE_LIBRARY FREETYPE_INCLUDE_DIRS)
  -- Looking for vsnprintf
  -- Looking for vsnprintf - found
  -- Looking for sys/socket.h
  -- Looking for sys/socket.h - found
  -- Check size of socklen_t
  -- Check size of socklen_t - done
  -- Check size of intptr_t
  -- Check size of intptr_t - done
  -- Looking for dlfcn.h
  -- Looking for dlfcn.h - found
  -- Found Dlopen
  -- Looking for linux/joystick.h
  -- Looking for linux/joystick.h - found
  -- Found LinuxJoystick
  -- Performing Test WALL
  -- Performing Test WALL - Success
  -- Performing Test WCAST
  -- Performing Test WCAST - Failed
  -- Performing Test WDEPREC
  -- Performing Test WDEPREC - Success
  -- Performing Test WIND
  -- Performing Test WIND - Success
  -- Performing Test WATTR
  -- Performing Test WATTR - Success
  -- Performing Test NOWARN
  -- Performing Test NOWARN - Success
  -- Performing Test NOOPT
  -- Performing Test NOOPT - Success
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/api/square.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/baffles.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/boolean.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/chamfer.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/coherence.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/compsolid.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/compsolid2.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/extend_field.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/extrude.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/extrude2.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fillet.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fillet2.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fillet3.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fillet4.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fillet_chamfer.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fleur.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/fragment_numbering.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/hybrid_occ_builtin.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/hyperboloid.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/import.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/import2.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/intersect_line_volume.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/neuron.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/number_of_tets.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/periodic.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/periodic_embedded.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/pipe.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/primitives.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/revolve.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/revolve2.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/shell_sewing.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple2.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple3.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple4.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple5.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple6.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/simple7.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/slicer.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/slicer_surfaces.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/spherical_surf.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/spline.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/step_assembly.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/surface_filling.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/thicksolid.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/thrusections.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/transfinite.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/transform.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/boolean/twist.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/examples/post_processing/compute_area_volume.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/tutorials/t16.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/tutorials/t17.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/tutorials/t18.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/tutorials/t19.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/tutorials/t20.geo
  Skipping test /home/rquey/Progs/gmsh-4.11.1-source/tutorials/t21.geo
  --
  -- Gmsh 4.11.1 has been configured for Linux64
  --
  --  * Build options: 64Bit ALGLIB[contrib] ANN[contrib] Bamg Blossom DIntegration Dlopen DomHex Eigen[contrib] Gmm[contrib] Hxt Kbipack LinuxJoystick MathEx[contrib] Mesh Metis[contrib] Netgen ONELAB ONELABMetamodel OpenMP OptHom Parser Plugins Post QuadMeshingTools QuadTri Solver TetGen/BR Voro++[contrib] WinslowUntangler
  --  * Build type: RelWithDebInfo
  --  * C compiler: /usr/bin/cc
  --  * C++ compiler: /usr/bin/c++
  --  * Install prefix: /usr/local
  --
  -- Configuring done
  -- Generating done
  -- Build files have been written to: /home/rquey/Progs/gmsh-4.11.1-source/build

Configuration completed and is sufficient for Neper. So, we can make and install:

.. code-block:: console

  $ make
  $ sudo make install

The second command copies the :file:`gmsh` binary file to a system location, which makes the command available to Neper.

Back to Neper's build directory, we re-run the tests:

.. code-block:: console

  $ cd ~/Progs/neper-4.6.1/src/build
  $ ctest

  99% tests passed, 2 tests failed out of 461

  Total Test time (real) =  47.92 sec

  The following tests FAILED:
          211 - V/asymptote (Failed)
          325 - V/povray (Failed)
  Errors while running CTest

All "M" tests now pass.

.. note:: We could also re-run only the "M" tests, using :command:`ctest -R M`.

Installing POV-Ray and Asymptote
--------------------------------

We install povray as follows:

.. code-block:: console

  $ sudo apt install povray

We re-run the tests:

.. code-block:: console

  $ ctest

  99% tests passed, 1 tests failed out of 461

  Total Test time (real) =  47.92 sec

  The following tests FAILED:
          211 - V/asymptote (Failed)
  Errors while running CTest

.. note:: We could also simply run :code:`ctest -R V/povray`

We now install Asymptote, which itself required full Tex Live, as we know from :ref:`installing_neper`:

.. code-block:: console

  $ sudo apt install asymptote
  $ sudo apt install texlive-full

We re-run the tests:

.. code-block:: console

  $ ctest

  [...]

  100% tests passed, 0 tests failed out of 461

.. note:: We could also simply run :code:`ctest -R V/asymptote`


Depending on your use of Neper, it may be necessary to make the following changes in your :file:`/etc/ImageMagick-6/policy.xml` file:

  - Comment out the following lines:

    .. code-block:: console

      <policy domain="coder" rights="none" pattern="PS" />
      <policy domain="coder" rights="none" pattern="PS2" />
      <policy domain="coder" rights="none" pattern="PS3" />
      <policy domain="coder" rights="none" pattern="EPS" />
      <policy domain="coder" rights="none" pattern="PDF" />
      <policy domain="coder" rights="none" pattern="XPS" />

  - Increase the following limits (especially :data:`memory` and :data:`disk`):

    .. code-block:: console

      <policy domain="resource" name="memory" value="256MiB"/>
      <policy domain="resource" name="map" value="512MiB"/>
      <policy domain="resource" name="width" value="16KP"/>
      <policy domain="resource" name="height" value="16KP"/>
      <!-- <policy domain="resource" name="list-length" value="128"/> -->
      <policy domain="resource" name="area" value="128MP"/>
      <policy domain="resource" name="disk" value="1GiB"/>

The installation is now complete!

Improving Gmsh's Installation (optional)
----------------------------------------

Back to Gmsh, the Gmsh installation may be improved so that we can actually open Gmsh's GUI, by installing OpenGL's development package (OpenGL itself may already be installed) and FLTK:

.. code-block:: console

  $ sudo apt-get install freeglut3-dev
  $ sudo apt-get install libfltk1.3 libfltk1.3-dev
  $ cd ~/Progs/gmsh-4.11.1-source
  $ cmake ..
  $ make
  $ sudo make install

Installing Neper's documentation (optional)
-------------------------------------------

Neper's documentation is available online, but we can also build our own local copy:

From the :file:`doc` directory, we compile the documentation:

.. code-block:: console

  $ cd ~/Progs/neper-4.6.0/doc
  $ make html

  sphinx-build -b html -d build/doctrees   . build/html
  Traceback (most recent call last):
    File "/home/rquey/.local/bin/sphinx-build", line 5, in <module>
      from sphinx.cmd.build import main
  ModuleNotFoundError: No module named 'sphinx'
  make: *** [Makefile:53: html] Error 1

Sphinx is missing.  We install it:

.. code-block:: console

  $ sudo apt-get install python3-sphinx

We re-compile the documentation:

.. code-block:: console

  $ make html

  sphinx-build -b html -d build/doctrees   . build/html
  Running Sphinx v4.3.2
  loading translations [en]... done
  loading pickled environment... done
  WARNING: sphinx_rtd_theme (< 0.3.0) found. It will not be available since Sphinx-6.0
  building [mo]: targets for 0 po files that are out of date
  building [html]: targets for 11 source files that are out of date
  updating environment: [config changed ('version')] 11 added, 0 changed, 0 removed
  reading sources... [100%] versions
  /home/rquey/Progs/fepx/doc/copying.rst:24: WARNING: duplicate citation DAWSON15, other instance in /home/rquey/Progs/fepx/doc/develhistory.rst
  /home/rquey/Progs/fepx/doc/develhistory.rst:77: WARNING: duplicate citation DAWSON15, other instance in /home/rquey/Progs/fepx/doc/copying.rst
  /home/rquey/Progs/fepx/doc/input.rst:1020: WARNING: Explicit markup ends without a blank line; unexpected unindent.
  looking for now-outdated files... none found
  pickling environment... done
  checking consistency... done
  preparing documents... done
  writing output... [100%] versions
  /home/rquey/Progs/fepx/doc/versions.rst:7: WARNING: Pygments lexer name 'plain' is not known
  generating indices... genindex done
  writing additional pages... search done
  copying images... [100%] images/minimalbcs.png
  copying static files... done
  copying extra files... done
  dumping search index in English (code: en)... done
  dumping object inventory... done
  build succeeded, 5 warnings.

  The HTML pages are in build/html.

  Build finished. The HTML pages are in build/html.

All done!  We may now copy the :file:`html` directory to somewhere more appropriate (there is no :command:`make install` command for the documentation).
