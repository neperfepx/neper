Scotch : a software package for graph and mesh/hypergraph partitioning, graph clustering, and sparse matrix ordering
==============================

The Scotch distribution is a set of programs and libraries which implement the static mapping and sparse matrix reordering algorithms developed within the Scotch project.

Scotch has many interesting features:

* Its capabilities can be used through a set of stand-alone programs as well as through the libScotch library, which offers both C and Fortran interfaces.

* It provides algorithms to partition graph structures, as well as mesh structures defined as node-element bipartite graphs and which can also represent hypergraphs.

* It can map any weighted source graph onto any weighted target graph. The source and target graphs may have any topology, and their vertices and edges may be weighted. Moreover, both source and target graphs may be disconnected. This feature allows for the mapping of programs onto disconnected subparts of a parallel architecture made up of heterogeneous processors and communication links.

* It computes amalgamated block orderings of sparse matrices, for efficient solving using BLAS routines.

* Its running time is linear in the number of edges of the source graph, and logarithmic in the number of vertices of the target graph for mapping computations.

* It can handle indifferently graph and mesh data structures created within C or Fortran programs, with array indices starting from 0 or 1.

* It offers extended support for adaptive graphs and meshes through the handling of disjoint edge arrays.

* It is dynamically parametrizable thanks to strategy strings that are interpreted at run-time.

* It uses system memory efficiently, to process large graphs and meshes without incurring out-of-memory faults.

* It is highly modular and documented. Since it has been released under the CeCILL-C free/libre software license, it can be used as a testbed for the easy and quick development and testing of new partitioning and ordering methods.

* It can be easily interfaced to other programs. The programs comprising the Scotch project have been designed to run in command-line mode without any interactive prompting, so that they can be called easily from other programs by means of system() or popen() calls, or piped together on a single command line. Moreover, vertex labeling capabilities allow for easy renumbering of vertices.

* It provides many tools to build, check, and display graphs, meshes and matrix patterns.

* It is written in C and uses the POSIX interface, which makes it highly portable. PT-Scotch uses the MPI interface, and optionally the POSIX threads.


Get Scotch
----------

To use last development state of Scotch, please clone the master
branch.

    # if git version >= 1.9
      git clone --recursive git@gitlab.inria.fr:scotch/scotch.git
      cd scotch
    # else
      git clone git@gitlab.inria.fr:scotch/scotch.git
      cd scotch
      git submodule init
      git submodule update

Releases of Scotch are hosted on the
[gforge.inria.fr](https://gforge.inria.fr/frs/?group_id=248) for now.
Future releases will be available on this Gitlab project repository.

Documentation
-------------

All user and maintenance manuals are available here:

https://gitlab.inria.fr/scotch/scotch/tree/master/doc

Installation
------------

### Build and install with Makefile

Get involved!
---------------------

### Reporting an issue

We strongly recommend all users to use the issue tracker to report any problems with the software, or for any feature request. We will try our best to answer them.

### Contributions

https://gitlab.inria.fr/scotch/scotch/blob/master/CONTRIBUTING.md

### Authors

The following people contribute(d) to the development of Scotch:
* Cédric CHEVALIER
* Sébastien FOURESTIER
* Jun-Ho HER
* Cédric LACHAT
* Amaury JACQUES
* François PELLEGRINI (PI)
* Luca SCARANO

### Citing Scotch

Feel free to use the following publications to reference Scotch:

* "Scotch and PT-Scotch Graph Partitioning Software: An Overview"
  https://hal.inria.fr/hal-00770422

* "PT-Scotch: A tool for efficient parallel graph ordering"
  https://hal.inria.fr/hal-00402893

### Licence

https://gitlab.inria.fr/scotch/scotch/blob/master/LICENSE_en.txt
