.. _ebsd_process:

Processing EBSD-type data
=========================

An experimental EBSD (orientation) map can be written as a :ref:`tesr_file` and visualized using the :ref:`neper_v`.  It can also be post-processed using the :ref:`neper_s`, just as a mesh (and simulation results) can be.

Writing EBSD-type data as a :ref:`tesr_file`
--------------------------------------------

An EBSD map typically consists of an orientation field described over a 2D grid.  This can be described by a :ref:`tesr_file`.  An example of an EBSD map comprising :math:`3 \times 4` pixels of size 0.001 is provided below (:file:`n2.tesr`):

.. literalinclude:: ebsd_process/n2.tesr
  :language: plain

.. note:: See the description of a :ref:`tesr_file` for details on the structure.  *Voxel* is used instead of *pixel* in the following, and in Neper in general.

The EBSD map can be visualized, colored by orientation, using the :ref:`neper_v`:

.. code-block:: console

  $ neper -V n2.tesr -datavoxcol ori -print img1

.. image:: ebsd_process/img1.png

In the EBSD map, it may also occur that a voxel is not assigned any orientation.  This can be indicated using the :data:`**oridef` section:

.. literalinclude:: ebsd_process/n2b.tesr
  :language: plain
  :emphasize-lines: 23-25

.. note:: Voxels for which the orientation is unknown (indicated :data:`0` in the :data:`**oridef` section) must still be assigned an orientation in the :data:`**oridata` section, as a placeholder.

The EBSD map can be visualized as before:

.. code-block:: console

  $ neper -V n2b.tesr -datavoxcol ori -print img2

.. image:: ebsd_process/img2.png

The "unindexed" voxels appear in gray by default, and their color can be changed using option :option:`-datavoxcol`.

Finally, cells (or "grains") can be defined using the :data:`**cell` and :data:`**data` sections:

.. literalinclude:: ebsd_process/n2c.tesr
  :language: plain
  :emphasize-lines: 8-14

.. note:: The lines highlighted above indicate that there are :data:`2` cells of numbers (or "ids") :data:`1` and :data:`2`.  The :data:`*id` section is optional if the cells are numbered contiguously from :data:`1`.  The :data:`**data` section should always contain numbers contiguously numbered from :data:`1` (:data:`0` can be used to represent empty voxels), where :data:`1` represents the first cell, :data:`2` the second cell, etc. (independently of the cell ids provided in :data:`*id`).

The EBSD map can be visualized, colored by its cell ids (which is the default), as follows:

.. code-block:: console

  $ neper -V n2c.tesr -print img3

.. image:: ebsd_process/img3.png

.. note:: The :data:`**data`, :data:`**oridata` and :data:`**oridef` sections are independent from each other, so that a cell may contain voxels that do not have any orientation, or a voxel that has an orientation may not belong to any cell.

Finally, it is a good idea to specify the crystal symmetry, using the :data:`*crysym` section:

.. literalinclude:: ebsd_process/n2d.tesr
  :language: plain
  :emphasize-lines: 12,13

Processing the Data
-------------------

A :ref:`tesr_file` (:file:`n2c.tesr`) can be written as a :ref:`simulation_directory`, for post-processing.  This is simply done as follows:

.. code-block:: console

  $ neper -T -loadtesr n2d.tesr -for sim

The resulting simulation directory is simply structured as follows:

.. code-block:: console

  n2d.sim
  └── inputs
      └── simulation.tesr

Cell properties can then be computed using option :option:`-rescell` of the :ref:`neper_s`, as follows:

.. code-block:: console

  $ neper -S n2d.sim -rescell ori

The resulting simulation directory is simply structured as follows:

.. code-block:: console

  n2d.sim
  ├── inputs
  │   └── simulation.tesr
  └── results
      └── cells
	  └── ori
	      └── ori.step0

.. note:: For consistency with actual simulation results, the result files are suffixed :file:`.step0`, which denotes the initial state.

The map can then be colored following its average cell orientations (compare to the first image):

.. code-block:: console

  $ neper -V n2d.sim -datacellcol ori -print img4

.. image:: ebsd_process/img4.png

Cell properties, such as the "grain orientation spread", :data:`gos`, can be computed:

.. code-block:: console

  $ neper -S n2d.sim -rescell gos

The resulting simulation directory is simply structured as follows:

.. code-block:: console

  n2d.sim
  ├── inputs
  │   └── simulation.tesr
  └── results
      └── cells
	  ├── gos
	  │   └── gos.step0
	  └── ori
	      └── ori.step0

The data are simply formatted and can be accessed easily:

.. code-block:: console

 $ more n2d.sim/results/cells/gos/gos.step0
 3.124415538011
 2.405535676705

The map can then be colored following its cell :data:`gos` values:

.. code-block:: console

  $ neper -V n2d.sim -datacellcol real:gos -datacellscale 2.0:3.0 -datacellscaletitle "GOS (degrees)" -print img5

This produces a PNG file named :file:`img5.png` for the map and a PNG file named :file:`img5-scale.png` for the scale bar.

To include the scale bar to the image, ImageMagick can be used:

.. code-block:: console

  $ convert img5.png img5-scale.png -gravity East -composite img5.png

.. image:: ebsd_process/img5.png
