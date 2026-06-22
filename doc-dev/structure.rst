.. _structure:

Source Code
===========

.. admonition:: Prerequisites

  The reader is assumed to be familiar with the C programming language. [#canteaut]_

Neper is primarily written in C, with a small number of C++ files used to interface with external C++ libraries. The codebase contains a large number of functions organized in a highly hierarchical directory structure. This organization results from the fact that Neper itself is composed of several distinct modules, but also from a `modular programming <https://en.wikipedia.org/wiki/Modular_programming>`_ approach.  Neper also relies on a number of external libraries, some of which are included within the source code, while others are build-time or run-time dependencies.  Neper's source code is managed with `CMake <https://cmake.org>`_.

Neper's hierarchical (or modular) structure makes it possible to navigate the codebase efficiently and to add new capabilities relatively easily.  Neper is a mature project with more than 160,000 lines of source code. So, when implementing a new feature that requires specific operations, keep in mind that these operations are *very likely* already implemented (as functions) somewhere in the codebase. The following sections provide information and tools to help you locate and use them.

General Structure
-----------------

Neper's source code is located in the :file:`src/` directory and is structured as follows (directories are indicated by a :file:`/`):

.. code-block:: text

   neper.c           \
   neper_.h           |
   neper_t/           |
   neper_m/           | Source code files and directories
   neper_v/           |
   neper_s/           |
   neut/              |
   contrib/          /
   cmake/            \
   CMakeLists.txt     | CMake files and directories
   neper_config.h.in /
   data/              > Data directory

These files and directories can be divided into actual *source code*, *CMake files* and *data*, as indicated above. These files and directories are as follows:

- The :file:`neper.c` file contains the :code:`main` function of the program.  This function operates according to the arguments provided to the program, for example by passing the arguments to the function specific to the Neper module that was invoked.
- The :file:`neper_.h` file is the main header file and is #included by :file:`neper.c`. It gathers all the headers required by that file. This includes standard library headers, as well as project files that define the data structures [#struct]_ and declare the function prototypes required by :file:`neper.c`.
- The :file:`neper_t/`, :file:`neper_m/`, :file:`neper_s/` and :file:`neper_v/` directories contain the code specific to *Neper's modules* (:code:`-T`, :code:`-M`, :code:`-S` and :code:`-V`).
  *The code located in one module is used only by this module.*
- The :file:`neut/` directory contains Neper’s utility functions. The name "neut" combines "ne" (for Neper) and "ut" (for utility). These functions can be used either internally within the directory or by the functions of Neper's modules (located in :file:`neper_t/`, :file:`neper_m/`, :file:`neper_s/` and :file:`neper_v/`).
- The :file:`contrib/` directory contains *contributed code* that is not specific to Neper.  It does not use any functions that would be defined outside the :file:`contrib/` directory.  It is composed of the following directories:

  .. code-block:: text

    ut/
    orilib/
    muparser/
    nanoflann/
    nlopt/
    openGJK/
    scotch/
    tinycolormap/
    bashcomp/

  Among them, :code:`ut/` and :code:`orilib/` are "internal":

  - The :file:`ut/` directory contains C utilities.
  - The :file:`orilib/` directory contains the Orilib library for orientation manipulation.

  Both of these libraries have their own documentation, and new functions can occasionally be added to them.  Other libraries are third-party libraries and should generally not be modified.

- The :file:`CMakeLists.txt` file is CMake's input file.
- The :file:`cmake/` directory contains additional files used by CMake to find and manage external dependencies.
- The :file:`neper_config.h.in` file is a CMake template file that is converted into an actual Neper header file upon configuration.  It defines C macros describing the status of dependencies, as well as the Neper version and other useful configuration values.

- The :file:`data/` directory contains *Neper's data files*, which (once installed) are used by the :program:`neper` binary or are part of Neper's :ref:`devenv`.

.. note ::

    - Due to the highly modular organization of the codebase, :file:`neut/` accounts for about 50% of the total source code, and :file:`ut/` for about 20%.
.. - While :code:`neut/` should not use any functions defined in the module directories (:code:`neper_t/`, :code:`neper_m/`, :code:`neper_s/` and :code:`neper_v/`), exceptions exist.  Similarly, while the module directories should not use any functions defined in the other module directories, exceptions exist.

Module Directories (:file:`neper_t/`, etc.)
-------------------------------------------

The source directory of a particular module contains several root files and directories.  For example, :file:`neper_t/` contains the following files and directories:

.. code-block:: text

	neper_t.c
	neper_t.h
	neper_t_.h
	net_dim/
	net_domain/
	net_flatten/
	net_group/
	net_input/
	net_ori/
	net_reg/
	net_res/
	net_sort/
	net_stat/
	net_tess/
	net_transform/
	net_utils/
	CMakeLists.txt

- The :file:`neper_t.c` file contains the :code:`neper_t` function, which is Neper -T's main function.  This function is called by the :code:`main` program (from :file:`neper.c`), and it calls functions located in the directories located at the same level as the :file:`neper_t.c` file (as well as functions from the utility directories).

- The :file:`neper_t_.h` and :file:`neper_t.h` are the header files associated to :file:`neper_t.c`:

  - :file:`neper_t_.h` is #included by :file:`neper_t.c` and gathers all the headers required by that source file. This includes standard library headers, as well as project that define the data structures and declare the function prototypes required by :file:`neper_t.c`.

  - :file:`neper_t.h` is the header file associated to :file:`neper_t.c` and contains the prototype of :code:`neper_t`. It is #included by :file:`neper_.h` (as the :code:`neper_t` function is called by the :code:`main` program).

- The :file:`net_*/` directories contain source code corresponding to specific capabilities.

- The :file:`CMakeLists.txt` file is the directory CMake input file.

The other module directories are structured similarly.

.. _neut:

Utility Directory (:file:`neut/`)
---------------------------------

The Neper utility directory contains several files and directories:

.. code-block:: text

	neut.h
	neut_t.h
	neut_m.h
	neut_v.h
	neut_struct/
	neut_tess/
	neut_tesr/
	neut_mesh/
	neut_nodes/
	neut_nset/
	neut_ori/
	neut_topt/
	[...]
	CMakeLists.txt

- The :file:`neut.h` header file includes the main header files of all subdirectories. These headers may themselves include additional header files, ultimately providing access to the prototypes of the functions implemented throughout the directory hierarchy.

- The :file:`neut_t.h`, :file:`neut_m.h` and :file:`neut_v.h` header files include the main header files of only the subdirectories used by the corresponding Neper module.  It is #included by the different modules.

- The :file:`neut_struct/` directory contains header files defining the data structures used by Neper, see :ref:`datastructures`.

- The other :file:`neut_*/` directories contain source code corresponding to specific capabilities and, very often, specific data structures.  For example, :file:`neut_tess/` contains all functions that operate on a :code:`struct TESS`.

- The :file:`CMakeLists.txt` file is the directory CMake input file.

.. _datastructures:

Data structures (:file:`neut_struct/`)
---------------------------------------

Neper relies extensively on data structures to describe its core objects and specific operations. These structures are defined in the :file:`neut/neut_struct/` directory:

.. code-block:: text

	neut_struct_tess.h
	neut_struct_tesr.h
	neut_struct_nodes.h
	neut_struct_mesh.h
	neut_struct_nset.h
	neut_struct_sim.h
	neut_struct_topt.h
	[...]

The data structures that are also written in output files and so directly interface with the user are the following:

- :code:`struct TESS`: the (scalar / vectorial) tessellation, as defined in the :file:`.tess` file.
- :code:`struct TESR`: the (raster) tessellation, as defined in the :file:`.tesr` file.
- :code:`struct NODES`, :code:`struct MESH`, :code:`struct NSET`    : the mesh, as defined in the :file:`.msh` file.
- :code:`struct SIM`: the sample directory, as defined in the :file:`.sim` directory.

Other structures generally correspond to specific operations (:code:`struct TOPT` for tessellation optimization, :code:`struct FLATTEN` for flattening, :code:`struct REG` for regularization, etc.).  All structure names are written in uppercase.

Hierarchical Structure and Naming
---------------------------------

The codebase is structured so that a function defined in a specific file may call functions defined at either the following locations:

  - Lower-level directories

    For example, the :code:`main` program, located in :file:`src/neper.c`, calls the :code:`neper_t` function, located in the :file:`src/neper_t/` directory (specifically, in :file:`src/neper_t/neper_t.c`).

  - Lower-level files

    It is sometimes not appropriate to write functions called by a (primary) function in a new directory, and using only a new file feels more suitable.  In that case, the filenames become numbered, as in :file:`src/neper_t/net_tess/`, which contains files :file:`net_tess1.c` and :file:`net_tess2.c`.  :file:`net_tess2.c` has a lower level than :file:`net_tess1.c` and contains functions defined in it.  Note that :file:`src/neper_t/net_tess/` also contains directories (:file:`src/neper_t/net_tess/net_tess_opt/`, etc.) for functions that do need their own directory.

  - Utility directories

    These include :code:`neut/` and :code:`contrib/`.

The functions are named after the file to which they belong.  For example, :file:`src/neper_t/neper_t.c` contains only the function :code:`neper_t`. File :file:`src/neper_t/net_tess1.c` contains the :code:`net_tess` function, while :file:`src/neper_t/net_tess2.c` contains the :code:`net_tess_perdomain` and :code:`net_tess_3dto2d` functions (where a ":code:`_`" level is added because these functions are defined in a lower-level file).

CMake Structure
---------------

CMake is used as a build system to manage and organize the source code. Configuration files named :file:`CMakeLists.txt` are distributed throughout the project directory structure, including the root :file:`src/` directory and the module directories :code:`src/neper_*/` and several :file:`contrib/` components. These files define how the source files are compiled, linked, and organized into executables or libraries. By relying on CMake, the project benefits from a portable and modular build configuration, allowing dependencies and compilation options to be handled consistently across different platforms and development environments.

The project uses CMake to manage the build process and organize the source code. Each component (e.g., :code:`neper_t/`, :code:`neper_m/`, :code:`neper_s/`, :code:`neper_v/`, :code:`neut/` and others) contains its own :file:`CMakeLists.txt` file, which defines the compilation rules for that component. In practice, each of these configuration files builds a shared library corresponding to its component, and these libraries are then linked together to produce the final :program:`neper` executable.

The CMake setup also allows to manage the build options and detected dependencies used during compilation:

- :code:`BUILD_TESTING_MODE`: Testing mode, which can be :code:`Normal`, :code:`Minimal` or :code:`Writing`
- :code:`CMAKE_BUILD_TYPE`: Build type, which can be :code:`Debug` or :code:`Release`
- :code:`CMAKE_INSTALL_COMPLETION`: Switch for command completion installation, which can be :code:`ON` or :code:`OFF`
- :code:`CMAKE_INSTALL_PREFIX`: Installation prefix, e.g. :file:`/usr/local`
- :code:`CMAKE_INSTALL_TYPE`: Installation type, which can be :code:`Full` or :code:`NoPost`
- :code:`ENABLE_OPENMP`: Switch to enable openMP, which can be :code:`ON` or :code:`OFF`
- :code:`FORCE_BUILTIN_LIBSCOTCH`: Switch to force use built-in libscotch, which can be :code:`ON` or :code:`OFF`
- :code:`FORCE_BUILTIN_NLOPT`: Switch to force use built-in NLopt, which can be :code:`ON` or :code:`OFF`
- :code:`HAVE_GSL`: Switch for the GSL dependency, which can be :code:`ON` or :code:`OFF`
- :code:`HAVE_LIBSCOTCH`: Switch for the GSL dependency, which can be :code:`ON` or :code:`OFF`
- :code:`HAVE_MUPARSER`: Switch for the GSL dependency, which can be :code:`ON` or :code:`OFF`
- :code:`HAVE_NLOPT`: Switch for the GSL dependency, which can be :code:`ON` or :code:`OFF`
- :code:`HAVE_OPENGJK`: Switch for the GSL dependency, which can be :code:`ON` or :code:`OFF`
- :code:`HAVE_OPENMP`: Switch for the GSL dependency, which can be :code:`ON` or :code:`OFF`
- :code:`MATHLIB`: Path to the math library
- :code:`REAL_PRINT_FORMAT`: Real print format, default :code:`"%.12f"`
- :code:`REAL_PRINT_FORMAT3`: Other real print format, default :code:`"%15.12f"`
- :code:`REAL_PRINT_FORMAT5`: Other real print format, default :code:`"%17.12f"`
- :code:`scotch_LIBRARY`: Path to the scotch library
- :code:`scotcherr_LIBRARY`: Path to the scotch err library
- :code:`scotcherrexit_LIBRARY`: Path to the scotch errexit library

For development, :code:`CMAKE_BUILD_TYPE` must be set to :code:`Debug` using :command:`cmake -DCMAKE_BUILD_TYPE=Debug ..` (from the :file:`build/` directory).

Coding Syntax
-------------

The source code follows the `GNU C Coding Standards <https://www.gnu.org/prep/standards/html_node/Writing-C.html>`_ with a few exceptions.  You must comply to these rules when adding new code.  Here are examples of well-formatted pieces of code:

.. code-block :: C

  extern int net_tess (struct IN_T In, int level, struct TESS *Tess, int tess,
                       int cell, struct SEEDSET *SSet, int TessId, struct MTESS *pMTess);


.. code-block :: C

  int
  net_tess (struct IN_T In, int level, struct TESS *Tess, int dtess, int dcell,
            struct SEEDSET *SSet, int TessId, struct MTESS *pMTess)
  {
    int status = -1, periodic;
    int dim = (In.levelqty > 1 && In.dim == 2) ? 3 : In.dim;
    struct TESS Dom;
    char *morpho = NULL, *ori = NULL;

    if (neut_tess_isreg (Tess[dtess]))
    {
      printf ("\n");
      ut_print_message (2, 2, "Parent tessellation is regularized.\n");
    }

    neut_tess_set_zero (&Dom);

    [...]

.. code-block :: C

  int
  ut_num_requal (double n1, double n2, double eps)
  {
    if (n1 == 0 && n2 == 0)
      return 1;
    else if (ut_num_equal (2 * fabs (n1 - n2) / (n1 + n2), 0, eps))
      return 1;
    else
      return 0;
  }

.. code-block :: C

  void
  ut_space_bary_2d (double **P, int qty, double *B)
  {
    int i, j;
    double tmp;

    for (i = 0; i < 2; i++)
    {
      tmp = 0;
      for (j = 0; j < qty; j++)
        tmp += P[j][i];

      B[i] = tmp / (double) qty;
    }

    return;
  }

The following rules apply to all source files:

- Indentation

  - Indentation is 2 spaces per level.
  - Tabs are not used.
  - Each nested block increases indentation by 2 spaces.

- Function Declarations and Definitions

  - The return type appears on its own line.
  - The function name starts at column 0.
  - The opening brace is placed on a new line.

- Spacing Before Parentheses

  - A space precedes parentheses in

    - Function definitions
    - Function calls
    - Control statements

- Braces

  - Opening braces are placed on their own lines.
  - Closing braces are aligned with the opening construct.
  - Braces are omitted for single statements.
  - Multi-statement blocks must use braces.

- Variables

    A variable name tends to be preferably lower-cased and uses the :code:`_` separator (:code:`beg_time`).  However, some (older) parts of the code contain capitalized variable names (:code:`BegTime`).

- Pointer Notation

  - At definition, the :code:`*` binds to the variable name, not the type.
  - A pointer to a variable is always named after the variable and prefixed with :code:`p` (:code:`pbeg_time = &beg_time`).

- Operators

  - Binary operators are surrounded by spaces.
  - Unary operators are not followed by a space.
  - No spaces are used inside array brackets.

- Return Statements

  - A return is always done, even for functions of void type.
  - A return should generally not be done before the end of the function.
  - A space follows return.
  - The returned expression follows standard spacing rules.

- Line Length

  - Source lines should not exceed approximately 120 characters.

- Empty Lines

  - Empty lines should be used to separate meaningful chunks of code.
  - No more than 1 empty line should be used (no successive lines).

- Functions

  - The names of the function reflect their purpose.  For example, :code:`net_tess_opt` performs tessellation optimization.
  - Function arguments are ordered with inputs first and outputs last.  Most functions therefore return values via *arguments* (pointers), particularly for complex operations.  The function may also return a status as a return *value*.

    .. code :: C
  
      int
      net_tess (struct IN_T In, int level, struct TESS *Tess, int dtess, int dcell,
                struct SEEDSET *SSet, int TessId, struct MTESS *pMTess)
      {
        [...]
  
        return status;
      }
  
    At the opposite, :code:`ut_num_max` returns its result directly as a value:
  
    .. code :: C
  
      double
      ut_num_max (double num1, double num2)
      {
        return ((num1 > num2) ? num1 : num2);
      }

.. [#canteaut] French readers unfamiliar with C can refer to `"Programmation en language C" by Anne Canteaut <https://www.rocq.inria.fr/secret/Anne.Canteaut/COURS_C>`_.

.. [#struct] Data structures are C structures.

Version Numbers
---------------

Version numbers are defined in a standard way, as :data:`<major>.\<minor\>.\<patch\>` (see https://semver.org for a thorough discussion).  Typically, patch versions (:data:`x.x.1`, :data:`x.x.2`, etc.) gather bug fixes and small improvements, minor versions (:data:`x.1.0`, :data:`x.2.0`, etc.) contain new features, and major versions (:data:`1.0.0`, :data:`2.0.0`, etc.) are little revolutions, such as the addition of outstanding new features or global code overhaul.  In the special circumstances where backward incompatibility is introduced, the new version should be either a minor or a major version. A new version number is automatically created on each commit (see :ref:`workflow`).  When not associated to a release, a version number is formatted as :data:`<major>.\<minor\>.\<patch\>-\<releasecandidate\>` when in the :data:`main` branch, and :data:`<major>.\<minor\>.\<patch\>-\<branch_name\>-\<releasecandidate\>` when in a development branch (either :data:`devel` or a new-feature branch), where :data:`patch` is the *next* patch number and :data:`releasecandidate` increments from 1 (whether the next release is a patch or minor or major release is generally not known in advance, and it will always be considered that it is a patch release - this is generally not problematic, as the new version number will be defined at release). For example, considering only :data:`main`, the versions immediately following :data:`1.0.0` are :data:`1.0.1-1` ("release candidate for :data:`1.0.1`"), :data:`1.0.1-2`, etc. and will eventually be part of :data:`1.0.1` (or :data:`1.1.0` if the next release happens to be a minor release, or :data:`2.0.0` for a major release). On an official release, the version number is prefixed :data:`v` to create a Git tag and GitHub release.

.. Version numbering is automatically handled by `neperfepx-git <https://github.com/neperfepx/neperfepx/tree/main/neperfepx-git>`_, described below.
