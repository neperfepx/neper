.. _installation:

Installation
============

Neper is written in (mostly ANSI) C and a little C++, and it can run on any Unix-like system.  Neper is also known to run on macOS and on Microsoft Windows using the Windows subsystem for Linux (WSL), the Ubuntu app or similar solutions, although we neither test nor support this usage and multithreading must be turned off (which slows down execution significantly).

When installed, Neper consists of the :program:`neper` binary, which can be called from the command line, and data files, which are used by the binary.  At runtime, :program:`neper` looks for its data files in :code:`$NEPER_DATA_DIR`, :file:`../share/neper` relative to the directory of the :program:`neper` binary (so that an installed binary uses its own data files), :file:`$HOME/.local/share/neper` and :file:`\<prefix\>/share/neper`, successively, where :file:`\<prefix\>` is the installation prefix (:file:`/usr/local` by default, see below).  The data directory is printed by :command:`neper --data`.

Neper can be installed using conda, which is the simplest method (see :ref:`installing_conda`), or compiled from its source code (see :ref:`general_procedure`).

.. _installing_conda:

Conda Package
-------------

Neper is available as a conda package from `conda-forge <https://conda-forge.org>`_, for Linux and macOS.  No compilation and no administrator rights are needed.

If conda is not available on your system (command :program:`conda`), install it first, using `Miniforge <https://github.com/conda-forge/miniforge>`_:

.. code-block:: console

  $ curl -L -O "https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-$(uname)-$(uname -m).sh"
  $ bash Miniforge3-$(uname)-$(uname -m).sh

Accept the license and the default installation directory (:file:`$HOME/miniforge3`), and answer :data:`yes` to the last question, about initializing conda, so that :program:`conda` is available in new terminals.  Then, open a new terminal (or run :command:`source ~/.bashrc`, for Bash).  The terminal prompt then starts with :data:`(base)`, which is the default conda environment.  The installer file can be removed.

Then, install Neper in a new environment (named :data:`neper` here) and activate it:

.. code-block:: console

  $ conda create -n neper -c conda-forge neper
  $ conda activate neper

This also installs the required programs and libraries: Gmsh, POV-Ray, etc.  Asymptote and ImageMagick, which are needed only for some outputs of :ref:`neper_v` (see :ref:`fine_configuration`), are not installed.

Neper can then be run as :program:`neper` in the activated environment, and updated by:

.. code-block:: console

  $ conda update -n neper -c conda-forge neper

Neper and `FEPX <https://fepx.info>`_ can also be installed together, in the same environment (named :data:`neperfepx` here), to generate meshes with Neper, simulate them with FEPX and post-process the results with Neper:

.. code-block:: console

  $ conda create -n neperfepx -c conda-forge neper fepx
  $ conda activate neperfepx

.. _general_procedure:

Building from Source
--------------------

Compilation is performed via `CMake <https://cmake.org>`_ (no root privileges needed):

- Create a :file:`build` directory, for instance as a subdirectory of Neper's :file:`src` directory:

  .. code-block:: console

    $ cd src
    $ mkdir build

- Run CMake from within the :file:`build` directory, pointing to Neper's :file:`src` directory:

  .. code-block:: console

    $ cd build
    $ cmake ..    (on Windows: cmake -DHAVE_OPENMP=OFF ..)

- Build Neper:

  .. code-block:: console

    $ make

  Use option :data:`-j` for a multithreaded compilation.

This generates the :file:`neper` binary file.

To make Neper available system-wide, run (using :program:`sudo` for administration privileges):

.. code-block:: console

  $ sudo make install

To make Neper available locally to the user, run:

.. code-block:: console

  $ make userinstall

This procedure uses the default configuration options and should work out of the box if the required dependencies are available in standard system locations.

.. attention:: Since version 4.11, either :command:`make install` or :command:`make userinstall` must be run for proper installation.  Copying the :file:`neper` binary to :file:`~/bin` for a local installation is no longer sufficient.

.. _fine_configuration:

Fine Configuration
~~~~~~~~~~~~~~~~~~

The locations where files are installed, which dependencies are included, other dependency information and program configuration options can be specified, before installation, using CMake variables.  This can be done using a terminal tool:

.. code-block:: console

  $ ccmake ..

or an interactive tool:

.. code-block:: console

  $ cmake-gui ..

or directly at the command line, using CMake's :data:`-D` option:

.. code-block:: console

  $ cmake -D<VARIABLE1>=<VALUE1> -D<VARIABLE2>=<VALUE2> ..

The installation directory is :code:`CMAKE_INSTALL_PREFIX`, which defaults to :code:`/usr/local` for :command:`make install` and :file:`$HOME/.local` for :command:`make userinstall`.

.. - The Bash completion script directory (variable :code:`CMAKE_BUILD_DIRECTORY_COMPLETION`, default :file:`/usr/share` on Linux and :file:`/usr/local/etc` on macOS).

Other compilation options are :code:`CMAKE_BUILD_TYPE` and :code:`ENABLE_PROFILING`, which should not be used for production.

The dependencies that are needed at compilation time, and must therefore be installed on your system before Neper is compiled, are the following:

- The `OpenMP <https://www.openmp.org/>`_ library (optional but highly recommended, enabled by default, variable :code:`HAVE_OPENMP`).  It is likely to be available on your system or from your system package manager. [#mac-install]_

- The pthread library (mandatory). It is likely to be available on your system or from your system package manager.

- The `Gmsh <https://gmsh.info>`_ library (version 4.9 or higher, mandatory).  It is used for meshing (module -M).  It is likely to be available from your system package manager (package :code:`libgmsh-dev` on Debian and Ubuntu); alternatively, it is provided by the Gmsh SDK, or it can be built from the Gmsh source code.  If it is not installed in a standard location, its installation directory can be specified using :code:`CMAKE_PREFIX_PATH`.

  To build the Gmsh library from the Gmsh source code, Gmsh must be configured with :code:`-DENABLE_BUILD_DYNAMIC=1` (which builds the shared library, :file:`libgmsh.so`, in addition to the Gmsh program), and then installed with :code:`make install` (which installs the library and its header files, including :file:`gmshc.h`, used by Neper):

  .. code-block:: console

    $ cd gmsh-<version>-source
    $ mkdir build
    $ cd build
    $ cmake -DENABLE_BUILD_DYNAMIC=1 ..
    $ make
    $ make install

  To install Gmsh in a non-standard location, add :code:`-DCMAKE_INSTALL_PREFIX=<directory>` to the :program:`cmake` command, pass :code:`-DCMAKE_PREFIX_PATH=<directory>` when configuring Neper, and make the library available at run time (for example, by adding its library directory to :code:`LD_LIBRARY_PATH`).

Other dependencies are only needed at runtime (they are not necessary for compilation):

- The `POV-Ray <http://www.povray.org>`_ program (version 3.7 or higher, mandatory for PNG output in :ref:`neper_v`).  It is likely to be available on your system or from your system package manager (binary package).  Alternatively, both binary and source-code versions can be downloaded from the program website.  POV-Ray must be available at the terminal as the command :program:`povray`, or the path to its binary must be specified with option :option:`-povray` (in module -V).

- The `Asymptote <https://asymptote.sourceforge.io/>`_ program and `TeX Live <https://www.tug.org/texlive>`_ (:code:`texlive-full` on Ubuntu, for pole figure plots in :ref:`neper_v`). They are likely to be available from your system package manager.  Alternatively, both binary and source-code versions can be downloaded from the program websites.  Asymptote must be available at the terminal as the command :program:`asy`, or the path to its binary must be specified with option :option:`-asymptote` (in module -V).

- The `ImageMagick <https://imagemagick.org>`_ program (mandatory for PNG output of orientation spaces in :ref:`neper_v`, :option:`-space` :data:`rodrigues`).  It is likely to be available on your system or from your system package manager.  ImageMagick must be available at the terminal as the command :program:`convert`.

  .. attention:: Depending on your configuration and your use of Neper, it may be necessary to make the following changes in your :file:`/etc/ImageMagick-6/policy.xml` file:

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

Finally, other third-party libraries are directly included in the source code (see directory :file:`src/contrib`) and are therefore not dependencies *per se*:

- The `GNU Scientific Library <https://www.gnu.org/software/gsl>`_ (GSL). If it is already available on your system, the system version will be used instead.  To force the use of the built-in version, set :code:`FORCE_BUILTIN_GSL` to :code:`ON` (default :code:`OFF`).

  .. note:: The built-in GSL version is 2.7.1 (subset).

- The `libscotch <https://www.labri.fr/perso/pelegrin/scotch>`_ library. If it is already available on your system, the system version will be used instead.  To force the use of the built-in version, set :code:`FORCE_BUILTIN_LIBSCOTCH` to :code:`ON` (default :code:`OFF`).

- The `tinycolormap <https://github.com/yuki-koyama/tinycolormap>`_ library.

- The `nanoflann <https://github.com/jlblancoc/nanoflann>`_ library.

- The `muparser <https://beltoforion.de/en/muparser>`_ library.

- The `openGJK <https://github.com/MattiaMontanari/openGJK>`_ library.

The program configuration variables concern the printing format of the real numbers in the result files (:file:`.tess`, :file:`.msh`, etc.):

- :code:`REAL_PRINT_FORMAT`, default :code:`"%.12f"` (12 decimal digits);
- :code:`REAL_PRINT_FORMAT3`, default :code:`"%15.12f"` (12 decimal digits, 15 total digits);
- :code:`REAL_PRINT_FORMAT5`, default :code:`"%17.12f"` (12 decimal digits, 17 total digits).

.. _testing_neper:

Testing
~~~~~~~

Neper comes packaged with tests and reference outputs. To run the tests, execute the following from your build folder:

.. code-block:: console

  $ make test

or (equivalently):

.. code-block:: console

  $ ctest

This runs the tests in :code:`Normal` mode (:code:`Minimal` on macOS), for which the produced output files are compared to reference ones. The (packaged) reference output files are generated on Ubuntu 20.04, using compiler GCC 9.3.0, (built-in) libscotch 6.0, Gmsh 4.9.4 and POV-Ray 3.7.0. It is expected that different versions may result in minor (insignificant) changes to the output, though this will generally result in failed tests.  If this happens, change the testing mode to :code:`Minimal`.  The testing mode is controlled by variable :code:`BUILD_TESTING_MODE`, which may be changed as described in :ref:`fine_configuration`.

- The :code:`Normal` mode checks if the program completes without error and if the produced output is the same as the reference output.  It is the default mode on Linux.

- The :code:`Minimal` mode only checks if the program completes without error. This mode may be useful when installing on a machine which has program or library versions different from the ones with which the reference output was generated.  It is the default mode on macOS.

- The :code:`Writing` mode overwrites the reference outputs with the generated output.  This mode may be useful when installing on a machine which has program or library versions different from the ones with which the reference output was generated and the user needs a reference output before making changes to the source code.

.. _orilib_program:

Orilib Program
--------------

Neper's library of orientation functions, Orilib, also comes as a program, :program:`orilib`, which makes it possible to run its functions from the command line, for example to convert orientations between descriptors or to compute disorientations.  It is built and installed together with :program:`neper` (it can also be built alone, using :command:`make orilib`).  Its functions are also available in Python (see :ref:`python_orilib`).

The program can be run in three ways:

- :command:`orilib` starts an interactive session, in which a command is typed, then its input data (as requested), and so on, until :data:`quit` is typed.  :data:`listall` lists the commands, :data:`signatures` lists their inputs and outputs (with their types) and :data:`help` prints help;

- :command:`orilib <command>` runs a single command, reading its input data from the standard input and writing its output to the standard output (with no other information), which makes it convenient for scripting, for example:

  .. code-block:: console

      $ echo 0 30 0 | orilib ol_e_q
       0.965925826  0.258819045  0.000000000  0.000000000

- :command:`orilib loop <command>` runs a command repeatedly, on all the input data.

The real output values are written with 9 decimals, and their format can be changed using environment variable :code:`ORILIB_FORMAT`, e.g. :code:`ORILIB_FORMAT=%.17g` for full precision.

The commands are named after the Orilib functions, which follow the pattern :data:`ol_<input>_<output>`, where the descriptors are abbreviated as :data:`e` (Euler angles, Bunge convention), :data:`g` (rotation matrix), :data:`q` (quaternion), :data:`m` (Miller indices), :data:`rtheta` (rotation axis / angle pair), :data:`R` (Rodrigues vector) and :data:`homochoric` (homochoric vector), and angles are in degrees (or in radians with the :data:`_rad` suffix).  The main families of commands are:

.. list-table::
   :header-rows: 1

   * - Commands
     - Description
   * - ``ol_e_g``, ``ol_q_R``, etc.
     - conversions between orientation descriptors
   * - ``ol_g_g_g``, ``ol_q_inverse``, etc.
     - combination and inversion of rotations
   * - ``ol_q_crysym``, ``ol_q_qcrysym``, etc.
     - crystallographically related and preferred orientations
   * - ``ol_q_q_misori``, ``ol_q_q_disori``, etc.
     - misorientations and disorientations (angles or rotations)
   * - ``ol_q_refsym``, etc.
     - symmetries with respect to the sample (orthotropic, etc.)
   * - ``ol_q_pf_stprojxy``, etc.
     - pole figure and inverse pole figure projections
   * - ``ol_nb_e``, ``ol_nb_max_rtheta``, etc.
     - generation of orientations from random numbers
   * - ``ol_label_q``, ``ol_q_des``, etc.
     - orientations given as labels or descriptor names

The complete list of commands is provided in :ref:`orilib_commands` (it can also be printed using :command:`orilib listall`).

.. _installing_python:

Python Interface
----------------

The Python interface is the :code:`neper` Python package, provided in the :file:`python` directory of the distribution.  It requires Python 3.8 or higher and Neper itself (installed as described above), and is installed using pip, from the top-level directory of the distribution:

.. code-block:: console

  $ pip install ./python

This also installs NumPy, if needed.  See :ref:`python_interface` for details.

.. [#mac-install] On macOS, install :program:`llvm` and :program:`libomp` via Homebrew.
