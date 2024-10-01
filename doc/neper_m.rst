.. |translate| replace:: :data:`translate(<dist_x>,<dist_y>,<dist_z>)`: translate by distances :data:`<dist_x>`, :data:`<dist_y>` and :data:`<dist_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |rotate| replace:: :data:`rotate(<axis_x>,<axis_y>,<axis_z>,<angle>)`: rotate about the center, by a given axis and angle (angle expressed in degrees)
.. |scale| replace:: :data:`scale(<fact_x>,<fact_y>,<fact_z>)`: scale by :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` along :data:`x`, :data:`y` and :data:`z`, respectively
.. |rasterscale| replace:: :data:`rasterscale(<fact>)` or :data:`rasterscale(<fact_x>,<fact_y>,<fact_z>)`: scale the number of voxels of a raster tessellation, where :data:`<fact>` is the scaling factor that applies in the three directions, and :data:`<fact_x>`, :data:`<fact_y>` and :data:`<fact_z>` are the scaling factors along :data:`x`, :data:`y` and :data:`z`, respectively
.. |crop| replace:: :data:`crop(<xmin>,<xmax>,<ymin>,<ymax>,<zmin>,<zmax>)`: crop a region of a raster tessellation, where :data:`<xmin>`, :data:`<xmax>`, :data:`<ymin>`, :data:`<ymax>`, :data:`<zmin>` and :data:`<zmax>` are the minimum and maximum positions along :data:`x`, :data:`y` and :data:`z`, respectively. For 2D raster tessellations, the :data:`z` values can be omitted

.. index::
   single: -M

.. _neper_m:

Meshing Module (-M)
===================

Module -M is the module for meshing scalar and raster tessellations.  Two meshing strategies are available. `Free (or unstructured) meshing` creates a conforming mesh into tetrahedral elements (triangular in 2D).  `Mapped (or structured) meshing` generates a non-conforming mesh into regular hexahedral elements (quadrangular in 2D). Free meshing is carried out so that the elements have sizes as close as possible to the target value, and show high quality, that is, with shapes as close to equilateral as possible.  The input file is a tessellation file (:file:`.tess`) or a raster tessellation file (:file:`.tesr`), as provided by module -T.  Standard tessellations, multiscale tessellations, and periodic (or semi-periodic) tessellations are supported.  Free meshing of raster tessellations works for 2D tessellations only.  The output mesh can be written as a :file:`.msh` file, which can be readily used by FEPX, or at other formats.

The target element size of the mesh can be specified through the element *characteristic length* (:data:`cl`).  It corresponds to the length of a 1D element, the length of the edge of a triangular or quadrangular element (2D), and the length of an edge of a tetrahedral or hexahedral element (3D). A *relative characteristic length* (:data:`rcl`) can be used instead, whose value is relative to the average cell size and provides a medium number of elements.  It is also possible to specify :data:`cl` or :data:`rcl` values on a per-cell basis, or different values along the three space directions.

For free meshing,  mesh quality is ensured to the greatest extent possible using several approaches:

- Optimized meshing rules: the mesh properties are controlled by size parameters (options :option:`-cl`, :data:`-rcl`, etc.) and a size gradient parameter used for 1D meshing (option :option:`-pl`);

- Multimeshing: each tessellation face and volume is meshed separately of the others, with several meshing algorithms, until a target mesh quality is reached.  This is controlled by options :data:`-meshqual*`, :option:`-mesh2dalgo` and :option:`-mesh3dalgo`.

Note that, in general, tessellation *regularization* is also necessary to ensure good-quality meshing (see :ref:`neper_t`, option :option:`-regularization`).

Remeshing can also be applied to generate a new, good-quality mesh from a mesh containing poor-quality elements.  The variables defined on the parent mesh can be transported on the child mesh (options :data:`-transport*`).

For mapped meshing, mesh cleaning options enable the removal of isolated elements or duplicate nodes, or the duplication of nodes that would create a singular behavior in simulations (options :option:`-clean`, :option:`-dupnodemerge` and :option:`-singnodedup`).

Mesh partitioning allows for the division the mesh nodes and elements into several sets while minimizing the interfaces between them [#partitioning]_, as needed for parallel simulations with FEPX.  Partitioning can return any number of partitions or be done according to a given parallel computer architecture (options :data:`-part*`).

In the output mesh, the individual entities of the tessellations (the vertices, edges, faces and polyhedra) can be described by element sets (option :option:`-dim`).  Node sets of the vertices, edges and faces located on the boundary of the tessellation are also provided for prescribing the boundary conditions (option :option:`-nset`).  The surface element sets are also provided (option :option:`-faset`).  Element sets other than those corresponding to the tessellation cells can be defined (option :option:`-elset`). The mesh order can be 1 or 2 (option :option:`-order`, default :data:`2`).  Statistical data can be obtained on the meshes (options :data:`-stat*`).

The methods implemented for meshing are described in [CMAME2011]_.

Here is what a typical run of module -M looks like:

.. code-block:: console

  $ neper -M n10-id1.tess

  ========================    N   e   p   e   r    =======================
  Info   : A software package for polycrystal generation and meshing.
  Info   : Version 4.0.0
  Info   : Built with: gsl|muparser|opengjk|openmp|nlopt|libscotch (full)
  Info   : Running on 8 threads.
  Info   : <https://neper.info>
  Info   : Copyright (C) 2003-2024, and GNU GPL'd, by Romain Quey.
  Info   : No initialization file found (`/home/rquey/.neperrc').
  Info   : ---------------------------------------------------------------
  Info   : MODULE  -M loaded with arguments:
  Info   : [ini file] (none)
  Info   : [com line] n10-id1.tess
  Info   : ---------------------------------------------------------------
  Info   : Reading input data...
  Info   :   - Reading arguments...
  Info   : Loading input data...
  Info   :   - Loading tessellation...
  Info   :     [i] Parsing file `n10-id1.tess'...
  Info   :     [i] Parsed file `n10-id1.tess'.
  Info   : Meshing...
  Info   :   - Preparing... (cl = 0.2321) 100%
  Info   :   - 0D meshing... 100%
  Info   :   - 1D meshing... 100%
  Info   :   - 2D meshing... 100% (0.69|0.86/92%| 4%| 4%)
  Info   :   - Fixing 2D-mesh pinches...
  Info   :   - 3D meshing... 100% (0.89|0.91/100%| 0%| 0%)
  Info   : Searching nsets and fasets...
  Info   : Writing mesh results...
  Info   :   - Preparing mesh...
  Info   :   - Mesh properties:
  Info   :     > Node number:      295
  Info   :     > Elt  number:     1063
  Info   :     > Mesh volume:    1.000
  Info   :   - Writing mesh...
  Info   :     [o] Writing file `n10-id1.msh'...
  Info   :     [o] Wrote file `n10-id1.msh'.
  Info   : Elapsed time: 1.127 secs.
  ========================================================================

Arguments
---------

Prerequisites
~~~~~~~~~~~~~

.. option:: -gmsh <binary>

  Specify the Gmsh command or the path to the Gmsh binary (for meshing into triangle and tetrahedral elements).

  **Default value**: :data:`gmsh`.

.. option:: -tmp <directory>

  Specify the path to the temporary directory (used by Gmsh).

  **Default value**: :option:`.`.

Input Data
~~~~~~~~~~

In normal use, the input data is a tessellation file, a raster tessellation file or a mesh file.

.. option:: <file_name1>[:<transformation1>][:<transformation2>]...,<file_name2>...

  Specify the input file, which can be:

  - a tessellation file (:file:`.tess`);
  - a raster tessellation file (:file:`.tesr`), with optional transformations, which can be:

    - |crop|.
    - |rasterscale|.

  - a mesh file (:file:`.msh`), for remeshing (in which case a tessellation file should preferably be loaded too), with an optional transformation:

    - :data:`<file_name>`: coordinates to load from a :ref:`position_file`.

  **Default value**: -.

It is also possible to load a mesh to be considered as output mesh (in contrast to loading it as input file).  If the file contains meshes of dimensions lower than the tessellation's dimension, they are used, and only higher-dimension meshes are computed. Use option :option:`-o` to avoid overwriting the file.

.. option:: -loadmesh <file_name>

  Load a mesh from a :ref:`mesh_file`.

  **Default value**: -.

Finally, it is possible to load a set of points.  These points are used only for statistics, in option :option:`-statpoint`.

.. option:: -loadpoint <file_name>

  Load points from a :ref:`position_file`.

  **Default value**: -.

Meshing Options
~~~~~~~~~~~~~~~

.. option:: -elttype <elt_type>

  Specify the type of elements, which can be:

  - :data:`tri`  : triangular elements;
  - :data:`quad` : quadrangular elements;
  - :data:`quad9`: 9-node quadrangular elements;
  - :data:`tet`  : tetrahedral elements;
  - :data:`hex`  : hexahedral elements.

  In 2D, :data:`tet` and :data:`hex` are treated as :data:`tri` and :data:`quad`, respectively.

  **Default value**: :data:`tet` in 3D and :data:`tri` in 2D.

.. index::
   single: -cl

.. index::
   single: -rcl

.. option:: -cl or -rcl <length> (secondary option)

  Specify the absolute or relative characteristic lengths of the elements.  :data:`-rcl` is defined relative to the average cell size.  The argument can be:

  - a real value;
  - an expression based on the :ref:`tessellation_keys` or :ref:`raster_tessellation_keys`, which allows to define individual values. An example is :data:`-rcl (body>0)?\<val1\>:\<val2\>`, to mesh interior cells with :data:`rcl=\<val1\>` and boundary cells with :data:`rcl=\<val2\>`.
  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  **Default value**: :data:`-rcl 1` (which corresponds to averages of about 100 elements per cell in 3D and 64 in 2D).

.. option:: -dim <dimension1>,<dimensions2>,...[:msh]

  Specify the mesh dimension(s), which can be:

  - an integer value between :data:`0` and :data:`3`;
  - :data:`inputdim`: the dimension of the input data;
  - :data:`all`: all (:data:`0` to :data:`inputdim`);
  - :data:`none`: none.

  .. attention::

    The :file:`.msh` file does not depend on this option and will always contain the meshes of all dimensions, unless :data:`:msh` is used.

  .. note::

    If a mesh dimension of 3 is requested while the input data is 2D, the 2D mesh is extruded into a 3D mesh (tetahedral elements).  The extrusion length is equal to the typical (in-plane) element characteristic length.

  With :data:`-format geof`, use :data:`1,inputdim` to get the 1D mesh written as lisets.

  **Default value**: :data:`inputdim`.

.. option:: -order <order>

  Specify the mesh order, which can be:

  - :data:`1`: linear-interpolation elements (2-node linear elements, 3-node triangular elements, 4-node quadrangular elements, 4-node tetrahedral elements and 8-node hexahedral elements);

  - :data:`2`: quadratic-interpolation elements (3-node linear elements, 6-node triangular elements, 8-node or 9-node quadrangular elements, 10-node tetrahedral elements and 20-node hexahedral elements).

  **Default value**: :data:`2`.

.. option:: -cl{ver,edge,face} or -rcl{ver,edge,face} <characteritic_length> (secondary options

  Specify the absolute or relative characteristic lengths of the elements at the vertex, edge or face level (compared to options :option:`-cl` and :data:`-rcl` applying to the cells), which can be:

  - :data:`default`: the same value as the parent, upper-dimension entity;
  - a real value;
  - an expression based on the :ref:`tessellation_keys`, which allows to define individual values.

    An example is :data:`-rcl "(domface==<id>)?<val>:default`, to get a finer mesh at domain surface :data:`id`.

  - :data:`file(<file_name>)`: individual values to load from a :ref:`data_file`.

  **Default value**: -.

.. option:: -pl <progression_factor> (secondary option)

  Specify the progression factor for the element characteristic lengths, which corresponds to
  the maximum ratio between the lengths of two adjacent 1D elements (:math:`\geq 1`).

  **Deafult value**: :data:`2`.

.. option:: -clratio <val_x>:<val_y>:<val_z> (secondary option)

  Specify the ratios between the :data:`cl` values along the :data:`x`, :data:`y` and :data:`z` directions. For example, :data:`2:1:1` leads to elements twice as long in the :data:`x` direction as in the :data:`y` and :data:`z` directions.

  **Default value**: :data:`1:1:1`.

The following options define the multimeshing algorithm (for 2D and 3D free meshings). `Multimeshing` consists of using several meshing algorithms concurrently, for each face or polyhedron, until a minimum, target mesh quality is reached. The mesh quality factor, :math:`O`, accounts for both the element sizes and aspect ratios. It is given by :math:`O = O_{dis}^\alpha \times O_{size}^{1-\alpha}`, where :math:`O_{dis}` and :math:`O_{size}` range from 0 (poor quality) to 1 (high quality), and :math:`\alpha` is a factor equal to 0.8.  Therefore, :math:`O` also ranges from 0 (poor quality) to 1 (high quality) [CMAME2011]_.  The minimum quality value can be modified using option :option:`-meshqualmin`.  The values of :math:`O` and :math:`O_{dis}` can be modified using options :option:`-meshqualexpr` and :option:`-meshqualdisexpr`.  The value of the target mesh quality significantly influences meshing speed and output mesh quality. A value of :data:`0` provides the fastest meshing while a value of :data:`1` provides the best-quality meshing. The default value provides an effective balance. 2D and 3D meshings are achieved using Gmsh and Netgen (options :option:`-mesh2dalgo` and :option:`-mesh3dalgo`).

.. option:: -meshqualmin <quality_value>

  Specify the minimum target value of mesh quality, :math:`O`, as defined by option :option:`-meshqualexpr`.

  **Default value**: :data:`0.9`.

.. option:: -meshqualexpr <quality_expression> (secondary option)

  Specify the expression of mesh quality, :math:`O`, as a function of :math:`O_{dis}` and :math:`O_{size}`.

  **Default value**: :data:`Odis^0.8*Osize^0.2`.

.. option:: -meshqualdisexpr <quality_expression> (secondary option)

  Specify the expression of the mesh element distortion parameter, :math:`O_{dis}`, as a function of the element distortion parameter :math:`dis`.

  **Default value**: :data:`dis^(exp((dis^0.1)/(dis^0.1-1)))`.

.. option:: -mesh2dalgo <algo1>,<algo2>,... (secondary option)

  Specify the 2D meshing algorithm(s), which can be:

  - :data:`mead`: MeshAdapt;
  - :data:`dela`: Delaunay;
  - :data:`fron`: Frontal.

  According to *multimeshing*, the algorithms are used concurrently until the quality criterion is reached.

  **Default value**: :data:`mead,dela,fron`.

.. option:: -mesh3dalgo <mesh_algo1>:<opti_algo1>,<mesh_algo2>:<opti_algo2>,... (secondary option)


  Specify the 3D meshing and optimization algorithms.  The only meshing algorithm is :data:`netg` (Netgen), and the optimization algorithms can be:

  - :data:`gmsh`: Gmsh;
  - :data:`netg`: Netgen;
  - :data:`gmne`: Gmsh + Netgen;
  - :data:`none`: none.

  According to *multimeshing*, the algorithms are used concurrently until the quality criterion is reached.

  **Default value**: :data:`netg:gmsh,netg:netg,netg:gmne`.

.. option:: -meshedge <edge_expr> (secondary option)

  Specify the edges to mesh, which can be :data:`all` or an expression based on the variables described in :ref:`tessellation_keys`.

  **Default value**: :data:`all`.

.. option:: -meshface <face_expr> (secondary option)

  Specify the faces to mesh, which can be :data:`all` or an expression based on the variables described in :ref:`tessellation_keys`.

  **Default value**: :data:`all`.

.. option:: -meshpoly <poly_expr> (secondary option)

  Specify the polyhedra to mesh, which can be :data:`all` or an expression based on the variables described in :ref:`tessellation_keys`.

  **Default value**: :data:`all`.

.. option:: -interface <interface_type> (secondary option)

  Specify the type of interface meshing, which can be:

  - :data:`continuous`: continuous mesh at interfaces with shared nodes between neighbor element sets (which are associated to the tessellation cells);
  - :data:`discontinuous`: discontinuous mesh at interfaces with distinct nodes for the neighbor element sets;
  - :data:`cohesive`: cohesive elements at interfaces joining the neighbor element sets.

  In the case of a multiscale tessellation, it is possible to provide a different value for each scale using the :data:`::` separator (if fewer values than tessellation scales are provided, the last provided value is used for all higher scales).

  **Default value**: :data:`continuous`.

.. option:: -mesh2dpinchfix <logical> (secondary option)

  Apply 2D-mesh pinches correction after 2D meshing (recommended).

  **Default value**: :data:`1`.

Raster Tessellation Meshing Options
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Direct 2D raster tessellation meshing involves interface reconstruction, interface mesh smoothing and then remeshing. The following options enable the control of interface smoothing.

.. option:: -tesrsmooth <smoothing> (secondary option)

  Specify the method for smoothing the interface meshes reconstructed from a raster tessellation, which can be:

  - :data:`laplacian`: Laplacian smoothing is an iterative method that modifies the coordinates of a node of a mesh using the coordinates of the connected nodes.  At iteration :math:`i`, the position of a node, :math:`X_i`, is calculated from its previous position, :math:`X_{i-1}`, and the position of the barycenter of the neighboring nodes (weighted barycenter considering the inverse of the distance between the node and the neighbor), :math:`X^n_{i-1}`, as follows: :math:`X_i=(1-A)\,X_{i-1} + A\,X^n_{i-1}`.  :math:`A \in \left[0,\,1\right]` is an adjustable parameter (see option :option:`-tesrsmoothfact`). The number of iterations is set by option :option:`-tesrsmoothitermax`.  There is no stop criterion, and so, :data:`itermax` will always be reached.
  - :data:`none`: none.

  **Default value**: :data:`laplacian`.

.. option:: -tesrsmoothfact <fact> (secondary option)

  Specify the factor used for the interface mesh smoothing (:math:`A` in option :option:`-tesrsmooth`).

  **Default value**: :data:`0.5`.

.. option:: -tesrsmoothitermax <iter_number> (secondary option)

  Specify the number of iterations used for interface mesh smoothing.

  **Default value**: :data:`5`.

Mesh Cleaning Options
~~~~~~~~~~~~~~~~~~~~~

The following options are specific to mapped meshing of raster tessellations containing voids.

.. option:: -clean <cleaning_level> (secondary option)

  Specify the mesh cleaning level.  Cleaning makes it so that the mesh is made of a set of connected elements by removing the elements that are not connected to the main skeleton according to a given criterion, which can be:

  - :data:`0`: no cleaning;
  - :data:`1`: two elements should be considered connected to each other if they share at least a vertex:
  - :data:`2`: two elements should be considered connected to each other if they share at least a face.

  **Default value**: :data:`0`.

.. option:: -singnodedup <logical> (secondary option)

  Duplicate singular nodes.  A singular node belongs to several elements which share only a node or an edge which provides a singular behavior [#singular]_.

  Duplicated nodes can be merged back using option :option:`-dupnodemerge`.

  **Default value**: :data:`0`.

.. option:: -dupnodemerge <distance> (secondary option)

  Merge duplicate nodes. Provide as argument the distance between nodes below which two nodes are merged.

  Note that Neper does not generate meshes with duplicate nodes, except when :option:`-singnodedup` or :option:`-interface` are used.

  **Default value**: -.

Transformation Options
~~~~~~~~~~~~~~~~~~~~~~

.. option:: -transform <transformation1>,<transformation2>,...

  Apply successive transformations to a mesh, which can be:

  - |translate|;
  - |rotate|;
  - |scale|;
  - :data:`smooth(<A>,<itermax>,<type>)`: smooth the interfaces of a mesh by Laplacian smoothing (use only if you know what you are doing).  Laplacian smoothing is an iterative method that modifies the coordinates of a node using the coordinates of the neighboring nodes.  At iteration :math:`i`, the position of a node, :math:`X_i`, is calculated from its previous position, :math:`X_{i-1}`, and the position of the barycenter of the neighboring nodes (weighted barycenter considering the inverse of the distance between the node and the neighbor), :math:`X^n_{i-1}` as follows: :math:`X_i=(1-A)\,X_{i-1} + A\,X^n_{i-1};\,0\leq A \leq 1`.  :data:`itermax` iterations are applied. :data:`type` defines the nodes used for smoothing and can be :data:`all` for all nodes or :data:`interior` for interior nodes.
  - :data:`explode(<fact>)`: produce a roughly "exploded" mesh for which elsets are apart from each other (or cohesive elements become thick).  To be used in conjunction with :data:`-interface {discontinuous,cohesive}`.  :data:`<fact>` is a factor controlling the distance between elsets.
  - :data:`slice(<d>,<a>,<b>,<c>)`: slice a 3D mesh by the (oriented) plane of equation :math:`a\,x+b\,y+c\,z=d` (yielding to a 2D mesh).
  - :data:`node(<file_name>)`: override node positions with ones defined in a :ref:`data_file`.
  - :data:`ori(<file_name>[,des=<descriptor>])`: override orientations with ones defined in a :ref:`data_file` written using a specific descriptor (see :ref:`rotations_and_orientations`, default :data:`rodrigues`).  The file may contain orientations for elsets or for elements, and this is automatically determined from the number of lines (in the case of element orientations, elset orientations are updated accordingly, by averaging).

  **Default value**: -.

.. index::
   single: -sortnode
   single: -sortelt

.. option:: -sort{node,elt} <sort_expression> (secondary option)

  Sort nodes or elements (typically to facilitate data post-processing) according to a mathematical expression based on the variables described in :ref:`mesh_keys`, in ascending order.

  **Default value**: -.

Mesh Partitioning Options
~~~~~~~~~~~~~~~~~~~~~~~~~

Mesh partitioning is achieved using the `libscotch <https://www.labri.fr/perso/pelegrin/scotch>`_ library. The principle of mesh partitioning is to create partitions of the same size, in terms of nodes and elements, while minimizing the interfaces between them. This attempts to distribute an equal load to all computation units and minimizes communications between them, and therefore minimizes the total computation time of simulations.  There are two available strategies for mesh partitioning. The first one creates partitions and arranges them independently of each other, while the second one consists of optimizing the size and arrangment of the partitions based on a given computer cluster architecture to minimize computation time further. For those clusters that are made of nodes containing several cores each, the communication time between cores on a common node is much lower than the communication time between cores of different nodes. To minimize the global communication time, partitions which are processed by cores of the same node can be grouped together (on modern architectures, the gain remains marginal).  Partitioning is applied to the higher-dimension mesh and (for the sake of interfacing with FEPX) renumbers nodes and elements by ascending partition number.  This can be managed using options :data:`-part*`.

.. option:: -part <part_number>|<part_architecture>

  Specify the number of partitions or a computer cluster architecture.  Using a number of partitions is generally sufficient.  Using a computer architecture may lead to a slight performance increase but the total number of partitions must be a power of 2. An architecture can be specified in two ways:

  - :data:`<number_of_nodes>:\<number_of_cores_per_node\>`: Number of nodes (or "processors") and number of cores per node or processor.  A ratio of 10 is considered between the computation time between cores located on different nodes and the one between cores of the same node.
  - :data:`<file_name>`: cluster architecture to load from a file and described at the libscotch format.

  **Default value**: -.

.. option:: -partmethod <part_method> (secondary option)

  Specify the partitioning method, which can be:

  - :data:`scotch`: libscotch partitioner;
  - :data:`linear`: linear partitioning based on the node and element numbers (not recommended).

  **Default value**: :data:`scotch`.

Field Transport Options
~~~~~~~~~~~~~~~~~~~~~~~

.. option:: -transport <entity_type1>:<data_type1>:<file_name1>,<data_type2>:<file_name2>,...

  Transport data from a parent mesh to a child mesh.  The parent mesh is the input mesh and the child mesh is the result mesh (created by remeshing or loaded with :option:`-loadmesh`).  :data:`<entity_type>` can be :data:`node` or :data:`elt`, and :data:`<data_type>` can be :data:`integer\<X\>` or :data:`real\<X\>`, where :data:`<X>` is the data dimensionality, and :data:`<file_name>` is the file containing the parent data.

  Nodal data are transported by interpolation using the shape functions (2D only).  Elemental data are transported as specified by :option:`-transporteltmethod`.

  **Default value**: -.

.. option:: -transporteltmethod <method> (secondary option)

  Specify the method to transport elemental data from the parent mesh to the child mesh. For each element of the child mesh, its center, :math:`c`, is considered.  The method can be:

  - :data:`distance`: choose, for each element of the child mesh (of center :math:`c`), the element of the parent mesh whose center is the closest to :math:`c`;
  - :data:`location`: choose the element :math:`c` belongs to.

  **Default value**: :data:`distance`.

Output Options
~~~~~~~~~~~~~~

.. option:: -o <file_name>

  Specify the output file name.

  **Default value**: input file's base name.

.. option:: -format <format1>,<format2>,...

  Specify the format(s) of the output file(s), which can be:

  - :data:`msh[:<encoding>]`: native (Gmsh-style) mesh file, where :data:`encoding` can be :data:`ascii` or :data:`binary` (default :data:`ascii`);
  - :data:`sim`: simulation directory;
  - :data:`msh4`: Gmsh version 4 mesh file;
  - :data:`vtk`: VTK mesh file;
  - :data:`inp`: Abaqus input file;
  - :data:`geof`: Z-set mesh file;
  - :data:`ori`: orientation file (optional FEPX input);
  - :data:`phase`: group file (optional FEPX input);
  - :data:`bcs`: boundary condition file (optional FEPX input);
  - :data:`per`: periodicity file (see also option :option:`-performat`);
  - :data:`tess`: tessellation file, where the tessellation is reconstructed from the mesh.

  **Default value**: :data:`msh`.

.. option:: -nset <nset1>,<nset2>,...

  Specify the node sets to provide, which can be:

  - :data:`faces`: domain faces;
  - :data:`edges`: domain edges;
  - :data:`vertices`: domain vertices;
  - :data:`facebodies`: interior of the domain faces;
  - :data:`edgebodies`: interior of the domain edges;
  - :data:`all`: all of the above node sets;
  - :data:`<dom_entity_label>`: label of a domain entity.

    - For a cubic domain, they are :data:`{x-z}{0,1}`: domain faces, :data:`{x-z}{0,1}{x-z}{0,1}`: domain  edges, and :data:`{x-z}{0,1}{x-z}{0,1}{x-z}{0,1}`: domain vertices.
    - For a cylindrical domain they, are :data:`z{0,1}` for the :data:`z` faces and :data:`f{1,2,...}` for the faces on the circular part of the domain.
    - For other domains, they are :data:`f{1,2,...}` for the faces.

    The labels of edges and vertices are derived by concatenating those of their parent faces. :data:`body` can be appended to obtain only the interior nodes.

  - :data:`none`: none.

  **Default value**: :data:`faces,edges,vertices` in 3D and :data:`edges,vertices` in 2D.

.. option:: -faset <faset1>,<faset2>,...

  Specify the element surface meshes (edge meshes in 2D) to output, which can be:

  - :data:`faces`: all domain faces;
  - :data:`<dom_face_label>`:  individual faces (see option :option:`-nset`);
  - :data:`internal`: internal faces (edges in 2D) as created by :data:`-interface discontinuous`.
  - :data:`none`: none.

  **Default value**: :data:`none`.

.. option:: -elset <elset1>,<elset2>,... (secondary option)

  Specify the element sets to output, which can be:

  - :data:`default`: default element sets (those corresponding to the input tessellation cells);
  - :data:`<elset_label>:\<elset_definition\>`, where :data:`<elset_label>` is a custom elset label and :data:`<elset_definition>` is an expression defining the elements belonging to the elset defined from the variables provided in :ref:`mesh_keys`.

  **Default value**: :data:`default`.

.. option:: -performat <format>

  Specify the format of the :file:`.per` output file, which can be:

  - :data:`msh`;
  - :data:`plain`: plain format;
  - :data:`geof`: Z-set format.

  **Default value**: :data:`msh`.

Post-Processing Options
~~~~~~~~~~~~~~~~~~~~~~~

The following options provide general statistics on the highest-dimension mesh (:data:`mesh`), 0D mesh (:data:`mesh0d`), 1D mesh (:data:`mesh1d`), 2D mesh (:data:`mesh2d`), 3D mesh (:data:`mesh3d`) and cohesive-element mesh (:data:`meshco`).

.. option:: -stat{mesh,mesh{0-3}d,meshco} <key1>,<key2>,...

  Provide statistics on the mesh. Available keys are described in :ref:`mesh_keys`.

  **Default value**: -.

  **File extension**: :option:`.stmesh`, :option:`.stmesh{0-3}d`, :option:`.stmeshco`.

The following options provide statistics on the nodes (:data:`nodes`), highest-dimension elements (:data:`elt`) and element sets (:data:`elset`), 0D elements (:data:`elt0d`) and element sets (:data:`elset0d`), 1D elements (:data:`elt1d`) and element sets (:data:`elset1d`), 2D elements (:data:`elt2d`) and element sets (:data:`elset2d`), 3D elements (:data:`elt3d`) and element sets (:data:`elset3d`), cohesive elements (:data:`eltco`) and element sets (:data:`elsetco`), and points (:data:`point`).

.. option:: -stat{node,elt,elt{0-3}d,elset,elset{0-3}d,eltco,elsetco} <key1>,<key2>,...

  Provide statistics on the nodes, elements or elsets.  Available keys are described in :ref:`mesh_keys`.

  **Default value**: -.

  **File extension**: :option:`.stnode`, :option:`.stelt`, :option:`.stelt{0-3}d`, :option:`.stelset`, :option:`.stelset{0-3}d`, :option:`.steltco`, :option:`.stelsetco`.

.. option:: -statpoint <key1>,<key2>,...

  Provide statistics on points.  The points must be loaded with option
  :option:`-loadpoint`. Available keys are described in :ref:`point_keys`.

  **Default value**: -.

  **File extension**: :option:`.stpoint`.

Advanced Options
~~~~~~~~~~~~~~~~

These advanced options set running conditions for the meshing libraries (2D and 3D meshing).

.. option:: -mesh3dclreps <tolerance> (secondary option)

  Specify the relative tolerance on the average element characteristic length of each cell (compared to :data:`cl`), which can be:

  - a real value;
  - an expression based on the variables defined in :ref:`tessellation_keys`;
  - :data:`<file_name>`: values from a :ref:`data_file`.

  The average element characteristic length is corrected iteratively, and so this is a highly CPU-sensitive option.

  **Default value**: :data:`0.02`.

.. index::
   single: -mesh2dmaxtime
   single: -mesh3dmaxtime

.. option:: -mesh{2,3}dmaxtime <time> (secondary option)

  Specify the maximum processing time allowed to the meshing library for meshing a tessellation face
  or polyhedron (in seconds).

  **Default value**: :data:`1000`.

.. index::
   single: -mesh2drmaxtime
   single: -mesh3drmaxtime

.. option:: -mesh{2,3}drmaxtime <relative_time> (secondary option)

  Specify a factor used to determine the maximum processing time allowed to the meshing library for meshing a tessellation face or polyhedron.  This option is similar to :data:`-mesh2dmaxtime`, but the actual maximum time is the product of the maximum processing time of the previous meshings by the value provided in argument.

  **Default value**: :data:`100`.

.. option:: -mesh{2,3}diter <iter_number> (secondary option)

  Specify the maximum number of 2D meshing attempts for a particular face or polyhedron (in the rare case of failure).

  **Default value**: :data:`3`.

Output Files
------------

Mesh
~~~~

  - :file:`.msh`: mesh file as described in :ref:`mesh_file`;
  - :file:`.msh4`: Gmsh version 4 mesh file;
  - :file:`.vtk`: VTK file;
  - :file:`.inp`: Abaqus input file;
  - :file:`.geof`: Z-set file;
  - :file:`.ori`: orientation file (optional FEPX file).  It contains the :data:`$ElsetOrientations` and :data:`$ElementOrientations` sections of the :file:`.msh` file;
  - :file:`.phase`: group file (optional FEPX file).  It contains the :data:`$Groups` section of the :file:`.msh` file;
  - :file:`.bcs`: boundary conditions file (optional FEPX file);
  - :file:`.per`: periodicity file.

Interfaces
~~~~~~~~~~

- File :file:`.intl`.  For each 2D interface, the file provides (one 2D interface per line) the labels of the two element sets of the interface.

Statistics
~~~~~~~~~~

Statistics files are provided for nodes, elements, element sets and points.  They are formatted with one entity per line.  Each line contains the data specified to the corresponding :data:`-stat` option and described in :ref:`mesh_keys`.

  - :file:`.stnode`: node statistics file;
  - :file:`.stmesh`: (highest-dimension) mesh statistics file;
  - :file:`.stelt`: (highest-dimension) element statistics file;
  - :file:`.stelset`: (highest-dimension) element set statistics file;
  - :file:`.stmesh0d`: 0D mesh statistics file;
  - :file:`.stmesh1d`: 1D mesh statistics file;
  - :file:`.stmesh2d`: 2D mesh statistics file;
  - :file:`.stmesh3d`: 3D mesh statistics file;
  - :file:`.stmeshco`: Cohesive-element mesh statistics file;
  - :file:`.stelt0d`: 0D element statistics file;
  - :file:`.stelt1d`: 1D element statistics file;
  - :file:`.stelt2d`: 2D element statistics file;
  - :file:`.stelt3d`: 3D element statistics file;
  - :file:`.stelset0d`: 0D elset statistics file;
  - :file:`.stelset1d`: 1D elset statistics file;
  - :file:`.stelset2d`: 2D elset statistics file;
  - :file:`.stelset3d`: 3D elset statistics file;
  - :file:`.stpoint`: point statistics file.

Examples
--------

Below are some examples of use of neper -M.

- Mesh tessellation :file:`n100-id1.tess`:

  .. code-block:: console

    $ neper -M n100-id1.tess

- Mesh 2D raster tessellation :file:`n100-id1.tesr`:

  .. code-block:: console

    $ neper -M n100-id1.tesr

- Mesh tessellation :file:`n100-id1.tess` with a mesh size of rcl = 0.5 and in 1st-order elements:

  .. code-block:: console

    $ neper -M n100-id1.tess -rcl 0.5

- Mesh tessellation :file:`n100-id1.tess` with small elements for the interior cells and bigger elements for the boundary cells:

  .. code-block:: console

    $ neper -M n100-id1.tess -rcl "body!=0?0.2:0.5"

- Remesh mesh :file:`n150_def.msh` (comprising poor-quality elements) into a clean, new mesh. Transport the scalar data of file :file:`n150_def.data` from the deformed mesh to the new mesh:

  .. code-block:: console

    $ neper -M n150.tess,n150_def.msh -transport elt:real1:n150_def.data -rcl 0.5 -o n150_new

- Mesh tessellation :file:`n100-id1.tess` and divide the mesh into 8 partitions:

  .. code-block:: console

    $ neper -M n100-id1.tess -part 8

- Mesh tessellation :file:`n100-id1.tess` into regular hexahedral elements (non-conformal mesh):

  .. code-block:: console

    $ neper -M n100-id1.tess -elt hex

- Mesh tessellation :file:`n100-id1.tess` and get, for each element, its radius ratio and its volume:

  .. code-block:: console

    $ neper -M n100-id1.tess -statelt rr,vol

- Mesh tessellation :file:`n100-id1.tess` and get the number of nodes and elements of the mesh:

  .. code-block:: console

    $ neper -M n100-id1.tess -statmesh nodenb,eltnb

References
----------

.. [CMAME2011] *R. Quey, P.R. Dawson and F. Barbe, Large-scale 3D random polycrystals for the finite element method: Generation, meshing and remeshing, Comput. Methods Appl. Mech. Engrg., vol. 200, pp. 1729-1745, 2011*.

.. [#partitioning] Each partition being assigned to a computation core in the finite element calculation, the minimization of the interfaces between the partitions is done in terms of the number of necessary communications between computation cores.

.. [#singular] In Mechanics, it corresponds to imposing a common displacement while the point can carry no stress, and, in Thermics, it corresponds to imposing a given temperature at a particular location shared by two bodies while the location can carry no heat flux.  When such a node is duplicated, each body has its own node and the singular behavior disappears.
