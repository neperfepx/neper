.. _devenv:

Development Environment
=======================

.. admonition:: Prerequisites

  The reader is assumed to be familiar with the Linux operating system and the command-line interface.

You are free to use whichever development environment you prefer for Neper. However, we use and promote a terminal-based, composable toolchain built around Vim for text editing, Tmux for terminal multiplexing, Git for version control, GDB and Valgrind for debugging, and standard shell utilities for searching and navigating the codebase. Neper has always been developed within this environment, and so certain workflows and conventions in the code may naturally reflect this setup.
The aim is to ensure complete flexibility, control, and transparency, enabling full understanding and manipulation of every aspect of the workflow. Although this approach may involve a steeper learning curve than using an IDE, the investment pays off because each tool has value beyond software development.  Vim, for example, can be used for writing a thesis or editing a configuration file (including on servers).  Git is just as effective for versioning a thesis or research paper as it is for managing source code, and command-line proficiency applies across servers and automation tasks.

Installation
------------

Although each tool could be installed and set up independently, we provide a command to facilitate the process. To install Neper's development environment, run

.. code :: bash

   $ make devinstall

which has the following effects:

  - It provides you with commands to put in your :file:`~/.bashrc`
  - It provides you with commands to put in your :file:`~/.tmux.conf`
  - It makes available the :ref:`commands`

Note that it *does not* install :program:`git` (and :program:`gitg`), :program:`vim`, :program:`tmux`, nor the compiler (:program:`gcc` and the associated tools, :program:`gdb` and :program:`valgrind`).  So, make sure these commands are also available on your system.

Environment Variables
---------------------

A few environment variables are defined to set up paths to different parts of the Neper installation, so that scripts, tools, or the shell can easily locate them.  You must adapt :code:`NEPERROOT` to your own setup.

.. code :: bash

  export NEPERROOT=$PWD/neper
  export NEPER=$NEPERROOT/src
  export NEPERDOC=$NEPERROOT/doc
  export NEPERDOCDEV=$NEPERROOT/doc-dev
  export FEPXROOT=""                    # must be defined.  An empty value works.

Tmux Layout (:command:`neperdev`)
---------------------------------

Neper's default development Tmux layout can be setup by running the :command:`neperdev` command from a (preferably) full-screen terminal [#nesting]_.  The session is divided into three panes to support an efficient edit-build workflow. The left pane occupies the entire left half of the screen and serves as the primary coding window, where source files are edited. The right half of the screen is split horizontally into two panes, where the upper part functions as a secondary coding window, typically used to view related source files. The lower-right pane is dedicated to compilation and build commands (e.g., :program:`make`), allowing compiler output and error messages to remain visible while editing. This layout enables continuous feedback during development and minimizes context switching between editing and building.

.. figure :: imgs/tmux-layout.png

   Neper's default development Tmux layout (font size enlarged for clarity).

The commands to switch and manage panes are the following:

  - Move left: Ctrl + Left arrow
  - Move right: Ctrl + Right arrow
  - Move up: Ctrl + Up arrow
  - Move down: Ctrl + Down arrow
  - Toggle to last active pane: Ctrl + p
  - Close a pane: :command:`exit`
  - Zoom a pane in or out: Ctrl-b + z
  - Enlarge a pane horizontally: Ctrl-b + Left arrow or Right arrow
  - Enlarge a pane vertically: Ctrl-b + Up arrow or Down arrow
  - Split a pane vertically: Ctrl-b + |
  - Split a pane horizontally: Ctrl-b + -
  - Scroll up or down inside a pane: Ctrl-b + (, and then Up arrow, Down arrow, Page up, Page down or mouse wheel

Terminal Prompt Symbol
----------------------

The hostname, current directory and Git branch are printed in color before the prompt symbol (:data:`$`) in the terminal:

.. code-block:: bash

  <hostname>:<current_directory> [devel] $

.. _commands:

Custom Commands
---------------

:command:`vic`
~~~~~~~~~~~~~~

We use Vim through a custom :command:`vic` command that streamlines navigation and editing within the project. The purpose of :program:`vic` is to open all source and header files located in the same directory as a given file, allowing related implementation and interface files to be edited simultaneously.  :program:`vic` locates the target directory and lanches Vim to open all relevant source and header files.  For example, invoking :command:`vic neper.c` (from anywhere inside the codebase) opens both :file:`neper_.h` and :file:`neper.c`. Similarly, invoking :command:`vic net_ori1.c` opens :file:`net_ori.h`, :file:`net_ori_.h`, :file:`net_ori1.c`, :file:`net_ori2.c` and :file:`net_ori3.c`. It is then possible to navigate through the files using Vim's :code:`:n` and :code:`:N` commands. This approach is bounded to the project's hierarchical structure and the fact that development typically involves working on the entire set of files rather than a single one.

:command:`grepc`
~~~~~~~~~~~~~~~~

To facilitate efficient code exploration, we use the :command:`grepc` custom command. This function acts as a focused wrapper around :command:`grep`, restricting searches to the project’s C and C++ source files and thereby avoiding unrelated files in the repository.
Matches are displayed with filenames and color highlighting to improve readability. By distinguishing likely definitions from general occurrences and limiting the scope to relevant source files, :command:`grepc` enables fast and systematic navigation of the codebase directly from the terminal.  Here is an example:

.. code :: bash

  $ grepc "\<neper_v\>"
  ./neper_v/neper_v.c:neper_v (int fargc, char **fargv, int argc, char **argv)
  ./neper.c:      neper_v (fargc, fargv, argc - 1 - arg_pos, argv + 1 + arg_pos);
  ./neper_v/neper_v_.h:#include"neper_v.h"
  ./neper_v/neper_v.h:  extern int neper_v (int, char **, int, char **);
  ./neper_.h:#include"neper_v/neper_v.h"

:command:`cdc`
~~~~~~~~~~~~~~

We use the :command:`cdc` command to quickly change to the directory that contains a given file within the Neper source tree. :command:`cdc` allows to jump directly to the location of a source file without manually navigating the directory structure.  For example :command:`cdc neper_v/neper_v.c` (where the filename can easily be copied from the :command:`grepc` output) jumps to :file:`neper_v/`. As a note, :command:`cdc` does not need to be used for simple code editing, as :command:`vic` opens files without the need to navigate to their containing directories.

:command:`m`, :command:`mm`, :command:`mmm`, etc.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We also use the :command:`m` set of commands to quickly navigate up multiple directories in a filesystem hierarchy, while also showing the current location and contents of the directory. :command:`m` goes up 1 directory, print path and list files.  Each additional "m" adds one more level up.

.. code :: bash

  net_tess_opt [devel] $ pwd
  /home/rquey/Progs/neper/neper/src/neper_t/net_tess/net_tess_opt
  src [devel] $ mmm
  /home/rquey/Progs/neper/neper/src
  cmake  CMakeLists.txt  contrib  data  neper.c  neper_config.h.in  neper_.h  neper_m  neper_s  neper_t  neper_v  neut

:command:`git`
~~~~~~~~~~~~~~

The system :command:`git` command is replaced by a call to :program:`$HOME/.local/bin/nepertools/neperfepx-git` (through an alias).  The command behaves as the system :command:`git` but runs extra commands specific to Neper (and FEPX) development.  When the argument is :command:`commit` or :command:`merge`, the script also updates the version number and (for :command:`merge`) helps to resolve merge conflicts. When the command is :command:`mergepr` (which is specific to the new :command:`git`), the script merges the remote branch associated to the pull request.  The command automatically detects whether it is run from within a Neper (or an FEPX) repository, and behaves accordingly. If it is run from outside of a Neper (or FEPX) repository, it behaves as the system :command:`git` command. It can therefore be used safely anywhere on the system.

The three following commands have special behaviors.  All other commands (:option:`push`, :option:`pull` etc.) work as standard Git commands.

git commit
^^^^^^^^^^

What :command:`git commit` does (chronologically) is to

- Create the commit
- Update the version number
- Open the :file:`VERSIONS` file for your to complete
- Update the commit to include the updated version number and :file:`VERSIONS` file

git merge
^^^^^^^^^

What :command:`git merge branch_name` does (chronologically) is to

- Merge :code:`branch_name`
- Update the version number
- Open :program:`meld` for you to solve conflicts (if any)
- Open the :file:`VERSIONS` file for your to complete
- Update the merge commit to include the updated version number and :file:`VERSIONS` file

git mergepr (for maintainers)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

What :command:`git mergepr remote_repository/branch_name` does (chronologically) is to

- Download objects from the remote repository (:command:`git fetch`)
- Merge :code:`repository/branch_name` using :command:`git merge`
- Push the base branch to the remote repository (this closes the PR)
- Remove :code:`branch_name` from the local repository (if it exists)
- Remove :code:`branch_name` from the remote repository
- Remove the reference to :code:`remote_repository/branch_name` from the local repository

Compilation
-----------

Compilation is multithreaded by default.

Debugging
---------

GDB and Valgrind are used for debugging.  To run GDB:

.. code :: bash

  $ gdb --args ./neper <arguments>

Then, type :command:`run`.  To run Valgrind:

.. code :: bash

  $ valgrind ./neper <arguments>

It is sometimes useful to execute Neper on a single thread for debugging.  To do so, run

.. code :: bash

   $ export OMP_NUM_THREADS=1


.. [#nesting] The :program:`neperdev` command cannot be run inside an existing Tmux session (nesting doesn't work).  So, make sure to exit Tmux before running the command.
