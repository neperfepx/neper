Scotch: a software package for graph and mesh/hypergraph partitioning, graph clustering, and sparse matrix ordering
===================================================================================================================

The **Scotch** distribution is a set of programs and libraries which implement the static mapping and sparse matrix reordering algorithms developed within the **Scotch** project. The development of **Scotch** is supported in part by **The Scotch Consortium** (see below).

**Scotch** has many interesting features:

* Its capabilities can be used through a set of stand-alone programs as well as through the **Scotch** library, which offers both C and Fortran interfaces.

* It possesses an official Python interface, [ScotchPy](https://codeberg.org/fpellegr/scotchpy).

* It provides algorithms to partition graph structures, as well as mesh structures defined as node-element bipartite graphs and which can also represent hypergraphs.

* It can map any weighted source graph onto any weighted target graph. The source and target graphs may have any topology, and their vertices and edges may be weighted. Moreover, both source and target graphs may be disconnected. This feature allows for the mapping of programs onto disconnected subparts of a parallel architecture made up of heterogeneous processors and communication links.

* Its running time is linear in the number of edges of the source graph, and logarithmic in the number of vertices of the target graph for mapping computations.

* It computes amalgamated block orderings of sparse matrices, for efficient solving using BLAS routines.

* It can handle indifferently graph and mesh data structures created within C or Fortran programs, with array indices starting from 0 or 1.

* It offers extended support for adaptive graphs and meshes through the handling of disjoint edge arrays.

* It is dynamically parametrizable thanks to strategy strings that are interpreted at run-time.

* To speed-up its computations, the **Scotch** library dynamically takes advantage of either POSIX threads or native Windows threads. **The PT-Scotch** library, used to manage very large graphs distributed across the nodes of a parallel computer, uses the MPI interface, possibly in combination with multi-threading when the MPI implementation allows for it.

* It uses system memory efficiently, to process large graphs and meshes without incurring out-of-memory faults.

* It is highly modular and documented. Since it is available under a free/libre software license, it can be used as a testbed for the easy and quick development and testing of new partitioning and ordering methods.

* It can be easily interfaced to other programs. The programs comprising the **Scotch** project have been designed to run in command-line mode without any interactive prompting, so that they can be called easily from other programs by means of system() or popen() calls, or piped together on a single command line. Moreover, vertex labeling capabilities allow for easy renumbering of vertices.

* It provides many tools to build, check, and display graphs, meshes and matrix patterns.

* It is written in C and uses the POSIX interface, which makes it highly portable. Additionally, **PT-Scotch** uses the **MPI** interface.


Obtaining Scotch
----------------

**Scotch** is publicly available under the CeCILL-C free software license, as described [here](https://gitlab.inria.fr/scotch/scotch/blob/master/LICENSE_en.txt). The license itself is available [here](https://gitlab.inria.fr/scotch/scotch/-/blob/master/doc/CeCILL-C_V1-en.txt).

To use the lastest version of **Scotch**, please clone the master branch:

    git clone git@gitlab.inria.fr:scotch/scotch.git

Tarballs of the **Scotch** releases are available [here](https://gitlab.inria.fr/scotch/scotch/-/releases).


Documentation
-------------

The most recent user and maintenance manuals are available [here](https://gitlab.inria.fr/scotch/scotch/tree/master/doc).

The principles and internals of **Scotch** are also described in several papers:

* [Distillating knowledge about SCOTCH](https://drops.dagstuhl.de/opus/volltexte/2009/2091/pdf/09061.PellegriniFrancois.Paper.2091.pdf), in *Combinatorial Scientific Computing*, U. Naumann, O. Schenk, H. D. Simon and S. Toledo, eds., [Dagstuhl Seminar Proceedings vol. 9061](https://drops.dagstuhl.de/opus/volltexte/2009/2091/), Schloss Dagstuhl - Leibniz-Zentrum für Informatik, Dagstuhl, Germany, 2009, ISSN 1862-4405

* [Scotch and PT-Scotch Graph Partitioning Software: An Overview](https://www.taylorfrancis.com/chapters/edit/10.1201/b11644-18/scotch-pt-scotch-graph-partitioning-software-overview-franc%C2%B8ois-pellegrini), in *[Combinatorial Scientific Computing](https://www.taylorfrancis.com/books/mono/10.1201/b11644/combinatorial-scientific-computing)*, U. Naumann, O. Schenk eds., Chapman and Hall/CRC, 2012, ISBN 978-0-429-15217-7

* [Contributions to multilevel parallel graph partitioning](http://tel.archives-ouvertes.fr/docs/00/54/05/81/PDF/hdr.pdf), habilitation thesis, 2009


Installation
------------

Two flavors of installation are available:

* With CMake, using version 3.10 or higher:

```bash
mkdir build && cd build && cmake .. && make -j5
```

Many options can be provided from the command line, using the CMmake flag `-D`.

Linux and MacOS-X are fully supported. MacOS-X users must use recent versions of Flex and Bison that are available from [Brew](https://brew.sh/); older versions from Xcode will fail. To use them, run, e.g.:

``` bash
cmake -DBUILD_SHARED_LIBS=ON -DBISON_EXECUTABLE=/usr/local/Cellar/bison/3.8.2/bin/bison -DFLEX_EXECUTABLE=/usr/local/Cellar/flex/2.6.4_2/bin/flex
```
You may also use alternate compilers, by overloading the `CMAKE_*_COMPILER` variables, e.g.:

```bash
cmake -DCMAKE_Fortran_COMPILER=ifx -DCMAKE_C_COMPILER=icx -DMPI_HOME=/path/to/oneAPI/mpi/latest/
```

Windows plaftorms are also supported, featuring native multi-threading. Processor affinity is not yet implemented. If a Unix-like version of Make is not available, CMake can generate Microsoft NMAKE Makefiles:

```bash
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release ..
nmake
```

When creating dynamic libraries, the `-DLIBSCOTCHERR` flag allows one to select at compile time which error library to link against. This flag is set to "" by default, but can be set to any predefined or user-defined error handling library, e.g.,
```bash
cmake -DLIBSCOTCHERR=scotcherr
```

* With a traditional Makefile:

CMake installation is easy and straightforward. It allows one to compile and install **Scotch** and **PT-Scotch**, depending on flags such as the use of multi-threading and/or MPI. The traditional Makefile installation provides additional freedom to perform (cross-)compilation for non-standard systems and configurations. Please refer to the `[INSTALL.txt](https://gitlab.inria.fr/scotch/scotch/blob/master/INSTALL.txt)` file at the root of the package tree for more information on the use of traditional `Makefile`s.


Execution
---------

The behavior of the **libScotch** library and of the **Scotch** programs that use it can be conditioned at run time by way of environment variables:

* `SCOTCH_PTHREAD_NUMBER`: the prescribed maximum number of threads that **Scotch** may use in the course of its computations, or `-1` to use as many threads as provided by the system at launch time;

* `SCOTCH_DETERMINISTIC`: `0` for allowing for the use of faster, non-deterministic, multi-threaded algorithms, and `1` for a fully, albeit sometimes slower, deterministic multi-threaded behavior;

* `SCOTCH_RANDOM_FIXED_SEED`: `0` for setting a new, dynamically-changing pseudo-random seed at every run, and `1` for a fixed pseudo-random seed.

### Intel MPI

Some hanging issues have been reported when running PT-Scotch in
multi-threaded mode on Intel MPI. Until Intel fixes this issue, a
workaround, available since Intel MPI version 2021.17, is to run
Intel's `mpiexec` command with some variables set as follows:

```
I_MPI_THREAD_SPLIT=1, I_MPI_THREAD_SPLIT_MODE=implicit, I_MPI_THREAD_MAX=x
```

where the `x` integer value is the maximum number of threads that Scotch will use, which must be set explicitly at compile time, or at execution time as an environment variable:

```
export SCOTCH_PTHREAD_NUMBER=x
```

Consortium
----------

**The Scotch Consortium**, hosted by Inria within the InriaSoft program, aims at supporting the scientific and industrial development and the long-term maintenance of **Scotch**. Through their subscriptions, industrial and academic Consortium Members, which have a strategic need in the availability and robustness of the **Scotch** software, benefit from extended support, direct contact with the development team, participation in the project roadmap, and access to early releases. Please contact the PI at <francois.pellegrini@u-bordeaux.fr> for more information.


Contributing to Scotch
----------------------

To report a bug or discuss potential improvements, you can contact directly the PI at <francois.pellegrini@u-bordeaux.fr>. However, the GitLab environment provides features that are worth taking advantage of, so we recommend you to take the time to use them. Before reporting a bug or submitting a patch in the Inria GitLab environment, you will need an account on the server.
**Please dot not hesitate to send an e-mail to <marc.fuentes@inria.fr> so that we create an account for you on the Inria Gitlab repository**. You will then be able to open issues in the bug tracker, request features, or propose patches using the "merge requests" feature.

Contribtions to `ScotchPy` are made on the [Codeberg](https://codeberg.org/) platform, on which registration is much easier.


Past and current contributors
-----------------------------

The following people contribute(d) to the development of **Scotch**:

* Clément BARTHÉLEMY

* Cédric CHEVALIER

* Sébastien FOURESTIER

* Marc FUENTES

* Jun-Ho HER

* Amaury JACQUES

* Cédric LACHAT

* Selmane LEBDAOUI

* Connor Alexander MAYON

* Tetsuya MISHIMA

* Xavier MULLER

* Charles PELLEGRINI

* François PELLEGRINI (PI)

* Florent PRUVOST

* Luca SCARANO


Citing Scotch
-------------

Feel free to use the following publications to reference **Scotch**:

* "Scotch and PT-Scotch Graph Partitioning Software: An Overview"
  https://hal.inria.fr/hal-00770422

* "PT-Scotch: A tool for efficient parallel graph ordering"
  https://hal.inria.fr/hal-00402893


Licence
-------

[https://gitlab.inria.fr/scotch/scotch/blob/master/LICENSE_en.txt](https://gitlab.inria.fr/scotch/scotch/blob/master/LICENSE_en.txt)
