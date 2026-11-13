.. _whatsnew:

What's New
==========

This page lists the main new capabilities of Neper version 7.0.0 since version 5.0.0, starting with some highlights.  Several of them change existing behavior or remove options, see :ref:`whatsnew_incompatible`.

Highlights
----------

Python Interface
~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      Neper can now be used from Python, using the :code:`neper` Python package: the modules are run as Python functions, whose arguments are the command-line options, Neper's files (tessellations, meshes, simulation directories, statistics, etc.) are read into NumPy arrays, NumPy arrays are passed to Neper as data, and simulation results computed in Python can be written back for the :ref:`neper_s` and the :ref:`neper_v`.  The functions of Neper's orientation library, Orilib, also apply to NumPy arrays.  Here, the rotations of the grains during a simulation are computed and plotted in Python.

      .. code-block:: python

        import neper

        sim = neper.read("n20.sim")
        ori = sim.read("elsets", "ori")           # (steps, grains, 3)
        rotation = neper.ori_angle(ori, ori[0], sim.orides)

      See :doc:`doc/python` and :doc:`doc/tutorials/python`.

   .. grid-item::

      .. image:: imgs/whatsnew/python.png
         :class: neper-whatsnew-img

Faster Execution
~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      All modules run faster, with results of the same quality.  For a 1000-cell grain-growth tessellation (:data:`-n 1000 -morpho gg`), on 12 threads, tessellation (:data:`-T`) is about 3 times faster (about 1 min instead of 3 min), meshing (:data:`-M`) about 35 times faster (a few seconds instead of more than 1 min), and visualization (:data:`-V`) of a cut through the mesh, at 2400 × 2400 pixels, about 1.3 times faster (about 4 s instead of 5 s).

   .. grid-item::

      .. image:: imgs/whatsnew/faster-execution.png
         :class: neper-whatsnew-img

Improved 3D Regularization
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      The 3D regularization (:option:`-reg`) has been improved: for a 1000-cell grain-growth tessellation, it leaves about 9 times fewer edges below the length threshold (53 instead of 475, out of about 1500 before regularization, shown in red).

      .. code-block:: console

        $ neper -T -n 1000 -morpho gg -reg 1

   .. grid-item::

      .. image:: imgs/whatsnew/reg-edges.png
         :class: neper-whatsnew-img

Regularization of Periodic Tessellations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      Periodic tessellations can now be regularized, which removes their small edges and so the local mesh overrefinements.

      .. code-block:: console

        $ neper -T -n 300 -morpho gg -periodicity all -o d -reg 1
        $ neper -M d.tess -rcl 0.8 -pl 10

      See :doc:`doc/tutorials/mesh_tessellation`.

   .. grid-item::

      .. image:: imgs/whatsnew/periodic-reg.png
         :class: neper-whatsnew-img

Multiscale Periodic Tessellations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      Multiscale tessellations can now be periodic, and they can also be regularized.  Here, grain-growth primary cells are divided into Voronoi secondary cells, whose number is proportional to the volume of the primary cell.

      .. code-block:: console

        $ neper -T -n "300::vol*300*3" -morpho "gg::voronoi" -periodicity all -o d -reg 1
        $ neper -M d.tess -rcl 0.8 -pl 10

      See :doc:`doc/tutorials/multiscale_tessellation` and :doc:`doc/tutorials/periodic_tessellation`.

   .. grid-item::

      .. image:: imgs/whatsnew/multiscale-periodic.png
         :class: neper-whatsnew-img

Conformal Hexahedral Meshing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      Tessellations can now be meshed into hexahedral elements that conform to the cell boundaries (:data:`-elttype hex`), as done by Gmsh: the cells are meshed into tetrahedral elements, each of which is then split into 4 hexahedral elements.  The mesh is conformal between the cells and follows curved domains, and it can be periodic.  The element quality is lower than for tetrahedral elements.  In 2D, :data:`-elttype quad` recombines the triangular elements into quadrangular elements before splitting them, which provides a better quality.  The former, non-conforming meshes into regular elements are now obtained using :data:`-elttype hexgrid` and :data:`quadgrid`.  Here, the mesh is shown with half of the elements removed.

      .. code-block:: console

        $ neper -T -n 100 -morpho gg -reg 1 -o d
        $ neper -M d.tess -elttype hex
        $ neper -V d.tess,d.msh -showelt3d "y>0.5" -dataelt3dedgerad 0.0015 -cameraangle 12 -print img

      See :doc:`doc/tutorials/mesh_tessellation`.

   .. grid-item::

      .. image:: imgs/whatsnew/hex-mesh.png
         :class: neper-whatsnew-img

Mesh Size Map
~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      The element sizes inside the faces and cells are now set by a mesh size map: they increase from the boundaries at a rate set by :option:`-pl`, up to the specified value, so that, for a given value of :data:`-rcl`, the mesh generally contains fewer elements.  The edges follow the same map.  Sizes can be set for the vertices, edges, faces and polyhedra, using :data:`-[r]clpoly` in particular, and progression factors for the edges, faces and polyhedra, using :data:`-pl{edge,face,poly}`.  Here, the mesh of a grain of a grain-growth polycrystal, cut at its centroid, is finer at the edges than at the faces, and at the faces than in the volume.

      .. code-block:: console

        $ neper -T -n 50 -morpho gg -reg 1 -o d
        $ neper -M d.tess -meshpoly "id==2" -faset none -rcl 1.5 -rclface 0.2 -rcledge 0.04 -pl 1.4
        $ neper -V d.tess,d.msh -showelt3d "y>0.38" -showelt1d "elset1d>=13&&elset1d<=48" -print img

      See :doc:`doc/tutorials/mesh_tessellation`.

   .. grid-item::

      .. image:: imgs/whatsnew/mesh-size-map.png
         :class: neper-whatsnew-img

.. _whatsnew_curved_domains:

Exact Curved Domains
~~~~~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      Cylindrical and spherical domains are now exact (curved) instead of faceted, unless a number of facets is requested, and they remain exact when translated or rotated.  An hourglass domain has been added.  Meshes follow the curved surfaces, including their second-order nodes, and large faces are meshed more accurately.

      .. code-block:: console

        $ neper -T -n 1000 -morpho gg -morphooptistop val=1e-2 -reg 1 -domain "hourglass(1.5,1,0.95)"
        $ neper -M n1000-id1.tess

      See :doc:`doc/tutorials/domain_cutting`.

   .. grid-item::

      .. image:: imgs/whatsnew/hourglass-mesh.png
         :class: neper-whatsnew-img

Discontinuous and Cohesive Elements for Periodic and Multiscale Tessellations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Option :option:`-interface` can now produce discontinuous and cohesive interfaces for periodic tessellations, where an interface between a cell and the periodic image of another cell is described at its master location, and for multiscale tessellations, with a different value for each scale (e.g., cohesive elements between the primary cells only).

.. code-block:: console

  $ neper -M per.tess -interface cohesive
  $ neper -M n10x10.tess -interface "cohesive::continuous"

Custom Node Sets
~~~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      Option :option:`-nset` can now define node sets from expressions, as :data:`\<label\>:\<expr\>`, and from :data:`min`, :data:`max` and :data:`closest`.  A node set can then be shown using :data:`-shownode nset==\<label\>`.  Here, a node set is defined at the center of the top face of the hourglass sample of :ref:`whatsnew_curved_domains`.

      .. code-block:: console

        $ neper -T -n 1000 -morpho gg -morphooptistop val=1e-2 -reg 1 -domain "hourglass(1.5,1,0.95)"
        $ neper -M n1000-id1.tess -nset "faces,edges,vertices,topcenter:closest(0.5,0.5,1.5)"
        $ neper -V n1000-id1.tess,n1000-id1.msh -shownode nset==topcenter -datanodecol red -datanoderad 0.04

      See :doc:`doc/tutorials/mesh_tessellation`.

   .. grid-item::

      .. image:: imgs/whatsnew/custom-nset.png
         :class: neper-whatsnew-img

Orilib Program
~~~~~~~~~~~~~~

.. grid:: 1 1 2 2
   :gutter: 4

   .. grid-item::

      The :program:`orilib` program can run most of the functions of the Orilib library from the command line, interactively or in scripts, for example to convert orientations between descriptors or to compute disorientations:

      .. code-block:: console

        $ echo 0 30 0 | orilib ol_e_q
         0.965925826  0.258819045  0.000000000  0.000000000

      It is installed together with :program:`neper`, and its functions are also available in Python (see :ref:`python_orilib`).  See :ref:`orilib_program`.

   .. grid-item::

      .. image:: imgs/whatsnew/orilib-terminal.png
         :class: neper-whatsnew-img

Tutorials
~~~~~~~~~

The tutorials have been extended and rewritten, and they now cover most capabilities of Neper, from tessellation generation to meshing, visualization and post-processing of simulation results.  Each tutorial is a step-by-step guide whose commands can be run directly, and they are part of the documentation:

- :doc:`doc/tutorials/voronoi_tessellation`
- :doc:`doc/tutorials/domain_cutting`
- :doc:`doc/tutorials/regular_tessellation`
- :doc:`doc/tutorials/morpho_statistics`
- :doc:`doc/tutorials/periodic_tessellation`
- :doc:`doc/tutorials/multiscale_tessellation`
- :doc:`doc/tutorials/cell_groups`
- :doc:`doc/tutorials/ori_statistics`
- :doc:`doc/tutorials/mesh_tessellation`
- :doc:`doc/tutorials/visualize_tessellation`
- :doc:`doc/tutorials/visualize_mesh`
- :doc:`doc/tutorials/pole_figure`
- :doc:`doc/tutorials/visualize_ori`
- :doc:`doc/tutorials/morpho_tesr_mesh`
- :doc:`doc/tutorials/simulation_results`
- :doc:`doc/tutorials/ebsd_process`
- :doc:`doc/tutorials/orientation_color_key`
- :doc:`doc/tutorials/orientation_trajectories`

New Capabilities by Module
--------------------------

Tessellation Module (-T)
~~~~~~~~~~~~~~~~~~~~~~~~

- Exact cylinder and sphere domains, also for a cylinder periodic along its axis, and hourglass domain (:option:`-domain`).  The curved domain faces are labeled :data:`cyl` or :data:`hourglass`.
- Improved cutting algorithm (direct cutting).
- Periodic multiscale tessellations.
- :data:`-transform rmseed`, which removes seeds matching expressions and recomputes the tessellation, and the seed keys :data:`cell_\<key\>`, which give access to the keys of the cell of a seed.
- :data:`-transform renamedomface`, which renames a domain face.
- :option:`-regdelcell`, which allows regularization to delete cells.
- Improved 3D regularization.
- Expressions in all :data:`-stat` options.
- :data:`curpdf0` value of :option:`-morphooptilogdis`, for the current, not smoothed PDF.
- Faster morphology optimization: about 25% faster computation of the cells, and faster neighbor search.
- Deterministic morphology optimization, and results of regularization that do not depend on the build.

Meshing Module (-M)
~~~~~~~~~~~~~~~~~~~

- Conformal hexahedral and quadrangular meshing, by splitting tetrahedral and triangular elements, as Gmsh does, and, in 2D, recombining triangular elements first (:data:`-elttype hex` and :data:`quad`).
- Mesh size map, for the edges, faces and cells (:option:`-pl`), with :data:`-[r]clpoly`, :data:`-pl{edge,face,poly}` and :option:`-mesh2dclreps`.
- Custom node sets, as :data:`\<label\>:\<expr\>`, :data:`min`, :data:`max` and :data:`closest` (:option:`-nset`).
- :data:`discontinuous` and :data:`cohesive` interfaces for periodic tessellations (:option:`-interface`).
- Second-order nodes projected onto the curved faces and edges.
- Improved meshing of the curved faces: conformal projection for the sphere, and conformal unrolling for the hourglass and torus cuts.
- Any number of partitions, not necessarily a power of 2 (:option:`-part`).
- Node sets kept when a mesh is loaded without its tessellation, and updated by :data:`-order` and :data:`-sortnode`.
- Expressions in all :data:`-stat` options.
- Reproducible parallel grid meshing of regularized tessellations (:data:`-elttype hexgrid`).
- Abaqus surfaces for hexahedral elements (:data:`-format inp`).

Simulation Module (-S)
~~~~~~~~~~~~~~~~~~~~~~

- :data:`\<res\>_eq` element, elset and mesh results, which give the equivalent value of a symmetric tensor result, as computed by FEPX (e.g., :data:`stress_eq`).
- Simplified simulation directories: the directory content is read directly, without a :file:`.sim` file, and definitions are written to :file:`.def` files.  Input files are identified by their extensions and are not necessarily named :file:`simulation.*`.
- :data:`type=` argument of :data:`file()`, and steps of a simulation directory without results set from the :file:`.step*` files of :data:`file()`.
- Case-insensitive keywords and values in :file:`simulation.cfg`.
- Steps without result files skipped during post-processing.
- Clear error messages for invalid simulation directories, configuration files and results.

Visualization Module (-V)
~~~~~~~~~~~~~~~~~~~~~~~~~

- Sample symmetry in orientation space (:option:`-orisym`).
- Highlighting colors :data:`hl1`, :data:`hl2` and :data:`hl3`.
- Node sets in the expressions of :data:`-shownode` (:data:`nset==\<label\>`).
- :data:`-crysym` for :data:`-space rodrigues`.
- Clear errors when POV-Ray or Asymptote is missing or does not generate a file.

General
~~~~~~~

- Python interface (:code:`neper` package), which runs the modules from Python, reads Neper's files into NumPy arrays, passes NumPy arrays to Neper and writes simulation results, see :doc:`doc/python`.
- :program:`orilib` program, which can run most of the functions of the Orilib library from the command line (installed together with :program:`neper`) and from Python, see :ref:`orilib_program`.
- Fewer dependencies: NLopt is no longer needed.
- macOS compatibility fixes, and Bash completion for all options.
- Numerous bug fixes in all modules, including memory errors found with AddressSanitizer.

.. _whatsnew_incompatible:

Incompatible Changes
--------------------

Tessellation Module (-T)
~~~~~~~~~~~~~~~~~~~~~~~~

.. warning::

  Seed weights are now the actual Laguerre weights, :math:`w` (power distance :math:`d^2 - w`), instead of their square roots.  Tessellations are unchanged, but the weight values are not.  In particular, :data:`-morphooptiini weight:radeq` (and :data:`avradeq`, etc.) must be replaced by :data:`weight:radeq^2` (and :data:`avradeq^2`, etc.): **the former syntax still runs, with no warning, but sets a different initial solution**.  Custom weight values (real values, data files, multiscale cell files) must be squared too.

- The :file:`tess` and :file:`tesr` file format versions are now 3.6 and 2.3.  Older files can still be read, and their weights are converted; new files cannot be read by former versions.
- :data:`-morphooptiboundlower/upper w:`, and the :data:`w` keys of :data:`-morphooptilogvar` and :data:`-statcell` / :data:`-statseed`, now apply to :math:`w`, while :data:`-morphooptideltamax` applies to :math:`\sqrt{w}`, the optimization variable.
- :data:`-orioptiini` now uses :data:`:` instead of :data:`=`, as :data:`-morphooptiini` does (e.g., :data:`weight:file(\<file\>)` instead of :data:`weight=\<file\>`).  The former syntax is now an error.
- :data:`-regularization`, :data:`-fmax`, :data:`-[r]sel`, :data:`-mloop` and :data:`-maxedgedelqty` are renamed :option:`-reg`, :option:`-regfmax`, :data:`-reg[r]sel`, :option:`-regloop` and :data:`-regedgedelmax` (the former names are deprecated).
- :data:`[r]sel` is removed from :data:`-morpho`.
- The transformations of :data:`-loadtesr` are removed (use :data:`-transform`).
- Only the built-in :data:`subplex` optimization algorithm remains (:data:`praxis`, :data:`neldermead`, :data:`cobyla`, :data:`bobyqa` and :data:`newuoa` are removed).

Meshing Module (-M)
~~~~~~~~~~~~~~~~~~~

- The Gmsh library is now mandatory (version 4.9 or higher), and the :command:`gmsh` program is no longer used: :data:`-gmsh` and :data:`-tmp` are obsolete.
- The 3D meshing algorithm :data:`netg` is no longer available.
- The element sizes change as a result of the mesh size map.
- The node order of the :data:`$Fasets` elements is now the Gmsh order (it was reversed), and the :file:`msh` format version is now 2.4 (version 2.3 can still be read).
- Nodes and elements are no longer renumbered by partition after partitioning (left to FEPX).
- :option:`-part` now takes only a number of partitions, and :data:`-partmethod` is removed (libscotch is always used).
- The transformations of the tesr input are removed (use :data:`neper -T -transform`).
- :data:`-elttype hex`, :data:`quad` and :data:`quad9`, which provided non-conforming meshes into regular elements, are renamed :data:`hexgrid`, :data:`quadgrid` and :data:`quad9grid` (:data:`quad9` is deprecated), and :data:`-elttype hex` and :data:`quad` now provide conformal meshes.
- The :data:`msh4` format (Gmsh version 4 mesh file) is removed from :option:`-format`, and unknown values of :option:`-format` are now errors.

Simulation Module (-S)
~~~~~~~~~~~~~~~~~~~~~~

- Simulation directories no longer use a :file:`.sim` file (the definitions of a legacy file are migrated).
- The old simulation directory format (report file) can no longer be read.
- The conversion of FEPX raw result directories (:file:`post.report`) is removed.
- :data:`-step` is removed.
- :data:`-orispace` now only accepts :data:`rodrigues(fr\<N\>)`.
- ODF-based results are not available for the triclinic crystal symmetry.

Visualization Module (-V)
~~~~~~~~~~~~~~~~~~~~~~~~~

- :data:`-colormode` (dark color mode), :data:`-povrayantialiasing` (antialiasing is always used) and :data:`-outdir` (images are written relative to the current directory) are removed.
