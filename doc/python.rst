.. index::
   single: Python

.. _python_interface:

Python Interface
================

Neper's modules can be run from Python, using the :code:`neper` Python package, which is provided in the :file:`python` directory of the distribution.  Each module is a function, whose arguments are the module's command-line options:

.. code-block:: python

  import neper

  neper.T(n=100, id=1, o="poly")         # neper -T -n 100 -id 1 -o poly
  neper.M("poly.tess", elttype="hex")    # neper -M poly.tess -elttype hex
  neper.V("poly.tess", datacellcol="id", print="img")
                                         # neper -V poly.tess -datacellcol id -print img

The package runs the :program:`neper` program (in a separate process), so that it supports all Neper capabilities and requires no compilation, and a Neper error (which ends the :program:`neper` program) cannot end the Python program, but raises an exception instead.  It also reads Neper's files into NumPy arrays and passes NumPy arrays to Neper, so that tessellations, meshes and simulation results can be processed in Python:

.. code-block:: python

  tess = neper.T(n=100, id=1).read("tess")
  tess.seed_coo      # seed coordinates, NumPy array of shape (100, 3)

Installation
------------

The package requires Python 3.8 or higher and NumPy (installed automatically by pip; NumPy is needed only to read files and pass arrays).  It is installed using pip, from the :file:`python` directory of the distribution:

.. code-block:: console

  $ pip install ./python

Neper itself must also be installed (see :ref:`installation`).  The :program:`neper` program is looked for, successively, in environment variable :code:`NEPER_EXECUTABLE` (which can be the program or its directory), and in the :code:`PATH`.  It can also be set from Python, using :code:`neper.set_defaults(executable=...)`.

Running Modules
---------------

Modules -T, -M, -S and -V are run by functions :code:`neper.T`, :code:`neper.M`, :code:`neper.S` and :code:`neper.V`, respectively.  Their arguments are converted into command-line arguments and passed in order, positional arguments first:

- A positional argument that is a string (or a path) is passed as is.  This is typically the input file of module -M or -V, or the simulation directory of module -S, but it can also be any command-line argument, such as :data:`-loop` in module -V.

- A positional argument that is a list (or a tuple) is passed as a comma-separated argument, e.g. :code:`["n10.tess", "n10.msh"]` gives :data:`n10.tess,n10.msh`.

- A positional argument that is a dictionary is passed as options, in order.  This makes it possible to repeat an option, as needed by module -V (see below).

- A keyword argument is passed as an option: :code:`name=value` gives :data:`-name value`.  A trailing underscore is removed from the name (so that Python keywords can be used, e.g. :code:`in_` for :data:`-in`).  A value of :code:`None` omits the option, :code:`True` and :code:`False` give :data:`1` and :data:`0`, a list (or a tuple, or a 1D NumPy array) gives a comma-separated argument, e.g. :code:`format=["tess", "vtk"]` gives :data:`-format tess,vtk`, and a 2D array (or a list of lists) is written to a data file and gives :data:`file(\<file_name\>)` (see :ref:`python_passing_data`).  Integers, reals, paths and NumPy scalars are converted to strings.

String arguments are passed to Neper without going through a shell, so that they need no quotes, even when they contain shell metacharacters, e.g. :code:`morpho="diameq:lognormal(1,0.35)"`.

In module -V, options are read in batches that end with :option:`-print`, so that several images can be generated in one run by passing dictionaries:

.. code-block:: python

  neper.V("n10-id1.tess",
          {"datacellcol": "id", "print": "img1"},
          {"datacellcol": "ori", "print": "img2"})

Function :code:`neper.run` runs Neper with any command-line arguments, provided as a list or as a string (split as by a shell), as in :code:`neper.run("-T -n 100 -id 1")`.

Run Parameters
--------------

The following keyword arguments are not passed to Neper, but set how it is run:

- :code:`cwd`: working directory, i.e. where the input files are read and the output files are written (default: the current directory);

- :code:`threads`: number of OpenMP threads (default: :code:`OMP_NUM_THREADS`, or all threads, see :ref:`introduction`);

- :code:`rcfile`: initialization file (default: :file:`$HOME/.neperrc`); :code:`"none"` ignores the initialization file, which makes results independent of the user's settings;

- :code:`verbose`: print Neper's messages while it runs (default: :code:`False`);

- :code:`check`: raise an exception when Neper fails (default: :code:`True`);

- :code:`env`: environment variables to set (or, for a value of :code:`None`, to unset) for Neper, as a dictionary;

- :code:`executable`: :program:`neper` program.

Their default values can be changed using :code:`neper.set_defaults`, as in :code:`neper.set_defaults(threads=8, rcfile="none", verbose=True)`.

Results
-------

A function returns a result, whose main attributes are:

- :code:`files`: the files written by Neper, as a list of absolute paths (:code:`pathlib.Path`), in the order they were written.  Method :code:`file` returns the (first) written file of a given extension, or the first written file (the main output of the module), as in :code:`neper.T(n=100).file("tess")`;

- :code:`output`: Neper's messages (a string);

- :code:`returncode`: the exit status of :program:`neper` (:data:`0` on success);

- :code:`cmd`: the command line (a list of strings);

- :code:`elapsed`: the elapsed time (in seconds).

The output files can then be used as input of another module, or read in Python, using method :code:`read`, which reads the (first) written file of a given extension, or the first written file (see :ref:`python_reading_files`), as in :code:`neper.T(n=100).read("tess")`.  Method :code:`stat` reads the statistics of an entity as a dictionary indexed by the keys of the corresponding :data:`-stat*` option, as in:

.. code-block:: python

  res = neper.T(n=100, statcell="vol,sphericity")
  res.stat("cell")["vol"]       # NumPy array

In Jupyter, a result that includes an image (e.g., of module -V) is displayed as the image.

.. _python_reading_files:

Reading Files
-------------

Neper's files are read into NumPy arrays by function :code:`neper.read`, which calls, depending on the file extension:

- :code:`neper.read_tess` for a tessellation file (:file:`.tess`, see :ref:`tess_file`), which returns a :code:`neper.Tess`;

- :code:`neper.read_tesr` for a raster tessellation file (:file:`.tesr`, see :ref:`tesr_file`), which returns a :code:`neper.Tesr`;

- :code:`neper.read_mesh` for a mesh file (:file:`.msh`, ASCII or binary, see :ref:`mesh_file`), which returns a :code:`neper.Mesh`;

- :code:`neper.read_sim` for a simulation directory (see :ref:`simulation_directory`), which returns a :code:`neper.Sim` (see :ref:`python_simulation_directories`);

- :code:`neper.read_odf` for an orientation distribution function file (:file:`.odf`, see :ref:`odf_file`), which returns a :code:`neper.Odf` (with method :code:`mesh` to read the corresponding orientation-space mesh);

- :code:`neper.read_stat` for a statistics file (:file:`.st*`) or another table of numbers (e.g., an orientation file, :file:`.ori`), which returns an array with one row per entity and one column per value (or a list of arrays if the numbers of values differ, as for :data:`faces`).  If a list of keys is provided, as in :code:`neper.read_stat("n100-id1.stcell", "vol,sphericity")`, a dictionary (key → values) is returned instead.

Entities are numbered from 1, as in Neper: row :data:`i` of an array describes entity :data:`i + 1`, and references to entities are identifiers.  For example, the coordinates of the vertices of edge 1 are :code:`tess.ver_coo[tess.edge_vers[0] - 1]`.  The attribute names follow Neper's structures and keys (:data:`seed`, :data:`cell`, :data:`ver`, :data:`edge`, :data:`face`, :data:`poly`, :data:`node`, :data:`elt`, :data:`elset`, etc.), and :code:`print (x)` lists the attributes of an object :code:`x`.

A :code:`neper.Tess` contains:

- the cells: :code:`cell_qty`, :code:`seed_coo` (shape :code:`(n, 3)`), :code:`seed_weight`, :code:`cell_ori` (shape :code:`(n, k)`), :code:`cell_orides` (the orientation descriptor, such as :data:`rodrigues:passive`), :code:`cell_crysym`, and, if any, :code:`cell_id`, :code:`cell_lam`, :code:`cell_mode`, :code:`cell_group`, :code:`cell_orispread` and :code:`cell_weight`;

- the vertices, edges, faces and polyhedra: :code:`ver_coo` (shape :code:`(n, 3)`), :code:`edge_vers` (shape :code:`(n, 2)`), :code:`face_vers`, :code:`face_edges`, :code:`face_eq` (shape :code:`(n, 4)`, :data:`d`, :data:`a`, :data:`b` and :data:`c`, such that :math:`a \, x + b \, y + c \, z = d`) and :code:`poly_faces`, where :code:`face_vers`, :code:`face_edges` and :code:`poly_faces` are lists of arrays (with signed edges and faces, as in the file) and :code:`ver_state`, :code:`edge_state`, :code:`face_state`, :code:`face_pt` and :code:`face_ptcoo` are also available;

- the domain: :code:`dom_type`, :code:`dom_ver_*`, :code:`dom_edge_*` and :code:`dom_face_*` (such as :code:`dom_face_label` and :code:`dom_face_tessfaces`);

- the periodicity (if any): :code:`periodic`, :code:`periodic_dist`, :code:`per_seed`, :code:`per_ver`, :code:`per_edge` and :code:`per_face`, and the scales (if any): :code:`scale_qty` and :code:`cell_scaleid`.

A :code:`neper.Tesr` contains :code:`size`, :code:`vsize` and :code:`origin`, the cells of the voxels, :code:`vox_cell`, an integer array indexed as :code:`[x, y, z]` (or :code:`[x, y]` in 2D), the voxel orientations, :code:`vox_ori` (indexed as :code:`[x, y, z, k]`), :code:`vox_orides` and :code:`vox_oridef` (if any), and the cell data (:code:`cell_qty`, :code:`seed_coo`, :code:`cell_ori`, etc.).

A :code:`neper.Mesh` contains the node coordinates, :code:`node_coo` (shape :code:`(n, 3)`), and the elements, as blocks of elements of a same type, :code:`blocks`.  Method :code:`elts` returns the block of a given dimension (default: the mesh dimension), whose attributes are :code:`type` (such as :data:`tet` or :data:`hex`), :code:`order`, :code:`elt_id`, :code:`elt_nodes` (shape :code:`(n, k)`), :code:`elt_elset` and :code:`elt_part`.  The mesh also contains (if any) :code:`nsets` and :code:`fasets` (dictionaries indexed by label), :code:`node_part`, :code:`periodicity`, :code:`elset_ori`, :code:`elset_orides`, :code:`elset_crysym`, :code:`elt_ori`, :code:`elt_orides`, :code:`elset_group` and :code:`physical_names`.

For example:

.. code-block:: python

  import numpy as np

  tess = neper.T(n=100, id=1).read()
  # vertices of polyhedron 1
  faces = np.abs(tess.poly_faces[0])
  vers = np.unique(np.concatenate([tess.face_vers[f - 1] for f in faces]))

  mesh = neper.M("n100-id1.tess", elttype="tet", order=1).read()
  elts = mesh.elts()
  centroids = mesh.node_coo[elts.elt_nodes - 1].mean(axis=1)

  vol, sph = neper.read_stat(neper.T(n=100, statcell="vol,sphericity").file("stcell")).T

.. _python_simulation_directories:

Simulation Directories
----------------------

A simulation directory (see :ref:`simulation_directory`) is read by :code:`neper.read_sim` (or :code:`neper.read`), which returns a :code:`neper.Sim`, and its results are read on demand.  A :code:`neper.Sim` provides:

- the input files, :code:`inputs` (a dictionary indexed by extension, such as :data:`msh`), which are read by methods :code:`tess`, :code:`tesr` and :code:`mesh` (once);

- the result entities, :code:`entities` (such as :data:`nodes`, :data:`elts`, :data:`elsets` and :data:`mesh`), and the steps, :code:`steps`;

- the orientation descriptor of the orientation results, :code:`orides` (that of the input mesh or tessellation);

- method :code:`results`, which returns the results of an entity, and methods :code:`result_type` and :code:`result_steps`, which return the type (from the definition file or for a result of known type, see :ref:`simulation_results`) and the steps of a result;

- method :code:`read`, which reads a result at a step, as an array of shape :code:`(n, k)` (:code:`(n,)` for a single component), or at all steps, as an array of shape :code:`(steps, n, k)` (or :code:`(steps, n)`), and method :code:`read_all`, which reads all results, as a dictionary (entity → result → array);

- method :code:`write`, which writes a result (at a step or all steps, as for :code:`read`), with its definition file, so that it can be used as any other result by modules -S and -V (a result used in an expression of module -S must be written for all steps), and method :code:`remove`, which removes a result.

Results can then be computed by module -S (:code:`neper.S`) or in Python, and plotted or visualized by module -V.  For example:

.. code-block:: python

  import numpy as np
  import matplotlib.pyplot as plt

  neper.S("n100.sim", resnode="disp", reselset="vol")
  sim = neper.read("n100.sim")

  disp = sim.read("nodes", "disp")             # (steps, nodes, 3)
  top = sim.mesh().nsets["z1"] - 1
  plt.plot(sim.steps, disp[:, top, 2].mean(axis=1))

  vol = sim.read("elsets", "vol")              # (steps, elsets)
  sim.write("elsets", "volchange", vol / vol[0] - 1)
  neper.V("n100.sim", step=sim.steps[-1], dataelsetcol="real:volchange", print="img")

When the simulation directory is modified by module -S, method :code:`refresh` updates the entities and steps.

Orientations
------------

Orientations are converted between descriptors (see :ref:`rotations_and_orientations`) by :code:`neper.ori_convert`, following Neper's conventions:

.. code-block:: python

  tess = neper.read("n100-id1.tess")
  euler = neper.ori_convert(tess.cell_ori, tess.cell_orides, "euler-bunge")

The descriptors are :data:`rodrigues`, :data:`euler-bunge`, :data:`euler-kocks`, :data:`euler-roe`, :data:`rotmat` (9 components, row by row), :data:`axis-angle` and :data:`quaternion`, optionally followed by the convention (:data:`:passive`, the default, or :data:`:active`).  Angles are in degrees, and the values are those that Neper writes (e.g., Euler angles in :math:`[0,\,360) \times [0,\,180] \times [0,\,360)`).  The arrays can have any shape, with the parameters along the last axis.  :code:`neper.ori_angle` returns the rotation angle between orientations (not accounting for crystal symmetry), as in :code:`neper.ori_angle(ori, ori[0], sim.orides)` for the rotations of the grains of a simulation from step 0.

.. _python_orilib:

Orilib
------

The functions of Orilib, Neper's library of orientation functions, are available in Python in module :code:`neper.orilib`, under their Orilib names (see :ref:`orilib_commands`).  They apply to NumPy arrays:

.. code-block:: python

  from neper import orilib

  orilib.ol_e_q([0, 30, 0])                     # quaternion, shape (4,)
  q = orilib.ol_e_q(euler)                      # euler: shape (n, 3) -> (n, 4)
  orilib.ol_q_q_disori(q, q[0], "cubic")        # disorientation angles, shape (n,)
  orilib.ol_q_crysym(q, "cubic", 7)             # equivalent orientations, shape (n, 4)

Each argument is an array whose last axis holds the values of one item, e.g. the 4 components of a quaternion (or whose two last axes hold a rotation matrix), and whose leading axes index the items.  The leading axes of the arguments are broadcast (as in NumPy), so that a single item can be combined with several items, as :code:`q[0]` above.  An integer or string argument (e.g. a crystal symmetry) applies to all items, or is given per item (as an array).  The results have the leading axes, followed by the axes of an item.  A function that returns several values returns a tuple, and a function that returns a list of values whose length differs between the items (e.g. :code:`ol_polef_polecrysym`) returns a list of the item results.

The arguments and results of a function are given by :code:`orilib.signature`, as in :code:`orilib.signature("ol_q_q_disori")`, which gives :data:`ol_q_q_disori (q1: q, q2: q, crysym: str) -> theta: theta`, and by :code:`help (orilib.ol_q_q_disori)`, which also prints the description of the corresponding C function.  The types are those of Orilib: :data:`e` (Euler angles, in degrees), :data:`g` (rotation matrix, shape :code:`(3, 3)`), :data:`q` (quaternion), :data:`R` (Rodrigues vector), :data:`rtheta` (rotation axis / angle pair), :data:`m` (Miller indices), :data:`theta` (angle), :data:`vect` (vector), :data:`pole`, etc.  :code:`orilib.commands` lists the functions.

The functions and their arguments are those of the :program:`orilib` program (see :command:`orilib signatures`).  They run the program (once per call, for all items, about 0.5 s for 100,000 items), which is looked for, successively, in environment variable :code:`ORILIB_EXECUTABLE`, next to the :program:`neper` program, and in the :code:`PATH`.  An error of Orilib (e.g. an unknown crystal symmetry) raises a :code:`neper.orilib.OrilibError` exception.  The functions that do not apply to individual items (:data:`ol_g_m`, :data:`ol_q_m`, :data:`ol_set_filter` and :data:`ol_set_filter_iter`) are not available, but any command can be run with :code:`orilib.run`, which takes and returns text.

.. _python_passing_data:

Passing Data
------------

Data are passed to Neper as data files (see :ref:`data_file`), and NumPy arrays can be written to data files using function :code:`neper.datafile`, which returns an object that gives :data:`file(\<file_name\>)` when passed to Neper, as an option value or in a string.  The data file is written to a temporary directory (deleted when Python exits), or to a given path (argument :code:`path`).  A 2D array (or a list of lists) passed as an option value is written to a data file automatically.  For example:

.. code-block:: python

  import numpy as np

  ori = np.random.uniform(-0.3, 0.3, (100, 3))          # Rodrigues vectors
  neper.T(n=100, ori=ori)                               # -ori file(<file_name>)

  seeds = np.random.random((100, 3))
  neper.T(n=100, morphooptiini=f"coo:{neper.datafile(seeds)}")

  tess = neper.read("n100-id1.tess")
  neper.V("n100-id1.tess", datacellcol=f"real:{neper.datafile(tess.seed_coo[:, 2])}",
          print="img")

Errors
------

When Neper fails (e.g., because of an invalid option or a missing file), a :code:`neper.NeperError` exception is raised.  Its message is Neper's error message, and its attributes include :code:`returncode` and :code:`output` (Neper's messages):

.. code-block:: python

  try:
      neper.T(n=100, morpho="foo")
  except neper.NeperError as e:
      print(e.message)    # Unknown argument `foo'.

Using :code:`check=False`, no exception is raised and the result can be tested instead (attribute :code:`ok`).

Interrupting Python (:kbd:`Ctrl+C`) interrupts Neper, which then behaves as when it is interrupted from a terminal: during the morphology optimization of module -T, the optimization stops and Neper continues and completes (and the function returns normally), while at other stages Neper stops (and a :code:`KeyboardInterrupt` exception is raised).  A second interruption stops Neper in all cases.

Example
-------

The following script generates a polycrystal with a grain size distribution, meshes it, and prints an image of the mesh colored by grain orientation (see also :ref:`python_tutorial`):

.. code-block:: python

  import neper

  neper.set_defaults(rcfile="none")

  tess = neper.T(n=100, id=1, morpho="diameq:lognormal(1,0.35),1-sphericity:lognormal(0.145,0.03)",
                 reg=True).file("tess")
  mesh = neper.M(tess, rcl=0.7).file("msh")
  neper.V([tess, mesh], dataelsetcol="ori", print="img")

Since Neper is run as a separate process, several Neper runs can also be carried out in parallel, e.g. using :code:`concurrent.futures.ThreadPoolExecutor` (with a reduced number of threads each, and different output files).
