.. _ori_statistics:

Specifying Orientation Information for Tessellation Cells
=========================================================

.. important:: Read first: :ref:`simple_model` and (optionally) :ref:`orientation_space` and :ref:`pole_figure`.

Orientations for the cells can be specified as input, using option :option:`-ori`. By default, "random orientations" are generated:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic

This generates a :ref:`tess_file` named :file:`n1000-id1.tess`.

.. note::

  - In general, it is important to define the crystal symmetry, using :option:`-crysym`.
  - It is possible to generate only orientations (no actual tessellation) using :option:`-format` :data:`ori`, which only produces an orientation file (technically a :ref:`data_file`) of extension :file:`.ori`.
  - In the case where an actual tessellation is generated, the cell orientations are generated after (and fully independently of) the tessellation morphology.

The orientations can be visualized on a pole figure using the :ref:`neper_v` and :option:`-space` :data:`pf`:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img1

.. image:: ori_statistics/img1.png

We recognize a distribution of random orientations.  The apparent higher density in the center is due to the use of the stereographic projection (see :option:`-pfprojection`), and an equal-area projection would effectively produce uniform density.

.. _specifying_an_odf:

Using an ODF
------------

An ODF can be specified by providing a mesh of orientation space and intensity values at the mesh elements.  A mesh of the fundamental region of Rodrigues space (for the considered crystal symmetry, here *cubic*) can be generated as follows (see tutorial ":ref:`orientation_space`"):

.. code-block:: console

  $ neper -T -n 1 -domain "rodrigues(cubic)" -o fr
  $ neper -M fr.tess -cl 0.05

The elemental values of the ODF, which are defined externally, are written in an external file, :download:`odf.txt <ori_statistics/odf.txt>`.  In the present case, it defines a generic rolling texture of an FCC material.  The mesh of orientation space (generated as described above) also is provided by an external file, :download:`fr.msh <ori_statistics/fr.msh>`.

Cell orientations can be generated from the ODF as follows:

.. code-block:: console

  $ neper -T -n 1000 -ori "odf(mesh=file(fr.msh),val=file(odf.txt))" -crysym cubic

.. important:: The crystal symmetry must be specified using :option:`-crysym` and should be the same as the one of the orientation space (:file:`fr.msh`); otherwise, an error is generated.

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img2

.. image:: ori_statistics/img2.png

We recognize a typical rolling texture.

Using Discrete Orientations and Orientation Fibers
--------------------------------------------------

An orientation distribution can be defined from "base", discrete orientations and orientation fibers (see :option:`-ori` and :ref:`rotations_and_orientations`), optionally surrounded by distributions.

A simple case of a Cube-type orientation distribution can be obtained as follows:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic -ori "Cube:normal(8)"

.. note:: ":data:`normal(8)`" generates a distribution about the nominal orientation corresponding to a tri-variate normal distribution (in tangent space, :math:`r\,\theta`) of a (1D) standard deviation equal to 8 degrees.  When applied to an orientation fiber, the distribution becomes bi-variate (perpendicular to the fiber).

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img3

.. image:: ori_statistics/img3.png

A simple case of a :math:`\left<1 0 0\right> | X_i` fiber surrounded by a distribution of a 1D standard deviation of 4 degrees can be obtained as follows:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic -ori "fiber(1,0,0,1,0,0):normal(4)"

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img5

.. image:: ori_statistics/img5.png

Several orientations (or orientation fibers) can be used to produce more complex distributions, each with its own weight and distribution.
An example is as follows:

.. code-block:: console

  $ neper -T -n 1000 -crysym cubic -ori "2*Brass1:normal(3.9)+2*Brass2:normal(3.9)+S1:normal(4.7)+S2:normal(4.7)+S3:normal(4.7)+S4:normal(4.7)+0.8*Copper1:normal(5.6)+0.8*Copper2:normal(5.6)+Cube:normal(3.9)+2*Goss:normal(3.9)"

The orientations can be visualized as before:

.. code-block:: console

  $ neper -V n1000-id1.tess -imagesize 500:500 -space pf -print img4

.. image:: ori_statistics/img4.png

Incidentally, it is similar to the one generated in :ref:`specifying_an_odf`.
