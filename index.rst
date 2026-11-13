.. _neper:

Neper: Polycrystal Generation, Meshing and Post-Processing
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. grid:: 1 1 2 2
   :gutter: 3

   .. grid-item::
      :columns: 12 12 8 8

      .. image:: imgs/home-neper.png
         :class: neper-hero

   .. grid-item::
      :columns: 12 12 4 4
      :child-align: center

      Neper is a free / open-source software package for polycrystal studies.  It generates 2D and 3D polycrystals, from simple tessellations to multiphase or multiscale microstructures, together with their crystal orientations, and meshes them into high-quality finite-element meshes.  It also post-processes simulation results (especially from FEPX), and visualizes all of them in publication-quality images.

      .. container:: neper-buttons

         .. button-ref:: downloads
            :ref-type: doc
            :color: primary

            Download

         .. button-ref:: doc/quickstart
            :ref-type: doc
            :color: primary
            :outline:

            Get started

         .. button-ref:: whatsnew
            :ref-type: doc
            :color: primary
            :outline:

            What's New

.. grid:: 1 2 2 4
   :gutter: 3

   .. grid-item-card:: Generation (-T)
      :img-top: imgs/home-tess.png
      :class-img-top: neper-card-img
      :link: gallery_generation
      :link-type: ref

      Tessellations from simple to multiphase or multiscale morphologies.

   .. grid-item-card:: Meshing (-M)
      :img-top: imgs/home-mesh.png
      :class-img-top: neper-card-img
      :link: gallery_meshing
      :link-type: ref

      Meshes of tessellations, with uniform or per-grain mesh sizes.

   .. grid-item-card:: Simulation (-S)
      :img-top: imgs/home-sim.png
      :class-img-top: neper-card-img
      :link: gallery_post_processing
      :link-type: ref

      Simulation directories from FEPX results, and their post-processing.

   .. grid-item-card:: Visualization (-V)
      :img-top: imgs/home-visu.png
      :class-img-top: neper-card-img
      :link: gallery_visualization
      :link-type: ref

      Images of tessellations, meshes and simulation results.

.. tip::

  Neper can also be used from Python: the modules run as Python functions, and tessellations, meshes and simulation results are read into NumPy arrays.  See :doc:`doc/python`.

.. note::

  See also `FEPX <https://fepx.info>`_, Neper's companion program: a finite element software package for polycrystal plasticity, whose meshes Neper generates and whose results it post-processes.

Neper is developed by Romain Quey at `CNRS <http://www.cnrs.fr/en>`_ and `Mines Saint-Etienne <https://www.mines-stetienne.fr/en/>`_.

.. toctree::
   :hidden:
   :maxdepth: 1

   gallery.rst
   doc/index.rst
   doc/tutorials.rst
   papers.rst
   applications.rst
   downloads.rst
   whatsnew.rst
   community.rst
